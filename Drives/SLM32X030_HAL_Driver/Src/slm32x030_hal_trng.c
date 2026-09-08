/**
  ******************************************************************************
  * @file    slm32x030_hal_trng.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   RNG HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Random Number Generator (RNG) peripheral:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions 
  *           + Peripheral State functions
  *         
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
      The RNG HAL driver can be used as follows:

      (#) Enable the RNG controller clock using __HAL_RCMU_TRNG_CLK_ENABLE() macro 
          in HAL_TRNG_MspInit().
      (#) Activate the RNG peripheral using HAL_TRNG_Init() function.
      (#) Wait until the 32 bit Random Number Generator contains a valid 
          random data using (polling/interrupt) mode.   
      (#) Get the 32 bit random number using HAL_TRNG_GenerateRandomNumber() function.
  
  @endverbatim
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

#ifdef HAL_TRNG_MODULE_ENABLED

/* Private types -------------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup RNG_Private_Constants
  * @{
  */
#define TRNG_TIMEOUT_VALUE     2U
/**
  * @}
  */ 
/* Private macros ------------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup RNG_Exported_Functions
  * @{
  */

/** @addtogroup RNG_Exported_Functions_Group1
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
 ===============================================================================
          ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the RNG according to the specified parameters 
          in the RNG_InitTypeDef and create the associated handle
      (+) DeInitialize the RNG peripheral
      (+) Initialize the RNG MSP
      (+) DeInitialize RNG MSP 

@endverbatim
  * @{
  */
  
/**
  * @brief  Initializes the RNG peripheral and creates the associated handle.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TRNG_Init(TRNG_HandleTypeDef *htrng)
{ 
    /* Check the RNG handle allocation */
    if(htrng == NULL)
    {
        return HAL_ERROR;
    }

    if(htrng->State == HAL_TRNG_STATE_RESET)
    {  
        /* Allocate lock resource and initialize it */
        htrng->Lock = HAL_UNLOCKED;
        /* Init the low level hardware */
        HAL_TRNG_MspInit(htrng);
    }

    /* Change RNG peripheral state */
    htrng->State = HAL_TRNG_STATE_BUSY;

    /* Enable the RNG Peripheral */
    __HAL_TRNG_ENABLE(htrng);

    /* Initialize the RNG state */
    htrng->State = HAL_TRNG_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  DeInitializes the RNG peripheral. 
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TRNG_DeInit(TRNG_HandleTypeDef *htrng)
{ 
    /* Check the RNG handle allocation */
    if(htrng == NULL)
    {
        return HAL_ERROR;
    }

    /* Disable the RNG Peripheral */
    CLEAR_BIT(htrng->Instance->CR, TRNG_CR_IE | TRNG_CR_TRNGEN);

    /* Clear RNG interrupt status flags */
    CLEAR_BIT(htrng->Instance->SR, TRNG_SR_CEIS | TRNG_SR_SEIS);

    /* DeInit the low level hardware */
    HAL_TRNG_MspDeInit(htrng);

    /* Update the RNG state */
    htrng->State = HAL_TRNG_STATE_RESET; 

    /* Release Lock */
    __HAL_UNLOCK(htrng);

    /* Return the function status */
    return HAL_OK;
}

/**
  * @brief  Initializes the RNG MSP.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval None
  */
__weak void HAL_TRNG_MspInit(TRNG_HandleTypeDef *htrng)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htrng);
    /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_TRNG_MspInit must be implemented in the user file.
    */
}

/**
  * @brief  DeInitializes the RNG MSP.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval None
  */
__weak void HAL_TRNG_MspDeInit(TRNG_HandleTypeDef *htrng)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htrng);
    /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_TRNG_MspDeInit must be implemented in the user file.
    */
}

/**
  * @}
  */

/** @addtogroup RNG_Exported_Functions_Group2
 *  @brief   Peripheral Control functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Get the 32 bit Random number
      (+) Get the 32 bit Random number with interrupt enabled
      (+) Handle RNG interrupt request 

@endverbatim
  * @{
  */
   
/**
  * @brief  Generates a 32-bit random number.
  * @note   Each time the random number data is read the TRNG_FLAG_DRDY flag 
  *         is automatically cleared.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @param  random32bit pointer to generated random number variable if successful.
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber(TRNG_HandleTypeDef *htrng, uint32_t *random32bit)
{
    uint32_t tickstart = 0U;    
    HAL_StatusTypeDef status = HAL_OK;

    /* Process Locked */
    __HAL_LOCK(htrng); 

    /* Check RNG peripheral state */
    if(htrng->State == HAL_TRNG_STATE_READY)
    {
        /* Change RNG peripheral state */  
        htrng->State = HAL_TRNG_STATE_BUSY;  

        /* Get tick */
        tickstart = HAL_GetTick();

        /* Check if data register contains valid random data */
        while(__HAL_TRNG_GET_FLAG(htrng, TRNG_FLAG_DRDY) == RESET)
        {
            if((HAL_GetTick() - tickstart ) > TRNG_TIMEOUT_VALUE)
            {    
                htrng->State = HAL_TRNG_STATE_ERROR;

                /* Process Unlocked */
                __HAL_UNLOCK(htrng);
                
                return HAL_TIMEOUT;
            } 
        }

        /* Get a 32bit Random number */
        htrng->RandomNumber = htrng->Instance->DR;
        *random32bit = htrng->RandomNumber;

        htrng->State = HAL_TRNG_STATE_READY;
    }
    else
    {
        status = HAL_ERROR;
    }

    /* Process Unlocked */
    __HAL_UNLOCK(htrng);

    return status;
}

/**
  * @brief  Generates a 32-bit random number in interrupt mode.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber_IT(TRNG_HandleTypeDef *htrng)
{
    HAL_StatusTypeDef status = HAL_OK;

    /* Process Locked */
    __HAL_LOCK(htrng);

    /* Check RNG peripheral state */
    if(htrng->State == HAL_TRNG_STATE_READY)
    {
        /* Change RNG peripheral state */  
        htrng->State = HAL_TRNG_STATE_BUSY;  

        /* Process Unlocked */
        __HAL_UNLOCK(htrng);

        /* Enable the RNG Interrupts: Data Ready, Clock error, Seed error */ 
        __HAL_TRNG_ENABLE_IT(htrng);
    }
    else
    {
        /* Process Unlocked */
        __HAL_UNLOCK(htrng);

        status = HAL_ERROR;
    }

    return status;
}

/**
  * @brief  Handles RNG interrupt request.
  * @note   In the case of a clock error, the RNG is no more able to generate 
  *         random numbers because the PLL48CLK clock is not correct. app has 
  *         to check that the clock controller is correctly configured to provide
  *         the RNG clock and clear the CEIS bit using __HAL_TRNG_CLEAR_IT(). 
  *         The clock error has no impact on the previously generated 
  *         random numbers, and the RNG_DR register contents can be used.
  * @note   In the case of a seed error, the generation of random numbers is 
  *         interrupted as long as the SECS bit is '1'. If a number is 
  *         available in the RNG_DR register, it must not be used because it may 
  *         not have enough entropy. In this case, it is recommended to clear the 
  *         SEIS bit using __HAL_TRNG_CLEAR_IT(), then disable and enable 
  *         the RNG peripheral to reinitialize and restart the RNG.
  * @note   app-written HAL_TRNG_ErrorCallback() API is called once whether SEIS
  *         or CEIS are set.  
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval None

  */
void HAL_TRNG_IRQHandler(TRNG_HandleTypeDef *htrng)
{
    /* RNG clock error interrupt occurred */
    if((__HAL_TRNG_GET_IT(htrng, TRNG_IT_CEI) != RESET) ||  (__HAL_TRNG_GET_IT(htrng, TRNG_IT_SEI) != RESET))
    { 
        /* Change RNG peripheral state */
        htrng->State = HAL_TRNG_STATE_ERROR;

        HAL_TRNG_ErrorCallback(htrng);

        /* Clear the clock error flag */
        __HAL_TRNG_CLEAR_IT(htrng, TRNG_IT_CEI | TRNG_IT_SEI);
    }

    /* Check RNG data ready interrupt occurred */    
    if(__HAL_TRNG_GET_IT(htrng, TRNG_IT_DRDY) != RESET)
    {
        /* Generate random number once, so disable the IT */
        __HAL_TRNG_DISABLE_IT(htrng);

        /* Get the 32bit Random number (DRDY flag automatically cleared) */ 
        htrng->RandomNumber = htrng->Instance->DR;

        if(htrng->State != HAL_TRNG_STATE_ERROR)
        {
            /* Change RNG peripheral state */
            htrng->State = HAL_TRNG_STATE_READY; 
            
            /* Data Ready callback */ 
            HAL_TRNG_ReadyDataCallback(htrng, htrng->RandomNumber);
        } 
    }
} 

/**
  * @brief  Returns generated random number in polling mode (Obsolete)
  *         Use HAL_TRNG_GenerateRandomNumber() API instead.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval Random value
  */
uint32_t HAL_TRNG_GetRandomNumber(TRNG_HandleTypeDef *htrng)
{
    if(HAL_TRNG_GenerateRandomNumber(htrng, &(htrng->RandomNumber)) == HAL_OK)
    {
        return htrng->RandomNumber; 
    }
    else
    {
        return 0U;
    }
}

/**
  * @brief  Returns a 32-bit random number with interrupt enabled (Obsolete),
  *         Use HAL_TRNG_GenerateRandomNumber_IT() API instead.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval 32-bit random number
  */
uint32_t HAL_TRNG_GetRandomNumber_IT(TRNG_HandleTypeDef *htrng)
{
    uint32_t random32bit = 0U;

    /* Process locked */
    __HAL_LOCK(htrng);

    /* Change RNG peripheral state */  
    htrng->State = HAL_TRNG_STATE_BUSY;  

    /* Get a 32bit Random number */ 
    random32bit = htrng->Instance->DR;

    /* Enable the RNG Interrupts: Data Ready, Clock error, Seed error */ 
    __HAL_TRNG_ENABLE_IT(htrng); 

    /* Return the 32 bit random number */   
    return random32bit;
}

/**
  * @brief  Read latest generated random number. 
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval random value
  */
uint32_t HAL_TRNG_ReadLastRandomNumber(TRNG_HandleTypeDef *htrng)
{
    return(htrng->RandomNumber);
}

/**
  * @brief  Data Ready callback in non-blocking mode. 
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @param  random32bit generated random number.
  * @retval None
  */
__weak void HAL_TRNG_ReadyDataCallback(TRNG_HandleTypeDef *htrng, uint32_t random32bit)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htrng);
    UNUSED(random32bit);
    /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_TRNG_ReadyDataCallback must be implemented in the user file.
    */
}

/**
  * @brief  RNG error callbacks.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval None
  */
__weak void HAL_TRNG_ErrorCallback(TRNG_HandleTypeDef *htrng)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htrng);
    /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_TRNG_ErrorCallback must be implemented in the user file.
    */
}
/**
  * @}
  */ 

  
/** @addtogroup RNG_Exported_Functions_Group3
 *  @brief   Peripheral State functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================  
    [..]
    This subsection permits to get in run-time the status of the peripheral 
    and the data flow.

@endverbatim
  * @{
  */
  
/**
  * @brief  Returns the RNG state.
  * @param  htrng pointer to a TRNG_HandleTypeDef structure that contains
  *                the configuration information for RNG.
  * @retval HAL state
  */
HAL_TRNG_StateTypeDef HAL_TRNG_GetState(TRNG_HandleTypeDef *htrng)
{
    return htrng->State;
}

/**
  * @}
  */
  
/**
  * @}
  */

#endif /* HAL_TRNG_MODULE_ENABLED */
