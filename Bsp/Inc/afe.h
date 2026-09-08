#ifndef __AFE_H__
#define __AFE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "slm32x030_hal.h"
#include "saturn_ll_reg.h"
#include "user_config.h" /* AFE_PGA_GAIN_NUM, HW_OC_TRIP_A, sense resistors */

/* PGA gain as the SATURN register enum (derived from the numeric AFE_PGA_GAIN_NUM). */
#define AFE_PGA_GAIN (AFE_PGA_GAIN_NUM == 4u ? PGA_GAIN_4 : AFE_PGA_GAIN_NUM == 8u ? PGA_GAIN_8 : AFE_PGA_GAIN_NUM == 16u ? PGA_GAIN_16 : PGA_GAIN_32)

/* HW over-current DAC comparator code (SATURN reg00, 8-bit). */
#define DAC_OUTPUT_CODE (((HW_OC_TRIP_A * AFE_PGA_GAIN_NUM * SAMPLE_RESISTOR_IBUS + ANALOG_REF_VOLTAGE / 2.0f) / ANALOG_REF_VOLTAGE * 256.0f))

void afe_init(void);

#ifdef __cplusplus
}
#endif

#endif
