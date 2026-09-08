/**
  ******************************************************************************
  * @file    slm32x030_hal_tim.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of TIM HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_TIM_H_
#define _SLM32X030_HAL_TIM_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup TIM
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup TIM_Exported_Types TIM Exported Types
  * @{
  */
/**
  * @brief  TIM Time base Configuration Structure definition.
  */
typedef struct
{
    uint32_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                    This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */
    uint32_t CounterMode;       /*!< Specifies the counter mode.
                                    This parameter can be a value of @ref TIM_Counter_Mode */
    uint32_t Period;            /*!< Specifies the period value to be loaded into the active
                                    Auto-Reload Register at the next update event.
                                    This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.  */
    uint32_t ClockDivision;     /*!< Specifies the clock division.
                                    This parameter can be a value of @ref TIM_ClockDivision */
    uint32_t RepetitionCounter; /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                    reaches zero, an update event is generated and counting restarts
                                    from the RCR value (N).
                                    This means in PWM mode that (N+1) corresponds to:
                                        - the number of PWM periods in edge-aligned mode
                                        - the number of half PWM period in center-aligned mode
                                        This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.
                                        @note This parameter is valid only for TIM1 and TIM8. */                                    
    uint32_t AutoReloadPreload; /*!< Specifies the auto-reload preload.
                                    This parameter can be a value of @ref TIM_AutoReloadPreload */
} TIM_Base_InitTypeDef;

/**
  * @brief  TIM Output Compare Configuration Structure definition.
  */
typedef struct
{
    uint32_t OCMode;          /*!< Specifies the TIM mode.
                                This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */
    uint32_t Pulse;           /*!< Specifies the pulse value to be loaded into the Capture Compare Register.
                                This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */
    uint32_t OCPolarity;      /*!< Specifies the output polarity.
                                This parameter can be a value of @ref TIM_Output_Compare_Polarity */
    uint32_t OCNPolarity;     /*!< Specifies the complementary output polarity.
                                This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
                                @note This parameter is valid only for TIM1 and TIM8. */
    uint32_t OCFastMode;      /*!< Specifies the Fast mode state.
                                This parameter can be a value of @ref TIM_Output_Fast_State
                                @note This parameter is valid only in PWM1 and PWM2 mode. */
    uint32_t OCIdleState;     /*!< Specifies the TIM Output Compare pin state during Idle state.
                                This parameter can be a value of @ref TIM_Output_Compare_Idle_State
                                @note This parameter is valid only for TIM1 and TIM8. */
    uint32_t OCNIdleState;    /*!< Specifies the TIM Output Compare pin state during Idle state.
                                This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
                                @note This parameter is valid only for TIM1 and TIM8. */
} TIM_OC_InitTypeDef;

/**
  * @brief  TIM One Pulse Mode Configuration Structure definition.
  */
typedef struct
{
    uint32_t OCMode;        /*!< Specifies the TIM mode.
                                This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */
    uint32_t Pulse;         /*!< Specifies the pulse value to be loaded into the Capture Compare Register.
                                This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */
    uint32_t OCPolarity;    /*!< Specifies the output polarity.
                                This parameter can be a value of @ref TIM_Output_Compare_Polarity */
    uint32_t OCNPolarity;   /*!< Specifies the complementary output polarity.
                                This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
                                @note This parameter is valid only for TIM1 and TIM8. */
    uint32_t OCIdleState;   /*!< Specifies the TIM Output Compare pin state during Idle state.
                                This parameter can be a value of @ref TIM_Output_Compare_Idle_State
                                @note This parameter is valid only for TIM1 and TIM8. */
    uint32_t OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during Idle state.
                                This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
                                @note This parameter is valid only for TIM1 and TIM8. */
    uint32_t ICPolarity;    /*!< Specifies the active edge of the input signal.
                                This parameter can be a value of @ref TIM_Input_Capture_Polarity */
    uint32_t ICSelection;   /*!< Specifies the input.
                                This parameter can be a value of @ref TIM_Input_Capture_Selection */
    uint32_t ICFilter;      /*!< Specifies the input capture filter.
                                This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
} TIM_OnePulse_InitTypeDef;


/**
  * @brief  TIM Input Capture Configuration Structure definition.
  */
typedef struct
{
    uint32_t  ICPolarity;   /*!< Specifies the active edge of the input signal.
                                This parameter can be a value of @ref TIM_Input_Capture_Polarity */
    uint32_t ICSelection;   /*!< Specifies the input.
                                This parameter can be a value of @ref TIM_Input_Capture_Selection */
    uint32_t ICPrescaler;   /*!< Specifies the Input Capture Prescaler.
                                This parameter can be a value of @ref TIM_Input_Capture_Prescaler */
    uint32_t ICFilter;      /*!< Specifies the input capture filter.
                                This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
} TIM_IC_InitTypeDef;

/**
  * @brief  TIM Encoder Configuration Structure definition
  */
typedef struct
{
    uint32_t EncoderMode;   /*!< Specifies the active edge of the input signal.
                                This parameter can be a value of @ref TIM_Encoder_Mode */
    uint32_t IC1Polarity;   /*!< Specifies the active edge of the input signal.
                                This parameter can be a value of @ref TIM_Input_Capture_Polarity */
    uint32_t IC1Selection;  /*!< Specifies the input.
                                This parameter can be a value of @ref TIM_Input_Capture_Selection */
    uint32_t IC1Prescaler;  /*!< Specifies the Input Capture Prescaler.
                                This parameter can be a value of @ref TIM_Input_Capture_Prescaler */
    uint32_t IC1Filter;     /*!< Specifies the input capture filter.
                                This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
    uint32_t IC2Polarity;   /*!< Specifies the active edge of the input signal.
                                This parameter can be a value of @ref TIM_Input_Capture_Polarity */
    uint32_t IC2Selection;  /*!< Specifies the input.
                                This parameter can be a value of @ref TIM_Input_Capture_Selection */
    uint32_t IC2Prescaler;  /*!< Specifies the Input Capture Prescaler.
                                This parameter can be a value of @ref TIM_Input_Capture_Prescaler */
    uint32_t IC2Filter;     /*!< Specifies the input capture filter.
                                This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
} TIM_Encoder_InitTypeDef;

/**
  * @brief  TIM Clock Configuration Handle Structure definition
  */
typedef struct
{
    uint32_t ClockSource;       /*!< TIM clock sources
                                    This parameter can be a value of @ref TIM_Clock_Source */
    uint32_t ClockPolarity;     /*!< TIM clock polarity
                                    This parameter can be a value of @ref TIM_Clock_Polarity */
    uint32_t ClockPrescaler;    /*!< TIM clock prescaler
                                    This parameter can be a value of @ref TIM_Clock_Prescaler */
    uint32_t ClockFilter;       /*!< TIM clock filter
                                    This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
}TIM_ClockConfigTypeDef;

/**
  * @brief  TIM Clear Input Configuration Handle Structure definition
  */
typedef struct
{
    uint32_t ClearInputState;     /*!< TIM clear Input state
                                    This parameter can be ENABLE or DISABLE */
    uint32_t ClearInputSource;    /*!< TIM clear Input sources
                                    This parameter can be a value of @ref TIM_ClearInput_Source */
    uint32_t ClearInputPolarity;  /*!< TIM Clear Input polarity
                                    This parameter can be a value of @ref TIM_ClearInput_Polarity */
    uint32_t ClearInputPrescaler; /*!< TIM Clear Input prescaler
                                    This parameter can be a value of @ref TIM_ClearInput_Prescaler */
    uint32_t ClearInputFilter;    /*!< TIM Clear Input filter
                                    This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
}TIM_ClearInputConfigTypeDef;

/**
  * @brief  TIM Slave configuration Structure definition
  */
typedef struct 
{
    uint32_t  SlaveMode;          /*!< Slave mode selection
                                    This parameter can be a value of @ref TIM_Slave_Mode */
    uint32_t  InputTrigger;       /*!< Input Trigger source
                                    This parameter can be a value of @ref TIM_Trigger_Selection */
    uint32_t  TriggerPolarity;    /*!< Input Trigger polarity
                                    This parameter can be a value of @ref TIM_Trigger_Polarity */
    uint32_t  TriggerPrescaler;   /*!< Input trigger prescaler
                                    This parameter can be a value of @ref TIM_Trigger_Prescaler */
    uint32_t  TriggerFilter;      /*!< Input trigger filter
                                    This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
}TIM_SlaveConfigTypeDef;

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_TIM_STATE_RESET             = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
    HAL_TIM_STATE_READY             = 0x01U,    /*!< Peripheral Initialized and ready for use    */
    HAL_TIM_STATE_BUSY              = 0x02U,    /*!< An internal process is ongoing              */
    HAL_TIM_STATE_TIMEOUT           = 0x03U,    /*!< Timeout state                               */
    HAL_TIM_STATE_ERROR             = 0x04U     /*!< Reception process is ongoing                */
}HAL_TIM_StateTypeDef;

/**
  * @brief  HAL Active channel structures definition
  */
typedef enum
{
    HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,    /*!< The active channel is 1     */
    HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,    /*!< The active channel is 2     */
    HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,    /*!< The active channel is 3     */
    HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,    /*!< The active channel is 4     */
    HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00U     /*!< All active channels cleared */
}HAL_TIM_ActiveChannel;

/**
  * @brief  TIM Time Base Handle Structure definition
  */
typedef struct
{
    TIM_TypeDef                 *Instance;     /*!< Register base address             */
    TIM_Base_InitTypeDef        Init;          /*!< TIM Time Base required parameters */
    HAL_TIM_ActiveChannel       Channel;       /*!< Active channel                    */
    DMA_HandleTypeDef           *hdma[7U];     /*!< DMA Handlers array
                                                    This array is accessed by a @ref TIM_DMA_Handle_index */
    HAL_LockTypeDef             Lock;          /*!< Locking object                    */
    __IO HAL_TIM_StateTypeDef   State;         /*!< TIM operation state               */
}TIM_HandleTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup TIM_Exported_Constants TIM Exported Constants
  * @{
  */

/** @defgroup TIM_Input_Channel_Polarity TIM Input Channel Polarity
  * @{
  */
#define  TIM_INPUTCHANNELPOLARITY_RISING      (0x00000000U)                       /*!< Polarity for TIx source */
#define  TIM_INPUTCHANNELPOLARITY_FALLING     (TIMX_CCER_CC1P)                   /*!< Polarity for TIx source */
#define  TIM_INPUTCHANNELPOLARITY_BOTHEDGE    (TIMX_CCER_CC1P | TIMX_CCER_CC1NP)  /*!< Polarity for TIx source */
/**
  * @}
  */

/** @defgroup TIM_ETR_Polarity TIM ETR Polarity
  * @{
  */
#define TIM_ETRPOLARITY_INVERTED              (TIMX_SMCR_ETP)                    /*!< Polarity for ETR source */
#define TIM_ETRPOLARITY_NONINVERTED           (0x00000000U)                       /*!< Polarity for ETR source */
/**
  * @}
  */

/** @defgroup TIM_ETR_Prescaler TIM ETR Prescaler
  * @{
  */
#define TIM_ETRPRESCALER_DIV1                 (0x00000000U)                       /*!< No prescaler is used */
#define TIM_ETRPRESCALER_DIV2                 (TIM_SMCR_ETPS_0)                 /*!< ETR input source is divided by 2 */
#define TIM_ETRPRESCALER_DIV4                 (TIM_SMCR_ETPS_1)                 /*!< ETR input source is divided by 4 */
#define TIM_ETRPRESCALER_DIV8                 (TIMX_SMCR_ETPS)                   /*!< ETR input source is divided by 8 */
/**
  * @}
  */

/** @defgroup TIM_Counter_Mode TIM Counter Mode
  * @{
  */
#define TIMX_CR1_CMS_0                         (0x1UL << TIMX_CR1_CMS_Pos)
#define TIMX_CR1_CMS_1                         (0x2UL << TIMX_CR1_CMS_Pos)

#define TIM_COUNTERMODE_UP                     (0x00000000U)
#define TIM_COUNTERMODE_DOWN                   TIMX_CR1_DIR
#define TIM_COUNTERMODE_CENTERALIGNED1         TIMX_CR1_CMS_0
#define TIM_COUNTERMODE_CENTERALIGNED2         TIMX_CR1_CMS_1
#define TIM_COUNTERMODE_CENTERALIGNED3         TIMX_CR1_CMS
#define TIM_ONE_PULSE_MODE                     TIMX_CR1_OPM
/**
  * @}
  */

#define TIMX_CR1_CKD_0                          (0x1UL << TIMX_CR1_CKD_Pos)
#define TIMX_CR1_CKD_1                          (0x2UL << TIMX_CR1_CKD_Pos)
/** @defgroup TIM_ClockDivision TIM ClockDivision
  * @{
  */
#define TIM_CLOCKDIVISION_DIV1                   (0x00000000U)
#define TIM_CLOCKDIVISION_DIV2                   (TIMX_CR1_CKD_0)
#define TIM_CLOCKDIVISION_DIV4                   (TIMX_CR1_CKD_1)
/**
  * @}
  */

/** @defgroup TIM_AutoReloadPreload TIM Auto-Reload Preload
  * @{
  */
#define TIM_AUTORELOAD_PRELOAD_DISABLE          (0x0000U)              /*!< TIMx_ARR register is not buffered */
#define TIM_AUTORELOAD_PRELOAD_ENABLE           (TIMX_CR1_ARPE)       /*!< TIMx_ARR register is buffered */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_and_PWM_modes TIM Output Compare and PWM modes
  * @{
  */


/** @defgroup TIM_Output_Compare_and_PWM_modes TIM Output Compare and PWM modes
  * @{
  */

#define TIM_CCMR1_OC1M_0                    (0x1UL << TIMX_CCMR1_OC1M_Pos)       /*!< 0x00000010 */
#define TIM_CCMR1_OC1M_1                    (0x2UL << TIMX_CCMR1_OC1M_Pos)       /*!< 0x00000020 */
#define TIM_CCMR1_OC1M_2                    (0x4UL << TIMX_CCMR1_OC1M_Pos)       /*!< 0x00000040 */
#define TIM_OCMODE_TIMING                   (0x00000000U)
#define TIM_OCMODE_ACTIVE                   (0x1UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_INACTIVE                 (0x2UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_TOGGLE                   (0x3UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_PWM1                     (0x6UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_PWM2                     (0x7UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_FORCED_ACTIVE            (0x5UL << TIMX_CCMR1_OC1M_Pos)
#define TIM_OCMODE_FORCED_INACTIVE          (0x4UL << TIMX_CCMR1_OC1M_Pos)

/**
  * @}
  */

/** @defgroup TIM_Output_Compare_State TIM Output Compare State
  * @{
  */
#define TIM_OUTPUTSTATE_DISABLE             (0x00000000U)
#define TIM_OUTPUTSTATE_ENABLE              (TIMX_CCER_CC1E)
/**
  * @}
  */

/** @defgroup TIM_Output_Fast_State TIM Output Fast State
  * @{
  */
#define TIM_OCFAST_DISABLE                 (0x00000000U)
#define TIM_OCFAST_ENABLE                  (TIMX_CCMR1_OC1FE)
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_State TIM Complementary Output Compare State
  * @{
  */
#define TIM_OUTPUTNSTATE_DISABLE            (0x00000000U)
#define TIM_OUTPUTNSTATE_ENABLE             (TIMX_CCER_CC1NE)
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_Polarity TIM Output Compare Polarity
  * @{
  */
#define TIM_OCPOLARITY_HIGH                (0x00000000U)
#define TIM_OCPOLARITY_LOW                 (TIMX_CCER_CC1P)
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_Polarity TIM Complementary Output Compare Polarity
  * @{
  */
#define TIM_OCNPOLARITY_HIGH               (0x00000000U)
#define TIM_OCNPOLARITY_LOW                (TIMX_CCER_CC1NP)
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_Idle_State TIM Output Compare Idle State
  * @{
  */
#define TIM_OCIDLESTATE_RESET              (0x00000000U)
#define TIM_OCIDLESTATE_SET                (TIMX_CR2_OIS1)
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_Idle_State TIM Complementary Output Compare Idle State
  * @{
  */
#define TIM_OCNIDLESTATE_RESET             (0x00000000U)
#define TIM_OCNIDLESTATE_SET               (TIMX_CR2_OIS1N)
/**
  * @}
  */

/** @defgroup TIM_Channel TIM Channel
  * @{
  */
#define TIM_CHANNEL_1                      (0x00000000U)
#define TIM_CHANNEL_2                      (0x00000004U)
#define TIM_CHANNEL_3                      (0x00000008U)
#define TIM_CHANNEL_4                      (0x0000000CU)
#define TIM_CHANNEL_ALL                    (0x00000018U)

/**
  * @}
  */

/** @defgroup TIM_Input_Capture_Polarity TIM Input Capture Polarity
  * @{
  */
#define  TIM_ICPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING      /*!< Capture triggered by rising edge on timer input */
#define  TIM_ICPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING     /*!< Capture triggered by falling edge on timer input */
#define  TIM_ICPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE    /*!< Capture triggered by both rising and falling edges on timer input */
/**
  * @}
  */

/** @defgroup TIM_Input_Capture_Selection TIM Input Capture Selection
  * @{
  */ 
#define TIM_CCMR1_CC1S_0                    (0x1UL << TIMX_CCMR1_CC1S_Pos)      /*!< 0x00000001 */
#define TIM_CCMR1_CC1S_1                    (0x2UL << TIMX_CCMR1_CC1S_Pos)      /*!< 0x00000002 */                    
#define TIM_ICSELECTION_DIRECTTI            TIM_CCMR1_CC1S_0                    /*!< TIM Input 1, 2, 3 or 4 is selected to be
                                                                                    connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_ICSELECTION_INDIRECTTI          TIM_CCMR1_CC1S_1                    /*!< TIM Input 1, 2, 3 or 4 is selected to be
                                                                                    connected to IC2, IC1, IC4 or IC3, respectively */
#define TIM_ICSELECTION_TRC                 TIMX_CCMR1_CC1S                     /*!< TIM Input 1, 2, 3 or 4 is selected to be connected to TRC */
/**
  * @}
  */
/** @defgroup TIM_Input_Capture_Prescaler TIM Input Capture Prescaler
  * @{
  */

 #define TIM_ICPSC_DIV1                     (0x00000000U)                        /*!< Capture performed each time an edge is detected on the capture input */
 #define TIM_ICPSC_DIV2                     (0x01UL << TIMX_CCMR1_IC1PS_Pos)     /*!< Capture performed once every 2 events */
 #define TIM_ICPSC_DIV4                     (0x02UL << TIMX_CCMR1_IC1PS_Pos)     /*!< Capture performed once every 4 events */
 #define TIM_ICPSC_DIV8                     (0x03UL << TIMX_CCMR1_IC1PS_Pos)     /*!< Capture performed once every 8 events */

/**
  * @}
  */

/** @defgroup TIM_One_Pulse_Mode TIM One Pulse Mode
  * @{
  */
#define TIM_OPMODE_SINGLE                  TIMX_CR1_OPM
#define TIM_OPMODE_REPETITIVE              (0x00000000U)
/**
  * @}
  */

/** @defgroup TIM_Encoder_Mode TIM Encoder Mode
  * @{
  */
#define TIM_SMCR_SMS_0                      (0x01UL << TIMX_SMCR_SMS_Pos)         /*!< 0x00000001 */
#define TIM_SMCR_SMS_1                      (0x02UL << TIMX_SMCR_SMS_Pos)         /*!< 0x00000002 */
#define TIM_SMCR_SMS_2                      (0x04UL << TIMX_SMCR_SMS_Pos)         /*!< 0x00000004 */ 
#define TIM_ENCODERMODE_TI1                 TIM_SMCR_SMS_0
#define TIM_ENCODERMODE_TI2                 TIM_SMCR_SMS_1
#define TIM_ENCODERMODE_TI12                (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0)

/**
  * @}
  */

/** @defgroup TIM_Interrupt_definition TIM Interrupt Definition
  * @{
  */
#define TIM_IT_UPDATE                       TIMX_DIER_UIE
#define TIM_IT_CC1                          TIMX_DIER_CC1IE
#define TIM_IT_CC2                          TIMX_DIER_CC2IE
#define TIM_IT_CC3                          TIMX_DIER_CC3IE
#define TIM_IT_CC4                          TIMX_DIER_CC4IE
#define TIM_IT_COM                          TIMX_DIER_COMIE
#define TIM_IT_TRIGGER                      TIMX_DIER_TIE
#define TIM_IT_BREAK                        TIMX_DIER_BIE

/**
  * @}
  */

/** @defgroup TIM_Commutation_Source  TIM Commutation Source
  * @{
  */
#define TIM_COMMUTATION_TRGI               TIMX_CR2_CCUS
#define TIM_COMMUTATION_SOFTWARE           (0x00000000U)

/**
  * @}
  */

/** @defgroup TIM_DMA_sources TIM DMA Sources
  * @{
  */
#define TIM_DMA_UPDATE                     TIMX_DIER_UDE
#define TIM_DMA_CC1                        TIMX_DIER_CC1DE
#define TIM_DMA_CC2                        TIMX_DIER_CC2DE
#define TIM_DMA_CC3                        TIMX_DIER_CC3DE
#define TIM_DMA_CC4                        TIMX_DIER_CC4DE
#define TIM_DMA_COM                        TIMX_DIER_COMDE
#define TIM_DMA_TRIGGER                    TIMX_DIER_TDE
/**
  * @}
  */

/** @defgroup TIM_Event_Source TIM Event Source
  * @{
  */
#define TIM_EVENTSOURCE_UPDATE              TIMX_EGR_UG
#define TIM_EVENTSOURCE_CC1                 TIMX_EGR_CC1G
#define TIM_EVENTSOURCE_CC2                 TIMX_EGR_CC2G
#define TIM_EVENTSOURCE_CC3                 TIMX_EGR_CC3G
#define TIM_EVENTSOURCE_CC4                 TIMX_EGR_CC4G
#define TIM_EVENTSOURCE_COM                 TIMX_EGR_COMG
#define TIM_EVENTSOURCE_TRIGGER             TIMX_EGR_TG
#define TIM_EVENTSOURCE_BREAK               TIMX_EGR_BG
/**
  * @}
  */

/** @defgroup TIM_Flag_definition TIM Flag Definition
  * @{
  */
#define TIM_FLAG_UPDATE                    TIMX_SR_UIF
#define TIM_FLAG_CC1                       TIMX_SR_CC1IF
#define TIM_FLAG_CC2                       TIMX_SR_CC2IF
#define TIM_FLAG_CC3                       TIMX_SR_CC3IF
#define TIM_FLAG_CC4                       TIMX_SR_CC4IF
#define TIM_FLAG_COM                       TIMX_SR_COMIF
#define TIM_FLAG_TRIGGER                   TIMX_SR_TIF
#define TIM_FLAG_BREAK                     TIMX_SR_BIF
#define TIM_FLAG_CC1OF                     TIMX_SR_CC1OF
#define TIM_FLAG_CC2OF                     TIMX_SR_CC2OF
#define TIM_FLAG_CC3OF                     TIMX_SR_CC3OF
#define TIM_FLAG_CC4OF                     TIMX_SR_CC4OF
/**
  * @}
  */

/** @defgroup TIM_Clock_Source TIM Clock Source
  * @{
  */
#define TIM_SMCR_ETPS_0                     (0x1UL << TIMX_SMCR_ETPS_Pos)        /*!< 0x00001000 */
#define TIM_SMCR_ETPS_1                     (0x2UL << TIMX_SMCR_ETPS_Pos)        /*!< 0x00002000 */ 
#define TIM_CLOCKSOURCE_ETRMODE2            TIM_SMCR_ETPS_1
#define TIM_CLOCKSOURCE_INTERNAL            TIM_SMCR_ETPS_0
#define TIM_CLOCKSOURCE_ITR0                (0x0000UL)

#define TIM_SMCR_TS_0                       (0x1UL << TIMX_SMCR_TS_Pos)          /*!< 0x00000010 */
#define TIM_SMCR_TS_1                       (0x2UL << TIMX_SMCR_TS_Pos)          /*!< 0x00000020 */
#define TIM_SMCR_TS_2                       (0x4UL << TIMX_SMCR_TS_Pos)          /*!< 0x00000040 */
#define TIM_CLOCKSOURCE_ITR1                TIM_SMCR_TS_0
#define TIM_CLOCKSOURCE_ITR2                TIM_SMCR_TS_1
#define TIM_CLOCKSOURCE_ITR3                (TIM_SMCR_TS_0 | TIM_SMCR_TS_1)
#define TIM_CLOCKSOURCE_TI1ED               TIM_SMCR_TS_2
#define TIM_CLOCKSOURCE_TI1                 (TIM_SMCR_TS_0 | TIM_SMCR_TS_2)
#define TIM_CLOCKSOURCE_TI2                 (TIM_SMCR_TS_1 | TIM_SMCR_TS_2)
#define TIM_CLOCKSOURCE_ETRMODE1            TIMX_SMCR_TS
/**
  * @}
  */

/** @defgroup TIM_Clock_Polarity TIM Clock Polarity
  * @{
  */
#define TIM_CLOCKPOLARITY_INVERTED           TIM_ETRPOLARITY_INVERTED           /*!< Polarity for ETRx clock sources */
#define TIM_CLOCKPOLARITY_NONINVERTED        TIM_ETRPOLARITY_NONINVERTED        /*!< Polarity for ETRx clock sources */
#define TIM_CLOCKPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING    /*!< Polarity for TIx clock sources */
#define TIM_CLOCKPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING   /*!< Polarity for TIx clock sources */
#define TIM_CLOCKPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE  /*!< Polarity for TIx clock sources */
/**
  * @}
  */

/** @defgroup TIM_Clock_Prescaler TIM Clock Prescaler
  * @{
  */
#define TIM_CLOCKPRESCALER_DIV1              TIM_ETRPRESCALER_DIV1     /*!< No prescaler is used */
#define TIM_CLOCKPRESCALER_DIV2              TIM_ETRPRESCALER_DIV2     /*!< Prescaler for External ETR Clock: Capture performed once every 2 events. */
#define TIM_CLOCKPRESCALER_DIV4              TIM_ETRPRESCALER_DIV4     /*!< Prescaler for External ETR Clock: Capture performed once every 4 events. */
#define TIM_CLOCKPRESCALER_DIV8              TIM_ETRPRESCALER_DIV8     /*!< Prescaler for External ETR Clock: Capture performed once every 8 events. */
/**
  * @}
  */

/** @defgroup TIM_ClearInput_Source TIM ClearInput Source
  * @{
  */
#define TIM_CLEARINPUTSOURCE_ETR              (0x00000001U)
#define TIM_CLEARINPUTSOURCE_NONE             (0x00000000U)
/**
  * @}
  */

/** @defgroup TIM_ClearInput_Polarity TIM Clear Input Polarity
  * @{
  */
#define TIM_CLEARINPUTPOLARITY_INVERTED                 TIM_ETRPOLARITY_INVERTED                    /*!< Polarity for ETRx pin */
#define TIM_CLEARINPUTPOLARITY_NONINVERTED              TIM_ETRPOLARITY_NONINVERTED                 /*!< Polarity for ETRx pin */
/**
  * @}
  */

/** @defgroup TIM_ClearInput_Prescaler TIM Clear Input Prescaler
  * @{
  */
#define TIM_CLEARINPUTPRESCALER_DIV1                    TIM_ETRPRESCALER_DIV1      /*!< No prescaler is used */
#define TIM_CLEARINPUTPRESCALER_DIV2                    TIM_ETRPRESCALER_DIV2      /*!< Prescaler for External ETR pin: Capture performed once every 2 events. */
#define TIM_CLEARINPUTPRESCALER_DIV4                    TIM_ETRPRESCALER_DIV4      /*!< Prescaler for External ETR pin: Capture performed once every 4 events. */
#define TIM_CLEARINPUTPRESCALER_DIV8                    TIM_ETRPRESCALER_DIV8        /*!< Prescaler for External ETR pin: Capture performed once every 8 events. */
/**
  * @}
  */

/** @defgroup TIM_OSSR_Off_State_Selection_for_Run_mode_state TIM OSSR Off State Selection for Run mode state
  * @{
  */
#define TIM_OSSR_DISABLE                                (0x00000000U)
#define TIM_OSSR_ENABLE                                 TIMX_BDTR4_OSSR
/**
  * @}
  */

/** @defgroup TIM_OSSI_Off_State_Selection_for_Idle_mode_state TIM OSSI Off State Selection for Idle mode state
  * @{
  */
#define TIM_OSSI_DISABLE                                (0x00000000U)
#define TIM_OSSI_ENABLE                                 TIMX_BDTR4_OSSI
/**
  * @}
  */

/** @defgroup TIM_Lock_level TIM Lock level
  * @{
  */
#define TIM_BDTR_LOCK_0                                 (0x01UL << TIMX_BDTR4_LOCK_Pos)        /*!< 0x00000100 */
#define TIM_BDTR_LOCK_1                                 (0x02UL << TIMX_BDTR4_LOCK_Pos)        /*!< 0x00000200 */
#define TIM_LOCKLEVEL_OFF                               (0x00UL << TIMX_BDTR4_LOCK_Pos)
#define TIM_LOCKLEVEL_1                                 (0x01UL << TIMX_BDTR4_LOCK_Pos)
#define TIM_LOCKLEVEL_2                                 (0x02UL << TIMX_BDTR4_LOCK_Pos)
#define TIM_LOCKLEVEL_3                                 (0x03UL << TIMX_BDTR4_LOCK_Pos)
/**
  * @}
  */

/** @defgroup TIM_Break_Input_enable_disable TIM Break Input Enable Disable
  * @{
  */
#define TIM_BREAK_DISABLE                               (0x00000000U)
#define TIM_BREAK_ENABLE                                TIMX_BDTR4_BKE
/**
  * @}
  */

/** @defgroup TIM_Break_Polarity TIM Break Input Polarity
  * @{
  */
#define TIM_BREAKPOLARITY_LOW                           (0x00000000U)
#define TIM_BREAKPOLARITY_HIGH                          TIMX_BDTR4_BKP
/**
  * @}
  */
/** @defgroup TIM_AOE_Bit_Set_Reset TIM Automatic Output Enable
  * @{
  */
#define TIM_AUTOMATICOUTPUT_DISABLE                     (0x00000000U)
#define TIM_AUTOMATICOUTPUT_ENABLE                      TIMX_BDTR4_AOE
/**
  * @}
  */

/** @defgroup TIM_Master_Mode_Selection TIM Master Mode Selection
  * @{
  */
#define TIM_CR2_MMS_0                                   (0x01UL << TIMX_CR2_MIMS_Pos)          /*!<  */
#define TIM_CR2_MMS_1                                   (0x02UL << TIMX_CR2_MIMS_Pos)          /*!<  */
#define TIM_CR2_MMS_2                                   (0x03UL << TIMX_CR2_MIMS_Pos)          /*!<  */
#define TIM_CR2_MMS_3                                   (0x04UL << TIMX_CR2_MIMS_Pos)          /*!<  */

#define TIM_TRGO_RESET                                  (0x00UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_ENABLE                                 (0x01UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_UPDATE                                 (0x02UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_OC1                                    (0x03UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_OC1REF                                 (0x04UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_OC2REF                                 (0x05UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_OC3REF                                 (0x06UL << TIMX_CR2_MIMS_Pos)
#define TIM_TRGO_OC4REF                                 (0x07UL << TIMX_CR2_MIMS_Pos)
/**
  * @}
  */

/** @defgroup TIM_Slave_Mode TIM Slave Mode
  * @{
  */
#define TIM_SLAVEMODE_DISABLE                           (0x00000000U)
#define TIM_SLAVEMODE_RESET                             (0x00000004U)
#define TIM_SLAVEMODE_GATED                             (0x00000005U)
#define TIM_SLAVEMODE_TRIGGER                           (0x00000006U)
#define TIM_SLAVEMODE_EXTERNAL1                         (0x00000007U)
/**
  * @}
  */

/** @defgroup TIM_Master_Slave_Mode TIM Master Slave Mode
  * @{
  */
#define TIM_MASTERSLAVEMODE_ENABLE                      (0x00000080U)
#define TIM_MASTERSLAVEMODE_DISABLE                     (0x00000000U)
/**
  * @}
  */

/** @defgroup TIM_Trigger_Selection TIM Trigger Selection
  * @{
  */
#define TIM_TS_ITR0                                     (0x00000000U)
#define TIM_TS_ITR1                                     (0x00000010U)
#define TIM_TS_ITR2                                     (0x00000020U)
#define TIM_TS_ITR3                                     (0x00000030U)
#define TIM_TS_TI1F_ED                                  (0x00000040U)
#define TIM_TS_TI1FP1                                   (0x00000050U)
#define TIM_TS_TI2FP2                                   (0x00000060U)
#define TIM_TS_ETRF                                     (0x00000070U)
#define TIM_TS_NONE                                     (0x0000FFFFU)
/**
  * @}
  */

/** @defgroup TIM_Trigger_Polarity TIM Trigger Polarity
  * @{
  */
#define TIM_TRIGGERPOLARITY_INVERTED        TIM_ETRPOLARITY_INVERTED               /*!< Polarity for ETRx trigger sources */
#define TIM_TRIGGERPOLARITY_NONINVERTED     TIM_ETRPOLARITY_NONINVERTED            /*!< Polarity for ETRx trigger sources */
#define TIM_TRIGGERPOLARITY_RISING          TIM_INPUTCHANNELPOLARITY_RISING        /*!< Polarity for TIxFPx or TI1_ED trigger sources */
#define TIM_TRIGGERPOLARITY_FALLING         TIM_INPUTCHANNELPOLARITY_FALLING       /*!< Polarity for TIxFPx or TI1_ED trigger sources */
#define TIM_TRIGGERPOLARITY_BOTHEDGE        TIM_INPUTCHANNELPOLARITY_BOTHEDGE      /*!< Polarity for TIxFPx or TI1_ED trigger sources */
/**
  * @}
  */

/** @defgroup TIM_Trigger_Prescaler TIM Trigger Prescaler
  * @{
  */
#define TIM_TRIGGERPRESCALER_DIV1           TIM_ETRPRESCALER_DIV1     /*!< No prescaler is used */
#define TIM_TRIGGERPRESCALER_DIV2           TIM_ETRPRESCALER_DIV2     /*!< Prescaler for External ETR Trigger: Capture performed once every 2 events. */
#define TIM_TRIGGERPRESCALER_DIV4           TIM_ETRPRESCALER_DIV4     /*!< Prescaler for External ETR Trigger: Capture performed once every 4 events. */
#define TIM_TRIGGERPRESCALER_DIV8           TIM_ETRPRESCALER_DIV8     /*!< Prescaler for External ETR Trigger: Capture performed once every 8 events. */
/**
  * @}
  */

/** @defgroup TIM_TI1_Selection TIM TI1 Input Selection
  * @{
  */
#define TIM_TI1SELECTION_CH1                (0x00000000U)
#define TIM_TI1SELECTION_XORCOMBINATION     TIMX_CR2_TI1S
/**
  * @}
  */

/** @defgroup TIM_DMA_Base_address TIM DMA Base Address
  * @{
  */
#define TIM_DMABASE_CR1                     (0x00000000U)
#define TIM_DMABASE_CR2                     (0x00000001U)
#define TIM_DMABASE_SMCR                    (0x00000002U)
#define TIM_DMABASE_DIER                    (0x00000003U)
#define TIM_DMABASE_SR                      (0x00000004U)
#define TIM_DMABASE_EGR                     (0x00000005U)
#define TIM_DMABASE_CCMR1                   (0x00000006U)
#define TIM_DMABASE_CCMR2                   (0x00000007U)
#define TIM_DMABASE_CCER                    (0x00000008U)
#define TIM_DMABASE_CNT                     (0x00000009U)
#define TIM_DMABASE_PSC                     (0x0000000AU)
#define TIM_DMABASE_ARR                     (0x0000000BU)
#define TIM_DMABASE_RCR                     (0x0000000CU)
#define TIM_DMABASE_CCR1                    (0x0000000DU)
#define TIM_DMABASE_CCR2                    (0x0000000EU)
#define TIM_DMABASE_CCR3                    (0x0000000FU)
#define TIM_DMABASE_CCR4                    (0x00000010U)
#define TIM_DMABASE_BDTR                    (0x00000011U)
#define TIM_DMABASE_DCR                     (0x00000012U)
/**
  * @}
  */

/** @defgroup TIM_DMA_Burst_Length TIM DMA Burst Length
  * @{
  */
#define TIM_DMABURSTLENGTH_1TRANSFER        (0x00000000U)
#define TIM_DMABURSTLENGTH_2TRANSFERS       (0x00000100U)
#define TIM_DMABURSTLENGTH_3TRANSFERS       (0x00000200U)
#define TIM_DMABURSTLENGTH_4TRANSFERS       (0x00000300U)
#define TIM_DMABURSTLENGTH_5TRANSFERS       (0x00000400U)
#define TIM_DMABURSTLENGTH_6TRANSFERS       (0x00000500U)
#define TIM_DMABURSTLENGTH_7TRANSFERS       (0x00000600U)
#define TIM_DMABURSTLENGTH_8TRANSFERS       (0x00000700U)
#define TIM_DMABURSTLENGTH_9TRANSFERS       (0x00000800U)
#define TIM_DMABURSTLENGTH_10TRANSFERS      (0x00000900U)
#define TIM_DMABURSTLENGTH_11TRANSFERS      (0x00000A00U)
#define TIM_DMABURSTLENGTH_12TRANSFERS      (0x00000B00U)
#define TIM_DMABURSTLENGTH_13TRANSFERS      (0x00000C00U)
#define TIM_DMABURSTLENGTH_14TRANSFERS      (0x00000D00U)
#define TIM_DMABURSTLENGTH_15TRANSFERS      (0x00000E00U)
#define TIM_DMABURSTLENGTH_16TRANSFERS      (0x00000F00U)
#define TIM_DMABURSTLENGTH_17TRANSFERS      (0x00001000U)
#define TIM_DMABURSTLENGTH_18TRANSFERS      (0x00001100U)
/**
  * @}
  */

/** @defgroup TIM_DMA_Handle_index TIM DMA Handle Index
  * @{
  */
#define TIM_DMA_ID_UPDATE                ((uint16_t)0x00)       /*!< Index of the DMA handle used for Update DMA requests */
#define TIM_DMA_ID_CC1                   ((uint16_t)0x01)       /*!< Index of the DMA handle used for Capture/Compare 1 DMA requests */
#define TIM_DMA_ID_CC2                   ((uint16_t)0x02)       /*!< Index of the DMA handle used for Capture/Compare 2 DMA requests */
#define TIM_DMA_ID_CC3                   ((uint16_t)0x03)       /*!< Index of the DMA handle used for Capture/Compare 3 DMA requests */
#define TIM_DMA_ID_CC4                   ((uint16_t)0x04)       /*!< Index of the DMA handle used for Capture/Compare 4 DMA requests */
#define TIM_DMA_ID_COMMUTATION           ((uint16_t)0x05)       /*!< Index of the DMA handle used for Commutation DMA requests */
#define TIM_DMA_ID_TRIGGER               ((uint16_t)0x06)       /*!< Index of the DMA handle used for Trigger DMA requests */
/**
  * @}
  */

/** @defgroup TIM_Channel_CC_State TIM Capture/Compare Channel State
  * @{
  */
#define TIM_CCx_ENABLE                   (0x00000001U)
#define TIM_CCx_DISABLE                  (0x00000000U)
#define TIM_CCxN_ENABLE                  (0x00000004U)
#define TIM_CCxN_DISABLE                 (0x00000000U)

/**
  * @}
  */

/**
  * @}
  */

/* Private Constants -----------------------------------------------------------*/
/** @defgroup TIM_Private_Constants TIM Private Constants
  * @{
  */

/* The counter of a timer instance is disabled only if all the CCx and CCxN
   channels have been disabled */
#define TIM_CCER_CCxE_MASK              ((uint32_t)(TIMX_CCER_CC1E | TIMX_CCER_CC2E | TIMX_CCER_CC3E | TIMX_CCER_CC4E))
#define TIM_CCER_CCxNE_MASK             ((uint32_t)(TIMX_CCER_CC1NE | TIMX_CCER_CC2NE | TIMX_CCER_CC3NE))

/**
  * @}
  */

/* Private Macros -----------------------------------------------------------*/
/** @defgroup TIM_Private_Macros TIM Private Macros
 * @{
 */

#define IS_TIM_COUNTER_MODE(MODE)               (((MODE) == TIM_COUNTERMODE_UP) || ((MODE) == TIM_COUNTERMODE_DOWN) || \
                                                 ((MODE) == TIM_COUNTERMODE_CENTERALIGNED1) || ((MODE) == TIM_COUNTERMODE_CENTERALIGNED2) || \
                                                 ((MODE) == TIM_COUNTERMODE_CENTERALIGNED3))

#define IS_TIM_CLOCKDIVISION_DIV(DIV)           (((DIV) == TIM_CLOCKDIVISION_DIV1) || ((DIV) == TIM_CLOCKDIVISION_DIV2) || ((DIV) == TIM_CLOCKDIVISION_DIV4))

#define IS_TIM_AUTORELOAD_PRELOAD(PRELOAD)      (((PRELOAD) == TIM_AUTORELOAD_PRELOAD_DISABLE) || ((PRELOAD) == TIM_AUTORELOAD_PRELOAD_ENABLE))

#define IS_TIM_PWM_MODE(MODE)                   (((MODE) == TIM_OCMODE_PWM1) || ((MODE) == TIM_OCMODE_PWM2))
                              
#define IS_TIM_OC_MODE(MODE)                    (((MODE) == TIM_OCMODE_TIMING) || ((MODE) == TIM_OCMODE_ACTIVE) || ((MODE) == TIM_OCMODE_INACTIVE) || \
                                                 ((MODE) == TIM_OCMODE_TOGGLE) || ((MODE) == TIM_OCMODE_FORCED_ACTIVE) || ((MODE) == TIM_OCMODE_FORCED_INACTIVE))

#define IS_TIM_FAST_STATE(STATE)                (((STATE) == TIM_OCFAST_DISABLE) || ((STATE) == TIM_OCFAST_ENABLE))

#define IS_TIM_OC_POLARITY(POLARITY)            (((POLARITY) == TIM_OCPOLARITY_HIGH) || ((POLARITY) == TIM_OCPOLARITY_LOW))

#define IS_TIM_OCN_POLARITY(POLARITY)           (((POLARITY) == TIM_OCNPOLARITY_HIGH) || ((POLARITY) == TIM_OCNPOLARITY_LOW))

#define IS_TIM_OCIDLE_STATE(STATE)              (((STATE) == TIM_OCIDLESTATE_SET) || ((STATE) == TIM_OCIDLESTATE_RESET))

#define IS_TIM_OCNIDLE_STATE(STATE)             (((STATE) == TIM_OCNIDLESTATE_SET) || ((STATE) == TIM_OCNIDLESTATE_RESET))

#define IS_TIM_CHANNELS(CHANNEL)                (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2) || ((CHANNEL) == TIM_CHANNEL_3) || \
                                                 ((CHANNEL) == TIM_CHANNEL_4) || ((CHANNEL) == TIM_CHANNEL_ALL))

#define IS_TIM_OPM_CHANNELS(CHANNEL)            (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2))

#define IS_TIM_COMPLEMENTARY_CHANNELS(CHANNEL)  (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2) || ((CHANNEL) == TIM_CHANNEL_3))

#define IS_TIM_IC_POLARITY(__POLARITY__)        (((__POLARITY__) == TIM_ICPOLARITY_RISING) || ((__POLARITY__) == TIM_ICPOLARITY_FALLING) || \
                                                 ((__POLARITY__) == TIM_ICPOLARITY_BOTHEDGE))

#define IS_TIM_IC_SELECTION(SELECTION)          (((SELECTION) == TIM_ICSELECTION_DIRECTTI) || ((SELECTION) == TIM_ICSELECTION_INDIRECTTI) || \
                                                 ((SELECTION) == TIM_ICSELECTION_TRC))

#define IS_TIM_IC_PRESCALER(PRESCALER)          (((PRESCALER) == TIM_ICPSC_DIV1) || ((PRESCALER) == TIM_ICPSC_DIV2) || \
                                                 ((PRESCALER) == TIM_ICPSC_DIV4) || ((PRESCALER) == TIM_ICPSC_DIV8))

#define IS_TIM_OPM_MODE(MODE)                   (((MODE) == TIM_OPMODE_SINGLE) || ((MODE) == TIM_OPMODE_REPETITIVE))

#define IS_TIM_ENCODER_MODE(MODE)               (((MODE) == TIM_ENCODERMODE_TI1) || ((MODE) == TIM_ENCODERMODE_TI2) || ((MODE) == TIM_ENCODERMODE_TI12))   

#define IS_TIM_DMA_SOURCE(SOURCE)               ((((SOURCE) & 0xFFFF80FFU) == 0x00000000U) && ((SOURCE) != 0x00000000U))

#define IS_TIM_EVENT_SOURCE(SOURCE)             ((((SOURCE) & 0xFFFFFF00U) == 0x00000000U) && ((SOURCE) != 0x00000000U))

#define IS_TIM_CLOCKSOURCE(CLOCK)               (((CLOCK) == TIM_CLOCKSOURCE_INTERNAL) || ((CLOCK) == TIM_CLOCKSOURCE_ETRMODE2) || \
                                                 ((CLOCK) == TIM_CLOCKSOURCE_ITR0) || ((CLOCK) == TIM_CLOCKSOURCE_ITR1) || \
                                                 ((CLOCK) == TIM_CLOCKSOURCE_ITR2) || ((CLOCK) == TIM_CLOCKSOURCE_ITR3) || \
                                                 ((CLOCK) == TIM_CLOCKSOURCE_TI1ED) || ((CLOCK) == TIM_CLOCKSOURCE_TI1) || \
                                                 ((CLOCK) == TIM_CLOCKSOURCE_TI2) || ((CLOCK) == TIM_CLOCKSOURCE_ETRMODE1))

#define IS_TIM_CLOCKPOLARITY(POLARITY)          (((POLARITY) == TIM_CLOCKPOLARITY_INVERTED) || ((POLARITY) == TIM_CLOCKPOLARITY_NONINVERTED) || \
                                                 ((POLARITY) == TIM_CLOCKPOLARITY_RISING) || ((POLARITY) == TIM_CLOCKPOLARITY_FALLING) || \
                                                 ((POLARITY) == TIM_CLOCKPOLARITY_BOTHEDGE))

#define IS_TIM_CLOCKPRESCALER(PRESCALER)        (((PRESCALER) == TIM_CLOCKPRESCALER_DIV1) || ((PRESCALER) == TIM_CLOCKPRESCALER_DIV2) || \
                                                 ((PRESCALER) == TIM_CLOCKPRESCALER_DIV4) || ((PRESCALER) == TIM_CLOCKPRESCALER_DIV8)) 

#define IS_TIM_CLOCKFILTER(ICFILTER)            ((ICFILTER) <= 0x0FU) 

#define IS_TIM_CLEARINPUT_SOURCE(SOURCE)        (((SOURCE) == TIM_CLEARINPUTSOURCE_ETR) || ((SOURCE) == TIM_CLEARINPUTSOURCE_NONE))

#define IS_TIM_CLEARINPUT_POLARITY(POLARITY)    (((POLARITY) == TIM_CLEARINPUTPOLARITY_INVERTED) || ((POLARITY) == TIM_CLEARINPUTPOLARITY_NONINVERTED))

#define IS_TIM_CLEARINPUT_PRESCALER(PRESCALER)  (((PRESCALER) == TIM_CLEARINPUTPRESCALER_DIV1) || ((PRESCALER) == TIM_CLEARINPUTPRESCALER_DIV2) || \
                                                 ((PRESCALER) == TIM_CLEARINPUTPRESCALER_DIV4) || ((PRESCALER) == TIM_CLEARINPUTPRESCALER_DIV8))

#define IS_TIM_CLEARINPUT_FILTER(ICFILTER)      ((ICFILTER) <= 0x0FU)

#define IS_TIM_OSSR_STATE(STATE)                (((STATE) == TIM_OSSR_ENABLE) || ((STATE) == TIM_OSSR_DISABLE))

#define IS_TIM_OSSI_STATE(STATE)                (((STATE) == TIM_OSSI_ENABLE) || ((STATE) == TIM_OSSI_DISABLE))

#define IS_TIM_LOCK_LEVEL(LEVEL)                (((LEVEL) == TIM_LOCKLEVEL_OFF) || ((LEVEL) == TIM_LOCKLEVEL_1) || \
                                                 ((LEVEL) == TIM_LOCKLEVEL_2) || ((LEVEL) == TIM_LOCKLEVEL_3))

#define IS_TIM_BREAK_STATE(STATE)               (((STATE) == TIM_BREAK_ENABLE) || ((STATE) == TIM_BREAK_DISABLE))

#define IS_TIM_BREAK_POLARITY(POLARITY)         (((POLARITY) == TIM_BREAKPOLARITY_LOW) || ((POLARITY) == TIM_BREAKPOLARITY_HIGH))

#define IS_TIM_AUTOMATIC_OUTPUT_STATE(STATE)    (((STATE) == TIM_AUTOMATICOUTPUT_ENABLE) || ((STATE) == TIM_AUTOMATICOUTPUT_DISABLE))

#define IS_TIM_TRGO_SOURCE(SOURCE)              (((SOURCE) == TIM_TRGO_RESET) || ((SOURCE) == TIM_TRGO_ENABLE) || \
                                                 ((SOURCE) == TIM_TRGO_UPDATE) || ((SOURCE) == TIM_TRGO_OC1) || \
                                                 ((SOURCE) == TIM_TRGO_OC1REF) || ((SOURCE) == TIM_TRGO_OC2REF) || \
                                                 ((SOURCE) == TIM_TRGO_OC3REF) || ((SOURCE) == TIM_TRGO_OC4REF))

#define IS_TIM_SLAVE_MODE(MODE)                 (((MODE) == TIM_SLAVEMODE_DISABLE) || ((MODE) == TIM_SLAVEMODE_GATED) || \
                                                 ((MODE) == TIM_SLAVEMODE_RESET) || ((MODE) == TIM_SLAVEMODE_TRIGGER) || \
                                                 ((MODE) == TIM_SLAVEMODE_EXTERNAL1))

#define IS_TIM_MSM_STATE(STATE)                 (((STATE) == TIM_MASTERSLAVEMODE_ENABLE) || ((STATE) == TIM_MASTERSLAVEMODE_DISABLE))

#define IS_TIM_TRIGGER_SELECTION(SELECTION)     (((SELECTION) == TIM_TS_ITR0) || ((SELECTION) == TIM_TS_ITR1) || \
                                                 ((SELECTION) == TIM_TS_ITR2) || ((SELECTION) == TIM_TS_ITR3) || \
                                                 ((SELECTION) == TIM_TS_TI1F_ED) || ((SELECTION) == TIM_TS_TI1FP1) || \
                                                 ((SELECTION) == TIM_TS_TI2FP2) || ((SELECTION) == TIM_TS_ETRF))

#define IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(SELECTION)       (((SELECTION) == TIM_TS_ITR0) || ((SELECTION) == TIM_TS_ITR1) || \
                                                                 ((SELECTION) == TIM_TS_ITR2) || ((SELECTION) == TIM_TS_ITR3) || \
                                                                 ((SELECTION) == TIM_TS_NONE))

#define IS_TIM_TRIGGERPOLARITY(POLARITY)        (((POLARITY) == TIM_TRIGGERPOLARITY_INVERTED) || ((POLARITY) == TIM_TRIGGERPOLARITY_NONINVERTED) || \
                                                 ((POLARITY) == TIM_TRIGGERPOLARITY_RISING) || ((POLARITY) == TIM_TRIGGERPOLARITY_FALLING) || \
                                                 ((POLARITY) == TIM_TRIGGERPOLARITY_BOTHEDGE))

#define IS_TIM_TRIGGERPRESCALER(PRESCALER)      (((PRESCALER) == TIM_TRIGGERPRESCALER_DIV1) || ((PRESCALER) == TIM_TRIGGERPRESCALER_DIV2) || \
                                                 ((PRESCALER) == TIM_TRIGGERPRESCALER_DIV4) || ((PRESCALER) == TIM_TRIGGERPRESCALER_DIV8))

#define IS_TIM_TRIGGERFILTER(ICFILTER)          ((ICFILTER) <= 0x0FU)

#define IS_TIM_TI1SELECTION(TI1SELECTION)       (((TI1SELECTION) == TIM_TI1SELECTION_CH1) || ((TI1SELECTION) == TIM_TI1SELECTION_XORCOMBINATION))

#define IS_TIM_DMA_BASE(BASE)                   (((BASE) == TIM_DMABASE_CR1) || ((BASE) == TIM_DMABASE_CR2) || \
                                                 ((BASE) == TIM_DMABASE_SMCR) || ((BASE) == TIM_DMABASE_DIER) || \
                                                 ((BASE) == TIM_DMABASE_SR) || ((BASE) == TIM_DMABASE_EGR) || \
                                                 ((BASE) == TIM_DMABASE_CCMR1) || ((BASE) == TIM_DMABASE_CCMR2) || \
                                                 ((BASE) == TIM_DMABASE_CCER) || ((BASE) == TIM_DMABASE_CNT) || \
                                                 ((BASE) == TIM_DMABASE_PSC) || ((BASE) == TIM_DMABASE_ARR) || \
                                                 ((BASE) == TIM_DMABASE_RCR) || ((BASE) == TIM_DMABASE_CCR1)  || \
                                                 ((BASE) == TIM_DMABASE_CCR2) || ((BASE) == TIM_DMABASE_CCR3) || \
                                                 ((BASE) == TIM_DMABASE_CCR4) || ((BASE) == TIM_DMABASE_BDTR) || \
                                                 ((BASE) == TIM_DMABASE_DCR))

#define IS_TIM_DMA_LENGTH(LENGTH)               (((LENGTH) == TIM_DMABURSTLENGTH_1TRANSFER)   || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_2TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_3TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_4TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_5TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_6TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_7TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_8TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_9TRANSFERS)  || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_10TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_11TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_12TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_13TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_14TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_15TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_16TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_17TRANSFERS) || \
                                                 ((LENGTH) == TIM_DMABURSTLENGTH_18TRANSFERS))

#define IS_TIM_IC_FILTER(ICFILTER) ((ICFILTER) <= 0x0FU)

/** @brief Set TIM IC prescaler
  * @param  __HANDLE__: TIM handle
  * @param  __CHANNEL__: specifies TIM Channel
  * @param  __ICPSC__: specifies the prescaler value.
  * @retval None
  */
#define TIM_SET_ICPRESCALERVALUE(__HANDLE__, __CHANNEL__, __ICPSC__)    (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 |= (__ICPSC__)) : \
                                                                         ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 |= ((__ICPSC__) << 8U)) : \
                                                                         ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 |= (__ICPSC__)) : \
                                                                         ((__HANDLE__)->Instance->CCMR2 |= ((__ICPSC__) << 8U)))

/** @brief Reset TIM IC prescaler
  * @param  __HANDLE__: TIM handle
  * @param  __CHANNEL__: specifies TIM Channel
  * @retval None
  */
#define TIM_RESET_ICPRESCALERVALUE(__HANDLE__, __CHANNEL__)             (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC) : \
                                                                         ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC) : \
                                                                         ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_IC3PSC) : \
                                                                         ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_IC4PSC))


/** @brief Set TIM IC polarity
  * @param  __HANDLE__: TIM handle
  * @param  __CHANNEL__: specifies TIM Channel
  * @param  __POLARITY__: specifies TIM Channel Polarity
  * @retval None
  */
#define TIM_SET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__, __POLARITY__)                                                                 \
                                        (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCER |= (__POLARITY__)) :         \
                                         ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCER |= ((__POLARITY__) << 4U)) : \
                                         ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCER |= ((__POLARITY__) << 8U)) : \
                                         ((__HANDLE__)->Instance->CCER |= (((__POLARITY__) << 12U) & TIM_CCER_CC4P)))

/** @brief Reset TIM IC polarity
  * @param  __HANDLE__: TIM handle
  * @param  __CHANNEL__: specifies TIM Channel
  * @retval None
  */
#define TIM_RESET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__)                                                                                                    \
                                        (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCER &= (uint16_t)~(TIMX_CCER_CC1P | TIMX_CCER_CC1NP)) : \
                                         ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCER &= (uint16_t)~(TIMX_CCER_CC2P | TIMX_CCER_CC2NP)) : \
                                         ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCER &= (uint16_t)~(TIMX_CCER_CC3P | TIMX_CCER_CC3NP)) : \
                                         ((__HANDLE__)->Instance->CCER &= (uint16_t)~TIM_CCER_CC4P))

/**
  * @}
  */

/* Private Functions --------------------------------------------------------*/
/** @addtogroup TIM_Private_Functions
 * @{
 */
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure);
void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma);
void TIM_DMAError(DMA_HandleTypeDef *hdma);
void TIM_DMACaptureCplt(DMA_HandleTypeDef *hdma);
void TIM_CCxChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelState);
void TIM_CCxNChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelNState);
/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup TIM_Exported_Macros TIM Exported Macros
  * @{
  */

/** @brief  Reset TIM handle state
  * @param  __HANDLE__: TIM handle.
  * @retval None
  */
#define __HAL_TIM_RESET_HANDLE_STATE(__HANDLE__)        ((__HANDLE__)->State = HAL_TIM_STATE_RESET)

/**
  * @brief  Enable the TIM peripheral.
  * @param  __HANDLE__: TIM handle
  * @retval None
 */
#define __HAL_TIM_ENABLE(__HANDLE__)                    ((__HANDLE__)->Instance->CR1|=(TIMX_CR1_CEN))

/**
  * @brief  Enable the TIM main Output.
  * @param  __HANDLE__: TIM handle
  * @retval None
  */
#define __HAL_TIM_MOE_ENABLE(__HANDLE__)                ((__HANDLE__)->Instance->BDTR |= (TIMX_BDTR4_MOE))

/**
  * @brief  Disable the TIM peripheral.
  * @param  __HANDLE__: TIM handle
  * @retval None
  */
#define __HAL_TIM_DISABLE(__HANDLE__)                   (__HANDLE__)->Instance->CR1 &= ~(TIMX_CR1_CEN)
                           
/* The Main Output Enable of a timer instance is disabled only if all the CCx and CCxN
   channels have been disabled */
/**
  * @brief  Disable the TIM main Output.
  * @param  __HANDLE__: TIM handle
  * @retval None
  * @note The Main Output Enable of a timer instance is disabled only if all the CCx and CCxN channels have been disabled
  */
#define __HAL_TIM_MOE_DISABLE(__HANDLE__)               do {                                                                    \
                                                            if (((__HANDLE__)->Instance->CCER & TIM_CCER_CCxE_MASK) == 0U)      \
                                                            {                                                                   \
                                                                if(((__HANDLE__)->Instance->CCER & TIM_CCER_CCxNE_MASK) == 0U)  \
                                                                {                                                               \
                                                                    (__HANDLE__)->Instance->BDTR &= ~(TIMX_BDTR4_MOE);          \
                                                                }                                                               \
                                                            }                                                                   \
                                                        } while(0)

/**
  * @brief  Disable the TIM main Output.
  * @param  __HANDLE__: TIM handle
  * @retval None
  * @note The Main Output Enable of a timer instance is disabled unconditionally
  */
#define __HAL_TIM_MOE_DISABLE_UNCONDITIONALLY(__HANDLE__)  (__HANDLE__)->Instance->BDTR &= ~(TIMX_BDTR4_MOE)

/**
  * @brief  Enables the specified TIM interrupt.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __INTERRUPT__: specifies the TIM interrupt source to enable.
  *          This parameter can be one of the following values:
  *            @arg TIM_IT_UPDATE: Update interrupt
  *            @arg TIM_IT_CC1:  Capture/Compare 1 interrupt
  *            @arg TIM_IT_CC2:  Capture/Compare 2 interrupt
  *            @arg TIM_IT_CC3:  Capture/Compare 3 interrupt
  *            @arg TIM_IT_CC4:  Capture/Compare 4 interrupt
  *            @arg TIM_IT_COM:  Commutation interrupt
  *            @arg TIM_IT_TRIGGER: Trigger interrupt
  *            @arg TIM_IT_BREAK: Break interrupt
  * @retval None
  */
#define __HAL_TIM_ENABLE_IT(__HANDLE__, __INTERRUPT__)    ((__HANDLE__)->Instance->DIER |= (__INTERRUPT__))

/**
  * @brief  Disables the specified TIM interrupt.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __INTERRUPT__: specifies the TIM interrupt source to disable.
  *          This parameter can be one of the following values:
  *            @arg TIM_IT_UPDATE: Update interrupt
  *            @arg TIM_IT_CC1:  Capture/Compare 1 interrupt
  *            @arg TIM_IT_CC2:  Capture/Compare 2 interrupt
  *            @arg TIM_IT_CC3:  Capture/Compare 3 interrupt
  *            @arg TIM_IT_CC4:  Capture/Compare 4 interrupt
  *            @arg TIM_IT_COM:  Commutation interrupt
  *            @arg TIM_IT_TRIGGER: Trigger interrupt
  *            @arg TIM_IT_BREAK: Break interrupt
  * @retval None
  */
#define __HAL_TIM_DISABLE_IT(__HANDLE__, __INTERRUPT__)   ((__HANDLE__)->Instance->DIER &= ~(__INTERRUPT__))

/**
  * @brief  Enables the specified DMA request.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __DMA__: specifies the TIM DMA request to enable.
  *          This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: Update DMA request
  *            @arg TIM_DMA_CC1:  Capture/Compare 1 DMA request
  *            @arg TIM_DMA_CC2:  Capture/Compare 2 DMA request
  *            @arg TIM_DMA_CC3:  Capture/Compare 3 DMA request
  *            @arg TIM_DMA_CC4:  Capture/Compare 4 DMA request
  *            @arg TIM_DMA_COM:  Commutation DMA request
  *            @arg TIM_DMA_TRIGGER: Trigger DMA request
  * @retval None
  */
#define __HAL_TIM_ENABLE_DMA(__HANDLE__, __DMA__)         ((__HANDLE__)->Instance->DIER |= (__DMA__))

/**
  * @brief  Disables the specified DMA request.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __DMA__: specifies the TIM DMA request to disable.
  *          This parameter can be one of the following values:
  *            @arg TIM_DMA_UPDATE: Update DMA request
  *            @arg TIM_DMA_CC1:  Capture/Compare 1 DMA request
  *            @arg TIM_DMA_CC2:  Capture/Compare 2 DMA request
  *            @arg TIM_DMA_CC3:  Capture/Compare 3 DMA request
  *            @arg TIM_DMA_CC4:  Capture/Compare 4 DMA request
  *            @arg TIM_DMA_COM:  Commutation DMA request
  *            @arg TIM_DMA_TRIGGER: Trigger DMA request
  * @retval None
  */
#define __HAL_TIM_DISABLE_DMA(__HANDLE__, __DMA__)        ((__HANDLE__)->Instance->DIER &= ~(__DMA__))

/**
  * @brief  Checks whether the specified TIM interrupt flag is set or not.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __FLAG__: specifies the TIM interrupt flag to check.
  *        This parameter can be one of the following values:
  *            @arg TIM_FLAG_UPDATE: Update interrupt flag
  *            @arg TIM_FLAG_CC1: Capture/Compare 1 interrupt flag
  *            @arg TIM_FLAG_CC2: Capture/Compare 2 interrupt flag
  *            @arg TIM_FLAG_CC3: Capture/Compare 3 interrupt flag
  *            @arg TIM_FLAG_CC4: Capture/Compare 4 interrupt flag
  *            @arg TIM_FLAG_COM:  Commutation interrupt flag
  *            @arg TIM_FLAG_TRIGGER: Trigger interrupt flag
  *            @arg TIM_FLAG_BREAK: Break interrupt flag   
  *            @arg TIM_FLAG_CC1OF: Capture/Compare 1 overcapture flag
  *            @arg TIM_FLAG_CC2OF: Capture/Compare 2 overcapture flag
  *            @arg TIM_FLAG_CC3OF: Capture/Compare 3 overcapture flag
  *            @arg TIM_FLAG_CC4OF: Capture/Compare 4 overcapture flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_TIM_GET_FLAG(__HANDLE__, __FLAG__)          (((__HANDLE__)->Instance->SR &(__FLAG__)) == (__FLAG__))

/**
  * @brief  Clears the specified TIM interrupt flag.
  * @param  __HANDLE__: specifies the TIM Handle.
  * @param  __FLAG__: specifies the TIM interrupt flag to clear.
  *        This parameter can be one of the following values:
  *            @arg TIM_FLAG_UPDATE: Update interrupt flag
  *            @arg TIM_FLAG_CC1: Capture/Compare 1 interrupt flag
  *            @arg TIM_FLAG_CC2: Capture/Compare 2 interrupt flag
  *            @arg TIM_FLAG_CC3: Capture/Compare 3 interrupt flag
  *            @arg TIM_FLAG_CC4: Capture/Compare 4 interrupt flag
  *            @arg TIM_FLAG_COM:  Commutation interrupt flag
  *            @arg TIM_FLAG_TRIGGER: Trigger interrupt flag
  *            @arg TIM_FLAG_BREAK: Break interrupt flag   
  *            @arg TIM_FLAG_CC1OF: Capture/Compare 1 overcapture flag
  *            @arg TIM_FLAG_CC2OF: Capture/Compare 2 overcapture flag
  *            @arg TIM_FLAG_CC3OF: Capture/Compare 3 overcapture flag
  *            @arg TIM_FLAG_CC4OF: Capture/Compare 4 overcapture flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_TIM_CLEAR_FLAG(__HANDLE__, __FLAG__)        ((__HANDLE__)->Instance->SR = ~(__FLAG__))

/**
  * @brief  Checks whether the specified TIM interrupt has occurred or not.
  * @param  __HANDLE__: TIM handle
  * @param  __INTERRUPT__: specifies the TIM interrupt source to check.
  * @retval The state of TIM_IT (SET or RESET).
  */
#define __HAL_TIM_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->DIER & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/**
  * @brief Clear the TIM interrupt pending bits
  * @param  __HANDLE__: TIM handle
  * @param  __INTERRUPT__: specifies the interrupt pending bit to clear.
  * @retval None
  */
#define __HAL_TIM_CLEAR_IT(__HANDLE__, __INTERRUPT__)     ((__HANDLE__)->Instance->SR &= ~(__INTERRUPT__))

/**
  * @brief  Indicates whether or not the TIM Counter is used as downcounter
  * @param  __HANDLE__: TIM handle.
  * @retval False (Counter used as upcounter) or True (Counter used as downcounter)
  * @note This macro is particularly usefull to get the counting mode when the timer operates in Center-aligned mode or Encoder
mode.
  */
#define __HAL_TIM_IS_TIM_COUNTING_DOWN(__HANDLE__)        (((__HANDLE__)->Instance->CR1 & (TIM_CR1_DIR)) == (TIM_CR1_DIR))

/**
  * @brief  Sets the TIM active prescaler register value on update event.
  * @param  __HANDLE__: TIM handle.
  * @param  __PRESC__: specifies the active prescaler register new value.
  * @retval None
  */
#define __HAL_TIM_SET_PRESCALER(__HANDLE__, __PRESC__)    ((__HANDLE__)->Instance->PSC = (__PRESC__))

/**
  * @brief  Sets the TIM Capture Compare Register value on runtime without
  *         calling another time ConfigChannel function.
  * @param  __HANDLE__: TIM handle.
  * @param  __CHANNEL__ : TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  __COMPARE__: specifies the Capture Compare register new value.
  * @retval None
  */
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)                     \
                                                            (*(__IO uint32_t *)(&((__HANDLE__)->Instance->CCR1) + ((__CHANNEL__) >> 2U)) = (__COMPARE__))

/**
  * @brief  Gets the TIM Capture Compare Register value on runtime
  * @param  __HANDLE__: TIM handle.
  * @param  __CHANNEL__ : TIM Channel associated with the capture compare register
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: get capture/compare 1 register value
  *            @arg TIM_CHANNEL_2: get capture/compare 2 register value
  *            @arg TIM_CHANNEL_3: get capture/compare 3 register value
  *            @arg TIM_CHANNEL_4: get capture/compare 4 register value
  * @retval 16-bit or 32-bit value of the capture/compare register (TIMx_CCRy)
  */
#define __HAL_TIM_GET_COMPARE(__HANDLE__, __CHANNEL__)                                   \
                                                               (*(__IO uint32_t *)(&((__HANDLE__)->Instance->CCR1) + ((__CHANNEL__) >> 2U)))

/**
  * @brief  Sets the TIM Counter Register value on runtime.
  * @param  __HANDLE__: TIM handle.
  * @param  __COUNTER__: specifies the Counter register new value.
  * @retval None
  */
#define __HAL_TIM_SET_COUNTER(__HANDLE__, __COUNTER__)          ((__HANDLE__)->Instance->CNT = (__COUNTER__))

/**
  * @brief  Gets the TIM Counter Register value on runtime.
  * @param  __HANDLE__: TIM handle.
  * @retval 16-bit or 32-bit value of the timer counter register (TIMx_CNT)
  */
#define __HAL_TIM_GET_COUNTER(__HANDLE__)                       ((__HANDLE__)->Instance->CNT)

/**
  * @brief  Sets the TIM Autoreload Register value on runtime without calling
  *         another time any Init function.
  * @param  __HANDLE__: TIM handle.
  * @param  __AUTORELOAD__: specifies the Counter register new value.
  * @retval None
  */
#define __HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__)    do{                                                  \
                                                                    (__HANDLE__)->Instance->ARR = (__AUTORELOAD__);  \
                                                                    (__HANDLE__)->Init.Period = (__AUTORELOAD__);    \
                                                                } while(0)

/**
  * @brief  Gets the TIM Autoreload Register value on runtime
  * @param  __HANDLE__: TIM handle.
  * @retval 16-bit or 32-bit value of the timer auto-reload register(TIMx_ARR)
  */
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__)                    ((__HANDLE__)->Instance->ARR)

/**
  * @brief  Sets the TIM Clock Division value on runtime without calling
  *         another time any Init function.
  * @param  __HANDLE__: TIM handle.
  * @param  __CKD__: specifies the clock division value.
  *          This parameter can be one of the following value:
  *            @arg TIM_CLOCKDIVISION_DIV1: tDTS=tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV2: tDTS=2*tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV4: tDTS=4*tCK_INT 
  * @retval None
  */
#define __HAL_TIM_SET_CLOCKDIVISION(__HANDLE__, __CKD__)        do{                                                           \
                                                                    (__HANDLE__)->Instance->CR1 &= (uint16_t)(~TIMX_CR1_CKD); \
                                                                    (__HANDLE__)->Instance->CR1 |= (__CKD__);                 \
                                                                    (__HANDLE__)->Init.ClockDivision = (__CKD__);             \
                                                                } while(0)

/**
  * @brief  Gets the TIM Clock Division value on runtime
  * @param  __HANDLE__: TIM handle.
  * @retval The clock division can be one of the following values:
  *            @arg TIM_CLOCKDIVISION_DIV1: tDTS=tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV2: tDTS=2*tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV4: tDTS=4*tCK_INT 
  */
#define __HAL_TIM_GET_CLOCKDIVISION(__HANDLE__)                 ((__HANDLE__)->Instance->CR1 & TIMX_CR1_CKD)

/**
  * @brief  Sets the TIM Input Capture prescaler on runtime without calling
  *         another time HAL_TIM_IC_ConfigChannel() function.
  * @param  __HANDLE__: TIM handle.
  * @param  __CHANNEL__ : TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  __ICPSC__: specifies the Input Capture4 prescaler new value.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPSC_DIV1: no prescaler
  *            @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *            @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *            @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
#define __HAL_TIM_SET_ICPRESCALER(__HANDLE__, __CHANNEL__, __ICPSC__)       do{                                                                     \
                                                                                TIM_RESET_ICPRESCALERVALUE((__HANDLE__), (__CHANNEL__));            \
                                                                                TIM_SET_ICPRESCALERVALUE((__HANDLE__), (__CHANNEL__), (__ICPSC__)); \
                                                                            } while(0)

/**
  * @brief  Gets the TIM Input Capture prescaler on runtime
  * @param  __HANDLE__: TIM handle.
  * @param  __CHANNEL__: TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: get input capture 1 prescaler value
  *            @arg TIM_CHANNEL_2: get input capture 2 prescaler value
  *            @arg TIM_CHANNEL_3: get input capture 3 prescaler value
  *            @arg TIM_CHANNEL_4: get input capture 4 prescaler value
  * @retval The input capture prescaler can be one of the following values:
  *            @arg TIM_ICPSC_DIV1: no prescaler
  *            @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *            @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *            @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  */
#define __HAL_TIM_GET_ICPRESCALER(__HANDLE__, __CHANNEL__)      (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 & TIM_CCMR1_IC1PSC) :         \
                                                                 ((__CHANNEL__) == TIM_CHANNEL_2) ? (((__HANDLE__)->Instance->CCMR1 & TIM_CCMR1_IC2PSC) >> 8U) : \
                                                                 ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 & TIM_CCMR2_IC3PSC) :         \
                                                                 (((__HANDLE__)->Instance->CCMR2 & TIM_CCMR2_IC4PSC)) >> 8U)

/**
  * @brief  Set the Update Request Source (URS) bit of the TIMx_CR1 register
  * @param  __HANDLE__: TIM handle.
  * @note  When the USR bit of the TIMx_CR1 register is set, only counter
  *        overflow/underflow generates an update interrupt or DMA request (if
  *        enabled)
  * @retval None
  */
#define __HAL_TIM_URS_ENABLE(__HANDLE__)                        ((__HANDLE__)->Instance->CR1|= (TIMX_CR1_URS))

/**
  * @brief  Reset the Update Request Source (URS) bit of the TIMx_CR1 register
  * @param  __HANDLE__: TIM handle.
  * @note  When the USR bit of the TIMx_CR1 register is reset, any of the
  *        following events generate an update interrupt or DMA request (if
  *        enabled):
  *          (+) Counter overflow/underflow
  *          (+) Setting the UG bit
  *          (+) Update generation through the slave mode controller
  * @retval None
  */
#define __HAL_TIM_URS_DISABLE(__HANDLE__)                       ((__HANDLE__)->Instance->CR1&=~(TIMX_CR1_URS))

/**
  * @brief  Sets the TIM Capture x input polarity on runtime.
  * @param  __HANDLE__: TIM handle.
  * @param  __CHANNEL__: TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  __POLARITY__: Polarity for TIx source   
  *            @arg TIM_INPUTCHANNELPOLARITY_RISING: Rising Edge
  *            @arg TIM_INPUTCHANNELPOLARITY_FALLING: Falling Edge
  *            @arg TIM_INPUTCHANNELPOLARITY_BOTHEDGE: Rising and Falling Edge
  * @note  The polarity TIM_INPUTCHANNELPOLARITY_BOTHEDGE is not authorized  for TIM Channel 4.     
  * @retval None
  */
#define __HAL_TIM_SET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__, __POLARITY__)            do{                                                                       \
                                                                                            TIM_RESET_CAPTUREPOLARITY((__HANDLE__), (__CHANNEL__));               \
                                                                                            TIM_SET_CAPTUREPOLARITY((__HANDLE__), (__CHANNEL__), (__POLARITY__)); \
                                                                                        }while(0)

/****************************** START TIM Instances ***************************/
/****************************** TIM Instances *********************************/
#define IS_TIM_INSTANCE(INSTANCE)                               (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM6) || \
                                                                 ((INSTANCE) == TIM7) || ((INSTANCE) == TIM14) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))

#define IS_TIM_CC1_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))     

#define IS_TIM_CC2_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15))       

#define IS_TIM_CC3_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3))    

#define IS_TIM_CC4_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3))    

#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE)          (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))    

#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE)          (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3))    

#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE)               (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))       

#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE)              (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15))        

#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE)                  (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))    

#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE)             (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15))

#define IS_TIM_XOR_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3))    

#define IS_TIM_MASTER_INSTANCE(INSTANCE)                        (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))     

#define IS_TIM_SLAVE_INSTANCE(INSTANCE)                         (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15) ||\
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))     

#define IS_TIM_SYNCHRO_INSTANCE(INSTANCE)                       IS_TIM_MASTER_INSTANCE(INSTANCE)

#define IS_TIM_DMABURST_INSTANCE(INSTANCE)                      (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))

#define IS_TIM_BREAK_INSTANCE(INSTANCE)                         (((INSTANCE) == TIM1) || ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || \
                                                                 ((INSTANCE) == TIM17))    
     
#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL)                  ((((INSTANCE) == TIM1) &&                                            \
                                                                  (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2) ||   \
                                                                   ((CHANNEL) == TIM_CHANNEL_3) || ((CHANNEL) == TIM_CHANNEL_4))) || \
                                                                 (((INSTANCE) == TIM3) &&                                            \
                                                                  (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2) ||   \
                                                                   ((CHANNEL) == TIM_CHANNEL_3) || ((CHANNEL) == TIM_CHANNEL_4))) || \
                                                                 (((INSTANCE) == TIM14) && (((CHANNEL) == TIM_CHANNEL_1))) ||        \
                                                                 (((INSTANCE) == TIM15) &&                                           \
                                                                  (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2))) || \
                                                                 (((INSTANCE) == TIM16) && (((CHANNEL) == TIM_CHANNEL_1)))        || \
                                                                 (((INSTANCE) == TIM17) && (((CHANNEL) == TIM_CHANNEL_1))))
            
#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL)                 ((((INSTANCE) == TIM1) &&                                             \
                                                                  (((CHANNEL) == TIM_CHANNEL_1) || ((CHANNEL) == TIM_CHANNEL_2) ||    \
                                                                   ((CHANNEL) == TIM_CHANNEL_3))) ||                                  \
                                                                 (((INSTANCE) == TIM15) && (((CHANNEL) == TIM_CHANNEL_1))) ||         \
                                                                 (((INSTANCE) == TIM16) && (((CHANNEL) == TIM_CHANNEL_1))) ||         \
                                                                 (((INSTANCE) == TIM17) && ((CHANNEL) == TIM_CHANNEL_1)))

#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE)           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17)) 

#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE)            (((INSTANCE) == TIM1) || ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || \
                                                                 ((INSTANCE) == TIM17)) 

#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE)                (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM6) ||   \
                                                                 ((INSTANCE) == TIM7) || ((INSTANCE) == TIM14) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))

#define IS_TIM_DMA_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM6) ||   \
                                                                 ((INSTANCE) == TIM7) || ((INSTANCE) == TIM14) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))
    
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE)                        (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM14) || \
                                                                 ((INSTANCE) == TIM15) || ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17)) 
    
#define IS_TIM_COMMUTATION_EVENT_INSTANCE(INSTANCE)             (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))

#define IS_TIM_ETR_INSTANCE(INSTANCE)                           (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3))

#define IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(INSTANCE)         (((INSTANCE) == TIM1) || ((INSTANCE) == TIM3) || ((INSTANCE) == TIM6) ||   \
                                                                 ((INSTANCE) == TIM7) || ((INSTANCE) == TIM14) || ((INSTANCE) == TIM15) || \
                                                                 ((INSTANCE) == TIM16) || ((INSTANCE) == TIM17))    

#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE)                   (0U)
/**
  * @}
  */

/* Include TIM HAL Extension module */
#include "slm32x030_hal_tim_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup TIM_Exported_Functions
  * @{
  */

/** @addtogroup TIM_Exported_Functions_Group1
 * @{
 */
/* Time Base functions ********************************************************/
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim);
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group2
 * @{
 */
/* Timer Output Compare functions **********************************************/
HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim);
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group3
 * @{
 */
/* Timer PWM functions *********************************************************/
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim);
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group4
 * @{
 */
/* Timer Input Capture functions ***********************************************/
HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_IC_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim);
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_IC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group5
 * @{
 */
/* Timer One Pulse functions ***************************************************/
HAL_StatusTypeDef HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, uint32_t OnePulseMode);
HAL_StatusTypeDef HAL_TIM_OnePulse_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim);
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group6
 * @{
 */
/* Timer Encoder functions *****************************************************/
HAL_StatusTypeDef HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim,  TIM_Encoder_InitTypeDef* sConfig);
HAL_StatusTypeDef HAL_TIM_Encoder_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim);
 /* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData1, uint32_t *pData2, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group7
 * @{
 */
/* Interrupt Handler functions  **********************************************/
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group8
 * @{
 */
/* Control functions  *********************************************************/
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef* sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OnePulse_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OnePulse_InitTypeDef* sConfig, uint32_t OutputChannel,  uint32_t InputChannel);
HAL_StatusTypeDef HAL_TIM_ConfigOCrefClear(TIM_HandleTypeDef *htim, TIM_ClearInputConfigTypeDef * sClearInputConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_ConfigClockSource(TIM_HandleTypeDef *htim, TIM_ClockConfigTypeDef * sClockSourceConfig);
HAL_StatusTypeDef HAL_TIM_ConfigTI1Input(TIM_HandleTypeDef *htim, uint32_t TI1_Selection);
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchronization(TIM_HandleTypeDef *htim, TIM_SlaveConfigTypeDef * sSlaveConfig);
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchronization_IT(TIM_HandleTypeDef *htim, TIM_SlaveConfigTypeDef * sSlaveConfig);
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress, uint32_t BurstRequestSrc, \
                                              uint32_t  *BurstBuffer, uint32_t  BurstLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc);
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress, uint32_t BurstRequestSrc, \
                                              uint32_t  *BurstBuffer, uint32_t  BurstLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc);
HAL_StatusTypeDef HAL_TIM_GenerateEvent(TIM_HandleTypeDef *htim, uint32_t EventSource);
uint32_t HAL_TIM_ReadCapturedValue(TIM_HandleTypeDef *htim, uint32_t Channel);

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group9
 * @{
 */
/* Callback in non blocking modes (Interrupt and DMA) *************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group10
 * @{
 */
/* Peripheral State functions  **************************************************/
HAL_TIM_StateTypeDef HAL_TIM_Base_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_PWM_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_IC_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_OnePulse_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_Encoder_GetState(TIM_HandleTypeDef *htim);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_TIM_H_ */


