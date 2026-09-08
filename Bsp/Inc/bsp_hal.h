#ifndef __BSP_HAL_H__
#define __BSP_HAL_H__

/* bsp_hal.h -- HAL driver handles owned by the BSP. Only BSP sources include
 * this (and main.c during init); Modules / User never see HAL types. The
 * handle definitions live in the owning driver .c (tim.c, ad.c, spi.c,
 * uart.c, porttimer.c). */

#ifdef __cplusplus
extern "C"
{
#endif

#include "slm32x030_hal.h"

extern SPI_HandleTypeDef   g_spi1_handle;
extern ADC_HandleTypeDef   g_adc_handle;
extern TIM_HandleTypeDef   g_tim1_handle;
extern TIM_HandleTypeDef   g_tim3_handle;
extern TIM_HandleTypeDef   g_tim6_handle;
extern TIM_HandleTypeDef   g_tim14_handle;
extern USART_HandleTypeDef g_uart2_handle;

#ifdef __cplusplus
}
#endif

#endif
