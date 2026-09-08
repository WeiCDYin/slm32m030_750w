#ifndef __STATE_TASK_H__
#define __STATE_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>     /* NULL */
#include "mc.h"         /* mc_in_t, duties_t */
#include "user_config.h" /* monitor_parameter_t */

typedef struct {
    q15_t ia_meas;
    q15_t ib_meas;
    q15_t ic_meas;
    q15_t idc_meas;
    q15_t udc_meas;
} state_meas_t;

/* All state-task runtime state is packed into one static instance. The type is
 * state_para_t (not state_t, which is the HSM leaf-state enum from foc.h). */
typedef struct
{
    uint8_t  main_state;
    uint8_t  ctrl_req;
    uint8_t  run_mode;
    int16_t  spd_rpm_ref;
    uint16_t pwr_watt_ref;

    /* hw measure value */
    volatile q15_t ia_meas;
    volatile q15_t ib_meas;
    volatile q15_t ic_meas;
    volatile q15_t idc_meas;
    volatile q15_t udc_meas;

    /* DC bus V/I */
    volatile uint32_t udc_mv;
    volatile uint16_t idc_ma;

    /* Phase-current zero offsets; default mid-scale so the ISR sees ~zero current
     * from boot; CMDBUS_CALI re-measures on every START. Exposed to the ISR via
     * the state_task_adc_off_*() accessors (it.c lives outside this file). */
    volatile int32_t adc_off_ib;
    volatile int32_t adc_off_ic;
    volatile int32_t adc_off_idc;

    /* AC peak window (10 x 50 Hz cycles), refreshed by the 1 ms task. */
    uint16_t ac_peak_high_v;
    uint16_t ac_peak_low_v;

    /* Last FOC duties (pu), carried between carrier frames for idc reconstruction. */
    duties_t duties_q15;

    /* Charge state (sequential low-side bootstrap A -> B -> C). */
    volatile uint8_t charge_active;
    volatile uint8_t charge_phase; /* 0=A, 1=B, 2=C during sequential charge */
    uint16_t         charge_cnt;

    /* Phase-current offset calibration (CMDBUS_CALI): the carrier ISR accumulates
     * raw I_B / I_C zero-current codes while cali_active==1; after CALI_SAMPLE_COUNT
     * frames it publishes the averaged offsets into adc_off_ib/ic and clears
     * cali_active. */
    volatile uint8_t  cali_active;
    volatile int32_t  cali_sum_ib;
    volatile int32_t  cali_sum_ic;
    volatile int32_t  cali_sum_idc;
    volatile uint16_t cali_cnt;
} state_para_t;

/* state_task -- the top application task. It owns the main state machine
 * (INIT/IDLE/CALI/CHARGE/RUNNING/FAULT, held on the command bus) and is the
 * ONE place that drives the control engine: it orchestrates foc, poke, fault
 * and ntc. The other two tasks (modbus_task, poke_task) and the ISR never call
 * foc/fault/ntc directly -- they only post commands on cmdbus, which this task
 * consumes.
 *
 *   state_task_init()  power-on: tune FOC, init fault table + ntc, register
 *                      command handlers on the bus, reset machine state.
 *   state_task_poll()  main loop: drain one bus command, run FOC hsm + poke,
 *                      advance the state machine.
 *   state_task_1ms()   1 ms tick: FOC slow loop, fault poll, poke resync,
 *                      ntc temperature, telemetry snapshot.
 *   state_task_isr()   carrier ISR: offset calibration / bootstrap while not
 *                      running; while RUNNING, software OC check, poke force,
 *                      FOC fast step; writes the new PWM duties into *dabc_ccr.
 *   state_task_isr_break()  TIM1 hardware break (DC over current).
 *
 * Telemetry: state_task owns the monitor snapshot g_monitor_para; the 1 ms
 * task refreshes it and the ISR refreshes the fast V/I fields. */

void state_task_init(void);
void state_task_poll(void);
void state_task_1ms(void);

/* Carrier frame. mc_in is already filled with the measured phase currents /
 * dc voltage (pu) by the ISR. When the machine is RUNNING this writes the new
 * PWM CCR ticks into ccr[0..2] and returns true; otherwise it returns false
 * (cali / charge timing handled internally, no CCR update). */
bool state_task_isr(uint16_t ccr[3]);

/* TIM1 hardware break: DC-bus over current latched by the break input. */
void state_task_isr_break(void);

/* Phase-current zero offsets (adc counts) used by the carrier ISR in it.c. */
int32_t state_task_adc_off_ib(void);
int32_t state_task_adc_off_ic(void);
int32_t state_task_adc_off_idc(void);

/* Telemetry snapshot (monitoring only; values may lag their source by 1 ms). */
extern monitor_parameter_t g_monitor_para;

#ifdef __cplusplus
}
#endif
#endif
