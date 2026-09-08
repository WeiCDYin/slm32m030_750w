#include "convert.h"
#include "svm.h" /* SVM_UDC_NOM */

q15_t percent_to_q15(int16_t pct)
{
    if (pct <= 0)
        return 0;
    if (pct >= 100)
        return Q15_MAX;
    /* pct * 1342177 >> 12 == pct * Q15_ONE / 100, compile-time scaled */
    return (q15_t)(((int32_t)pct * PCT_TO_Q15_FIXED) >> PCT_TO_Q15_SHIFT);
}

int16_t q15_to_percent(q15_t v)
{
    return (int16_t)(((int32_t)v * 100) >> Q15_SHIFT);
}

angle_t deg_to_bam(int16_t deg)
{
    /* %360: wraps any int16 input and keeps (deg * FIXED) inside int32 */
    return (angle_t)(((int32_t)(deg % 360) * DEG_TO_BAM_FIXED) >> DEG_TO_BAM_SHIFT);
}

int16_t bam_to_deg(angle_t bam)
{
    return (int16_t)(((int32_t)bam * 360) >> 16); /* 65536 BAM = 360 deg */
}

q15_t rpm_to_pu(int16_t rpm)
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

int16_t pu_to_rpm(q15_t pu)
{
    return (int16_t)(((int32_t)pu * PU_TO_RPM_FIXED) >> PU_TO_RPM_SHIFT);
}

uint16_t duty_to_ccr(q15_t d, uint16_t arr)
{
    int32_t c = ((int32_t)d * (int32_t)arr) >> Q15_SHIFT;

    if (c < 0)
        c = 0;

    if (c > (int32_t)arr)
        c = (int32_t)arr;

    return (uint16_t)c;
}

q15_t iphase_code_to_pu(int32_t count, int32_t offset)
{
    int32_t pu = -(((count - offset) * IPHASE_SCALE_FIXED) >> IPHASE_SCALE_SHIFT);
    if (pu > Q15_MAX)
        pu = Q15_MAX;
    if (pu < Q15_MIN)
        pu = Q15_MIN;
    return (q15_t)pu;
}

q15_t idc_code_to_pu(int32_t count, int32_t offset)
{
    #define IDC_IIT_SHIFT 4
    static int32_t idc_filter = 0;

    int32_t raw;

    raw = (((count - offset) * IDC_SCALE_FIXED) >> IDC_PU_SCALE_SHIFT);
    idc_filter += (raw - idc_filter) >>  IDC_IIT_SHIFT;

    if (idc_filter > Q15_MAX)
        idc_filter = Q15_MAX;
    if (idc_filter < Q15_MIN)
        idc_filter = Q15_MIN;
    return (q15_t)idc_filter;
}

uint32_t idc_pu_to_ma(q15_t idc_pu)
{
    if (idc_pu <= 0)
        return 0;
    return (uint32_t)(((int32_t)idc_pu * IDC_MA_FIXED) >> IDC_MA_SHIFT);
}

int32_t udc_code_to_pu(int32_t count)
{
/*
 * α = 1 / 2^4 = 1/16 = 0.0625
 * y[n] = y[n−1] + α⋅(x[n]−y[n−1])
 * fc = α⋅Fs / 2pi
 * Fs = 1000Hz, fc = 10Hz
 */
#define UDC_IIR_SHIFT 4
    static int32_t udc_filer = SVM_UDC_NOM;
    int32_t        raw;

    raw = (count * UDC_SCALE_FIXED) >> UDC_SCALE_SHIFT;
    udc_filer += (raw - udc_filer) >> UDC_IIR_SHIFT;

    return (udc_pu_t)udc_filer;
}

uint32_t udc_pu_to_mv(int32_t udc_pu)
{
    if (udc_pu <= 0)
        return 0;
    return (uint32_t)(((int32_t)udc_pu * UDC_MV_FIXED) >> UDC_MV_SHIFT);
}

uint32_t ac_code_to_v(int32_t count)
{
    if (count <= 0)
        return 0;
    return (uint32_t)((count * AC_V_SCALE_FIXED) >> AC_V_SCALE_SHIFT);
}

uint16_t udc_idc_to_pwr_x10(uint32_t udc_mv, uint32_t idc_ma)
{
    return (uint16_t)(((udc_mv >> LOAD_UDC_PRESHIFT) * (idc_ma >> LOAD_IDC_PRESHIFT) * (uint32_t)LOAD_X10_FIXED) >> LOAD_X10_SHIFT);
}
