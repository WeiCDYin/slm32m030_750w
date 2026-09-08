/**
  ******************************************************************************
  * @file    slm32x030_hal_vc.c
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   VC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Voltage comparator (VC) peripheral:
  *           + Initialization and Start functions
  *           + IO operation functions
  *
  @verbatim
  ==============================================================================
                    ##### VC Generic features #####
  ==============================================================================
  [..]
    (+) The VC can be started by  software (configurable through option byte).

    (+) The VC Filter is clocked by Low-Speed clock (LIRC)or PCLK .

                     ##### How to use this driver #####
  ==============================================================================
  [..]
    (#) Use VC using HAL_VC_Init() function to :
      (++) Enable VC 25DIV in VC_CR0 register. 
      (++) Select input source for "+" and "-"
      (++) Configure the VC filter set.
      (++) set triger and interrupt.
    

     *** VC HAL driver macros list ***
     ====================================
   @endverbatim
  ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

#ifdef HAL_VC_MODULE_ENABLED
/** @defgroup VC VC
  * @brief VC HAL module driver.
  * @{
  */


/** @addtogroup VC_Exported_Functions
  * @{
  */

/** @addtogroup VC_Exported_Functions_Group1
  *  @brief    Initialization and Start functions.
  *
@verbatim
 ===============================================================================
          ##### Initialization and Start functions #####
 ===============================================================================
 [..]  This section provides functions allowing to:
      (+) Initialize the VC according to the specified parameters in the
          VC_InitTypeDef of associated handle.
      (+) Once initialization is performed in HAL_VC_Init function, 
          the Voltage comparator will work.

@endverbatim
  * @{
  */
/**
  * @brief  Initialize the VC according to the specified parameters in the
  *         VC_HandleTypeDef and start VC. 
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_VC_Init(VC_HandleTypeDef *hvc)
{
    uint32_t regValue;
    /* Check the VC handle allocation */
    if (hvc == NULL)
    {
        return HAL_ERROR;
    }
    if(hvc->State == HAL_VC_STATE_RESET)
    {
        /* Init the low level hardware : GPIO, CLOCK... */
        HAL_VC_MspInit(hvc);
    }
    /* Check the parameters */
    assert_param(IS_VC_INSTANCE(hvc->Instance));
  
    regValue = 0;
    if(hvc->Init.VC0Enable == HAL_VC_ENABLE)
    {
        assert_param(IS_VC0_INT_STATE(hvc->Init.VC0INT_Enable));
        regValue = (hvc->Init.RefSel << VCCR_VREF_RSEL_Pos) | (hvc->Init.VrefSel << VCCR_VREFSEL_Pos)  | (hvc->Init.VC0HysSel << VCCR_VC0_HYS_SEL_Pos) | \
                    (hvc->Init.VC0PinSel << VCCR_VC0_P_SEL_Pos) | (hvc->Init.VC0NinSel << VCCR_VC0_N_SEL_Pos); 
    }
    if(hvc->Init.VC0Filter_Clk == HAL_VC_FILTER_CLK_LSI ||hvc->Init.VC1Filter_Clk == HAL_VC_FILTER_CLK_LSI)
    {
        SET_BIT(RCMU->CSR, RCMU_CSR_LSION);
    }
    if(hvc->Init.VC1Enable == HAL_VC_ENABLE)
    {
        assert_param(IS_VC1_INT_STATE(hvc->Init.VC1INT_Enable));
        regValue = (hvc->Init.RefSel << VCCR_VREF_RSEL_Pos) | (hvc->Init.VrefSel << VCCR_VREFSEL_Pos)  | (hvc->Init.VC1HysSel << VCCR_VC1_HYS_SEL_Pos) | (hvc->Init.VC1PinSel << VCCR_VC1_P_SEL_Pos) | (hvc->Init.VC1NinSel << VCCR_VC1_N_SEL_Pos); 
    }   
    
    if((hvc->Init.VC0Enable == HAL_VC_ENABLE) ||(hvc->Init.VC1Enable == HAL_VC_ENABLE))
    {
        WRITE_REG(hvc->Instance->VCCR,regValue);
    }
    
    /* write VC0CR */
    if(hvc->Init.VC0Enable == HAL_VC_ENABLE)
    {
        assert_param(IS_VC_INT_MODE(hvc->Init.VC0INT_Mode));
        assert_param(IS_VC_POLARITY_TYPE(hvc->Init.VC0OutPolarity));
        assert_param(IS_VC_OUTPUT_TYPE(hvc->Init.VC0OutType));      
        assert_param(IS_VC_FILTER_STATE(hvc->Init.VC0Filter_Enable));
        assert_param(IS_VC_FILTER_NUM(hvc->Init.VC0Fliter_Num));
        assert_param(IS_VC_FILTER_CLK(hvc->Init.VC0Filter_Clk));
  
        hvc->State = HAL_VC_STATE_BUSY;
        regValue = hvc->Init.VC0INT_Enable |hvc->Init.VC0INT_Mode | hvc->Init.VC0OutPolarity | hvc->Init.VC0OutType | hvc->Init.VC0Fliter_Num | hvc->Init.VC0Filter_Enable | hvc->Init.VC0Filter_Clk;
        WRITE_REG(hvc->Instance->VC0CR,regValue);
    }

    /* write VC1CR */
    if(hvc->Init.VC1Enable == HAL_VC_ENABLE)
    {
        if(hvc->State == HAL_VC_STATE_RESET)
        {
            /* Init the low level hardware : GPIO, CLOCK... */
            HAL_VC_MspInit(hvc);
        }
  
        hvc->State = HAL_VC_STATE_BUSY;
        assert_param(IS_VC_INT_MODE(hvc->Init.VC1INT_Mode));
        assert_param(IS_VC_POLARITY_TYPE(hvc->Init.VC1OutPolarity));
        assert_param(IS_VC_OUTPUT_TYPE(hvc->Init.VC1OutType));      
        assert_param(IS_VC_FILTER_STATE(hvc->Init.VC1Filter_Enable));
        assert_param(IS_VC_FILTER_NUM(hvc->Init.VC1Fliter_Num));
        assert_param(IS_VC_FILTER_CLK(hvc->Init.VC1Filter_Clk));

        regValue = hvc->Init.VC1INT_Enable |hvc->Init.VC1INT_Mode | hvc->Init.VC1OutPolarity | hvc->Init.VC1OutType | hvc->Init.VC1Fliter_Num | hvc->Init.VC1Filter_Enable | hvc->Init.VC1Filter_Clk;
        WRITE_REG(hvc->Instance->VC1CR,regValue);
    }

    HAL_VC_Enable(hvc);
    hvc->State = HAL_VC_STATE_READY;
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  DE Initialize the VC according to the specified parameters in the
  *         VC_HandleTypeDef . 
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC module.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_VC_DeInit(VC_HandleTypeDef *hvc)
{
    /* Check the parameters */
    assert_param(IS_VC_INSTANCE(hvc->Instance));

    hvc->State = HAL_VC_STATE_BUSY;
    
  
    CLEAR_REG(hvc->Instance->VCCR);
    CLEAR_REG(hvc->Instance->VCSR);
    CLEAR_REG(hvc->Instance->VC0CR);
    CLEAR_REG(hvc->Instance->VC1CR);

    /* DeInit the low level hardware: CLOCK, NVIC */
    HAL_VC_MspDeInit(hvc);

    /* Change LVD state */
    hvc->State = HAL_VC_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(hvc);

    return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup VC_Exported_Functions_Group2
  *  @brief   IO operation functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
 [..]  This section provides functions allowing to:
      (+) VC OUTPUT Config.

@endverbatim
  * @{
  */
/**
  * @}
  */


/** @addtogroup VC_Exported_Functions_Group3
  *  @brief    Get/Clear Status functions.
  *
   * @{
  */
/**
  * @brief  Checks whether the interrupt VC flag is set or reset.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC module.
  * @retval The new state of VC0 interrupt FLAG (SET or RESET).
  */
FlagStatus HAL_VC0_Get_INT_Flag(VC_HandleTypeDef *hvc)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    if (((hvc->Instance->VCSR & VCSR_VC0INT) >> VCSR_VC0INT_Pos) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the flag status */
    return bitstatus;
}

/**
  * @brief  Checks whether the interrupt VC flag is set or reset.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC module.
  * @retval The new state of VC interrupt FLAG (SET or RESET).
  */
FlagStatus HAL_VC1_Get_INT_Flag(VC_HandleTypeDef *hvc)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    if (((hvc->Instance->VCSR & VCSR_VC1INT) >> VCSR_VC1INT_Pos) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the flag status */
    return bitstatus;
}

/**
  * @brief  Clears interrupt flag.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC0 module.
  * @retval None
  */
void HAL_VC0_Clear_INT_Flag(VC_HandleTypeDef *hvc)
{   
    CLEAR_BIT(hvc->Instance->VCSR, VCSR_VC0INT);
}
/**
  * @brief  Clears interrupt flag.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC1 module.
  * @retval None
  */
void HAL_VC1_Clear_INT_Flag(VC_HandleTypeDef *hvc)
{   
    CLEAR_BIT(hvc->Instance->VCSR, VCSR_VC1INT);
}

/**
  * @brief  Checks VC0 output is set or reset.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC0 module.
  * @retval The new state of VC0_OUTPUT (SET or RESET).
  */
FlagStatus HAL_VC0_Get_OutPut_State(VC_HandleTypeDef *hvc)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    if (((hvc->Instance->VCSR & VCSR_VC0OUT) >> VCSR_VC0OUT_Pos) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the vc output status */
    return bitstatus;
}

/**
  * @brief  Checks VC1 output is set or reset.
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC1 module.
  * @retval The new state of VC0_OUTPUT (SET or RESET).
  */
FlagStatus HAL_VC1_Get_OutPut_State(VC_HandleTypeDef *hvc)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    if (((hvc->Instance->VCSR & VCSR_VC1OUT) >> VCSR_VC1OUT_Pos)!= (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the vc output status */
    return bitstatus;
}

/**
  * @brief  Enable VC (write access to VC_CR1 registers).
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC module.
  * @retval None
  */
void HAL_VC_Enable(VC_HandleTypeDef *hvc)
{
    if(hvc->Init.VC0Enable == HAL_VC_ENABLE)
    {       
        SET_BIT(hvc->Instance->VC0CR, VC0CR_VC0_EN);
    }
    if(hvc->Init.VC1Enable == HAL_VC_ENABLE)
    {
        SET_BIT(hvc->Instance->VC1CR, VC1CR_VC1_EN);        
    }
}

/**
  * @brief  Disables VC (write access to VC0CR registers).
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC0 module.
  * @retval None
  */
void HAL_VC0_Disable(VC_HandleTypeDef *hvc)
{
    CLEAR_BIT(hvc->Instance->VC0CR, VC0CR_VC0_EN);
}

/**
  * @brief  Disables VC (write access to VC1CR registers).
  * @param  hvc  pointer to a VC_HandleTypeDef structure that contains
  *                the configuration information for the specified VC1 module.
  * @retval None
  */
void HAL_VC1_Disable(VC_HandleTypeDef *hvc)
{
    CLEAR_BIT(hvc->Instance->VC1CR, VC1CR_VC1_EN);      
}

/**
  * @}
  */

/**
  * @brief  Initialize the VC MSP.
  * @param  hvc: pointer to a VC_HandleTypeDef structure that contains
  *               the configuration information for VC module.
  * @retval None
  */
__weak void HAL_VC_MspInit(VC_HandleTypeDef *hvc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hvc);
    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_VC_MspInit should be implemented in the user file
    */
}

/**
  * @brief  DeInitializes VC MSP.
  * @param  hvc : VC handle
  * @retval None
  */
__weak void HAL_VC_MspDeInit(VC_HandleTypeDef *hvc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hvc);
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_VC_MspDeInit could be implemented in the user file
    */
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_VC_MODULE_ENABLED */

