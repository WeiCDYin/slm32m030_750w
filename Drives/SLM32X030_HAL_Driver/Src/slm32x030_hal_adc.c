/**
 ******************************************************************************
 * @file    slm32x030_hal_adc.c
 * @author  MCU Software Team
 * @Version V1.2.0
 * @Date    8-Nov-2024
 * @brief   This file provides firmware functions to manage the following
 *          functionalities of the Analog to Digital Convertor (ADC)
 *          peripheral:
 *           + Initialization and de-initialization functions
 *             ++ Initialization and Configuration of ADC
 *           + Operation functions
 *             ++ Start, stop, get result of conversions adc, using 3 possible modes: polling, interruption.
 *           + Control functions
 *             ++ Threshold configuration
 *           + State functions
 *             ++ ADC state machine management
 *             ++ Interrupts and flags management
 *
 ********************************************************************************/
#include "slm32x030_hal.h"

#ifdef HAL_ADC_MODULE_ENABLED

/* Timeout values for ADC enable and disable settling time.                 */
/* Values defined to be higher than worst cases: low clocks freq,           */
/* maximum prescaler.                                                       */
/* Ex of profile low frequency : Clock source at 0.1 MHz, ADC clock         */
/* prescaler 4, sampling time 12.5 ADC clock cycles, resolution 12 bits.    */
/* Unit: ms                                                                 */
#define ADC_ENABLE_TIMEOUT 2U
#define ADC_DISABLE_TIMEOUT 2U

/* Delay for ADC stabilization time.                                        */
/* Maximum delay is 1us (refer to device datasheet, parameter tSTAB).       */
/* Unit: us                                                                 */
#define ADC_STAB_DELAY_US 1U

/* Delay for temperature sensor stabilization time.                         */
/* Maximum delay is 10us (refer to device datasheet, parameter tSTART).     */
/* Unit: us                                                                 */
#define ADC_TEMPSENSOR_DELAY_US 10U

/**
 * @brief  Initializes the ADC peripheral and regular group according to
 *         parameters specified in structure "ADC_InitTypeDef".
 * @note   As prerequisite, ADC clock must be configured at RCMU top level
 *         (clock source APB).
 *         See commented example code below that can be copied and uncommented
 *         into HAL_ADC_MspInit().
 * @note   Possibility to update parameters on the fly:
 *         This function initializes the ADC MSP (HAL_ADC_MspInit()) only when
 *         coming from ADC state reset. Following calls to this function can
 *         be used to reconfigure some parameters of ADC_InitTypeDef
 *         structure on the fly, without modifying MSP configuration. If ADC
 *         MSP has to be modified again, HAL_ADC_DeInit() must be called
 *         before HAL_ADC_Init().
 *         The setting of these parameters is conditioned to ADC state.
 *         For parameters constraints, see comments of structure
 *         "ADC_InitTypeDef".
 * @param  hadc: ADC handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;
    uint32_t tmp_regs = 0U;

    if (hadc == NULL)
    {
        return HAL_ERROR;
    }

    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
    assert_param(IS_FUNCTIONAL_STATE(hadc->Init.SingleContinueMode));

    HAL_ADC_MspInit(hadc);
    /* Actions performed only if ADC is coming from state reset:                */
    /* - Initialization of ADC MSP                                              */
    if (hadc->State == HAL_ADC_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hadc->Lock = HAL_UNLOCKED;
    }

    /* Disable ADC peripheral */
    /* Note: Reset ADC continuous conversion status        */
    tmp_hal_status = ADC_ConversionStatus_Reset(hadc);

    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL) && (tmp_hal_status == HAL_OK))
    {
        /* Set ADC state */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY, HAL_ADC_STATE_BUSY_INTERNAL);

        tmp_regs = hadc->Instance->CR;
        /* VREFSEL  ADC reference voltage */
        tmp_regs |= hadc->Init.VolRefSel << ADC_CR_VREFSEL_Pos;

        /* CLK_SEL  ADC clock division,from APB2 clock */
        tmp_regs |= hadc->Init.ClkSel << ADC_CR_CLK_SEL_Pos;

        /* Set ADC parameters */
        /* Configuration of ADC:                                                  */
        /*  - external trigger or software to start conversion                    */
        /*  - continuous conversion mode                                          */
        tmp_regs |= ((hadc->Init.Seq1SampleCnt << ADC_CR_SAMP_Pos) | (hadc->Init.ClkSel << ADC_CR_CLK_SEL_Pos));

        /* Continuous conversion or Single conversion selection */
        tmp_regs |= (hadc->Init.SingleContinueMode << ADC_CR_SEQ1_CONT_EN_Pos);
        /* SEQ1_CONT_NUM  seq1 continue conversion count */
        tmp_regs |= ((hadc->Init.NbrOfConversion) << ADC_CR_SEQ1_CONT_NUM_Pos);

        /* SEQ1_ACC_EN  seq1 accumulation enable */
        tmp_regs |= (hadc->Init.AutoAccumulation << ADC_CR_SEQ1_ACC_EN_Pos);

        /* DMA_EN  DMA enable */
        tmp_regs |= (hadc->Init.DmaEn << ADC_CR_DMA_EN_Pos);

        /* TS_EN  temperature sensor enable */
        tmp_regs |= (hadc->Init.TemprEn << ADC_CR_TS_EN_Pos);
        WRITE_REG(hadc->Instance->CR, tmp_regs);

        /* config seq1 channel and trigger */
        tmp_regs = hadc->Instance->SEQ1CR1;
        tmp_regs |= hadc->Init.Seq1_num << ADC_SEQ1CR1_SEQ1_NUM_Pos;
        tmp_regs |= hadc->Init.Seq1Trig1 << ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Pos;
        tmp_regs |= hadc->Init.Seq1Trig2 << ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Pos;
        WRITE_REG(hadc->Instance->SEQ1CR1, tmp_regs);

        /* config seq2 channel and trigger */
        tmp_regs = hadc->Instance->SEQ2CR1;
        tmp_regs |= hadc->Init.Seq2_num << ADC_SEQ2CR1_SEQ2_NUM_Pos;
        tmp_regs |= hadc->Init.Seq2Trig1 << ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Pos;
        tmp_regs |= hadc->Init.Seq2Trig2 << ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Pos;
        WRITE_REG(hadc->Instance->SEQ2CR1, tmp_regs);

        /* config ADC interrupt state register */
        tmp_regs = hadc->Instance->INTR;
        tmp_regs |= hadc->Init.Seq2_awd_lt_int << ADC_INTR_SEQ2_AWD_LT_Pos;
        tmp_regs |= hadc->Init.Seq2_awd_ht_int << ADC_INTR_SEQ2_AWD_HT_Pos;
        tmp_regs |= hadc->Init.Seq2_awd_reg_int << ADC_INTR_SEQ2_AWD_REG_Pos;
        tmp_regs |= hadc->Init.Seq2_1seq_int << ADC_INTR_SEQ2_1SEQ_Pos;
        tmp_regs |= hadc->Init.Seq2_1ch_int << ADC_INTR_SEQ2_1CH_Pos;

        tmp_regs |= hadc->Init.Seq1_awd_lt_int << ADC_INTR_SEQ1_AWD_LT_Pos;
        tmp_regs |= hadc->Init.Seq1_awd_ht_int << ADC_INTR_SEQ1_AWD_HT_Pos;
        tmp_regs |= hadc->Init.Seq1_awd_reg_int << ADC_INTR_SEQ1_AWD_REG_Pos;
        tmp_regs |= hadc->Init.Seq1_all_int << ADC_INTR_SEQ1_ALL_Pos;
        tmp_regs |= hadc->Init.Seq1_1seq_int << ADC_INTR_SEQ1_1SEQ_Pos;
        tmp_regs |= hadc->Init.Seq1_1ch_int << ADC_INTR_SEQ1_1CH_Pos;
        tmp_regs |= hadc->Init.Dma_overrun_int << ADC_INTR_DMA_OVERRUN_Pos;
        WRITE_REG(hadc->Instance->INTR, tmp_regs);
    }
    else
    {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        tmp_hal_status = HAL_ERROR;
    }
    return tmp_hal_status;
}

/**
 * @brief  Configures the the selected channel.
 * @note   Possibility to update parameters on the fly:
 *         This function initializes channel to be converted, following
 *         calls to this function can be used to reconfigure some parameters
 *         of structure "ADC_ChannelConfTypeDef" on the fly, without reseting
 *         the ADC.
 *         The setting of these parameters is conditioned to ADC state.
 *         For parameters constraints, see comments of structure
 *         "ADC_ChannelConfTypeDef".
 * @param  hadc: ADC handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *hadc)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;
    __IO uint32_t wait_loop_index = 0U;
    uint32_t index = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
    assert_param(IS_ADC_SEQ_TYPE(hadc->ChCfg.Seq1Ch));
    assert_param(IS_ADC_SEQ_TYPE(hadc->ChCfg.Seq2Ch));

    /* Process locked */
    __HAL_LOCK(hadc);
    for (index = 0; index < (ADC_SEQ1_CHANNEL_MAX / 6); index++)
    {
        *(__IO uint32_t *)(ADC_SEQ1CR2_ADDR + index * 4) = (hadc->ChCfg.Seq1Ch[0 + index * 6] << 0) | (hadc->ChCfg.Seq1Ch[1 + index * 6] << 5) | (hadc->ChCfg.Seq1Ch[2 + index * 6] << 10) |
                                                           (hadc->ChCfg.Seq1Ch[3 + index * 6] << 15) | (hadc->ChCfg.Seq1Ch[4 + index * 6] << 20) | (hadc->ChCfg.Seq1Ch[5 + index * 6] << 25);
    }

    *(__IO uint32_t *)(ADC_SEQ2CR2_ADDR) = (hadc->ChCfg.Seq2Ch[0] << 0) | (hadc->ChCfg.Seq2Ch[1] << 5) | (hadc->ChCfg.Seq2Ch[2] << 10) |
                                           (hadc->ChCfg.Seq2Ch[3] << 15) | (hadc->ChCfg.Seq2Ch[4] << 20) | (hadc->ChCfg.Seq2Ch[5] << 25);
    /* Process unlocked */
    __HAL_UNLOCK(hadc);

    /* Return function status */
    return tmp_hal_status;
}

/**
 * @brief  Get ADC conversion result.
 * @param  hadc: ADC handle
 * @param  channel: ADC channel number used for continuous mode @ref ADC_ContinueChannelSel
 *                              = ADC_CONTINUE_CHANNEL_0
 *                              = ADC_CONTINUE_CHANNEL_1
 *                              = ADC_CONTINUE_CHANNEL_2
 *                              = ADC_CONTINUE_CHANNEL_3
 *                              = ADC_CONTINUE_CHANNEL_4
 *                              = ADC_CONTINUE_CHANNEL_5
 *                              = ADC_CONTINUE_CHANNEL_6
 *                              = ADC_CONTINUE_CHANNEL_7
 * @note   For single channel mode, param channel will have no effect.
 * @retval ADC conversion data
 */
void HAL_ADC_GetValue(ADC_HandleTypeDef *hadc, uint8_t seqindex)
{
    uint32_t index = 0;
    if (seqindex == ADC_SEQ1_DEF)
    {
        for (index = 0; index <= hadc->Init.Seq1_num; index++)
        {
            if (hadc->ChCfg.Seq1Ch[index] == ADC_SQE_SEL_NULL)
            {
                hadc->Seq1ChnValue[index] = 0u;
            }
            else
            {
                hadc->Seq1ChnValue[index] = (uint32_t)(*(__IO uint32_t *)(ADC_SEQ1SR1_ADDR + index * 4));
            }
        }
    }
    else if (seqindex == ADC_SEQ2_DEF)
    {
        for (index = 0; index <= hadc->Init.Seq2_num; index++)
        {
            if (hadc->ChCfg.Seq1Ch[index] == ADC_SQE_SEL_NULL)
            {
                hadc->Seq2ChnValue[index] = 0u;
            }
            else
            {
                hadc->Seq2ChnValue[index] = (uint32_t)(*(__IO uint32_t *)(ADC_SEQ2SR1_ADDR + index * 4));
            }
        }
    }
}

/**
 * @brief  Get ADC last conversion result.
 * @param  hadc: ADC handle
 * @note   None
 * @retval ADC conversion data
 */
void HAL_ADC_GetLastValue(ADC_HandleTypeDef *hadc)
{
    uint32_t adc_reg_dr;
    adc_reg_dr = (uint32_t)(*(__IO uint32_t *)(ADC_DR_ADDR));
    hadc->Seq1ChnValue[0] = (uint32_t)(adc_reg_dr & 0x0000FFFF);
    hadc->Seq2ChnValue[0] = (uint32_t)((adc_reg_dr >> 16) & 0x0000FFFF);
}

/**
 * @brief  Get ADC conversion result.
 * @param  hadc: ADC handle
 * @note   This function is only valid when RACC_EN is set.
 * @retval ADC conversion data
 */
uint32_t HAL_ADC_GetAccValue(ADC_HandleTypeDef *hadc)
{
    uint32_t tmpADCvalue;
    /* Check the parameters */
    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
    tmpADCvalue = READ_BIT(hadc->Instance->ACCR, ADC_ACCR_ACC);
    return tmpADCvalue;
}

/**
 * @brief  Enables ADC, starts conversion.
 *         Interruptions enabled in this function: None.
 * @param  hadc: ADC handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

    /* Process locked */
    __HAL_LOCK(hadc);

    /* Enable the ADC peripheral */
    tmp_hal_status = ADC_Enable(hadc);

    if (hadc->Init.Seq1_num != ADC_SQE_SAMPLE_NULL)
    {
        SET_BIT(hadc->Instance->CR, ADC_CR_SEQ1_START);
    }    
    if (hadc->Init.Seq2_num != ADC_SQE_SAMPLE_NULL)
    {
        SET_BIT(hadc->Instance->CR, ADC_CR_SEQ2_START);
    }

    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
        /* Set ADC state                                                          */
        /* - Clear state bitfield related to conversion results                                 */
        /* - Set state bitfield related to conversion                                   */
        ADC_STATE_CLR_SET(hadc->State,
                          HAL_ADC_STATE_READY | HAL_ADC_STATE_EOC,
                          HAL_ADC_STATE_BUSY);

        /* Reset ADC all error code fields */
        ADC_CLEAR_ERRORCODE(hadc);

        /* Process unlocked */
        /* Unlock before starting ADC conversions: in case of potential           */
        /* interruption, to let the process to ADC IRQ Handler.                   */
        __HAL_UNLOCK(hadc);
    }
    else
    {
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
    }

    /* Return function status */
    return tmp_hal_status;
}

/**
 * @brief  Poll for conversion state.
 * @param  hadc: ADC handle
 * @param  State: the ADC conversion state from the ADC_SR register.
 * @param  Timeout: Timeout value in millisecond.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_ADC_PollForState(ADC_HandleTypeDef *hadc, ADC_CUR_STA State, uint16_t Timeout)
{
    volatile uint32_t tickstart = 0U;
    volatile uint32_t tickdelta = 0U;
    volatile uint32_t reg_sr;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

    /* Get tick count */
    tickstart = HAL_GetTick();

    /* Check selected event flag */
    do
    {
        reg_sr = hadc->Instance->SR;
        reg_sr >>= ADC_SR_CUR_STATE_Pos; /* get ADC_SR cur_state bit */

        if ((reg_sr & State) == State)
            break;

        /* Get tick count */
        tickdelta = HAL_GetTick() - tickstart;
        if ((Timeout == 0U) || tickdelta > Timeout)
        {
            /* Update ADC state machine to timeout */
            SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

            /* Process unlocked */
            __HAL_UNLOCK(hadc);

            return HAL_TIMEOUT;
        }

    } while (1);

    /* Return ADC state */
    return HAL_OK;
}

/**
 * @brief  Stop ADC conversion, disable interrution of
 *         end-of-conversion, disable ADC peripheral.
 * @param  hadc: ADC handle
 * @retval None
 */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *hadc)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

    /* Process locked */
    __HAL_LOCK(hadc);

    /* Stop potential conversion on going */
    /* Disable ADC peripheral */
    tmp_hal_status = ADC_ConversionStatus_Reset(hadc);

    /* stop ADC conversion */
    CLEAR_BIT(hadc->Instance->CR, ADC_CR_SEQ1_START);
    CLEAR_BIT(hadc->Instance->CR, ADC_CR_SEQ2_START);

    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
        /* Disable ADC */
        __HAL_ADC_DISABLE(hadc);

        /* Set ADC state */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY, HAL_ADC_STATE_READY);
    }

    /* Process unlocked */
    __HAL_UNLOCK(hadc);

    /* Return function status */
    return tmp_hal_status;
}
/**
 * @brief  HAL_ADC_IRQHandler
 * @param  hadc: ADC handle
 * @retval
 */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef *hadc)
{
    uint32_t regs_value = 0U;              
    /*if(hadc->Instance == ADC) */
    {
        /* Set ADC state */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_BUSY);
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
        regs_value = hadc->Instance->INTR;

        if (HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ2_1SEQ_STS) || HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ2_1CH_STS))
        {
            /* Set ADC state */
            SET_BIT(hadc->State, HAL_ADC_STATE_EOC);

            /* Conv Callback */
            HAL_ADC_Seq2_ConvCpltCallback(hadc);

            /* Clear the ADC ADC_INTR_SEQ2_1SEQ flag */
            __HAL_ADC_CLEAR_FLAG(hadc, ADC_INTR_SEQ2_1SEQ_STS | ADC_INTR_SEQ2_1CH_STS);
        }

        if (HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_1SEQ_STS) ||\
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_1CH_STS) ||\
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_ALL_STS))
        {
            /* Set ADC state */
            SET_BIT(hadc->State, HAL_ADC_STATE_EOC);

            /* Conv Callback */
            HAL_ADC_Seq1_ConvCpltCallback(hadc);

            /* Clear the ADC ADC_INTR_SEQ1_1SEQ flag */
            __HAL_ADC_CLEAR_FLAG(hadc, ADC_INTR_SEQ1_1SEQ_STS | ADC_INTR_SEQ1_1CH_STS | ADC_INTR_SEQ1_ALL_STS);
        }        

        /* ========== Check out of threshold flags ========== */
        if ((hadc->Init.Threshold_En == ENABLE)&&                   \
            (HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_AWD_LT_STS) ||\
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_AWD_HT_STS) || \
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ1_AWD_REG_STS) ||\
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ2_AWD_LT_STS) || \
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ2_AWD_HT_STS) ||\
            HAL_IS_BIT_SET(regs_value, ADC_INTR_SEQ2_AWD_REG_STS)))
        {
            /* Set ADC state */
            SET_BIT(hadc->State, HAL_ADC_STATE_OUTRANGE);

            /* Level out of window callback */
            HAL_ADC_LevelOutOfRangeCallback(hadc);

            /* Clear the ADC out of threshold flag */
            __HAL_ADC_CLEAR_FLAG(hadc, ADC_INTR_SEQ1_AWD_LT_STS | ADC_INTR_SEQ1_AWD_HT_STS | ADC_INTR_SEQ1_AWD_REG_STS |\
                                ADC_INTR_SEQ2_AWD_LT_STS | ADC_INTR_SEQ2_AWD_HT_STS | ADC_INTR_SEQ2_AWD_REG_STS);
        }
    }        
}

/**
 * @brief  HAL_ADC_GetState
 * @param  hadc: ADC handle
 * @retval State
 */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef *hadc)
{
    return hadc->State;
}

/**
 * @brief  HAL_ADC_GetState
 * @param  hadc: ADC handle
 * @retval State
 */
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
    return hadc->ErrorCode;
}

/**
 * @brief  Get the ADC divider value
 * @param  hadc: ADC handle
 * @retval ADC divider value
 */
uint32_t HAL_ADC_GetDiv(ADC_HandleTypeDef *hadc)
{
    return hadc->Init.ClkSel;
}

/**
 * @brief  Enable the selected ADC.
 * @note   Prerequisite condition to use this function: ADC must be disabled
 *         and clock must be enabled (done into HAL_ADC_Init()).
 * @param  hadc: ADC handle
 * @retval HAL status.
 */
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef *hadc)
{
    uint32_t tickstart = 0U;
    __IO uint32_t wait_loop_index = 0U;

    /* ADC enable and wait for ADC ready                                          */
    /* Timeout implemented to not be stuck if ADC cannot be enabled (possible   */
    /* causes: ADC clock not running, ...).                                     */

    /* Enable the Peripheral */
    __HAL_ADC_ENABLE(hadc);

    /* Delay for ADC stabilization time */
    /* Compute number of CPU cycles to wait for */
    wait_loop_index = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
    while (wait_loop_index != 0U)
    {
        wait_loop_index--;
    }

    tickstart = HAL_GetTick();

    /* Wait for ADC effectively enabled */
    // while(ADC_IS_ENABLE(hadc) == RESET)
    {
        if ((HAL_GetTick() - tickstart) > ADC_ENABLE_TIMEOUT)
        {
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

            __HAL_UNLOCK(hadc);

            return HAL_ERROR;
        }
    }
    return HAL_OK;
}

/**
 * @brief  Soft trigger the selected ADC.
 * @note   Prerequisite condition to use this function: ADC must be disabled
 *         and clock must be enabled (done into HAL_ADC_Init()).
 * @param  hadc: ADC handle
 * @retval HAL status.
 */
HAL_StatusTypeDef HAL_ADC_Soft_Trigger(ADC_HandleTypeDef *hadc, uint8_t seqindex)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;

    if (seqindex == ADC_SEQ1_DEF)
    {
        SET_BIT(hadc->Instance->CR, ADC_CR_SEQ1_START); /* soft trigger SEQ1 */
    }
    else if (seqindex == ADC_SEQ2_DEF)
    {
        SET_BIT(hadc->Instance->CR, ADC_CR_SEQ2_START); /* soft trigger SEQ2 */
    }

    return tmp_hal_status;
}

/**
 * @brief  Reset ADC conversion status and disable the selected ADC
 * @param  hadc: ADC handle
 * @retval HAL status.
 */
HAL_StatusTypeDef ADC_ConversionStatus_Reset(ADC_HandleTypeDef *hadc)
{
    uint32_t tickstart = 0U;

    /* Verification if ADC is not already disabled */
    if (ADC_IS_ENABLE(hadc) != RESET)
    {
        /* Disable the ADC peripheral */
        __HAL_ADC_DISABLE(hadc);

        tickstart = HAL_GetTick();

        SET_BIT(hadc->Instance->CR, ADC_CR_STATE_RST);

        /* Wait for ADC effectively disabled */
        while (ADC_IS_ENABLE(hadc) != RESET)
        {
            if ((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
            {
                SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}

/**
 * @brief  Deinitialize the ADC peripheral registers to their default reset
 *         values, with deinitialization of the ADC MSP.
 * @param  hadc: ADC handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc)
{
    HAL_StatusTypeDef tmp_hal_status = HAL_OK;

    if (hadc == NULL)
    {
        return HAL_ERROR;
    }

    assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);

    /* Stop potential conversion on going*/
    /* Disable ADC peripheral */
    tmp_hal_status = ADC_ConversionStatus_Reset(hadc);

    /* Configuration of ADC parameters if previous preliminary actions are      */
    /* correctly completed.                                                     */
    if (tmp_hal_status == HAL_OK)
    {
        /* ========== Hard reset ADC peripheral ========== */
        /* Performs a global reset of the entire ADC peripheral: ADC state is     */
        /* forced to a similar state after device power-on.                       */

        /* DeInit the low level hardware: GPIO, NVIC */
        HAL_ADC_MspDeInit(hadc);

        /* Set ADC error code to none */
        ADC_CLEAR_ERRORCODE(hadc);

        /* Set ADC state */
        hadc->State = HAL_ADC_STATE_RESET;
    }

    __HAL_UNLOCK(hadc);

    return tmp_hal_status;
}

/**
 * @brief DMA SPI transmit process complete callback.
 * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */

void ADC_DMACallbackCplt(DMA_HandleTypeDef *hdma)
{
    ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

    if (hdma->ChannelNum == hadc->hdmaSeq1->ChannelNum)
    {
        /* Conv Callback */
        HAL_ADC_Seq1_ConvCpltCallback(hadc);
        __HAL_ADC_CLEAR_FLAG(hadc, ADC_INTR_SEQ1_1CH_STS);
    }

    if (hadc->ErrorCode != HAL_ADC_ERROR_NONE)
    {
        HAL_ADC_ErrorCallback(hadc);
        return;
    }
}

/**
 * @brief  Initializes the ADC MSP.
 * @param  hadc: ADC handle
 * @retval None
 */
__weak void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_MspInit must be implemented in the user file.
     */
}

/**
 * @brief  DeInitializes the ADC MSP.
 * @param  hadc: ADC handle
 * @retval None
 */
__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_MspDeInit must be implemented in the user file.
     */
}

/**
 * @brief  Conversion complete callback in non blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ConvCpltCallback must be implemented in the user file.
     */
}

/**
 * @brief  Out of threshold callback in non blocking mode.
 * @param  hadc: ADC handle
 * @retval None
 */
__weak void HAL_ADC_LevelOutOfRangeCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_LevelOutOfRangeCallback must be implemented in the user file.
    */
}

/**
 * @brief  ADC error callback in non blocking mode
 *        (ADC conversion with interruption or transfer by DMA)
 * @param  hadc: ADC handle
 * @retval None
 */
__weak void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ErrorCallback must be implemented in the user file.
    */
}
/**
 * \brief       HAL_ADC_Seq1_ConvCpltCallback
 * \note        None
 * \param[in]   None
 * \param[out]  None
 * \retval      None
 */
__weak void HAL_ADC_Seq1_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ErrorCallback must be implemented in the user file.
    */
}
/**
 * \brief       HAL_ADC_Seq2_ConvCpltCallback
 * \note        None
 * \param[in]   None
 * \param[out]  None
 * \retval      None
 */
__weak void HAL_ADC_Seq2_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_ADC_ErrorCallback must be implemented in the user file.
    */
}

#endif /* HAL_ADC_MODULE_ENABLED */
