/**
  ******************************************************************************
  * @file    slm32x030_hal_gpio.h
  * @author  MCU Software Team
  * @Version V1.2.0
  * @Date    8-Nov-2024
  * @brief   Header file of GPIO HAL module.
  ******************************************************************************
 **/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_GPIO_H_
#define _SLM32X030_HAL_GPIO_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include "slm32x030_hal_conf.h"

/**
  * @brief GPIO Exti Init structure definition
        These parameters can be values of @ref GPIO_exti_define 
  */
typedef struct 
{
    uint32_t Enable;
    uint32_t EdgeLevelSel;
    uint32_t RiseFallSel;
} GPIO_Exti_InitTypeDef;

typedef enum 
{
    GPIO_AF0 = 0U,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4,   
    GPIO_AF5,
    GPIO_AF6,
    GPIO_AF7,                
} GPIO_AF_TypeDef;

/**
  * @brief GPIO Init structure definition
  */
typedef struct
{
    uint32_t Pin;           /*!< Specifies the GPIO pins to be configured. This parameter can be any value of @ref GPIO_pins_define */                                        
    uint32_t Mode;          /*!< Specifies the operating mode for the selected pins.This parameter can be a value of @ref GPIO_mode_define */                                        
    uint32_t Pull;          /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.This parameter can be a value of @ref GPIO_pull_define */                                        
    uint32_t Otype;         /*!< Specifies the PushPull or OpenDrain output for the selected pins. This parameter can be a value of @ref GPIO_opendrain_define */  
    uint32_t Speed;         /*!< Specifies the speed for the selected pins. This parameter can be a value of @ref GPIO_speed_define */
    uint32_t DrvStrength;   /*!< Specifies the driver strength for the selected pins. This parameter can be a value of @ref GPIO_driver_strength_define */
    GPIO_Exti_InitTypeDef Exti;     /*!< Specifies exti type for the selected pins. This parameter can be a value of @ref GPIO_exti_define */
    GPIO_AF_TypeDef Alternate;      /*!< Peripheral to be connected to the selected pins. This parameter can be a value of @ref GPIO_alternate_function_selection */  
} GPIO_InitTypeDef;

/**
  * @brief  GPIO Bit SET and Bit RESET enumeration
  */
typedef enum
{
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET   = 1U
} GPIO_PinState;

/** @defgroup GPIO_NUMBER
  */
#define GPIO_NUMBER                (16U)

/** @defgroup GPIO_pins_define GPIO pins define
  */
#define GPIO_PIN_0                              ((uint16_t)0x0001)  
#define GPIO_PIN_1                              ((uint16_t)0x0002)  
#define GPIO_PIN_2                              ((uint16_t)0x0004)  
#define GPIO_PIN_3                              ((uint16_t)0x0008)  
#define GPIO_PIN_4                              ((uint16_t)0x0010)  
#define GPIO_PIN_5                              ((uint16_t)0x0020)  
#define GPIO_PIN_6                              ((uint16_t)0x0040)  
#define GPIO_PIN_7                              ((uint16_t)0x0080)  
#define GPIO_PIN_8                              ((uint16_t)0x0100)  
#define GPIO_PIN_9                              ((uint16_t)0x0200)  
#define GPIO_PIN_10                             ((uint16_t)0x0400)  
#define GPIO_PIN_11                             ((uint16_t)0x0800)  
#define GPIO_PIN_12                             ((uint16_t)0x1000)  
#define GPIO_PIN_13                             ((uint16_t)0x2000) 
#define GPIO_PIN_14                             ((uint16_t)0x4000) 
#define GPIO_PIN_15                             ((uint16_t)0x8000) 
#define GPIO_PIN_All                            ((uint16_t)0xFFFF)  /* All pins selected */
#define GPIO_PIN_MASK                           (0x0000FFFFU) /* PIN mask for assert test */

#define  GPIO_MODE_INPUT                        (0x00000000U)   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT                       (0x00000001U)   /*!< Output Mode                           */
#define  GPIO_MODE_AF                           (0x00000002U)   /*!< Alternate Function Mode               */
#define  GPIO_MODE_ANALOG                       (0x00000003U)   /*!< Analog Mode                           */

#define  GPIO_SPEED_LOW                         (0x00000000U)
#define  GPIO_SPEED_HIGH                        (0x00000001U)

/** @defgroup GPIO_pull_define GPIO pull define
  */
#define  GPIO_NOPULL                            (0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP                            (0x00000001U)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN                          (0x00000002U)   /*!< Pull-down activation                */
#define  GPIO_NOPULL_1                          (0x00000003U)   /*!< No Pull-up or Pull-down activation  */

/** @defgroup GPIO_opendrain_define GPIO output mode define
  */
#define  GPIO_PUSHPULL                          (0x00000000U)   /*!< Push pull activation              */
#define  GPIO_OPENDRAIN                         (0x00000001U)   /*!< Open drain activation             */

/** @defgroup GPIO_driver_strength_define GPIO driver strengh
  */
#define  GPIO_DRV_STRENGTH_LOW                  (0x00000000U)   /*!< GPIO driver strengh low   */
#define  GPIO_DRV_STRENGTH_HIGH                 (0x00000001U)   /*!< GPIO driver strengh high  */

/** @defgroup GPIO_exti_define GPIO exti mode define
  */
#define  GPIO_EXTI_INT_DISABLE                  (0x00000000U)     
#define  GPIO_EXTI_INT_ENABLE                   (0x00000001U)

#define  GPIO_EXTI_INT_EDGE                     (0x00000000U)
#define  GPIO_EXTI_INT_LEVEL                    (0x00000001U)

#define  GPIO_EXTI_INT_LOWFALL                  (0x00000000U)  
#define  GPIO_EXTI_INT_HIGHRISE                 (0x00000001U)
#define  GPIO_EXTI_INT_FALLRISE                 (0x00000002U)

/** @defgroup GPIO_alternate_function_selection  GPIO alternate function selection
  */
#define GPIO_AF0_I2C2_SCL                                   (0x00)  
#define GPIO_AF0_I2C2_SDA                                   (0x00)  
#define GPIO_AF0_SPI2_NSS                                   (0x00)  
#define GPIO_AF0_SPI2_SCK                                   (0x00)  
#define GPIO_AF0_SPI2_MISO                                  (0x00)  
#define GPIO_AF0_SPI2_MOSI                                  (0x00)  
#define GPIO_AF0_SPI1_NSS                                   (0x00)
#define GPIO_AF0_SPI1_SCK                                   (0x00)  
#define GPIO_AF0_SPI1_MISO                                  (0x00)  
#define GPIO_AF0_SPI1_MOSI                                  (0x00)
#define GPIO_AF0_I2S_MCLK                                   (0x00)  
#define GPIO_AF0_VC0_OUT                                    (0x00)
#define GPIO_AF0_SWDIO                                      (0x00)
#define GPIO_AF0_SWCLK                                      (0x00)

#define GPIO_AF1_USART2_TXD                                 (0x01)  
#define GPIO_AF1_USART2_RXD                                 (0x01)  
#define GPIO_AF1_USART1_CTS                                 (0x01)  
#define GPIO_AF1_USART1_RTS                                 (0x01)  
#define GPIO_AF1_USART1_TXD                                 (0x01)  
#define GPIO_AF1_USART1_RXD                                 (0x01)  
#define GPIO_AF1_USART1_CK                                  (0x01)  
#define GPIO_AF1_LPUART_TXD                                 (0x01)  
#define GPIO_AF1_LPUART_RXD                                 (0x01)  
#define GPIO_AF1_SPI1_NSS                                   (0x01)  
#define GPIO_AF1_SPI1_SCK                                   (0x01)  
#define GPIO_AF1_SPI1_MISO                                  (0x01)  
#define GPIO_AF1_SPI1_MOSI                                  (0x01)  
#define GPIO_AF1_I2C1_SCL                                   (0x01)  
#define GPIO_AF1_I2C1_SDA                                   (0x01)  
#define GPIO_AF1_IR_OUT                                     (0x01)  

#define GPIO_AF2_LPUART_TXD                                 (0x02)
#define GPIO_AF2_LPUART_RXD                                 (0x02)
#define GPIO_AF2_USART2_CTS                                 (0x02)
#define GPIO_AF2_USART2_RTS                                 (0x02)
#define GPIO_AF2_USART2_TXD                                 (0x02)
#define GPIO_AF2_USART2_RXD                                 (0x02)
#define GPIO_AF2_USART2_CK                                  (0x02)
#define GPIO_AF2_I2C2_SCL                                   (0x02)
#define GPIO_AF2_I2C2_SDA                                   (0x02)
#define GPIO_AF2_USART1_CK                                  (0x02)
#define GPIO_AF2_USART1_TXD                                 (0x02)
#define GPIO_AF2_USART1_RXD                                 (0x02)
#define GPIO_AF2_USART1_CTS                                 (0x02)
#define GPIO_AF2_USART1_RTS                                 (0x02)
#define GPIO_AF2_I2C1_SMBA                                  (0x02)

#define GPIO_AF3_I2C1_SCL                                   (0x03)
#define GPIO_AF3_I2C1_SDA                                   (0x03)
#define GPIO_AF3_TIM16_CH1                                  (0x03)
#define GPIO_AF3_TIM17_CH1                                  (0x03)
#define GPIO_AF3_TIM14_CH1                                  (0x03)
#define GPIO_AF3_TIM15_CH1                                  (0x03)
#define GPIO_AF3_I2C2_SCL                                   (0x03)
#define GPIO_AF3_I2C2_SDA                                   (0x03)
#define GPIO_AF3_VC1_OUT                                    (0x03)

#define GPIO_AF4_TIM1_BKIN                                  (0x04)
#define GPIO_AF4_TIM15_BKIN                                 (0x04)
#define GPIO_AF4_TIM14_CH1                                  (0x04)
#define GPIO_AF4_TIM15_CH1N                                 (0x04)
#define GPIO_AF4_TIM15_CH1                                  (0x04)
#define GPIO_AF4_TIM15_CH2                                  (0x04)
#define GPIO_AF4_TIM14_CH1                                  (0x04)
#define GPIO_AF4_TIM1_CH1N                                  (0x04)
#define GPIO_AF4_TIM1_CH2N                                  (0x04)
#define GPIO_AF4_TIM1_CH3N                                  (0x04)
#define GPIO_AF4_TIM1_CH1                                   (0x04)
#define GPIO_AF4_TIM1_CH2                                   (0x04)
#define GPIO_AF4_TIM1_CH3                                   (0x04)
#define GPIO_AF4_TIM1_CH4                                   (0x04)
#define GPIO_AF4_TIM1_ETR                                   (0x04)
#define GPIO_AF4_TIM3_ETR                                   (0x04)
#define GPIO_AF4_TIM17_BKIN                                 (0x04)
#define GPIO_AF4_TIM16_BKIN                                 (0x04)
#define GPIO_AF4_TIM16_CH1N                                 (0x04)
#define GPIO_AF4_TIM17_CH1N                                 (0x04)
#define GPIO_AF4_TIM16_CH1                                  (0x04)
#define GPIO_AF4_TIM17_CH1                                  (0x04)

#define GPIO_AF5_BEEP                                       (0x05)
#define GPIO_AF5_LPTIM_TOG                                  (0x05)
#define GPIO_AF5_LPTIM_TOGN                                 (0x05)
#define GPIO_AF5_LPTIM_EXT                                  (0x05)
#define GPIO_AF5_TIM3_ETR                                   (0x05)
#define GPIO_AF5_TIM3_CH1                                   (0x05)
#define GPIO_AF5_TIM3_CH2                                   (0x05)
#define GPIO_AF5_TIM3_CH3                                   (0x05)
#define GPIO_AF5_TIM3_CH4                                   (0x05)
#define GPIO_AF5_TIM15_CH1                                  (0x05)
#define GPIO_AF5_TIM15_CH2                                  (0x05)
#define GPIO_AF5_TIM15_BKIN                                 (0x05)
#define GPIO_AF5_TIM17_BKIN                                 (0x05)

#define GPIO_AF6_VC1_OUT                                    (0x06)
#define GPIO_AF6_LVD_OUT                                    (0x06)
#define GPIO_AF6_TIM14_CH1                                  (0x06)
#define GPIO_AF6_LPTIM_GATE                                 (0x06)
#define GPIO_AF6_TIM15_CH1N                                 (0x06)
#define GPIO_AF6_MCO                                        (0x06)
#define GPIO_AF6_VC0_OUT                                    (0x06)
#define GPIO_AF6_BEEP                                       (0x06)
#define GPIO_AF6_IR_OUT                                     (0x06)

#define GPIO_AF7_EVENTOUT                                   (0x07) 




/**
  * @brief  Set GPIO_PIN_x as input
  * @param  __GPIOx__: specifies the GPIO.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __GPIO_PIN__: specifies the pin.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval None.
  */
#define __HAL_GPIO_SET_INPUT(__GPIOx__, __GPIO_PIN__)   (__GPIOx__->DIRCR &= (~(__GPIO_PIN__)))

/**
  * @brief  Set GPIO_PIN_x as output
  * @param  __GPIOx__: specifies the GPIO.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __GPIO_PIN__: specifies the pin.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval None.
  */
#define __HAL_GPIO_SET_OUTPUT(__GPIOx__, __GPIO_PIN__)  (__GPIOx__->DIRCR |= (__GPIO_PIN__))

/**
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  __GPIOx__: specifies the GPIO to check.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __EXTI_LINE__: specifies the EXTI line to check.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval The new state of __EXTI_LINE__ (SET or RESET).
  */
#define __HAL_GPIO_EXTI_GET_FLAG(__GPIOx__, __EXTI_LINE__) (__GPIOx__->MSKINTSR & (__EXTI_LINE__))

/**
  * @brief  Clears the EXTI's line pending flags.
  * @param  __GPIOx__: specifies the GPIO to check.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __EXTI_LINE__: specifies the EXTI line to check.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_FLAG(__GPIOx__, __EXTI_LINE__) (__GPIOx__->INTCLR = (__EXTI_LINE__))

/**
  * @brief  Checks whether the specified EXTI line is asserted or not.
  * @param  __GPIOx__: specifies the GPIO to check.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __EXTI_LINE__: specifies the EXTI line to check.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval The new state of __EXTI_LINE__ (SET or RESET).
  */
#define __HAL_GPIO_EXTI_GET_IT(__GPIOx__, __EXTI_LINE__)    (__GPIOx__->INTEN & (__EXTI_LINE__))

/**
  * @brief  Clears the EXTI's line pending bits.
  * @param  __GPIOx__: specifies the GPIO to check.
                        This parameter can be GPIOx where x can be(A..D)
  * @param  __EXTI_LINE__: specifies the EXTI line to check.
  *          This parameter can be GPIO_PIN_x where x can be(0..7)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_IT(__GPIOx__, __EXTI_LINE__)  (__GPIOx__->INTEN &= (~(__EXTI_LINE__)))


void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void  HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);  
void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#define IS_GPIO_ALL_INSTANCE(INSTANCE)  (((INSTANCE) == GPIOA) || ((INSTANCE) == GPIOB) || \
                                         ((INSTANCE) == GPIOC) || ((INSTANCE) == GPIOD))

#define IS_GPIO_PIN_ACTION(ACTION)      (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))

#define IS_GPIO_PIN(PIN)                ((((PIN) & GPIO_PIN_MASK ) != 0x00U) && (((PIN) & ~GPIO_PIN_MASK) == 0x00U))

#define IS_GPIO_MODE(MODE)              (((MODE) == GPIO_MODE_INPUT) || ((MODE) == GPIO_MODE_OUTPUT) || \
                                         ((MODE) == GPIO_MODE_AF) || ((MODE) == GPIO_MODE_ANALOG))
                            
#define IS_GPIO_PULL(PULL)              (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || ((PULL) == GPIO_PULLDOWN))

/** @defgroup GPIO_Get_Port_Index GPIO Get Port Index
  * @{
  */    
#define GPIO_GET_INDEX(__GPIOx__)       (uint8_t)(((__GPIOx__) == (GPIOA)) ? 0U :\
                                                  ((__GPIOx__) == (GPIOB)) ? 1U :\
                                                  ((__GPIOx__) == (GPIOC)) ? 2U :\
                                                  ((__GPIOx__) == (GPIOD)) ? 3U : 4U)                                      

#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_GPIO_H_ */

