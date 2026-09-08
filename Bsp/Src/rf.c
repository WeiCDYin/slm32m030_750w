#include "slm32x030_hal.h"
#include "rf.h"

void rf_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCMU_GPIOD_CLK_ENABLE();
    // RF-> PD0
    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Otype = 0;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
