#include "state_task.h"
#include "cmdbus.h"
#include "user_config.h"
#include "poke_task.h"
#include "foc.h"
#include "fault.h"
#include "convert.h"
#include "ntc.h"
#include "ad.h"
#include "tim.h"
#include "gpio.h"
#include "iwdg.h"
#include "port.h"

/* Charge state: three low-side phases pumped in turn. */
#define CHARGE_PHASE_NUM 3u

state_para_t g_state = {
    .main_state     = CMDBUS_INIT,
    .run_mode       = CMDBUS_RUN_MODE_IF_FOC,
    .ctrl_req       = CMDBUS_CTRL_NONE,
    .spd_rpm_ref    = SPEED_REF_DEFAULT,
    .adc_off_ib     = ADC_OFFSET_CALI_DEFAULT,
    .adc_off_ic     = ADC_OFFSET_CALI_DEFAULT,
    .adc_off_idc    = ADC_OFFSET_CALI_DEFAULT,
    .ac_peak_high_v = 220,
    .ac_peak_low_v  = 220,
};

/* Telemetry snapshot (monitoring only). Read across files, so it stays a
 * standalone extern rather than inside the static state block. */
monitor_parameter_t g_monitor_para;

/* Carrier ISR input bundle, file-static to avoid a ~20-byte stack zero-init on
 * every 6 kHz frame. It is touched only by the single carrier ISR context; the
 * sensorless build never reads theta/spd/idq, so just the used fields are set. */
static mc_in_t g_isr_foc_in;

typedef void (*state_fn_t)(void);

/* State machine: each state has an entry (one-shot setup), a state handler
 * (run every poll and call state_switch to leave), and an exit (teardown;
 * NULL if none). */
typedef struct
{
    void (*entry)(void); /* runs once when the state is entered (after exit) */
    void (*run)(void);   /* runs every poll while in the state */
    void (*exit)(void);  /* runs once when leaving the state (NULL = none)    */
} state_ops_t;

static void entry_init(void);
static void run_init(void);
static void entry_idle(void);
static void run_idle(void);
static void entry_cali(void);
static void run_cali(void);
static void exit_cali(void);
static void entry_charge(void);
static void run_charge(void);
static void exit_charge(void);
static void entry_running(void);
static void run_running(void);
static void entry_fault(void);
static void run_fault(void);
static void state_switch(uint8_t next);
/* One ops row per cmdbus_state_t (index == state value); exit NULL when a state
 * needs no teardown. Transitions run exit(old) -> entry(new). */
static const state_ops_t g_state_poll_cb[CMDBUS_STATE_COUNT] = {
    [CMDBUS_INIT]    = {entry_init, run_init, NULL},            // CMDBUS_INIT
    [CMDBUS_IDLE]    = {entry_idle, run_idle, NULL},            // CMDBUS_IDLE
    [CMDBUS_CALI]    = {entry_cali, run_cali, exit_cali},       // CMDBUS_CALI
    [CMDBUS_CHARGE]  = {entry_charge, run_charge, exit_charge}, // CMDBUS_CHARGE
    [CMDBUS_RUNNING] = {entry_running, run_running, NULL},      // CMDBUS_RUNNING
    [CMDBUS_FAULT]   = {entry_fault, run_fault, NULL},          // CMDBUS_FAULT
};

static void cmd_ctrl(const void *payload);
static void cmd_mode(const void *payload);
static void cmd_speed(const void *payload);
static void cmd_power(const void *payload);
static void cmd_duty(const void *payload);
static void cmd_vec(const void *payload);

/* Register one bus handler per command, with its expected payload size. */
static void state_register_cmds(void)
{
    cmdbus_register(CMDBUS_CMD_CTRL, cmd_ctrl, sizeof(cmdbus_ctrl_t));
    cmdbus_register(CMDBUS_CMD_MODE, cmd_mode, sizeof(cmdbus_mode_t));
    cmdbus_register(CMDBUS_CMD_SPEED, cmd_speed, sizeof(cmdbus_speed_t));
    cmdbus_register(CMDBUS_CMD_POWER, cmd_power, sizeof(cmdbus_power_t));
    cmdbus_register(CMDBUS_CMD_DUTY, cmd_duty, sizeof(cmdbus_duty_t));
    cmdbus_register(CMDBUS_CMD_VEC, cmd_vec, sizeof(cmdbus_vec_t));
}

/* run mode (cmdbus_run_mode_t) -> FOC leaf state (hsm ST_*), indexed by mode */
static const state_t g_mode_to_hsm[CMDBUS_RUN_MODE_COUNT] = {
    [CMDBUS_RUN_MODE_NONE]   = ST_IDLE,   // CMDBUS_RUN_MODE_NONE
    [CMDBUS_RUN_MODE_DUTY]   = ST_DUTY,   // CMDBUS_RUN_MODE_DUTY
    [CMDBUS_RUN_MODE_VV]     = ST_VV,     // CMDBUS_RUN_MODE_VV
    [CMDBUS_RUN_MODE_VF]     = ST_VF,     // CMDBUS_RUN_MODE_VF
    [CMDBUS_RUN_MODE_CV]     = ST_CV,     // CMDBUS_RUN_MODE_CV
    [CMDBUS_RUN_MODE_IF]     = ST_IF,     // CMDBUS_RUN_MODE_IF
    [CMDBUS_RUN_MODE_IF_FOC] = ST_IF_FOC, // CMDBUS_RUN_MODE_IF_FOC
};

/* Switch to another state: runs the current state's exit (if any), then the
 * next state's entry. Driven from the state handlers; at most one poll-deep
 * chain (entry_running may fall back to IDLE). */
static void state_switch(uint8_t next)
{
    if (next >= CMDBUS_STATE_COUNT || next == g_state.main_state)
        return;

    const state_ops_t *old = &g_state_poll_cb[g_state.main_state];
    if (old->exit)
        old->exit();

    g_state.main_state = next; /* set before entry so a nested switch sees the current state */

    const state_ops_t *newo = &g_state_poll_cb[next];
    if (newo->entry)
        newo->entry();
}

static void entry_init(void)
{
}

static void goto_user_fault(void)
{
    fault_set(FAULT_ID_USER_SET_ERROR);
    state_switch(CMDBUS_FAULT);
}

static void run_init(void)
{
    state_switch(CMDBUS_IDLE);
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

static void entry_idle(void)
{
    foc_hsm_set(EV_TRAN, EV_FIELD_A, ST_IDLE, 0, 0);
}

static void run_idle(void)
{
    if (fault_get() != 0)
        state_switch(CMDBUS_FAULT);
    else if (g_state.ctrl_req == CMDBUS_CTRL_FAULT)
        goto_user_fault();
    else if (g_state.ctrl_req == CMDBUS_CTRL_START)
        state_switch(CMDBUS_CALI);
    else if (g_state.ctrl_req == CMDBUS_CTRL_RECOVERY)
        fault_latch_clr(); /* clear sticky history while live faults stay handled */
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

static void cali_start(void)
{
    ENTER_CRITICAL_SECTION();
    g_state.cali_active = 1;
    EXIT_CRITICAL_SECTION();
}

static void entry_cali(void)
{
    cali_start();
}

static void exit_cali(void)
{
    ENTER_CRITICAL_SECTION();
    g_state.cali_active = 0;
    EXIT_CRITICAL_SECTION();
}

static bool cali_offset_ok(void)
{
    int32_t delta_offset_ib  = g_state.adc_off_ib - (int32_t)ADC_OFFSET_CALI_DEFAULT;
    int32_t delta_offset_ic  = g_state.adc_off_ic - (int32_t)ADC_OFFSET_CALI_DEFAULT;
    int32_t delta_offset_idc = g_state.adc_off_idc - (int32_t)ADC_OFFSET_CALI_DEFAULT;

#if FAULT_ONE_SHOT_ZERO_OFFSET_ERR_ENABLE
    if (delta_offset_ib < -(int32_t)ADC_OFFSET_CALI_THRESHOLD || delta_offset_ib > (int32_t)ADC_OFFSET_CALI_THRESHOLD ||
        delta_offset_ic < -(int32_t)ADC_OFFSET_CALI_THRESHOLD || delta_offset_ic > (int32_t)ADC_OFFSET_CALI_THRESHOLD ||
        delta_offset_idc < -(int32_t)ADC_OFFSET_CALI_THRESHOLD || delta_offset_idc > (int32_t)ADC_OFFSET_CALI_THRESHOLD)
        return false;
#endif
    return true;
}

static void run_cali(void)
{
    if (fault_get() != 0)
        state_switch(CMDBUS_FAULT); /* exit_cali runs via state_switch */
    else if (g_state.ctrl_req == CMDBUS_CTRL_FAULT)
        goto_user_fault();
    else if (g_state.ctrl_req == CMDBUS_CTRL_STOP)
        state_switch(CMDBUS_IDLE);
    else if (g_state.ctrl_req == CMDBUS_CTRL_RECOVERY)
        fault_latch_clr();
    else if (!g_state.cali_active)
    {
        if (cali_offset_ok())
        {
            state_switch(CMDBUS_CHARGE);
        }
        else
        {
            fault_set(FAULT_ID_ZERO_OFFSET_ERROR);
            state_switch(CMDBUS_FAULT);
        }
    }
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

/* ---- CHARGE: pump each bootstrap cap, one phase (low-side) at a time,
 * A -> B -> C for CHARGE_CARRIER_CYCLES each ---- */
static void entry_charge(void)
{
    ENTER_CRITICAL_SECTION();
    tim_pwm_update_ccr(0, 0, 0);
    tim_pwm_charge_phase(0);
    tim_pwm_enable();
    g_state.charge_active = 1;
    EXIT_CRITICAL_SECTION();
}

static void exit_charge(void)
{
    ENTER_CRITICAL_SECTION();
    g_state.charge_active = 0;
    tim_pwm_disable();
    tim_pwm_restore();
    EXIT_CRITICAL_SECTION();
}

static void run_charge(void)
{
    if (fault_get() != 0)
        state_switch(CMDBUS_FAULT);
    else if (g_state.ctrl_req == CMDBUS_CTRL_FAULT)
        goto_user_fault();
    else if (g_state.ctrl_req == CMDBUS_CTRL_STOP)
        state_switch(CMDBUS_IDLE);
    else if (g_state.ctrl_req == CMDBUS_CTRL_RECOVERY)
        fault_latch_clr();
    else if (!g_state.charge_active)
        state_switch(CMDBUS_RUNNING);
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

static void entry_running(void)
{
    uint8_t mode = g_state.run_mode;
    if (mode >= CMDBUS_RUN_MODE_DUTY && mode <= CMDBUS_RUN_MODE_IF_FOC)
    {
        foc_hsm_set(EV_TRAN, EV_FIELD_A, g_mode_to_hsm[mode], 0, 0);
        foc_hsm_set(EV_SET_SPD, EV_FIELD_A, rpm_to_pu(g_state.spd_rpm_ref), 0, 0);
    }
    else
    {
        /* no valid mode armed: nothing to run, fall back to IDLE */
        state_switch(CMDBUS_IDLE);
    }
}

static void run_running(void)
{
    if (fault_get() != 0)
        state_switch(CMDBUS_FAULT);
    else if (g_state.ctrl_req == CMDBUS_CTRL_FAULT)
        goto_user_fault();
    else if (g_state.ctrl_req == CMDBUS_CTRL_STOP)
        state_switch(CMDBUS_IDLE);
    else if (g_state.ctrl_req == CMDBUS_CTRL_RECOVERY)
        fault_latch_clr();
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

static void entry_fault(void)
{
    hsm_pwm_disable(&g_hsm);
    foc_hsm_set(EV_FAULT, 0, 0, 0, 0);
}

static void run_fault(void)
{
    if (g_state.ctrl_req == CMDBUS_CTRL_RECOVERY)
    {
        fault_clr_all();
        state_switch(CMDBUS_IDLE);
    }
    else if (fault_get() == 0)
    {
        state_switch(CMDBUS_IDLE);
    }
    g_state.ctrl_req = CMDBUS_CTRL_NONE;
}

static angle_t vec_bam(uint16_t ang_deg)
{
    angle_t off = deg_to_bam((int16_t)ang_deg);

    return (g_state.run_mode == CMDBUS_RUN_MODE_CV) ? off : (angle_t)(off - 16384);
}

static void cmd_ctrl(const void *payload)
{
    /* g_state.ctrl_req is always consumed (cleared) at the end of the previous
     * run_*, so a freshly latched CTRL cannot be overwritten. */
    const cmdbus_ctrl_t *p = (const cmdbus_ctrl_t *)payload;

    uint8_t v = p->ctrl_type;

    if (v <= CMDBUS_CTRL_RECOVERY)
        g_state.ctrl_req = v;
}

static void cmd_mode(const void *payload)
{
    /* remembered for the next START; only a known mode is accepted */
    const cmdbus_mode_t *p = (const cmdbus_mode_t *)payload;
    if (p->run_mode <= CMDBUS_RUN_MODE_IF_FOC)
        g_state.run_mode = p->run_mode;
}

static void cmd_speed(const void *payload)
{
    const cmdbus_speed_t *p   = (const cmdbus_speed_t *)payload;
    int16_t               rpm = p->speed_rpm;

    if (rpm > SPEED_RATE_RPM)
        rpm = (int16_t)SPEED_RATE_RPM;
    else if (rpm < -SPEED_RATE_RPM)
        rpm = (int16_t)-SPEED_RATE_RPM;

    foc_hsm_set(EV_SET_SPD, EV_FIELD_A, rpm_to_pu(rpm), 0, 0);
    g_state.spd_rpm_ref = rpm;
}

static void cmd_duty(const void *payload)
{
    if (g_state.main_state == CMDBUS_RUNNING)
    {
        const cmdbus_duty_t *p = (const cmdbus_duty_t *)payload;
        foc_hsm_set(EV_SET_DUTY, EV_FIELD_A | EV_FIELD_B | EV_FIELD_C, percent_to_q15(p->duty_a), percent_to_q15(p->duty_b),
                    percent_to_q15(p->duty_c));
    }
}

static void cmd_vec(const void *payload)
{
    if (g_state.main_state == CMDBUS_RUNNING)
    {
        const cmdbus_vec_t *p = (const cmdbus_vec_t *)payload;

        /* clamp the vector magnitude by mode: VV voltage-vector max 5%, CV current-vector max 30% */
        uint16_t mag_max;
        uint16_t mag = p->mag_pct;

        if (g_state.run_mode == CMDBUS_RUN_MODE_VV)
            mag_max = VV_MAG_PCT_MAX;
        else if (g_state.run_mode == CMDBUS_RUN_MODE_CV)
            mag_max = CV_MAG_PCT_MAX;
        else
            mag_max = 100u;

        if (mag > mag_max)
            mag = mag_max;

        foc_hsm_set(EV_SET_VEC, EV_FIELD_A | EV_FIELD_B, percent_to_q15((int16_t)mag), (int16_t)vec_bam(p->ang_deg), 0);
    }
}

static void cmd_power(const void *payload)
{
    /* power W, consumed by the application layer (no FOC event) */
    const cmdbus_power_t *p = (const cmdbus_power_t *)payload;
    g_state.pwr_watt_ref    = p->power_w;
}

/* ------------------------------------------------------------------ */
/* LED + power-delay pin helpers (1 ms)                                */
/* ------------------------------------------------------------------ */
static void led_1ms_proc(void)
{
    uint32_t half_period_ms = (fault_get() != 0) ? 100u : 500u; /* 5 Hz fault / 1 Hz ok */
    uint32_t on             = ((systick_get() / half_period_ms) & 1u) == 0u;
    gpio_led_set(on);
}

static void dc_delay_1ms_proc(void)
{
    static uint8_t  pin_high = 0;
    static uint16_t hold_cnt = 0;

    /* pin control only, no UV alarm; g_state.udc_mv comes from the carrier ISR */
    uint32_t on_thresh  = DC_DELAY_PIN_ON_THRESH_MV;
    uint32_t off_thresh = DC_DELAY_PIN_ON_THRESH_MV - DC_DELAY_PIN_HYST_MV;

    if (pin_high == 0)
    {
        if (g_state.udc_mv >= on_thresh)
        {
            if (++hold_cnt >= DC_DELAY_PIN_HOLD_MS)
            {
                gpio_power_delay_set(1);
                pin_high = 1;
                hold_cnt = 0;
            }
        }
        else
            hold_cnt = 0;
    }
    else
    {
        if (g_state.udc_mv <= off_thresh)
        {
            if (++hold_cnt >= DC_DELAY_PIN_HOLD_MS)
            {
                gpio_power_delay_set(0);
                pin_high = 0;
                hold_cnt = 0;
            }
        }
        else
            hold_cnt = 0;
    }
}

static void ac_peak_1ms_proc(void)
{
    static uint16_t cnt  = 0;
    static uint32_t hi_v = 0;
    static uint32_t lo_v = 0xFFFFFFFFu;

    uint32_t ac_v = ac_code_to_v((int32_t)adc_get_code(ADC_SEQ1_AC_PEAK));
    if (ac_v > hi_v)
        hi_v = ac_v;
    if (ac_v < lo_v)
        lo_v = ac_v;

    if (++cnt >= AC_PEAK_WIN_MS)
    {
        g_state.ac_peak_high_v = (uint16_t)hi_v;
        g_state.ac_peak_low_v  = (uint16_t)lo_v;

        cnt  = 0;
        hi_v = 0;
        lo_v = 0xFFFFFFFFu;
    }
}

/* ------------------------------------------------------------------ */
/* Public task API                                                     */
/* ------------------------------------------------------------------ */
void state_task_init(void)
{
    state_register_cmds();
    cmdbus_reset();
    foc_init();
    fault_init();
}

void state_task_poll(void)
{
    /* Consume at most ONE queued command (dispatches to the registered
     * handler), then run the state handler so each command gets a full machine
     * step before the next applies (g_state.ctrl_req is never overwritten). */
    cmdbus_dispatch();

    uint8_t st = g_state.main_state;
    if (st < CMDBUS_STATE_COUNT && g_state_poll_cb[st].run)
        g_state_poll_cb[st].run();

    /* background engine processing */
    foc_poll_proc(g_state.main_state);
}

/* 1 ms tick: telemetry + slow loops + housekeeping. */
void state_task_1ms(void)
{
    int16_t temperature, spd_rpm_fb, pwr_watt_fb;
    /* DC bus V/I telemetry + protection feed (reconstruct idc from last duties). */
    g_state.udc_mv = udc_pu_to_mv(g_state.udc_meas);
    g_state.idc_ma = idc_pu_to_ma(g_state.idc_meas);
    temperature    = ntc_temp_c((uint16_t)adc_get_code(ADC_SEQ1_NTC));
    spd_rpm_fb     = pu_to_rpm(g_mc.act_spd_fb);
    pwr_watt_fb    = udc_idc_to_pwr_x10(g_state.udc_mv, g_state.idc_ma);

    ac_peak_1ms_proc();
    dc_delay_1ms_proc();
    led_1ms_proc();
    foc_1ms_proc();
    fault_1ms_proc();

    /* monitoring snapshot only -- values may lag their source by up to 1 ms */
    g_monitor_para.state          = g_state.main_state;
    g_monitor_para.run_mode       = g_state.run_mode;
    g_monitor_para.fault_curr     = fault_get();
    g_monitor_para.fault_latch    = fault_get_latch();
    g_monitor_para.ac_peak_high_v = g_state.ac_peak_high_v;
    g_monitor_para.ac_peak_low_v  = g_state.ac_peak_low_v;
    g_monitor_para.udc_mv         = g_state.udc_mv;
    g_monitor_para.idc_ma         = g_state.idc_ma;
    g_monitor_para.temperature    = temperature;
    g_monitor_para.spd_rpm_ref    = g_state.spd_rpm_ref;
    g_monitor_para.spd_rpm_fb     = spd_rpm_fb;
    g_monitor_para.pwr_watt_ref   = g_state.pwr_watt_ref;
    g_monitor_para.pwr_watt_fb    = pwr_watt_fb;
}

/* Hardware break (TIM1): DC-bus over current latched by the break input. */
void state_task_isr_break(void)
{
    tim_pwm_disable();
    fault_set(FAULT_ID_HW_IDC_OVER_CURRENT);
}

void state_task_isr()
{
    switch (g_state.main_state)
    {
        case CMDBUS_RUNNING:
        {
            g_isr_foc_in.iabc_meas.a = g_state.ia_meas;
            g_isr_foc_in.iabc_meas.b = g_state.ib_meas;
            g_isr_foc_in.iabc_meas.c = g_state.ic_meas;
            g_isr_foc_in.udc_meas    = g_state.udc_meas;
            fault_isr_proc(g_state.ia_meas, g_state.ib_meas, g_state.ic_meas);
            foc_isr_proc(&g_isr_foc_in, &g_state.duties_q15);
            tim_pwm_update_ccr(duty_to_ccr_arr(g_state.duties_q15.a), duty_to_ccr_arr(g_state.duties_q15.b), duty_to_ccr_arr(g_state.duties_q15.c));
            break;
        }
        case CMDBUS_CHARGE:
        {
            if (g_state.charge_active)
            {
                if (++g_state.charge_cnt >= CHARGE_CARRIER_CYCLES)
                {
                    g_state.charge_cnt = 0;
                    if (++g_state.charge_phase >= CHARGE_PHASE_NUM)
                    {
                        g_state.charge_active = 0;
                        g_state.charge_phase  = 0;
                    }
                    else
                    {
                        tim_pwm_charge_phase(g_state.charge_phase);
                    }
                }
            }
            break;
        }
        case CMDBUS_CALI:
        {
            if (g_state.cali_active)
            {
                g_state.cali_sum_ib += (int32_t)adc_get_code(ADC_SEQ1_I_B);
                g_state.cali_sum_ic += (int32_t)adc_get_code(ADC_SEQ1_I_C);
                g_state.cali_sum_idc += (int32_t)adc_get_code(ADC_SEQ1_I_DC);

                if (++g_state.cali_cnt >= ADC_OFFSET_CALI_SAMPLE_CNT)
                {
                    g_state.adc_off_ib   = g_state.cali_sum_ib / ADC_OFFSET_CALI_SAMPLE_CNT;
                    g_state.adc_off_ic   = g_state.cali_sum_ic / ADC_OFFSET_CALI_SAMPLE_CNT;
                    g_state.adc_off_idc  = g_state.cali_sum_idc / ADC_OFFSET_CALI_SAMPLE_CNT;
                    g_state.cali_active  = 0;
                    g_state.cali_cnt     = 0;
                    g_state.cali_sum_ib  = 0;
                    g_state.cali_sum_ic  = 0;
                    g_state.cali_sum_idc = 0;
                }
            }
            break;
        }
        default:
            break;
    }
}
