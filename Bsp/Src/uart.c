#include "slm32x030_hal.h"
#include "uart.h"

USART_HandleTypeDef  g_uart2_handle;
static volatile bool g_rx_en;
static uint8_t       g_rx_byte;
static uint8_t       g_tx_byte;
static uart_rx_cb_t  g_rx_cb;
static uart_tx_cb_t  g_tx_cb;

/* Weak HAL callbacks -> registered byte-level callbacks. */
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
    if (husart == &g_uart2_handle)
    {
        if (g_rx_en)
        {
            if (g_rx_cb)
                g_rx_cb(g_rx_byte);
            HAL_USART_Receive_IT(&g_uart2_handle, &g_rx_byte, 1); /* re-arm next byte */
        }
    }
}

void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
    if (husart == &g_uart2_handle)
    {
        if (g_tx_cb)
            g_tx_cb();
    }
}

void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart)
{
    if (husart == &g_uart2_handle)
    {
        __HAL_USART_CLEAR_OREFLAG(&g_uart2_handle);
        __HAL_USART_CLEAR_NEFLAG(&g_uart2_handle);
        __HAL_USART_CLEAR_FEFLAG(&g_uart2_handle);
        if (g_rx_en)
            HAL_USART_Receive_IT(&g_uart2_handle, &g_rx_byte, 1); /* re-arm after error */
    }
}

void uart2_init(uint32_t baud, uint8_t data_bits, uart_par_t parity)
{
    g_uart2_handle.State            = HAL_USART_STATE_RESET;
    g_uart2_handle.Instance         = USART2;
    g_uart2_handle.Init.BaudRate    = baud;
    g_uart2_handle.Init.WordLength  = (data_bits == 9) ? USART_WORDLENGTH_9B : USART_WORDLENGTH_8B;
    g_uart2_handle.Init.StopBits    = USART_STOPBITS_1;
    g_uart2_handle.Init.Parity      = (parity == UART_PAR_EVEN) ? USART_PARITY_EVEN : (parity == UART_PAR_ODD) ? USART_PARITY_ODD : USART_PARITY_NONE;
    g_uart2_handle.Init.Mode        = USART_MODE_TX_RX;
    g_uart2_handle.Init.CLKPolarity = 0;
    g_uart2_handle.Init.CLKPhase    = 0;
    g_uart2_handle.Init.CLKLastBit  = USART_LASTBIT_DISABLE;

    HAL_USART_Init(&g_uart2_handle);
}

void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (husart->Instance == USART2)
    {
        __HAL_RCMU_USART2_CLK_ENABLE();
        __HAL_RCMU_GPIOA_CLK_ENABLE();
        __HAL_RCMU_GPIOD_CLK_ENABLE();

        // USART2 (datasheet pin mapping: PD0 = RXD, PD1 = TXD)
        GPIO_InitStruct.Pin         = GPIO_PIN_0;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF;
        GPIO_InitStruct.Pull        = GPIO_PULLUP;
        GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Alternate   = (GPIO_AF_TypeDef)GPIO_AF1_USART2_RXD;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        GPIO_InitStruct.Pin       = GPIO_PIN_1;
        GPIO_InitStruct.Alternate = (GPIO_AF_TypeDef)GPIO_AF1_USART2_TXD;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        // RS485 direction (DE/RE): PB6, high = drive(TX) / low = receive(RX)
        GPIO_InitStruct.Pin         = GPIO_PIN_6;
        GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Alternate   = (GPIO_AF_TypeDef)0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); /* start in receive */

        HAL_NVIC_SetPriority(USART2_IRQn, 2);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

void HAL_USART_MspDeInit(USART_HandleTypeDef *husart)
{
    if (husart->Instance == USART2)
    {
        __HAL_RCMU_USART2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

void uart2_set_rx_cb(uart_rx_cb_t cb)
{
    g_rx_cb = cb;
}

void uart2_set_tx_cb(uart_tx_cb_t cb)
{
    g_tx_cb = cb;
}

void uart2_rx_enable(bool enable)
{
    g_rx_en = enable;
    if (enable)
    {
        HAL_USART_Receive_IT(&g_uart2_handle, &g_rx_byte, 1);
    }
    else
    {
        __HAL_USART_DISABLE_IT(&g_uart2_handle, USART_IT_RXNE);
        /* Drop the BUSY_RX state so a subsequent HAL_USART_Transmit_IT is not
         * rejected with HAL_BUSY. HAL has no AbortReceive API here; the receiver
         * is re-armed on the next enable. */
        g_uart2_handle.State = HAL_USART_STATE_READY;
        g_uart2_handle.Lock  = HAL_UNLOCKED;
    }
}

void uart2_putc(uint8_t byte)
{
    g_tx_byte = byte;
    HAL_USART_Transmit_IT(&g_uart2_handle, &g_tx_byte, 1);
}

void uart2_set_de(bool tx)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, tx ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
