/**
  ******************************************************************************
  * @file    slm32x030_hal_pmu.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024 
  * @brief   Header file of PMU HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_PMU_H_
#define _SLM32X030_HAL_PMU_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup PMU
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup PMU_Exported_Types PMU Exported Types
  * @{
  */ 

/**
  * @brief  PMU EXIT configuration structure definition
  */
typedef struct
{
    uint32_t PVDLevel;          /*!< PVDLevel: Specifies the PVD detection level.
                                    This parameter can be a value of @ref PMU_PVD_detection_level */
    uint32_t Mode;              /*!< Mode: Specifies the operating mode for the selected pins.
                                    This parameter can be a value of @ref PMU_PVD_Mode */
    uint32_t ExitGpioPortType;  /*!< cfg exit gpio port*/
    uint32_t ExitGpioPin;       /*!< cfg exit gpio pin */
    uint32_t MRx_IntMask;       /*!< interrupt mask cfg */
    uint32_t EMRx_EventMask;    /*!< event mask cfg */
    uint32_t RisingTRx;         /*!< event rising select */
    uint32_t FallingTRx;        /*!< event falling select */
}PMU_EXTI_InitTypeDef;


/**
  * @}
  */


/* Internal constants --------------------------------------------------------*/

/** @addtogroup PMU_Private_Constants
  * @{
  */ 

#define PMU_EXTI_LINE_PVD                   ((uint32_t)0x00FFFFFF)  /*!< External interrupt line 16 Connected to the PVD EXTI Line */

/**
  * @}
  */

 
/* Exported constants --------------------------------------------------------*/

                                                          
/**
  * @}
  */

/** @defgroup PMU_PVD_Mode PMU PVD Mode
  * @{
  */
#define PMU_PVD_MODE_NORMAL                 (0x00000000U)   /*!< basic mode is used */
#define PMU_PVD_MODE_IT_RISING              (0x00010001U)   /*!< External Interrupt Mode with Rising edge trigger detection */
#define PMU_PVD_MODE_IT_FALLING             (0x00010002U)   /*!< External Interrupt Mode with Falling edge trigger detection */
#define PMU_PVD_MODE_IT_RISING_FALLING      (0x00010003U)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define PMU_PVD_MODE_EVENT_RISING           (0x00020001U)   /*!< Event Mode with Rising edge trigger detection */
#define PMU_PVD_MODE_EVENT_FALLING          (0x00020002U)   /*!< Event Mode with Falling edge trigger detection */
#define PMU_PVD_MODE_EVENT_RISING_FALLING   (0x00020003U)   /*!< Event Mode with Rising/Falling edge trigger detection */

/** @defgroup PMU_WakeUp_Pins PMU WakeUp Pins
  * @{
  */

#define PMU_WAKEUP_PIN1                     PMU_CSR_EWUP1
#define PMU_WAKEUP_PIN2                     PMU_CSR_EWUP2
#define PMU_WAKEUP_PIN4                     PMU_CSR_EWUP4
#define PMU_WAKEUP_PIN7                     PMU_CSR_EWUP7
/**
  * @}
  */

/** @defgroup PMU_Regulator_state_in_SLEEP_STOP_mode PMU Regulator state in SLEEP/STOP mode
  * @{
  */
#define PMU_MAINREGULATOR_ON                 (0x00000000U)
#define PMU_LOWPOWERREGULATOR_ON             PMU_CR_LPDS

/**
  * @}
  */

/** @defgroup PMU_SLEEP_mode_entry PMU SLEEP mode entry
  * @{
  */
#define PMU_SLEEPENTRY_WFI                  ((uint8_t)0x01)
#define PMU_SLEEPENTRY_WFE                  ((uint8_t)0x02)

/**
  * @}
  */

/** @defgroup PMU_STOP_mode_entry PMU STOP mode entry
  * @{
  */
#define PMU_STOPENTRY_WFI                   ((uint8_t)0x01)
#define PMU_STOPENTRY_WFE                   ((uint8_t)0x02)

/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PMU_Exported_Macros PMU Exported Macros
  * @{
  */

/** @brief  Check PMU flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *           This parameter can be one of the following values:
  *            @arg PMU_FLAG_WU: Wake Up flag. This flag indicates that a wakeup event
  *                  was received from the WKUP pin or from the RTC alarm
  *                  An additional wakeup event is detected if the WKUP pin is enabled
  *                  (by setting the EWUP bit) when the WKUP pin level is already high.
  *            @arg PMU_FLAG_SB: StandBy flag. This flag indicates that the system was
  *                  resumed from StandBy mode.
  *            @arg PMU_FLAG_PVDO: PVD Output. This flag is valid only if PVD is enabled
  *                  by the HAL_PMU_EnablePVD() function. The PVD is stopped by Standby mode
  *                  For this reason, this bit is equal to 0 after Standby or reset
  *                  until the PVDE bit is set.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_PMU_GET_FLAG(__FLAG__)        ((PMU->CSR & (__FLAG__)) == (__FLAG__))

/** @brief  Clear the PMU's pending flags.
  * @param  __FLAG__: specifies the flag to clear.
  *          This parameter can be one of the following values:
  *            @arg PMU_FLAG_WU: Wake Up flag
  *            @arg PMU_FLAG_SB: StandBy flag
  */
#define __HAL_PMU_CLEAR_FLAG(__FLAG__)      SET_BIT(PMU->CR, ((__FLAG__) << 2))

/**
  * @brief Enable interrupt on PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_ENABLE_IT()      SET_BIT(EXTI->IMR, PMU_EXTI_LINE_PVD)

/**
  * @brief Disable interrupt on PVD Exti Line 16. 
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_DISABLE_IT()     CLEAR_BIT(EXTI->IMR, PMU_EXTI_LINE_PVD)

/**
  * @brief Enable event on PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_ENABLE_EVENT()   SET_BIT(EXTI->EMR, PMU_EXTI_LINE_PVD)

/**
  * @brief Disable event on PVD Exti Line 16.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_DISABLE_EVENT()  CLEAR_BIT(EXTI->EMR, PMU_EXTI_LINE_PVD)


/**
  * @brief  PVD EXTI line configuration: set falling edge trigger.  
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_ENABLE_FALLING_EDGE()  SET_BIT(EXTI->FTSR, PMU_EXTI_LINE_PVD)


/**
  * @brief Disable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_DISABLE_FALLING_EDGE()  CLEAR_BIT(EXTI->FTSR, PMU_EXTI_LINE_PVD)


/**
  * @brief  PVD EXTI line configuration: set rising edge trigger.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_ENABLE_RISING_EDGE()   SET_BIT(EXTI->RTSR, PMU_EXTI_LINE_PVD)

/**
  * @brief Disable the PVD Extended Interrupt Rising Trigger.
  * This parameter can be:
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_DISABLE_RISING_EDGE()  CLEAR_BIT(EXTI->RTSR, PMU_EXTI_LINE_PVD)

/**
  * @brief  PVD EXTI line configuration: set rising & falling edge trigger.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_ENABLE_RISING_FALLING_EDGE()   __HAL_PMU_PVD_EXTI_ENABLE_RISING_EDGE();__HAL_PMU_PVD_EXTI_ENABLE_FALLING_EDGE();

/**
  * @brief Disable the PVD Extended Interrupt Rising & Falling Trigger.
  * This parameter can be:
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_DISABLE_RISING_FALLING_EDGE()  __HAL_PMU_PVD_EXTI_DISABLE_RISING_EDGE();__HAL_PMU_PVD_EXTI_DISABLE_FALLING_EDGE();



/**
  * @brief Check whether the specified PVD EXTI interrupt flag is set or not.
  * @retval EXTI PVD Line Status.
  */
#define __HAL_PMU_PVD_EXTI_GET_FLAG()       (EXTI->PR & (PMU_EXTI_LINE_PVD))

/**
  * @brief Clear the PVD EXTI flag.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_CLEAR_FLAG()     (EXTI->PR = (PMU_EXTI_LINE_PVD))

/**
  * @brief Generate a Software interrupt on selected EXTI line.
  * @retval None.
  */
#define __HAL_PMU_PVD_EXTI_GENERATE_SWIT()  SET_BIT(EXTI->SWIER, PMU_EXTI_LINE_PVD)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup PMU_Private_Macros PMU Private Macros
  * @{
  */
#define IS_PMU_PVD_LEVEL(LEVEL) (((LEVEL) == PMU_PVDLEVEL_0) || ((LEVEL) == PMU_PVDLEVEL_1) || \
                                 ((LEVEL) == PMU_PVDLEVEL_2) || ((LEVEL) == PMU_PVDLEVEL_3) || \
                                 ((LEVEL) == PMU_PVDLEVEL_4) || ((LEVEL) == PMU_PVDLEVEL_5) || \
                                 ((LEVEL) == PMU_PVDLEVEL_6) || ((LEVEL) == PMU_PVDLEVEL_7))


#define IS_PMU_PVD_MODE(MODE)  (((MODE) == PMU_PVD_MODE_IT_RISING)|| ((MODE) == PMU_PVD_MODE_IT_FALLING) || \
                                ((MODE) == PMU_PVD_MODE_IT_RISING_FALLING) || ((MODE) == PMU_PVD_MODE_EVENT_RISING) || \
                                ((MODE) == PMU_PVD_MODE_EVENT_FALLING) || ((MODE) == PMU_PVD_MODE_EVENT_RISING_FALLING) || \
                                ((MODE) == PMU_PVD_MODE_NORMAL)) 

#define IS_PMU_WAKEUP_PIN(PIN) (((PIN) == PMU_WAKEUP_PIN1) || ((PIN) == PMU_WAKEUP_PIN2)  || ((PIN) == PMU_WAKEUP_PIN4)   || ((PIN) == PMU_WAKEUP_PIN7))

#define IS_PMU_REGULATOR(REGULATOR)     (((REGULATOR) == PMU_MAINREGULATOR_ON) || \
                                         ((REGULATOR) == PMU_LOWPOWERREGULATOR_ON))

#define IS_PMU_SLEEP_ENTRY(ENTRY)       (((ENTRY) == PMU_SLEEPENTRY_WFI) || ((ENTRY) == PMU_SLEEPENTRY_WFE))

#define IS_PMU_STOP_ENTRY(ENTRY)        (((ENTRY) == PMU_STOPENTRY_WFI) || ((ENTRY) == PMU_STOPENTRY_WFE))

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/

/** @addtogroup PMU_Exported_Functions PMU Exported Functions
  * @{
  */
  
/** @addtogroup PMU_Exported_Functions_Group1 Initialization and de-initialization functions 
  * @{
  */

/* Initialization and de-initialization functions *******************************/
void HAL_PMU_DeInit(void);
void HAL_PMU_EnableBkUpAccess(void);
void HAL_PMU_DisableBkUpAccess(void);

/**
  * @}
  */

/** @addtogroup PMU_Exported_Functions_Group2 Peripheral Control functions 
  * @{
  */

/* Peripheral Control functions  ************************************************/
void HAL_PMU_EXIT_Init(PMU_EXTI_InitTypeDef *exitInit);
/* #define HAL_PMU_ConfigPVD 12*/
void HAL_PMU_EnablePVD(void);
void HAL_PMU_DisablePVD(void);

/* WakeUp pins configuration functions ****************************************/
void HAL_PMU_EnableWakeUpPin(uint32_t WakeUpPinx);
void HAL_PMU_DisableWakeUpPin(uint32_t WakeUpPinx);

/* Low Power modes configuration functions ************************************/
void HAL_PMU_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);
void HAL_PMU_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);
void HAL_PMU_EnterSTANDBYMode(void);

void HAL_PMU_EnableSleepOnExit(void);
void HAL_PMU_DisableSleepOnExit(void);
void HAL_PMU_EnableSEVOnPend(void);
void HAL_PMU_DisableSEVOnPend(void);

void HAL_PMU_PVD_IRQHandler(void);
void HAL_PMU_PVDCallback(void);
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


#endif /* _SLM32X030_HAL_PMU_H_ */

