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

#define ADC_CLK_DIV_NUM     (0x01 << ADC_CLK_DIV)
#define ADC_SAMPLE_CLK_NUM  (ADC_SAMPLE_SYSCLK == ADC_SAMPLE_8CYCLE ? 8u : 4u)
#define ADC_CONVERT_CLK_NUM (12u)
#define ADC_CH_TICKS        ((ADC_SAMPLE_CLK_NUM + ADC_CONVERT_CLK_NUM) * ADC_CLK_DIV_NUM)

/* SEQ1 (software-triggered, polled by the 1 ms task): slow monitor channels.
 * Channel order == enum order == g_adc_seq1_code index. */
enum
{
    ADC_SEQ1_AC_PEAK, /* [AIN1]-PA1, ac uvw peak voltage, for lost phase detect */
    ADC_SEQ1_BEMF_A,  /* [AIN2]-PA2, phase-A bemf */
    ADC_SEQ1_BEMF_B,  /* [AIN7]-PA7, phase-B bemf */
    ADC_SEQ1_BEMF_C,  /* [AIN9]-PB1, phase-C bemf */
    ADC_SEQ1_NTC,     /* [AIN13]-PA11, ntc */

    ADC_SEQ1_COUNT
};
#define ADC_SEQ1_SAMPLE_NUM (ADC_SEQ1_COUNT - 1)

/* SEQ2 (injected, TIM1_CC4 triggered, interrupt-driven): fast current / DC
 * channels. Order is fixed by the straddle requirement and MUST NOT change:
 * I_B, I_C, V_DC, I_DC. Channel order == enum order == g_adc_seq2_code index. */
enum
{
    ADC_SEQ2_I_B,  /* [AIN3]-PA3, phase-B current */
    ADC_SEQ2_I_C,  /* [AIN5]-PA5, phase-C current */
    ADC_SEQ2_V_DC, /* [AIN0]-PA0, dc bus voltage */
    ADC_SEQ2_I_DC, /* [AIN6]-PA6, dc bus current */

    ADC_SEQ2_COUNT
};
#define ADC_SEQ2_SAMPLE_NUM (ADC_SEQ2_COUNT - 1)

/* Injected SEQ2 straddles the ARR apex: I_B (slot 0) samples just before the
 * apex and I_C (slot 1) just after it, so the CC4 trigger leads the apex by
 * exactly the distance from the trigger to the I_B/I_C boundary (one channel
 * window). Used by tim.c for the TIM_CCR4_TRIGGER_ADC_CNT point. */
#define ADC_SEQ2_TRIG_LEAD_TICKS (ADC_CH_TICKS * (ADC_SEQ2_I_C - ADC_SEQ2_I_B))

/* SEQ1 (software) code sink: filled by adc_seq1_sw_conv() (1 ms task). */
extern volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];
/* SEQ2 (injected) code sink: filled by the ADC ISR (it.c) every carrier. */
extern volatile uint32_t g_adc_seq2_code[ADC_SEQ2_COUNT];

/* Macro getters: the raw code of one channel. adc_get_seq1_code() addresses the
 * software SEQ1 slow channels, adc_get_seq2_code() the injected SEQ2 fast ones. */
#define adc_get_seq1_code(id) (g_adc_seq1_code[(id)])
#define adc_get_seq2_code(id) (g_adc_seq2_code[(id)])

void adc_init(void);
void adc_seq1_sw_conv(void); /* 1 ms task: one software-triggered SEQ1 frame, polled */

#ifdef __cplusplus
}
#endif

#endif
