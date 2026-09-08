/**
  ******************************************************************************
  * @file    slm32x030_hal_rcmu_ex.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Header file of RCMU HAL Extension module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_RCMU_EX_H_
#define _SLM32X030_HAL_RCMU_EX_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup RCMUx
  * @{
  */ 

/** @addtogroup RCMUx_Private_Constants
 * @{
 */



#define CR_REG_INDEX                      ((uint8_t)1)    

/**
  * @}
  */

/** @addtogroup RCMUx_Private_Macros
 * @{
 */



#define IS_RCMU_HSE_PREDIV(__DIV__)       (((__DIV__) == RCMU_HSE_PREDIV_DIV1)  || ((__DIV__) == RCMU_HSE_PREDIV_DIV2))


#define IS_RCMU_PLL_MUL(__MUL__)          (((__MUL__) == RCMU_PLL_MUL2)  || ((__MUL__) == RCMU_PLL_MUL3)  || \
                                           ((__MUL__) == RCMU_PLL_MUL4)  || ((__MUL__) == RCMU_PLL_MUL5)  || \
                                           ((__MUL__) == RCMU_PLL_MUL6)  || ((__MUL__) == RCMU_PLL_MUL7)  || \
                                           ((__MUL__) == RCMU_PLL_MUL8)  || ((__MUL__) == RCMU_PLL_MUL9)  || \
                                           ((__MUL__) == RCMU_PLL_MUL10) || ((__MUL__) == RCMU_PLL_MUL11) || \
                                           ((__MUL__) == RCMU_PLL_MUL12) || ((__MUL__) == RCMU_PLL_MUL13) || \
                                           ((__MUL__) == RCMU_PLL_MUL14) || ((__MUL__) == RCMU_PLL_MUL15) || \
                                           ((__MUL__) == RCMU_PLL_MUL16))

#define IS_RCMU_ADCPLLCLK_DIV(__ADCCLK__) (((__ADCCLK__) == RCMU_ADCPCLK2_DIV2) || ((__ADCCLK__) == RCMU_ADCPCLK2_DIV4) || \
                                           ((__ADCCLK__) == RCMU_ADCPCLK2_DIV6) || ((__ADCCLK__) == RCMU_ADCPCLK2_DIV8))







/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/ 

/** @defgroup RCMUEx_Exported_Types RCMUx Exported Types
  * @{
  */

/** 
  * @brief  RCMU Internal/External Oscillator (HSE, HSI, LSE LSI PLL) configuration structure definition  
  */
typedef struct
{
    uint8_t HSEState;                /*!< The new state of the HSE.This parameter can be a value of @ref RCMU_HSE_Config */
    uint8_t HSEBypass;               /*!< HSE */                                                                
    uint32_t HSEPredivValue;         /*!<  The Prediv1 factor value (named PREDIV1 or PLLXTPRE in RM) 
	                                       This parameter can be a value of @ref RCMUEx_Prediv1_Factor */
    uint8_t LSEState;                /*!<  The new state of the LSE.
                                          This parameter can be a value of @ref RCMU_LSE_Config */
    uint8_t LSEBypass;               /*!< LSE*/                                              
    uint32_t LSEPredivValue;         /*!<  The LSEPrediv factor */
    uint8_t HSIState;                /*!< The new state of the HSI.
                                          This parameter can be a value of @ref RCMU_HSI_Config */
    uint8_t HSISelect;               /*!< select HSI frequency  */
    uint8_t HSIBypass;               /*!< LSE */                                      
    uint32_t HSIPredivValue;         /*!<  The LSEPrediv factor */
    uint8_t LSIState;                /*!<  The new state of the LSI.*/
    uint8_t LSISelect;               /*!< select LSI frequency  */  
    uint8_t LSIBypass;               /*!< LSI */                                      
    uint32_t LSIPredivValue;         /*!<  The LSIPrediv factor */
    RCMU_PLLInitTypeDef PLL;         /*!< PLL structure parameters */   
} RCMU_OscInitTypeDef;

/** 
  * @brief  RCMU extended clocks structure definition  
  */
typedef struct
{
    uint32_t PeriphClockSelection;      /*!< The Extended Clock to be configured.
                                             This parameter can be a value of @ref RCMUEx_Periph_Clock_Selection */
    uint32_t RTCClockSelection;         /*!< Specifies the RTC clock source.
                                             This parameter can be a value of @ref RCMU_RTC_Clock_Source */
    uint32_t AdcClockSelection;         /*!< ADC clock source      
                                             This parameter can be a value of @ref RCMUEx_ADC_Prescaler */
    uint32_t I2s2ClockSelection;         /*!< I2S2 clock source
                                              This parameter can be a value of @ref RCMUEx_I2S2_Clock_Source */
    uint32_t I2s3ClockSelection;         /*!< I2S3 clock source
                                              This parameter can be a value of @ref RCMUEx_I2S3_Clock_Source */
    uint32_t UsbClockSelection;         /*!< USB clock source      
                                             This parameter can be a value of @ref RCMUEx_USB_Prescaler */
} RCMU_PeriphCLKInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup RCMUEx_Exported_Constants RCMUx Exported Constants
  * @{
  */

/** @defgroup RCMUEx_Periph_Clock_Selection Periph Clock Selection
  * @{
  */
#define RCMU_PERIPHCLK_RTC              (0x00000001UL)
#define RCMU_PERIPHCLK_ADC              (0x00000002UL)
#define RCMU_PERIPHCLK_I2S2             (0x00000004UL)
#define RCMU_PERIPHCLK_I2S3             (0x00000008UL)


/**
  * @}
  */

/** @defgroup RCMUEx_ADC_Prescaler ADC Prescaler
  * @{
  */
#define RCMU_ADCPCLK2_DIV2              RCMU_CFGR_ADCPRE_DIV2
#define RCMU_ADCPCLK2_DIV4              RCMU_CFGR_ADCPRE_DIV4
#define RCMU_ADCPCLK2_DIV6              RCMU_CFGR_ADCPRE_DIV6
#define RCMU_ADCPCLK2_DIV8              RCMU_CFGR_ADCPRE_DIV8

/**
  * @}
  */

/** @defgroup RCMUEx_I2S2_Clock_Source I2S2 Clock Source
  * @{
  */
#define RCMU_I2S2CLKSOURCE_SYSCLK       (0x00000000UL)


/**
  * @}
  */

/** @defgroup RCMUEx_I2S3_Clock_Source I2S3 Clock Source
  * @{
  */
#define RCMU_I2S3CLKSOURCE_SYSCLK       (0x00000000UL)


/**
  * @}
  */


/** @defgroup RCMUEx_USB_Prescaler USB Prescaler
  * @{
  */
#define RCMU_USBCLKSOURCE_PLL           RCMU_CFGR_USBPRE
#define RCMU_USBCLKSOURCE_PLL_DIV1_5    (0x00000000UL)

/**
  * @}
  */






/** @defgroup RCMUEx_Prediv1_Factor HSE Prediv1 Factor
  * @{
  */

#define RCMU_HSE_PREDIV_DIV1            (0x00000000UL)

#define RCMU_HSE_PREDIV_DIV2            RCMU_CFGR_PLLXTPRE


/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */

/** @defgroup RCMUEx_MCO1_Clock_Source MCO1 Clock Source
  * @{
  */
#define RCMU_MCO1SOURCE_NOCLOCK         ((uint32_t)RCMU_CFGR_MCO_NOCLOCK)
#define RCMU_MCO1SOURCE_SYSCLK          ((uint32_t)RCMU_CFGR_MCO_SYSCLK)
#define RCMU_MCO1SOURCE_HSI             ((uint32_t)RCMU_CFGR_MCO_HSI)
#define RCMU_MCO1SOURCE_HSE             ((uint32_t)RCMU_CFGR_MCO_HSE)
#define RCMU_MCO1SOURCE_PLLCLK          ((uint32_t)RCMU_CFGR_MCO_PLLCLK_DIV2)


/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup RCMUEx_Exported_Macros RCMUx Exported Macros
 * @{
 */


/** @defgroup RCMUEx_APB1_Clock_Enable_Disable APB1 Clock Enable Disable
  * @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @{   
  */
#define __HAL_RCMU_CAN1_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_CAN1EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_CAN1EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_CAN1_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_CAN1EN))

#define __HAL_RCMU_TIM4_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM4EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM4EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_SPI2_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI2EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI2EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_USART3_CLK_ENABLE()      do {                                                         \
                                                __IO uint32_t tmpreg;                                    \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USART3EN);           \
                                                /* Delay after an RCMU peripheral clock enabling */      \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USART3EN); \
                                                UNUSED(tmpreg);                                          \
                                            } while(0)

#define __HAL_RCMU_I2C2_CLK_ENABLE()        do {                                                       \
                                                __IO uint32_t tmpreg;                                  \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C2EN);           \
                                                /* Delay after an RCMU peripheral clock enabling */    \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C2EN); \
                                                UNUSED(tmpreg); \
                                            } while(0)

#define __HAL_RCMU_TIM4_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM4EN))
#define __HAL_RCMU_SPI2_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_SPI2EN))
#define __HAL_RCMU_USART3_CLK_DISABLE()     (RCMU->APB1ENR &= ~(RCMU_APB1ENR_USART3EN))
#define __HAL_RCMU_I2C2_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_I2C2EN))



#define __HAL_RCMU_USB_CLK_ENABLE()         do {                                                      \
                                                __IO uint32_t tmpreg;                                 \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USBEN);           \
                                                /* Delay after an RCMU peripheral clock enabling */   \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USBEN); \
                                                UNUSED(tmpreg);                                       \
                                            } while(0)

#define __HAL_RCMU_USB_CLK_DISABLE()        (RCMU->APB1ENR &= ~(RCMU_APB1ENR_USBEN))

#define __HAL_RCMU_TIM5_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM5EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM5EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_TIM6_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM6EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM6EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_TIM7_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM7EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM7EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_SPI3_CLK_ENABLE()        do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI3EN);            \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI3EN);  \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_UART4_CLK_ENABLE()       do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_UART4EN);           \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_UART4EN); \
                                                UNUSED(tmpreg); \
                                            } while(0)

#define __HAL_RCMU_UART5_CLK_ENABLE()       do {                                                        \
                                                __IO uint32_t tmpreg;                                   \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_UART5EN);           \
                                                /* Delay after an RCMU peripheral clock enabling */     \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_UART5EN); \
                                                UNUSED(tmpreg);                                         \
                                            } while(0)

#define __HAL_RCMU_DAC_CLK_ENABLE()         do {                                                      \
                                                __IO uint32_t tmpreg;                                 \
                                                SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_DACEN);           \
                                                /* Delay after an RCMU peripheral clock enabling */   \
                                                tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_DACEN); \
                                                UNUSED(tmpreg);                                       \
                                            } while(0)

#define __HAL_RCMU_TIM5_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM5EN))
#define __HAL_RCMU_TIM6_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM6EN))
#define __HAL_RCMU_TIM7_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM7EN))
#define __HAL_RCMU_SPI3_CLK_DISABLE()       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_SPI3EN))
#define __HAL_RCMU_UART4_CLK_DISABLE()      (RCMU->APB1ENR &= ~(RCMU_APB1ENR_UART4EN))
#define __HAL_RCMU_UART5_CLK_DISABLE()      (RCMU->APB1ENR &= ~(RCMU_APB1ENR_UART5EN))
#define __HAL_RCMU_DAC_CLK_DISABLE()        (RCMU->APB1ENR &= ~(RCMU_APB1ENR_DACEN))







/**
  * @}
  */

/** @defgroup RCMUEx_APB1_Peripheral_Clock_Enable_Disable_Status APB1 Peripheral Clock Enable Disable Status
  * @brief  Get the enable or disable status of the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCMU_CAN1_IS_CLK_ENABLED()       ((RCMU->APB1ENR & (RCMU_APB1ENR_CAN1EN)) != RESET)
#define __HAL_RCMU_CAN1_IS_CLK_DISABLED()      ((RCMU->APB1ENR & (RCMU_APB1ENR_CAN1EN)) == RESET)


#define __HAL_RCMU_TIM4_IS_CLK_ENABLED()       ((RCMU->APB1ENR & (RCMU_APB1ENR_TIM4EN)) != RESET)
#define __HAL_RCMU_TIM4_IS_CLK_DISABLED()      ((RCMU->APB1ENR & (RCMU_APB1ENR_TIM4EN)) == RESET)
#define __HAL_RCMU_SPI2_IS_CLK_ENABLED()       ((RCMU->APB1ENR & (RCMU_APB1ENR_SPI2EN)) != RESET)
#define __HAL_RCMU_SPI2_IS_CLK_DISABLED()      ((RCMU->APB1ENR & (RCMU_APB1ENR_SPI2EN)) == RESET)
#define __HAL_RCMU_USART3_IS_CLK_ENABLED()     ((RCMU->APB1ENR & (RCMU_APB1ENR_USART3EN)) != RESET)
#define __HAL_RCMU_USART3_IS_CLK_DISABLED()    ((RCMU->APB1ENR & (RCMU_APB1ENR_USART3EN)) == RESET)
#define __HAL_RCMU_I2C2_IS_CLK_ENABLED()       ((RCMU->APB1ENR & (RCMU_APB1ENR_I2C2EN)) != RESET)
#define __HAL_RCMU_I2C2_IS_CLK_DISABLED()      ((RCMU->APB1ENR & (RCMU_APB1ENR_I2C2EN)) == RESET)


#define __HAL_RCMU_USB_IS_CLK_ENABLED()        ((RCMU->APB1ENR & (RCMU_APB1ENR_USBEN)) != RESET)
#define __HAL_RCMU_USB_IS_CLK_DISABLED()       ((RCMU->APB1ENR & (RCMU_APB1ENR_USBEN)) == RESET)




/**
  * @}
  */

/** @defgroup RCMUEx_APB2_Clock_Enable_Disable APB2 Clock Enable Disable
  * @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @{   
  */


#define __HAL_RCMU_ADC2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_ADC2EN);\
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_ADC2EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCMU_ADC2_CLK_DISABLE()        (RCMU->APB2ENR &= ~(RCMU_APB2ENR_ADC2EN))




#define __HAL_RCMU_GPIOE_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_IOPEEN);\
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_IOPEEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCMU_GPIOE_CLK_DISABLE()       (RCMU->APB2ENR &= ~(RCMU_APB2ENR_IOPEEN))








/**
  * @}
  */

/** @defgroup RCMUEx_APB2_Peripheral_Clock_Enable_Disable_Status APB2 Peripheral Clock Enable Disable Status
  * @brief  Get the enable or disable status of the APB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */


#define __HAL_RCMU_ADC2_IS_CLK_ENABLED()        ((RCMU->APB2ENR & (RCMU_APB2ENR_ADC2EN)) != RESET)
#define __HAL_RCMU_ADC2_IS_CLK_DISABLED()       ((RCMU->APB2ENR & (RCMU_APB2ENR_ADC2EN)) == RESET)



#define __HAL_RCMU_GPIOE_IS_CLK_ENABLED()       ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPEEN)) != RESET)
#define __HAL_RCMU_GPIOE_IS_CLK_DISABLED()      ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPEEN)) == RESET)




/**
  * @}
  */




/** @defgroup RCMUEx_APB1_Force_Release_Reset APB1 Force Release Reset
  * @brief  Force or release APB1 peripheral reset.
  * @{   
  */


#define __HAL_RCMU_CAN1_FORCE_RESET()           (RCMU->APB1RSTR |= (RCMU_APB1RSTR_CAN1RST))

#define __HAL_RCMU_CAN1_RELEASE_RESET()         (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_CAN1RST))



#define __HAL_RCMU_TIM4_FORCE_RESET()           (RCMU->APB1RSTR |= (RCMU_APB1RSTR_TIM4RST))
#define __HAL_RCMU_SPI2_FORCE_RESET()           (RCMU->APB1RSTR |= (RCMU_APB1RSTR_SPI2RST))
#define __HAL_RCMU_USART3_FORCE_RESET()         (RCMU->APB1RSTR |= (RCMU_APB1RSTR_USART3RST))
#define __HAL_RCMU_I2C2_FORCE_RESET()           (RCMU->APB1RSTR |= (RCMU_APB1RSTR_I2C2RST))
#define __HAL_RCMU_DAC_FORCE_RESET()            (RCMU->APB1RSTR |= (RCMU_APB1RSTR_DACRST))

#define __HAL_RCMU_TIM4_RELEASE_RESET()         (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_TIM4RST))
#define __HAL_RCMU_SPI2_RELEASE_RESET()         (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_SPI2RST))
#define __HAL_RCMU_USART3_RELEASE_RESET()       (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_USART3RST))
#define __HAL_RCMU_I2C2_RELEASE_RESET()         (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_I2C2RST))
#define __HAL_RCMU_DAC_RELEASE_RESET()          (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_DACRST))


#define __HAL_RCMU_USB_FORCE_RESET()            (RCMU->APB1RSTR |= (RCMU_APB1RSTR_USBRST))
#define __HAL_RCMU_USB_RELEASE_RESET()          (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_USBRST))



/** @defgroup RCMUEx_APB2_Force_Release_Reset APB2 Force Release Reset
  * @brief  Force or release APB2 peripheral reset.
  * @{   
  */


#define __HAL_RCMU_ADC2_FORCE_RESET()           (RCMU->APB2RSTR |= (RCMU_APB2RSTR_ADC2RST))

#define __HAL_RCMU_ADC2_RELEASE_RESET()         (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_ADC2RST))




#define __HAL_RCMU_GPIOE_FORCE_RESET()          (RCMU->APB2RSTR |= (RCMU_APB2RSTR_IOPERST))

#define __HAL_RCMU_GPIOE_RELEASE_RESET()        (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_IOPERST))








/** @defgroup RCMUEx_HSE_Configuration HSE Configuration
  * @{   
  */ 

/*
  * @brief  Macro to configure the External High Speed oscillator (HSE) Predivision factor for PLL.
  * @note   Predivision factor can not be changed if PLL is used as system clock
  *         In this case, you have to select another source of the system clock, disable the PLL and
  *         then change the HSE predivision factor.
  * @param  __HSE_PREDIV_VALUE__ specifies the division value applied to HSE.
  *         This parameter must be a number between RCMU_HSE_PREDIV_DIV1 and RCMU_HSE_PREDIV_DIV2.
  */
#define __HAL_RCMU_HSE_PREDIV_CONFIG(__HSE_PREDIV_VALUE__) \
                  MODIFY_REG(RCMU->CFGR,RCMU_CFGR_PLLXTPRE, (uint32_t)(__HSE_PREDIV_VALUE__))


#define __HAL_RCMU_HSE_GET_PREDIV() READ_BIT(RCMU->CFGR, RCMU_CFGR_PLLXTPRE)


/**
  * @}
  */


/** @defgroup RCMUEx_Peripheral_Configuration Peripheral Configuration
  * @brief  Macros to configure clock source of different peripherals.
  * @{
  */  


/** @brief  Macro to configure the USB clock.
  * @param  __USBCLKSOURCE__ specifies the USB clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_USBCLKSOURCE_PLL PLL clock divided by 1 selected as USB clock
  *            @arg @ref RCMU_USBCLKSOURCE_PLL_DIV1_5 PLL clock divided by 1.5 selected as USB clock
  */
#define __HAL_RCMU_USB_CONFIG(__USBCLKSOURCE__) \
                  MODIFY_REG(RCMU->CFGR, RCMU_CFGR_USBPRE, (uint32_t)(__USBCLKSOURCE__))

/** @brief  Macro to get the USB clock (USBCLK).
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCMU_USBCLKSOURCE_PLL PLL clock divided by 1 selected as USB clock
  *            @arg @ref RCMU_USBCLKSOURCE_PLL_DIV1_5 PLL clock divided by 1.5 selected as USB clock
  */
#define __HAL_RCMU_GET_USB_SOURCE() ((uint32_t)(READ_BIT(RCMU->CFGR, RCMU_CFGR_USBPRE)))





/** @brief  Macro to configure the ADCx clock (x=1 to 3 depending on devices).
  * @param  __ADCCLKSOURCE__ specifies the ADC clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_ADCPCLK2_DIV2 PCLK2 clock divided by 2 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV4 PCLK2 clock divided by 4 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV6 PCLK2 clock divided by 6 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV8 PCLK2 clock divided by 8 selected as ADC clock
  */
#define __HAL_RCMU_ADC_CONFIG(__ADCCLKSOURCE__) \
                  MODIFY_REG(RCMU->CFGR, RCMU_CFGR_PPRE2, (uint32_t)(__ADCCLKSOURCE__))

/** @brief  Macro to get the ADC clock (ADCxCLK, x=1 to 3 depending on devices).
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCMU_ADCPCLK2_DIV2 PCLK2 clock divided by 2 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV4 PCLK2 clock divided by 4 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV6 PCLK2 clock divided by 6 selected as ADC clock
  *            @arg @ref RCMU_ADCPCLK2_DIV8 PCLK2 clock divided by 8 selected as ADC clock
  */
#define __HAL_RCMU_GET_ADC_SOURCE() ((uint32_t)(READ_BIT(RCMU->CFGR, RCMU_CFGR_PPRE2)))

/**
  * @}
  */


/** @defgroup RCMUEx_PLL_Multiplication_Factor PLL Multiplication Factor
  * @{
  */
#define RCMU_PLL_MUL2                    RCMU_CFGR_PLLMULL2
#define RCMU_PLL_MUL3                    RCMU_CFGR_PLLMULL3
#define RCMU_PLL_MUL4                    RCMU_CFGR_PLLMULL4
#define RCMU_PLL_MUL5                    RCMU_CFGR_PLLMULL5
#define RCMU_PLL_MUL6                    RCMU_CFGR_PLLMULL6
#define RCMU_PLL_MUL7                    RCMU_CFGR_PLLMULL7
#define RCMU_PLL_MUL8                    RCMU_CFGR_PLLMULL8
#define RCMU_PLL_MUL9                    RCMU_CFGR_PLLMULL9
#define RCMU_PLL_MUL10                   RCMU_CFGR_PLLMULL10
#define RCMU_PLL_MUL11                   RCMU_CFGR_PLLMULL11
#define RCMU_PLL_MUL12                   RCMU_CFGR_PLLMULL12
#define RCMU_PLL_MUL13                   RCMU_CFGR_PLLMULL13
#define RCMU_PLL_MUL14                   RCMU_CFGR_PLLMULL14
#define RCMU_PLL_MUL15                   RCMU_CFGR_PLLMULL15
#define RCMU_PLL_MUL16                   RCMU_CFGR_PLLMULL16
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCMUx_Exported_Functions
  * @{
  */

/** @addtogroup RCMUx_Exported_Functions_Group1
  * @{
  */

HAL_StatusTypeDef HAL_RCMUEx_PeriphCLKConfig(RCMU_PeriphCLKInitTypeDef  *PeriphClkInit);
void              HAL_RCMUEx_GetPeriphCLKConfig(RCMU_PeriphCLKInitTypeDef  *PeriphClkInit);
uint32_t          HAL_RCMUEx_GetPeriphCLKFreq(uint32_t PeriphClk);

/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */
  
#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_RCMU_EX_H_ */



