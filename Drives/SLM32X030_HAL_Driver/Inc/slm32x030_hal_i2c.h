/**
  ******************************************************************************
  * @file    slm32x030_hal_i2c.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of I2C HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/    
#ifndef _SLM32X030_HAL_I2C_H_
#define _SLM32X030_HAL_I2C_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */ 

/******************************************************************************/
/* Exported types ------------------------------------------------------------*/
/******************************************************************************/

/** @defgroup I2C_Exported_Types I2C Exported Types
  * @{
  */


/**
  * @brief  HAL State structure definition
  * @note  HAL I2C State value coding follow below described bitmap :
  *          b7-b6  Error information 
  *             00 : No Error
  *             01 : Abort (Abort user request on going)
  *             10 : Timeout
  *             11 : Error
  *          b5     IP initilisation status
  *             0  : Reset (IP not initialized)
  *             1  : Init done (IP initialized and ready to use. HAL I2C Init function called)
  *          b4     (not used)
  *             x  : Should be set to 0
  *          b3
  *             0  : Ready or Busy (No Listen mode ongoing)
  *             1  : Listen (IP in Address Listen Mode)
  *          b2     Intrinsic process state
  *             0  : Ready
  *             1  : Busy (IP busy with some configuration or internal operations)
  *          b1     Rx state
  *             0  : Ready (no Rx operation ongoing)
  *             1  : Busy (Rx operation ongoing)
  *          b0     Tx state
  *             0  : Ready (no Tx operation ongoing)
  *             1  : Busy (Tx operation ongoing)
  */
typedef enum
{
    HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */    
    HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
    HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
    HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
    HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
    HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
    HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                    process is ongoing                         */
    HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                    process is ongoing                         */
    HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
    HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
    HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */
}HAL_I2C_StateTypeDef;

/** 
  * @brief  I2C Initial Configuration Structure definition  
  */
typedef struct 
{
    uint32_t master;                /*!< master mode enable/disable. This parameter can be a value of @ref I2C_Master_Mode */    
    uint32_t slave;                 /*!< slave mode enable/disable. This parameter can be a value of @ref I2C_Slave_Mode */ 
    uint32_t restart;               /*!< restart enable/disable. This parameter can be a value of @ref I2C_Restart_Enable */
    uint32_t speedMode;    
    uint32_t masterAddrBits;        /*!< master address bits: 10-bit/7-bit. This parameter can be a value of @ref I2C_Master_Addr_Bits */    
    uint32_t slaveAddrBits;         /*!< slave address bits: 10-bit/7-bit. This parameter can be a value of @ref I2C_Slave_Addr_Bits */    
    uint32_t slaveAddr;             /*!< slave address */    
    uint32_t sclHcnt;               /*!< standard speed high count. This parameter can be a value between 6-65535 */    
    uint32_t sclLcnt;               /*!< standard speed low count. This parameter can be a value between 8-65535 */    
    uint32_t rxFifoThd;             /*!< receive FIFO threshold level. This parameter can be a value between 0-255 */    
    uint32_t txFifoThd;             /*!< transmit FIFO threshold level. This parameter can be a value between 0-255 */
    uint32_t dualSlaveAddrEn;       /*!< slave address dual enable/disable. This parameter can be a value of @ref I2C_Slave_Dual_Addr */   
    uint32_t slaveAddr2;    
}I2C_InitTypeDef; 

/**
  * @brief  HAL Mode structure definition
  * @note  HAL I2C Mode value coding follow below described bitmap :
  *          b7     (not used)
  *             x  : Should be set to 0
  *          b6
  *             0  : None
  *             1  : Memory (HAL I2C communication is in Memory Mode)
  *          b5
  *             0  : None
  *             1  : Slave (HAL I2C communication is in Slave Mode)
  *          b4
  *             0  : None
  *             1  : Master (HAL I2C communication is in Master Mode)
  *          b3-b2-b1-b0  (not used)
  *             xxxx : Should be set to 0000
  */
typedef enum
{
    HAL_I2C_MODE_NONE             = 0x00U,   /*!< No I2C communication on going             */
    HAL_I2C_MODE_MASTER           = 0x10U,   /*!< I2C communication is in Master Mode       */
    HAL_I2C_MODE_SLAVE            = 0x20U,   /*!< I2C communication is in Slave Mode        */
    HAL_I2C_MODE_MEM              = 0x40U    /*!< I2C communication is in Memory Mode       */
}HAL_I2C_ModeTypeDef;

typedef enum
{
    HAL_I2C_TNTERFACE_BLOCK     = 0x01U,
    HAL_I2C_TNTERFACE_IT        = 0x02U,
    HAL_I2C_TNTERFACE_DMA       = 0x03U,
}HAL_I2C_IntfTypeDef;

/** 
  * @brief  LPTIM handle Structure definition  
  */ 
typedef struct 
{
    I2C_TypeDef                *Instance;    
    I2C_InitTypeDef            Init;    
    uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer           */                                                
    uint16_t                   XferSize;       /*!< I2C transfer size                        */                                                
    __IO uint16_t              XferCount;      /*!< I2C transfer counter                     */                                                
    __IO uint32_t              XferOptions;    /*!< I2C transfer options                     */                                                
    __IO uint32_t              PreviousState;  /*!< I2C communication Previous state and mode context for internal usage */                                                                                                
    DMA_HandleTypeDef          *hdmatx;        /*!< I2C Tx DMA handle parameters             */                                                
    DMA_HandleTypeDef          *hdmarx;        /*!< I2C Rx DMA handle parameters             */                                                
    HAL_LockTypeDef            Lock;           /*!< I2C locking object                       */
    __IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                  */                                                
    __IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C communication mode                   */
    __IO HAL_I2C_IntfTypeDef   Intf;           /*!< I2C interface mode                       */                                                
    __IO uint32_t              ErrorCode;      /*!< I2C Error code                           */
    __IO uint32_t              Devaddress;     /*!< I2C Target device address                */
    __IO uint32_t              Memaddress;     /*!< I2C Target memory address                */
    __IO uint32_t              MemaddSize;     /*!< I2C Target memory address  size          */
    __IO uint32_t              EventCount;     /*!< I2C Event counter                        */
}I2C_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/


/** @defgroup DMA Register address and offsets
  * @{
  */
#define I2C1_OFFSET                (I2C1_BASE - PERIPH_BASE)
#define I2C2_OFFSET                (I2C2_BASE - PERIPH_BASE)

/** @defgroup I2C_Exported_Constants I2C Exported Constants
  * @{
  */


/** @defgroup I2C_Error_Code I2C Error Code
  * @brief    I2C Error Code 
  * @{
  */ 
#define HAL_I2C_ERROR_NONE          (0x00000000U)    /*!< No error           */
#define HAL_I2C_ERROR_BERR          (0x00000001U)    /*!< BERR error         */
#define HAL_I2C_ERROR_ARLO          (0x00000002U)    /*!< ARLO error         */
#define HAL_I2C_ERROR_AF            (0x00000004U)    /*!< AF error           */
#define HAL_I2C_ERROR_OVR           (0x00000008U)    /*!< OVR error          */
#define HAL_I2C_ERROR_DMA           (0x00000010U)    /*!< DMA transfer error */
#define HAL_I2C_ERROR_TIMEOUT       (0x00000020U)    /*!< Timeout Error      */
/**
  * @}
  */


/** @defgroup I2C_Master_Mode I2C master mode enable/disable
  * @{
  */
#define I2C_MASTER_MODE_DISABLE      ((uint32_t)0x00000000U)
#define I2C_MASTER_MODE_ENABLE       I2C_CON_MASTER_MODE

/**
  * @}
  */


/** @defgroup I2C_Slave_Mode I2C slave mode enable/disable
  * @{
  */
#define I2C_SLAVE_MODE_ENABLE         ((uint32_t)0x00000000U)
#define I2C_SLAVE_MODE_DISABLE        I2C_CON_SLAVE_DISABLE

/**
  * @}
  */


/** @defgroup I2C_Restart_Enable I2C restart enable/disable
  * @{
  */
#define I2C_RESTART_DISABLE          ((uint32_t)0x00000000U) 
#define I2C_RESTART_ENABLE           I2C_CON_RESTART_EN

#define I2C_SPEED_STANDARD           (((uint32_t)0x01)<<I2C_CON_SPEED_Pos)
#define I2C_SPEED_FAST               (((uint32_t)0x02)<<I2C_CON_SPEED_Pos)
#define I2C_SPEED_FASTPLUS           (((uint32_t)0x02)<<I2C_CON_SPEED_Pos)


/**
  * @}
  */

/** @defgroup I2C_Master_Addr_Bits I2C master address bits
  * @{
  */
#define I2C_MASTER_ADDRESS7BITS     ((uint32_t)0x00000000U)
#define I2C_MASTER_ADDRESS10BITS    I2C_CON_10BITADDR_MASTER

/**
  * @}
  */


/** @defgroup I2C_Slave_Addr_Bits I2C slave address bits
  * @{
  */
#define I2C_SLAVE_ADDRESS7BITS      ((uint32_t)0x00000000U)
#define I2C_SLAVE_ADDRESS10BITS     I2C_CON_10BITADDR_SLAVE

/**
  * @}
  */

 /** @defgroup I2C_Slave_Dual_Addr I2C slave dual address enable/disable
  * @{
  */
#define I2C_SLAVE_DUAL_ADDR_DISABLE ((uint32_t)0x00000000U)
#define I2C_SLAVE_DUAL_ADDR_ENABLE  I2C_CON_SAR_DUAL_EN

/**
  * @}
  */


/** @defgroup I2C_Interrupt_configuration_definition I2C Interrupt configuration definition
  * @brief I2C Interrupt definition
  *        Elements values convention: 0xXXXXXXXX
  *           - XXXXXXXX  : Interrupt control mask
  * @{
  */
#define I2C_IT_ALL                              0xFFFF  
#define I2C_IT_RX_UNDER                         I2C_INTR_MASK_M_RX_UNDER
#define I2C_IT_RX_OVER                          I2C_INTR_MASK_M_RX_OVER
#define I2C_IT_RX_FULL                          I2C_INTR_MASK_M_RX_FULL
#define I2C_IT_TX_OVER                          I2C_INTR_MASK_M_TX_OVER
#define I2C_IT_TX_EMPTY                         I2C_INTR_MASK_M_TX_EMPTY
#define I2C_IT_RD_REQ                           I2C_INTR_MASK_M_RD_REQ
#define I2C_IT_TX_ABRT                          I2C_INTR_MASK_M_TX_ABRT

#define I2C_IT_RX_DONE                          I2C_INTR_MASK_M_RX_DONE
#define I2C_IT_ACTIVITY                         I2C_INTR_MASK_M_ACTIVITY
#define I2C_IT_STOP_DET                         I2C_INTR_MASK_M_STOP_DET
#define I2C_IT_START_DET                        I2C_INTR_MASK_M_START_DET
#define I2C_IT_GEN_CALL                         I2C_INTR_MASK_M_GEN_CALL

#define I2C_IT_SCL_STUCK_AT_LOW                 I2C_INTR_MASK_SCL_STUCK_AT_LOW
#define I2C_IT_RX_NOT_EMPTY                     I2C_INTR_MASK_RX_NOT_EMPTY
#define I2C_IT_TX_DONE                          I2C_INTR_MASK_TX_DONE
#define I2C_IT_ADDR_MATCH                       I2C_INTR_MASK_ADDR_MATCH
#define I2C_IT_RECEIVE_NACK                     I2C_INTR_MASK_RECEIVE_NACK
#define I2C_IT_BUS_ERROR                        I2C_INTR_MASK_BUS_ERROR
#define I2C_IT_ARB_LOST                         I2C_INTR_MASK_ARB_LOST

/**
  * @}
  */



/** @defgroup I2C_Flag_definition I2C Flag definition
  * @{
  */
#define I2C_FLAG_RX_UNDER                       I2C_INTR_STAT_R_RX_UNDER
#define I2C_FLAG_RX_OVER                        I2C_INTR_STAT_R_RX_OVER
#define I2C_FLAG_RX_FULL                        I2C_INTR_STAT_R_RX_FULL
#define I2C_FLAG_TX_OVER                        I2C_INTR_STAT_R_TX_OVER
#define I2C_FLAG_TX_EMPTY                       I2C_INTR_STAT_R_TX_EMPTY
#define I2C_FLAG_RD_REQ                         I2C_INTR_STAT_R_RD_REQ
#define I2C_FLAG_TX_ABRT                        I2C_INTR_STAT_R_TX_ABRT

#define I2C_FLAG_RX_DONE                        I2C_INTR_STAT_R_RX_DONE
#define I2C_FLAG_ACTIVITY                       I2C_INTR_STAT_R_ACTIVITY
#define I2C_FLAG_STOP_DET                       I2C_INTR_STAT_R_STOP_DET
#define I2C_FLAG_START_DET                      I2C_INTR_STAT_R_START_DET
#define I2C_FLAG_GEN_CALL                       I2C_INTR_STAT_R_GEN_CALL

#define I2C_FLAG_SCL_STUCK_AT_LOW               I2C_INTR_STAT_R_SCL_STUCK_AT_LOW
#define I2C_FLAG_ARB_LOST                       I2C_TX_ABRT_SOURCE_ARB_LOST

/**
  * @}
  */



/** @defgroup I2C_Cmd_definition I2C Cmd definition
  * @{
  */
#define I2C_CMD_READ                            I2C_DATA_CMD_CMD
#define I2C_CMD_WRITE                           ((uint32_t)0x00000000U)
#define I2C_CMD_STOP                            I2C_DATA_CMD_STOP


/**
  * @}
  */


/** @defgroup I2C_DeviceId I2C deviceid cmd
  * @{
  */
#define I2C_DEVICEID_READ                       ((uint32_t)(I2C_TAR_GC_OR_START /*| I2C_TAR_SPECIAL*/))

/**
  * @}
  */


/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/

/** @defgroup I2C_Exported_Macros I2C Exported Macros
  * @{
  */
    
/**
  * @brief  Enable/Disable the I2C peripheral.
  * @param  __HANDLE__: I2C handle
  * @retval None
  */
#define __HAL_I2C_ENABLE(__HANDLE__)            ((__HANDLE__)->Instance->ENABLE |=  (I2C_ENABLE_ENABLE))
#define __HAL_I2C_DISABLE(__HANDLE__)           ((__HANDLE__)->Instance->ENABLE &=  ~(I2C_ENABLE_ENABLE))


/** @brief  Enable or disable the specified I2C interrupts.
  * @param  __HANDLE__: specifies the I2C Handle.
  *         This parameter can be I2C where x: 1, 2, or 3 to select the I2C peripheral.
  * @param  __INTERRUPT__: specifies the interrupt source to enable or disable.
  *         This parameter can be one of the following values:
  *            @arg @ref I2C_IT_ARB_LOST                Arbitration lost  
  *            @arg @ref I2C_IT_BUS_ERROR               Bus error  
  *            @arg @ref I2C_IT_RECEIVE_NACK    
  *            @arg @ref I2C_IT_ADDR_MATCH          Address matched (slave mode)
  *            @arg @ref I2C_IT_TX_DONE                 Transfer done
  *            @arg @ref I2C_IT_RX_NOT_EMPTY        Receive data register not empty
  *            @arg @ref I2C_IT_SCL_STUCK_AT_LOW      
  *            @arg @ref I2C_IT_MST_ON_HOLD     
  *            @arg @ref I2C_IT_RESTART_DET         Restart detect
  *            @arg @ref I2C_IT_GEN_CALL    
  *            @arg @ref I2C_IT_START_DET               start detect
  *            @arg @ref I2C_IT_STOP_DET                    STOP detection flag
  *            @arg @ref I2C_IT_ACTIVITY   
  *            @arg @ref I2C_IT_RX_DONE                 Receive data done
  *            @arg @ref I2C_IT_TX_ABRT     
  *            @arg @ref I2C_IT_RD_REQ 
    *            @arg @ref I2C_IT_TX_EMPTY                  Transmit data register empty
    *            @arg @ref I2C_IT_TX_OVER
    *            @arg @ref I2C_IT_RX_FULL
    *            @arg @ref I2C_IT_RX_OVER
    *            @arg @ref I2C_IT_RX_UNDER
    *            @arg @ref I2C_IT_ALL   
  *
  * @retval None
  */
#define __HAL_I2C_ENABLE_IT(__HANDLE__, __INTERRUPT__)   ((__HANDLE__)->Instance->INTR_MASK |= (__INTERRUPT__))
#define __HAL_I2C_DISABLE_IT(__HANDLE__, __INTERRUPT__)  ((__HANDLE__)->Instance->INTR_MASK &= (~(__INTERRUPT__)))





/** @brief  Check whether the specified I2C interrupt source is enabled or not.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __INTERRUPT__ specifies the I2C interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg @ref I2C_IT_ARB_LOST                Arbitration lost  
  *            @arg @ref I2C_IT_BUS_ERROR               Bus error  
  *            @arg @ref I2C_IT_RECEIVE_NACK    
  *            @arg @ref I2C_IT_ADDR_MATCH          Address matched (slave mode)
  *            @arg @ref I2C_IT_TX_DONE                 Transfer done
  *            @arg @ref I2C_IT_RX_NOT_EMPTY        Receive data register not empty
  *            @arg @ref I2C_IT_SCL_STUCK_AT_LOW      
  *            @arg @ref I2C_IT_MST_ON_HOLD     
  *            @arg @ref I2C_IT_RESTART_DET         Restart detect
  *            @arg @ref I2C_IT_GEN_CALL    
  *            @arg @ref I2C_IT_START_DET               start detect
  *            @arg @ref I2C_IT_STOP_DET                    STOP detection flag
  *            @arg @ref I2C_IT_ACTIVITY   
  *            @arg @ref I2C_IT_RX_DONE                 Receive data done
  *            @arg @ref I2C_IT_TX_ABRT     
  *            @arg @ref I2C_IT_RD_REQ 
    *            @arg @ref I2C_IT_TX_EMPTY                  Transmit data register empty
    *            @arg @ref I2C_IT_TX_OVER
    *            @arg @ref I2C_IT_RX_FULL
    *            @arg @ref I2C_IT_RX_OVER
    *            @arg @ref I2C_IT_RX_UNDER
  *
  * @retval The new state of __INTERRUPT__ (SET or RESET).
  */
#define __HAL_I2C_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)  ((((__HANDLE__)->Instance->INTR_MASK & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)


/** @brief  Check whether the specified I2C flag is set or not.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __FLAG__ specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg @ref I2C_FLAG_ARB_LOST                  Arbitration lost  
  *            @arg @ref I2C_FLAG_BUS_ERROR                 Bus error  
  *            @arg @ref I2C_FLAG_RECEIVE_NACK    
  *            @arg @ref I2C_FLAG_ADDR_MATCH            Address matched (slave mode)
  *            @arg @ref I2C_FLAG_TX_DONE               Transfer done
  *            @arg @ref I2C_FLAG_RX_NOT_EMPTY          Receive data register not empty
  *            @arg @ref I2C_FLAG_SCL_STUCK_AT_LOW      
  *            @arg @ref I2C_FLAG_MST_ON_HOLD     
  *            @arg @ref I2C_FLAG_RESTART_DET           Restart detect
  *            @arg @ref I2C_FLAG_GEN_CALL    
  *            @arg @ref I2C_FLAG_START_DET                 start detect
  *            @arg @ref I2C_FLAG_STOP_DET                  STOP detection flag
  *            @arg @ref I2C_FLAG_ACTIVITY   
  *            @arg @ref I2C_FLAG_RX_DONE                   Receive data done
  *            @arg @ref I2C_FLAG_TX_ABRT     
  *            @arg @ref I2C_FLAG_RD_REQ 
    *            @arg @ref I2C_FLAG_TX_EMPTY                    Transmit data register empty
    *            @arg @ref I2C_FLAG_TX_OVER
    *            @arg @ref I2C_FLAG_RX_FULL
    *            @arg @ref I2C_FLAG_RX_OVER
    *            @arg @ref I2C_FLAG_RX_UNDER
    
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__)                    (((((__HANDLE__)->Instance->RAW_INTR_STAT) & (__FLAG__)) == (__FLAG__)) ? SET : RESET)



/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_TX_ABRT_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_TX_ABRT)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RD_REQ_FLAG(__HANDLE__, __READBACK__)       (__READBACK__ = (__HANDLE__)->Instance->CLR_RD_REQ)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_TX_DONE_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_TX_DONE)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_ADDR_MATCH_FLAG(__HANDLE__, __READBACK__)   (__READBACK__ = (__HANDLE__)->Instance->CLR_ADDR_MATCH)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_INTR_FLAG(__HANDLE__, __READBACK__)         (__READBACK__ = (__HANDLE__)->Instance->CLR_INTR)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RX_UNDER_FLAG(__HANDLE__, __READBACK__)     (__READBACK__ = (__HANDLE__)->Instance->CLR_RX_UNDER)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RX_OVER_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_RX_OVER)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_TX_OVER_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_TX_OVER)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RX_DONE_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_RX_DONE)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_ACTIVITY_FLAG(__HANDLE__, __READBACK__)     (__READBACK__ = (__HANDLE__)->Instance->CLR_ACTIVITY)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_STOP_DET_FLAG(__HANDLE__, __READBACK__)     (__READBACK__ = (__HANDLE__)->Instance->CLR_STOP_DET)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_START_DET_FLAG(__HANDLE__, __READBACK__)    (__READBACK__ = (__HANDLE__)->Instance->CLR_START_DET)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_GEN_CALL_FLAG(__HANDLE__, __READBACK__)     (__READBACK__ = (__HANDLE__)->Instance->CLR_GEN_CALL)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RESTART_DET_FLAG(__HANDLE__, __READBACK__)      (__READBACK__ = (__HANDLE__)->Instance->CLR_RESTART_DET)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_SCL_STUCK_DET_FLAG(__HANDLE__, __READBACK__)    (__READBACK__ = (__HANDLE__)->Instance->CLR_SCL_STUCK_DET)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_SMBUS_INTR_FLAG(__HANDLE__, __READBACK__)       (__READBACK__ = (__HANDLE__)->Instance->CLR_SMBUS_INTR)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_RECEIVE_NACK_FLAG(__HANDLE__, __READBACK__)     (__READBACK__ = (__HANDLE__)->Instance->CLR_RECEIVE_NACK)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_BUS_ERROR_FLAG(__HANDLE__, __READBACK__)        (__READBACK__ = (__HANDLE__)->Instance->CLR_BUS_ERROR)


/** @brief  Clear the I2C pending flags which are cleared by reading specific bit.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __READBACK__ the instance to save the read back state.
  * @retval None
  */
#define __HAL_I2C_CLEAR_ARB_LOST_FLAG(__HANDLE__, __READBACK__)         (__READBACK__ = (__HANDLE__)->Instance->CLR_ARB_LOST)

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/

/** @addtogroup I2C_Exported_Functions
  * @{
  */ 

/* Initialization/de-initialization functions  ********************************/
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c);
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c);

/* MSP functions  *************************************************************/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c); 

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2C_Read_DeviceId(I2C_HandleTypeDef *hi2c, uint8_t *pDeviceId);
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
 
#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_I2C_H_ */
