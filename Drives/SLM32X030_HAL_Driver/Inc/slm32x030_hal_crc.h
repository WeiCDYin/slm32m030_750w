/**
  ******************************************************************************
  * @file    slm32x030_hal_crc.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of CRC HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_CRC_H_
#define _SLM32X030_HAL_CRC_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** @defgroup CRC_Exported_Types CRC Exported Types
  * @{
  */

/** 
  * @brief  CRC HAL State Structure definition  
  */ 
typedef enum
{
    HAL_CRC_STATE_RESET     = 0x00U,  /*!< CRC not yet initialized or disabled */
    HAL_CRC_STATE_READY     = 0x01U,  /*!< CRC initialized and ready for use   */
    HAL_CRC_STATE_BUSY      = 0x02U,  /*!< CRC internal process is ongoing     */
    HAL_CRC_STATE_TIMEOUT   = 0x03U,  /*!< CRC timeout state                   */
    HAL_CRC_STATE_ERROR     = 0x04U   /*!< CRC error state                     */
}HAL_CRC_StateTypeDef;

/**
  * @brief  CRC is written to data inversely
  */
typedef enum
{
    CRC_CR_INV_IN_NOREV = 0x00U,
    CRC_CR_INV_IN_REV   = 0x01U
} HAL_Inv_In_TypeDef;

/** 
  * @brief  Each CRC read data is reversed
  */
typedef enum
{
    CRC_CR_INV_OUT_NOREV = 0x00U,
    CRC_CR_INV_OUT_REV   = 0x01U
} HAL_Inv_Out_TypeDef;
/**
  * @}
  */
/** 
  * @brief  Inverts each byte of CRC written data
  */
typedef enum
{
    CRC_CR_REV_IN_NOREV = 0x00U,
    CRC_CR_REV_IN_REV   = 0x01U
} HAL_Rev_In_TypeDef;
/**
  * @}
  */ 
/** 
  * @brief  Invert each byte of CRC read data
  */
typedef enum
{
    CRC_CR_REV_OUT_NOREV = 0x00U,
    CRC_CR_REV_OUT_REV   = 0x01U
} HAL_Rev_Out_TypeDef;
/**
  * @}
  */ 

/**
  * @brief  CRC Configuration Structure definition
  */
typedef struct
{
    uint8_t                                                 Mode;       /*!< CRC mode   This parameter can be a value of @ref CRC_Mode*/
    HAL_Inv_In_TypeDef                                      Inv_in;     /*Write CRC into data and invert each data point */
    HAL_Inv_Out_TypeDef                                     Inv_out;    /*Invert each CRC read data*/
    HAL_Rev_In_TypeDef                                      Rev_in;     /*Invert the number of bits within each byte of CRC written data*/
    HAL_Rev_Out_TypeDef                                     Rev_out;    /*Invert the number of bits within each byte of CRC read data*/
}CRC_InitTypeDef;

/** 
  * @brief  CRC handle Structure definition
  */ 
typedef struct
{
    CRC_TypeDef                                             *Instance;  /*!< Register base address   */
    HAL_LockTypeDef                                         Lock;       /*!< CRC locking object      */
    CRC_InitTypeDef                                         Init;       /*!< CRC communication parameters */
    __IO HAL_CRC_StateTypeDef                               State;      /*!< CRC communication state */    
}CRC_HandleTypeDef;

/**
  * @}
  */ 

/** @defgroup CRC_Mode crc Mode
  * @{
  */
#define CRC_MODE_32                                         CRC_CR_MODE_CRC32
#define CRC_MODE_16                                         CRC_CR_MODE_CRC16
#define CRC_MODE_CCITT                                      CRC_CR_MODE_CRC_CCITT
#define CRC_MODE_8                                          CRC_CR_MODE_CRC_8

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup CRC_Exported_Macros CRC Exported Macros
  * @{
  */

/** @defgroup CRC INSTANCE
  * @{
  */
#define IS_CRC_ALL_INSTANCE(INSTANCE)                       ((INSTANCE) == CRC)

/** @brief Reset CRC handle state
  * @param  __HANDLE__: CRC handle
  * @retval None
  */
    
    /** @defgroup CRC MODE
  * @{
  */
#define IS_CRC_MODE(MODE)                                   (((MODE) == CRC_CR_MODE_CRC_8) || ((MODE) == CRC_CR_MODE_CRC_CCITT) || \
                                                             ((MODE) == CRC_CR_MODE_CRC16) || ((MODE) == CRC_CR_MODE_CRC32))
    /** @brief Select the CRC mode
  * @param  __HANDLE__: CRC handle
  * @retval None
  */                                                    
#define IS_CRC_INV_IN(IN)                                   ((IN) == CRC_CR_INV_IN_REV||CRC_CR_INV_IN_NOREV)
#define IS_CRC_REV_IN(IN)                                   ((IN) == CRC_CR_REV_IN_REV||CRC_CR_REV_IN_NOREV)

/** @brief Reset CRC handle state
  * @param  __HANDLE__: CRC handle
  * @retval None
  */
#define IS_CRC_INV_OUT(OUT)                                 ((OUT) == CRC_CR_INV_OUT_REV||CRC_CR_INV_OUT_NOREV)
#define __HAL_CRC_RESET_HANDLE_STATE(__HANDLE__)            ((__HANDLE__)->State = HAL_CRC_STATE_RESET)

/**
  * @brief  Resets CRC Data Register.
  * @param  __HANDLE__: CRC handle
  * @retval None
  */
#define __HAL_CRC_DR_RESET(__HANDLE__)                      (SET_BIT((__HANDLE__)->Instance->CR,CRC_CR_RST))

/**
  * @brief Stores a 8-bit data in the Independent Data(ID) register.
  * @param __HANDLE__: CRC handle
  * @param __VALUE__: 8-bit value to be stored in the ID register
  * @retval None
  */
    
#define __HAL_CRC_SET_CR_MODE(__HANDLE__,__VALUE__)         (SET_BIT((__HANDLE__)->Instance->CR,(__VALUE__)))

/**
  * @brief Stores a 8-bit data in the Independent Data(ID) register.
  * @param __HANDLE__: CRC handle
  * @param __VALUE__: 8-bit value to be stored in the ID register
  * @retval None
  */
#define __HAL_CRC_SET_CR_INV_INRES(__HANDLE__)          (SET_BIT((__HANDLE__)->Instance->CR,CRC_CR_INV_IN))
#define __HAL_CRC_SET_CR_INV_IN(__HANDLE__)             (CLEAR_BIT((__HANDLE__)->Instance->CR,CRC_CR_INV_IN))

#define __HAL_CRC_SET_CR_REV_INRES(__HANDLE__)          (SET_BIT((__HANDLE__)->Instance->CR,CRC_CR_REV_IN))
#define __HAL_CRC_SET_CR_REV_IN(__HANDLE__)             (CLEAR_BIT((__HANDLE__)->Instance->CR,CRC_CR_REV_IN))

#define __HAL_CRC_SET_CR_INV_OUTRES(__HANDLE__)         (SET_BIT((__HANDLE__)->Instance->CR,CRC_CR_INV_OUT))
#define __HAL_CRC_SET_CR_INV_OUT(__HANDLE__)            (CLEAR_BIT((__HANDLE__)->Instance->CR,CRC_CR_INV_OUT))

#define __HAL_CRC_SET_CR_REV_OUTRES(__HANDLE__)         (SET_BIT((__HANDLE__)->Instance->CR,CRC_CR_REV_OUT))
#define __HAL_CRC_SET_CR_REV_OUT(__HANDLE__)            (CLEAR_BIT((__HANDLE__)->Instance->CR,CRC_CR_REV_OUT))

/**
  * @brief Stores a 8-bit data in the Independent Data(ID) register.
  * @param __HANDLE__: CRC handle
  * @param __VALUE__: 8-bit value to be stored in the ID register
  * @retval None
  */
    
#define __HAL_CRC_SET_IDR(__HANDLE__, __VALUE__) (WRITE_REG((__HANDLE__)->Instance->IDR, (__VALUE__)))

/**
  * @brief Returns the 8-bit data stored in the Independent Data(ID) register.
  * @param __HANDLE__: CRC handle
  * @retval 8-bit value of the ID register 
  */
#define __HAL_CRC_GET_IDR(__HANDLE__) (((__HANDLE__)->Instance->IDR) & CRC_IDR_IDR)

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/

/** @addtogroup CRC_Exported_Functions
  * @{
  */ 

/** @addtogroup CRC_Exported_Functions_Group1
  * @{
  */ 

/* Initialization/de-initialization functions  **********************************/
HAL_StatusTypeDef     HAL_CRC_Init(CRC_HandleTypeDef *hcrc);
HAL_StatusTypeDef     HAL_CRC_DeInit (CRC_HandleTypeDef *hcrc);
void                  HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc);
void                  HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc);

/**
  * @}
  */ 

/** @addtogroup CRC_Exported_Functions_Group2
  * @{
  */ 

/* Peripheral Control functions  ************************************************/
uint32_t              HAL_CRC_Accumulate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);
uint32_t              HAL_CRC_Calculate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);
uint32_t              endianSwap32(uint32_t value);
/**
  * @}
  */ 

/** @addtogroup CRC_Exported_Functions_Group3
  ** @{
  */ 

/* Peripheral State functions  **************************************************/
HAL_CRC_StateTypeDef  HAL_CRC_GetState(CRC_HandleTypeDef *hcrc);

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

#endif /* _SLM32X030_HAL_CRC_H_ */
