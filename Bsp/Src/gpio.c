#include "slm32x030_hal.h"
#include "gpio.h"

void gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCMU_GPIOA_CLK_ENABLE();
    __HAL_RCMU_GPIOB_CLK_ENABLE();
    __HAL_RCMU_GPIOC_CLK_ENABLE();
    __HAL_RCMU_GPIOD_CLK_ENABLE();

    // power delay
    GPIO_InitStruct.Pin         = GPIO_PIN_7;
    GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
    GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // led
    GPIO_InitStruct.Pin         = GPIO_PIN_12;
    GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
    GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void gpio_led_set(uint8_t on)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void gpio_power_delay_set(uint8_t on)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
