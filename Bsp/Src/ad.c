#include "slm32x030_hal.h"
#include "bsp_hal.h"
#include "ad.h"

ADC_HandleTypeDef g_adc_handle;

extern volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];
static DMA_HandleTypeDef g_adc_hdma_seq1;

static void adc_dma_init(void)
{
    __HAL_RCMU_DMA_CLK_ENABLE();

    g_adc_hdma_seq1.DmaBaseAddress   = DMA;
    g_adc_hdma_seq1.Init.DmaChannel  = DMA_CH1;
    g_adc_hdma_seq1.Init.Periphx_sel = DMA_ADC;
    g_adc_hdma_seq1.Init.Flowcntrl   = DMA_PERIPH_TO_SRAM;
    g_adc_hdma_seq1.Init.SrcInc      = DMA_SINC_ENABLE;
    g_adc_hdma_seq1.Init.DestInc     = DMA_DINC_ENABLE;
    g_adc_hdma_seq1.Init.Continue    = DMA_CONTI_ENABLE;

    __HAL_LINKDMA(&g_adc_handle, hdmaSeq1, g_adc_hdma_seq1);

    HAL_DMA_Init(&g_adc_hdma_seq1);

    /* NOTE: the DMA is NOT armed and its IRQ is NOT enabled here. The
     * carrier-synchronised TIM1_CC4 + DMA-interrupt mode is armed by
     * adc_hw_start(); adc_sw_conv() is a software-triggered single-frame
     * helper kept for manual / diagnostic capture. */
}

void adc_init(void)
{
    /* ADC parameters */
    g_adc_handle.Instance                = ADC;
    g_adc_handle.Init.Seq1SampleCnt      = ADC_SAMPLE_SYSCLK;
    g_adc_handle.Init.ClkSel             = ADC_CLK_DIV;               /* 16 MHz */
    g_adc_handle.Init.SingleContinueMode = ADC_MODE_SINGLE;           /* single conversion */
    g_adc_handle.Init.NbrOfConversion    = ADC_SEQ1_CONT_NUM_FOREVER; /* continue mode */
    g_adc_handle.Init.AutoAccumulation   = ADC_AUTOACC_DISABLE;
    g_adc_handle.Init.VolRefSel          = ADC_VREFSEL_INPUT;
    g_adc_handle.Init.TemprEn            = ADC_TS_DISABLE;
    /* SEQ1 config */
    g_adc_handle.Init.Seq1_num         = ADC_SQE_SAMPLE_NUM;
    g_adc_handle.Init.Seq1Trig1        = ADC_TRIG_SEL_NULL; /* software trigger during the startup phase */
    g_adc_handle.Init.Seq1Trig2        = ADC_TRIG_SEL_NULL;
    g_adc_handle.Init.Seq1_awd_lt_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_awd_ht_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_awd_reg_int = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_1seq_int    = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_1ch_int     = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq1_all_int     = ADC_INTRPT_DISABLE;
    /* SEQ2 config */
    g_adc_handle.Init.Seq2_num         = ADC_SQE_SAMPLE_NULL;
    g_adc_handle.Init.Seq2Trig1        = ADC_TRIG_SEL_NULL;
    g_adc_handle.Init.Seq2Trig2        = ADC_TRIG_SEL_NULL;
    g_adc_handle.Init.Seq2_awd_lt_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_awd_ht_int  = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_awd_reg_int = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_1seq_int    = ADC_INTRPT_DISABLE;
    g_adc_handle.Init.Seq2_1ch_int     = ADC_INTRPT_DISABLE;

    /* SEQ1 channel order == ad.h enum order == g_adc_seq1_code index */
    uint8_t adc_seq1_chn[ADC_SEQ1_CHANNEL_MAX] = {ADC_SQE_SEL_AIN0,  /* [0] ADC_SEQ1_V_DC    dc bus voltage,          PA0  */
                                                  ADC_SQE_SEL_AIN1,  /* [1] ADC_SEQ1_AC_PEAK ac uvw peak, lost phase, PA1  */
                                                  ADC_SQE_SEL_AIN2,  /* [2] ADC_SEQ1_BEMF_A  phase-A bemf,            PA2  */
                                                  ADC_SQE_SEL_AIN7,  /* [3] ADC_SEQ1_BEMF_B  phase-B bemf,            PA7  */
                                                  ADC_SQE_SEL_AIN9,  /* [4] ADC_SEQ1_BEMF_C  phase-C bemf,            PB1  */
                                                  ADC_SQE_SEL_AIN13, /* [5] ADC_SEQ1_NTC     ntc,                     PA11 */
                                                  ADC_SQE_SEL_AIN6,  /* [6] ADC_SEQ1_I_DC    dc bus current,          PA6  */
                                                  ADC_SQE_SEL_AIN3,  /* [7] ADC_SEQ1_I_B     phase-B current,         PA3  */
                                                  ADC_SQE_SEL_AIN5,  /* [8] ADC_SEQ1_I_C     phase-C current,         PA5  */
                                                  /* remaining SEQ1 slots unused (Seq1_num = ADC_SQE_SAMPLE_9): NULL */
                                                  ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL,
                                                  ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL};
    uint8_t adc_seq2_chn[ADC_SEQ2_CHANNEL_MAX] = {ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL, ADC_SQE_SEL_NULL};

    g_adc_handle.ChCfg.Seq1Ch         = adc_seq1_chn;
    g_adc_handle.ChCfg.Seq2Ch         = adc_seq2_chn;
    g_adc_handle.Init.DmaEn           = ADC_DMA_ENABLE;
    g_adc_handle.Init.Dma_overrun_int = ADC_INTRPT_DISABLE;

    HAL_ADC_Init(&g_adc_handle);
    HAL_ADC_ConfigChannel(&g_adc_handle);
    adc_dma_init();
    ADC_Enable(&g_adc_handle);

    adc_hw_start();
}

/* Software-triggered single-frame capture (manual / diagnostic helper).
 *
 * HAL_DMA_Start_IT (not HAL_DMA_Start) is used on purpose: the TC status bit in
 * INTTCSTA is the MASKED flag and only asserts when CONFIG.TC_INT_MASK is set,
 * which HAL_DMA_Start never does but HAL_DMA_Start_IT does. The DMA NVIC IRQ is
 * not enabled until adc_hw_start(), so no ISR fires here -- we only borrow the
 * mask bit so the poll can observe completion. Do NOT call while the
 * carrier-synchronised TIM1_CC4 + DMA mode is running: it clears DMA CONTI and
 * busy-waits, conflicting with the live path.
 */
void adc_sw_conv(void)
{
    uint32_t ch = 1u << g_adc_hdma_seq1.ChannelNum;

    g_adc_hdma_seq1.State = HAL_DMA_STATE_READY;
    g_adc_hdma_seq1.Lock  = HAL_UNLOCKED;
    /* Single-shot: clear CONTI so the controller stops after this frame and
     * the TC state machine returns to READY. Restored in adc_hw_start(). */
    g_adc_hdma_seq1.Instance->CONTROL &= ~DMA_CONTI_ENABLE;
    HAL_DMA_Start_IT(&g_adc_hdma_seq1, ADC_SEQ1SR1_ADDR, (uint32_t)g_adc_seq1_code, ADC_SEQ1_COUNT);
    __HAL_DMA_CLEAR_INTTCCLR_FLAG(&g_adc_hdma_seq1, ch);
    CLEAR_BIT(ADC->CR, ADC_CR_SEQ1_START);
    SET_BIT(ADC->CR, ADC_CR_SEQ1_START);
    while (!__HAL_DMA_GET_INTTCSTA_FLAG(&g_adc_hdma_seq1, ch))
    {
        /* busy-wait for the frame */
    }
    __HAL_DMA_CLEAR_INTTCCLR_FLAG(&g_adc_hdma_seq1, ch);
    CLEAR_BIT(ADC->CR, ADC_CR_SEQ1_START);
}

/* End of the software-sampling startup phase: switch the SEQ1 trigger to the
 * TIM1_CC4 carrier edge and the DMA to interrupt mode, so the carrier ISR
 * (DMA1_Channel0_5_IRQHandler) starts running the FOC fast loop. */
void adc_hw_start(void)
{
    MODIFY_REG(ADC->SEQ1CR1, ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Msk, (uint32_t)ADC_TRIG_SEL_TIM1_CC4 << ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Pos);

    HAL_NVIC_SetPriority(DMA1_Channel0_5_IRQn, 2);
    HAL_NVIC_EnableIRQ(DMA1_Channel0_5_IRQn);

    g_adc_hdma_seq1.State = HAL_DMA_STATE_READY;
    g_adc_hdma_seq1.Lock  = HAL_UNLOCKED;
    HAL_DMA_Start_IT(&g_adc_hdma_seq1, ADC_SEQ1SR1_ADDR, (uint32_t)g_adc_seq1_code, ADC_SEQ1_COUNT);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hadc->Instance == ADC)
    {
        __HAL_RCMU_ADC_CLK_ENABLE();
        __HAL_RCMU_GPIOA_CLK_ENABLE();
        __HAL_RCMU_GPIOB_CLK_ENABLE();

        // ADC analog inputs. SEQ1 uses AIN0..AIN13 in enum order; the last two
        // are NON-SEQ1 monitor pins (configured here, read outside the loop):
        //   PA0  -> AIN0,  SEQ1 V_DC     (dc bus voltage)
        //   PA1  -> AIN1,  SEQ1 AC_PEAK  (ac uvw peak, lost-phase detect)
        //   PA2  -> AIN2,  SEQ1 BEMF_A   (phase-A bemf)
        //   PA3  -> AIN3,  SEQ1 I_B      (phase-B current)
        //   PA4  -> AIN4,  EXTRA dc current BEFORE filter
        //   PA5  -> AIN5,  SEQ1 I_C      (phase-C current)
        //   PA6  -> AIN6,  SEQ1 I_DC     (dc bus current)
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
