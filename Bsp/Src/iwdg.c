#include "slm32x030_hal.h"
#include "iwdg.h"

static IWDG_HandleTypeDef g_iwdg_handle;

void iwdg_init(void)
{
    g_iwdg_handle.Instance       = IWDG;
    g_iwdg_handle.Init.Prescaler = IWDG_PRESCALER_32; // 40Khz / 32 , 1.25k
    g_iwdg_handle.Init.Reload    = 1249;              // 1 second

    HAL_IWDG_Init(&g_iwdg_handle);
}

void HAL_IWDG_MspInit(IWDG_HandleTypeDef *hiwdg)
{
    if (hiwdg->Instance == IWDG)
    {
        __HAL_RCMU_IWDG_CLK_ENABLE();
    }
}

void iwdg_refresh(void)
{
    HAL_IWDG_Refresh(&g_iwdg_handle);
}
