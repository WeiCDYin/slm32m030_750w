/**
  ******************************************************************************
  * @file    slm32x030_hal_trng.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of TRNG HAL module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_TRNG_H_
#define _SLM32X030_HAL_TRNG_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup TRNG
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 

/** @defgroup TRNG_Exported_Types TRNG Exported Types
  * @{
  */

/** @defgroup TRNG_Exported_Types_Group1 TRNG State Structure definition 
  * @{
  */
typedef enum
{
    HAL_TRNG_STATE_RESET     = 0x00U,  /*!< TRNG not yet initialized or disabled */
    HAL_TRNG_STATE_READY     = 0x01U,  /*!< TRNG initialized and ready for use   */
    HAL_TRNG_STATE_BUSY      = 0x02U,  /*!< TRNG internal process is ongoing     */ 
    HAL_TRNG_STATE_TIMEOUT   = 0x03U,  /*!< TRNG timeout state                   */
    HAL_TRNG_STATE_ERROR     = 0x04U   /*!< TRNG error state                     */    
}HAL_TRNG_StateTypeDef;

/** 
  * @}
  */

/** @defgroup TRNG_Exported_Types_Group2 TRNG Handle Structure definition   
  * @{
  */ 
typedef struct
{
    TRNG_TypeDef                 *Instance;    /*!< Register base address   */
    HAL_LockTypeDef             Lock;         /*!< TRNG locking object      */
    __IO HAL_TRNG_StateTypeDef  State;        /*!< TRNG communication state */
    uint32_t                    RandomNumber; /*!< Last Generated TRNG Data */
}TRNG_HandleTypeDef;

/** 
  * @}
  */

/**
  * @}
  */ 
   
/* Exported constants --------------------------------------------------------*/

/** @defgroup TRNG_Exported_Constants TRNG Exported Constants
  * @{
  */

/** @defgroup TRNG_Exported_Constants_Group1 TRNG Interrupt definition
  * @{
  */
#define TRNG_IT_DRDY        TRNG_SR_DRDY  /*!< Data Ready interrupt  */
#define TRNG_IT_CEI         TRNG_SR_CEIS  /*!< Clock error interrupt */
#define TRNG_IT_SEI         TRNG_SR_SEIS  /*!< Seed error interrupt  */
/**
  * @}
  */

/** @defgroup TRNG_Exported_Constants_Group2 TRNG Flag definition
  * @{
  */
#define TRNG_FLAG_DRDY      TRNG_SR_DRDY  /*!< Data ready                 */
#define TRNG_FLAG_CECS      TRNG_SR_CECS  /*!< Clock error current status */
#define TRNG_FLAG_SECS      TRNG_SR_SECS  /*!< Seed error current status  */

/**
  * @}
  */

/**
  * @}
  */ 
  
/* Exported macros -----------------------------------------------------------*/

/** @defgroup TRNG_Exported_Macros TRNG Exported Macros
  * @{
  */

/** @brief Reset TRNG handle state
  * @param  __HANDLE__ TRNG Handle
  * @retval None
  */
#define __HAL_TRNG_RESET_HANDLE_STATE(__HANDLE__)       ((__HANDLE__)->State = HAL_TRNG_STATE_RESET)

/**
  * @brief  Enables the TRNG peripheral.
  * @param  __HANDLE__ TRNG Handle
  * @retval None
  */
#define __HAL_TRNG_ENABLE(__HANDLE__)                   ((__HANDLE__)->Instance->CR |=  TRNG_CR_TRNGEN)

/**
  * @brief  Disables the TRNG peripheral.
  * @param  __HANDLE__ TRNG Handle
  * @retval None
  */
#define __HAL_TRNG_DISABLE(__HANDLE__)                  ((__HANDLE__)->Instance->CR &= ~TRNG_CR_TRNGEN)

/**
  * @brief  Check the selected TRNG flag status.
  * @param  __HANDLE__ TRNG Handle
  * @param  __FLAG__ TRNG flag
  *          This parameter can be one of the following values:
  *            @arg TRNG_FLAG_DRDY: Data ready                
  *            @arg TRNG_FLAG_CECS: Clock error current status
  *            @arg TRNG_FLAG_SECS: Seed error current status 
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_TRNG_GET_FLAG(__HANDLE__, __FLAG__)       (((__HANDLE__)->Instance->SR & (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clears the selected TRNG flag status.
  * @param  __HANDLE__ TRNG handle
  * @param  __FLAG__ TRNG flag to clear  
  * @note   WARNING: This is a dummy macro for HAL code alignment,
  *         flags TRNG_FLAG_DRDY, TRNG_FLAG_CECS and TRNG_FLAG_SECS are read-only.
  * @retval None
  */
#define __HAL_TRNG_CLEAR_FLAG(__HANDLE__, __FLAG__)     /* dummy  macro */



/**
  * @brief  Enables the TRNG interrupts.
  * @param  __HANDLE__ TRNG Handle
  * @retval None
  */
#define __HAL_TRNG_ENABLE_IT(__HANDLE__)                ((__HANDLE__)->Instance->CR |=  TRNG_CR_IE)
    
/**
  * @brief  Disables the TRNG interrupts.
  * @param  __HANDLE__ TRNG Handle
  * @retval None
  */
#define __HAL_TRNG_DISABLE_IT(__HANDLE__)               ((__HANDLE__)->Instance->CR &= ~TRNG_CR_IE)

/**
  * @brief  Checks whether the specified TRNG interrupt has occurred or not.
  * @param  __HANDLE__ TRNG Handle
  * @param  __INTERRUPT__ specifies the TRNG interrupt status flag to check.
  *         This parameter can be one of the following values:
  *            @arg TRNG_IT_DRDY: Data ready interrupt              
  *            @arg TRNG_IT_CEI: Clock error interrupt
  *            @arg TRNG_IT_SEI: Seed error interrupt
  * @retval The new state of __INTERRUPT__ (SET or RESET).
  */
#define __HAL_TRNG_GET_IT(__HANDLE__, __INTERRUPT__)    (((__HANDLE__)->Instance->SR & (__INTERRUPT__)) == (__INTERRUPT__))   

/**
  * @brief  Clear the TRNG interrupt status flags.
  * @param  __HANDLE__ TRNG Handle
  * @param  __INTERRUPT__ specifies the TRNG interrupt status flag to clear.
  *          This parameter can be one of the following values:            
  *            @arg TRNG_IT_CEI: Clock error interrupt
  *            @arg TRNG_IT_SEI: Seed error interrupt
  * @note   TRNG_IT_DRDY flag is read-only, reading TRNG_DR register automatically clears TRNG_IT_DRDY.          
  * @retval None
  */
#define __HAL_TRNG_CLEAR_IT(__HANDLE__, __INTERRUPT__)  (((__HANDLE__)->Instance->SR) = ~(__INTERRUPT__))

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
/** @defgroup TRNG_Exported_Functions TRNG Exported Functions
  * @{
  */

/** @defgroup TRNG_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */  
HAL_StatusTypeDef HAL_TRNG_Init(TRNG_HandleTypeDef *htrng);
HAL_StatusTypeDef HAL_TRNG_DeInit (TRNG_HandleTypeDef *htrng);
void HAL_TRNG_MspInit(TRNG_HandleTypeDef *htrng);
void HAL_TRNG_MspDeInit(TRNG_HandleTypeDef *htrng);

/**
  * @}
  */ 

/** @defgroup TRNG_Exported_Functions_Group2 Peripheral Control functions
  * @{
  */
uint32_t HAL_TRNG_GetRandomNumber(TRNG_HandleTypeDef *htrng);    /* Obsolete, use HAL_TRNG_GenerateRandomNumber() instead    */
uint32_t HAL_TRNG_GetRandomNumber_IT(TRNG_HandleTypeDef *htrng); /* Obsolete, use HAL_TRNG_GenerateRandomNumber_IT() instead */

HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber(TRNG_HandleTypeDef *htrng, uint32_t *random32bit);
HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber_IT(TRNG_HandleTypeDef *htrng);
uint32_t HAL_TRNG_ReadLastRandomNumber(TRNG_HandleTypeDef *htrng);

void HAL_TRNG_IRQHandler(TRNG_HandleTypeDef *htrng);
void HAL_TRNG_ErrorCallback(TRNG_HandleTypeDef *htrng);
void HAL_TRNG_ReadyDataCallback(TRNG_HandleTypeDef* htrng, uint32_t random32bit);

/**
  * @}
  */ 

/** @defgroup TRNG_Exported_Functions_Group3 Peripheral State functions
  * @{
  */
HAL_TRNG_StateTypeDef HAL_TRNG_GetState(TRNG_HandleTypeDef *htrng);

/**
  * @}
  */
  
/**
  * @}
  */ 

/* Private types -------------------------------------------------------------*/
/** @defgroup TRNG_Private_Types TRNG Private Types
  * @{
  */

/**
  * @}
  */ 

/* Private defines -----------------------------------------------------------*/
/** @defgroup TRNG_Private_Defines TRNG Private Defines
  * @{
  */

/**
  * @}
  */ 
          
/* Private variables ---------------------------------------------------------*/
/** @defgroup TRNG_Private_Variables TRNG Private Variables
  * @{
  */

/**
  * @}
  */ 

/* Private constants ---------------------------------------------------------*/
/** @defgroup TRNG_Private_Constants TRNG Private Constants
  * @{
  */

/**
  * @}
  */ 

/* Private macros ------------------------------------------------------------*/
/** @defgroup TRNG_Private_Macros TRNG Private Macros
  * @{
  */
#define IS_TRNG_IT(IT)              (((IT) == TRNG_IT_CEI) || ((IT) == TRNG_IT_SEI))

#define IS_TRNG_FLAG(FLAG)          (((FLAG) == TRNG_FLAG_DRDY) || ((FLAG) == TRNG_FLAG_CECS) || ((FLAG) == TRNG_FLAG_SECS))

/**
  * @}
  */ 

/* Private functions prototypes ----------------------------------------------*/
/** @defgroup TRNG_Private_Functions_Prototypes TRNG Private Functions Prototypes
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup TRNG_Private_Functions TRNG Private Functions
  * @{
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


#endif /* _SLM32X030_HAL_TRNG_H_ */

