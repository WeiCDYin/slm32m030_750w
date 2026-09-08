/**
  ******************************************************************************
  * @file    system_slm32x030.h
  * @author  Software Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for SLM32L0xx devices.            
  *            
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *  
  ******************************************************************************
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup slm32x030_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef _SYSTEM_SLM32X030_H_
#define _SYSTEM_SLM32X030_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/** @addtogroup slm32x030_System_Includes
  * @{
  */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @addtogroup slm32x030_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @addtogroup slm32x030_System_Exported_types
  * @{
  */
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */



/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/
/** @addtogroup slm32x030_System_Exported_Macros
  * @{
  */

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE               16000000U /*!< Default value of the External oscillator in Hz. This value can be provided and adapted by the user application. */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE               8000000U /*!< Default value of the Internal oscillator in Hz. This value can be provided and adapted by the user application. */
#endif /* HSI_VALUE */


#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    ((uint32_t)100)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE) 
 #define LSI_VALUE               38400U    /*!< LSI Typical Value in Hz */
#endif /* LSI_VALUE */                     /*!< Value of the Internal Low Speed oscillator in Hz
                                                The real value may vary depending on the variations
                                                in voltage and temperature. */

/**
  * @brief External Low Speed oscillator (LSE) value.
  *        This value is used by the UART, RTC HAL module to compute the system frequency
  */
#if !defined  (LSE_VALUE)
  #define LSE_VALUE    ((uint32_t)32768) /*!< Value of the External oscillator in Hz*/
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */


/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00000000U /*!< Vector Table base offset field. 
                                          This value must be a multiple of 0x200. */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup slm32x030_system_exported_types
  * @{
  */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/** @addtogroup slm32x030_system_exported_types
  * @{
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*_SYSTEM_SLM32X030_H_ */

/**
  * @}
  */ /* End of group slm32x030_system */
  
/**
  * @}
  */ /* End of group CMSIS */

/************************ (C) COPYRIGHT Soulsemi *****END OF FILE****/
