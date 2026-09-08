/**
  ******************************************************************************
  * @file    slm32x030_hal_conf.h
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   HAL configuration file.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_CONF_H_
#define _SLM32X030_HAL_CONF_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include "slm32x030.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver 
  */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED 
#define HAL_DMA_MODULE_ENABLED  
#define HAL_CRC_MODULE_ENABLED 
#define HAL_AWK_MODULE_ENABLED
#define HAL_BEEP_MODULE_ENABLED 
#define HAL_ADC_MODULE_ENABLED   
#define HAL_FLASH_MODULE_ENABLED   
#define HAL_GPIO_MODULE_ENABLED 
#define HAL_I2C_MODULE_ENABLED 
#define HAL_IWDG_MODULE_ENABLED
#define HAL_LPTIM_MODULE_ENABLED 
#define HAL_LPUART_MODULE_ENABLED
#define HAL_LVD_MODULE_ENABLED 
#define HAL_PMU_MODULE_ENABLED
#define HAL_RCMU_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED  
#define HAL_SPI_MODULE_ENABLED 
#define HAL_TIM_MODULE_ENABLED 
#define HAL_TRNG_MODULE_ENABLED 
#define HAL_USART_MODULE_ENABLED 
#define HAL_VC_MODULE_ENABLED
#define HAL_WWDG_MODULE_ENABLED 
#define HAL_EXTI_MODULE_ENABLED
#define HAL_SYSCFG_MODULE_ENABLED

#define USE_SPI_CRC 0

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */     
#define  VDD_VALUE                      ((uint32_t)3300) /*!< Value of VDD in mv */           
#define  PRIORITY_HIGHEST               (0U)
#define  PRIORITY_HIGH                  (1U)
#define  PRIORITY_LOW                   (2U)
#define  PRIORITY_LOWEST                (3U)
#define  TICK_INT_PRIORITY              ((uint32_t)PRIORITY_LOWEST)    /*!< tick interrupt priority (lowest by default)  */            
#define  USE_RTOS                       (0U)
#define  PREFETCH_ENABLE                (1U)

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */


/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file 
  */

#ifdef HAL_CORTEX_MODULE_ENABLED
    #include "slm32x030_hal_cortex.h"
#endif /* HAL_CORTEX_MODULE_ENABLED */ 

#ifdef HAL_RCMU_MODULE_ENABLED
    #include "slm32x030_hal_rcmu.h"
    #include "slm32x030_hal_rcmu_ex.h"
#endif /* HAL_RCMU_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
    #include "slm32x030_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
    #include "slm32x030_hal_dma.h"
#endif /* HAL_DMA_MODULE_ENABLED */

#ifdef HAL_PMU_MODULE_ENABLED
    #include "slm32x030_hal_pmu.h"
#endif /* HAL_PMU_MODULE_ENABLED */

#ifdef HAL_CRC_MODULE_ENABLED
    #include "slm32x030_hal_crc.h"
#endif /* HAL_CRC_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
    #include "slm32x030_hal_adc.h"
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_AWK_MODULE_ENABLED
    #include "slm32x030_hal_awk.h"
#endif /* HAL_AWK_MODULE_ENABLED */  

#ifdef HAL_BEEP_MODULE_ENABLED
    #include "slm32x030_hal_beep.h"
#endif /* HAL_BEEP_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
    #include "slm32x030_hal_flash.h"
    #include "slm32x030_hal_flash_ex.h"
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
    #include "slm32x030_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_IWDG_MODULE_ENABLED
    #include "slm32x030_hal_iwdg.h"
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef HAL_LPTIM_MODULE_ENABLED
    #include "slm32x030_hal_lptim.h"
#endif /* HAL_LPTIM_MODULE_ENABLED */

#ifdef HAL_LPUART_MODULE_ENABLED
    #include "slm32x030_hal_lpuart.h"
#endif /* HAL_LPUART_MODULE_ENABLED */

#ifdef HAL_LVD_MODULE_ENABLED
    #include "slm32x030_hal_lvd.h"
#endif /* HAL_LVD_MODULE_ENABLED */ 

#ifdef HAL_RTC_MODULE_ENABLED
    #include "slm32x030_hal_rtc.h"
#endif /* HAL_RTC_MODULE_ENABLED */   

#ifdef HAL_SPI_MODULE_ENABLED
    #include "slm32x030_hal_spi.h"
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
    #include "slm32x030_hal_tim.h"
    #include "slm32x030_hal_tim_ex.h"
#endif /* HAL_TIM_MODULE_ENABLED */

#ifdef HAL_TRNG_MODULE_ENABLED
    #include "slm32x030_hal_trng.h"
#endif /* HAL_PCA_MODULE_ENABLED */ 

#ifdef HAL_USART_MODULE_ENABLED
    #include "slm32x030_hal_usart.h"
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_VC_MODULE_ENABLED
    #include "slm32x030_hal_vc.h"
#endif /* HAL_VC_MODULE_ENABLED */

#ifdef HAL_WWDG_MODULE_ENABLED
    #include "slm32x030_hal_wwdg.h"
#endif /* HAL_WWDG_MODULE_ENABLED */

#ifdef HAL_EXTI_MODULE_ENABLED
    #include "slm32x030_hal_exti.h"
#endif /* HAL_EXTI_MODULE_ENABLED */

#ifdef HAL_SYSCFG_MODULE_ENABLED
    #include "slm32x030_hal_syscfg.h"
#endif /* HAL_SYSCFG_MODULE_ENABLED */

#include "system_slm32x030.h"

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed. 
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_CONF_H_ */
