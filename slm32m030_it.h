/**
 ******************************************************************************
 * @file    slm32f030_it.h
 * @author  Application Team
 * @Version V1.0.0
 * @Date    1-April-2020
 * @brief   This file contains the headers of the interrupt handlers.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SLM32M030_IT_H
#define __SLM32M030_IT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
#ifdef __cplusplus
}
#endif

#endif /* __SLM32M030_IT_H */
