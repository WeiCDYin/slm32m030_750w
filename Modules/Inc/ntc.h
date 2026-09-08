#ifndef __NTC_H__
#define __NTC_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/*
 * NTC thermistor temperature via a 256-entry direct lookup table.
 * Sensor: 10k @ 25 deg C, B = 3950. NTC pulls up to 5 V, 10k resistor pulls
 * down to GND, 12-bit ADC. Index = ADC code >> 4 (0..255), each entry stores
 * the temperature at the middle of its 16-code bin, clamped to -40..120 deg C.
 * O(1) lookup -- no search, no logf, no FPU cost.
 */

/* Convert the NTC ADC code to temperature in deg C.
 * -400 = NTC shorted (or above 120 deg C), +3000 = open (over temp flag). */
int16_t ntc_temp_c(uint16_t code);

#ifdef __cplusplus
}
#endif
#endif
