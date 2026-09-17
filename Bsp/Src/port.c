#include "port.h"
#include "ad.h"
#include "tim.h"
#include "gpio.h"

/* Hardware abstraction layer: the ONE place (besides main.c / slm32m030_it.c)
 * that calls the Bsp board drivers. Platform-independent code uses these. */

/* ---- power stage (PWM) ---- */
void port_pwm_enable(void)
{
    tim_pwm_enable();
}

void port_pwm_disable(void)
{
    tim_pwm_disable();
}

void port_pwm_set_duty(uint16_t a, uint16_t b, uint16_t c)
{
    tim_pwm_update_ccr(a, b, c);
}

void port_pwm_charge_phase(uint8_t phase)
{
    tim_pwm_charge_phase(phase);
}

void port_pwm_restore(void)
{
    tim_pwm_restore();
}

uint16_t port_pwm_get_reload(void)
{
    return (uint16_t)TIM_PWM_RELOAD_CNT;
}

/* ---- slow analog channels ---- */
void port_adc_trigger(void)
{
    adc_seq1_sw_conv();
}

uint32_t port_adc_get(port_adc_ch_t ch)
{
    switch (ch)
    {
        case PORT_ADC_AC_PEAK:
            return adc_get_seq1_code(ADC_SEQ1_AC_PEAK);
        case PORT_ADC_NTC:
            return adc_get_seq1_code(ADC_SEQ1_NTC);
        default:
            return 0;
    }
}

/* ---- discrete outputs ---- */
void port_led_set(uint8_t on)
{
    gpio_led_set(on);
}

void port_power_delay_set(uint8_t on)
{
    gpio_power_delay_set(on);
}

/* ---- time ---- */
uint32_t port_time_ms(void)
{
    return systick_get();
}

/* ---- HSM power-stage vtable ---- */
static const mc_hw_if_t g_port_hw_if = {
    .pwm_enable  = port_pwm_enable,
    .pwm_disable = port_pwm_disable,
};

const mc_hw_if_t *port_hw_if(void)
{
    return &g_port_hw_if;
}
