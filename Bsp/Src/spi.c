#include "slm32x030_hal.h"
#include "spi.h"

SPI_HandleTypeDef g_spi1_handle;

void spi1_init(void)
{
    g_spi1_handle.State                  = HAL_SPI_STATE_RESET;
    g_spi1_handle.Instance               = SPI1;
    g_spi1_handle.Init.Direction         = SPI_DIRECTION_2LINES;
    g_spi1_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
    g_spi1_handle.Init.CLKPhase          = SPI_POLARITY_LOW;
    g_spi1_handle.Init.CLKPolarity       = SPI_PHASE_1EDGE;
    g_spi1_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 250k
    g_spi1_handle.Init.CRCPolynomial     = 0x7;
    g_spi1_handle.Init.Mode              = SPI_MODE_MASTER;
    g_spi1_handle.Init.NSS               = SPI_NSS_SOFT;

    HAL_SPI_Init(&g_spi1_handle);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        __HAL_RCMU_SPI1_CLK_ENABLE();
        __HAL_RCMU_GPIOA_CLK_ENABLE();
        __HAL_RCMU_GPIOB_CLK_ENABLE();

        // SPI
        // PA15-> SPI1_NSS
        // PB3-> SPI1_SCK
        // PB4-> SPI1_MISO
        // PB5-> SPI1_MOSI
        GPIO_InitStruct.Pin   = GPIO_PIN_15;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Pull  = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF;
        GPIO_InitStruct.Pull      = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed     = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_InitStruct.Pin       = GPIO_PIN_4;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF;
        GPIO_InitStruct.Pull      = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed     = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF;
        GPIO_InitStruct.Pull      = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed     = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        __HAL_RCMU_SPI1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    }
}
