#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void gpio_init(void);
void gpio_led_set(uint8_t on);         /* status LED (PA12) */
void gpio_power_delay_set(uint8_t on); /* bootstrap power-delay pin (PB7) */

#ifdef __cplusplus
}
#endif

#endif
