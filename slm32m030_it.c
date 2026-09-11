#include "ad.h"
#include "bsp_hal.h"
#include "convert.h"
#include "state_task.h"
#include "tim.h"
#include "uart.h"
#include "user_config.h"

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
            state_task_isr_break();
        }
    }
}

/**
 * @brief ADC global interrupt: injected SEQ2 frame done -> carrier step.
 *        Reads the four injected results (I_B, I_C, V_DC, I_DC) straight from
 *        the SEQ2 result registers, converts them to pu, and hands the frame
 *        to the state task. Nothing else runs here.
 */
extern volatile uint32_t g_isr_cyc;
extern volatile uint32_t g_isr_cyc_max;
extern state_para_t      g_state;

void ADC_IRQHandler(void)
{
    if (__HAL_ADC_GET_FLAG(&g_adc_handle, ADC_INTR_SEQ2_1SEQ_STS) != RESET)
    {
        __HAL_ADC_CLEAR_FLAG(&g_adc_handle, ADC_INTR_SEQ2_1SEQ_STS);

        /* Injected SEQ2 results, register order == g_adc_seq2_code enum order. */
        g_adc_seq2_code[ADC_SEQ2_I_B]  = *(volatile uint32_t *)(ADC_SEQ2SR1_ADDR);
        g_adc_seq2_code[ADC_SEQ2_I_C]  = *(volatile uint32_t *)(ADC_SEQ2SR2_ADDR);
        g_adc_seq2_code[ADC_SEQ2_V_DC] = *(volatile uint32_t *)(ADC_SEQ2SR3_ADDR);
        g_adc_seq2_code[ADC_SEQ2_I_DC] = *(volatile uint32_t *)(ADC_SEQ2SR4_ADDR);

        uint32_t t0 = tim_load_isr_get();

        /* raw code -> gain-compensated Q15 in one fused multiply (inline);
         * offsets read directly, ia = -(ib+ic) derived from register caches to
         * avoid bouncing the volatile meas fields. */
        int32_t ib = iphase_code_to_gain_pu((int32_t)adc_get_seq2_code(ADC_SEQ2_I_B), g_state.adc_off_ib);
        int32_t ic = iphase_code_to_gain_pu((int32_t)adc_get_seq2_code(ADC_SEQ2_I_C), g_state.adc_off_ic);
        int32_t ia = q15_sat(-ib - ic);

        g_state.ib_meas  = (q15_t)ib;
        g_state.ic_meas  = (q15_t)ic;
        g_state.ia_meas  = (q15_t)ia;
        g_state.udc_meas = udc_code_to_pu((int32_t)adc_get_seq2_code(ADC_SEQ2_V_DC));

#if (IDC_SOURCE == IDC_FROM_ADC)
        g_state.idc_meas = idc_code_to_pu((int32_t)adc_get_seq2_code(ADC_SEQ2_I_DC), g_state.adc_off_idc);
        g_state.idc_meas = (q15_t)(((int32_t)g_state.idc_meas * CURRENT_GAIN_Q8 >> CURRENT_GAIN_SHIFT));
#else
        q15_t da = g_state.duties_q15.a;
        q15_t db = g_state.duties_q15.b;
        q15_t dc = g_state.duties_q15.c;

        g_state.idc_meas = q15_sat((((ia * da) >> Q15_SHIFT) + ((ib * db) >> Q15_SHIFT) + ((ic * dc) >> Q15_SHIFT)));
#endif

        /* state task: cali/charge timing + (when RUNNING) OC, poke, FOC -> ccr */
        state_task_isr();

        uint32_t t1 = tim_load_isr_get();
        g_isr_cyc   = t1 - t0;
        if (g_isr_cyc > g_isr_cyc_max)
            g_isr_cyc_max = g_isr_cyc;
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
