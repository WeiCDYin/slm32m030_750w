#ifndef _PORT_H
#define _PORT_H

#include <stdint.h>
#include <stdbool.h>
#include "user_board.h" /* TIM_PWM_RELOAD_CNT */
#include "hw_if.h"      /* mc_hw_if_t: the power-stage vtable */

/* ===== hardware abstraction layer (PAL) =====
 * Platform-independent code (Modules/, User/) talks to the board ONLY through
 * these functions. The Bsp drivers (ad/tim/gpio/iwdg) are called directly only
 * by main.c, slm32m030_it.c and Bsp/Src/port.c. */

/* Interrupt-free critical sections, compiler-local and HAL free: the FreeModbus
 * glue and the application protection code (fault.c, main_task.c) share these. */
#if defined(__GNUC__) || defined(__clang__)
#define ENTER_CRITICAL_SECTION() __asm volatile("cpsid i" ::: "memory")
#define EXIT_CRITICAL_SECTION()  __asm volatile("cpsie i" ::: "memory")
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define ENTER_CRITICAL_SECTION() __disable_irq()
#define EXIT_CRITICAL_SECTION()  __enable_irq()
#else
#error "port.h: unsupported compiler for critical section"
#endif

#define INLINE inline
#define PR_BEGIN_EXTERN_C                                                                                                                            \
    extern "C"                                                                                                                                       \
    {
#define PR_END_EXTERN_C }

typedef uint8_t       BOOL;
typedef unsigned char UCHAR;
typedef char          CHAR;
typedef uint16_t      USHORT;
typedef int16_t       SHORT;
typedef uint32_t      ULONG;
typedef int32_t       LONG;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* ---- power stage (PWM) ---- */
void     port_pwm_enable(void);
void     port_pwm_disable(void);
void     port_pwm_set_duty(uint16_t a, uint16_t b, uint16_t c); /* CCR ticks */
void     port_pwm_charge_phase(uint8_t phase);                  /* 0=A,1=B,2=C */
void     port_pwm_restore(void);                                /* re-enable all 3 phases */
uint16_t port_pwm_get_reload(void);                             /* carrier ARR ticks */

/* ---- slow analog channels (1 ms task) ---- */
typedef enum
{
    PORT_ADC_AC_PEAK = 0, /* ac uvw peak voltage */
    PORT_ADC_NTC,         /* temperature */
    PORT_ADC_COUNT
} port_adc_ch_t;

void     port_adc_trigger(void);         /* one software-triggered slow frame */
uint32_t port_adc_get(port_adc_ch_t ch); /* raw code */

/* ---- discrete outputs ---- */
void port_led_set(uint8_t on);
void port_power_delay_set(uint8_t on);

/* ---- time ---- */
uint32_t port_time_ms(void); /* ms since boot */

/* ---- HSM power-stage vtable (pwm enable/disable) ---- */
const mc_hw_if_t *port_hw_if(void);

#endif
