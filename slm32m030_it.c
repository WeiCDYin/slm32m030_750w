#include "ad.h"
#include "bsp_hal.h"
#include "convert.h"
#include "state_task.h"
#include "tim.h"
#include "uart.h"
#include "user_config.h"

/* DMA sink for the SEQ1 conversions: one raw code per ADC_SEQ1_* channel,
 * filled by the ADC DMA every carrier frame (enum order == channel order). */
volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];

extern state_para_t g_state;
/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers */
/******************************************************************************/
void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    tim_pwm_disable();
    __disable_irq();
    __DSB();
    while (1) {}
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick(); /* 1 ms tick base; the 1 ms task runs in main() */
}

/**
 * @brief TIM1 Break/Update/Trig/Com: hardware DC-bus over-current break.
 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&g_tim1_handle, TIM_FLAG_BREAK) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&g_tim1_handle, TIM_IT_BREAK) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&g_tim1_handle, TIM_IT_BREAK);
            state_task_isr_break();
        }
    }
}

/**
 * @brief DMA1 channel 0..5: ADC SEQ1 frame done -> state_task carrier step.
 *        Handled inline: convert raw codes to pu, hand the frame to the
 *        state task, push the CCR it returns. Nothing else runs here.
 */
extern volatile uint32_t g_isr_cyc;
extern volatile uint32_t g_isr_cyc_max;

void DMA1_Channel0_5_IRQHandler(void)
{
    DMA_HandleTypeDef *hdma = g_adc_handle.hdmaSeq1;

    uint32_t ch = 0x1U << hdma->ChannelNum;

    if (__HAL_DMA_GET_INTTCSTA_FLAG(hdma, ch) != RESET)
    {
        __HAL_DMA_CLEAR_INTTCCLR_FLAG(hdma, ch);
        __HAL_ADC_CLEAR_FLAG(&g_adc_handle, ADC_INTR_SEQ1_1SEQ_STS);

        uint32_t t0 = tim_load_isr_get();

        g_state.ib_meas  = iphase_code_to_pu((int32_t)adc_get_code(ADC_SEQ1_I_B), state_task_adc_off_ib());
        g_state.ic_meas  = iphase_code_to_pu((int32_t)adc_get_code(ADC_SEQ1_I_C), state_task_adc_off_ic());
        g_state.udc_meas = udc_code_to_pu((int32_t)adc_get_code(ADC_SEQ1_V_DC));

        /* gain compensation */
        g_state.ib_meas = (q15_t)(((int32_t)g_state.ib_meas * CURRENT_GAIN_Q8 >> 8));
        g_state.ic_meas = (q15_t)(((int32_t)g_state.ic_meas * CURRENT_GAIN_Q8 >> 8));
        g_state.ia_meas = q15_sat(-g_state.ib_meas - g_state.ic_meas);

#if (IDC_SOURCE == IDC_FROM_ADC)
        g_state.idc_meas = idc_code_to_pu((int32_t)adc_get_code(ADC_SEQ1_I_DC), state_task_adc_off_idc());
        g_state.idc_meas = (q15_t)(((int32_t)g_state.idc_meas * CURRENT_GAIN_Q8 >> 8));
#else
        int32_t idc_acc = ((int32_t)g_state.ia_meas * g_state.duties_q15.a >> Q15_SHIFT) +
                          ((int32_t)g_state.ib_meas * g_state.duties_q15.b >> Q15_SHIFT) +
                          ((int32_t)g_state.ic_meas * g_state.duties_q15.c >> Q15_SHIFT);
        g_state.idc_meas = q15_sat(idc_acc);
#endif

        /* state task: cali/charge timing + (when RUNNING) OC, poke, FOC -> ccr */
        uint16_t ccr[3];
        if (state_task_isr(ccr))
            tim_pwm_update_ccr(ccr[0], ccr[1], ccr[2]);

        uint32_t t1 = tim_load_isr_get();
        g_isr_cyc   = t1 - t0;
        if (g_isr_cyc > g_isr_cyc_max)
            g_isr_cyc_max = g_isr_cyc;
    }

    /* transfer error: clear; the Continue DMA re-triggers on the next frame */
    if (__HAL_DMA_GET_INTERRSTA_FLAG(hdma, ch) != RESET)
        __HAL_DMA_CLEAR_INTERRCLR_FLAG(hdma, ch);
}

/**
 * @brief USART2 global interrupt (Modbus RTU link).
 */
void USART2_IRQHandler(void)
{
    HAL_USART_IRQHandler(&g_uart2_handle);
}

/**
 * @brief TIM6 global interrupt (FreeModbus T35 timer).
 */
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim6_handle);
}
