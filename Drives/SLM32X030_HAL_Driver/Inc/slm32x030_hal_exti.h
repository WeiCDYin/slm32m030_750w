/**
  ******************************************************************************
  * @file    slm32x030_hal_exti.h
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   Header file of EXTI HAL module.
  ****************************************************************************** 
 **/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_EXTI_H_
#define _SLM32X030_HAL_EXTI_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
 * @{
 */

/** @defgroup EXTI EXTI
 * @brief EXTI HAL module driver
 * @{
 */

/* Exported types ------------------------------------------------------------*/

/** @defgroup EXTI_Exported_Types EXTI Exported Types
 * @{
 */

/**
 * @brief  HAL EXTI Port sel
 */
typedef enum
{
    HAL_EXTI_GPIO_PORT_PA = 0x00UL,
    HAL_EXTI_GPIO_PORT_PB,
    HAL_EXTI_GPIO_PORT_PC,
    HAL_EXTI_GPIO_PORT_PD
} EXTI_PortSelDef;

/**
 * @brief  HAL EXTI common Callback ID enumeration definition
 */
typedef enum
{
    HAL_EXTI_COMMON_CB_ID = 0x00U
} EXTI_CallbackIDTypeDef;

/**
 * @brief  EXTI Configuration structure definition
 */
typedef struct
{
    uint32_t Line;                  /*!< The Exti line to be configured. This parameter
                                            can be a value of @ref EXTI_Line */
    uint32_t PortSel;               /*!< The Exti port to be configured. This parameter
                                            can be a value of @ref EXTI_PortSelDef */
    uint32_t Mode;                  /*!< The Exit Mode to be configured for a core.
                                            This parameter can be a combination of @ref EXTI_Mode */
    uint32_t Trigger;               /*!< The Exti Trigger to be configured. This parameter
                                            can be a value of @ref EXTI_Trigger */
} EXTI_InitTypeDef;

/**
 * @brief  EXTI Handle structure definition
 */
typedef struct
{
    EXTI_TypeDef        *Instance;           /*!< EXTI registers base address */
    EXTI_InitTypeDef    Init;
    uint32_t            Line;
    void (* PendingCallback)(void);          /*!<  Exti pending callback */
} EXTI_HandleTypeDef;

/**
 * @}
 */
/* Exported constants --------------------------------------------------------*/
/** @defgroup EXTI_Exported_Constants EXTI Exported Constants
 * @{
 */

/** @defgroup EXTI_Line  EXTI Line
 * @{
 */
#define EXTI_LINE_0                      (0x00u)  /*!< External interrupt line 0 */
#define EXTI_LINE_1                      (0x01u)  /*!< External interrupt line 1 */
#define EXTI_LINE_2                      (0x02u)  /*!< External interrupt line 2 */
#define EXTI_LINE_3                      (0x03u)  /*!< External interrupt line 3 */
#define EXTI_LINE_4                      (0x04u)  /*!< External interrupt line 4 */
#define EXTI_LINE_5                      (0x05u)  /*!< External interrupt line 5 */
#define EXTI_LINE_6                      (0x06u)  /*!< External interrupt line 6 */
#define EXTI_LINE_7                      (0x07u)  /*!< External interrupt line 7 */
#define EXTI_LINE_8                      (0x08u)  /*!< External interrupt line 8 */
#define EXTI_LINE_9                      (0x09u)  /*!< External interrupt line 9 */
#define EXTI_LINE_10                     (0x0Au)  /*!< External interrupt line 10 */
#define EXTI_LINE_11                     (0x0Bu)  /*!< External interrupt line 11 */
#define EXTI_LINE_12                     (0x0Cu)  /*!< External interrupt line 12 */
#define EXTI_LINE_13                     (0x0Du)  /*!< External interrupt line 13 */
#define EXTI_LINE_14                     (0x0Eu)  /*!< External interrupt line 14 */
#define EXTI_LINE_15                     (0x0Fu)  /*!< External interrupt line 15 */
#define EXTI_LINE_16_LVD_OUTPUT          (0x10u)  /*!< LVD output */
#define EXTI_LINE_17_RTC_ALARM           (0x11u)  /*!< RTC alarm event */
#define EXTI_LINE_18_RTC_CHANGE          (0x12u)  /*!< RTC change event */
#define EXTI_LINE_19_INVASION            (0x13u)  /*!< Invasion event */
#define EXTI_LINE_20_USART1_WAKEUP       (0x14u)  /*!< UART1 wakeup event */
#define EXTI_LINE_21_VC0_OUTPUT          (0x15u)  /*!< VC0 output */
#define EXTI_LINE_22_VC1_OUTPUT          (0x16u)  /*!< VC1 output */
#define EXTI_LINE_23_USART2_WAKEUP       (0x17u)  /*!< UART2 wakeup event */


/**
 * @}
 */

/** @defgroup EXTI_Mode  EXTI Mode
 * @{
 */
#define EXTI_MODE_NONE                  (0x00u)
#define EXTI_MODE_INTERRUPT             (0x01u)
#define EXTI_MODE_EVENT                 (0x02u)


/** @defgroup EXTI_Trigger  EXTI Trigger
 * @{
 */
#define EXTI_TRIGGER_NONE               (0x00u)
#define EXTI_TRIGGER_RISING             (0x01u)
#define EXTI_TRIGGER_FALLING            (0x02u)
#define EXTI_TRIGGER_RISING_FALLING     (EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING)


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

/* Exported macro ------------------------------------------------------------*/
/** @defgroup EXTI_Exported_Macros EXTI Exported Macros
 * @{
 */

/**
 * @}
 */

/* Private constants --------------------------------------------------------*/
/** @defgroup EXTI_Private_Constants EXTI Private Constants
 * @{
 */

/**
 * @brief  EXTI bit usage
 */
 #define EXTI_PIN_MASK (0x0000000Fu)

/**
 * @brief  EXTI Mask for interrupt & event mode
 */
#define EXTI_MODE_MASK (EXTI_MODE_EVENT | EXTI_MODE_INTERRUPT)

/**
 * @brief  EXTI Mask for trigger possibilities
 */
#define EXTI_TRIGGER_MASK (EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING)



/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup EXTI_Private_Macros EXTI Private Macros
 * @{
 */
#define IS_EXTI_INSTANCE(INSTANCE)          ((INSTANCE == IEMU) || (INSTANCE == EXTI))

#define IS_EXTI_LINE(__LINE__)              ((__LINE__) <= EXTI_LINE_23_USART2_WAKEUP)

#define IS_EXTI_MODE(__LINE__)              ((((__LINE__) & EXTI_MODE_MASK) != 0x00u) && (((__LINE__) & ~EXTI_MODE_MASK) == 0x00u))

#define IS_EXTI_TRIGGER(__LINE__)           ((((__LINE__) & EXTI_TRIGGER_MASK) != 0x00u) && (((__LINE__) & ~EXTI_TRIGGER_MASK) == 0x00u))
                                                                
#define IS_EXTI_PENDING_EDGE(__LINE__)      ((__LINE__) == EXTI_TRIGGER_RISING_FALLING)

#define IS_EXTI_GPIO_PORT(__PORT__)         (((__PORT__) == HAL_EXTI_GPIO_PORT_PA) || ((__PORT__) == HAL_EXTI_GPIO_PORT_PB) || \
                                             ((__PORT__) == HAL_EXTI_GPIO_PORT_PC) || ((__PORT__) == HAL_EXTI_GPIO_PORT_PD))

#define IS_EXTI_GPIO_PIN(__PIN__)          ((__PIN__) < 16u)

/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup EXTI_Exported_Functions EXTI Exported Functions
 * @brief    EXTI Exported Functions
 * @{
 */

/** @defgroup EXTI_Exported_Functions_Group1 Configuration functions
 * @brief    Configuration functions
 * @{
 */
/* Configuration functions ****************************************************/
HAL_StatusTypeDef HAL_EXTI_Init(EXTI_HandleTypeDef *hexti);
HAL_StatusTypeDef HAL_EXTI_DeInit(EXTI_HandleTypeDef *hexti);
void HAL_EXTI_MspInit(EXTI_HandleTypeDef *hexti);
void HAL_EXTI_MspDeInit(EXTI_HandleTypeDef *hexti);
HAL_StatusTypeDef HAL_EXTI_ConfigLineMask(EXTI_HandleTypeDef *hexti, uint8_t mode);
HAL_StatusTypeDef HAL_EXTI_ConfigLineClearMask(EXTI_HandleTypeDef *hexti, uint8_t mode);
HAL_StatusTypeDef HAL_EXTI_RegisterCallback(EXTI_HandleTypeDef *hexti, EXTI_CallbackIDTypeDef CallbackID, void (*pPendingCbfn)(void));

/**
 * @}
 */

/** @defgroup EXTI_Exported_Functions_Group2 IO operation functions
 * @brief    IO operation functions
 * @{
 */
/* IO operation functions *****************************************************/
void HAL_EXTI_IRQHandler(EXTI_HandleTypeDef *hexti);
uint32_t HAL_EXTI_GetPending(EXTI_HandleTypeDef *hexti, uint32_t Edge);
void HAL_EXTI_ClearPending(EXTI_HandleTypeDef *hexti, uint32_t Edge);
void HAL_EXTI_GenerateSWI(EXTI_HandleTypeDef *hexti);

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

#endif /* _SLM32X030_HAL_EXTI_H_ */


