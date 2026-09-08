/**
  ******************************************************************************
  * @file    slm32x030_hal_dma.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-11-2024
  * @brief   Header file of DMA HAL module.
  *
  ******************************************************************************
 **/
    
/* Define to prevent recursive inclusion -------------------------------------*/    
#ifndef _SLM32X030_HAL_DMA_H_
#define _SLM32X030_HAL_DMA_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32F1XA_HAL_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

typedef enum 
{
    DMA_CH0                 = 0x00,
    DMA_CH1                 = 0x01,
    DMA_CH2                 = 0x02,
    DMA_CH3                 = 0x03,
    DMA_CH4                 = 0x04,
    DMA_CH5                 = 0x05,
    DMA_CH_Max,
}DMA_ChannelDef;

typedef enum 
{
    DMA_SRAM_TO_SRAM        = 0x00,
    DMA_SRAM_TO_PERIPH      = 0x01,
    DMA_PERIPH_TO_SRAM      = 0x02,
    DMA_PERIPH_TO_PERIPH    = 0x03,
}DMA_FlowcntrlDef;


typedef enum 
{
    DMA_AES_RX              = 0x00U    ,
    DMA_AES_TX              = 0x01U    ,
    DMA_ADC                 = 0x02U    ,
    DMA_I2C1_RX             = 0x03U    ,
    DMA_I2C1_TX             = 0x04U    ,
    DMA_I2C2_RX             = 0x05U    ,
    DMA_I2C2_TX             = 0x06U    ,
    DMA_USART1_RX           = 0x07U    ,
    DMA_USART1_TX           = 0x08U    ,
    DMA_USART2_RX           = 0x09U    ,
    DMA_USART2_TX           = 0x0AU    ,
    DMA_SPI1_RX             = 0x0BU    ,
    DMA_SPI1_TX             = 0x0CU    ,
    DMA_SPI2_RX             = 0x0DU    ,
    DMA_SPI2_TX             = 0x0EU    ,
    DMA_TIMER1_CH1          = 0x14U    ,
    DMA_TIMER1_CH2          = 0x15U    ,
    DMA_TIMER1_CH3          = 0x16U    ,
    DMA_TIMER1_CH4          = 0x17U    ,
    DMA_TIMER1_TRIG         = 0x18U    ,
    DMA_TIMER1_UP           = 0x19U    ,
    DMA_TIMER1_COM          = 0x1AU    ,
    DMA_TIMER3_CH1          = 0x1DU    ,
    DMA_TIMER3_CH2          = 0x1EU    ,
    DMA_TIMER3_CH3          = 0x1FU    ,
    DMA_TIMER3_CH4          = 0x20U    ,
    DMA_TIMER3_TRIG         = 0x21U    ,
    DMA_TIMER3_UP           = 0x22U    ,
    DMA_TIMER6_UP           = 0x24U    ,
    DMA_TIMER7_UP           = 0x26U    ,
    DMA_TIMER14_CH1         = 0x28U    ,
    DMA_TIMER14_UP          = 0x29U    ,
    DMA_TIMER15_CH1         = 0x2AU    ,
    DMA_TIMER15_TRIG        = 0x2BU    ,
    DMA_TIMER15_UP          = 0x2CU    ,
    DMA_TIMER15_COM         = 0x2DU    ,
    DMA_TIMER16_CH1         = 0x2EU    ,
    DMA_TIMER16_UP          = 0x2FU    ,
    DMA_TIMER17_CH1         = 0x30U    ,
    DMA_TIMER17_UP          = 0x31U    ,
}DMA_PeriphSelDef;

/**
  * @brief  HAL DMA State structures definition
  */
typedef enum
{
    HAL_DMA_STATE_RESET             = 0x00U,  /*!< DMA not yet initialized or disabled    */
    HAL_DMA_STATE_READY             = 0x01U,  /*!< DMA initialized and ready for use      */
    HAL_DMA_STATE_BUSY              = 0x02U,  /*!< DMA process is ongoing                 */
    HAL_DMA_STATE_TIMEOUT           = 0x03U   /*!< DMA timeout state                      */
}HAL_DMA_StateTypeDef;

/** 
  * @brief  HAL DMA Callback ID structure definition
  */
typedef enum
{
    HAL_DMA_XFER_CPLT_CB_ID         = 0x00U,    /*!< Full transfer                          */
    HAL_DMA_XFER_OVERLAP_CB_ID      = 0x01U,    /*!< Overload                               */
    HAL_DMA_XFER_ERROR_CB_ID        = 0x02U,    /*!< Error                                  */ 
    HAL_DMA_XFER_ABORT_CB_ID        = 0x03U,    /*!< Abort                                  */ 
    HAL_DMA_XFER_ALL_CB_ID          = 0x04U     /*!< All                                    */ 
}HAL_DMA_CallbackIDTypeDef;

typedef struct
{
    __IO uint32_t SRCADDR;    /*!< offset:0x100*/
    __IO uint32_t DESTADDR;    /*!< offset:0x104 */
    __IO uint32_t RSVD1[1];    /*!< offset:0x108*/
    __IO uint32_t CONTROL;    /*!< offset:0x10C */
    __IO uint32_t CONFIG;    /*!< offset:0x110 */
}DMA_Channel_TypeDef;
 
#define DMA_CHN_BASE            (DMA_BASE + 0x100U)
#define DMA_CHN_OFFSET          (0X20U)
#define DMA_CHN_0               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 0 * DMA_CHN_OFFSET))
#define DMA_CHN_1               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 1 * DMA_CHN_OFFSET))
#define DMA_CHN_2               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 2 * DMA_CHN_OFFSET))
#define DMA_CHN_3               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 3 * DMA_CHN_OFFSET))
#define DMA_CHN_4               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 4 * DMA_CHN_OFFSET))
#define DMA_CHN_5               ((DMA_Channel_TypeDef *)(DMA_CHN_BASE + 5 * DMA_CHN_OFFSET))

typedef struct
{
    DMA_ChannelDef              DmaChannel;                 /*!< DMA Channel                */

    DMA_PeriphSelDef            Periphx_sel;                /*!< DMA_PeriphSelDef           */
    
    // DMA_TfmodeDef               Tfmode;                     /*!< DMA_TfmodeDef              */

    DMA_FlowcntrlDef            Flowcntrl;                  /*!< DMA_FlowcntrlDef           */   

    uint32_t                    SrcInc;

    uint32_t                    DestInc;

    uint32_t                    Brken;

    uint32_t                    Continue;

}DMA_InitTypeDef;
 
typedef struct __DMA_HandleTypeDef
{
    DMA_Channel_TypeDef         *Instance;                  /*!< DMA Channel Base Address   */  

    DMA_InitTypeDef             Init;

    HAL_LockTypeDef             Lock;                       /*!< DMA locking object         */  

    __IO HAL_DMA_StateTypeDef   State;                      /*!< DMA transfer state         */

    void                        *Parent;                    /*!< Parent object state        */	   
    
    void                        (*XferErrorCallback)(struct __DMA_HandleTypeDef *hdma);

    void                        (*XferCpltCallback)(struct __DMA_HandleTypeDef *hdma);

    void                        (*XferOverlapCallback)(struct __DMA_HandleTypeDef *hdma);

    void                        (*XferAbortCallback)(struct __DMA_HandleTypeDef *hdma);

    __IO uint32_t               ErrorCode;                  /*!< DMA Error code             */

    DMA_TypeDef                 *DmaBaseAddress;            /*!< DMA Base Address           */  

    DMA_ChannelDef              ChannelNum;
    
}DMA_HandleTypeDef; 

/** @defgroup DMA Register address and offsets
  * @{
  */
#define DMA_OFFSET                   (DMA_BASE - PERIPH_BASE)
#define DMA_INTSTA_OFFSET            (0x00U)
#define DMA_INTTCSTA_OFFSET          (0x04U)
#define DMA_INTTCCLR_OFFSET          (0x08U)
#define DMA_INTERRSTA_OFFSET         (0x0cU)  
#define DMA_INTERRCLR_OFFSET         (0x10U)
#define DMA_RAWINTTCSTA_OFFSET       (0x14U)    
#define DMA_RAWINTERRSTA_OFFSET      (0x18U)  
#define DMA_ENBLDCHNSTA_OFFSET       (0x1cU)  

#define DMA_SOFTBREQ_OFFSET          (0x20U)
#define DMA_SOFTSREQ_OFFSET          (0x24U)    
#define DMA_OVSTA_OFFSET             (0x28U) 
#define DMA_OVCLR_OFFSET             (0x2cU)  

#define DMA_CONFIG_OFFSET            (0x30U)
#define DMA_PERIPH0_SEL_OFFSET       (0x34U)    
#define DMA_PERIPH1_SEL_OFFSET       (0x38U)
#define DMA_PERIPH2_SEL_OFFSET       (0x3cU)    
#define DMA_PERIPH3_SEL_OFFSET       (0x40U)
#define DMA_PERIPH4_SEL_OFFSET       (0x44U)    
#define DMA_PERIPH5_SEL_OFFSET       (0x48U)

#define DMA_CH0_SRCADDR_OFFSET       (0x100U)   
#define DMA_CH0_DESTADDR_OFFSET      (0x104U)   
#define DMA_CH0_RSVD1_OFFSET         (0x108U)
#define DMA_CH0_CONTROL_OFFSET       (0x10cU)
#define DMA_CH0_CONFIG_OFFSET        (0x110U)

#define DMA_CH1_SRCADDR_OFFSET       (0x120U)   
#define DMA_CH1_DESTADDR_OFFSET      (0x124U)   
#define DMA_CH1_RSVD1_OFFSET         (0x128U)
#define DMA_CH1_CONTROL_OFFSET       (0x12cU)
#define DMA_CH1_CONFIG_OFFSET        (0x130U)

#define DMA_CH2_SRCADDR_OFFSET       (0x140U)   
#define DMA_CH2_DESTADDR_OFFSET      (0x144U)   
#define DMA_CH2_RSVD1_OFFSET         (0x148U)
#define DMA_CH2_CONTROL_OFFSET       (0x14cU)
#define DMA_CH2_CONFIG_OFFSET        (0x150U)

#define DMA_CH3_SRCADDR_OFFSET       (0x160U)   
#define DMA_CH3_DESTADDR_OFFSET      (0x164U)   
#define DMA_CH3_RSVD1_OFFSET         (0x168U)
#define DMA_CH3_CONTROL_OFFSET       (0x16cU)
#define DMA_CH3_CONFIG_OFFSET        (0x170U)

#define DMA_CH4_SRCADDR_OFFSET       (0x180U)   
#define DMA_CH4_DESTADDR_OFFSET      (0x184U)   
#define DMA_CH4_RSVD1_OFFSET         (0x188U)
#define DMA_CH4_CONTROL_OFFSET       (0x18cU)
#define DMA_CH4_CONFIG_OFFSET        (0x190U)

#define DMA_CH5_SRCADDR_OFFSET       (0x1A0U)   
#define DMA_CH5_DESTADDR_OFFSET      (0x1A4U)   
#define DMA_CH5_RSVD1_OFFSET         (0x1AAU)
#define DMA_CH5_CONTROL_OFFSET       (0x1AcU)
#define DMA_CH5_CONFIG_OFFSET        (0x1B0U)

#define DMA_INTSTA_ADDR             (DMA_BASE + 0x00U)
#define DMA_INTTCSTA_ADDR           (DMA_BASE + 0x04U)
#define DMA_INTTCCLR_ADDR           (DMA_BASE + 0x08U)
#define DMA_INTERRSTA_ADDR          (DMA_BASE + 0x0cU)  
#define DMA_INTERRCLR_ADDR          (DMA_BASE + 0x10U)
#define DMA_RAWINTTCSTA_ADDR        (DMA_BASE + 0x14U)      
#define DMA_RAWINTERRSTA_ADDR       (DMA_BASE + 0x18U)  
#define DMA_ENBLDCHNSTA_ADDR        (DMA_BASE + 0x1cU)  

#define DMA_SOFTBREQ_ADDR           (DMA_BASE + 0x20U)
#define DMA_SOFTSREQ_ADDR           (DMA_BASE + 0x24U)      
#define DMA_OVSTA_ADDR              (DMA_BASE + 0x28U) 
#define DMA_OVCLR_ADDR              (DMA_BASE + 0x2cU)  

#define DMA_CONFIG_ADDR             (DMA_BASE + 0x30U)
#define DMA_PERIPH0_SEL_ADDR        (DMA_BASE + 0x34U)      
#define DMA_PERIPH1_SEL_ADDR        (DMA_BASE + 0x38U)
#define DMA_PERIPH2_SEL_ADDR        (DMA_BASE + 0x3cU)      
#define DMA_PERIPH3_SEL_ADDR        (DMA_BASE + 0x40U)
#define DMA_PERIPH4_SEL_ADDR        (DMA_BASE + 0x44U)      
#define DMA_PERIPH5_SEL_ADDR        (DMA_BASE + 0x48U)

#define DMA_CH0_SRCADDR_ADDR        (DMA_BASE + 0x100U) 
#define DMA_CH0_DESTADDR_ADDR       (DMA_BASE + 0x104U) 
#define DMA_CH0_RSVD1_ADDR          (DMA_BASE + 0x108U)
#define DMA_CH0_CONTROL_ADDR        (DMA_BASE + 0x10cU)
#define DMA_CH0_CONFIG_ADDR         (DMA_BASE + 0x110U)

#define DMA_CH1_SRCADDR_ADDR        (DMA_BASE + 0x120U) 
#define DMA_CH1_DESTADDR_ADDR       (DMA_BASE + 0x124U) 
#define DMA_CH1_RSVD1_ADDR          (DMA_BASE + 0x128U)
#define DMA_CH1_CONTROL_ADDR        (DMA_BASE + 0x12cU)
#define DMA_CH1_CONFIG_ADDR         (DMA_BASE + 0x130U)

#define DMA_CH2_SRCADDR_ADDR        (DMA_BASE + 0x140U) 
#define DMA_CH2_DESTADDR_ADDR       (DMA_BASE + 0x144U) 
#define DMA_CH2_RSVD1_ADDR          (DMA_BASE + 0x148U)
#define DMA_CH2_CONTROL_ADDR        (DMA_BASE + 0x14cU)
#define DMA_CH2_CONFIG_ADDR         (DMA_BASE + 0x150U)

#define DMA_CH3_SRCADDR_ADDR        (DMA_BASE + 0x160U) 
#define DMA_CH3_DESTADDR_ADDR       (DMA_BASE + 0x164U) 
#define DMA_CH3_RSVD1_ADDR          (DMA_BASE + 0x168U)
#define DMA_CH3_CONTROL_ADDR        (DMA_BASE + 0x16cU)
#define DMA_CH3_CONFIG_ADDR         (DMA_BASE + 0x170U)

#define DMA_CH4_SRCADDR_ADDR        (DMA_BASE + 0x180U) 
#define DMA_CH4_DESTADDR_ADDR       (DMA_BASE + 0x184U) 
#define DMA_CH4_RSVD1_ADDR          (DMA_BASE + 0x188U)
#define DMA_CH4_CONTROL_ADDR        (DMA_BASE + 0x18cU)
#define DMA_CH4_CONFIG_ADDR         (DMA_BASE + 0x190U)

#define DMA_CH5_SRCADDR_ADDR        (DMA_BASE + 0x1A0U) 
#define DMA_CH5_DESTADDR_ADDR       (DMA_BASE + 0x1A4U) 
#define DMA_CH5_RSVD1_ADDR          (DMA_BASE + 0x1AAU)
#define DMA_CH5_CONTROL_ADDR        (DMA_BASE + 0x1AcU)
#define DMA_CH5_CONFIG_ADDR         (DMA_BASE + 0x1B0U)

/* Exported constants --------------------------------------------------------*/

/** @defgroup DMA_Exported_Constants DMA Exported Constants
  * @{
  */

/** @defgroup DMA_Error_Code DMA Error Code
  * @{
  */
 #define HAL_DMA_ERROR_NONE                     0x00000000U    /*!< No error            */
 #define HAL_DMA_ERROR_TE                       0x00000001U    /*!< Transfer error      */
 #define HAL_DMA_ERROR_NO_XFER                  0x00000004U    /*!< no ongoing transfer */
 #define HAL_DMA_ERROR_TIMEOUT                  0x00000020U    /*!< Timeout error       */
 #define HAL_DMA_ERROR_NOT_SUPPORTED            0x00000100U    /*!< Not supported mode  */ 

/** @defgroup   
  * @{
  */
#define DMA_SRC_PERIPH0             (0x0U) 
#define DMA_SRC_PERIPH1             (0x1U)
#define DMA_SRC_PERIPH2             (0x2U)
#define DMA_SRC_PERIPH3             (0x3U)
#define DMA_SRC_PERIPH4             (0x4U)
#define DMA_SRC_PERIPH5             (0x5U)

/** @defgroup   
  * @{
  */
#define DMA_DEST_PERIPH0            (0x0U) 
#define DMA_DEST_PERIPH1            (0x1U)
#define DMA_DEST_PERIPH2            (0x2U)
#define DMA_DEST_PERIPH3            (0x3U)
#define DMA_DEST_PERIPH4            (0x4U)
#define DMA_DEST_PERIPH5            (0x5U)

/** @defgroup   DMA enable
  * @{
  */
#define DMA_EN_DISABLE              (0x0U)
#define DMA_EN_ENABLE               DMA_CONFIG_DMA_EN

/** @defgroup   DMA channel enable
  * @{
  */
#define DMA_CH_EN_DISABLE           (0x0U)
#define DMA_CH_EN_ENABLE            DMA_CHX_CONFIG_CH_EN

/** @defgroup Halt
  * @{
  */
#define DMA_HALT_DISABLE            (0x0U)  
#define DMA_HALT_ENABLE             DMA_CHX_CONFIG_HALT	

/** @defgroup Brken
  * @{
  */
#define DMA_BRKEN_DISABLE           (0x0U) 	
#define DMA_BRKEN_ENABLE            DMA_CHX_CONTROL_BRKEN

/** @defgroup Source address increase
  * @{
  */
#define DMA_SINC_DISABLE            (0x0U) 	
#define DMA_SINC_ENABLE             DMA_CHX_CONTROL_SINC

/** @defgroup Destination address increase
  * @{
  */
#define DMA_DINC_DISABLE            (0x0U)
#define DMA_DINC_ENABLE             DMA_CHX_CONTROL_DINC

/** @defgroup Conti
  * @{
  */
#define DMA_CONTI_DISABLE           (0x0U)   
#define DMA_CONTI_ENABLE            DMA_CHX_CONTROL_CONTI   

/** @defgroup Interrput
  * @{
  */
#define DMA_IT_ERRINT               DMA_CHX_CONFIG_ERR_INT_MASK
#define DMA_IT_TCINT                DMA_CHX_CONFIG_TC_INT_MASK
#define DMA_IT_ALL					(DMA_IT_ERRINT | DMA_IT_TCINT)

/**
  * @brief  Return the all DMA Channel Global interrupt flag.
  */
#define __HAL_DMA_GET_INTSTA_RESG() 			(DMA->INTSTA)

/**
  * @brief  Return the current DMA Channel Global interrupt flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return value
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_INTSTA_FLAG(__HANDLE__, __FLAG__) 			((__HANDLE__)->DmaBaseAddress->INTSTA & (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel transfer complete flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return value 
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_INTTCSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->INTTCSTA & (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel transfer error flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return   
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_INTERRSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->INTERRSTA & (__FLAG__))

/**
  * @brief  Clears the DMA Channel transfer complete flags.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Clear the specified flag.
  */
#define __HAL_DMA_CLEAR_INTTCCLR_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->INTTCCLR = (__FLAG__))
 
/**
  * @brief Clears the DMA Channel transfer error flags.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Clear the specified flag.
  */
#define __HAL_DMA_CLEAR_INTERRCLR_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->INTERRCLR = (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel Non maskable transfer complete flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return   
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_RAWINTTCSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->RAWINTTCSTA & (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel Non maskable transfer error flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return   
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_RAWINTERRSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->RAWINTERRSTA & (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel transfer state flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return   
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_ENBLDCHNSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->ENBLDCHNSTA & (__FLAG__))
 
/**
  * @brief  Set single the DMA Channel data transfer flags.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Set the specified bit.
  */
#define __HAL_DMA_SET_SOFTSREQ_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->SOFTSREQ = (__FLAG__))
 
/**
  * @brief  Returns the current DMA Channel overload flag.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Get the specified flag.
  *          This parameter would be the following values:
  *            @arg return   
  * @retval The state of FLAG (SET or RESET).
  */
#define __HAL_DMA_GET_OVSTA_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->OVSTA & (__FLAG__))
 
/**
  * @brief  Clears the DMA Channel overload flags.
  * @param  __HANDLE__: DMA handle
  * @param  __FLAG__: Clear the specified flag.
  */
#define __HAL_DMA_CLEAR_OVCLR_FLAG(__HANDLE__, __FLAG__) 		((__HANDLE__)->DmaBaseAddress->OVCLR = (__FLAG__))
 
/**
  * @brief  Enable the specified DMA Channel.
  * @param  __HANDLE__: DMA handle  
  */
#define __HAL_DMA_SET_CHX_EN(__HANDLE__) 		((__HANDLE__)->Instance->CONFIG |= DMA_CHX_CONFIG_CH_EN)
 
/**
  * @brief  Disable the specified DMA Channel.
  * @param  __HANDLE__: DMA handle 
  */
#define __HAL_DMA_CLR_CHX_EN(__HANDLE__) 		((__HANDLE__)->Instance->CONFIG &= ~DMA_CHX_CONFIG_CH_EN)
 
/**
  * @brief  Enables the specified DMA Channel interrupts.
  * @param  __HANDLE__: DMA handle
  * @param  __INTERRUPT__: 
            #define DMA_IT_ERRINT
            #define DMA_IT_TCINT
            #define DMA_IT_ALL
  */
#define __HAL_DMA_ENABLE_IT(__HANDLE__, __INTERRUPT__)      ((__HANDLE__)->Instance->CONFIG &= ~(__INTERRUPT__))
#define __HAL_DMA_DISABLE_IT(__HANDLE__, __INTERRUPT__)     ((__HANDLE__)->Instance->CONFIG |= (__INTERRUPT__))

/**
  * @brief  DMA peripheral selection
  * @param  __HANDLE__: DMA handle 
  */
#define __HAL_DMA_SET_PERIPH(__HANDLE__, __PERIPHX__)   (*(uint32_t *)((uint32_t)DMA_PERIPH0_SEL_ADDR + (__HANDLE__)->ChannelNum * 4) = (__PERIPHX__))

/**
  * @brief  Returns the current DMA Channel value of Continuous counter
  * @param  __HANDLE__: DMA handle 
  */
#define __HAL_DMA_GET_CONTINUOUS_CNT(__HANDLE__) 	    (((__HANDLE__)->Instance->CONFIG & DMA_CHX_CONFIG_CONTI_CNT) >> DMA_CHX_CONFIG_CONTI_CNT_Pos)
 
/**
  * @brief  Clear the current DMA Channel value of Continuous counter
  * @param  __HANDLE__: DMA handle
  */
#define __HAL_DMA_CLEAR_CONTINUOUS_CNT(__HANDLE__) 		((__HANDLE__)->Instance->CONFIG |= DMA_CHX_CONFIG_CONTI_CLR)
 
/**
  * @brief  the DMA transfer suspend. 
  * @param  __HANDLE__: DMA handle
  */
#define __HAL_DMACHx_SUSPEND_ENABLE(__HANDLE__)      ((__HANDLE__)->Instance->CONFIG |= DMA_CHX_CONFIG_HALT)
#define __HAL_DMACHx_SUSPEND_DISABLE(__HANDLE__)     ((__HANDLE__)->Instance->CONFIG &= ~DMA_CHX_CONFIG_HALT)
 
/**
  * @brief  wait the DMA transfer fifoempty. 
  * @param  __HANDLE__: specifies the DMA channel handle. 
  */
#define __HAL_DMACHx_Poll_FIFOEMPTY(__HANDLE__) 	while(((__HANDLE__)->Instance->CONFIG & DMA_CHX_CONFIG_ACTIVE) != RESET){break;}

HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma); 
HAL_StatusTypeDef HAL_DMA_DeInit (DMA_HandleTypeDef *hdma);

HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_USART_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_USART_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);

HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t Timeout);
HAL_StatusTypeDef HAL_DMA_USART_PollForTransfer(DMA_HandleTypeDef *hdma);

void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)( DMA_HandleTypeDef * _hdma));
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID);

HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma);
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma);	

#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_DMA_H_ */


