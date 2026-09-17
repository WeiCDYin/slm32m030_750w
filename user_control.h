#ifndef __USER_CONTROL_H__
#define __USER_CONTROL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "user_board.h"
#include "types.h"
#include "mc.h"

/* ===== application / control configuration =====
 * Platform-independent (Modules/, User/, Core/). Board / hardware parameters
 * come from user_board.h; hardware access goes through Bsp/Inc/port.h. */

/* ===== 0. constants ===== */
#define PI_F (3.14159265f)

/* ===== 1. auto test (main.c) ===== */
#define AUTO_RUN_MODE              (0u)     /* 0:normal; 1:free-running; 2:start-stop test */
#define AUTO_RUN_SPD_RPM           (1200)   /* auto test spd [rpm] */
#define AUTO_FREE_RUNNING_DELAY_MS (3000u)  /* delay time after poweron */
#define AUTO_TEST_START_TIME_MS    (45000u) /* start-stop test start time */
#define AUTO_TEST_STOP_TIME_MS     (15000u) /* start-stop test stop time */

/* ===== 2. pre-charge timing ===== */
#define CHARGE_CARRIER_CYCLES (5u) /* pre-charge: low-side on, per phase [carrier cycles] */

/* ===== 3. motor electrical / mechanical parameters ===== */
#define MOTOR_RS_OHM       (9.0f)    /* stator resistance [ohm]  */
#define MOTOR_LD_H         (24e-3f)  /* d-axis inductance [H]    */
#define MOTOR_LQ_H         (31e-3f)  /* q-axis inductance [H]    */
#define MOTOR_LAMBDA_PM_WB (0.2197f) /* PM flux linkage [Wb]     */
#define MOTOR_NPP          (7u)      /* pole pairs               */
#define MOTOR_J_KGM2       (0.06f)   /* rotor inertia [kg*m^2]   */
#define MOTOR_B_NMS        (0.0f)    /* viscous friction [N*m*s] */

/* ===== 4. FOC control-loop configuration ===== */
#define SPEED_RATE_RPM    (1450)                               /* rate speed [rpm] */
#define SPEED_BASE_RMP    (2000)                               /* pu speed, dueto feedback speed may over flow(negative) */
#define SPEED_REF_DEFAULT (1260)                               /* default speed reference after start [rpm] */
#define IQ_MAX_PU         (0.35f)                              /* q-axis current clamp [pu] */
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

/* ===== 5. fault parameters =====
 * bit0~7 one-shot: fault_set() latches immediately (HW break, ISR OC); cleared
 *                 by CTRL=3 Recovery, not gated by FAULT_POLL_DETECT_ALL_ENABLE.
 * bit8~31 poll: window *_LIMIT + *_DETECT_CNT / *_RECOVER_CNT (1 ms polls). */
// poll fault enable/disable switcher
#define FAULT_POLL_DETECT_ALL_ENABLE          (1u)
#define FAULT_POLL_DC_IN_OVER_VOLTAGE_ENABLE  (1u)
#define FAULT_POLL_DC_IN_UNDER_VOLTAGE_ENABLE (1u)
#define FAULT_POLL_POWER_OVER_LOAD_ENABLE     (1u)
#define FAULT_POLL_MOTOR_OVER_SPEED_ENABLE    (1u)
#define FAULT_POLL_TEMPERATURE_OVER_ENABLE    (1u)
#define FAULT_POLL_AC_IN_OVER_VOLTAGE_ENABLE  (1u)
#define FAULT_POLL_AC_IN_UNDER_VOLTAGE_ENABLE (1u)
#define FAULT_POLL_AC_IN_LOST_PHASE_ENABLE    (1u)
// one-shot fault enable/disable switcher
#define FAULT_ONE_SHOT_HW_OVER_CURRENT_ENABLE  (1u)
#define FAULT_ONE_SHOT_SW_OVER_CURRENT_ENABLE  (1u)
#define FAULT_ONE_SHOT_MOTOR_LOST_PHASE_ENABLE (1u)
#define FAULT_ONE_SHOT_ZERO_OFFSET_ERR_ENABLE  (1u)
#define FAULT_ONE_SHOT_FOC_STARTUP_ERR_ENABLE  (0u)
/***************** [one shot] fault threshold setting *****************/
/* id 1: DC bus over current SW */
#define SW_BUS_OC_TRIP_A  (8.0f) /* [A] */
#define SW_BUS_OC_TRIP_PU ((q15_t)((SW_BUS_OC_TRIP_A / I_BASE_A) * Q15_ONE + 0.5f))
/* id 2: DC phase over current SW */
#define SW_PHASE_OC_TRIP_A  (8.0f) /* [A] */
#define SW_PHASE_OC_TRIP_PU ((q15_t)((SW_PHASE_OC_TRIP_A / I_BASE_A) * Q15_ONE + 0.5f))
/* id 3: motor phase lost error */
#define PHASE_LOSS_WIN_SHIFT   (13u)   /* averaging window = 2^13 carrier frames (~0.8 s @10 kHz) */
#define PHASE_LOSS_RATIO_PCT   (50u)   /* min/max below this = imbalanced [%] */
#define PHASE_LOSS_HOLD_WINS   (4u)    /* consecutive bad windows before the fault (~3 s) */
#define PHASE_LOSS_MIN_A       (0.05f) /* per-phase |i| below this = lost phase [A] */
#define PHASE_LOSS_MIN_PU      ((uint16_t)(PHASE_LOSS_MIN_A / I_BASE_A * Q15_ONE + 0.5f))
#define PHASE_LOSS_WIN_SAMPLES (1u << PHASE_LOSS_WIN_SHIFT)
/* id 4: adc offset calibration error */
#define ADC_OFFSET_CALI_DEFAULT    (2048) /* [lsb] */
#define ADC_OFFSET_CALI_THRESHOLD  (100)
#define ADC_OFFSET_CALI_SAMPLE_CNT (128u)
/* id 5: fault set by user protocol */
/* id 6: FOC start timeout */
#define FOC_STARTUP_TIMEOUT_MS (10000u) /* startup sequence (RESYNC/STARTUP -> sensorless FOC) */
/***************** [poll] fault threshold setting *****************/
/* id 8: DC bus over voltage (sample: udc_mv [mV]) */
#define DC_IN_OVER_VOLTAGE_LIMIT       (DC_VOLTAGE_MAX * 1000.0f)
#define DC_IN_OVER_VOLTAGE_DETECT_CNT  (1000u)
#define DC_IN_OVER_VOLTAGE_RECOVER_CNT (1000u)
/* id 9: DC bus under voltage (sample: udc_mv [mV]) */
#define DC_IN_UNDER_VOLTAGE_LIMIT       (DC_VOLTAGE_MIN * 1000.0f)
#define DC_IN_UNDER_VOLTAGE_DETECT_CNT  (1000u)
#define DC_IN_UNDER_VOLTAGE_RECOVER_CNT (1000u)
/* id 10: output over load (sample: Vdc x Idc [0.1 W]) */
#define POWER_OVER_LOAD_LIMIT       (13000u) /* [0.1 W] */
#define POWER_OVER_LOAD_DETECT_CNT  (5000u)
#define POWER_OVER_LOAD_RECOVER_CNT (5000u)
/* id 11: motor over speed (sample: spd_rpm_fb [rpm]) */
#define MOTOR_OVER_SPEED_LIMIT       (1600) /* [rpm] */
#define MOTOR_OVER_SPEED_DETECT_CNT  (100u)
#define MOTOR_OVER_SPEED_RECOVER_CNT (100u)
/* id 12: over temperature (sample: NTC [C]) */
#define TEMPERATURE_OVER_LIMIT       (95u) /* [C]*/
#define TEMPERATURE_OVER_DETECT_CNT  (1000u)
#define TEMPERATURE_OVER_RECOVER_CNT (1000u)
/* id 13: AC over voltage (sample: AC rectified HIGH peak [V]) */
#define AC_IN_OVER_VOLTAGE_LIMIT       ((int)(AC_INPUT_VOLTAGE_MAX))
#define AC_IN_OVER_VOLTAGE_DETECT_CNT  (500u)
#define AC_IN_OVER_VOLTAGE_RECOVER_CNT (500u)
/* id 14: AC under voltage (sample: AC HIGH peak [V]) */
#define AC_IN_UNDER_VOLTAGE_LIMIT       ((int)(AC_INPUT_VOLTAGE_MIN))
#define AC_IN_UNDER_VOLTAGE_DETECT_CNT  (500u)
#define AC_IN_UNDER_VOLTAGE_RECOVER_CNT (500u)
/* id 15: AC lost phase (sample: AC common-point LOW peak / valley [V]) */
#define AC_IN_LOST_PHASE_LIMIT       ((int32_t)AC_INPUT_VOLTAGE_MIN >> 1)
#define AC_IN_LOST_PHASE_DETECT_CNT  (500u)
#define AC_IN_LOST_PHASE_RECOVER_CNT (500u)

/* ===== 6. modbus configuration ===== */
#define MB_SLAVE_ADDR (0x01u)
#define MB_BAUD       (9600u)

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
 * g_mc is defined in foc.c; g_monitor_para in main_task.c. */
extern mc_t                g_mc;
extern monitor_parameter_t g_monitor_para;

#ifdef __cplusplus
}
#endif
#endif
