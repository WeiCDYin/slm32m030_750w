#include "slm32x030_hal.h"
#include "bsp_hal.h"
#include "user_config.h"
#include "ad.h"
#include "tim.h"

/*
DTG[7:5]=0xx => DT=DTG[7:0] * tdtg,      tdtg = tDTS
DTG[7:5]=10x => DT=(64+DTG[5:0]) * tdtg, Tdtg = 2xtDTS
DTG[7:5]=110 => DT=(32+DTG[4:0]) * tdtg, Tdtg = 8xtDTS
DTG[7:5]=111 => DT=(32+DTG[4:0]) * tdtg, Tdtg = 16xtDTS
eg. 4Div and 64MHz
    tDTS = 4 * 15.625ns = 62.5ns
if DTG[7:5]=0xx ==>> tdtg = 62.5ns * DTG[6:0]  ==>> (0, 7937.5)ns
if DTG[7:5]=10x ==>> tdtg = 2 * 62.5ns * (DTG[5:0] + 64)  ==>> (8000, 11875)ns
*/
#define tDTS_NS  (1000000000.0f / SYSTEM_CORE_CLOCK_HZ * 4.0f)
#define DTG7_0   (TIM_DEAD_TIME_NS / tDTS_NS)
#define DEADTIME ((uint16_t)(SYSTEM_CORE_CLOCK_HZ / 2.0f * TIM_DEAD_TIME_NS / 1000000000uL))

/* CCR4 fires the ADC sequence early so the last two (current) channels sample at
 * the PWM apex: reload minus the whole SEQ1 conversion window (ad.h). */
#define TIM_CCR4_TRIGGER_ADC_CNT (TIM_PWM_RELOAD_CNT - ADC_SQE_SAMPLE_TICKS)

#define TIM_MOE_ENABLE(htim)  SET_BIT((htim)->Instance->BDTR, TIMX_BDTR4_MOE)
#define TIM_MOE_DISABLE(htim) CLEAR_BIT((htim)->Instance->BDTR, TIMX_BDTR4_MOE)
#define TIM_CNT_ENABLE(htim)  SET_BIT((htim)->Instance->CR1, TIMX_CR1_CEN)
#define TIM_CNT_DISABLE(htim) CLEAR_BIT((htim)->Instance->CR1, TIMX_CR1_CEN)

TIM_HandleTypeDef g_tim1_handle, g_tim3_handle, g_tim14_handle;

void tim_pwm_init(void)
{
    TIM_OC_InitTypeDef             tim1_oc_init   = {0};
    TIM_BreakDeadTimeConfigTypeDef tim1_break_cfg = {0};
    TIM_ClockConfigTypeDef         tim1_clock_cfg = {0};

    g_tim1_handle.Instance               = TIM1;
    g_tim1_handle.Init.Prescaler         = 0;
    g_tim1_handle.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED2;
    g_tim1_handle.Init.Period            = TIM_PWM_RELOAD_CNT;
    g_tim1_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV4; // only for deadtime
    g_tim1_handle.Init.RepetitionCounter = 0;
    g_tim1_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&g_tim1_handle);

    tim1_clock_cfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&g_tim1_handle, &tim1_clock_cfg);

    tim1_oc_init.OCMode       = TIM_OCMODE_PWM1;
    tim1_oc_init.OCPolarity   = TIM_OCPOLARITY_HIGH;
    tim1_oc_init.OCNPolarity  = TIM_OCPOLARITY_HIGH;
    tim1_oc_init.OCFastMode   = TIM_OCFAST_DISABLE;
    tim1_oc_init.OCIdleState  = TIM_OCIDLESTATE_RESET;
    tim1_oc_init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    tim1_oc_init.Pulse        = 0;
    HAL_TIM_PWM_ConfigChannel(&g_tim1_handle, &tim1_oc_init, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&g_tim1_handle, &tim1_oc_init, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&g_tim1_handle, &tim1_oc_init, TIM_CHANNEL_3);
    tim1_oc_init.OCMode       = TIM_OCMODE_PWM2;
    tim1_oc_init.OCPolarity   = TIM_OCPOLARITY_HIGH;
    tim1_oc_init.OCNPolarity  = TIM_OCPOLARITY_HIGH;
    tim1_oc_init.OCFastMode   = TIM_OCFAST_DISABLE;
    tim1_oc_init.OCIdleState  = TIM_OCIDLESTATE_RESET;
    tim1_oc_init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    tim1_oc_init.Pulse        = TIM_CCR4_TRIGGER_ADC_CNT; /* leads the apex so the SEQ1 current channels (last two) sample around it */
    HAL_TIM_PWM_ConfigChannel(&g_tim1_handle, &tim1_oc_init, TIM_CHANNEL_4);

#if FAULT_ONE_SHOT_HW_OVER_CURRENT_ENABLE
    tim1_break_cfg.BreakState = TIM_BREAK_ENABLE;
#else
    tim1_break_cfg.BreakState = TIM_BREAK_DISABLE;
#endif
    tim1_break_cfg.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    tim1_break_cfg.DeadTime         = DTG7_0;
    tim1_break_cfg.OffStateRunMode  = TIM_OSSR_ENABLE; // brake & MOE=1, PWM output setting by OCIdleState and OCNIdleState without floating
    tim1_break_cfg.OffStateIDLEMode = TIM_OSSI_ENABLE; // MOE=0, PWM output setting by OCIdleState and OCNIdleState without floating
    tim1_break_cfg.LockLevel        = TIM_LOCKLEVEL_OFF;
    tim1_break_cfg.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(&g_tim1_handle, &tim1_break_cfg);

    TIM_CCxChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
    TIM_CCxChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
    TIM_CCxChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
    TIM_CCxChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_4, TIM_CCx_ENABLE);
    TIM_CCxNChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_1, TIM_CCxN_ENABLE);
    TIM_CCxNChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_2, TIM_CCxN_ENABLE);
    TIM_CCxNChannelCmd(g_tim1_handle.Instance, TIM_CHANNEL_3, TIM_CCxN_ENABLE);

    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_3, 0);
    __HAL_TIM_ENABLE_IT(&g_tim1_handle, TIM_IT_BREAK);
    TIM_CNT_ENABLE(&g_tim1_handle);
    TIM1->RCR = 1;
}

void tim_load_isr_init(void)
{
    g_tim3_handle.Instance               = TIM3;
    g_tim3_handle.Init.Prescaler         = 0;
    g_tim3_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_tim3_handle.Init.Period            = 0xFFFFFFFF;
    g_tim3_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1; // only for deadtime
    g_tim3_handle.Init.RepetitionCounter = 0;
    g_tim3_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&g_tim3_handle);
    HAL_TIM_Base_Start(&g_tim3_handle);
}

void tim_load_poll_init(void)
{
    g_tim14_handle.Instance               = TIM14;
    g_tim14_handle.Init.Prescaler         = 0;
    g_tim14_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_tim14_handle.Init.Period            = 0xFFFFFFFF;
    g_tim14_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_tim14_handle.Init.RepetitionCounter = 0;
    g_tim14_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&g_tim14_handle);
    HAL_TIM_Base_Start(&g_tim14_handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (htim->Instance == TIM1)
    {
        __HAL_RCMU_TIM1_CLK_ENABLE();
        __HAL_RCMU_GPIOA_CLK_ENABLE();
        __HAL_RCMU_GPIOB_CLK_ENABLE();
        // SVPWM
        // PB13-> TIM1_CH1N
        // PB14-> TIM1_CH2N
        // PB15-> TIM1_CH3N
        GPIO_InitStruct.Pin         = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF;
        GPIO_InitStruct.Pull        = GPIO_PULLDOWN;
        GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        // PA8-> TIM1_CH1
        // PA9-> TIM1_CH2
        // PA10-> TIM1_CH3
        GPIO_InitStruct.Pin         = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF;
        GPIO_InitStruct.Pull        = GPIO_PULLDOWN;
        GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        // PB12-> TIM1BRKIN
        GPIO_InitStruct.Pin         = GPIO_PIN_12;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Otype       = GPIO_PUSHPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_HIGH;
        GPIO_InitStruct.DrvStrength = GPIO_DRV_STRENGTH_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0);
        HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    }
    else if (htim->Instance == TIM3)
    {
        __HAL_RCMU_TIM3_CLK_ENABLE();
    }
    else if (htim->Instance == TIM14)
    {
        __HAL_RCMU_TIM14_CLK_ENABLE();
    }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        __HAL_RCMU_TIM1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_6);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
        HAL_NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    }
    else if (htim->Instance == TIM3)
    {
        __HAL_RCMU_TIM3_CLK_DISABLE();
    }
    else if (htim->Instance == TIM14)
    {
        __HAL_RCMU_TIM14_CLK_DISABLE();
    }
}

void tim_pwm_enable(void)
{
    // enable pwm output
    __HAL_TIM_CLEAR_IT(&g_tim1_handle, TIM_IT_BREAK);
    TIM_MOE_ENABLE(&g_tim1_handle);
}

void tim_pwm_disable(void)
{
    // disable pwm output
    __HAL_TIM_CLEAR_IT(&g_tim1_handle, TIM_IT_BREAK);
    TIM_MOE_DISABLE(&g_tim1_handle);
}

/* Bootstrap pre-charge one phase: disable every channel, then enable only the
 * given phase's LOW-side output (CHxN). With CCR=0 that low-side FET stays on;
 * the other phases (both FETs) and the selected high side stay off. phase:
 * 0=A (TIM_CH3), 1=B (TIM_CH1), 2=C (TIM_CH2) per update_ccr mapping. */
void tim_pwm_charge_phase(uint8_t phase)
{
    TIM_TypeDef *TIMx = g_tim1_handle.Instance;

    static const uint32_t ch_of_phase[3] = {TIM_CHANNEL_3, TIM_CHANNEL_1, TIM_CHANNEL_2};

    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_1, TIM_CCx_DISABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_1, TIM_CCxN_DISABLE);
    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_2, TIM_CCx_DISABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_2, TIM_CCxN_DISABLE);
    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_3, TIM_CCx_DISABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_3, TIM_CCxN_DISABLE);

    if (phase < 3)
        TIM_CCxNChannelCmd(TIMx, ch_of_phase[phase], TIM_CCxN_ENABLE);
}

/* Restore the normal 3-phase complementary outputs (CHx + CHxN) for FOC. */
void tim_pwm_restore(void)
{
    TIM_TypeDef *TIMx = g_tim1_handle.Instance;

    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_1, TIM_CCx_ENABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_1, TIM_CCxN_ENABLE);
    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_2, TIM_CCx_ENABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_2, TIM_CCxN_ENABLE);
    TIM_CCxChannelCmd(TIMx, TIM_CHANNEL_3, TIM_CCx_ENABLE);
    TIM_CCxNChannelCmd(TIMx, TIM_CHANNEL_3, TIM_CCxN_ENABLE);
}

void tim_pwm_update_ccr(uint16_t ccr1, uint16_t ccr2, uint16_t ccr3)
{
    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_1, ccr2); // ch-1 -> B
    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_2, ccr3); // ch-2 -> C
    __HAL_TIM_SET_COMPARE(&g_tim1_handle, TIM_CHANNEL_3, ccr1); // ch-3 -> A
}

uint32_t tim_load_isr_get(void)
{
    return __HAL_TIM_GET_COUNTER(&g_tim3_handle);
}

uint32_t tim_load_poll_get(void)
{
    return __HAL_TIM_GET_COUNTER(&g_tim14_handle);
}

uint32_t systick_get(void)
{
    return HAL_GetTick();
}
