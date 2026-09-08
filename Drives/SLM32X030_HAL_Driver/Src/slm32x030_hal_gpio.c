/**
  ******************************************************************************
  * @file    slm32x030_hal_gpio.c
  * @author  MCU Software Team
  * @Version V1.2.0
  * @Date    8-Nov-2024
  * @brief   GPIO HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the General Purpose Input/Output (GPIO) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + GPIO_MODE_GPIO macro    
  *
  @verbatim
  ==============================================================================
                    ##### GPIO Peripheral features #####
  ==============================================================================
  [..]
  Subject to the specific hardware characteristics of each I/O port listed in the datasheet, each
  port bit of the General Purpose IO (GPIO) Ports, can be individually configured by software
  in several modes:
  (+) Input mode
  (+) Analog mode
  (+) Output mode
  (+) Alternate function mode
  (+) External interrupt lines

  [..]
  During and just after reset, the alternate functions and external interrupt
  lines are not active and the I/O ports are configured in input floating mode.

  [..]
  All GPIO pins have weak internal pull-up and pull-down resistors, which can be
  activated or not.

  [..]
  In Output or Alternate mode, each IO can be configured on open-drain or push-pull
  type and the IO slew rate can be selected.

  [..]
  All ports have external interrupt capability. To use external interrupt
  lines, the port must be configured in input mode.

  [..]
  The external interrupt controller consists of up to 16 edge detectors in devices for 
    generating interrupt requests. Each input line can be independently configured to 
    select the corresponding trigger event (rising or falling or both). Each line can also masked
  independently. A pending register maintains the status line of the interrupt requests

                     ##### How to use this driver #####
  ==============================================================================
 [..]
   (#) Enable the GPIO AHB clock using the following function : 
             __HAL_RCMU_GPIOA_CLK_ENABLE().
             __HAL_RCMU_GPIOB_CLK_ENABLE().
             __HAL_RCMU_GPIOC_CLK_ENABLE().
             __HAL_RCMU_GPIOD_CLK_ENABLE().          
   (#) Configure the GPIO pin(s) using HAL_GPIO_Init().
       (++) Configure the IO mode using "Mode" member from GPIO_InitTypeDef structure
       (++) Activate Pull-up, Pull-down resistor using "Pull" member from GPIO_InitTypeDef
            structure.
       (++) Activate Pushpull, Opendrain using "OpenDrain" member from GPIO_InitTypeDef
            structure.                      
       (++) In case of Output or alternate function mode selection: the SlewRate is
            configured through "SlewRate" member from GPIO_InitTypeDef structure
       (++) Analog mode is required when a pin is to be used as ADC channel.
       (++) In case of external interrupt selection the "Mode" member from
            GPIO_InitTypeDef structure select the corresponding trigger event 
                        (rising or falling or both) through "Exti" member from GPIO_InitTypeDef structure.

   (#) In case of external interrupt mode selection, configure NVIC IRQ priority
       mapped to the EXTI line using HAL_NVIC_SetPriority() and enable it using
       HAL_NVIC_EnableIRQ().

   (#) To get the level of a pin configured in input mode use HAL_GPIO_ReadPin().

   (#) To set/reset the level of a pin configured in output mode use
       HAL_GPIO_WritePin()/HAL_GPIO_TogglePin().

   (#) During and just after reset, the alternate functions are not
       active and the GPIO pins are configured in input floating mode (except SWD
       pins).

   (#) The LXT oscillator pins X32_IN and X32_OUT can be used as general purpose
       (PB5 and PB4, respectively) when the LXT oscillator is off. The LXT has
       priority over the GPIO function.

   (#) The HXT oscillator pins OSC_IN and OSC_OUT can be used as general purpose
       (PA1 and PA2, respectively) when the HXT oscillator is off. The HXT has
       priority over the GPIO function.

  @endverbatim
  ******************************************************************************
 **/
#include "slm32x030_hal.h"

#ifdef HAL_GPIO_MODULE_ENABLED
/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  * @param  GPIOx where x can be (A..D) to select the GPIO peripheral for SLM32X030 device
  * @param  GPIO_Init pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t position;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
    assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
    assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

    /* Configure the port pins */
    for (position = 0U; position < GPIO_NUMBER; position++)
    {
        /* Get the IO position */
        ioposition = (0x01U << position);

        /* Get the current IO position */
        iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

        if (iocurrent == ioposition)
        { 
            GPIOx->MODER = (GPIOx->MODER & ~( 0x3UL << (position*2))) | (GPIO_Init->Mode << (position*2) );        

            switch (GPIO_Init->Mode)
            {
            case GPIO_MODE_INPUT:
                GPIOx->PUPDER = (GPIOx->PUPDER & ~( 0x3UL << (position*2))) | (GPIO_Init->Pull << (position*2) ); 
                break;
                
            case GPIO_MODE_OUTPUT:
                GPIOx->OSPEEDER = (GPIOx->OSPEEDER & ~ioposition) | (GPIO_Init->Speed << position);
                GPIOx->OTYPER = (GPIOx->OTYPER & ~ioposition) | (GPIO_Init->Otype << position);
                GPIOx->SR = (GPIOx->SR & ~ioposition) | (GPIO_Init->DrvStrength << position);    
                break;
            case GPIO_MODE_AF:
                if(position < 8 ) 
                {
                    GPIOx->AFRL = (GPIOx->AFRL & ~(0x0FUL<<(position*4))) | (GPIO_Init->Alternate << (position*4));
                } 
                else 
                {
                    GPIOx->AFRH = (GPIOx->AFRH & ~(0x0FUL<<((position-8)*4))) | (GPIO_Init->Alternate << ((position-8)*4));
                }
                GPIOx->PUPDER = (GPIOx->PUPDER & ~( 0x3UL << (position*2))) | (GPIO_Init->Pull << (position*2) ); 
                GPIOx->OTYPER = (GPIOx->OTYPER & ~ioposition) | (GPIO_Init->Otype << position);
                break;
            case GPIO_MODE_ANALOG:
                break;
            default:
                break;
            }
        }
    }
}

/**
  * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx where x can be (A..D) to select the GPIO peripheral for SLM32X030 device
  * @param  GPIO_Pin specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..7).
  * @retval None
  */
void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
    uint32_t position;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));

    /* Configure the port pins */
    for(position = 0U; position < GPIO_NUMBER; position++)
    {
        /* Get the IO position */
        ioposition = 0x01U << position;
        /* Get the current IO position */
        iocurrent = (GPIO_Pin) & ioposition;

        if(iocurrent == ioposition)
        {
            //
        }
    }
}

/** @defgroup GPIO_Exported_Functions_Group2 IO operation functions
 *  @brief   GPIO Read and Write
 *
@verbatim
 ===============================================================================
                       ##### IO operation functions #####
 ===============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the GPIOs.

@endverbatim
  * @{
  */

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..D depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: specifies the port bit to read.
  *         This parameter can be GPIO_PIN_x where x can be (0..7).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState bitstatus;

    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        bitstatus = GPIO_PIN_SET;
    }
    else
    {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_ODSET register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx: where x can be (A..D depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..7).
  * @param  PinState: specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_PIN_ACTION(PinState));

    if (PinState == GPIO_PIN_SET)
    {
        GPIOx->BSRR |= GPIO_Pin;
    }
    else
    {
        GPIOx->BRR |= GPIO_Pin;
    }
}

/**
  * @brief  Toggles the specified GPIO pin
  * @param  GPIOx: where x can be (A..D depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: Specifies the pins to be toggled.
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->ODR ^= GPIO_Pin;
}

/**
  * @brief  This function handles EXTI interrupt request.
  * @param  GPIOx: where x can be (A..D depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */    
void HAL_GPIO_EXTI_IRQHandler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{   
    /* EXTI line interrupt detected */    
    HAL_GPIO_EXTI_Callback(GPIOx, GPIO_Pin);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIOx: where x can be (A..D depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
__weak void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIOx);
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_GPIO_EXTI_Callback could be implemented in the user file
    */
}

#endif /* HAL_GPIO_MODULE_ENABLED */
