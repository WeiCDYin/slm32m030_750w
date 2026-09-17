#include "ad.h"
#include "bsp_hal.h"
#include "main_task.h"
#include "tim.h"
#include "uart.h"
#include "user_control.h"

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers */
/******************************************************************************/

/* Code sinks for the two ADC sequences:
 *  - g_adc_seq1_code: SEQ1 software-triggered slow channels (AC peak, BEMF,
 *    NTC), filled by adc_seq1_sw_conv() in the 1 ms task.
 *  - g_adc_seq2_code: SEQ2 injected fast channels (I_B, I_C, V_DC, I_DC),
 *    filled by ADC_IRQHandler on every TIM1_CC4 carrier edge. */
volatile uint32_t g_adc_seq1_code[ADC_SEQ1_COUNT];
volatile uint32_t g_adc_seq2_code[ADC_SEQ2_COUNT];

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
    /* 1 ms tick base; the 1 ms task runs in main() */
    HAL_IncTick();
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
            main_task_isr_break();
        }
    }
}

/**
 * @brief ADC global interrupt: injected SEQ2 frame done -> carrier step.
 *        Reads the injected results (I_B, I_C, V_DC) straight from the SEQ2
 *        result registers and hands the raw codes to the state task, which does
 *        the code -> pu conversion. Nothing else runs here.
 */
extern volatile uint32_t g_isr_cyc;
extern volatile uint32_t g_isr_cyc_max;

void ADC_IRQHandler(void)
{
    if (__HAL_ADC_GET_FLAG(&g_adc_handle, ADC_INTR_SEQ2_1SEQ_STS) != RESET)
    {
        __HAL_ADC_CLEAR_FLAG(&g_adc_handle, ADC_INTR_SEQ2_1SEQ_STS);

        /* Injected SEQ2 results, register order == g_adc_seq2_code enum order. */
        g_adc_seq2_code[ADC_SEQ2_I_B]  = *(volatile uint32_t *)(ADC_SEQ2SR1_ADDR);
        g_adc_seq2_code[ADC_SEQ2_I_C]  = *(volatile uint32_t *)(ADC_SEQ2SR2_ADDR);
        g_adc_seq2_code[ADC_SEQ2_V_DC] = *(volatile uint32_t *)(ADC_SEQ2SR3_ADDR);

        uint32_t t0 = tim_load_isr_get();

        /* Sample only: hand the raw codes to the state task, which converts.
         * Dual-shunt board (I_B / I_C measured): derive phase A, zero-current
         * code = ADC_OFFSET_CALI_DEFAULT. A three-shunt board would pass the
         * sampled A code instead -- main_task_isr stays topology-agnostic. */
        int32_t ib_code  = (int32_t)adc_get_seq2_code(ADC_SEQ2_I_B);
        int32_t ic_code  = (int32_t)adc_get_seq2_code(ADC_SEQ2_I_C);
        int32_t ia_code  = 3 * (int32_t)ADC_OFFSET_CALI_DEFAULT - ib_code - ic_code;
        int32_t udc_code = (int32_t)adc_get_seq2_code(ADC_SEQ2_V_DC);

        /* state task: code->pu, cali/charge timing + (when RUNNING) OC, FOC -> ccr */
        main_task_isr(ia_code, ib_code, ic_code, udc_code);

        uint32_t t1 = tim_load_isr_get();
        g_isr_cyc   = t1 - t0;
        if (g_isr_cyc > g_isr_cyc_max)
            g_isr_cyc_max = g_isr_cyc;
    }

    if (__HAL_ADC_GET_FLAG(&g_adc_handle, ADC_INTR_SEQ1_1SEQ_STS) != RESET)
    {
        __HAL_ADC_CLEAR_FLAG(&g_adc_handle, ADC_INTR_SEQ1_1SEQ_STS);
        for (uint32_t i = 0; i < ADC_SEQ1_COUNT; i++)
            g_adc_seq1_code[i] = *(volatile uint32_t *)(ADC_SEQ1SR1_ADDR + i * 4u);
    }
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
