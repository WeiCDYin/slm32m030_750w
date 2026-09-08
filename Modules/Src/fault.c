#include "fault.h"
#include "user_config.h"
#include "ad.h"
#include "convert.h"
#include "ntc.h"
#include "tim.h"

/* Interrupt-free critical section (Cortex-M0+), compiler-local: fault.c is not
 * part of FreeModbus, so it uses the same cpsid/cpsie pair directly instead of
 * pulling in port.h (which also defines the FreeModbus BOOL/TRUE/FALSE). */
#define ENTER_CRITICAL_SECTION() __asm volatile("cpsid i" ::: "memory")
#define EXIT_CRITICAL_SECTION()  __asm volatile("cpsie i" ::: "memory")

/* Limit enable mask for a [poll] channel's window edges, OR-ed into
 * fault_id_t.fault_msk at register time. */
enum
{
    FAULT_MASK_UPPER = 0x01, /* trip when value > upper_limit */
    FAULT_MASK_LOWER = 0x02, /* trip when value < lower_limit */
    FAULT_MASK_ALL   = 0x03,
};

/* One fault channel: configuration + debounce state. Only [poll] ids own one;
 * a [one shot] id leaves fault_msk == 0 forever and is driven by fault_set. */
typedef struct
{
    uint16_t          fault_msk;               /* FAULT_MASK_* combo; 0 = channel not registered */
    uint16_t          fault_detect_threshold;  /* detect time in poll cycles; cnt >= this trips   */
    uint16_t          fault_recover_threshold; /* recover time; cnt >= this clears the fault  */
    int32_t           fault_upper_limit;       /* trip when value > upper  (ignored unless UPPER) */
    int32_t           fault_lower_limit;       /* trip when value < lower  (ignored unless LOWER) */
    volatile uint16_t fault_detect_cnt;        /* consecutive violating polls (detect debounce)   */
    volatile uint16_t fault_recover_cnt;       /* consecutive in-range polls (recovery debounce)  */
} fault_id_t;

typedef struct
{
    volatile uint32_t fault_curr;  /* packed fault code: bit id == (1u << id) */
    volatile uint32_t fault_latch; /* sticky history: every fault id that ever tripped, OR-ed at the
                                    * trip instant (fault_set / fault_poll). Cleared only by
                                    * fault_latch_clr() (modbus Recovery); never by auto-recovery. */
    fault_id_t fault_id[FAULT_ID_MAX];
} fault_t;

static fault_t g_fault;

/* ------------------------------------------------------------------ */
/* ISR <-> thread critical section                                     */
/* ------------------------------------------------------------------ */
/* fault_curr and fault_latch are READ-MODIFY-WRITTEN from TWO contexts: the
 * ISRs (fault_set -- TIM1_BRK hardware over-current and the carrier-ISR phase
 * over-current trip) and the thread (fault_poll on the 1 ms tick,
 * fault_clr_all from the main loop).
 *
 * Cortex-M0+ (ARMv6-M) has NO exclusive load/store, so `fault_curr |= bit`
 * compiles to LDR/ORR/STR. An ISR trip landing between the thread's LDR and
 * STR is written back OVER and lost outright -- and that is not a cosmetic
 * loss: the ISR has already opened the bridge, but the main machine polls
 * fault_get() and would see 0, never enter FAULT, and a FAULT state could
 * auto-recover to IDLE on the stale zero and re-arm the PWM, clearing the
 * TIM1_BRK latch on the way.
 *
 * The sections use the port's ENTER/EXIT_CRITICAL_SECTION (port.h) and cover
 * ONLY the fault_curr / fault_latch updates -- a few instructions, so the
 * carrier ISR's WCET is unaffected.
 *
 * NOT guarded, deliberately:
 *   - fault_get() / fault_get_latch(): a 32-bit aligned load is a single LDR
 *     on ARMv6-M, so a reader cannot tear.
 *   - the per-channel debounce counters: each belongs to ONE channel, and a
 *     channel is either polled by the thread or set by an ISR, never both.
 *   - fault_reset() / fault_register(): init-time only (fault_init), before
 *     any ISR that could trip is armed. */

/* A sample violates the channel's window if it is beyond ANY enabled limit. */
static bool fault_is_over(const fault_id_t *e, int32_t value)
{
    if ((e->fault_msk & FAULT_MASK_UPPER) && (value > e->fault_upper_limit))
        return true;
    if ((e->fault_msk & FAULT_MASK_LOWER) && (value < e->fault_lower_limit))
        return true;
    return false;
}

/* [poll] channels only: feed one sample and run the detect/recover debounce.
 * File-local -- every caller is a fault_1ms_proc poll below, and a [one shot]
 * id must never reach it. A channel that was never registered (fault_msk == 0)
 * is inert, which is what keeps the one-shot ids structurally out. */
static uint32_t fault_poll(uint16_t id, int32_t value)
{
    fault_t *   f = &g_fault;
    fault_id_t *e;

    e = &f->fault_id[id];
    if (e->fault_msk == 0) /* not registered: leave everything alone */
        return f->fault_curr;

    /* The debounce counters are this channel's alone (no ISR touches a polled
     * channel), so only the shared bit fields are sectioned -- and only on the
     * ticks that actually trip or clear, not on every poll. */
    if (fault_is_over(e, value))
    {
        /* Violating sample: reset the recovery progress, keep counting. */
        e->fault_recover_cnt = 0;
        if (e->fault_detect_cnt < e->fault_detect_threshold)
            e->fault_detect_cnt++;
        if (e->fault_detect_cnt >= e->fault_detect_threshold)
        {
            __disable_irq();
            f->fault_curr |= (uint32_t)1u << id;
            f->fault_latch |= (uint32_t)1u << id; /* sticky history: record the trip */
            __enable_irq();
        }
    }
    else if (f->fault_curr & ((uint32_t)1u << id))
    {
        /* Fault latched: require consecutive in-range samples before it
         * clears (recovery hysteresis), so a marginal line does not flap. */
        if (e->fault_recover_cnt < e->fault_recover_threshold)
            e->fault_recover_cnt++;
        if (e->fault_recover_cnt >= e->fault_recover_threshold)
        {
            __disable_irq();
            f->fault_curr &= ~((uint32_t)1u << id);
            __enable_irq();
            e->fault_detect_cnt = 0;
        }
    }
    else
    {
        e->fault_detect_cnt = 0;
    }
    return f->fault_curr;
}

/* [poll] channels only, init-time (fault_init) -- before any tripping ISR is
 * armed, so no critical section. File-local: fault_init is the only caller. */
static void fault_register(uint16_t id, uint16_t msk, int32_t upper_limit, int32_t lower_limit, uint16_t detect_cnt, uint16_t recover_cnt)
{
    fault_t *   f = &g_fault;
    fault_id_t *e = &f->fault_id[id];

    e->fault_msk               = msk;
    e->fault_upper_limit       = upper_limit;
    e->fault_lower_limit       = lower_limit;
    e->fault_detect_cnt        = 0;
    e->fault_detect_threshold  = detect_cnt;
    e->fault_recover_cnt       = 0;
    e->fault_recover_threshold = recover_cnt;
}

/* Init-time only (fault_init), before any tripping ISR is armed -- no section. */
static void fault_reset(void)
{
    fault_t *f = &g_fault;

    f->fault_curr  = 0;
    f->fault_latch = 0; /* power-on / re-init: no fault history */
    for (uint16_t i = 0; i < FAULT_ID_MAX; i++)
    {
        f->fault_id[i].fault_msk               = 0; /* 0 = channel not registered */
        f->fault_id[i].fault_detect_cnt        = 0;
        f->fault_id[i].fault_recover_cnt       = 0;
        f->fault_id[i].fault_upper_limit       = 0;
        f->fault_id[i].fault_lower_limit       = 0;
        f->fault_id[i].fault_detect_threshold  = 0;
        f->fault_id[i].fault_recover_threshold = 0;
    }
}

/* ------------------------------------------------------------------ */
/* Application task API                                               */
/* ------------------------------------------------------------------ */

/* Register every [poll] channel (bit8~bit31) with its main.h window + debounce.
 * The [one shot] ids (bit0~bit7: DC_OVER_CURRENT from TIM1_BRK,
 * SOFTWARE_OVER_CURRENT from the carrier ISR, INIT_PARAMETER_ERROR) are
 * deliberately absent: they have no window, are never polled, and reach the
 * fault table only through fault_set(). [no used] ids are absent for the same
 * structural reason. */
void fault_init(void)
{
    fault_reset();

#if FAULT_DETECT_ENABLE
#if DC_IN_OVER_VOLTAGE_ENABLE
    fault_register(FAULT_ID_DC_OVER_VOLTAGE, FAULT_MASK_UPPER, DC_IN_OVER_VOLTAGE_LIMIT, 0, DC_IN_OVER_VOLTAGE_DETECT_CNT,
                   DC_IN_OVER_VOLTAGE_RECOVER_CNT);
#endif
#if DC_IN_UNDER_VOLTAGE_ENABLE
    fault_register(FAULT_ID_DC_UNDER_VOLTAGE, FAULT_MASK_LOWER, 0, DC_IN_UNDER_VOLTAGE_LIMIT, DC_IN_UNDER_VOLTAGE_DETECT_CNT,
                   DC_IN_UNDER_VOLTAGE_RECOVER_CNT);
#endif
#if POWER_OVER_LOAD_ENABLE
    /* over loading(power) */
    fault_register(FAULT_ID_OVER_LOADING, FAULT_MASK_UPPER, POWER_OVER_LOAD_LIMIT, 0, POWER_OVER_LOAD_DETECT_CNT, POWER_OVER_LOAD_RECOVER_CNT);
#endif
#if MOTOR_OVER_SPEED_ENABLE
    /* over speed */
    fault_register(FAULT_ID_OVER_SPEED, FAULT_MASK_UPPER, MOTOR_OVER_SPEED_LIMIT, 0, MOTOR_OVER_SPEED_DETECT_CNT, MOTOR_OVER_SPEED_RECOVER_CNT);
#endif
#if TEMPERATURE_OVER_ENABLE
    /* over temperature */
    fault_register(FAULT_ID_OVER_TEMPERATURE, FAULT_MASK_UPPER, TEMPERATURE_OVER_LIMIT, 0, TEMPERATURE_OVER_DETECT_CNT, TEMPERATURE_OVER_RECOVER_CNT);
#endif
#if AC_IN_OVER_VOLTAGE_ENABLE
    fault_register(FAULT_ID_AC_OVER_VOLTAGE, FAULT_MASK_UPPER, AC_IN_OVER_VOLTAGE_LIMIT, 0, AC_IN_OVER_VOLTAGE_DETECT_CNT,
                   AC_IN_OVER_VOLTAGE_RECOVER_CNT);
#endif
#if AC_IN_UNDER_VOLTAGE_ENABLE
    fault_register(FAULT_ID_AC_UNDER_VOLTAGE, FAULT_MASK_LOWER, 0, AC_IN_UNDER_VOLTAGE_LIMIT, AC_IN_UNDER_VOLTAGE_DETECT_CNT,
                   AC_IN_UNDER_VOLTAGE_RECOVER_CNT);
#endif
#if AC_IN_LOST_PHASE_ENABLE
    /* ac lost phase: LOWER edge on the rectified common-point LOW peak (valley);
     * it collapses when a phase drops (see main.h id 15) */
    fault_register(FAULT_ID_AC_LOST_PHASE, FAULT_MASK_LOWER, 0, AC_IN_LOST_PHASE_LIMIT, AC_IN_LOST_PHASE_DETECT_CNT, AC_IN_LOST_PHASE_RECOVER_CNT);
#endif
#endif
}

void fault_1ms_proc(void)
{
#if FAULT_DETECT_ENABLE

#if AC_IN_OVER_VOLTAGE_ENABLE
    fault_poll(FAULT_ID_AC_OVER_VOLTAGE, (int32_t)g_monitor_para.ac_peak_high_v);
#endif
#if AC_IN_UNDER_VOLTAGE_ENABLE
    fault_poll(FAULT_ID_AC_UNDER_VOLTAGE, (int32_t)g_monitor_para.ac_peak_high_v);
#endif
#if AC_IN_LOST_PHASE_ENABLE
    fault_poll(FAULT_ID_AC_LOST_PHASE, (int32_t)g_monitor_para.ac_peak_low_v);
#endif
#if DC_IN_OVER_VOLTAGE_ENABLE
    fault_poll(FAULT_ID_DC_OVER_VOLTAGE, (int32_t)g_monitor_para.udc_mv);
#endif
#if DC_IN_UNDER_VOLTAGE_ENABLE
    fault_poll(FAULT_ID_DC_UNDER_VOLTAGE, (int32_t)g_monitor_para.udc_mv);
#endif
#if POWER_OVER_LOAD_ENABLE
    fault_poll(FAULT_ID_OVER_LOADING, g_monitor_para.pwr_watt_fb);
#endif
#if TEMPERATURE_OVER_ENABLE
    fault_poll(FAULT_ID_OVER_TEMPERATURE, g_monitor_para.temperature);
#endif
#if MOTOR_OVER_SPEED_ENABLE
    fault_poll(FAULT_ID_OVER_SPEED, g_monitor_para.spd_rpm_fb);
#endif
#endif
}

void fault_isr_proc(q15_t ia, q15_t ib, q15_t ic)
{
    static uint8_t phase_oc_cnt = 0;

    // phase current over protection
    if ((int32_t)ia >= (int32_t)SW_PHASE_OC_TRIP_PU || (int32_t)ia <= -(int32_t)SW_PHASE_OC_TRIP_PU || (int32_t)ib >= (int32_t)SW_PHASE_OC_TRIP_PU ||
        (int32_t)ib <= -(int32_t)SW_PHASE_OC_TRIP_PU || (int32_t)ic >= (int32_t)SW_PHASE_OC_TRIP_PU || (int32_t)ic <= -(int32_t)SW_PHASE_OC_TRIP_PU)
    {
        phase_oc_cnt++;
        if (phase_oc_cnt >= 3)
        {
            fault_set(FAULT_ID_SW_IPHASE_OVER_CURRENT);
            tim_pwm_disable();
            phase_oc_cnt = 0;
            return;
        }
    }
    else
    {
        phase_oc_cnt = 0;
    }
}

void fault_set(uint16_t id)
{
    fault_t *f = &g_fault;

    if (!f || id >= FAULT_ID_MAX)
        return;
    /* A fresh trip restarts both debounce windows: the recovery count must
     * not carry over from an earlier cycle, or a latched fault (e.g. the
     * TIM1_BRK hardware latch) could clear after a single in-range sample. */
    f->fault_id[id].fault_detect_cnt  = 0;
    f->fault_id[id].fault_recover_cnt = 0;
    /* Only the shared bit fields need the section (see the note above). */
    __disable_irq();
    f->fault_curr |= (uint32_t)1u << id;
    f->fault_latch |= (uint32_t)1u << id; /* sticky history: record the trip */
    __enable_irq();
}

uint32_t fault_get(void)
{
    return g_fault.fault_curr;
}

uint32_t fault_get_latch(void)
{
    return g_fault.fault_latch;
}

void fault_clr_all(void)
{
    fault_t *f = &g_fault;

    __disable_irq();
    f->fault_curr  = 0;
    f->fault_latch = 0; /* operator acknowledge also drops the sticky history */
    __enable_irq();
    for (uint16_t i = 0; i < FAULT_ID_MAX; i++)
    {
        f->fault_id[i].fault_detect_cnt  = 0;
        f->fault_id[i].fault_recover_cnt = 0;
    }
}

void fault_latch_clr(void)
{
    fault_t *f = &g_fault;

    __disable_irq();
    f->fault_latch = 0;
    __enable_irq();
}
