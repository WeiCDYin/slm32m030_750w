#ifndef __USER_BOARD_H__
#define __USER_BOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/* ===== board / hardware configuration (SLM32M030) =====
 * This header is PLATFORM-LAYER only: Bsp/, Drives/, main.c, slm32m030_it.c.
 * Platform-independent code (Modules/, User/, Core/) must not depend on it
 * directly -- it uses user_control.h (which pulls in what it needs) and the
 * Bsp/Inc/port.h hardware abstraction. */

/* ===== 0. input power ===== */
#define AC_PEAK_WIN_MS         (200u)
#define AC_INPUT_VOLTAGE_RATED (381.0f)
#define AC_INPUT_VOLTAGE_MIN   (AC_INPUT_VOLTAGE_RATED * 0.8f * 1.414f) /* 431 V */
#define AC_INPUT_VOLTAGE_MAX   (AC_INPUT_VOLTAGE_RATED * 1.2f * 1.414f) /* 646 V */
#define DC_VOLTAGE_MIN         (AC_INPUT_VOLTAGE_RATED * 0.7f * 1.414f) /* 377 V */
#define DC_VOLTAGE_MAX         (AC_INPUT_VOLTAGE_RATED * 1.2f * 1.414f) /* 646 V */
/* power-delay pin */
#define DC_DELAY_PIN_ON_THRESH_MV ((uint32_t)(DC_VOLTAGE_MIN * 0.8f * 1000.0f)) /* raise at 0.8 * UV [mV] */
#define DC_DELAY_PIN_HYST_MV      (30000u)                                      /* lower at ON threshold - 30 V [mV] */
#define DC_DELAY_PIN_HOLD_MS      (30u)                                         /* confirm time before toggle [ms] */

/* ===== 1. system clock ===== */
#define SYSTEM_CORE_CLOCK_HZ (64000000ul) /* system clock [Hz] */

/* ===== 2. timer / PWM config (Bsp tim.c) ===== */
#define PWM_FREQ_HZ        (10000.0f) /* carrier frequency [Hz] */
#define TIM_DEAD_TIME_NS   (1000.0f)  /* dead time [ns], 0..7937.5 */
#define TIM_PWM_RELOAD_CNT ((SYSTEM_CORE_CLOCK_HZ / (uint32_t)(2 * PWM_FREQ_HZ)) - 1)

/* ===== 3. analog front end ===== */
/* 12-bit ADC: code [0,4095] -> [0, ANALOG_REF_VOLTAGE] V */
#define ANALOG_REF_VOLTAGE        (5.0f)  /* ADC reference [V] */
#define SAMPLE_RESISTOR_PHASE     (0.03f) /* phase-current sense resistor [ohm] */
#define SAMPLE_RESISTOR_IBUS      (0.03f) /* DC-bus current sense resistor [ohm] */
#define SAMPLE_DIVISION_RATE_VBUS ((301 * 5 + 6.2f) / 6.2f)
#define SAMPLE_DIVISION_RATE_AC   ((301 * 5 + 6.2f) / 6.2f)
/* AFE current-sense PGA gain: numeric multiple only (convert.h scales by it);
 * the SATURN register enum derived from it lives in the BSP afe.h. 4/8/16/32 */
#define AFE_PGA_GAIN_NUM (8u)
/* post-conversion current gain compensation, applied to the pu phase/bus
 * currents in the ADC ISR (slm32m030_it.c). Float multiple: 1.2f = x1.2,
 * 1.0f = no extra gain. Implemented as Q8 fixed-point (x * N >> 8). */
#define CURRENT_GAIN_COMP  (1.2f)
#define CURRENT_GAIN_Q8    ((int32_t)(CURRENT_GAIN_COMP * 256.0f + 0.5f))
#define CURRENT_GAIN_SHIFT (8u)

/* ===== 4. hardware over-current comparator (DAC; code built in BSP afe.h) ===== */
#define HW_OC_TRIP_A (8.0f) /* [A] */

/* ===== 5. NVIC priorities (platform init) ===== */
#define NVIC_PRIORITY_TIM_BREAK  (0)
#define NVIC_PRIORITY_UART       (1)
#define NVIC_PRIORITY_ADC        (2)
#define NVIC_PRIORITY_TIM_MODBUS (3)

#ifdef __cplusplus
}
#endif
#endif
