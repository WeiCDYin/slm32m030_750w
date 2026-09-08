#ifndef __FAULT_H__
#define __FAULT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "types.h"

/* Fault framework + application task, merged in the Modules layer.
 *
 * Every fault id belongs to exactly ONE class, annotated on the enum below:
 *
 *   [poll]     A threshold channel. fault_init REGISTERS it (fault_register)
 *              with a limit window + debounce counts from main.h, and
 *              fault_1ms_proc feeds it a sample every 1 ms (fault_poll). It
 *              trips after *_DETECT_CNT violating samples and AUTO-CLEARS
 *              after *_RECOVER_CNT in-range ones. Needs
 *              *_ENABLE / *_LIMIT / *_DETECT_CNT / *_RECOVER_CNT in main.h.
 *
 *   [one shot] An event, not a threshold. NOT registered and never polled: the
 *              detector calls fault_set(id) at the instant it happens (the
 *              carrier ISR phase over-current trip, a flash/parameter check),
 *              and the bit stays LATCHED until the operator acknowledges over
 *              modbus (CTRL=3 Recovery -> fault_clr_all). No main.h window
 *              macros -- only a trip level where the detector needs one.
 *
 *   [no used]  Not implemented. No macros, never registered, never set.
 *
 * FAULT_DETECT_ENABLE (main.h) gates the POLL channels only; a one-shot trip
 * always latches, because the hardware event already happened.
 *
 * Both classes land in the same packed fault code: bit (1u << id) in
 * fault_get(), plus a sticky copy in fault_get_latch(). */
/* ------------------------------------------------------------------ */
/* Fault id -> fault-code bit assignment.                             */
/* fault_code bit (1u << id) is set once the fault trips.             */
/* ------------------------------------------------------------------ */
enum
{
    // [one shot], bit0~bit7
    FAULT_ID_HW_IDC_OVER_CURRENT    = 0, // 0x00000001, [one shot] hardware dc over current
    FAULT_ID_SW_IDC_OVER_CURRENT    = 1, // 0x00000002, [one shot] software dc over current
    FAULT_ID_SW_IPHASE_OVER_CURRENT = 2, // 0x00000004, [one shot] software phase-abc over current
    FAULT_ID_MOTOR_LOST_PHASE       = 3, // 0x00000008, [one shot] motor lost phase
    FAULT_ID_ZERO_OFFSET_ERROR      = 4, // 0x00000010, [one shot] phase offset error
    FAULT_ID_USER_SET_ERROR         = 5, // 0x00000020, [one shot] user setting error state by FAULT cmd
    FAULT_ID_FOC_STARTUP_ERROR      = 6, // 0x00000040, [one shot] foc startup error

    // [poll], bit8~bit31
    FAULT_ID_DC_OVER_VOLTAGE  = 8,  // 0x00000100, [poll] dc over voltage
    FAULT_ID_DC_UNDER_VOLTAGE = 9,  // 0x00000200, [poll] dc under voltage
    FAULT_ID_OVER_LOADING     = 10, // 0x00000400, [poll] motor over load
    FAULT_ID_OVER_SPEED       = 11, // 0x00000800, [poll] motor over speed
    FAULT_ID_OVER_TEMPERATURE = 12, // 0x00001000, [poll] over temperature(inversion)
    FAULT_ID_AC_OVER_VOLTAGE  = 13, // 0x00002000, [poll] ac over voltage
    FAULT_ID_AC_UNDER_VOLTAGE = 14, // 0x00004000, [poll] ac under voltage
    FAULT_ID_AC_LOST_PHASE    = 15, // 0x00008000, [poll] ac lost phase
    FAULT_ID_SHORT_CIRCUIT    = 16, // 0x00010000, [no used] output short circuit
    FAULT_ID_MOTOR_STALL      = 17, // 0x00020000, [no used] motor stall

    FAULT_ID_MAX, // must stay <= 32: fault code is uint32_t
};

/* ---- framework API ---- */
/* [one shot] channels: latch the event now. ISR-safe. */
void fault_set(uint16_t id);

/* Carrier-ISR software protection: phase currents and DC bus current.
 * Disables PWM immediately and latches the fault on trip.
 * Returns true if a fault tripped (caller should skip the rest of the ISR). */
void fault_isr_proc(q15_t ia, q15_t ib, q15_t ic);

/* ---- application task API ---- */
void fault_init(void);
void fault_clr_all(void);
void fault_latch_clr(void);
void fault_1ms_proc(void);

uint32_t fault_get(void);
uint32_t fault_get_latch(void);

#ifdef __cplusplus
}
#endif
#endif
