/**
  ******************************************************************************
  * @file    slm32fx030_hal_adc.c
  * @author  MCU Software Team
  * @Version V1.2.0
  * @Date    14-Nov-2020
  * @brief   SYSCFG HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Voltage comparator (SYSCFG) peripheral:
  *           + Initialization and Start functions
  *           + IO operation functions
  *
  @verbatim
  ==============================================================================
                    ##### SYSCFG Generic features #####
  ==============================================================================
  [..]
    (+) The SYSCFG can be started by  software (configurable through option byte).

    (+) The SYSCFG Filter is clocked by Low-Speed clock (LIRC)or PCLK .

   @endverbatim
  ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

#ifdef HAL_SYSCFG_MODULE_ENABLED
/** @defgroup SYSCFG SYSCFG
  * @brief SYSCFG HAL module driver.
  * @{
  */


/** @addtogroup SYSCFG_Exported_Functions
  * @{
  */

/** @addtogroup SYSCFG_Exported_Functions_Group1
  *  @brief    Initialization and Start functions.
  *
@verbatim
 ===============================================================================
          ##### Initialization and Start functions #####
 ===============================================================================
 [..]  This section provides functions allowing to:
      (+) Initialize the SYSCFG according to the specified parameters in the
          SYSCFG_InitTypeDef of associated handle.
      (+) Once initialization is performed in HAL_SYSCFG_Init function, 
          the Voltage comparator will work.

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the SYSCFG MSP.
  * @param  hsyscfg : SYSCFG handle
  * @retval None
  */
__weak void HAL_SYSCFG_IR_OUT_MspInit(SYSCFG_IR_HanderTypeDef *hsyscfg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsyscfg);
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSCFG_MspInit could be implemented in the user file
    */
}

/**
  * @brief  DeInitializes SYSCFG MSP.
  * @param  hsyscfg : SYSCFG handle
  * @retval None
  */
__weak void HAL_SYSCFG_IR_OUT_MspDeInit(SYSCFG_IR_HanderTypeDef *hsyscfg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsyscfg);
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSCFG_MspInit could be implemented in the user file
    */
}

/**
  * @brief  Initializes the SYSCFG Unit according to the specified
  *         parameters in the SYSCFG_IR_HanderTypeDef and create the associated handle.
  * @note no comment
  * @param  hsyscfg : SYSCFG handle
  *         InitCfgPtr : SYSCFG_IR_InitTypeDef
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SYSCFG_Ircr_Init(SYSCFG_IR_HanderTypeDef* hsyscfg)
{
    assert_param(IS_SYSCFG_ALL_INSTANCE(hsyscfg->Instance));

    unsigned int ircr = 0;
    HAL_StatusTypeDef ret = HAL_OK;
    
    __HAL_RCMU_SYSCFG_CLK_ENABLE();
    ircr =  (hsyscfg->Init.ir_signal_sel << SYSCFG_IRCR_SIG_SEL_Pos);
    ircr += (hsyscfg->Init.ir_carrier_sel << SYSCFG_IRCR_CAR_SEL_Pos);
    ircr += (hsyscfg->Init.ir_signal_inv << SYSCFG_IRCR_SIG_INV_Pos);
    ircr += (hsyscfg->Init.ir_carrier_inv << SYSCFG_IRCR_CAR_INV_Pos);
    hsyscfg->Instance->IRCR = ircr;

    if (hsyscfg->State == HAL_SYSCFG_STATE_RESET)
    {
    /* Init the low level hardware: CLOCK, NVIC */
        HAL_SYSCFG_IR_OUT_MspInit(hsyscfg);
    }

    /* Initialize the SYSCFG state */
    hsyscfg->State = HAL_SYSCFG_STATE_READY;

    return ret;
}

#endif /* HAL_SYSCFG_MODULE_ENABLED */
