/**
 ******************************************************************************
 * @file    slm32x030_hal_exti.c
 * @author  SOC Team
 * @version V0.0.1
 * @date    8-11-2024
 * @brief
 ******************************************************************************
 **/
/*****************************************************************************************************
|    Other Header File Inclusion                                               
*****************************************************************************************************/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
 * @{
 */

/** @addtogroup EXTI
 * @{
 */
/** MISRA C:2012 deviation rule has been granted for following rule:
 * Rule-18.1_b - Medium: Array `EXTICR' 1st subscript interval [0,7] may be out
 * of bounds [0,3] in following API :
 * HAL_EXTI_SetConfigLine
 * HAL_EXTI_GetConfigLine
 * HAL_EXTI_ClearConfigLine
 */

#ifdef HAL_EXTI_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup EXTI_Private_Constants EXTI Private Constants
 * @{
 */

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
#define HAL_EXTI_GPIO_PORT_MASK                                         (0xFFFFul)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup EXTI_Exported_Functions
 * @{
 */

/** @addtogroup EXTI_Exported_Functions_Group1
  *  @brief    Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */
/**
 * @brief  Set configuration of a dedicated Exti line.
 * @param  hexti: Exti handle.
 * @retval HAL Status.
 */
HAL_StatusTypeDef HAL_EXTI_Init(EXTI_HandleTypeDef *hexti)
{
    uint32_t linepos;
    uint32_t maskline;
    uint8_t byte_index;
    uint8_t bit_index;
    
    if (NULL == hexti)
    {
        return HAL_ERROR;
    }
    
    /* Check the parameter */
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));
    assert_param(IS_EXTI_LINE(hexti->Init.Line));
    assert_param(IS_EXTI_MODE(hexti->Init.Mode));
    assert_param(IS_EXTI_TRIGGER(hexti->Init.Trigger));
    assert_param(IS_EXTI_GPIO_PORT(hexti->Init.PortSel));
    
    /* Low layer Init */
    HAL_EXTI_MspInit(hexti);

    /* Compute line mask */
    hexti->Line = hexti->Init.Line;
    linepos = (hexti->Line & EXTI_PIN_MASK);
    maskline = (1ul << linepos);

    /* Configure triggers for configurable lines */
    if ((hexti->Line & EXTI_PIN_MASK) <= 0xFu)
    {
        /* Configure rising trigger */
        /* Mask or set line */
        if ((hexti->Init.Trigger & EXTI_TRIGGER_RISING) != 0u)
        {
            hexti->Instance->RTSR |= maskline;
        }
        else
        {
            hexti->Instance->RTSR &= ~maskline;
        }

        /* Configure falling trigger */
        /* Mask or set line */
        if ((hexti->Init.Trigger & EXTI_TRIGGER_FALLING) != 0u)
        {
            hexti->Instance->FTSR |= maskline;
        }
        else
        {
            hexti->Instance->FTSR &= ~maskline;
        }

        /* Clear bit field and set value */
        if (hexti->Line <= EXTI_LINE_23_USART2_WAKEUP)
        {
            byte_index = hexti->Line / 4u;
            bit_index = hexti->Line % 4u;
            
            SYSCFG->EXTICR[byte_index] &= ~(HAL_EXTI_GPIO_PORT_MASK << (bit_index << 2u));
            SYSCFG->EXTICR[byte_index] |= (hexti->Init.PortSel << (bit_index << 2u));
        }
        
        /* Mask or unmask interrupt or event */
        if ((hexti->Init.Mode & EXTI_MODE_INTERRUPT) != 0u)
        {
            hexti->Instance->IMR |= maskline;
        }
        else
        {
            hexti->Instance->IMR &= ~maskline;
        }

        if ((hexti->Init.Mode & EXTI_MODE_EVENT) != 0u)
        {
            hexti->Instance->EMR |= maskline;
        }
        else
        {
            hexti->Instance->EMR &= ~maskline;
        }
        
        /* Clear potential pending bit */
        hexti->Instance->PR = maskline;
    }
    return HAL_OK;
}

/**
 * @brief  Clear whole configuration of a dedicated Exti line.
 * @param  hexti Exti handle.
 * @retval HAL Status.
 */
HAL_StatusTypeDef HAL_EXTI_DeInit(EXTI_HandleTypeDef *hexti)
{
    uint32_t linepos;
    uint32_t maskline;
    uint32_t temp;

    /* Check null pointer */
    if (hexti == NULL)
    {
        return HAL_ERROR;
    }
    
    /* Check the parameter */
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));

    /* Low layer Deinit */
    HAL_EXTI_MspDeInit(hexti);

    /* compute line mask */
    linepos = (hexti->Line & EXTI_PIN_MASK);
    maskline = (1ul << linepos);

    /* 1] Clear interrupt mode */
    hexti->Instance->IMR &= ~maskline;

    /* 2] Clear event mode */
    hexti->Instance->EMR &= ~maskline;

    /* 3] Clear triggers in case of configurable lines */
    if (hexti->Line <= EXTI_LINE_23_USART2_WAKEUP)
    {
        hexti->Instance->RTSR &= ~maskline;
        hexti->Instance->FTSR &= ~maskline;

        /* Clear Gpio port selection for gpio lines */
        if (hexti->Line < EXTI_LINE_16_LVD_OUTPUT)
        {
            temp = SYSCFG->EXTICR[linepos >> 2u];
            temp &= (0x0Ful << ((linepos & 0x03u) << 2u));
            SYSCFG->EXTICR[linepos >> 2u] = temp;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Mask the interrupt or event request of specified line.
 * @param  hexti Exti handle.
 * @param  mode referrence to EXTI_Mode.
 * @retval HAL Status.
 */
HAL_StatusTypeDef HAL_EXTI_ConfigLineMask(EXTI_HandleTypeDef *hexti, uint8_t mode)
{
    uint32_t linepos;
    uint32_t maskline;

    /* Check null pointer */
    if (hexti == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameter */
    assert_param(IS_EXTI_LINE(hexti->Line));
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));
    UNUSED(mode);
    
    /* compute line mask */
    linepos = (hexti->Line & EXTI_PIN_MASK);
    maskline = (1ul << linepos);

    if (EXTI_MODE_INTERRUPT == mode)
    {
        /* 1] Clear interrupt mode */
        hexti->Instance->IMR = (hexti->Instance->IMR & ~maskline);
    }
    else
    {
        /* 2] Clear event mode */
        hexti->Instance->EMR = (hexti->Instance->EMR & ~maskline);
    }

    return HAL_OK;
}

/**
 * @brief  Unmask the interrupt or event request of specified line.
 * @param  hexti Exti handle.
 * @param  mode referrence to EXTI_Mode.
 * @retval HAL Status.
 */
HAL_StatusTypeDef HAL_EXTI_ConfigLineClearMask(EXTI_HandleTypeDef *hexti, uint8_t mode)
{
    uint32_t linepos;
    uint32_t maskline;

    /* Check null pointer */
    if (hexti == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameter */
    assert_param(IS_EXTI_LINE(hexti->Line));
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));
    UNUSED(mode);

    /* compute line mask */
    linepos = (hexti->Line & EXTI_PIN_MASK);
    maskline = (1ul << linepos);

    if (EXTI_MODE_INTERRUPT == mode)
    {
        /* 1] Clear interrupt mode */
        hexti->Instance->IMR = (hexti->Instance->IMR | maskline);
    }
    else
    {
        /* 2] Clear event mode */
        hexti->Instance->EMR = (hexti->Instance->EMR | maskline);
    }

    return HAL_OK;
}

/**
 * @brief  Register callback for a dedicated Exti line.
 * @param  hexti Exti handle.
 * @param  CallbackID User callback identifier.
 *         This parameter can be one of @arg @ref EXTI_CallbackIDTypeDef values.
 * @param  pPendingCbfn function pointer to be stored as callback.
 * @retval HAL Status.
 */
HAL_StatusTypeDef HAL_EXTI_RegisterCallback(EXTI_HandleTypeDef *hexti, EXTI_CallbackIDTypeDef CallbackID, void (*pPendingCbfn)(void))
{
    HAL_StatusTypeDef status = HAL_OK;

    switch (CallbackID)
    {
        case  HAL_EXTI_COMMON_CB_ID:
            hexti->PendingCallback = pPendingCbfn;
            break;

        default:
            status = HAL_ERROR;
            break;
    }

    return status;
}

/**
 * @}
 */

/** @addtogroup EXTI_Exported_Functions_Group2
  *  @brief EXTI IO functions.
  *
@verbatim
 ===============================================================================
                       ##### IO operation functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
 * @brief  Handle EXTI interrupt request.
 * @param  hexti Exti handle.
 * @retval none.
 */
void HAL_EXTI_IRQHandler(EXTI_HandleTypeDef *hexti)
{
    uint32_t regval;
    uint32_t maskline;

    assert_param(IS_EXTI_INSTANCE(hexti->Instance));

    /* Compute line mask */
    maskline = (1ul << (hexti->Line & EXTI_PIN_MASK));

    /* Get pending bit  */
    regval = (hexti->Instance->PR & maskline);
    if (regval != 0x00u)
    {
        /* Clear pending bit and Software bit*/
        hexti->Instance->PR = maskline;

        /* Call callback */
        if (hexti->PendingCallback != NULL)
        {
            /* Call callback */
            hexti->PendingCallback();
        }
    }
}

/**
 * @brief  Get interrupt pending bit of a dedicated line.
 * @param  hexti Exti handle.
 * @param  Edge Specify which pending edge as to be checked.
 *         This parameter can be one of the following values:
 *           @arg @ref EXTI_TRIGGER_RISING_FALLING
 *         This parameter is kept for compatibility with other series.
 * @retval 1 if interrupt is pending else 0.
 */
uint32_t HAL_EXTI_GetPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)
{
    uint32_t regval;
    uint32_t maskline;
    uint32_t linepos;

    /* Check parameters */
    assert_param(IS_EXTI_LINE(hexti->Line));
    assert_param(IS_EXTI_PENDING_EDGE(Edge));
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));
    UNUSED(Edge);

    /* Compute line mask */
    linepos = (hexti->Line & EXTI_PIN_MASK);
    maskline = (1ul << linepos);

    /* return 1 if bit is set else 0 */
    regval = ((hexti->Instance->PR & maskline) >> linepos);
    return regval;
}

/**
 * @brief  Clear interrupt pending bit of a dedicated line.
 * @param  hexti Exti handle.
 * @param  Edge Specify which pending edge as to be clear.
 *         This parameter can be one of the following values:
 *           @arg @ref EXTI_TRIGGER_RISING_FALLING
 *         This parameter is kept for compatibility with other series.
 * @retval None.
 */
void HAL_EXTI_ClearPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)
{
    uint32_t maskline;

    /* Check parameters */
    assert_param(IS_EXTI_LINE(hexti->Line));
    assert_param(IS_EXTI_PENDING_EDGE(Edge));
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));
    UNUSED(Edge);

    /* Compute line mask */
    maskline = (1ul << (hexti->Line & EXTI_PIN_MASK));

    /* Clear Pending bit */
    hexti->Instance->PR = maskline;
}


/**
 * @brief  Generate a software interrupt for a dedicated line.
 * @param  hexti Exti handle.
 * @retval None.
 */
void HAL_EXTI_GenerateSWI(EXTI_HandleTypeDef *hexti)
{
    uint32_t maskline;

    /* Check parameters */
    assert_param(IS_EXTI_LINE(hexti->Line));
    assert_param(IS_EXTI_INSTANCE(hexti->Instance));

    /* Compute line mask */
    maskline = (1ul << (hexti->Line & EXTI_PIN_MASK));

    /* Generate Software interrupt */
    hexti->Instance->SWIER = maskline;
}

/**
 * @brief  Initializes the EXTI MSP.
 * @param  hexti: exti handle
 * @retval None
 */
__weak void HAL_EXTI_MspInit(EXTI_HandleTypeDef *hexti)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hexti);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_EXTI_MspInit must be implemented in the user file.
     */
}

/**
 * @brief  DeInitializes the EXTI MSP.
 * @param  hexti: exti handle
 * @retval None
 */
__weak void HAL_EXTI_MspDeInit(EXTI_HandleTypeDef *hexti)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hexti);
    /* NOTE : This function should not be modified. When the callback is needed,
              function HAL_EXTI_MspDeInit must be implemented in the user file.
     */
}

/**
 * @}
 */


/**
 * @}
 */

#endif /* HAL_EXTI_MODULE_ENABLED */
/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT SoulSemi *****END OF FILE****/
