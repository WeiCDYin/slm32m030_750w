#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void tim_pwm_init(void);
void tim_pwm_enable(void);
void tim_pwm_disable(void);
void tim_pwm_update_ccr(uint16_t ccr1, uint16_t ccr2, uint16_t ccr3);
void tim_pwm_charge_phase(uint8_t phase); /* enable only phase (0=A,1=B,2=C) low-side; other channels off */
void tim_pwm_restore(void);               /* re-enable CHx + CHxN of all 3 phases (back to FOC) */

/* carrier-ISR cycle counter (TIM3, free running, 64 MHz ticks) */
void     tim_load_isr_init(void);
uint32_t tim_load_isr_get(void);

/* main-poll cycle counter (TIM14, free running, 64 MHz ticks) */
void     tim_load_poll_init(void);
uint32_t tim_load_poll_get(void);

/* system 1 ms tick (SysTick base, ms since boot) */
uint32_t systick_get(void);

#ifdef __cplusplus
}
#endif

#endif
