#include "slm32x030_hal.h"
#include "bsp_hal.h"
#include "ad.h"

ADC_HandleTypeDef g_adc_handle;

extern volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];
extern volatile uint32_t g_adc_seq2_code[ADC_SEQ2_COUNT];

void adc_init(void)
{
    /* ADC parameters */
    g_adc_handle.Instance                = ADC;
    g_adc_handle.Init.Seq1SampleCnt      = ADC_SAMPLE_SYSCLK;
    g_adc_handle.Init.ClkSel             = ADC_CLK_DIV;     /* 16 MHz */
    g_adc_handle.Init.SingleContinueMode = ADC_MODE_SINGLE; /* single conversion */
    g_adc_handle.Init.NbrOfConversion    = ADC_SEQ1_CONT_NUM_FOREVER;
    g_adc_handle.Init.AutoAccumulation   = ADC_AUTOACC_DISABLE;
    g_adc_handle.Init.VolRefSel          = ADC_VREFSEL_INPUT;
    g_adc_handle.Init.TemprEn            = ADC_TS_DISABLE;
    /* SEQ1 config: software-triggered slow channels (1 ms task, polled).
     * Trigger stays NULL; adc_seq1_sw_conv() kicks a frame on demand. */
    g_adc_handle.Init.Seq1_num         = ADC_SEQ1_SAMPLE_NUM;
    g_adc_handle.Init.Seq1Trig1        = ADC_TRIG_SEL_NULL; /* software trigger */
    g_adc_handle.Init.Seq1Trig2        = ADC_TRIG_SEL_NULL;
    g_adc_handle.Init.Seq1_awd_lt_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_awd_ht_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_awd_reg_int = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_1seq_int    = ADC_INTRPT_DISABLE; /* polled, no ISR */
    g_adc_handle.Init.Seq1_1ch_int     = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_all_int     = ADC_INTRPT_DISABLE;
    /* SEQ2 config: injected fast channels, TIM1_CC4 triggered, ISR-driven. */
    g_adc_handle.Init.Seq2_num         = ADC_SEQ2_SAMPLE_NUM;
    g_adc_handle.Init.Seq2Trig1        = ADC_TRIG_SEL_TIM1_CC4;
    g_adc_handle.Init.Seq2Trig2        = ADC_TRIG_SEL_NULL;
    g_adc_handle.Init.Seq2_awd_lt_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_awd_ht_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_awd_reg_int = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_1seq_int    = ADC_INTRPT_ENABLE; /* one-seq done -> ISR */
    g_adc_handle.Init.Seq2_1ch_int     = ADC_INTRPT_DISABLE;

    /* SEQ1 channel order == ad.h enum order == g_adc_seq1_code index */
    uint8_t adc_seq1_chn[ADC_SEQ1_CHANNEL_MAX] = {ADC_SQE_SEL_AIN1,  /* [0] ADC_SEQ1_AC_PEAK ac uvw peak voltage, PA1 */
                                                  ADC_SQE_SEL_AIN2,  /* [1] ADC_SEQ1_BEMF_A  phase-A bemf,       PA2  */
                                                  ADC_SQE_SEL_AIN7,  /* [2] ADC_SEQ1_BEMF_B  phase-B bemf,       PA7  */
                                                  ADC_SQE_SEL_AIN9,  /* [3] ADC_SEQ1_BEMF_C  phase-C bemf,       PB1  */
                                                  ADC_SQE_SEL_AIN13, /* [4] ADC_SEQ1_NTC     ntc,                PA11 */
                                                  ADC_SQE_SEL_NULL,  ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL,
                                                  ADC_SQE_SEL_NULL,  ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL,
                                                  ADC_SQE_SEL_NULL,  ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL};
    /* SEQ2 channel order == ad.h enum order == g_adc_seq2_code index.
     * Order MUST stay I_B, I_C, V_DC, I_DC so I_B/I_C straddle the ARR apex.
     * HAL_ADC_ConfigChannel packs six 5-bit slots into SEQ2CR2, so keep six. */
    uint8_t adc_seq2_chn[6] = {ADC_SQE_SEL_AIN3, /* [0] ADC_SEQ2_I_B  phase-B current,      PA3 */
                               ADC_SQE_SEL_AIN5, /* [1] ADC_SEQ2_I_C  phase-C current,      PA5 */
                               ADC_SQE_SEL_AIN0, /* [2] ADC_SEQ2_V_DC dc bus voltage,       PA0 */
                               ADC_SQE_SEL_AIN6, /* [3] ADC_SEQ2_I_DC dc bus current,       PA6 */
                               ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL};

    g_adc_handle.ChCfg.Seq1Ch         = adc_seq1_chn;
    g_adc_handle.ChCfg.Seq2Ch         = adc_seq2_chn;
    g_adc_handle.Init.DmaEn           = ADC_DMA_DISABLE;
    g_adc_handle.Init.Dma_overrun_int = ADC_INTRPT_DISABLE;

    HAL_ADC_Init(&g_adc_handle);
    HAL_ADC_ConfigChannel(&g_adc_handle);
    ADC_Enable(&g_adc_handle);

    /* Injected SEQ2 completion -> ADC IRQ (fast loop, it.c ADC_IRQHandler) */
    HAL_NVIC_SetPriority(ADC_IRQn, 2);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/* One software-triggered SEQ1 (slow) frame, polled to completion, results
 * snapshotted into g_adc_seq1_code. Called by the 1 ms task. */
void adc_seq1_sw_conv(void)
{
    uint32_t i;

    if (__HAL_ADC_GET_FLAG(&g_adc_handle, ADC_INTR_SEQ1_1SEQ_STS) == SET)
    {
        __HAL_ADC_CLEAR_FLAG(&g_adc_handle, ADC_INTR_SEQ1_1SEQ_STS);
        for (i = 0; i < ADC_SEQ1_COUNT; i++)
            g_adc_seq1_code[i] = *(volatile uint32_t *)(ADC_SEQ1SR1_ADDR + i * 4u);
    }
    CLEAR_BIT(ADC->CR, ADC_CR_SEQ1_START);
    SET_BIT(ADC->CR, ADC_CR_SEQ1_START);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hadc->Instance == ADC)
    {
        __HAL_RCMU_ADC_CLK_ENABLE();
        __HAL_RCMU_GPIOA_CLK_ENABLE();
        __HAL_RCMU_GPIOB_CLK_ENABLE();

        // ADC analog inputs.
        //   PA0  -> AIN0,  SEQ2 V_DC     (dc bus voltage)
        //   PA1  -> AIN1,  SEQ1 AC_PEAK  (ac uvw peak, lost-phase detect)
        //   PA2  -> AIN2,  SEQ1 BEMF_A   (phase-A bemf)
        //   PA3  -> AIN3,  SEQ2 I_B      (phase-B current)
        //   PA4  -> AIN4,  EXTRA dc current BEFORE filter
        //   PA5  -> AIN5,  SEQ2 I_C      (phase-C current)
        //   PA6  -> AIN6,  SEQ2 I_DC     (dc bus current)
        //   PA7  -> AIN7,  SEQ1 BEMF_B   (phase-B bemf)
        //   PA11 -> AIN13, SEQ1 NTC
        GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
        GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Otype = 0;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = 0;
        GPIO_InitStruct.Alternate   = GPIO_AF0;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        //   PB0  -> AIN8,  EXTRA adc ref voltage (ADC_VRef)
        //   PB1  -> AIN9,  SEQ1 BEMF_C   (phase-C bemf)
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC)
    {
        __HAL_RCMU_ADC_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);
    }
}
