#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "slm32x030_hal.h"

/* ADC clock + sample-time timing (HAL enum based). Used by ad.c (setup) and
 * tim.c (CCR4 trigger point). */
#define ADC_CLK_DIV       ADC_CLOCK_PCLK_DIV4
#define ADC_SAMPLE_SYSCLK ADC_SAMPLE_8CYCLE

#define ADC_CLK_DIV_NUM      (0x01 << ADC_CLK_DIV)
#define ADC_SAMPLE_CLK_NUM   (ADC_SAMPLE_SYSCLK == ADC_SAMPLE_8CYCLE ? 8u : 4u)
#define ADC_CONVERT_CLK_NUM  (12u)
#define ADC_CH_TICKS         ((ADC_SAMPLE_CLK_NUM + ADC_CONVERT_CLK_NUM) * ADC_CLK_DIV_NUM)
#define ADC_SQE_SAMPLE_NUM   (ADC_SEQ1_COUNT - 1)
#define ADC_SQE_SAMPLE_TICKS (ADC_CH_TICKS * (ADC_SEQ1_COUNT - 1))

/* Note: Phase current sampling must be placed in the last two positions to accommodate DMA transfer */
enum
{
    ADC_SEQ1_V_DC,    /* [AIN0]-PA0, dc bus voltage */
    ADC_SEQ1_AC_PEAK, /* [AIN1]-PA1, ac uvw peak voltage, for lost phase detect */
    ADC_SEQ1_BEMF_A,  /* [AIN2]-PA2, phase-A bemf */
    ADC_SEQ1_BEMF_B,  /* [AIN7]-PA7, phase-B bemf */
    ADC_SEQ1_BEMF_C,  /* [AIN9]-PB1, phase-C bemf */
    ADC_SEQ1_NTC,     /* [AIN13]-PA11, ntc */
    ADC_SEQ1_I_DC,    /* [AIN6]-PA6, dc bus current */
    ADC_SEQ1_I_B,     /* [AIN3]-PA3, phase-B bemf */
    ADC_SEQ1_I_C,     /* [AIN5]-PA5, phase-C bemf */

    ADC_SEQ1_COUNT
};

/* DMA sink for the SEQ1 conversions: one code per enum member, stored in enum
 * order (the SEQ1 channel order equals the enum order). Defined in
 * it.c, filled by the ADC DMA on every carrier tick. */
extern volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];

/* Macro getter: the raw code of one channel, e.g. adc_get_code(ADC_SEQ1_V_DC)
 * returns the dc-bus-voltage code. The enum value doubles as the array index. */
#define adc_get_code(id) (g_adc_seq1_code[(id)])

void adc_init(void);
void adc_sw_conv(void);  /* startup: one software-triggered frame, polled (see ad.c) */
void adc_hw_start(void); /* startup done: switch to TIM1_CC4 trigger + DMA interrupt mode */

#ifdef __cplusplus
}
#endif

#endif
