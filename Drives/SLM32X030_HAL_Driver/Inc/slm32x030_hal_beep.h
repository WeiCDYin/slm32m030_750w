/**
  ******************************************************************************
  * @file    slm32x030_hal_beep.h
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   Header file of BEEP HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_BEEP_H_
#define _SLM32X030_HAL_BEEP_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup BEEP
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup BEEP_Exported_Types BEEP Exported Types
  * @{
  */
/**
  * @brief  BEEP base Configuration Structure definition
    * @note   This structure is for BEEP    
  */
typedef struct
{   
    uint32_t ClkSel;                /*!< Specifies beep clock.This parameter can be a value of \ref BEEP_ClkSel */
    uint32_t PreFreqDiv;            /*!< Specifies beep clock output divider. This parameter can be a value between 0 to 0xFFF */
    uint32_t OutFreqSel;            /*!< Specifies beep clock frequency select. This parameter can be a value of \ref BEEP_OutFreqSel */
} BEEP_InitTypeDef;

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_BEEP_STATE_RESET             = 0x00U,                       /*!< Peripheral not yet initialized or disabled  */
    HAL_BEEP_STATE_READY             = 0x01U,                       /*!< Peripheral Initialized and ready for use    */
    HAL_BEEP_STATE_BUSY              = 0x02U,                       /*!< An internal process is ongoing              */
    HAL_BEEP_STATE_TIMEOUT           = 0x03U,                       /*!< Timeout state                               */
    HAL_BEEP_STATE_ERROR             = 0x04U                        /*!< Reception process is ongoing                */
}HAL_BEEP_StateTypeDef;

/**
  * @brief  BEEP Base Time Base Handle Structure definition
  */
typedef struct
{
    BEEP_TypeDef                        *Instance;                  /*!< Register base address              */
    BEEP_InitTypeDef                    Init;                       /*!< BEEP Time Base required parameters */
    HAL_LockTypeDef                     Lock;                       /*!< Locking object                     */
    __IO HAL_BEEP_StateTypeDef          State;                      /*!< BEEP operation state               */
}BEEP_HandleTypeDef;

/**
  * @}
  */ /* End of group BEEP_Exported_Types */

/* Exported constants --------------------------------------------------------*/
/** @defgroup BEEP_Exported_Constants BEEP Exported Constants
  * @{
  */
#define BEEP_CSR_CLKSEL_0               (0x00000001UL << BEEP_CSR_CLKSEL_Pos)
#define BEEP_CSR_CLKSEL_1               (0x00000002UL << BEEP_CSR_CLKSEL_Pos)
#define BEEP_CLOCK_STOP                 (0x00000000UL << BEEP_CSR_CLKSEL_Pos)                /*!< Clock stop */
#define BEEP_CLOCK_PCLK                 BEEP_CSR_CLKSEL_0                                   /*!< PCLK select */
#define BEEP_CLOCK_LSI                  BEEP_CSR_CLKSEL_1                                   /*!< LSI select */
#define BEEP_CLOCK_LSE                  (BEEP_CSR_CLKSEL_0 | BEEP_CSR_CLKSEL_1)               /*!< LSE select */

/**
  * @}
  */

#define BEEP_CSR_BEEPSEL_0              (0x00000001UL << BEEP_CSR_BEEPSEL_Pos)
#define BEEP_CSR_BEEPSEL_1              (0x00000002UL << BEEP_CSR_BEEPSEL_Pos)
#define BEEP_OUTFREQSEL_DIV8            (0x00000000U)                                       /*!< Frequency output select div8 */
#define BEEP_OUTFREQSEL_DIV4            BEEP_CSR_BEEPSEL_0                                  /*!< Frequency output select div4 */
#define BEEP_OUTFREQSEL_DIV2            BEEP_CSR_BEEPSEL_1                                  /*!< Frequency output select div2 */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup BEEP_Exported_Macros BEEP Exported Macros
  * @{
  */

/** @brief  Reset BEEP handle state
  * @param  __HANDLE__: BEEP handle.
  * @retval None
  */
#define __HAL_BEEP_RESET_HANDLE_STATE(__HANDLE__)                       ((__HANDLE__)->State = HAL_BEEP_STATE_RESET)


/** @brief  Enable BEEP function
  * @param  __HANDLE__: BEEP handle.
  * @retval None
  */
#define __HAL_BEEP_ENABLE(__HANDLE__)                                   ((__HANDLE__)->Instance->CSR |= BEEP_CSR_BEEPEN)


/** @brief  Disable BEEP function
  * @param  __HANDLE__: BEEP handle.
  * @retval None
  */
#define __HAL_BEEP_DISABLE(__HANDLE__)                                  ((__HANDLE__)->Instance->CSR &= ~BEEP_CSR_BEEPEN)


/** @brief  BEEP clock selection
  * @param  __HANDLE__: BEEP handle.
  * @param  __CLKSRC__: clock source, @ref BEEP_ClkSel
  * @retval None
  */
#define __HAL_BEEP_CLKSEL(__HANDLE__, __CLKSRC__)                       (MODIFY_REG(hbeep->Instance->CSR, BEEP_CSR_CLKSEL, __CLKSRC__))


/** @brief  BEEP clock divider
  * @param  __HANDLE__: BEEP handle.
  * @param  __CLKDIV__: clock source, This parameter can be a value between 0 to 0xFFF
  * @retval None
  */
#define __HAL_BEEP_SETDIV(__HANDLE__, __CLKDIV__)                       (MODIFY_REG(hbeep->Instance->CSR, BEEP_CSR_BEEPDIV, __DIV__))


/** @brief  BEEP output frequency select
  * @param  __HANDLE__: BEEP handle.
  * @param  __OUTDIV__: output freqency clock select, @ref BEEP_OutFreqSel
  * @retval None
  */
#define __HAL_BEEP_OUTDIV(__HANDLE__, __OUTDIV__)                       (MODIFY_REG(hbeep->Instance->CSR, BEEP_CSR_BEEPSEL, __OUTDIV__))


/**
  * @}
  */ /* End of group BEEP_Exported_Macros */


/* Private Macros -----------------------------------------------------------*/
/** \brief Check if the instance of the buzzer beep BEEP_HandleTypeDef is correct 
 *  \param  __INSTANCE__ The parameters of the buzzer instance
 */
#define IS_BEEP_INSTANCE(__INSTANCE__)                                  ((__INSTANCE__) == BEEP)   

/** \brief  Check if the ClkSel parameter of the BEEP-InitTypeDef structure of the buzzer beep BEEP_HandleTypeDef is within the range
  * \param  __CLKSEL__ specifies the BEEP ClkSel.
  * \retval None
  */
#define IS_BEEP_INIT_CLKSEL(__CLKSEL__)                                 (((__CLKSEL__) == BEEP_CLOCK_STOP) ||   \
                                                                        ((__CLKSEL__) == BEEP_CLOCK_PCLK) ||   \
                                                                        ((__CLKSEL__) == BEEP_CLOCK_LSI)  ||   \
                                                                        ((__CLKSEL__) == BEEP_CLOCK_LSE)  )   

/** \brief  Check if the pre division parameter OutFreqSel of the BEEP-InitTypeDef structure of the buzzer beep BEEP_HandleTypeDef is within the range
  * \param  __OUTFREQSEL__ Beep output frequency division selection parameter OutFreqSel
  * \retval None
  */
#define IS_BEEP_INIT_OUTFREQSEL(__OUTFREQSEL__)                         (((__OUTFREQSEL__) == BEEP_OUTFREQSEL_DIV8 ) ||   \
                                                                        ((__OUTFREQSEL__) == BEEP_OUTFREQSEL_DIV4 ) ||   \
                                                                        ((__OUTFREQSEL__) == BEEP_OUTFREQSEL_DIV2 ) )  

/** \brief  Check if the pre division parameter PreFreqDiv of the BEEP-InitTypeDef structure of the buzzer beep BEEP_HandleTypeDef is within the range
  * \param  __PREFREQDIV__  Beep pre division frequency parameter PreFreqDiv
  * \retval None
  */
#define IS_BEEP_INIT_PREFREQDIV(__PREFREQDIV__)                         ((__PREFREQDIV__ >= 0) && (__PREFREQDIV__ <= 0xFFF))

/* Exported functions --------------------------------------------------------*/


/* BEEP Base functions ********************************************************/
HAL_StatusTypeDef HAL_BEEP_Init(BEEP_HandleTypeDef *hbeep);
HAL_StatusTypeDef HAL_BEEP_DeInit(BEEP_HandleTypeDef *hbeep);
void HAL_BEEP_MspInit(BEEP_HandleTypeDef *hbeep);
void HAL_BEEP_MspDeInit(BEEP_HandleTypeDef *hbeep);
HAL_StatusTypeDef HAL_BEEP_Start(BEEP_HandleTypeDef *hbeep);
HAL_StatusTypeDef HAL_BEEP_Stop(BEEP_HandleTypeDef *hbeep);

/**
  * @}
  */ /* End of group BEEP */


#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_BEEP_H_ */
