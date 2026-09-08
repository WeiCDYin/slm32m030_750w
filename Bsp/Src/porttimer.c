/* FreeModbus port: T35 frame-gap timer on TIM6 (basic timer, APB1 @ 64 MHz).
 * xMBPortTimersInit receives the timeout in 50 us units (RTU layer computes
 * T35 = 3.5 char times, or 1750 us above 19200 baud). The timer runs one-shot:
 * enable -> count down -> HAL_TIM_PeriodElapsedCallback -> pxMBPortCBTimerExpired. */
#include "slm32x030_hal.h"
#include "bsp_hal.h"
#include "mb.h"
#include "mbport.h"

#define TIM6_PRESCALER (64u - 1u) /* 64 MHz / 64 = 1 MHz -> 1 us per tick */

TIM_HandleTypeDef g_tim6_handle;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &g_tim6_handle)
    {
        vMBPortTimersDisable();
        (void)pxMBPortCBTimerExpired();
    }
}

BOOL xMBPortTimersInit(USHORT usTimeOut50us)
{
    __HAL_RCMU_TIM6_CLK_ENABLE();

    g_tim6_handle.Instance               = TIM6;
    g_tim6_handle.Init.Prescaler         = TIM6_PRESCALER;
    g_tim6_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_tim6_handle.Init.Period            = (uint32_t)usTimeOut50us * 50u; /* 50us -> us */
    g_tim6_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_tim6_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&g_tim6_handle);

    HAL_NVIC_SetPriority(TIM6_IRQn, 2);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);

    return TRUE;
}

void vMBPortTimersEnable(void)
{
    __HAL_TIM_SET_COUNTER(&g_tim6_handle, 0);
    HAL_TIM_Base_Start_IT(&g_tim6_handle);
}

void vMBPortTimersDisable(void)
{
    HAL_TIM_Base_Stop_IT(&g_tim6_handle);
}

void vMBPortTimersDelay(USHORT usTimeOutMS)
{
    HAL_Delay(usTimeOutMS);
}

void vMBPortTimersClose(void)
{
    vMBPortTimersDisable();
}
