/**
  ******************************************************************************
  * @file    slm32x030_hal_vc.h
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   Header file of VC HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_VC_H_
#define _SLM32X030_HAL_VC_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"


/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup VC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup VC_Exported_Types VC Exported Types
  * @{
  */

/**
  * @brief  VC P select enum definition
  */
typedef enum
{
    HAL_VC_P_SEL_P0 = 0x00U,
    HAL_VC_P_SEL_P1,  
    HAL_VC_P_SEL_P2,  
    HAL_VC_P_SEL_P3,  
    HAL_VC_P_SEL_P4,  
    HAL_VC_P_SEL_P5,  
    HAL_VC_P_SEL_P6,
    HAL_VC_P_SEL_P7,  
    HAL_VC_P_SEL_P8,  
    HAL_VC_P_SEL_P9,  
    HAL_VC_P_SEL_P10,
    HAL_VC_P_SEL_P11,  
    HAL_VC_P_SEL_P12,  
    HAL_VC_P_SEL_P13,  
    HAL_VC_P_SEL_P14,
    HAL_VC_P_SEL_P15            
}VC_P_Sel_TypeDef;

/**
  * @brief  VC N select enum definition
  */
typedef enum
{
    HAL_VC_N_SEL_VC_VREF_N_128_DIV = 0x00U,
    HAL_VC_N_SEL_VREF_ADC,
    HAL_VC_N_SEL_VTS,
    HAL_VC_N_SEL_VCAP,
    HAL_VC_N_SEL_N0,
    HAL_VC_N_SEL_N1,
    HAL_VC_N_SEL_N2,
    HAL_VC_N_SEL_N3,
    HAL_VC_N_SEL_N4,
    HAL_VC_N_SEL_N5,
    HAL_VC_N_SEL_N6,
    HAL_VC_N_SEL_N7,
    HAL_VC_N_SEL_N8,
    HAL_VC_N_SEL_N9,
    HAL_VC_N_SEL_N10,
    HAL_VC_N_SEL_GND,
} VC_N_Sel_TypeDef;

/**
  * @brief  VC VREF_RSEL enum definition
  */
typedef enum
{
    VC_REF_RSEL_NULL = 0x3FU,
    VC_REF_RSEL_1_MUL_64_DIV,
    VC_REF_RSEL_2_MUL_64_DIV, 
    VC_REF_RSEL_3_MUL_64_DIV,
    VC_REF_RSEL_4_MUL_64_DIV,
    VC_REF_RSEL_5_MUL_64_DIV,   
    VC_REF_RSEL_6_MUL_64_DIV,   
    VC_REF_RSEL_7_MUL_64_DIV,
    VC_REF_RSEL_8_MUL_64_DIV,
    VC_REF_RSEL_9_MUL_64_DIV,
    VC_REF_RSEL_10_MUL_64_DIV,
    VC_REF_RSEL_11_MUL_64_DIV,
    VC_REF_RSEL_12_MUL_64_DIV, 
    VC_REF_RSEL_13_MUL_64_DIV,
    VC_REF_RSEL_14_MUL_64_DIV,
    VC_REF_RSEL_15_MUL_64_DIV,  
    VC_REF_RSEL_16_MUL_64_DIV,  
    VC_REF_RSEL_17_MUL_64_DIV,
    VC_REF_RSEL_18_MUL_64_DIV,
    VC_REF_RSEL_19_MUL_64_DIV,
    VC_REF_RSEL_20_MUL_64_DIV,  
    VC_REF_RSEL_21_MUL_64_DIV,
    VC_REF_RSEL_22_MUL_64_DIV, 
    VC_REF_RSEL_23_MUL_64_DIV,
    VC_REF_RSEL_24_MUL_64_DIV,
    VC_REF_RSEL_25_MUL_64_DIV,  
    VC_REF_RSEL_26_MUL_64_DIV,  
    VC_REF_RSEL_27_MUL_64_DIV,
    VC_REF_RSEL_28_MUL_64_DIV,
    VC_REF_RSEL_29_MUL_64_DIV,
    VC_REF_RSEL_30_MUL_64_DIV,
    VC_REF_RSEL_31_MUL_64_DIV,
    VC_REF_RSEL_32_MUL_64_DIV, 
    VC_REF_RSEL_33_MUL_64_DIV,
    VC_REF_RSEL_34_MUL_64_DIV,
    VC_REF_RSEL_35_MUL_64_DIV,  
    VC_REF_RSEL_36_MUL_64_DIV,  
    VC_REF_RSEL_37_MUL_64_DIV,
    VC_REF_RSEL_38_MUL_64_DIV,
    VC_REF_RSEL_39_MUL_64_DIV,
    VC_REF_RSEL_40_MUL_64_DIV,
    VC_REF_RSEL_41_MUL_64_DIV,
    VC_REF_RSEL_42_MUL_64_DIV, 
    VC_REF_RSEL_43_MUL_64_DIV,
    VC_REF_RSEL_44_MUL_64_DIV,
    VC_REF_RSEL_45_MUL_64_DIV,  
    VC_REF_RSEL_46_MUL_64_DIV,  
    VC_REF_RSEL_47_MUL_64_DIV,
    VC_REF_RSEL_48_MUL_64_DIV,
    VC_REF_RSEL_49_MUL_64_DIV,
    VC_REF_RSEL_50_MUL_64_DIV,
    VC_REF_RSEL_51_MUL_64_DIV,
    VC_REF_RSEL_52_MUL_64_DIV, 
    VC_REF_RSEL_53_MUL_64_DIV,
    VC_REF_RSEL_54_MUL_64_DIV,
    VC_REF_RSEL_55_MUL_64_DIV,  
    VC_REF_RSEL_56_MUL_64_DIV,  
    VC_REF_RSEL_57_MUL_64_DIV,
    VC_REF_RSEL_58_MUL_64_DIV,
    VC_REF_RSEL_59_MUL_64_DIV,
    VC_REF_RSEL_60_MUL_64_DIV,
    VC_REF_RSEL_61_MUL_64_DIV,
    VC_REF_RSEL_62_MUL_64_DIV, 
    VC_REF_RSEL_63_MUL_64_DIV,
    VC_REF_RSEL_64_MUL_64_DIV,
}VC_VREF_RSEL_TypeDef; 

/**
  * @brief  VC VREF_RSEL enum definition
  */
typedef enum
{
    VC_AVDD50       = 0x00u,
    VC_VREF_ADC,
}VC_VREFSEL_TypeDef; 

/**
  * @brief  VC VREF_RSEL enum definition
  */
typedef enum
{
    VC_HYS_SEL_NULL = 0x00u,
    VC_HYS_SEL_10mV,
    VC_HYS_SEL_20mV,
    VC_HYS_SEL_30mV,        
}VC_HYS_SEL_TypeDef; 


/**
  * @brief  HAL VC State structure definition
  */
typedef enum
{
    HAL_VC_STATE_RESET      = 0x00U,    /*!< Peripheral not Initialized                         */
    HAL_VC_STATE_READY      = 0x01U,    /*!< Peripheral Initialized and ready for use           */
    HAL_VC_STATE_BUSY       = 0x02U,    /*!< an internal process is ongoing                     */
    HAL_VC_STATE_ERROR      = 0x03U     /*!< VC error state                                    */
}HAL_VC_StateTypeDef;

/**
  * @brief  VC Init structure definition
  */

typedef struct
{
    uint8_t                 VC0Enable;
    uint8_t                 VC1Enable;
    VC_VREF_RSEL_TypeDef    RefSel;            
    VC_VREFSEL_TypeDef      VrefSel;           /*!< VREFSEL  */
    VC_HYS_SEL_TypeDef      VC0HysSel;       
    VC_P_Sel_TypeDef        VC0PinSel;         /*!< Positive input select,@ref VC_P_Sel_TypeDef */
    VC_N_Sel_TypeDef        VC0NinSel;         /*!< Negative input select,@ref VC_N_Sel_TyNeDef */
    uint16_t                VC0INT_Enable;     /*!< Interrupt Enable or Disable,@ref VC_ENABLE_Definitions */
    uint16_t                VC0INT_Mode;       /*!< Interrupt triger mode,@ref VC_ENABLE_Definitions*/
    uint16_t                VC0OutPolarity;          
    uint16_t                VC0OutType;
    uint8_t                 VC0Filter_Enable;  /*!< Filter counter Enable or Disable,@ref VC_ENABLE_Definitions */
    uint16_t                VC0Filter_Clk;     /*!< Filter clk select @ref VC_FILTER_CLK_SELECT*/
    uint32_t                VC0Fliter_Num;     /*!< Filter counter number.Min_Data >= 0x00 and Max_Data = 0xFFFF */
    VC_HYS_SEL_TypeDef      VC1HysSel;       
    VC_P_Sel_TypeDef        VC1PinSel;         /*!< Positive input select,@ref VC_P_Sel_TypeDef */ 
    VC_N_Sel_TypeDef        VC1NinSel;         /*!< Negative input select,@ref VC_N_Sel_TyNeDef*/      
    uint16_t                VC1INT_Enable;     /*!< Interrupt Enable or Disable,@ref VC_ENABLE_Definitions*/
    uint16_t                VC1INT_Mode;       /*!< Interrupt triger mode,@ref VC_ENABLE_Definitions*/
    uint16_t                VC1OutPolarity;           
    uint16_t                VC1OutType;           
    uint8_t                 VC1Filter_Enable;  /*!< Filter counter Enable or Disable,@ref VC_ENABLE_Definitions*/
    uint16_t                VC1Filter_Clk;     /*!< Filter clk select @ref VC_FILTER_CLK_SELECT*/
    uint32_t                VC1Fliter_Num;     /*!< Filter counter number.Min_Data >= 0x00 and Max_Data = 0xFFFF*/
}VC_InitTypeDef;

/** 
  * @brief  VC Handle Structure definition  
  */
typedef struct
{
    VC_TypeDef                 *Instance;  /*!< Register base address  */
    VC_InitTypeDef             Init;       /*!< VC required parameters */
    HAL_LockTypeDef            Lock;       /*!< Locking object         */
    __IO HAL_VC_StateTypeDef   State;      /*!< SPI communication state */
}VC_HandleTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup VC_Exported_Constants VC Exported Constants
  * @{
  */

/**
  * @brief  VC_ENABLE_Definitions 
  */
#define HAL_VC_ENABLE                     (0x1U)            /*!< VC INTERRUPT ENABLE   */                           
#define HAL_VC_DISABLE                    (0x0U)            /*!< VC INTERRUPT ENABLE   */ 
/**
  * @brief  VC_ENABLE_Definitions  enable and disable 
  */
#define HAL_VC0_INT_ENABLE                VC0CR_INT_EN      /*!< VC INTERRUPT ENABLE   */
#define HAL_VC0_INT_DISABLE               (0x0U)            /*!< VC INTERRUPT DISABLE   */

#define HAL_VC1_INT_ENABLE                VC1CR_INT_EN      /*!< VC INTERRUPT ENABLE   */
#define HAL_VC1_INT_DISABLE               (0x0U)            /*!< VC INTERRUPT DISABLE   */

/*VC0CR and VC1CR  is same */
#define HAL_VC_HIGH_INT_ENABLE            VC0CR_DET_HI_EN       /*!< VC INTERRUPT HIGHT LEVEL TRIGER ENABLE   */
#define HAL_VC_HIGH_INT_DISABLE           (0x0U)                /*!< VC INTERRUPT HIGHT LEVEL TRIGER DISABLE   */
#define HAL_VC_RISE_INT_ENABLE            VC0CR_DET_RISE_EN     /*!< VC INTERRUPT RISE TRIGER ENABLE  */
#define HAL_VC_RISE_INT_DISABLE           (0x0U)                /*!< VC INTERRUPT RISE TRIGER DISABLE  */
#define HAL_VC_FALL_INT_ENABLE            VC0CR_DET_FALL_EN     /*!< VC INTERRUPT FALL TRIGER ENABLE */
#define HAL_VC_FALL_INT_DISABLE           (0x0U)                /*!< VC INTERRUPT FALL TRIGER DISABLE */

#define HAL_VC_FILTER_ENABLE              VC0CR_FILTER_EN       /*!< VC FILTER ENABLE */
#define HAL_VC_FILTER_DISABLE             (0x0U)                /*!< VC FILTER DISABLE */

/**
  * @brief  MIX_EN  
  */
#define HAL_VC_OUT_MIX_DISABLE             (0x0U)              
#define HAL_VC_OUT_MIX_ENABLE              VC0CR_MIX_EN        

/**
  * @brief  INV_EN  
  */
#define HAL_VC_POLARITY_HIGH               (0x0U)              
#define HAL_VC_POLARITY_LOW                VC0CR_INV_EN        

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup VC_FILTER_CLK_SELECT  VC Filter clock select
  * @{
  */
#define HAL_VC_FILTER_CLK_NONE              (0x00U)                                 /*!< VC Filter clock select NONE */
#define HAL_VC_FILTER_CLK_PCLK              (0x01UL << VC0CR_CLK_SEL_Pos)           /*!< VC Filter clock select PCLK */
#define HAL_VC_FILTER_CLK_LSI               (0x02UL << VC0CR_CLK_SEL_Pos)           /*!< VC Filter clock select LIRC */
#define HAL_VC_FILTER_CLK_LSE               (0x03UL << VC0CR_CLK_SEL_Pos)           /*!< VC Filter clock select LIRC */
#define HAL_VC_OUTCFG_Msk                   (0x7FFFFU)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup VC_Exported_Functions  VC Exported Functions
  * @{
  */

/** @defgroup VC_Exported_Functions_Group1 Initialization and DeInitialization functions
  * @{
  */
/* Initialization/Start functions  ********************************************/
HAL_StatusTypeDef HAL_VC_Init(VC_HandleTypeDef *hvc);
void HAL_VC_MspInit(VC_HandleTypeDef *hvc);
/* DeInitialization functions  ********************************************/
HAL_StatusTypeDef HAL_VC_DeInit(VC_HandleTypeDef *hvc);
void HAL_VC_MspDeInit(VC_HandleTypeDef *hvc);
/**
  * @}
  */

/** @defgroup VC_Exported_Functions_Group2 IO operation functions
  * @{
  */
/* I/O operation functions ****************************************************/

FlagStatus HAL_VC0_Get_INT_Flag(VC_HandleTypeDef *hvc);
FlagStatus HAL_VC1_Get_INT_Flag(VC_HandleTypeDef *hvc);
FlagStatus HAL_VC0_Get_OutPut_State(VC_HandleTypeDef *hvc);
FlagStatus HAL_VC1_Get_OutPut_State(VC_HandleTypeDef *hvc);
void HAL_VC0_Clear_INT_Flag(VC_HandleTypeDef *hvc);
void HAL_VC1_Clear_INT_Flag(VC_HandleTypeDef *hvc);
void HAL_VC0_Disable(VC_HandleTypeDef *hvc);
void HAL_VC1_Disable(VC_HandleTypeDef *hvc);
void HAL_VC_Enable(VC_HandleTypeDef  *hvc);


/**
  * @}
  */

/**
  * @}
  */


/* Private macros ------------------------------------------------------------*/
/** @defgroup VC_Private_Macros VC Private Macros
  * @{
  */

/**
  * @brief  Check VC INPUT SELECT.
  * @param  INPUTE  VC Input Source
  * @retval None
  */
#define IS_VC_INPUT(INPUTE)             (((INPUT) == HAL_VC_INPUT_0) || ((INPUT) == HAL_VC_INPUT_1) || \
                                         ((INPUT) == HAL_VC_INPUT_2) || ((INPUT) == HAL_VC_INPUT_V25DIV))
    /**
  * @brief  Check VC INTERRUPT ENABLE OR DISABLE.
  * @param  STATE  VC interrupt enable or disable
  * @retval None
  */
#define IS_VC0_INT_STATE(STATE)         ((STATE == HAL_VC0_INT_ENABLE) || (STATE == HAL_VC0_INT_DISABLE))

/**
  * @brief  Check VC INTERRUPT TRIGER MODE.
  * @param  MODE  VC interrupt triger mode
  * @retval None
  */
#define IS_VC1_INT_STATE(STATE)         ((STATE == HAL_VC1_INT_ENABLE) || (STATE == HAL_VC1_INT_DISABLE))

/**
  * @brief  Check VC INTERRUPT TRIGER MODE.
  * @param  MODE  VC interrupt triger mode
  * @retval None
  */
#define IS_VC_INT_MODE(MODE)            (((MODE) == HAL_VC_HIGH_INT_ENABLE) || ((MODE) == HAL_VC_RISE_INT_ENABLE) || \
                                         ((MODE) == HAL_VC_FALL_INT_ENABLE) || ((MODE) == 0x00U))
/**
  * @brief  Check VC FILTER ENABLE OR DISABLE.
  * @param  STATE  VC Filter enable or disable
  * @retval None
  */
#define IS_VC_FILTER_STATE(STATE)       ((STATE == HAL_VC_FILTER_ENABLE) || (STATE == HAL_VC_FILTER_DISABLE))

/**
  * @brief  Check VC FILTER NUMBER.
  * @param  FILTER_NUM  VC Filter number
  * @retval None
  */
#define IS_VC_FILTER_NUM(FILTER_NUM)    ((FILTER_NUM >= 0U) && (FILTER_NUM <= 0xFFFFU)) 

/**
  * @brief  Check VC FILTER CLK SOURCE.
  * @param  FILTER_CLK  VC Filter clock source
  * @retval None
  */
#define IS_VC_FILTER_CLK(FILTER_CLK)    ((FILTER_CLK == HAL_VC_FILTER_CLK_NONE) || (FILTER_CLK == HAL_VC_FILTER_CLK_PCLK) || \
                                         (FILTER_CLK == HAL_VC_FILTER_CLK_LSI) || (FILTER_CLK == HAL_VC_FILTER_CLK_LSE))

/**
  * @brief  Check MIX_EN 
  * @param  TYPE
  * @retval None
  */
#define IS_VC_OUTPUT_TYPE(TYPE)         ((STATE == HAL_VC_OUT_MIX_DISABLE) || (STATE == HAL_VC_OUT_MIX_ENABLE))

/**
  * @brief  Check VC0 
  * @param  POLARITY
  * @retval None
  */
#define IS_VC_POLARITY_TYPE(POLARITY)   ((STATE == HAL_VC_POLARITY_HIGH) || (STATE == HAL_VC_POLARITY_LOW))

/**
  * @brief  Check VC INSTANCE INhandle .
  * @param  INSTANCE  VC Instance
  * @retval None
  */
#define IS_VC_INSTANCE(INSTANCE)        ((INSTANCE) == VC)

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

#endif /* _SLM32X030_HAL_VC_H_ */


