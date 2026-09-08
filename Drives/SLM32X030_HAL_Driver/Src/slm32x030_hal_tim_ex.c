/**
  ******************************************************************************
  * @file    slm32x030_hal_tim_ex.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024 
  * @brief   TIM HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Timer Extended peripheral:
  *           + Time HALL_T Sensor Interface Initialization
  *           + Time HALL_T Sensor Interface Start
  *           + Time Complementary signal bread and dead time configuration
  *           + Time Master and Slave synchronization configuration
  *           + Timer remapping capabilities configuration
  @verbatim
  ==============================================================================
                      ##### TIMER Extended features #####
  ==============================================================================
  [..]
    The Timer Extended features include:
    (#) Complementary outputs with programmable dead-time for :
        (++) Output Compare
        (++) PWM generation (Edge and Center-aligned Mode)
        (++) One-pulse mode output
    (#) Synchronization circuit to control the timer with external signals and to
        interconnect several timers together.
    (#) Break input to put the timer output signals in reset state or in a known state.
    (#) Supports incremental (quadrature) encoder and hall-sensor circuitry for
        positioning purposes

            ##### How to use this driver #####
  ==============================================================================
    [..]
     (#) Initialize the TIM low level resources by implementing the following functions
         depending from feature used :
           (++) Complementary Output Compare : HAL_TIM_OC_MspInit()
           (++) Complementary PWM generation : HAL_TIM_PWM_MspInit()
           (++) Complementary One-pulse mode output : HAL_TIM_OnePulse_MspInit()
           (++) HALL_T Sensor output : HAL_TIMEx_HallSensor_MspInit()

     (#) Initialize the TIM low level resources :
        (##) Enable the TIM interface clock using __HAL_RCMU_TIMx_CLK_ENABLE();
        (##) TIM pins configuration
            (+++) Enable the clock for the TIM GPIOs using the following function:
              __HAL_RCMU_GPIOx_CLK_ENABLE();
            (+++) Configure these TIM pins in Alternate function mode using HAL_GPIO_Init();

     (#) The external Clock can be configured, if needed (the default clock is the
         internal clock from the APBx), using the following function:
         HAL_TIM_ConfigClockSource, the clock configuration should be done before
         any start function.

     (#) Configure the TIM in the desired functioning mode using one of the
         initialization function of this driver:
          (++) HAL_TIMEx_HallSensor_Init and HAL_TIMEx_ConfigCommutationEvent: to use the
              Timer HALL_T Sensor Interface and the commutation event with the corresponding
              Interrupt and DMA request if needed (Note that One Timer is used to interface
             with the HALL_T sensor Interface and another Timer should be used to use
             the commutation event).

     (#) Activate the TIM peripheral using one of the start functions:
           (++) Complementary Output Compare : HAL_TIMEx_OCN_Start(), HAL_TIMEx_OCN_Start_DMA(), HAL_TIMEx_OCN_Start_IT()
           (++) Complementary PWM generation : HAL_TIMEx_PWMN_Start(), HAL_TIMEx_PWMN_Start_DMA(), HAL_TIMEx_PWMN_Start_IT()
           (++) Complementary One-pulse mode output : HAL_TIMEx_OnePulseN_Start(), HAL_TIMEx_OnePulseN_Start_IT()
           (++) HALL_T Sensor output : HAL_TIMEx_HallSensor_Start(), HAL_TIMEx_HallSensor_Start_DMA(), HAL_TIMEx_HallSensor_Start_IT().


  @endverbatim
  ******************************************************************************
 **/ 

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */
  
/** @defgroup TIMEx TIMEx
  * @brief TIM Extended HAL module driver
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/** @defgroup TIMEx_Private_Functions TIMEx Private Functions
  * @{
  */
// static void TIM_CCxNChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelNState);
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup TIMEx_Exported_Functions TIMEx Exported Functions
  * @{
  */


/** @defgroup TIMEx_Exported_Functions_Group1 Timer HALL_T Sensor functions
 *  @brief    Timer HALL_T Sensor functions
 *
@verbatim
  ==============================================================================
                      ##### Timer HALL_T Sensor functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure TIM HAL Sensor.
    (+) De-initialize TIM HAL Sensor.
    (+) Start the HALL_T Sensor Interface.
    (+) Stop the HALL_T Sensor Interface.
    (+) Start the HALL_T Sensor Interface and enable interrupts.
    (+) Stop the HALL_T Sensor Interface and disable interrupts.
    (+) Start the HALL_T Sensor Interface and enable DMA transfers.
    (+) Stop the HALL_T Sensor Interface and disable DMA transfers.

@endverbatim
  * @{
  */



void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx = 0U;
  uint32_t tmpccer = 0U;
  uint32_t tmpcr2 = 0U;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIMX_CCER_CC4E;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;

  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
//   tmpccmrx &= ~TIM_CCMR2_OC4M;
//   tmpccmrx &= ~TIM_CCMR2_CC4S;

  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8U);

  /* Reset the Output Polarity level */
  tmpccer &= ~TIMX_CCER_CC4P;
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 12U);

  if(IS_TIM_BREAK_INSTANCE(TIMx))
  {
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

   /* Reset the Output Compare IDLE State */
    tmpcr2 &= ~TIMX_CR2_OIS4;
    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 6);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR4 = OC_Config->Pulse;

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}


void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);


/**
  * @brief  Initializes the TIM HALL_T Sensor Interface and create the associated handle.
  * @param  htim : TIM Encoder Interface handle
  * @param  sConfig : TIM HALL_T Sensor configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef *htim, TIM_HallSensor_InitTypeDef* sConfig)
{
    TIM_OC_InitTypeDef OC_Config;

    /* Check the TIM handle allocation */
    if(htim == NULL)
    {
        return HAL_ERROR;
    }

    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));
    assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
    assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
    assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));
    assert_param(IS_TIM_IC_POLARITY(sConfig->IC1Polarity));
    assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));
    assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));

    if(htim->State == HAL_TIM_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        htim->Lock = HAL_UNLOCKED;

        /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
        HAL_TIMEx_HallSensor_MspInit(htim);
    }

    /* Set the TIM state */
    htim->State= HAL_TIM_STATE_BUSY;

    /* Configure the Time base in the Encoder Mode */
    TIM_Base_SetConfig(htim->Instance, &htim->Init);

    /* Configure the Channel 1 as Input Channel to interface with the three Outputs of the  HALL_T sensor */
    TIM_TI1_SetConfig(htim->Instance, sConfig->IC1Polarity, TIM_ICSELECTION_TRC, sConfig->IC1Filter);

    /* Set the IC1PSC value */
    htim->Instance->CCMR1 |= sConfig->IC1Prescaler;

    /* Enable the HALL_T sensor interface (XOR function of the three inputs) */
    htim->Instance->CR2 |= TIMX_CR2_TI1S;

    /* Select the TIM_TS_TI1F_ED signal as Input trigger for the TIM */
    htim->Instance->SMCR &= ~TIMX_SMCR_TS;
    htim->Instance->SMCR |= TIM_TS_TI1F_ED;

    /* Use the TIM_TS_TI1F_ED signal to reset the TIM counter each edge detection */
    htim->Instance->SMCR &= ~TIMX_SMCR_SMS;
    htim->Instance->SMCR |= TIM_SLAVEMODE_RESET;

    /* Program channel 2 in PWM 2 mode with the desired Commutation_Delay*/
    OC_Config.OCFastMode = TIM_OCFAST_DISABLE;
    OC_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
    OC_Config.OCMode = TIM_OCMODE_PWM2;
    OC_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    OC_Config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
    OC_Config.Pulse = sConfig->Commutation_Delay;

    TIM_OC2_SetConfig(htim->Instance, &OC_Config);

    /* Select OC2REF as trigger output on TRGO: write the MMS bits in the TIMx_CR2
    register to 101 */
    htim->Instance->CR2 &= ~TIMX_CR2_MIMS;
    htim->Instance->CR2 |= TIM_TRGO_OC2REF;

    /* Initialize the TIM state*/
    htim->State= HAL_TIM_STATE_READY;

    return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM HALL_T Sensor interface
  * @param  htim : TIM HALL_T Sensor handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_INSTANCE(htim->Instance));

    htim->State = HAL_TIM_STATE_BUSY;

    /* Disable the TIM Peripheral Clock */
    __HAL_TIM_DISABLE(htim);

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    HAL_TIMEx_HallSensor_MspDeInit(htim);

    /* Change TIM state */
    htim->State = HAL_TIM_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(htim);

    return HAL_OK;
}

/**
  * @brief  Initializes the TIM HALL_T Sensor MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htim);
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIMEx_HallSensor_MspInit could be implemented in the user file
    */
}

/**
  * @brief  DeInitializes TIM HALL_T Sensor MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef *htim)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htim);
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIMEx_HallSensor_MspDeInit could be implemented in the user file
    */
}

/**
  * @brief  Starts the TIM HALL_T Sensor Interface.
  * @param  htim : TIM HALL_T Sensor handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    /* Enable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

    /* Enable the Peripheral */
    __HAL_TIM_ENABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Stops the TIM HALL_T sensor Interface.
  * @param  htim : TIM HALL_T Sensor handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    /* Disable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

    /* Disable the Peripheral */
    __HAL_TIM_DISABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Starts the TIM HALL_T Sensor Interface in interrupt mode.
  * @param  htim : TIM HALL_T Sensor handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    /* Enable the capture compare Interrupts 1 event */
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

    /* Enable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

    /* Enable the Peripheral */
    __HAL_TIM_ENABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Stops the TIM HALL_T Sensor Interface in interrupt mode.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    /* Disable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

    /* Disable the capture compare Interrupts event */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

    /* Disable the Peripheral */
    __HAL_TIM_DISABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Starts the TIM HALL_T Sensor Interface in DMA mode.
  * @param  htim : TIM HALL_T Sensor handle
  * @param  pData : The destination Buffer address.
  * @param  Length : The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    if((htim->State == HAL_TIM_STATE_BUSY))
    {
        return HAL_BUSY;
    }
    else if((htim->State == HAL_TIM_STATE_READY))
    {
        if(((uint32_t)pData == 0U) && (Length > 0U))
        {
            return HAL_ERROR;
        }
        else
        {
            htim->State = HAL_TIM_STATE_BUSY;
        }
    }
    else
    {
        /* */
    }

    /* Enable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

    /* Set the DMA Input Capture 1 Callback */
    htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

    /* Enable the DMA channel for Capture 1*/
    HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData, Length);

    /* Enable the capture compare 1 Interrupt */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);

    /* Enable the Peripheral */
    __HAL_TIM_ENABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Stops the TIM HALL_T Sensor Interface in DMA mode.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef *htim)
{
    /* Check the parameters */
    assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));

    /* Disable the Input Capture channel 1
    (in the HALL_T Sensor Interface the 3 possible channels that are used are TIM_CHANNEL_1, TIM_CHANNEL_2 and TIM_CHANNEL_3) */
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);


    /* Disable the capture compare Interrupts 1 event */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);

    /* Disable the Peripheral */
    __HAL_TIM_DISABLE(htim);

    /* Return function status */
    return HAL_OK;
}

/**
  * @}
  */




/** @defgroup TIMEx_Exported_Functions_Group2 Timer Complementary Output Compare functions
 *  @brief    Timer Complementary Output Compare functions
 *
@verbatim
  ==============================================================================
              ##### Timer Complementary Output Compare functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary Output Compare/PWM.
    (+) Stop the Complementary Output Compare/PWM.
    (+) Start the Complementary Output Compare/PWM and enable interrupts.
    (+) Stop the Complementary Output Compare/PWM and disable interrupts.
    (+) Start the Complementary Output Compare/PWM and enable DMA transfers.
    (+) Stop the Complementary Output Compare/PWM and disable DMA transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the TIM Output Compare signal generation on the complementary
  *         output.
  * @param  htim : TIM Output Compare handle
  * @param  Channel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation on the complementary
  *         output.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Output Compare signal generation in interrupt mode
  *         on the complementary output.
  * @param  htim : TIM OC handle
  * @param  Channel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Enable the TIM Output Compare interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    default:
    break;
  }

  /* Enable the TIM Break interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation in interrupt mode
  *         on the complementary output.
  * @param  htim : TIM Output Compare handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpccer = 0U;

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Output Compare interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    default:
    break;
  }

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the TIM Break interrupt (only if no more channel is active) */
  tmpccer = htim->Instance->CCER;
  if ((tmpccer & (TIMX_CCER_CC1NE | TIMX_CCER_CC2NE | TIMX_CCER_CC3NE)) == RESET)
  {
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);
  }

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Output Compare signal generation in DMA mode
  *         on the complementary output.
  * @param  htim : TIM Output Compare handle
  * @param  Channel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @param  pData : The source Buffer address.
  * @param  Length : The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if(((uint32_t)pData == 0U) && (Length > 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length);

      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);

      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,Length);

      /* Enable the TIM Output Compare DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    default:
    break;
  }

  /* Enable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation in DMA mode
  *         on the complementary output.
  * @param  htim : TIM Output Compare handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Output Compare DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    default:
    break;
  }

  /* Disable the Capture compare channel N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group3 Timer Complementary PWM functions
 *  @brief    Timer Complementary PWM functions
 *
@verbatim
  ==============================================================================
                 ##### Timer Complementary PWM functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary PWM.
    (+) Stop the Complementary PWM.
    (+) Start the Complementary PWM and enable interrupts.
    (+) Stop the Complementary PWM and disable interrupts.
    (+) Start the Complementary PWM and enable DMA transfers.
    (+) Stop the Complementary PWM and disable DMA transfers.
    (+) Start the Complementary Input Capture measurement.
    (+) Stop the Complementary Input Capture.
    (+) Start the Complementary Input Capture and enable interrupts.
    (+) Stop the Complementary Input Capture and disable interrupts.
    (+) Start the Complementary Input Capture and enable DMA transfers.
    (+) Stop the Complementary Input Capture and disable DMA transfers.
    (+) Start the Complementary One Pulse generation.
    (+) Stop the Complementary One Pulse.
    (+) Start the Complementary One Pulse and enable interrupts.
    (+) Stop the Complementary One Pulse and disable interrupts.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the PWM signal generation on the complementary output.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the PWM signal generation on the complementary output.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* Disable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the PWM signal generation in interrupt mode on the
  *         complementary output.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    default:
    break;
  }

  /* Enable the TIM Break interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the PWM signal generation in interrupt mode on the
  *         complementary output.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpccer = 0U;

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    default:
    break;
  }

  /* Disable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the TIM Break interrupt (only if no more channel is active) */
  tmpccer = htim->Instance->CCER;
  if ((tmpccer & (TIMX_CCER_CC1NE | TIMX_CCER_CC2NE | TIMX_CCER_CC3NE)) == RESET)
  {
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);
  }

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM PWM signal generation in DMA mode on the
  *         complementary output
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @param  pData : The source Buffer address.
  * @param  Length : The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if(((uint32_t)pData == 0U) && (Length > 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length);

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);

      /* Enable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,Length);

      /* Enable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    default:
    break;
  }

  /* Enable the complementary PWM output  */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM PWM signal generation in DMA mode on the complementary
  *         output
  * @param  htim : TIM handle
  * @param  Channel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    default:
    break;
  }

  /* Disable the complementary PWM output */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group4 Timer Complementary One Pulse functions
 *  @brief    Timer Complementary One Pulse functions
 *
@verbatim
  ==============================================================================
                ##### Timer Complementary One Pulse functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Start the Complementary One Pulse generation.
    (+) Stop the Complementary One Pulse.
    (+) Start the Complementary One Pulse and enable interrupts.
    (+) Stop the Complementary One Pulse and disable interrupts.

@endverbatim
  * @{
  */

/**
  * @brief  Starts the TIM One Pulse signal generation on the complemetary
  *         output.
  * @param  htim : TIM One Pulse handle
  * @param  OutputChannel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* Enable the complementary One Pulse output */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM One Pulse signal generation on the complementary
  *         output.
  * @param  htim : TIM One Pulse handle
  * @param  OutputChannel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{

  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* Disable the complementary One Pulse output */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM One Pulse signal generation in interrupt mode on the
  *         complementary channel.
  * @param  htim : TIM One Pulse handle
  * @param  OutputChannel : TIM Channel to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* Enable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

  /* Enable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);

  /* Enable the complementary One Pulse output */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);

  /* Enable the Main Ouput */
  __HAL_TIM_MOE_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM One Pulse signal generation in interrupt mode on the
  *         complementary channel.
  * @param  htim : TIM One Pulse handle
  * @param  OutputChannel : TIM Channel to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* Disable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

  /* Disable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);

  /* Disable the complementary One Pulse output */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);

  /* Disable the Main Ouput */
  __HAL_TIM_MOE_DISABLE(htim);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */



/** @defgroup TIMEx_Exported_Functions_Group5 Peripheral Control functions
 *  @brief    Peripheral Control functions
 *
@verbatim
  ==============================================================================
                    ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Configure the commutation event in case of use of the HALL_T sensor interface.
      (+) Configure Complementary channels, break features and dead time.
      (+) Configure Master synchronization.

@endverbatim
  * @{
  */



/**
  * @brief  Configure the TIM commutation event sequence.
  * @note: this function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in HALL_T sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @param  htim : TIM handle
  * @param  InputTrigger : the Internal trigger corresponding to the Timer Interfacing with the HALL_T sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource : the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent(TIM_HandleTypeDef *htim, uint32_t  InputTrigger, uint32_t  CommutationSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIMX_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIMX_CR2_CCPC;
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIMX_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Configure the TIM commutation event sequence with interrupt.
  * @note: this function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in HALL_T sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @param  htim : TIM handle
  * @param  InputTrigger : the Internal trigger corresponding to the Timer Interfacing with the HALL_T sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource : the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent_IT(TIM_HandleTypeDef *htim, uint32_t  InputTrigger, uint32_t  CommutationSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIMX_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIMX_CR2_CCPC;
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIMX_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  /* Enable the Commutation Interrupt Request */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_COM);

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Configure the TIM commutation event sequence with DMA.
  * @note: this function is mandatory to use the commutation event in order to
  *        update the configuration at each commutation detection on the TRGI input of the Timer,
  *        the typical use of this feature is with the use of another Timer(interface Timer)
  *        configured in HALL_T sensor interface, this interface Timer will generate the
  *        commutation at its TRGO output (connected to Timer used in this function) each time
  *        the TI1 of the Interface Timer detect a commutation at its input TI1.
  * @note: The user should configure the DMA in his own software, in This function only the COMDE bit is set
  * @param  htim : TIM handle
  * @param  InputTrigger : the Internal trigger corresponding to the Timer Interfacing with the HALL_T sensor
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal trigger 0 selected
  *            @arg TIM_TS_ITR1: Internal trigger 1 selected
  *            @arg TIM_TS_ITR2: Internal trigger 2 selected
  *            @arg TIM_TS_ITR3: Internal trigger 3 selected
  *            @arg TIM_TS_NONE: No trigger is needed
  * @param  CommutationSource : the Commutation Event source
  *          This parameter can be one of the following values:
  *            @arg TIM_COMMUTATION_TRGI: Commutation source is the TRGI of the Interface Timer
  *            @arg TIM_COMMUTATION_SOFTWARE:  Commutation source is set by software using the COMG bit
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutationEvent_DMA(TIM_HandleTypeDef *htim, uint32_t  InputTrigger, uint32_t  CommutationSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* Select the Input trigger */
    htim->Instance->SMCR &= ~TIMX_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* Select the Capture Compare preload feature */
  htim->Instance->CR2 |= TIMX_CR2_CCPC;
  /* Select the Commutation event source */
  htim->Instance->CR2 &= ~TIMX_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  /* Enable the Commutation DMA Request */
  /* Set the DMA Commutation Callback */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback = TIMEx_DMACommutationCplt;
  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError;

  /* Enable the Commutation DMA Request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_COM);

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief   Configures the Break feature, dead time, Lock level, OSSI/OSSR State
  *          and the AOE(automatic output enable).
  * @param  htim : TIM handle
  * @param  sBreakDeadTimeConfig : pointer to a TIM_ConfigBreakDeadConfigTypeDef structure that
  *         contains the BDTR Register configuration  information for the TIM peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *htim,
                                                TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig)
{
  uint32_t tmpbdtr = 0U;

  /* Check the parameters */
  assert_param(IS_TIM_BREAK_INSTANCE(htim->Instance));
  assert_param(IS_TIM_OSSR_STATE(sBreakDeadTimeConfig->OffStateRunMode));
  assert_param(IS_TIM_OSSI_STATE(sBreakDeadTimeConfig->OffStateIDLEMode));
  assert_param(IS_TIM_LOCK_LEVEL(sBreakDeadTimeConfig->LockLevel));
  assert_param(IS_TIM_DEADTIME(sBreakDeadTimeConfig->DeadTime));
  assert_param(IS_TIM_BREAK_STATE(sBreakDeadTimeConfig->BreakState));
  assert_param(IS_TIM_BREAK_POLARITY(sBreakDeadTimeConfig->BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(sBreakDeadTimeConfig->AutomaticOutput));

  /* Process Locked */
  __HAL_LOCK(htim);

  /* Set the Lock level, the Break enable Bit and the Polarity, the OSSR State,
     the OSSI State, the dead time value and the Automatic Output Enable Bit */

  /* Set the BDTR bits */
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_DTG, sBreakDeadTimeConfig->DeadTime);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_LOCK, sBreakDeadTimeConfig->LockLevel);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_OSSI, sBreakDeadTimeConfig->OffStateIDLEMode);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_OSSR, sBreakDeadTimeConfig->OffStateRunMode);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_BKE, sBreakDeadTimeConfig->BreakState);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_BKP, sBreakDeadTimeConfig->BreakPolarity);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_AOE, sBreakDeadTimeConfig->AutomaticOutput);
  MODIFY_REG(tmpbdtr, TIMX_BDTR4_MOE, sBreakDeadTimeConfig->AutomaticOutput);

  /* Set TIMx_BDTR */
  htim->Instance->BDTR = tmpbdtr;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Configures the TIM in master mode.
  * @param  htim : TIM handle.
  * @param  sMasterConfig : pointer to a TIM_MasterConfigTypeDef structure that
  *         contains the selected trigger output (TRGO) and the Master/Slave
  *         mode.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim, TIM_MasterConfigTypeDef * sMasterConfig)
{
  /* Check the parameters */
  assert_param(IS_TIM_MASTER_INSTANCE(htim->Instance));
  assert_param(IS_TIM_TRGO_SOURCE(sMasterConfig->MasterOutputTrigger));
  assert_param(IS_TIM_MSM_STATE(sMasterConfig->MasterSlaveMode));

  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  /* Reset the MMS Bits */
  htim->Instance->CR2 &= ~TIMX_CR2_MIMS;
  /* Select the TRGO source */
  htim->Instance->CR2 |=  sMasterConfig->MasterOutputTrigger;

  /* Reset the MSM Bit */
  htim->Instance->SMCR &= ~TIMX_SMCR_MSM;
  /* Set or Reset the MSM Bit */
  htim->Instance->SMCR |= sMasterConfig->MasterSlaveMode;

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group6 Extension Callbacks functions
 *  @brief   Extension Callbacks functions
 *
@verbatim
  ==============================================================================
                    ##### Extension Callbacks functions #####
  ==============================================================================
  [..]
    This section provides Extension TIM callback functions:
    (+) Timer Commutation callback
    (+) Timer Break callback

@endverbatim
  * @{
  */

/**
  * @brief  HALL_T commutation changed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIMEx_CommutationCallback could be implemented in the user file
   */
}

/**
  * @brief  HALL_T Break detection callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIMEx_BreakCallback could be implemented in the user file
   */
}

/**
  * @brief  TIM DMA Commutation callback.
  * @param  hdma : pointer to DMA handle.
  * @retval None
  */
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  htim->State= HAL_TIM_STATE_READY;

  HAL_TIMEx_CommutationCallback(htim);
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group7 Extension Peripheral State functions
 *  @brief   Extension Peripheral State functions
 *
@verbatim
  ==============================================================================
                ##### Extension Peripheral State functions #####
  ==============================================================================
  [..]
    This subsection permit to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the TIM HALL_T Sensor interface state
  * @param  htim : TIM HALL_T Sensor handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup TIMEx_Private_Functions
  * @{
  */

/**
  * @brief  Enables or disables the TIM Capture Compare Channel xN.
  * @param  TIMx  to select the TIM peripheral
  * @param  Channel : specifies the TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_Channel_1: TIM Channel 1
  *            @arg TIM_Channel_2: TIM Channel 2
  *            @arg TIM_Channel_3: TIM Channel 3
  * @param  ChannelNState : specifies the TIM Channel CCxNE bit new state.
  *          This parameter can be: TIM_CCxN_ENABLE or TIM_CCxN_Disable.
  * @retval None
  */
void TIM_CCxNChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelNState)
{
  uint32_t tmp = 0U;

  tmp = TIMX_CCER_CC1NE << Channel;

  /* Reset the CCxNE Bit */
  TIMx->CCER &=  ~tmp;

  /* Set or reset the CCxNE Bit */
  TIMx->CCER |=  (uint32_t)(ChannelNState << Channel);
}

/**
  * @}
  */

                           

#endif /* HAL_TIM_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

