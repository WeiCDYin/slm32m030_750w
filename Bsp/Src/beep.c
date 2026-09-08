#include "slm32x030_hal.h"
#include "beep.h"

static BEEP_HandleTypeDef g_beep_handle;

void beep_init(void)
{
    g_beep_handle.State           = HAL_BEEP_STATE_RESET;
    g_beep_handle.Instance        = BEEP;
    g_beep_handle.Init.ClkSel     = BEEP_CLOCK_PCLK;
    g_beep_handle.Init.OutFreqSel = BEEP_OUTFREQSEL_DIV8;
    g_beep_handle.Init.PreFreqDiv = HAL_RCMU_GetPCLK2Freq() / (BEEP_FREQ * 8) - 1; // Div = fbeepPre / fbeep - 1;
    HAL_BEEP_Init(&g_beep_handle);
}

void HAL_BEEP_MspInit(BEEP_HandleTypeDef *hbeep)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCMU_BEEP_CLK_ENABLE();
    __HAL_RCMU_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin         = GPIO_PIN_7;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF;
    GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
    GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
    GPIO_InitStruct.Alternate   = GPIO_AF6;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void HAL_BEEP_MspDeInit(BEEP_HandleTypeDef *hbeep)
{
    __HAL_RCMU_BEEP_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
}

void beep_start(void)
{
    HAL_BEEP_Start(&g_beep_handle);
}

void beep_stop(void)
{
    HAL_BEEP_Stop(&g_beep_handle);
}
