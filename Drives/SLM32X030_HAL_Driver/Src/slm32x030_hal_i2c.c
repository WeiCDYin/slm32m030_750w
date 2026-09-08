/**
 ******************************************************************************
 * @file    slm32x030_hal_i2c.c
 * @author  SOC Team
 * @version V0.0.1
 * @date    8-11-2024
 * @brief
 ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

#ifdef HAL_I2C_MODULE_ENABLED

/** @addtogroup I2C
 * @brief I2C HAL module driver.
 * @{
 */

/* Private define ------------------------------------------------------------*/
/** @addtogroup I2C_Private_Define
 * @{
 */
#define I2C_TIMEOUT_FLAG             (35U)         /*!< Timeout 35 ms             */
#define I2C_TIMEOUT_BUSY_FLAG        (25U)         /*!< Timeout 25 ms             */
#define I2C_NO_OPTION_FRAME          (0xFFFF0000U) /*!< XferOptions default value */

/* Private define for @ref PreviousState usage */
#define I2C_STATE_MSK                ((uint32_t)((HAL_I2C_STATE_BUSY_TX | HAL_I2C_STATE_BUSY_RX) & (~(uint32_t)HAL_I2C_STATE_READY)))  /*!< Mask State define, keep only RX and TX bits            */
#define I2C_STATE_NONE               ((uint32_t)(HAL_I2C_MODE_NONE))                                                                   /*!< Default Value                                          */
#define I2C_STATE_MASTER_BUSY_TX     ((uint32_t)((HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | HAL_I2C_MODE_MASTER))           /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define I2C_STATE_MASTER_BUSY_RX     ((uint32_t)((HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | HAL_I2C_MODE_MASTER))           /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define I2C_STATE_SLAVE_BUSY_TX      ((uint32_t)((HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | HAL_I2C_MODE_SLAVE))            /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define I2C_STATE_SLAVE_BUSY_RX      ((uint32_t)((HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | HAL_I2C_MODE_SLAVE))            /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */

/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/

/* Private functions to handle DMA transfer */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma);

/** @defgroup I2C_Private_Functions I2C Private Functions
 * @{
 */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);
static HAL_StatusTypeDef I2C_SlaveReceive_RXFULL(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_Master_ADD10(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_Master_ADDR(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_MasterReceive_RXFULL(I2C_HandleTypeDef *hi2c);
/**
 * @}
 */

/** @addtogroup I2C_Exported_Functions
 * @{
 */
/**
 * @brief  Return the I2C handle state.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval HAL state
 */
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)
{
    /* Return I2C handle state */
    return hi2c->State;
}

/**
 * @brief  Return the I2C error code.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *              the configuration information for the specified I2C.
 * @retval I2C Error Code
 */
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)
{
    return hi2c->ErrorCode;
}

/**
 * @brief  Initializes the I2C according to the specified parameters in the
 *         I2C_InitTypeDef and creates the associated handle.
 * @param  hi2c : I2C handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == NULL)
    {
        return HAL_ERROR;
    }

    if (hi2c->State == HAL_I2C_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hi2c->Lock = HAL_UNLOCKED;
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        HAL_I2C_MspInit(hi2c);
    }

    /*i2c1 master init flow reference to user manual*/
    hi2c->State = HAL_I2C_STATE_BUSY;

    /* step 1 -- disable i2c*/
    __HAL_I2C_DISABLE(hi2c);

    if (hi2c->Init.master == I2C_MASTER_MODE_ENABLE)
    {
        /* step 2 -- config i2c speed, address bits mode, disable slave mode,enable master mode*/
        hi2c->Instance->CON &= (uint32_t)(~(I2C_CON_10BITADDR_MASTER | I2C_CON_SLAVE_DISABLE | I2C_CON_MASTER_MODE | I2C_CON_SPEED));
        hi2c->Instance->CON = (hi2c->Init.masterAddrBits | hi2c->Init.slave | hi2c->Init.master | hi2c->Init.speedMode);

        /* step 3 -- set i2c tansfer deivece address */
        hi2c->Instance->TAR &= (uint32_t)(~(I2C_TAR_TAR));
        hi2c->Instance->TAR |= hi2c->Init.slaveAddr;
    }
    else if (hi2c->Init.slave != I2C_SLAVE_MODE_DISABLE)
    {

        /* step2 --  set slave device addres(include dual address)*/
        hi2c->Instance->SAR &= (uint32_t)(~(I2C_SAR_IC_SAR | I2C_SAR_IC_SAR_DUAL));
        hi2c->Instance->SAR |= hi2c->Init.slaveAddr;

        if (hi2c->Init.dualSlaveAddrEn == I2C_SLAVE_DUAL_ADDR_ENABLE)
        {
            hi2c->Instance->CON &= (~(I2C_SAR_IC_SAR_DUAL));
            hi2c->Instance->CON |= hi2c->Init.dualSlaveAddrEn;
            hi2c->Instance->SAR |= (hi2c->Init.slaveAddr2 << I2C_SAR_IC_SAR_DUAL_Pos);
        }

        /* step3 -- set slave address bits mode, enable slave mode, disable master mode*/
        hi2c->Instance->CON &= (uint32_t)(~(I2C_CON_10BITADDR_SLAVE | I2C_CON_SLAVE_DISABLE | I2C_CON_MASTER_MODE));
        hi2c->Instance->CON |= (hi2c->Init.slaveAddrBits | hi2c->Init.slave | hi2c->Init.master);
    }

    hi2c->Instance->CON |= hi2c->Init.restart;

    /* It is specified in the user manual that low byte must be written first */
    if (hi2c->Init.speedMode != I2C_SPEED_STANDARD)
    {
        hi2c->Instance->FS_SCL_LCNT = hi2c->Init.sclLcnt;
        hi2c->Instance->FS_SCL_HCNT = hi2c->Init.sclHcnt;
    }
    else
    {
        hi2c->Instance->SS_SCL_LCNT = hi2c->Init.sclLcnt;
        hi2c->Instance->SS_SCL_HCNT = hi2c->Init.sclHcnt;
    }

    hi2c->Instance->RX_TL = hi2c->Init.rxFifoThd;

    hi2c->Instance->TX_TL = hi2c->Init.txFifoThd;

    hi2c->Instance->SMBUS_THIGH_MAX_IDLE_COUNT = 0xf;

    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    return HAL_OK;
}

/**
 * @brief  Initializes the I2C MSP.
 * @param  hi2c : I2C handle
 * @retval None
 */
__weak void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_LPTIM_MspInit could be implemented in the user file
     */
}

/**
 * @brief  DeInitializes the I2C peripheral.
 * @param  hi2c : I2C handle
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
    /* Check the LPTIM handle allocation */
    if (hi2c == NULL)
    {
        return HAL_ERROR;
    }

    hi2c->State = HAL_I2C_STATE_BUSY;

    /* Disable the LPTIM Peripheral Clock */
    __HAL_I2C_DISABLE(hi2c);

    /* DeInit the low level hardware: CLOCK, NVIC.*/
    HAL_I2C_MspDeInit(hi2c);

    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->State = HAL_I2C_STATE_RESET;
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* Release Lock */
    __HAL_UNLOCK(hi2c);

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  DeInitializes the I2C MSP.
 * @param  hi2c : I2C handle
 * @retval None
 */
__weak void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_LPTIM_MspInit could be implemented in the user file
     */
}

/**
 * @brief  Transmits in master mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shift at right before call interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t timeout)
{
    volatile uint32_t tickstart = 0U;
    volatile uint8_t flag_test = 0;
    uint32_t readback;
    HAL_StatusTypeDef ret = HAL_OK;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    __HAL_LOCK(hi2c);
    __HAL_I2C_DISABLE(hi2c);

    hi2c->Intf = HAL_I2C_TNTERFACE_BLOCK;
    hi2c->Devaddress = (uint32_t)DevAddress;
    /* ADD10 Sent -------------------------------------------------------------*/
    if ((hi2c->Instance->CON & I2C_CON_10BITADDR_MASTER) == I2C_CON_10BITADDR_MASTER)
    {
        I2C_Master_ADD10(hi2c);
    }
    else
    {
        I2C_Master_ADDR(hi2c);
    }
    __HAL_I2C_DISABLE_IT(hi2c, (I2C_IT_TX_EMPTY | I2C_IT_RX_FULL));
    __HAL_I2C_CLEAR_RD_REQ_FLAG(hi2c, readback);
    __HAL_I2C_ENABLE(hi2c);

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;

    tickstart = HAL_GetTick();
    while (hi2c->XferSize > 0)
    {
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TX_EMPTY, RESET, tickstart, timeout) != HAL_OK)
        {
            return HAL_TIMEOUT;
        }

        if (hi2c->XferSize == 1)
        {
            hi2c->Instance->DATA_CMD = (I2C_CMD_STOP | (*pData++));
        }
        else
        {
            hi2c->Instance->DATA_CMD = (I2C_CMD_WRITE | (*pData++));
        }
        hi2c->XferSize--;
        hi2c->XferCount--;
    }

    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ACTIVITY, RESET, tickstart, timeout) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }

    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TX_ABRT))
    {
        __HAL_I2C_CLEAR_TX_ABRT_FLAG(hi2c, readback);
        UNUSED(readback);
        ret = HAL_ERROR;
    }

    if (ret == HAL_OK)
    {
        while (1)
        {
            if ((hi2c->Instance->STATUS & I2C_STATUS_TFE) == I2C_STATUS_TFE)
            {
                break;
            }

            if (HAL_GetTick() - tickstart > timeout)
            {
                ret = HAL_TIMEOUT;
            }
        }
    }

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;
    __HAL_I2C_DISABLE(hi2c);
    __HAL_UNLOCK(hi2c);

    return ret;
}

/**
 * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        hi2c->Intf = HAL_I2C_TNTERFACE_IT;
        hi2c->Devaddress = (uint32_t)DevAddress;
        /* ADD10 Sent -------------------------------------------------------------*/
        if ((hi2c->Instance->CON & I2C_CON_10BITADDR_MASTER) == I2C_CON_10BITADDR_MASTER)
        {
            I2C_Master_ADD10(hi2c);
        }
        /* ADDR Sent --------------------------------------------------------------*/
        else
        {
            I2C_Master_ADDR(hi2c);
        }
    }

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;
    hi2c->Devaddress = DevAddress;

    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_ALL);
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_TX_EMPTY);
    __HAL_I2C_ENABLE(hi2c);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef errorcode = HAL_OK;
    uint32_t *p_tmp_data = (uint32_t *)pData;
    uint32_t tmp_reg;

    /* Process Locked */
    __HAL_LOCK(hi2c);

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        errorcode = HAL_BUSY;
        goto error;
    }

    hi2c->Intf = HAL_I2C_TNTERFACE_DMA;
    hi2c->Devaddress = (uint32_t)DevAddress;

    if ((hi2c->Instance->CON & I2C_CON_10BITADDR_MASTER) == I2C_CON_10BITADDR_MASTER)
    {
        I2C_Master_ADD10(hi2c);
    }
    else
    {
        I2C_Master_ADDR(hi2c);
    }

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);
    __HAL_I2C_DISABLE(hi2c);

    p_tmp_data[Size - 1] = p_tmp_data[Size - 1] | I2C_DATA_CMD_STOP;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr = pData;

    hi2c->XferCount = Size;
    hi2c->XferSize = Size;

    /* Set the I2C DMA transfer complete callback */
    hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

    /* Set the DMA error callback */
    hi2c->hdmatx->XferErrorCallback = NULL;

    /* Set the unused DMA callbacks to NULL */
    hi2c->hdmatx->XferAbortCallback = NULL;

    // hi2c->Instance->DMA_CR |= I2C_DMA_CR_TDMAE;

    HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)(&(hi2c->Instance->DATA_CMD)), hi2c->XferSize);
    __HAL_I2C_ENABLE(hi2c);
    hi2c->Instance->DMA_TDLR = 0x00;

    tmp_reg = hi2c->Instance->DMA_CR;
    tmp_reg |= I2C_DMA_CR_TDMAE;
    hi2c->Instance->DMA_CR = tmp_reg;

error:
    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);
    return errorcode;
}

/**
 * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    __HAL_I2C_DISABLE(hi2c);
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        hi2c->Intf = HAL_I2C_TNTERFACE_IT;
        hi2c->Devaddress = (uint32_t)DevAddress;
        /* ADD10 Sent -------------------------------------------------------------*/
        if ((hi2c->Instance->CON & I2C_CON_10BITADDR_MASTER) == I2C_CON_10BITADDR_MASTER)
        {
            I2C_Master_ADD10(hi2c);
        }
        /* ADDR Sent --------------------------------------------------------------*/
        else
        {
            I2C_Master_ADDR(hi2c);
        }
    }

    hi2c->Instance->DATA_CMD &= ~(I2C_DATA_CMD_CMD | I2C_DATA_CMD_DAT);

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;
    hi2c->Devaddress = DevAddress;

    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_ALL);
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_RX_FULL);

    __HAL_I2C_ENABLE(hi2c);

    if (Size == 1)
    {
        hi2c->Instance->DATA_CMD |= I2C_CMD_READ | I2C_CMD_STOP;
    }
    else
    {
        hi2c->Instance->DATA_CMD |= I2C_CMD_READ;
    }

    return HAL_OK;
}
/**
 * @brief  Receives in master mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shift at right before call interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t timeout)
{
    uint32_t tickstart = 0U;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    __HAL_UNLOCK(hi2c);
    __HAL_I2C_DISABLE(hi2c);
    __HAL_I2C_DISABLE_IT(hi2c, (I2C_IT_TX_EMPTY | I2C_IT_RX_FULL));

    hi2c->Intf = HAL_I2C_TNTERFACE_BLOCK;
    /* ADD10 Sent -------------------------------------------------------------*/
    if ((hi2c->Instance->CON & I2C_CON_10BITADDR_MASTER) == I2C_CON_10BITADDR_MASTER)
    {
        I2C_Master_ADD10(hi2c);
    }
    else
    {
        I2C_Master_ADDR(hi2c);
    }

    __HAL_I2C_ENABLE(hi2c);
    /* Prepare transfer parameters */
    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;
    tickstart = HAL_GetTick();

    while (hi2c->XferSize > 0)
    {
        if (hi2c->XferSize == 1)
        {
            hi2c->Instance->DATA_CMD = (I2C_CMD_READ | I2C_CMD_STOP);
            if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RX_FULL, RESET, tickstart, timeout) != HAL_OK)
            {
                return HAL_TIMEOUT;
            }
            (*pData++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
        }
        else
        {
            hi2c->Instance->DATA_CMD |= I2C_CMD_READ;
            if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RX_FULL, RESET, tickstart, timeout) != HAL_OK)
            {
                return HAL_TIMEOUT;
            }
            (*pData++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
        }
        hi2c->XferCount--;
        hi2c->XferSize--;
    }

    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ACTIVITY, RESET, tickstart, timeout) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;
    __HAL_I2C_DISABLE(hi2c);
    return HAL_OK;
}

/**
 * @brief  Transmits in slave mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t timeout)
{
    uint32_t readback;
    volatile uint32_t tickstart;
    HAL_StatusTypeDef ret = HAL_OK;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    hi2c->Intf = HAL_I2C_TNTERFACE_BLOCK;
    hi2c->State = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;
    __HAL_I2C_ENABLE(hi2c);

    tickstart = HAL_GetTick();
    while (hi2c->XferSize > 0)
    {
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RD_REQ))
        {
            if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TX_ABRT))
            {
                UNUSED(readback);
                __HAL_I2C_CLEAR_TX_ABRT_FLAG(hi2c, readback);
                UNUSED(readback);
            }
            hi2c->Instance->DATA_CMD = *hi2c->pBuffPtr++;
            __HAL_I2C_CLEAR_RD_REQ_FLAG(hi2c, readback);
            hi2c->XferSize--;
            hi2c->XferCount--;
        }

        if (hi2c->XferCount == 0)
        {
            hi2c->State = HAL_I2C_STATE_READY;
            hi2c->Mode = HAL_I2C_MODE_NONE;
            ret = HAL_OK;
            break;
        }

        if (HAL_GetTick() - tickstart > timeout)
        {
            ret = HAL_TIMEOUT;
            break;
        }
    }

    __HAL_I2C_DISABLE(hi2c);

    return ret;
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        hi2c->Intf = HAL_I2C_TNTERFACE_IT;
        hi2c->State = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr = pData;
        hi2c->XferSize = Size;
        hi2c->XferCount = Size;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;

        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_ALL);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_RD_REQ);


        __HAL_I2C_ENABLE(hi2c);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
    uint32_t tmp_reg;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    if ((pData == NULL) || (Size == 0U))
    {
        return HAL_ERROR;
    }

    hi2c->Intf = HAL_I2C_TNTERFACE_DMA;

    /* Process Locked */
    __HAL_LOCK(hi2c);
    __HAL_I2C_DISABLE(hi2c);
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_RX_FULL);

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;

    /* Set the I2C DMA transfer complete callback */
    hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

    /* Set the DMA error callback */
    hi2c->hdmatx->XferErrorCallback = NULL;

    /* Set the unused DMA callbacks to NULL */
    hi2c->hdmatx->XferAbortCallback = NULL;

    __HAL_DMA_CLEAR_INTTCCLR_FLAG(hi2c->hdmatx, 0x3F);
    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DATA_CMD, hi2c->XferSize);
    __HAL_I2C_ENABLE(hi2c);

    /* Enable Address Acknowledge */
    __HAL_DMA_CLEAR_INTTCCLR_FLAG(hi2c->hdmatx, 0x3F);

    tmp_reg = hi2c->Instance->DMA_CR;
    tmp_reg |= I2C_DMA_CR_TDMAE;
    hi2c->Instance->DMA_CR = tmp_reg;
    hi2c->Instance->DMA_TDLR = 0x00;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);
    return HAL_OK;
}
/**
 * @brief  Receive in slave mode an amount of data in blocking mode
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */

HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    volatile uint32_t tickstart;
    HAL_StatusTypeDef ret = HAL_OK;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    hi2c->Intf = HAL_I2C_TNTERFACE_BLOCK;
    hi2c->State = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr = pData;
    hi2c->XferSize = Size;
    hi2c->XferCount = Size;

    __HAL_LOCK(hi2c);
    __HAL_I2C_DISABLE(hi2c);
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_RX_FULL);
    __HAL_I2C_ENABLE(hi2c);

    tickstart = HAL_GetTick();
    while (hi2c->XferSize > 0)
    {
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RX_FULL))
        {
            *pData++ = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
            hi2c->XferSize--;
            hi2c->XferCount--;
        }

        if (hi2c->XferCount == 0U)
        {
            /* Last Byte is received */
            hi2c->State = HAL_I2C_STATE_READY;
            hi2c->Mode = HAL_I2C_MODE_NONE;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            break;
        }

        if (HAL_GetTick() - tickstart > Timeout)
        {
            ret = HAL_TIMEOUT;
            break;
        }
    }

    __HAL_I2C_DISABLE(hi2c);

    return ret;
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        hi2c->Intf = HAL_I2C_TNTERFACE_IT;
        hi2c->State = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr = pData;
        hi2c->XferSize = Size;
        hi2c->XferCount = Size;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;

        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_ALL);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_RX_FULL);

        __HAL_I2C_ENABLE(hi2c);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
    uint32_t tmp_reg;

    if (hi2c->State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    if ((pData == NULL) || (Size == 0U))
    {
        return HAL_ERROR;
    }

    hi2c->Intf = HAL_I2C_TNTERFACE_DMA;

    /* Process Locked */
    __HAL_LOCK(hi2c);
    __HAL_I2C_DISABLE(hi2c);
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_RX_FULL);

    hi2c->State = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr = pData;
    hi2c->XferCount = Size;
    hi2c->XferSize = hi2c->XferCount;

    /* Set the I2C DMA transfer complete callback */
    hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

    /* Set the DMA error callback */
    hi2c->hdmarx->XferErrorCallback = NULL;

    /* Set the unused DMA callbacks to NULL */
    hi2c->hdmarx->XferAbortCallback = NULL;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DATA_CMD, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
    __HAL_I2C_ENABLE(hi2c);

    /* Enable Address Acknowledge */
    tmp_reg = hi2c->Instance->DMA_CR;
    tmp_reg |= I2C_DMA_CR_RDMAE;
    hi2c->Instance->DMA_CR = tmp_reg;
    hi2c->Instance->DMA_RDLR = 0x00;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);
    return HAL_OK;
}

/**
 * @brief  Receive in slave mode an amount of data in blocking mode
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  pDeviceId Pointer to DeviceId
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_I2C_Read_DeviceId(I2C_HandleTypeDef *hi2c, uint8_t *pDeviceId)
{
    __HAL_I2C_DISABLE(hi2c);

    hi2c->Instance->TAR |= I2C_DEVICEID_READ;

    __HAL_I2C_ENABLE(hi2c);

    hi2c->Instance->DATA_CMD |= I2C_CMD_READ;
    hi2c->Instance->DATA_CMD |= I2C_CMD_READ;
    hi2c->Instance->DATA_CMD |= I2C_CMD_READ;

    (*pDeviceId++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
    (*pDeviceId++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
    (*pDeviceId++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;

    return HAL_OK;
}

/**
 * @brief  This function handles I2C event interrupt request.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    uint32_t intrStatus = READ_REG(hi2c->Instance->INTR_STAT);
    uint32_t itsources = READ_REG(hi2c->Instance->INTR_MASK);

    uint32_t CurrentMode = hi2c->Mode;

    /* Master mode selected */
    if (CurrentMode == HAL_I2C_MODE_MASTER)
    {
        /* TXE set  -----------------------------------------------*/
        if (((intrStatus & I2C_FLAG_TX_EMPTY) != RESET) && ((itsources & I2C_IT_TX_EMPTY) != RESET))
        {
            I2C_MasterTransmit_TXE(hi2c);
        }
        if ((((intrStatus & I2C_FLAG_RX_FULL) != RESET) && ((itsources & I2C_IT_RX_FULL) != RESET)))
        {
            I2C_MasterReceive_RXFULL(hi2c);
        }
    }
    /* Slave mode selected */
    else
    {
        /* I2C in mode Receiver --------------------------------------------------*/
        /* RXNE set and BTF reset ----------------------------------------------*/

        if (((intrStatus & I2C_FLAG_RX_FULL) != RESET) && ((itsources & I2C_IT_RX_FULL) != RESET))
        {
            I2C_SlaveReceive_RXFULL(hi2c);
        }
        if (((intrStatus & I2C_FLAG_RD_REQ) != RESET) && ((itsources & I2C_IT_RD_REQ) != RESET))
        {

            I2C_SlaveTransmit_TXE(hi2c);
        }
    }
}

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup I2C_Private_Functions
 * @{
 */

/**
 * @brief  Handle ADD10 flag for Master
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *         the configuration information for I2C module
 * @retval HAL status
 */
static HAL_StatusTypeDef I2C_Master_ADD10(I2C_HandleTypeDef *hi2c)
{
    /* Send slave address */
    hi2c->Instance->TAR = hi2c->Devaddress & 0x03FF; // add by cxxu

    return HAL_OK;
}

/**
 * @brief  Handle ADDR flag for Master
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *         the configuration information for I2C module
 * @retval HAL status
 */
static HAL_StatusTypeDef I2C_Master_ADDR(I2C_HandleTypeDef *hi2c)
{
    /* Send slave address */
    hi2c->Instance->TAR = hi2c->Devaddress & 0x007F; //;

    return HAL_OK;
}

/**
 * @brief  Handle TXE flag for Master
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *         the configuration information for I2C module
 * @retval HAL status
 */
static HAL_StatusTypeDef I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->State == HAL_I2C_STATE_BUSY_TX)
    {
        if (hi2c->XferCount == 1U)
        {
            /* Generate Stop */
            hi2c->Instance->DATA_CMD = (I2C_CMD_STOP | (*hi2c->pBuffPtr++));
            hi2c->XferCount--; //
        }
        if (hi2c->XferCount == 0U)
        {
            /* Disable Tx empty interrupt */
            __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);
            hi2c->PreviousState = I2C_STATE_NONE;
            hi2c->State = HAL_I2C_STATE_READY;
            HAL_I2C_MasterTxCpltCallback(hi2c);
        }
        else
        {
            /* Write data to DR */
            hi2c->Instance->DATA_CMD = (*hi2c->pBuffPtr++);
            hi2c->XferCount--;
        }
    }
    return HAL_OK;
}

static HAL_StatusTypeDef I2C_MasterReceive_RXFULL(I2C_HandleTypeDef *hi2c)
{
    uint32_t readback;

    if (hi2c->State == HAL_I2C_STATE_BUSY_TX)
    {
        __HAL_I2C_CLEAR_INTR_FLAG(hi2c, readback);
        UNUSED(readback);

        if (hi2c->XferCount == 2U)
        {
            (*hi2c->pBuffPtr++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
            hi2c->XferCount--;
            hi2c->Instance->DATA_CMD |= I2C_CMD_READ | I2C_CMD_STOP;
        }
        else if (hi2c->XferCount == 1U)
        {
            *hi2c->pBuffPtr = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
            __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_RX_FULL);
            hi2c->PreviousState = I2C_STATE_NONE;
            hi2c->State = HAL_I2C_STATE_READY;
            HAL_I2C_MasterRxCpltCallback(hi2c);
        }
        else
        {
            (*hi2c->pBuffPtr++) = hi2c->Instance->DATA_CMD & I2C_DATA_CMD_DAT;
            hi2c->XferCount--;
            hi2c->Instance->DATA_CMD |= I2C_CMD_READ;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Receives an amount of data in non blocking mode
 * @param  huart: pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C module.
 * @retval HAL status
 */
static HAL_StatusTypeDef I2C_SlaveReceive_RXFULL(I2C_HandleTypeDef *hi2c)
{
    uint32_t rxByteNum;
    rxByteNum = hi2c->Instance->RXFLR;

    if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
    {
        /* Read data from DR */
        while (rxByteNum-- > 0) // rx_full will set after every  4byte received, but ic_rxFLR will set after the number of received datas more than ic_rx_tl
        {
            (*hi2c->pBuffPtr++) = hi2c->Instance->DATA_CMD;
            hi2c->XferCount--;
        }

        if (hi2c->XferCount == 0U)
        {
            /* Last Byte is received, disable Interrupt */
            __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_RX_FULL);

            /* Set state at HAL_I2C_STATE_LISTEN */
            hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;
            hi2c->State = HAL_I2C_STATE_READY;

            /* Call the Rx complete callback to inform upper layer of the end of receive process */
            HAL_I2C_SlaveRxCpltCallback(hi2c);
        }
    }
    return HAL_OK;
}

static HAL_StatusTypeDef I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c)
{
    uint32_t readback;

    if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
    {
        /* Read data from DR */
        hi2c->Instance->DATA_CMD = (*hi2c->pBuffPtr++);
        __HAL_I2C_CLEAR_RD_REQ_FLAG(hi2c, readback);
        UNUSED(readback);
        hi2c->XferCount--;

        if (hi2c->XferCount == 0U)
        {
            /* Last Byte is received, disable Interrupt */
            __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);

            /* Set state at HAL_I2C_STATE_LISTEN */
            hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;
            hi2c->State = HAL_I2C_STATE_READY;

            /* Call the Rx complete callback to inform upper layer of the end of receive process */
            HAL_I2C_SlaveTxCpltCallback(hi2c);
        }
    }
    return HAL_OK;
}

/**
 * @brief  This function handles I2C Communication Timeout.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  Flag Specifies the I2C flag to check.
 * @param  Status The new Flag status (SET or RESET).
 * @param  Timeout Timeout duration
 * @param  Tickstart Tick start value
 * @retval HAL status
 */

static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)
{

    /* Wait until flag is set */
    while ((__HAL_I2C_GET_FLAG(hi2c, Flag) ? SET : RESET) == Status)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if ((Timeout == 0U) || ((HAL_GetTick() - Tickstart) > Timeout))
            {
                hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
                hi2c->State = HAL_I2C_STATE_READY;
                hi2c->Mode = HAL_I2C_MODE_NONE;
                __HAL_I2C_DISABLE(hi2c);
                __HAL_UNLOCK(hi2c);

                return HAL_TIMEOUT;
            }
        }
    }

    return HAL_OK;
}

/**
 * @brief  DMA I2C process complete callback.
 * @param  hdma DMA handle
 * @retval None
 */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma)
{
    uint32_t readback;
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)(hdma->Parent);

    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */

    if (hi2c->State == HAL_I2C_STATE_BUSY_TX)
    {
        if (hi2c->Mode == HAL_I2C_MODE_MASTER)
        {
            HAL_I2C_MasterTxCpltCallback(hi2c);
        }
        else if (hi2c->Mode == HAL_I2C_MODE_SLAVE)
        {
            HAL_I2C_SlaveTxCpltCallback(hi2c);
        }

        /* wait for i2c bus transfer complete*/

        while ((hi2c->Instance->STATUS & I2C_STATUS_TFE) != I2C_STATUS_TFE)
            ;
    }
    else if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
    {
        if (hi2c->Mode == HAL_I2C_MODE_SLAVE)
        {
            HAL_I2C_SlaveRxCpltCallback(hi2c);
        }
    }

    hi2c->Instance->DMA_CR &= ~I2C_DMA_CR_RDMAE;
    hi2c->XferCount = 0U;
    __HAL_I2C_CLEAR_RD_REQ_FLAG(hi2c, readback);
    UNUSED(readback);
    hi2c->State = HAL_I2C_STATE_READY;
    __HAL_I2C_DISABLE(hi2c);
}

/**
 * @brief  Master Tx Transfer completed callback.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_I2C_MasterTxCpltCallback can be implemented in the user file
     */
}

/**
 * @brief  Master Tx Receive completed callback.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_I2C_MasterRxCpltCallback can be implemented in the user file
     */
}

/**
 * @brief  Slave Tx Transfer completed callback.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_I2C_SlaveTxCpltCallback can be implemented in the user file
     */
}

/**
 * @brief  Slave Rx Receive completed callback.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_I2C_SlaveRxCpltCallback can be implemented in the user file
     */
}

/**
 * @}
 */

#endif /* HAL_I2C_MODULE_ENABLED */
