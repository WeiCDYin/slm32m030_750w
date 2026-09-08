/**
  ******************************************************************************
  * @file    slm32x030_hal_flash.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-11-2024
  * @brief   Header file of Flash HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_FLASH_H_
#define _SLM32X030_HAL_FLASH_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"
   
/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */
  
/**
  * @brief  Flash Access Control Register£¨FLASH_ACR£©
  */
typedef enum 
{
    HCLK_0_32MHZ                = 0U, 
    HCLK_32_64MHZ               = 1U, 
    HCLK_DISABLE                = 2U
}FLASH_ACR_LATENCY_TypeDef;

/**
  * @brief  Flash controller key register£¨FLASH_KEYR£©
  *
  */

/** @addtogroup FLASH_Private_Constants
  * @{
  */
#define FLASH_TIMEOUT_VALUE          (5000U) /* 5 s */
/**
  * @}
  */

/** @addtogroup FLASH_Private_Macros
  * @{
  */

#define IS_FLASH_TYPEPROGRAM(VALUE)         (((VALUE) == FLASH_TYPEPROGRAM_HALFWORD) || \
                                             ((VALUE) == FLASH_TYPEPROGRAM_WORD)     || \
                                             ((VALUE) == FLASH_TYPEPROGRAM_DOUBLEWORD))  

#if   defined(FLASH_ACR_LATENCY)
    #define IS_FLASH_LATENCY(__LATENCY__)   (((__LATENCY__) == FLASH_LATENCY_0) || \
                                             ((__LATENCY__) == FLASH_LATENCY_1))
#else
    #define IS_FLASH_LATENCY(__LATENCY__)   ((__LATENCY__) == FLASH_LATENCY_0)
#endif /* FLASH_ACR_LATENCY */
/**
  * @}
  */  

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */  

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum 
{
    FLASH_PROC_NONE              = 0U, 
    FLASH_PROC_PAGEERASE         = 1U,
    FLASH_PROC_MASSERASE         = 2U,
    FLASH_PROC_PROGRAMHALFWORD   = 3U,
    FLASH_PROC_PROGRAMWORD       = 4U,
    FLASH_PROC_PROGRAMDOUBLEWORD = 5U
} FLASH_ProcedureTypeDef;

/** 
  * @brief  FLASH handle Structure definition  
  */
typedef struct
{
    __IO FLASH_ProcedureTypeDef ProcedureOnGoing; /*!< Internal variable to indicate which procedure is ongoing or not in IT context */

    __IO uint32_t               DataRemaining;    /*!< Internal variable to save the remaining pages to erase or half-word to program in IT context */

    __IO uint32_t               Address;          /*!< Internal variable to save address selected for program or erase */

    __IO uint64_t               Data;             /*!< Internal variable to save data to be programmed */

    HAL_LockTypeDef             Lock;             /*!< FLASH locking object */

    __IO uint32_t               ErrorCode;        /*!< FLASH error code. This parameter can be a value of @ref FLASH_Error_Codes  */
} FLASH_ProcessTypeDef;


/** @defgroup FLASH_KEY
  * @{
  */
#define RDP_KEY_Pos                         (0U)                               
#define RDP_KEY_Msk                         (0xA5UL << RDP_KEY_Pos)             /*!< 0x000000A5 */
#define RDP_KEY                             RDP_KEY_Msk                         /*!< RDP Key */
#define FLASH_KEY1_Pos                      (0U)                               
#define FLASH_KEY1_Msk                      (0x45670123UL << FLASH_KEY1_Pos)    /*!< 0x45670123 */
#define FLASH_KEY1                          FLASH_KEY1_Msk                      /*!< FPEC Key1 */
#define FLASH_KEY2_Pos                      (0U)                               
#define FLASH_KEY2_Msk                      (0xCDEF89ABUL << FLASH_KEY2_Pos)    /*!< 0xCDEF89AB */
#define FLASH_KEY2                          FLASH_KEY2_Msk                      /*!< FPEC Key2 */
#define  FLASH_OPTKEY1                      FLASH_KEY1                          /*!< Option Byte Key1 */
#define  FLASH_OPTKEY2                      FLASH_KEY2                          /*!< Option Byte Key2 */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */  

/** @defgroup FLASH_Error_Codes FLASH Error Codes
  * @{
  */

#define HAL_FLASH_ERROR_NONE                  (0x00U)  /*!< No error */
#define HAL_FLASH_ERROR_PROG                  (0x01U)  /*!< Programming error */
#define HAL_FLASH_ERROR_WRP                   (0x02U)  /*!< Write protection error */
#define HAL_FLASH_ERROR_OPTV                  (0x04U)  /*!< Option validity error */

/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */ 
#define FLASH_TYPEPROGRAM_HALFWORD             (0x01U)  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD                 (0x02U)  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD           (0x03U)  /*!<Program a double word (64-bit) at a specified address*/

/**
  * @}
  */

#if defined(FLASH_ACR_LATENCY)
/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
    #define FLASH_LATENCY_0                     (0x00000000U)  /*!< FLASH Zero Latency cycle */
    #define FLASH_LATENCY_1                     (0x00000001U)  /*!< FLASH One Latency cycle */

/**
  * @}
  */

#else
/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
    #define FLASH_LATENCY_0                     (0x00000000U)    /*!< FLASH Zero Latency cycle */

/**
  * @}
  */

#endif /* FLASH_ACR_LATENCY */
/**
  * @}
  */  
  
/* Exported macro ------------------------------------------------------------*/

/** @defgroup FLASH_Exported_Macros FLASH Exported Macros
 *  @brief macros to control FLASH features 
 *  @{
 */
 
#if defined(FLASH_ACR_LATENCY)
/** @defgroup FLASH_EM_Latency FLASH Latency
 *  @brief macros to handle FLASH Latency
 * @{
 */ 
  
/**
  * @brief  Set the FLASH Latency.
  * @param  __LATENCY__ FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  * @retval None
  */ 
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    (FLASH->ACR = (FLASH->ACR&(~FLASH_ACR_LATENCY)) | (__LATENCY__))


/**
  * @brief  Get the FLASH Latency.
  * @retval FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  */ 
#define __HAL_FLASH_GET_LATENCY()               (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY))

/**
  * @}
  */

#endif /* FLASH_ACR_LATENCY */
  
/**
  * @}
  */ 

/* Include FLASH HAL Extended module */
#include "slm32x030_hal_flash_ex.h"  

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions
  * @{
  */
  
/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

/* FLASH IRQ handler function */
void       HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */ 
void       HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void       HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);
void HAL_FLASH_OB_Launch(void);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
uint32_t HAL_FLASH_GetError(void);

/**
  * @}
  */

/**
  * @}
  */

/* Private function -------------------------------------------------*/
/** @addtogroup FLASH_Private_Functions
 * @{
 */
HAL_StatusTypeDef       FLASH_WaitForLastOperation(uint32_t Timeout);


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

#endif /* _SLM32X030_HAL_FLASH_H_ */



