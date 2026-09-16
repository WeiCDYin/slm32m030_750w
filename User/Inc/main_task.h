#ifndef __MAIN_TASK_H__
#define __MAIN_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "mc.h"
#include "user_config.h"

/* All main-task runtime state is packed into one static instance. The type is
 * main_para_t (not state_t, which is the HSM leaf-state enum from foc.h). */
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
     * from boot; CMDBUS_CALI re-measures the two shunted phases on every START.
     * The derived phase keeps the mid-scale default. main_task_isr reads these
     * directly from g_main_para when converting the raw codes. */
    volatile int32_t adc_off_ia;
    volatile int32_t adc_off_ib;
    volatile int32_t adc_off_ic;

    /* AC peak window (10 x 50 Hz cycles), refreshed by the 1 ms task. */
    uint16_t ac_peak_high_v;
    uint16_t ac_peak_low_v;

    /* Last FOC duties (pu), carried between carrier frames for idc reconstruction. */
    volatile duties_t duties_q15;

    /* Charge state (sequential low-side bootstrap A -> B -> C). */
    volatile uint8_t charge_active;
    volatile uint8_t charge_phase; /* 0=A, 1=B, 2=C during sequential charge */
    volatile uint8_t charge_cnt;

    /* Phase-current offset calibration (CMDBUS_CALI): while cali_active==1
     * main_task_isr accumulates the three codes it receives (raw for the shunted
     * phases, ISR-derived for the missing phase); after ADC_OFFSET_CALI_SAMPLE_CNT
     * frames it publishes the averaged offsets into adc_off_ia/ib/ic and clears
     * cali_active. */
    volatile uint8_t  cali_active;
    volatile int32_t  cali_sum_ia;
    volatile int32_t  cali_sum_ib;
    volatile int32_t  cali_sum_ic;
    volatile uint16_t cali_cnt;
} main_para_t;

/* main_task -- the top application task. It owns the main state machine
 * (INIT/IDLE/CALI/CHARGE/RUNNING/FAULT, held on the command bus) and is the
 * ONE place that drives the control engine: it orchestrates foc, poke, fault
 * and ntc. The other two tasks (modbus_task, poke_task) and the ISR never call
 * foc/fault/ntc directly -- they only post commands on cmdbus, which this task
 * consumes.
 *
 *   main_task_init()  power-on: tune FOC, init fault table + ntc, register
 *                      command handlers on the bus, reset machine state.
 *   main_task_poll()  main loop: drain one bus command, run FOC hsm + poke,
 *                      advance the state machine.
 *   main_task_1ms()   1 ms tick: FOC slow loop, fault poll, poke resync,
 *                      ntc temperature, telemetry snapshot.
 *   main_task_isr()   carrier ISR: offset calibration / bootstrap while not
 *                      running; while RUNNING, software OC check, poke force,
 *                      FOC fast step; writes the new PWM duties into *dabc_ccr.
 *   main_task_isr_break()  TIM1 hardware break (DC over current).
 *
 * Telemetry: main_task owns the monitor snapshot g_monitor_para; the 1 ms
 * task refreshes it and the ISR refreshes the fast V/I fields. */

void main_task_init(void);
void main_task_poll(void);
void main_task_1ms(void);

/* Carrier frame. The ADC ISR only samples: it hands over the raw codes of the
 * three phase currents (ia/ib/ic) and the dc bus voltage (udc). This is the ONE
 * place that converts code -> pu, so dual- vs three-shunt sampling stays in the
 * ISR. When the machine is RUNNING this writes the new PWM CCR ticks; otherwise
 * only the cali / charge timing runs. */
void main_task_isr(int32_t ia_code, int32_t ib_code, int32_t ic_code, int32_t udc_code);

/* TIM1 hardware break: DC-bus over current latched by the break input. */
void main_task_isr_break(void);

#ifdef __cplusplus
}
#endif
#endif
