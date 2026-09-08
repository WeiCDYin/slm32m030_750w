/**
  ******************************************************************************
  * @file    slm32x030_hal_usart.c
  * @author  Application Team
  * @brief   USART HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Universal Synchronous Asynchronous Receiver Transmitter (USART) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
    The USART HAL driver can be used as follows:

    (#) Declare a USART_HandleTypeDef handle structure.
    (#) Initialize the USART low level resources by implementing the HAL_USART_MspInit () API:
        (##) Enable the USARTx interface clock.
        (##) USART pins configuration:
             (+++) Enable the clock for the USART GPIOs.
             (+++) Configure the USART pins (TX as alternate function pull-up, RX as alternate function Input).
        (##) NVIC configuration if you need to use interrupt process (HAL_USART_Transmit_IT(),
             HAL_USART_Receive_IT() and HAL_USART_TransmitReceive_IT() APIs):
             (+++) Configure the USARTx interrupt priority.
             (+++) Enable the NVIC USART IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_USART_Transmit_DMA()
             HAL_USART_Receive_DMA() and HAL_USART_TransmitReceive_DMA() APIs):
             (+++) Declare a DMA handle structure for the Tx/Rx channel.
             (+++) Enable the DMAx interface clock.
             (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
             (+++) Configure the DMA Tx/Rx channel.
             (+++) Associate the initialized DMA handle to the USART DMA Tx/Rx handle.
             (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx/Rx channel.
            (+++) Configure the priority and enable the NVIC for the transfer complete
                  interrupt on the DMA Tx/Rx channel.
            (+++) Configure the USARTx interrupt priority and enable the NVIC USART IRQ handle
                  (used for last byte sending completion detection in DMA non circular mode)

    (#) Program the Baud Rate, Word Length, Stop Bit, Parity, Hardware
        flow control and Mode(Receiver/Transmitter) in the husart Init structure.

    (#) Initialize the USART registers by calling the HAL_USART_Init() API:
        (++) These APIs configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
             by calling the customized HAL_USART_MspInit(&husart) API.

        -@@- The specific USART interrupts (Transmission complete interrupt,
             RXNE interrupt and Error Interrupts) will be managed using the macros
             __HAL_USART_ENABLE_IT() and __HAL_USART_DISABLE_IT() inside the transmit and receive process.

    (#) Three operation modes are available within this driver :

     *** Polling mode IO operation ***
     =================================
     [..]
       (+) Send an amount of data in blocking mode using HAL_USART_Transmit()
       (+) Receive an amount of data in blocking mode using HAL_USART_Receive()

     *** Interrupt mode IO operation ***
     ===================================
     [..]
       (+) Send an amount of data in non blocking mode using HAL_USART_Transmit_IT()
       (+) Receive an amount of data in non blocking mode using HAL_USART_Receive_IT()
       (+) At reception end of transfer HAL_USART_RxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_USART_RxCpltCallback
       (+) In case of transfer Error, HAL_USART_ErrorCallback() function is executed and user can
            add his own code by customization of function pointer HAL_USART_ErrorCallback

     *** DMA mode IO operation ***
     ==============================
     [..]
       (+) Send an amount of data in non blocking mode (DMA) using HAL_USART_Transmit_DMA()
       (+) At transmission end of transfer HAL_USART_TxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_USART_TxCpltCallback
       (+) Receive an amount of data in non blocking mode (DMA) using HAL_USART_Receive_DMA()
       (+) At reception end of transfer HAL_USART_RxCpltCallback is executed and user can
            add his own code by customization of function pointer HAL_USART_RxCpltCallback
       (+) In case of transfer Error, HAL_USART_ErrorCallback() function is executed and user can
            add his own code by customization of function pointer HAL_USART_ErrorCallback
       (+) Pause the DMA Transfer using HAL_USART_DMAPause()
       (+) Resume the DMA Transfer using HAL_USART_DMAResume()
       (+) Stop the DMA Transfer using HAL_USART_DMAStop()

     *** USART HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in USART HAL driver.

       (+) __HAL_USART_ENABLE: Enable the USART peripheral
       (+) __HAL_USART_DISABLE: Disable the USART peripheral
       (+) __HAL_USART_GET_FLAG : Check whether the specified USART flag is set or not
       (+) __HAL_USART_CLEAR_FLAG : Clear the specified USART pending flag
       (+) __HAL_USART_ENABLE_IT: Enable the specified USART interrupt
       (+) __HAL_USART_DISABLE_IT: Disable the specified USART interrupt

     [..]
       (@) You can refer to the USART HAL driver header file for more useful macros

  @endverbatim
     [..]
       (@) Additionnal remark: If the parity is enabled, then the MSB bit of the data written
           in the data register is transmitted but is changed by the parity bit.
           Depending on the frame length defined by the M bit (8-bits or 9-bits),
           the possible USART frame formats are as listed in the following table:
    +-------------------------------------------------------------+
    |   M bit |  PCE bit  |            USART frame                 |
    |---------------------|---------------------------------------|
    |    0    |    0      |    | SB | 8 bit data | STB |          |
    |---------|-----------|---------------------------------------|
    |    0    |    1      |    | SB | 7 bit data | PB | STB |     |
    |---------|-----------|---------------------------------------|
    |    1    |    0      |    | SB | 9 bit data | STB |          |
    |---------|-----------|---------------------------------------|
    |    1    |    1      |    | SB | 8 bit data | PB | STB |     |
    +-------------------------------------------------------------+
  ******************************************************************************
 **/

/* Includes ---------------------------------c---------------------------------*/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
 * @{
 */

/** @defgroup USART USART
 * @brief HAL USART Synchronous module driver
 * @{
 */
#ifdef HAL_USART_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup USART_Private_Constants
 * @{
 */
#define DUMMY_DATA 0xFFFFU
#define USART_TIMEOUT_VALUE 22000U
/**
 * @}
 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @addtogroup USART_Private_Functions
 * @{
 */
static void USART_EndTxTransfer(USART_HandleTypeDef *husart);
static void USART_EndRxTransfer(USART_HandleTypeDef *husart);
static HAL_StatusTypeDef USART_Transmit_IT(USART_HandleTypeDef *husart);
static HAL_StatusTypeDef USART_EndTransmit_IT(USART_HandleTypeDef *husart);
static HAL_StatusTypeDef USART_Receive_IT(USART_HandleTypeDef *husart);
static HAL_StatusTypeDef USART_TransmitReceive_IT(USART_HandleTypeDef *husart);
static void USART_SetConfig(USART_HandleTypeDef *husart);
void USART_DMATransmitCplt(DMA_HandleTypeDef *hdma);
void USART_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
void USART_DMAError(DMA_HandleTypeDef *hdma);

static HAL_StatusTypeDef USART_WaitOnFlagUntilTimeout(USART_HandleTypeDef *husart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);
static uint8_t HAL_LIN_CalcParity(uint8_t ID);
static HAL_StatusTypeDef LIN_Slave_Receive(USART_HandleTypeDef *husart);
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup USART_Exported_Functions USART Exported Functions
 * @{
 */

/** @defgroup USART_Exported_Functions_Group1 USART Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
  ==============================================================================
            ##### Initialization and Configuration functions #####
  ==============================================================================
  [..]
  This subsection provides a set of functions allowing to initialize the USART
  in asynchronous and in synchronous modes.
  (+) For the asynchronous mode only these parameters can be configured:
      (++) Baud Rate
      (++) Word Length
      (++) Stop Bit
      (++) Parity: If the parity is enabled, then the MSB bit of the data written
           in the data register is transmitted but is changed by the parity bit.
           Depending on the frame length defined by the M bit (8-bits or 9-bits),
           please refer to Reference manual for possible USART frame formats.
      (++) USART polarity
      (++) USART phase
      (++) USART LastBit
      (++) Receiver/transmitter modes

  [..]
    The HAL_USART_Init() function follows the USART  synchronous configuration
    procedure (details for the procedure are available in reference manuals


@endverbatim
  * @{
  */

/**
 * @brief  Initializes the USART mode according to the specified
 *         parameters in the USART_InitTypeDef and create the associated handle.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Init(USART_HandleTypeDef *husart)
{
    /* Check the USART handle allocation */
    if (husart == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_USART_INSTANCE(husart->Instance));

    if (husart->State == HAL_USART_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        husart->Lock = HAL_UNLOCKED;

        /* Init the low level hardware */
        HAL_USART_MspInit(husart);
    }

    husart->State = HAL_USART_STATE_BUSY;

    /* Set the USART Communication parameters */
    USART_SetConfig(husart);

    /* In USART mode, the following bits must be kept cleared:
        - LINEN bit in the USART_CR2 register
        - HDSEL, SCEN and IREN bits in the USART_CR3 register */
    CLEAR_BIT(husart->Instance->CR2, USART_CR2_LINEN);
    CLEAR_BIT(husart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN | USART_CR3_EIE));

    SET_BIT(husart->Instance->CR1, USART_CR1_TE);

    /* Enable the Peripheral */
    __HAL_USART_ENABLE(husart);

    /* Initialize the USART state */
    husart->ErrorCode = HAL_USART_ERROR_NONE;
    husart->State = HAL_USART_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  DeInitializes the USART peripheral.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_DeInit(USART_HandleTypeDef *husart)
{
    /* Check the USART handle allocation */
    if (husart == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_USART_INSTANCE(husart->Instance));

    husart->State = HAL_USART_STATE_BUSY;

    /* DeInit the low level hardware */
    HAL_USART_MspDeInit(husart);

    husart->ErrorCode = HAL_USART_ERROR_NONE;
    husart->State = HAL_USART_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(husart);

    return HAL_OK;
}

/**
 * @brief  USART MSP Init.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
__weak void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_MspInit could be implemented in the user file
     */
}

/**
 * @brief  USART MSP DeInit.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
__weak void HAL_USART_MspDeInit(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_MspDeInit could be implemented in the user file
     */
}

/**
 * @}
 */

/** @defgroup USART_Exported_Functions_Group2 IO operation functions
  *  @brief   USART Transmit and Receive functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the USART synchronous
    data transfers.

  [..]
    The USART supports master mode only: it cannot receive or send data related to an input
    clock (SCLK is always an output).

    (#) There are two modes of transfer:
        (++) Blocking mode: The communication is performed in polling mode.
             The HAL status of all data processing is returned by the same function
             after finishing transfer.
        (++) No-Blocking mode: The communication is performed using Interrupts
             or DMA, These API's return the HAL status.
             The end of the data processing will be indicated through the
             dedicated USART IRQ when using Interrupt mode or the DMA IRQ when
             using DMA mode.
             The HAL_USART_TxCpltCallback(), HAL_USART_RxCpltCallback() and HAL_USART_TxRxCpltCallback()
             user callbacks
             will be executed respectively at the end of the transmit or Receive process
             The HAL_USART_ErrorCallback() user callback will be executed when a communication
             error is detected

    (#) Blocking mode APIs are :
        (++) HAL_USART_Transmit() in simplex mode
        (++) HAL_USART_Receive() in full duplex receive only
        (++) HAL_USART_TransmitReceive() in full duplex mode

    (#) Non Blocking mode APIs with Interrupt are :
        (++) HAL_USART_Transmit_IT()in simplex mode
        (++) HAL_USART_Receive_IT() in full duplex receive only
        (++) HAL_USART_TransmitReceive_IT() in full duplex mode
        (++) HAL_USART_IRQHandler()

    (#) Non Blocking mode functions with DMA are :
        (++) HAL_USART_Transmit_DMA()in simplex mode
        (++) HAL_USART_Receive_DMA() in full duplex receive only
        (++) HAL_USART_TransmitReceive_DMA() in full duplex mode
        (++) HAL_USART_DMAPause()
        (++) HAL_USART_DMAResume()
        (++) HAL_USART_DMAStop()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_USART_TxCpltCallback()
        (++) HAL_USART_RxCpltCallback()
        (++) HAL_USART_ErrorCallback()
        (++) HAL_USART_TxRxCpltCallback()

@endverbatim
  * @{
  */

/**
 * @brief  Simplex Send an amount of data in blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data buffer
 * @param  Size: Amount of data to be sent
 * @param  Timeout: Timeout duration
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Transmit(USART_HandleTypeDef *husart, uint8_t *pTxData, uint16_t Size, uint32_t Timeout)
{
    uint16_t *tmp;
    uint32_t tickstart = 0U;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        /* Process Locked */
        __HAL_LOCK(husart);

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_TX;

        /* Init tickstart for timeout managment */
        tickstart = HAL_GetTick();

        husart->TxXferSize = Size;
        husart->TxXferCount = Size;
        while (husart->TxXferCount > 0U)
        {
            husart->TxXferCount--;
            if (husart->Init.WordLength == USART_WORDLENGTH_9B)
            {
                /* Wait for TC flag in order to write data in DR */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                tmp = (uint16_t *)pTxData;
                WRITE_REG(husart->Instance->DR, (*tmp & (uint16_t)0x01FF));
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    pTxData += 2U;
                }
                else
                {
                    pTxData += 1U;
                }
            }
            else
            {
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                WRITE_REG(husart->Instance->DR, (*pTxData++ & (uint8_t)0xFF));
            }
        }

        if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)
        {
            return HAL_TIMEOUT;
        }

        husart->State = HAL_USART_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Receive an amount of data in blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pRxData: Pointer to data buffer
 * @param  Size: Amount of data to be received
 * @param  Timeout: Timeout duration
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Receive(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    uint16_t *tmp;
    uint32_t tickstart = 0U;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pRxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_RX;

        /* Init tickstart for timeout managment */
        tickstart = HAL_GetTick();

        husart->RxXferSize = Size;
        husart->RxXferCount = Size;
        /* Check the remain data to be received */
        while (husart->RxXferCount > 0U)
        {
            husart->RxXferCount--;
            if (husart->Init.WordLength == USART_WORDLENGTH_9B)
            {
                /* Wait until TXE flag is set to send dummy byte in order to generate the clock for the slave to send data */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                /* Wait for RXNE Flag */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                tmp = (uint16_t *)pRxData;
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x01FF);
                    pRxData += 2U;
                }
                else
                {
                    *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x00FF);
                    pRxData += 1U;
                }
            }
            else
            {
                /* Wait until TXE flag is set to send dummy byte in order to generate the clock for the slave to send data */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                /* Wait until RXNE flag is set to receive the byte */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    /* Receive data */
                    *pRxData++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x00FF);
                }
                else
                {
                    /* Receive data */
                    *pRxData++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x007F);
                }
            }
        }

        husart->State = HAL_USART_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Send receive an amount of data in full-duplex mode (blocking mode).
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data transmitted buffer
 * @param  pRxData: Pointer to data received buffer
 * @param  Size: Amount of data to be sent
 * @param  Timeout: Timeout duration
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_TransmitReceive(USART_HandleTypeDef *husart, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    uint16_t *tmp;
    uint32_t tickstart = 0U;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_RX;

        /* Init tickstart for timeout managment */
        tickstart = HAL_GetTick();

        husart->RxXferSize = Size;
        husart->TxXferSize = Size;
        husart->TxXferCount = Size;
        husart->RxXferCount = Size;

        /* Check the remain data to be received */
        while (husart->TxXferCount > 0U)
        {
            husart->TxXferCount--;
            husart->RxXferCount--;
            if (husart->Init.WordLength == USART_WORDLENGTH_9B)
            {
                /* Wait for TC flag in order to write data in DR */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                tmp = (uint16_t *)pTxData;
                WRITE_REG(husart->Instance->DR, (*tmp & (uint16_t)0x01FF));
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    pTxData += 2U;
                }
                else
                {
                    pTxData += 1U;
                }

                /* Wait for RXNE Flag */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                tmp = (uint16_t *)pRxData;
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x01FF);
                    pRxData += 2U;
                }
                else
                {
                    *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x00FF);
                    pRxData += 1U;
                }
            }
            else
            {
                /* Wait for TC flag in order to write data in DR */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                WRITE_REG(husart->Instance->DR, (*pTxData++ & (uint8_t)0x00FF));

                /* Wait for RXNE Flag */
                if (USART_WaitOnFlagUntilTimeout(husart, USART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
                {
                    return HAL_TIMEOUT;
                }
                if (husart->Init.Parity == USART_PARITY_NONE)
                {
                    /* Receive data */
                    *pRxData++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x00FF);
                }
                else
                {
                    /* Receive data */
                    *pRxData++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x007F);
                }
            }
        }

        husart->State = HAL_USART_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Simplex Send an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data buffer
 * @param  Size: Amount of data to be sent
 * @retval HAL status
 * @note   The USART errors are not managed to avoid the overrun error.
 */
HAL_StatusTypeDef HAL_USART_Transmit_IT(USART_HandleTypeDef *husart, uint8_t *pTxData, uint16_t Size)
{
    /* Check that a Tx process is not already ongoing */
    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pTxBuffPtr = pTxData;
        husart->TxXferSize = Size;
        husart->TxXferCount = Size;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_TX;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        /* Enable the USART Transmit Data Register Empty Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_TXE);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Simplex Receive an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                the configuration information for the specified USART module.
 * @param  pRxData: Pointer to data buffer
 * @param  Size: Amount of data to be received
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Receive_IT(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size)
{
    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pRxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pRxBuffPtr = pRxData;
        husart->RxXferSize = Size;
        husart->RxXferCount = Size;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_RX;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        /* Enable the USART Data Register not empty Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_RXNE);

        /* Enable the USART Parity Error Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_PE);

        /* Enable the USART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_USART_CR3_ENABLE_IT(husart, USART_IT_ERR);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Send receive an amount of data in full-duplex mode (non-blocking).
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data transmitted buffer
 * @param  pRxData: Pointer to data received buffer
 * @param  Size: Amount of data to be received
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_TransmitReceive_IT(USART_HandleTypeDef *husart, uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize, uint16_t TxSize)
{
    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (pRxData == NULL) || (RxSize == 0U) || (TxSize == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pRxBuffPtr = pRxData;
        husart->RxXferSize = RxSize;
        husart->RxXferCount = RxSize;
        husart->pTxBuffPtr = pTxData;
        husart->TxXferSize = TxSize;
        husart->TxXferCount = TxSize;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_TX_RX;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        /* Enable the USART Data Register not empty Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_RXNE);

        /* Enable the USART Parity Error Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_PE);

        /* Enable the USART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_USART_CR3_ENABLE_IT(husart, USART_IT_ERR);

        /* Enable the USART Transmit Data Register Empty Interrupt */
        __HAL_USART_ENABLE_IT(husart, USART_IT_TXE);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Simplex Send an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data buffer
 * @param  Size: Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Transmit_DMA(USART_HandleTypeDef *husart, uint8_t *pTxData, uint16_t Size)
{
    uint32_t *tmp;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pTxBuffPtr = pTxData;
        husart->TxXferSize = Size;
        husart->TxXferCount = Size;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_TX;

        husart->hdmatx->Parent = husart;

        /* Set the USART DMA transfer complete callback */
        husart->hdmatx->XferCpltCallback = USART_DMATransmitCplt;

        /* Set the DMA error callback */
        husart->hdmatx->XferErrorCallback = USART_DMAError;

        /* Set the DMA abort callback */
        husart->hdmatx->XferAbortCallback = NULL;

        /* Enable the USART transmit DMA channel */
        tmp = (uint32_t *)&pTxData;

        HAL_DMA_USART_Start_IT(husart->hdmatx, *(uint32_t *)tmp, (uint32_t)&husart->Instance->DR, Size);
        
        /* Process Unlocked */
        __HAL_UNLOCK(husart);
        /* Enable the DMA transfer for transmit request by setting the DMAT bit
        in the USART CR3 register */
        HAL_USART_DMA_Tx_Enable(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Receive an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pRxData: Pointer to data buffer
 * @param  Size: Amount of data to be received
 * @retval HAL status
 * @note   The USART DMA transmit channel must be configured in order to generate the clock for the slave.
 * @note   When the USART parity is enabled (PCE = 1) the data received contain the parity bit.
 */
HAL_StatusTypeDef HAL_USART_Receive_DMA(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size)
{
    uint32_t *tmp;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pRxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pRxBuffPtr = pRxData;
        husart->RxXferSize = Size;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_RX;

        husart->hdmarx->Parent = husart;

        /* Set the USART DMA Rx transfer complete callback */
        husart->hdmarx->XferCpltCallback = USART_DMAReceiveCplt;

        /* Set the USART DMA Rx transfer error callback */
        husart->hdmarx->XferErrorCallback = USART_DMAError;

        /* Set the DMA abort callback */
        husart->hdmarx->XferAbortCallback = NULL;

        /* Set the USART Tx DMA transfer complete callback as NULL because the communication closing
        is performed in DMA reception complete callback  */
        husart->hdmatx->XferCpltCallback = NULL;

        /* Set the DMA error callback */
        husart->hdmatx->XferErrorCallback = USART_DMAError;

        /* Set the DMA AbortCpltCallback */
        husart->hdmatx->XferAbortCallback = NULL;

        tmp = (uint32_t *)&pRxData;

        HAL_DMA_Start_IT(husart->hdmarx, (uint32_t)&husart->Instance->DR, *(uint32_t *)tmp, Size);
        /* Enable the USART transmit DMA channel: the transmit channel is used in order
        to generate in the non-blocking mode the clock to the slave device,
        this mode isn't a simplex receive mode but a full-duplex receive one */
        /* Clear the Overrun flag just before enabling the DMA Rx request: mandatory for the second transfer */
        __HAL_USART_CLEAR_OREFLAG(husart);

        /* Process Unlocked */
        __HAL_UNLOCK(husart);
        /* Enable the DMA transfer for the receiver request by setting the DMAR bit
        in the USART CR3 register */
        HAL_USART_DMA_Rx_Enable(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Transmit Receive an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  pTxData: Pointer to data transmitted buffer
 * @param  pRxData: Pointer to data received buffer
 * @param  Size: Amount of data to be received
 * @note   When the USART parity is enabled (PCE = 1) the data received contain the parity bit.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_TransmitReceive_DMA(USART_HandleTypeDef *husart, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    uint32_t *tmp;

    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(husart);

        husart->pRxBuffPtr = pRxData;
        husart->RxXferSize = Size;
        husart->pTxBuffPtr = pTxData;
        husart->TxXferSize = Size;

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_BUSY_TX_RX;

        /* Set the USART DMA Rx transfer complete callback */
        husart->hdmarx->XferCpltCallback = USART_DMAReceiveCplt;

        /* Set the USART DMA Tx transfer complete callback */
        husart->hdmatx->XferCpltCallback = USART_DMATransmitCplt;

        /* Set the USART DMA Tx transfer error callback */
        husart->hdmatx->XferErrorCallback = USART_DMAError;

        /* Set the USART DMA Rx transfer error callback */
        husart->hdmarx->XferErrorCallback = USART_DMAError;

        /* Set the DMA abort callback */
        husart->hdmarx->XferAbortCallback = NULL;

        /* Enable the USART receive DMA channel */
        tmp = (uint32_t *)&pRxData;
        HAL_DMA_Start_IT(husart->hdmarx, (uint32_t)&husart->Instance->DR, *(uint32_t *)tmp, Size);
        /* Enable the USART transmit DMA channel */
        tmp = (uint32_t *)&pTxData;
        HAL_DMA_Start_IT(husart->hdmatx, *(uint32_t *)tmp, (uint32_t)&husart->Instance->DR, Size);

        /* Clear the TC flag in the SR register by writing 0 to it */
        __HAL_USART_CLEAR_FLAG(husart, USART_FLAG_TC);

        /* Clear the Overrun flag: mandatory for the second transfer in circular mode */
        __HAL_USART_CLEAR_OREFLAG(husart);

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        /* Enable the USART Parity Error Interrupt */
        SET_BIT(husart->Instance->CR1, USART_CR1_PEIE);

        /* Enable the USART Error Interrupt: (Frame error, noise error, overrun error) */
        SET_BIT(husart->Instance->CR3, USART_CR3_EIE);

        /* Enable the DMA transfer for the receiver request by setting the DMAR bit
           in the USART CR3 register */
        SET_BIT(husart->Instance->CR3, USART_CR3_DMAR);

        /* Enable the DMA transfer for transmit request by setting the DMAT bit
           in the USART CR3 register */
        SET_BIT(husart->Instance->CR3, USART_CR3_DMAT);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief Pauses the DMA Transfer.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_DMAPause(USART_HandleTypeDef *husart)
{
    /* Process Locked */
    __HAL_LOCK(husart);

    /* Disable the USART DMA Tx request */
    CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAT);

    /* Process Unlocked */
    __HAL_UNLOCK(husart);

    return HAL_OK;
}

/**
 * @brief Resumes the DMA Transfer.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_DMA_Tx_Enable(USART_HandleTypeDef *husart)
{
    /* Process Locked */
    __HAL_LOCK(husart);

    /* Enable the USART DMA Tx request */
    SET_BIT(husart->Instance->CR3, USART_CR3_DMAT);

    /* Process Unlocked */
    __HAL_UNLOCK(husart);

    return HAL_OK;
}

/**
 * @brief Resumes the DMA received.
 * @param  husart:
 *
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_DMA_Rx_Enable(USART_HandleTypeDef *husart)
{
    /* Process Locked */
    __HAL_LOCK(husart);

    /* Enable the USART DMA rx request */
    SET_BIT(husart->Instance->CR3, USART_CR3_DMAR);

    /* Process Unlocked */
    __HAL_UNLOCK(husart);

    return HAL_OK;
}

/**
 * @brief Stops the DMA Transfer.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_DMAStop(USART_HandleTypeDef *husart)
{
    uint32_t dmarequest = 0x00U;
    /* The Lock is not implemented on this API to allow the user application
       to call the HAL USART API under callbacks HAL_USART_TxCpltCallback() / HAL_USART_RxCpltCallback():
       when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
       and the correspond call back is executed HAL_USART_TxCpltCallback() / HAL_USART_RxCpltCallback()
       */

    /* Stop USART DMA Tx request if ongoing */
    dmarequest = HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAT);
    if ((husart->State == HAL_USART_STATE_BUSY_TX) && dmarequest)
    {
        USART_EndTxTransfer(husart);

        /* Abort the USART DMA Tx channel */
        if (husart->hdmatx != NULL)
        {
            HAL_DMA_Abort(husart->hdmatx);
        }

        /* Disable the USART Tx DMA request */
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAT);
    }

    /* Stop USART DMA Rx request if ongoing */
    dmarequest = HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR);
    if ((husart->State == HAL_USART_STATE_BUSY_RX) && dmarequest)
    {
        USART_EndRxTransfer(husart);

        /* Abort the USART DMA Rx channel */
        if (husart->hdmarx != NULL)
        {
            HAL_DMA_Abort(husart->hdmarx);
        }

        /* Disable the USART Rx DMA request */
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAR);
    }

    return HAL_OK;
}

/**
 * @brief  Abort ongoing transfer (blocking mode).
 * @param  husart USART handle.
 * @note   This procedure could be used for aborting any ongoing transfer (either Tx or Rx,
 *         as described by TransferType parameter) started in Interrupt or DMA mode.
 *         This procedure performs following operations :
 *           - Disable PPP Interrupts (depending of transfer direction)
 *           - Disable the DMA transfer in the peripheral register (if enabled)
 *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
 *           - Set handle State to READY
 * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Abort(USART_HandleTypeDef *husart)
{
    /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(husart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));
    CLEAR_BIT(husart->Instance->CR3, USART_CR3_EIE);

    /* Disable the USART DMA Tx request if enabled */
    if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAT))
    {
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAT);

        /* Abort the USART DMA Tx channel : use blocking DMA Abort API (no callback) */
        if (husart->hdmatx != NULL)
        {
            /* Set the USART DMA Abort callback to Null.
               No call back execution at end of DMA abort procedure */
            husart->hdmatx->XferAbortCallback = NULL;

            HAL_DMA_Abort(husart->hdmatx);
        }
    }

    /* Disable the USART DMA Rx request if enabled */
    if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR))
    {
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAR);

        /* Abort the USART DMA Rx channel : use blocking DMA Abort API (no callback) */
        if (husart->hdmarx != NULL)
        {
            /* Set the USART DMA Abort callback to Null.
               No call back execution at end of DMA abort procedure */
            husart->hdmarx->XferAbortCallback = NULL;

            HAL_DMA_Abort(husart->hdmarx);
        }
    }

    /* Reset Tx and Rx transfer counters */
    husart->TxXferCount = 0x00U;
    husart->RxXferCount = 0x00U;

    /* Restore husart->State to Ready */
    husart->State = HAL_USART_STATE_READY;

    /* Reset Handle ErrorCode to No Error */
    husart->ErrorCode = HAL_USART_ERROR_NONE;

    return HAL_OK;
}

/**
 * @brief  Abort ongoing transfer (Interrupt mode).
 * @param  husart USART handle.
 * @note   This procedure could be used for aborting any ongoing transfer (either Tx or Rx,
 *         as described by TransferType parameter) started in Interrupt or DMA mode.
 *         This procedure performs following operations :
 *           - Disable PPP Interrupts (depending of transfer direction)
 *           - Disable the DMA transfer in the peripheral register (if enabled)
 *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
 *           - Set handle State to READY
 *           - At abort completion, call user abort complete callback
 * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
 *         considered as completed only when user abort complete callback is executed (not when exiting function).
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_USART_Abort_IT(USART_HandleTypeDef *husart)
{
    uint32_t AbortCplt = 0x01U;

    /* Disable TXEIE, TCIE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
    CLEAR_BIT(husart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));
    CLEAR_BIT(husart->Instance->CR3, USART_CR3_EIE);

    /* If DMA Tx and/or DMA Rx Handles are associated to USART Handle, DMA Abort complete callbacks should be initialised
       before any call to DMA Abort functions */
    /* DMA Tx Handle is valid */
    if (husart->hdmatx != NULL)
    {
        /* Set DMA Abort Complete callback if USART DMA Tx request if enabled.
           Otherwise, set it to NULL */
        if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAT))
        {
            husart->hdmatx->XferAbortCallback = NULL;
        }
        else
        {
            husart->hdmatx->XferAbortCallback = NULL;
        }
    }
    /* DMA Rx Handle is valid */
    if (husart->hdmarx != NULL)
    {
        /* Set DMA Abort Complete callback if USART DMA Rx request if enabled.
           Otherwise, set it to NULL */
        if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR))
        {
            husart->hdmarx->XferAbortCallback = NULL;
        }
        else
        {
            husart->hdmarx->XferAbortCallback = NULL;
        }
    }

    /* Disable the USART DMA Tx request if enabled */
    if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAT))
    {
        /* Disable DMA Tx at USART level */
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAT);

        /* Abort the USART DMA Tx channel : use non blocking DMA Abort API (callback) */
        if (husart->hdmatx != NULL)
        {
            /* USART Tx DMA Abort callback has already been initialised :
               will lead to call HAL_USART_AbortCpltCallback() at end of DMA abort procedure */

            /* Abort DMA TX */
            if (HAL_DMA_Abort_IT(husart->hdmatx) != HAL_OK)
            {
                husart->hdmatx->XferAbortCallback = NULL;
            }
            else
            {
                AbortCplt = 0x00U;
            }
        }
    }

    /* Disable the USART DMA Rx request if enabled */
    if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR))
    {
        CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAR);

        /* Abort the USART DMA Rx channel : use non blocking DMA Abort API (callback) */
        if (husart->hdmarx != NULL)
        {
            /* USART Rx DMA Abort callback has already been initialised :
               will lead to call HAL_USART_AbortCpltCallback() at end of DMA abort procedure */

            /* Abort DMA RX */
            if (HAL_DMA_Abort_IT(husart->hdmarx) != HAL_OK)
            {
                husart->hdmarx->XferAbortCallback = NULL;
                AbortCplt = 0x01U;
            }
            else
            {
                AbortCplt = 0x00U;
            }
        }
    }

    /* if no DMA abort complete callback execution is required => call user Abort Complete callback */
    if (AbortCplt == 0x01U)
    {
        /* Reset Tx and Rx transfer counters */
        husart->TxXferCount = 0x00U;
        husart->RxXferCount = 0x00U;

        /* Reset errorCode */
        husart->ErrorCode = HAL_USART_ERROR_NONE;

        /* Restore husart->State to Ready */
        husart->State = HAL_USART_STATE_READY;

        /* As no DMA to be aborted, call directly user Abort complete callback */
        HAL_USART_AbortCpltCallback(husart);
    }

    return HAL_OK;
}

/**
 * @brief  This function handles USART interrupt request.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                the configuration information for the specified USART module.
 * @retval None
 */
void HAL_USART_IRQHandler(USART_HandleTypeDef *husart)
{
    uint32_t isrflags = READ_REG(husart->Instance->SR);
    uint32_t cr1its = READ_REG(husart->Instance->CR1);
    uint32_t cr2its = READ_REG(husart->Instance->CR2);
    uint32_t cr3its = READ_REG(husart->Instance->CR3);
    uint32_t errorflags = 0x00U;
    uint32_t dmarequest = 0x00U;
    uint32_t data;
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if (errorflags == RESET)
    {
        /* USART in mode Receiver -------------------------------------------------*/
        if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
        {
            if(husart->State == HAL_USART_STATE_BUSY_RX)
            {
                USART_Receive_IT(husart);
            }
            else
            {
                USART_TransmitReceive_IT(husart);
            }
            return;
        }
    }
    /* If some errors occur */
    if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
    {
        data = husart->Instance->SR;    /* clear errors */
        data = husart->Instance->DR;    /* clear errors */
        UNUSED(data);
        /* USART parity error interrupt occurred ----------------------------------*/
        if(((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
        {
            husart->ErrorCode |= HAL_USART_ERROR_PE;
        }

        /* USART noise error interrupt occurred --------------------------------*/
        if(((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
        {
            husart->ErrorCode |= HAL_USART_ERROR_NE;
        }

        /* USART frame error interrupt occurred --------------------------------*/
        if(((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
        {
            husart->ErrorCode |= HAL_USART_ERROR_FE;
        }

        /* USART Over-Run interrupt occurred -----------------------------------*/
        if(((isrflags & USART_SR_ORE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
        {
            husart->ErrorCode |= HAL_USART_ERROR_ORE;
        }

        if (husart->ErrorCode != HAL_USART_ERROR_NONE)
        {
            /* USART in mode Receiver -----------------------------------------------*/
            if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
            {
                if (husart->State == HAL_USART_STATE_BUSY_RX)
                {
                    USART_Receive_IT(husart);
                }
                else
                {
                    USART_TransmitReceive_IT(husart);
                }
            }
            /* If Overrun error occurs, or if any error occurs in DMA mode reception,
            consider error as blocking */
            dmarequest = HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR);
            if (((husart->ErrorCode & HAL_USART_ERROR_ORE) != RESET) || dmarequest)
            {
                /* Set the USART state ready to be able to start again the process,
                Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
                USART_EndRxTransfer(husart);

                /* Disable the USART DMA Rx request if enabled */
                if (HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR))
                {
                    CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAR);

                    /* Abort the USART DMA Rx channel */
                    if (husart->hdmarx != NULL)
                    {
                        /* Set the USART DMA Abort callback :
                        will lead to call HAL_USART_ErrorCallback() at end of DMA abort procedure */

                        husart->hdmarx->XferAbortCallback = NULL;
                        if (HAL_DMA_Abort_IT(husart->hdmarx) != HAL_OK)
                        {
                            /* Call Directly XferAbortCallback function in case of error */
                        }
                    }
                    else
                    {
                        /* Call user error callback */
                        HAL_USART_ErrorCallback(husart);
                    }
                }
                else
                {
                    /* Call user error callback */
                    HAL_USART_ErrorCallback(husart);
                }
            }
            else
            {
                /* Call user error callback */
                HAL_USART_ErrorCallback(husart);
                husart->ErrorCode = HAL_USART_ERROR_NONE;
            }
        }
        return;
    }

    /* USART in mode Transmitter -----------------------------------------------*/
    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
        if (husart->State == HAL_USART_STATE_BUSY_TX)
        {
            USART_Transmit_IT(husart);
        }
        else
        {
            USART_TransmitReceive_IT(husart);
        }
        return;
    }

    /* USART in mode Transmitter (transmission end) ----------------------------*/
    if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
    {
        USART_EndTransmit_IT(husart);
        return;
    }
}

void HAL_LIN_IRQHandler(USART_HandleTypeDef *husart)
{
    uint32_t isrflags = READ_REG(husart->Instance->SR);
    uint32_t cr1its = READ_REG(husart->Instance->CR1);
    uint32_t cr2its = READ_REG(husart->Instance->CR2);
    uint32_t cr3its = READ_REG(husart->Instance->CR3);

    CLEAR_BIT(husart->Instance->SR, (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));

    if ((isrflags & USART_SR_LBD) != RESET)
    {
        CLEAR_BIT(husart->Instance->SR, USART_SR_LBD);
        CLEAR_BIT(husart->Instance->SR, USART_SR_RXNE);
        SET_BIT(husart->Instance->CR1, USART_CR1_RXNEIE);
        return;
    }
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET) && ((cr2its & USART_CR2_LINEN) != RESET))
    {
        LIN_Slave_Receive(husart);
        return;
    }
}

__weak void HAL_LIN_RxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_RxCpltCallback could be implemented in the user file
     */
}
/**
 * @brief  Tx Transfer completed callbacks.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
__weak void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_TxCpltCallback could be implemented in the user file
     */
}


/**
 * @brief  Rx Transfer completed callbacks.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
__weak void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_RxCpltCallback could be implemented in the user file
     */
}

/**
  * @brief  Tx/Rx Transfers completed callback for the non-blocking process.
  * @param  husart: pointer to a USART_HandleTypeDef structure that contains
  *                 the configuration information for the specified USART module.
  * @retval None
  */
__weak void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_TxRxCpltCallback could be implemented in the user file
     */
}

/**
 * @brief  USART error callbacks.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
__weak void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_USART_ErrorCallback could be implemented in the user file
     */
}

/**
 * @brief  USART Abort Complete callback.
 * @param  husart USART handle.
 * @retval None
 */
__weak void HAL_USART_AbortCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_USART_AbortCpltCallback can be implemented in the user file.
     */
}

/**
 * @}
 */

/** @defgroup USART_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   USART State and Errors functions
  *
@verbatim
  ==============================================================================
                  ##### Peripheral State and Errors functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to return the State of
    USART communication
    process, return Peripheral Errors occurred during communication process
     (+) HAL_USART_GetState() API can be helpful to check in run-time the state
         of the USART peripheral.
     (+) HAL_USART_GetError() check in run-time errors that could be occurred during
         communication.
@endverbatim
  * @{
  */

/**
 * @brief  Returns the USART state.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL state
 */
HAL_USART_StateTypeDef HAL_USART_GetState(USART_HandleTypeDef *husart)
{
    return husart->State;
}

/**
 * @brief  Return the USART error code
 * @param  husart : pointer to a USART_HandleTypeDef structure that contains
 *              the configuration information for the specified USART.
 * @retval USART Error Code
 */
uint32_t HAL_USART_GetError(USART_HandleTypeDef *husart)
{
    return husart->ErrorCode;
}

/**
 * @}
 */

/**
 * @}
 */

/** @defgroup USART_Private_Functions   USART Private Functions
 *  @brief   USART Private functions
 * @{
 */
/**
 * @brief  DMA USART transmit process complete callback.
 * @param  hdma: DMA handle
 * @param  hdma: Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
void USART_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
    USART_HandleTypeDef* husart = ( USART_HandleTypeDef* )hdma->Parent;

    /* DMA Normal mode */
    if(HAL_IS_BIT_CLR(hdma->Instance->CONTROL, DMA_CHX_CONTROL_CONTI))
    {
        husart->TxXferCount = 0x00U;
        if(husart->State == HAL_USART_STATE_BUSY_TX)
        {
            husart->State = HAL_USART_STATE_READY;
            CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAT);
            HAL_USART_TxCpltCallback(husart);            
        }
    }
    else
    /* DMA Circular mode */        
    {
        if(husart->State == HAL_USART_STATE_BUSY_TX)
        {
            HAL_USART_TxCpltCallback(husart);
        }
        
    }        
}

/**
 * @brief  DMA USART receive process complete callback.
 * @param  hdma: DMA handle
 * @retval None
 */
void USART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
    USART_HandleTypeDef *husart = (USART_HandleTypeDef *)hdma->Parent;
    /* DMA Normal mode */
    if(HAL_IS_BIT_CLR(hdma->Instance->CONTROL, DMA_CHX_CONTROL_CONTI))
    {
        husart->RxXferCount = 0x00U;
        if(husart->State == HAL_USART_STATE_BUSY_RX)
        {
            husart->State = HAL_USART_STATE_READY;
            CLEAR_BIT(husart->Instance->CR3, USART_CR3_DMAR);
            HAL_USART_RxCpltCallback(husart);
        }
    }
    /* DMA Circular mode */
    else
    {
        if(husart->State == HAL_USART_STATE_BUSY_RX)
        {
            HAL_USART_RxCpltCallback(husart);
        }        
    }

}

/**
 * @brief  DMA USART communication error callback.
 * @param  hdma: DMA handle
 * @retval None
 */
void USART_DMAError(DMA_HandleTypeDef *hdma)
{
    uint32_t dmarequest = 0x00U;
    USART_HandleTypeDef *husart = (USART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
    husart->RxXferCount = 0x00U;
    husart->TxXferCount = 0x00U;

    /* Stop USART DMA Tx request if ongoing */
    dmarequest = HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAT);
    if ((husart->State == HAL_USART_STATE_BUSY_TX) && dmarequest)
    {
        USART_EndTxTransfer(husart);
    }

    /* Stop USART DMA Rx request if ongoing */
    dmarequest = HAL_IS_BIT_SET(husart->Instance->CR3, USART_CR3_DMAR);
    if ((husart->State == HAL_USART_STATE_BUSY_RX) && dmarequest)
    {
        USART_EndRxTransfer(husart);
    }

    husart->ErrorCode |= HAL_USART_ERROR_DMA;
    husart->State = HAL_USART_STATE_READY;

    HAL_USART_ErrorCallback(husart);
}

/**
 * @brief  This function handles USART Communication Timeout.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @param  Flag: specifies the USART flag to check.
 * @param  Status: The new Flag status (SET or RESET).
 * @param  Tickstart: Tick start value.
 * @param  Timeout: Timeout duration.
 * @retval HAL status
 */
static HAL_StatusTypeDef USART_WaitOnFlagUntilTimeout(USART_HandleTypeDef *husart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)
{
    /* Wait until flag is set */
    while ((__HAL_USART_GET_FLAG(husart, Flag) ? SET : RESET) == Status)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if ((Timeout == 0U) || ((HAL_GetTick() - Tickstart) > Timeout))
            {
                /* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
                __HAL_USART_DISABLE_IT(husart, USART_IT_TXE);
                __HAL_USART_DISABLE_IT(husart, USART_IT_RXNE);
                __HAL_USART_DISABLE_IT(husart, USART_IT_PE);
                __HAL_USART_CR3_DISABLE_IT(husart, USART_IT_ERR);

                husart->State = HAL_USART_STATE_READY;

                /* Process Unlocked */
                __HAL_UNLOCK(husart);

                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}

/**
 * @brief  End ongoing Tx transfer on USART peripheral (following error detection or Transmit completion).
 * @param  husart: USART handle.
 * @retval None
 */
static void USART_EndTxTransfer(USART_HandleTypeDef *husart)
{
    /* Disable TXEIE and TCIE interrupts */
    CLEAR_BIT(husart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

    /* At end of Tx process, restore husart->State to Ready */
    husart->State = HAL_USART_STATE_READY;
}

/**
 * @brief  End ongoing Rx transfer on USART peripheral (following error detection or Reception completion).
 * @param  husart: USART handle.
 * @retval None
 */
static void USART_EndRxTransfer(USART_HandleTypeDef *husart)
{
    /* Disable RXNE, PE and ERR interrupts */
    CLEAR_BIT(husart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
    CLEAR_BIT(husart->Instance->CR3, USART_CR3_EIE);

    /* At end of Rx process, restore husart->State to Ready */
    husart->State = HAL_USART_STATE_READY;
}

/**
 * @brief  Simplex Send an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 * @note   The USART errors are not managed to avoid the overrun error.
 */
static HAL_StatusTypeDef USART_Transmit_IT(USART_HandleTypeDef *husart)
{
    uint16_t *tmp;

    if (husart->State == HAL_USART_STATE_BUSY_TX)
    {
        if (husart->Init.WordLength == USART_WORDLENGTH_9B)
        {
            tmp = (uint16_t *)husart->pTxBuffPtr;
            WRITE_REG(husart->Instance->DR, (uint16_t)(*tmp & (uint16_t)0x01FF));
            if (husart->Init.Parity == USART_PARITY_NONE)
            {
                husart->pTxBuffPtr += 2U;
            }
            else
            {
                husart->pTxBuffPtr += 1U;
            }
        }
        else
        {
            WRITE_REG(husart->Instance->DR, (uint8_t)(*husart->pTxBuffPtr++ & (uint8_t)0x00FF));
        }

        husart->TxXferCount--;

        if (husart->TxXferCount == 0U)
        {
            /* Disable the USART Transmit data register empty Interrupt */
            __HAL_USART_DISABLE_IT(husart, USART_IT_TXE);

            /* Enable the USART Transmit Complete Interrupt */
            __HAL_USART_ENABLE_IT(husart, USART_IT_TC);
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Wraps up transmission in non blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                the configuration information for the specified USART module.
 * @retval HAL status
 */
static HAL_StatusTypeDef USART_EndTransmit_IT(USART_HandleTypeDef *husart)
{
    /* Disable the USART Transmit Complete Interrupt */
    __HAL_USART_DISABLE_IT(husart, USART_IT_TC);

    /* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_USART_DISABLE_IT(husart, USART_IT_ERR);

    husart->State = HAL_USART_STATE_READY;

    HAL_USART_TxCpltCallback(husart);

    return HAL_OK;
}

/**
 * @brief  Simplex Receive an amount of data in non-blocking mode.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
static HAL_StatusTypeDef USART_Receive_IT(USART_HandleTypeDef *husart)
{
    uint16_t *tmp;
    if (husart->State == HAL_USART_STATE_BUSY_RX)
    {
        if (husart->Init.WordLength == USART_WORDLENGTH_9B)
        {
            tmp = (uint16_t *)husart->pRxBuffPtr;
            if (husart->Init.Parity == USART_PARITY_NONE)
            {
                *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x01FF);
                husart->pRxBuffPtr += 2U;
            }
            else
            {
                *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x00FF);
                husart->pRxBuffPtr += 1U;
            }
        }
        else
        {
            if (husart->Init.Parity == USART_PARITY_NONE)
            {
                *husart->pRxBuffPtr++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x00FF);
            }
            else
            {
                *husart->pRxBuffPtr++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x007F);
            }
        }

        husart->RxXferCount--;

        if (husart->RxXferCount == 0U)
        {
            /* Disable the USART RXNE Interrupt */
            CLEAR_BIT(husart->Instance->CR1, USART_CR1_RXNEIE);

            /* Disable the USART Parity Error Interrupt */
            CLEAR_BIT(husart->Instance->CR1, USART_CR1_PEIE);

            /* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
            CLEAR_BIT(husart->Instance->CR3, USART_CR3_EIE);

            husart->State = HAL_USART_STATE_READY;

            HAL_USART_RxCpltCallback(husart);

            return HAL_OK;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
 * @brief  Full-Duplex Send receive an amount of data in full-duplex mode (non-blocking).
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval HAL status
 */
static HAL_StatusTypeDef USART_TransmitReceive_IT(USART_HandleTypeDef *husart)
{
    uint16_t *tmp;

    if (husart->State == HAL_USART_STATE_BUSY_TX_RX)
    {
        if (husart->TxXferCount != 0x00U)
        {
            if (__HAL_USART_GET_FLAG(husart, USART_FLAG_TXE) != RESET)
            {
                if (husart->Init.WordLength == USART_WORDLENGTH_9B)
                {
                    tmp = (uint16_t *)husart->pTxBuffPtr;
                    WRITE_REG(husart->Instance->DR, (uint16_t)(*tmp & (uint16_t)0x01FF));
                    if (husart->Init.Parity == USART_PARITY_NONE)
                    {
                        husart->pTxBuffPtr += 2U;
                    }
                    else
                    {
                        husart->pTxBuffPtr += 1U;
                    }
                }
                else
                {
                    WRITE_REG(husart->Instance->DR, (uint8_t)(*husart->pTxBuffPtr++ & (uint8_t)0x00FF));
                }

                husart->TxXferCount--;

                /* Check the latest data transmitted */
                if (husart->TxXferCount == 0U)
                {
                    CLEAR_BIT(husart->Instance->CR1, USART_CR1_TXEIE);
                }
            }
        }

        if (husart->RxXferCount != 0x00U)
        {
            if (__HAL_USART_GET_FLAG(husart, USART_FLAG_RXNE) != RESET)
            {
                if (husart->Init.WordLength == USART_WORDLENGTH_9B)
                {
                    tmp = (uint16_t *)husart->pRxBuffPtr;
                    if (husart->Init.Parity == USART_PARITY_NONE)
                    {
                        *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x01FF);
                        husart->pRxBuffPtr += 2U;
                    }
                    else
                    {
                        *tmp = (uint16_t)(husart->Instance->DR & (uint16_t)0x00FF);
                        husart->pRxBuffPtr += 1U;
                    }
                }
                else
                {
                    if (husart->Init.Parity == USART_PARITY_NONE)
                    {
                        *husart->pRxBuffPtr++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x00FF);
                    }
                    else
                    {
                        *husart->pRxBuffPtr++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x007F);
                    }
                }

                husart->RxXferCount--;
            }
        }

        /* Check the latest data received */
        if (husart->RxXferCount == 0U)
        {
            /* Disable the USART RXNE Interrupt */
            CLEAR_BIT(husart->Instance->CR1, USART_CR1_RXNEIE);

            /* Disable the USART Parity Error Interrupt */
            CLEAR_BIT(husart->Instance->CR1, USART_CR1_PEIE);

            /* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
            CLEAR_BIT(husart->Instance->CR3, USART_CR3_EIE);

            husart->State = HAL_USART_STATE_READY;

            HAL_USART_TxRxCpltCallback(husart);

            return HAL_OK;
        }

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

static HAL_StatusTypeDef LIN_Slave_Receive(USART_HandleTypeDef *husart)
{
    if (husart->State == HAL_USART_STATE_BUSY_RX)
    {
        *husart->pRxBuffPtr++ = (uint8_t)(husart->Instance->DR & (uint8_t)0x00FF);
        husart->RxXferCount--;
        if (husart->RxXferCount == 0U)
        {
            CLEAR_BIT(husart->Instance->CR1, USART_CR1_RXNEIE);
            CLEAR_BIT(husart->Instance->CR2, USART_CR2_LBDIE);
            husart->State = HAL_USART_STATE_READY;
            HAL_LIN_RxCpltCallback(husart);
        }
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

static uint8_t HAL_LIN_CalcParity(uint8_t ID)
{
    uint8_t p0, p1, parity;
    parity = ID;
    p0 = (((parity) ^ (parity >> 1) ^ (parity >> 2) ^ (parity >> 4)) & 0x01) << 6;
    p1 = ((~((parity >> 1) ^ (parity >> 3) ^ (parity >> 4) ^ (parity >> 5))) & 0x01) << 7;
    parity |= (p0 | p1);
    return parity;
}

/**
 * @brief  Configures the USART pferipheral.
 * @param  husart: pointer to a USART_HandleTypeDef structure that contains
 *                 the configuration information for the specified USART module.
 * @retval None
 */
static void USART_SetConfig(USART_HandleTypeDef *husart)
{
    uint32_t sysclk;
    uint16_t mantissa;
    uint8_t fraciton;
    /* Check the parameters */
    assert_param(IS_USART_POLARITY(husart->Init.CLKPolarity));
    assert_param(IS_USART_PHASE(husart->Init.CLKPhase));
    assert_param(IS_USART_LASTBIT(husart->Init.CLKLastBit));
    assert_param(IS_USART_BAUDRATE(husart->Init.BaudRate));
    assert_param(IS_USART_WORD_LENGTH(husart->Init.WordLength));
    assert_param(IS_USART_STOPBITS(husart->Init.StopBits));
    assert_param(IS_USART_PARITY(husart->Init.Parity));
    assert_param(IS_USART_MODE(husart->Init.Mode));

    /* The LBCL, CPOL and CPHA bits have to be selected when both the transmitter and the
        receiver are disabled (TE=RE=0) to ensure that the clock pulses function correctly. */
    CLEAR_BIT(husart->Instance->CR1, ((uint32_t)(USART_CR1_TE | USART_CR1_RE)));

    /*---------------------------- USART CR2 Configuration ---------------------*/
    /* Configure the USART Clock, CPOL, CPHA and LastBit -----------------------*/
    /* Set CPOL bit according to husart->Init.CLKPolarity value */
    /* Set CPHA bit according to husart->Init.CLKPhase value */
    /* Set LBCL bit according to husart->Init.CLKLastBit value */
    /* Set Stop Bits: Set STOP[13:12] bits according to husart->Init.StopBits value */
    /* Write to USART CR2 */
    MODIFY_REG(husart->Instance->CR2, \
               (uint32_t)(USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_STOP), \
               ((uint32_t)(husart->Init.CLKPolarity | husart->Init.CLKPhase | husart->Init.CLKLastBit | husart->Init.StopBits)));

    /*-------------------------- USART CR1 Configuration -----------------------*/
    /* Configure the USART Word Length, Parity and mode:
        Set the M bits according to husart->Init.WordLength value
        Set PCE and PS bits according to husart->Init.Parity value
        Set TE and RE bits according to husart->Init.Mode value */
    MODIFY_REG(husart->Instance->CR1, \
               (uint32_t)(USART_CR1_WD_LEN | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE), \
               (uint32_t)husart->Init.WordLength | husart->Init.Parity | husart->Init.Mode /*| USART_CR1_TCIE*/);

    /*-------------------------- USART CR3 Configuration -----------------------*/
    /* Clear CTSE and RTSE bits */
    CLEAR_BIT(husart->Instance->CR3, (uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE));

    /*-------------------------- USART BRR Configuration -----------------------*/
    if ((husart->Instance == USART1))
    {
        sysclk = HAL_RCMU_GetPCLK2Freq();
    }
    else
    {
        sysclk = HAL_RCMU_GetPCLK1Freq();
    }

    mantissa = (sysclk >> 4) / husart->Init.BaudRate;

    fraciton = (sysclk - ((mantissa << 4) * husart->Init.BaudRate)) / husart->Init.BaudRate;

    husart->Instance->BRR = (mantissa << USART_BRR_DIV_MANTISSA_Pos) | (fraciton << USART_BRR_DIV_FRACITON_Pos);
}
/**
 * @}
 */
HAL_StatusTypeDef HAL_LIN_SendHeaderBlock(USART_HandleTypeDef *husart, uint8_t ID)
{
    if (husart->State == HAL_USART_STATE_READY)
    {
        if (husart == NULL)
        {
            return HAL_ERROR;
        }
        HAL_LIN_SendBreak(husart);
        WRITE_REG(husart->Instance->DR, (uint8_t)(0x55));
        while((__HAL_USART_GET_FLAG(husart, USART_FLAG_TXE) ? SET : RESET) == RESET) {};
        WRITE_REG(husart->Instance->DR, HAL_LIN_CalcParity(ID));
        while((__HAL_USART_GET_FLAG(husart, USART_FLAG_TXE) ? SET : RESET) == RESET) {};
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }

}

HAL_StatusTypeDef HAL_LIN_SendBreak(USART_HandleTypeDef *husart)
{
    if(husart->State == HAL_USART_STATE_READY)
    {
        /* Send break characters */
        SET_BIT(husart->Instance->CR1, USART_CR1_SBK);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }




}

/**
 * @brief  Initializes the LIN mode according to the specified
 *         parameters in the UART_InitTypeDef and create the associated handle.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 * @param  BreakDetectLength Specifies the LIN break detection length.
 *         This parameter can be one of the following values:
 *            @arg UART_LINBREAKDETECTLENGTH_10B: 10-bit break detection
 *            @arg UART_LINBREAKDETECTLENGTH_11B: 11-bit break detection
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_LIN_Init(USART_HandleTypeDef *husart, uint32_t BreakDetectLength)
{
    if (husart->State == HAL_USART_STATE_READY)
    {
        /* Check the USART handle allocation */
        if ((husart == NULL) || (husart->Init.StopBits != USART_STOPBITS_1) || (husart->Init.WordLength != USART_WORDLENGTH_8B) || (husart->Init.Parity != USART_PARITY_NONE))
        {
            return HAL_ERROR;
        }
        /* Allocate lock resource and initialize it */
        husart->Lock = HAL_UNLOCKED;

        husart->State = HAL_USART_STATE_BUSY;

        /* Disable the peripheral */
        __HAL_USART_DISABLE(husart);

        /* Set the USART Communication parameters */
        USART_SetConfig(husart);

        /* In LIN mode, the following bits must be kept cleared:
         - CLKEN bits in the USART_CR2 register,
        - SCEN, HDSEL and IREN bits in the USART_CR3 register.*/
        CLEAR_BIT(husart->Instance->CR2, (USART_CR2_CLKEN));
        CLEAR_BIT(husart->Instance->CR3, (USART_CR3_HDSEL | USART_CR3_IREN | USART_CR3_SCEN));

        /* Enable the LIN mode by setting the LINEN bit in the CR2 register */
        SET_BIT(husart->Instance->CR2, USART_CR2_LINEN);

        /* Set the USART LIN Break detection length. */
        CLEAR_BIT(husart->Instance->CR2, USART_CR2_LBDL);
        CLEAR_BIT(husart->Instance->CR1, USART_CR1_RXNEIE);
        SET_BIT(husart->Instance->CR2, BreakDetectLength);

        /* Enable the peripheral */
        __HAL_USART_ENABLE(husart);

        /* Initialize the UART state*/
        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->State = HAL_USART_STATE_READY;
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef HAL_LIN_SendFrameBlock(USART_HandleTypeDef *husart, uint8_t *pData, uint16_t Size, uint8_t ID)
{
    uint8_t checksum = 0U, i;
    uint16_t sum = 0U;
    uint8_t *p;
    p = pData;
    if (husart->State == HAL_USART_STATE_READY)
    {
        if ((pData == NULL) || (Size > 9U))
        {
            return HAL_ERROR;
        }

        /* Process Locked */
        __HAL_LOCK(husart);

        husart->ErrorCode = HAL_USART_ERROR_NONE;
        husart->TxXferSize = Size;
        husart->TxXferCount = Size;
        if (husart->CRCType == Enhanced)
        {
            sum = HAL_LIN_CalcParity(ID);
        }
        for (i = 0; i < Size; i++)
        {
            sum += (*p++);
            if (sum > 0xFF)
            {
                sum -= 0xFF;
            }
        }
        checksum = (uint8_t)(~sum);
        HAL_LIN_SendHeaderBlock(husart,ID);
        husart->State = HAL_USART_STATE_BUSY_TX;
        while(husart->TxXferCount > 0U)
        {
            husart->TxXferCount--;
            while((__HAL_USART_GET_FLAG(husart, USART_FLAG_TXE) ? SET : RESET) == RESET) {};
            WRITE_REG(husart->Instance->DR, (*pData++ & (uint8_t)0xFF));
        }
        while((__HAL_USART_GET_FLAG(husart, USART_FLAG_TXE) ? SET : RESET) == RESET) {};

        WRITE_REG(husart->Instance->DR, checksum & (uint8_t)0xFF);

        while((__HAL_USART_GET_FLAG(husart, USART_FLAG_TC) ? SET : RESET) == RESET) {};

        husart->State = HAL_USART_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(husart);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

HAL_StatusTypeDef HAL_LIN_Slave_Receive(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size)
{
    if (husart->State == HAL_USART_STATE_READY)
    {
        __HAL_LOCK(husart);
        husart->RxXferSize = Size;
        husart->RxXferCount = Size;
        husart->pRxBuffPtr = pRxData;
        husart->State = HAL_USART_STATE_BUSY_RX;
        CLEAR_BIT(husart->Instance->SR, USART_SR_LBD);
        SET_BIT(husart->Instance->CR2, USART_CR2_LBDIE);
        __HAL_UNLOCK(husart);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}
#endif /* HAL_USART_MODULE_ENABLED */

/**
 * @}
 */

/**
 * @}
 */
