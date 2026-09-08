/**
 ******************************************************************************
 * @file    slm32x030_hal_syscfg.h
 * @author  MCU Software Team
 * @Version V1.0.0
 * @Date    21-Oct-2020
 * @brief   Header file of SYSCFG HAL module.
 ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_SYSCFG_H_
#define _SLM32X030_HAL_SYSCFG_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup SYSCFG
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup SYSCFG_Exported_Types SYSCFG Exported Types
  * @{
  */
typedef enum
{
    HAL_SYSCFG_STATE_RESET             = 0x00U,  /*!< SYSCFG not yet initialized or disabled */
    HAL_SYSCFG_STATE_READY             = 0x01U,  /*!< SYSCFG initialized and ready for use   */
    HAL_SYSCFG_STATE_ERROR             = 0x02U   /*!< SYSCFG error state                     */
} HAL_SYSCFG_StateTypeDef;

/**
  * @brief IR Signal Select
  */
typedef enum
{
    IR_SIGNAL_SEL_USART1 = 0x00U,
    IR_SIGNAL_SEL_USART2,
    IR_SIGNAL_SEL_TIM3_CH1,
    IR_SIGNAL_SEL_TIM3_CH2,
}IR_SIGNAL_SEL;

/**
  * @brief IR Carrier Select
  */
typedef enum
{
    IR_CARRIER_SEL_TIM3_CH1 = 0x00U,
    IR_CARRIER_SEL_TIM3_CH2,
    IR_CARRIER_SEL_TIM14_CH1,
    IR_CARRIER_SEL_TIM15_CH1,
}IR_CARRIER_SEL;

/**
  * @brief Carrier and IR Signal  are inverted or not
  */
typedef enum
{
    IR_CARRIER_ORIGINAL = DISABLE,
    IR_CARRIER_OPPOSITE = ENABLE,
}CR_IR_SIGNAL_INV;

/**
  * @brief IR Out Mode Select
  */
typedef enum
{
    IR_AND_CW = 0x00U,
    IR_OR_CW,
    IR_AND_CW_OPPOSITE,
    IR_OR_CW_OPPOSITE,
}IR_OUT_SEL;

/** 
  * @brief  Structure definition of SYSCFG_IR initialization 
  * @note   The setting of these parameters with function HAL_SYSCFG_Ircr_Init() is conditioned to SYSCFG state.
  *         SYSCFG can be either disabled or enabled without conversion on going on regular group.
  */
typedef struct 
{
    SYSCFG_TypeDef                      *Instance;              /*!< Register base address */
    IR_SIGNAL_SEL                       ir_signal_sel;          /*!< select the ir singal */ 
    IR_CARRIER_SEL                      ir_carrier_sel;         /*!< select the cw singal */ 
    CR_IR_SIGNAL_INV                    ir_signal_inv;          /*!< ir signal orinal or opposite */ 
    CR_IR_SIGNAL_INV                    ir_carrier_inv;         /*!< cw signal orinal or opposite */ 
    IR_OUT_SEL                          ir_out_sel;             /*!< ir mode select */ 
} SYSCFG_IR_InitTypeDef;

/** 
  * @brief  SYSCFG IRCR handle Structure definition  
  * @note  
  *         
  */ 
typedef struct
{
    SYSCFG_TypeDef                      *Instance;              /*!< Register base address */
    SYSCFG_IR_InitTypeDef               Init;                   /*!< SYSCFG required parameters */
    __IO HAL_SYSCFG_StateTypeDef        State;                   /*!< SYSCFG required parameters */
} SYSCFG_IR_HanderTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup SYSCFG_Exported_Constants SYSCFG Exported Constants
  * @{
  */
#define HAL_MAIN_ZONE_ACTIVATION                        (0x01U)           
#define HAL_SYSTEM_ZONE_ACTIVATION                      (0x02U)    
#define HAL_SRAM_ZONE_ACTIVATION                        (0x04U)    

/** @defgroup IR_Mode
  * @{
  */
#define INTEROPERABILITY                                (0x00U)    
#define OR_OPERATION                                    (0x01U)    
#define INVERSE_POST_OPERATION                          (0x02U)    
#define OR_INVERDE_OPERATION                            (0x03U)   

#define IS_SYSCFG_ALL_INSTANCE(INSTANCE)                ((INSTANCE) == SYSCFG) 
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup SYSCFG_Exported_Functions  SYSCFG Exported Functions
  * @{
  */
HAL_StatusTypeDef HAL_SYSCFG_Ircr_Init(SYSCFG_IR_HanderTypeDef* hsyscfg);
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup SYSCFG_Private_Macros SYSCFG Private Macros
  * @{
  */

/**
  * @brief  System reset when M0+ latches up.
  * @param  STATE  
  * @retval None
  */
#define __HAL_LOCKUP_RSTEN(STATE)           (SYSCFG->CFGR1 |= SYSCFG_CFGR1_LOCKUP_RSTEN)

/**
  * @brief  DMA access interception configuration, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_FLASH_CHK_EN(STATE)           (SYSCFG->CFGR2 = 0x04 << SYSCFG_CFGR2_MASTER_CHK_EN_Pos)

/**
  * @brief  DMA access interception configuration, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_SRAM_CTRL_CHK_EN(STATE)       (SYSCFG->CFGR2 = 0x02 << SYSCFG_CFGR2_MASTER_CHK_EN_Pos)

/**
  * @brief  DMA access interception configuration, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_SRAM_CHK_EN(STATE)            (SYSCFG->CFGR2 = 0x01 << SYSCFG_CFGR2_MASTER_CHK_EN_Pos)

/**
  * @brief  Built-in brake signal generation selection, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_LVD_HIGH_BREAKEN(STATE)       (SYSCFG->CFGR2 = 0x04 << SYSCFG_CFGR2_BREAKEN_Pos)

/**
  * @brief  Built-in brake signal generation selection, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_SRAM_ERR_BREAKEN(STATE)       (SYSCFG->CFGR2 = 0x02 << SYSCFG_CFGR2_BREAKEN_Pos)

/**
  * @brief  Built-in brake signal generation selection, each bit can not write 0 after writing 1.
  * @param  STATE  
  * @retval None
  */
#define __HAL_LOCKUP_BREAKEN(STATE)         (SYSCFG->CFGR2 = 0x01 << SYSCFG_CFGR2_BREAKEN_Pos)

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

#endif /* _SLM32X030_HAL_SYSCFG_H_ */
