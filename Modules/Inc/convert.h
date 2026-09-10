#ifndef __CONVERT_H__
#define __CONVERT_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "user_config.h"
#include "svm.h" /* SVM_UDC_NOM */

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

/* Fused phase-current gain: raw (count-offset) -> gain-compensated Q15 in ONE
 * multiply. The raw->pu factor (IPHASE_SCALE_FIXED, net >>10) and the post
 * CURRENT_GAIN_Q8 (net >>8) combine into a single Q15/count factor (net >>15).
 * Rounding the product once differs from the two-stage pipeline by <=2 LSB over
 * the normal operating span, and the single saturate also rails the region the
 * old un-saturated gain multiply used to wrap. |delta| up to ~3268 counts stays
 * inside int32. Derived at compile time: IPHASE_SCALE_FIXED * GAIN / 8. */
#define IPHASE_GAIN_SHIFT  (15)
#define IPHASE_GAIN_FIXED  (((IPHASE_SCALE_FIXED * CURRENT_GAIN_Q8 + 4) / 8))

/* All converters are header-only static inline so the 6 kHz carrier path pays
 * no call boundary (push/pop/bl/bx) -- the bodies are a few integer ops. */

static inline q15_t percent_to_q15(int16_t pct)
{
    if (pct <= 0)
        return 0;
    if (pct >= 100)
        return Q15_MAX;
    /* pct * 1342177 >> 12 == pct * Q15_ONE / 100, compile-time scaled */
    return (q15_t)(((int32_t)pct * PCT_TO_Q15_FIXED) >> PCT_TO_Q15_SHIFT);
}

static inline int16_t q15_to_percent(q15_t v)
{
    return (int16_t)(((int32_t)v * 100) >> Q15_SHIFT);
}

static inline angle_t deg_to_bam(int16_t deg)
{
    /* %360: wraps any int16 input and keeps (deg * FIXED) inside int32 */
    return (angle_t)(((int32_t)(deg % 360) * DEG_TO_BAM_FIXED) >> DEG_TO_BAM_SHIFT);
}

static inline int16_t bam_to_deg(angle_t bam)
{
    return (int16_t)(((int32_t)bam * 360) >> 16); /* 65536 BAM = 360 deg */
}

static inline q15_t rpm_to_pu(int16_t rpm)
{
    int32_t pu;

    if (rpm > SPEED_RATE_RPM)
        rpm = (int16_t)SPEED_RATE_RPM;
    else if (rpm < -SPEED_RATE_RPM)
        rpm = (int16_t)-SPEED_RATE_RPM;

    pu = ((int32_t)rpm * RPM_TO_PU_FIXED) >> RPM_TO_PU_SHIFT;

    if (pu > Q15_MAX)
        pu = Q15_MAX;
    if (pu < Q15_MIN)
        pu = Q15_MIN;
    return (q15_t)pu;
}

static inline int16_t pu_to_rpm(q15_t pu)
{
    return (int16_t)(((int32_t)pu * PU_TO_RPM_FIXED) >> PU_TO_RPM_SHIFT);
}

/* Carrier-ISR specialization: arr is the compile-time PWM reload, so the whole
 * scale+clamp expands inline with a constant multiplier (3 calls/tick). */
static inline uint16_t duty_to_ccr_arr(q15_t d)
{
    int32_t c = ((int32_t)d * (int32_t)TIM_PWM_RELOAD_CNT) >> Q15_SHIFT;
    return (uint16_t)c;
}

static inline q15_t iphase_code_to_pu(int32_t count, int32_t offset)
{
    int32_t pu = -(((count - offset) * IPHASE_SCALE_FIXED) >> IPHASE_SCALE_SHIFT);
    return q15_sat(pu);
}

/* Raw phase-current code -> gain-compensated Q15, one fused multiply. The
 * negation preserves iphase_code_to_pu's current sign convention. */
static inline q15_t iphase_code_to_gain_pu(int32_t count, int32_t offset)
{
    int32_t pu = -(((count - offset) * IPHASE_GAIN_FIXED) >> IPHASE_GAIN_SHIFT);
    return q15_sat(pu);
}

/* IIR state lives in a function-local static, so this header-inline form must
 * be called from ONE translation unit only (the carrier ISR in it.c); a second
 * caller would get its own disconnected filter state. */
static inline q15_t idc_code_to_pu(int32_t count, int32_t offset)
{
#define IDC_IIT_SHIFT 4
    static int32_t idc_filter = 0;

    int32_t raw;

    raw = (((count - offset) * IDC_SCALE_FIXED) >> IDC_PU_SCALE_SHIFT);
    idc_filter += (raw - idc_filter) >> IDC_IIT_SHIFT;

    if (idc_filter > Q15_MAX)
        idc_filter = Q15_MAX;
    if (idc_filter < Q15_MIN)
        idc_filter = Q15_MIN;
    return (q15_t)idc_filter;
}

static inline uint32_t idc_pu_to_ma(q15_t idc_pu)
{
    if (idc_pu <= 0)
        return 0;
    return (uint32_t)(((int32_t)idc_pu * IDC_MA_FIXED) >> IDC_MA_SHIFT);
}

/* Same single-translation-unit caveat as idc_code_to_pu: the IIR state is a
 * function-local static, today owned solely by the carrier ISR in it.c. */
static inline int32_t udc_code_to_pu(int32_t count)
{
#define UDC_IIR_SHIFT 4
    static int32_t udc_filer = SVM_UDC_NOM;
    int32_t        raw;

    raw = (count * UDC_SCALE_FIXED) >> UDC_SCALE_SHIFT;
    udc_filer += (raw - udc_filer) >> UDC_IIR_SHIFT;

    return (udc_pu_t)udc_filer;
}

static inline uint32_t udc_pu_to_mv(int32_t udc_pu)
{
    if (udc_pu <= 0)
        return 0;
    return (uint32_t)(((int32_t)udc_pu * UDC_MV_FIXED) >> UDC_MV_SHIFT);
}

static inline uint32_t ac_code_to_v(int32_t count)
{
    if (count <= 0)
        return 0;
    return (uint32_t)((count * AC_V_SCALE_FIXED) >> AC_V_SCALE_SHIFT);
}

static inline uint16_t udc_idc_to_pwr_x10(uint32_t udc_mv, uint32_t idc_ma)
{
#define PWR_IIR_SHIFT 4
    static int32_t pwr_filter = 0;
    int32_t        raw;

    raw = (int32_t)(((udc_mv >> LOAD_UDC_PRESHIFT) * (idc_ma >> LOAD_IDC_PRESHIFT) * (uint32_t)LOAD_X10_FIXED) >> LOAD_X10_SHIFT);
    pwr_filter += (raw - pwr_filter) >> PWR_IIR_SHIFT;

    if (pwr_filter < 0)
        pwr_filter = 0;
    if (pwr_filter > UINT16_MAX)
        pwr_filter = UINT16_MAX;
    return (uint16_t)pwr_filter;
}

#ifdef __cplusplus
}
#endif
#endif
