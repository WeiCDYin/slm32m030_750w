#ifndef __CONVERT_H__
#define __CONVERT_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "user_config.h"

// adc code convert to I pu
#define IPHASE_PU_PER_COUNT (ANALOG_REF_VOLTAGE / 4096.0f / (AFE_PGA_GAIN_NUM * SAMPLE_RESISTOR_PHASE) / I_BASE_A)
#define IPHASE_SCALE_SHIFT  (10)
#define IPHASE_SCALE_FIXED  ((int32_t)(IPHASE_PU_PER_COUNT * 32768.0f * (1 << IPHASE_SCALE_SHIFT) + 0.5f))
// adc code convert to I pu
#define IDC_PU_PER_COUNT   (ANALOG_REF_VOLTAGE / 4096.0f / (AFE_PGA_GAIN_NUM * SAMPLE_RESISTOR_IBUS) / I_BASE_A)
#define IDC_PU_SCALE_SHIFT (10)
#define IDC_SCALE_FIXED    ((int32_t)(IDC_PU_PER_COUNT * 32768.0f * (1 << IDC_PU_SCALE_SHIFT) + 0.5f))
// adc code convert to V pu
#define UDC_PU_PER_COUNT (ANALOG_REF_VOLTAGE / 4096.0f * SAMPLE_DIVISION_RATE_VBUS / U_BASE_V)
#define UDC_SCALE_SHIFT  (11)
#define UDC_SCALE_FIXED  ((int32_t)(UDC_PU_PER_COUNT * 8192.0f * (1 << UDC_SCALE_SHIFT) + 0.5f))
// pu vdc (Q13, udc_code_to_pu output) convert to mv; 1.0 pu = DC_VOLTAGE_MAX
#define UDC_MV_PER_PU (U_BASE_V * 1000.0f / 8192.0f)
#define UDC_MV_SHIFT  (10)
#define UDC_MV_FIXED  ((int32_t)(UDC_MV_PER_PU * (1 << UDC_MV_SHIFT) + 0.5f))
// pu current (Q15, idc_code_to_pu output) convert to ma; 1.0 pu = I_BASE_A
#define IDC_MA_PER_PU (I_BASE_A * 1000.0f / 32768.0f)
#define IDC_MA_SHIFT  (12)
#define IDC_MA_FIXED  ((int32_t)(IDC_MA_PER_PU * (1 << IDC_MA_SHIFT) + 0.5f))
// percent <-> q15 (compile-time scaled, no division at call sites)
#define PCT_TO_Q15_SHIFT (10)
#define PCT_TO_Q15_FIXED ((int32_t)((float)Q15_ONE * (1 << PCT_TO_Q15_SHIFT) / 100.0f + 0.5f))
// deg <-> bam (65536 BAM = 360 deg, scaled; %360 kept for input wrap/overflow)
#define DEG_TO_BAM_SHIFT (13)
#define DEG_TO_BAM_FIXED ((int32_t)(65536.0f * (1 << DEG_TO_BAM_SHIFT) / 360.0f + 0.5f))
// rpm <-> pu Q15, same formula as rpm_q15() in port/stm32g431/main.c:
// pu = rpm * MOTOR_NPP / (60 * W_BASE_HZ) = rpm / SPEED_RATE_RPM
#define RPM_TO_PU_SHIFT (12)
#define RPM_TO_PU_FIXED ((int32_t)((float)Q15_ONE * (1 << RPM_TO_PU_SHIFT) * (float)MOTOR_NPP / (60.0f * W_BASE_HZ) + 0.5f))
#define PU_TO_RPM_SHIFT (12)
#define PU_TO_RPM_FIXED ((int32_t)(60.0f * W_BASE_HZ / (float)MOTOR_NPP * (1 << PU_TO_RPM_SHIFT) / (float)Q15_ONE + 0.5f))
// adc code convert to AC phase voltage V (fixed point, mul + shift)
// 4096 codes = ANALOG_REF_VOLTAGE x SAMPLE_DIVISION_RATE_AC; SHIFT is 10 so
// max code 4095 x FIXED ~= 1.25e9 stays inside int32.
#define AC_V_PER_COUNT   (ANALOG_REF_VOLTAGE / 4096.0f * SAMPLE_DIVISION_RATE_AC)
#define AC_V_SCALE_SHIFT (10)
#define AC_V_SCALE_FIXED ((int32_t)(AC_V_PER_COUNT * (1 << AC_V_SCALE_SHIFT) + 0.5f))
// output loading [0.1W] = Vdc x Idc / 1e5, 32-bit fixed point (mul + shift,
// no division). udc>>3 and idc>>2 pre-scale so the product stays well below
// 2^31; FIXED ~= 2^(16+3+2)/1e5 = 21, total error < 0.5%.
#define LOAD_UDC_PRESHIFT (3)
#define LOAD_IDC_PRESHIFT (2)
#define LOAD_X10_SHIFT    (16)
#define LOAD_X10_FIXED    ((int32_t)((float)(1 << (LOAD_X10_SHIFT + LOAD_UDC_PRESHIFT + LOAD_IDC_PRESHIFT)) / 100000.0f + 0.5f))

q15_t    percent_to_q15(int16_t pct);
int16_t  q15_to_percent(q15_t v);
angle_t  deg_to_bam(int16_t deg);
int16_t  bam_to_deg(angle_t bam);
q15_t    rpm_to_pu(int16_t rpm);
int16_t  pu_to_rpm(q15_t pu);
uint16_t duty_to_ccr(q15_t d, uint16_t arr);
q15_t    idc_code_to_pu(int32_t count, int32_t offset);
q15_t    iphase_code_to_pu(int32_t count, int32_t offset);
int32_t  udc_code_to_pu(int32_t count);
uint32_t idc_pu_to_ma(q15_t idc_pu);
uint32_t udc_pu_to_mv(int32_t udc_pu);
uint32_t ac_code_to_v(int32_t count);
uint16_t udc_idc_to_pwr_x10(uint32_t udc_mv, uint32_t idc_ma);

#ifdef __cplusplus
}
#endif
#endif
