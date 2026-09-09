#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

/* user_config.h -- project-wide numeric configuration + shared runtime data. HAL / register free: it
 * is consumed by Modules and User, and read by BSP drivers for the hardware-
 * tied values they need. Keep hardware values matched to the schematic / AFE
 * registers, since they feed conversions, protections and telemetry. */

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "types.h"
#include "mc.h"

/* ===== 0. input power ===== */
#define AC_PEAK_WIN_MS         (200u) /* 10 x 50 Hz mains cycles */
#define AC_INPUT_VOLTAGE_RATED (381.0f)
#define AC_INPUT_VOLTAGE_MIN   (AC_INPUT_VOLTAGE_RATED * 0.8f * 1.414f) /* 431 V */
#define AC_INPUT_VOLTAGE_MAX   (AC_INPUT_VOLTAGE_RATED * 1.2f * 1.414f) /* 646 V */
#define DC_VOLTAGE_MIN         (AC_INPUT_VOLTAGE_RATED * 0.7f * 1.414f) /* 377 V */
#define DC_VOLTAGE_MAX         (AC_INPUT_VOLTAGE_RATED * 1.2f * 1.414f) /* 646 V */
/* power-delay pin PB7: pin control only, no alarm */
#define DC_DELAY_PIN_ON_THRESH_MV ((uint32_t)(DC_VOLTAGE_MIN * 0.8f * 1000.0f)) /* raise at 0.8 * UV [mV] */
#define DC_DELAY_PIN_HYST_MV      (30000u)                                      /* lower at ON threshold - 30 V [mV] */
#define DC_DELAY_PIN_HOLD_MS      (30u)                                         /* confirm time before toggle [ms] */

/* ===== 1. constants ===== */
#define PI_F             (3.14159265f)
#define AUTO_RUN_MODE    (0) /* 0:normal; 1:free-running; 2:start-stop test */
#define AUTO_RUN_SPD_RPM (1200)

/* ===== 2. system clock ===== */
#define SYSTEM_CORE_CLOCK_HZ (64000000ul) /* system clock [Hz] */

/* ===== 3. timer / PWM config (Bsp tim.c) ===== */
#define PWM_FREQ_HZ        (6000.0f) /* carrier frequency [Hz] */
#define TIM_DEAD_TIME_NS   (1000.0f) /* dead time [ns], 0..7937.5 */
#define TIM_PWM_RELOAD_CNT ((SYSTEM_CORE_CLOCK_HZ / (uint32_t)(2 * PWM_FREQ_HZ)) - 1)

/* ===== 4. analog front end ===== */
/* 12-bit ADC: code [0,4095] -> [0, ANALOG_REF_VOLTAGE] V */
#define ANALOG_REF_VOLTAGE        (5.0f)  /* ADC reference [V] */
#define SAMPLE_RESISTOR_PHASE     (0.03f) /* phase-current sense resistor [ohm] */
#define SAMPLE_RESISTOR_IBUS      (0.03f) /* DC-bus current sense resistor [ohm] */
#define SAMPLE_DIVISION_RATE_VBUS ((301 * 5 + 6.2f) / 6.2f)
#define SAMPLE_DIVISION_RATE_AC   ((301 * 5 + 6.2f) / 6.2f)
#define SAMPLE_DC_VOLTAGE_MAX     (ANALOG_REF_VOLTAGE * SAMPLE_DIVISION_RATE_VBUS)

/* AFE current-sense PGA gain: numeric multiple only (convert.h scales by it);
 * the SATURN register enum derived from it lives in the BSP afe.h. 4/8/16/32 */
#define AFE_PGA_GAIN_NUM (8u)

/* post-conversion current gain compensation, applied to the pu phase/bus
 * currents in the ADC ISR (slm32m030_it.c). Float multiple: 1.2f = x1.2,
 * 1.0f = no extra gain. Implemented as Q8 fixed-point (x * N >> 8). */
#define CURRENT_GAIN_COMP (1.2f)
#define CURRENT_GAIN_Q8   ((int32_t)(CURRENT_GAIN_COMP * 256.0f + 0.5f))

/* DC-bus current (idc) source selection (ADC ISR, slm32m030_it.c):
 *   IDC_FROM_ADC        measure idc directly with the dedicated ADC channel
 *                       (current behaviour).
 *   IDC_FROM_PHASE_DUTY reconstruct idc from this frame's measured phase
 *                       currents weighted by the PREVIOUS frame's applied
 *                       phase duties: idc = ia*da + ib*db + ic*dc (pu). */
#define IDC_FROM_ADC        (0u)
#define IDC_FROM_PHASE_DUTY (1u)
#define IDC_SOURCE          (IDC_FROM_PHASE_DUTY)

/* CMDBUS_CALI phase-current zero offset: zero-current code = 12-bit mid-scale */
#define CALI_OFFSET_NOMINAL (2048) /* zero-current bias [adc count] */
#define CALI_OFFSET_TOL     (100)  /* allowed |offset - nominal| [adc count] */
#define CALI_SAMPLE_COUNT   (128)  /* carrier frames averaged */

/* CMDBUS_CHARGE bootstrap pre-charge: low-side on, per phase [carrier cycles] */
#define CHARGE_CARRIER_CYCLES (5)

/* ===== 5. motor electrical / mechanical parameters ===== */
#define MOTOR_RS_OHM       (9.0f)    /* stator resistance [ohm]  */
#define MOTOR_LD_H         (24e-3f)  /* d-axis inductance [H]    */
#define MOTOR_LQ_H         (31e-3f)  /* q-axis inductance [H]    */
#define MOTOR_LAMBDA_PM_WB (0.2197f) /* PM flux linkage [Wb]     */
#define MOTOR_NPP          (7)       /* pole pairs               */
#define MOTOR_J_KGM2       (0.06f)   /* rotor inertia [kg*m^2]   */
#define MOTOR_B_NMS        (0.0f)    /* viscous friction [N*m*s] */

/* ===== 6. FOC control-loop configuration ===== */
#define SPEED_RATE_RPM    (1450)                               /* rate speed [rpm] */
#define SPEED_BASE_RMP    (2000)                               /* pu speed, dueto feedback speed may over flow(negative) */
#define SPEED_REF_DEFAULT (1260)                               /* default speed reference after start [rpm] */
#define IQ_MAX_PU         (0.3f)                               /* q-axis current clamp [pu] */
#define I_BASE_A          (10.0f)                              /* max current [A] */
#define U_BASE_V          (2.0f * DC_VOLTAGE_MAX / PI_F)       /* max phase-voltage amplitude [V] */
#define W_BASE_HZ         (SPEED_BASE_RMP / 60.0f * MOTOR_NPP) /* max electrical frequency [Hz] */
#define CC_BW_RAD_S       (2.0f * PI_F * 200.0f)               /* current-loop bandwidth [rad/s] */
#define SC_BW_RAD_S       (2.0f * PI_F * 0.15f)                /* speed-loop bandwidth [rad/s] */
#define SC_ACCEL_PU_S     (0.5f)
#define FOC_FAST_HZ       PWM_FREQ_HZ /* carrier = fast loop */
#define FOC_SLOW_HZ       (1000.0f)   /* slow loop (speed) [Hz] */
/* IF (I-f open-loop startup) */
#define IF_HANDOVER_PU  (0.08f)   /* IF->FOC handover speed [pu] */
#define IF_I_MAG_PU     (0.15f)   /* current magnitude [pu]      */
#define IF_I_RAMP_MS    (2000.0f) /* 0 -> i_mag ramp [ms]        */
#define IF_I_HOLD_MS    (1000.0f) /* hold current vector [ms]    */
#define IF_W_ACCEL_PU_S (0.015f)  /* forced-speed ramp [pu/s]    */
/* SMO (sliding-mode observer) */
#define SMO_K_SLIDE       (0.80f)
#define SMO_SIG_A         (8.0f)
#define SMO_BW_PLL_RAD_S  (2.0f * PI_F * 20.0f) /* rotor-angle PLL [rad/s] */
#define SMO_ZETA_PLL      (1.0f)
#define CONV_FLOOR_SPD_PU (0.125f) /* below this speed no convergence verdict [pu] */
/* VF (scalar control) */
#define VF_V_BOOST_PU (0.02f)   /* standstill boost [pu] */
#define VF_V_RATED_PU (0.20f)   /* rated voltage [pu]    */
#define VF_W_RAMP_MS  (3000.0f) /* 0 -> full speed [ms]  */
/* VV / CV vector magnitude clamp (CMDBUS_CMD_VEC mag_pct) [% of base] */
#define VV_MAG_PCT_MAX (5u)  /* VV voltage-vector |u|, % of u_base */
#define CV_MAG_PCT_MAX (30u) /* CV current-vector |i|, % of i_base */

/* ===== 7. fault parameters =====
 * bit0~7 one-shot: fault_set() latches immediately (HW break, ISR OC); cleared
 *                 by CTRL=3 Recovery, not gated by FAULT_DETECT_ENABLE.
 * bit8~31 poll: window *_LIMIT + *_DETECT_CNT / *_RECOVER_CNT (1 ms polls). */
#define FAULT_DETECT_ENABLE        (1) /* master switch for poll channels */
#define DC_IN_OVER_VOLTAGE_ENABLE  (1)
#define DC_IN_UNDER_VOLTAGE_ENABLE (1)
#define POWER_OVER_LOAD_ENABLE     (0)
#define MOTOR_OVER_SPEED_ENABLE    (0)
#define TEMPERATURE_OVER_ENABLE    (1)
#define AC_IN_OVER_VOLTAGE_ENABLE  (1)
#define AC_IN_UNDER_VOLTAGE_ENABLE (1)
#define AC_IN_LOST_PHASE_ENABLE    (1)

/* id 0: DC bus over current HW (DAC comparator; code built in BSP afe.h) */
#define HW_OC_TRIP_A (8.0f) /* dc break over current [A] */
/* id 1: DC bus over current SW */
#define SW_BUS_OC_TRIP_A  (8.0f) /* dc software over current [A] */
#define SW_BUS_OC_TRIP_PU ((q15_t)((SW_BUS_OC_TRIP_A / I_BASE_A) * Q15_ONE + 0.5f))
/* id 2: DC phase over current SW */
#define SW_PHASE_OC_TRIP_A  (8.0f) /* dc phase software over current [A] */
#define SW_PHASE_OC_TRIP_PU ((q15_t)((SW_PHASE_OC_TRIP_A / I_BASE_A) * Q15_ONE + 0.5f))
/* id 6: FOC start timeout */
#define FOC_STARTUP_TIMEOUT_MS (10000u) /* startup sequence (RESYNC/STARTUP -> sensorless FOC) */

/* id 8: DC bus over voltage (sample: udc_mv [mV]) */
#define DC_IN_OVER_VOLTAGE_LIMIT       (DC_VOLTAGE_MAX * 1000.0f)
#define DC_IN_OVER_VOLTAGE_DETECT_CNT  (1000)
#define DC_IN_OVER_VOLTAGE_RECOVER_CNT (1000)
/* id 9: DC bus under voltage (sample: udc_mv [mV]) */
#define DC_IN_UNDER_VOLTAGE_LIMIT       (DC_VOLTAGE_MIN * 1000.0f)
#define DC_IN_UNDER_VOLTAGE_DETECT_CNT  (1000)
#define DC_IN_UNDER_VOLTAGE_RECOVER_CNT (1000)
/* id 10: output over load (sample: Vdc x Idc [0.1 W]) */
#define POWER_OVER_LOAD_LIMIT       (8000) /* output power [0.1 W] */
#define POWER_OVER_LOAD_DETECT_CNT  (5000)
#define POWER_OVER_LOAD_RECOVER_CNT (5000)
/* id 11: motor over speed (sample: spd_rpm_fb [rpm]) */
#define MOTOR_OVER_SPEED_LIMIT       (1680)
#define MOTOR_OVER_SPEED_DETECT_CNT  (100)
#define MOTOR_OVER_SPEED_RECOVER_CNT (100)
/* id 12: over temperature (sample: NTC [deg C]) */
#define TEMPERATURE_OVER_LIMIT       (95)
#define TEMPERATURE_OVER_DETECT_CNT  (1000)
#define TEMPERATURE_OVER_RECOVER_CNT (1000)
/* id 13: AC over voltage (sample: AC rectified HIGH peak [V]) */
#define AC_IN_OVER_VOLTAGE_LIMIT       ((int)(AC_INPUT_VOLTAGE_MAX))
#define AC_IN_OVER_VOLTAGE_DETECT_CNT  (500)
#define AC_IN_OVER_VOLTAGE_RECOVER_CNT (500)
/* id 14: AC under voltage (sample: AC HIGH peak [V]) */
#define AC_IN_UNDER_VOLTAGE_LIMIT       ((int)(AC_INPUT_VOLTAGE_MIN))
#define AC_IN_UNDER_VOLTAGE_DETECT_CNT  (500)
#define AC_IN_UNDER_VOLTAGE_RECOVER_CNT (500)
/* id 15: AC lost phase (sample: AC common-point LOW peak / valley [V]) */
#define AC_IN_LOST_PHASE_LIMIT       ((int32_t)AC_INPUT_VOLTAGE_MIN >> 1)
#define AC_IN_LOST_PHASE_DETECT_CNT  (500)
#define AC_IN_LOST_PHASE_RECOVER_CNT (500)

/* Telemetry snapshot. Monitoring only: refreshed by the 1 ms task; control /
 * fault code must not read it (values lag their source by up to 1 ms). */
typedef struct
{
    volatile uint8_t  state;          /* [cmdbus_state_t], 1 ms */
    volatile uint8_t  run_mode;       /* [cmdbus_run_mode_t], 1 ms */
    volatile uint32_t fault_curr;     /* [FAULT_ID], 1 ms */
    volatile uint32_t fault_latch;    /* [FAULT_ID], 1 ms */
    volatile uint16_t ac_peak_low_v;  /* [V] AC LOW peak (valley), 1 ms; lost phase */
    volatile uint16_t ac_peak_high_v; /* [V] AC HIGH peak (peak), 1 ms; over/under V */
    volatile uint32_t udc_mv;         /* [mV] DC bus voltage, 1 ms */
    volatile uint16_t idc_ma;         /* [mA] DC bus current, 1 ms */
    volatile int16_t  temperature;    /* [C] temperature, 1 ms */
    volatile int16_t  spd_rpm_ref;    /* [rpm] speed reference, 1 ms */
    volatile int16_t  spd_rpm_fb;     /* [rpm] speed feedback, 1 ms */
    volatile uint16_t pwr_watt_ref;   /* [0.1 W] power reference, 1 ms */
    volatile uint16_t pwr_watt_fb;    /* [0.1 W] power feedback, 1 ms */
} monitor_parameter_t;

/* ===== shared application runtime data =====
 * g_mc is defined in foc.c; g_monitor_para in state_task.c. The state-task's
 * internal state (machine state, offsets, V/I telemetry, cali/charge counters)
 * is packed into a static block in state_task.c; the carrier ISR reads the
 * zero offsets directly from g_state.adc_off_ib/ic/idc. */
extern mc_t                g_mc;
extern monitor_parameter_t g_monitor_para;

#ifdef __cplusplus
}
#endif

#endif
