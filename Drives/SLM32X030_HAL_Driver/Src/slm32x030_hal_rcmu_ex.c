/**
  ******************************************************************************
  * @file    slm32x030_hal_rcmu_ex.c    
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024 
  * @brief   Extended RCMU HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities RCMU extension peripheral:
  *           + Extended Peripheral Control functions
  *  
  ******************************************************************************
 **/ 

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

#ifdef HAL_RCMU_MODULE_ENABLED

/** @defgroup RCMUx RCMUx
  * @brief RCMU Extension HAL module driver.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCMUx_Private_Constants RCMUx Private Constants
  * @{
  */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup RCMUx_Private_Macros RCMUx Private Macros
  * @{
  */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCMUx_Exported_Functions RCMUx Exported Functions
  * @{
  */

/** @defgroup RCMUx_Exported_Functions_Group1 Peripheral Control functions 
  *  @brief  Extended Peripheral Control functions  
  *
@verbatim   
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the RCMU Clocks 
    frequencies.
    [..] 
    (@) Important note: Care must be taken when HAL_RCMUEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in  
        order to modify the RTC Clock source, as consequence RTC registers (including 
        the backup registers) are set to their reset values.
      
@endverbatim
  * @{
  */

/**
  * @brief  Initializes the RCMU extended peripherals clocks according to the specified parameters in the
  *         RCMU_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCMU_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals clocks(RTC clock).
  *
  * @note   Care must be taken when HAL_RCMUEx_PeriphCLKConfig() is used to select 
  *         the RTC clock source; in this case the Backup domain will be reset in  
  *         order to modify the RTC Clock source, as consequence RTC registers (including 
  *         the backup registers) are set to their reset values.
  *
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCMUEx_PeriphCLKConfig(RCMU_PeriphCLKInitTypeDef  *PeriphClkInit)
{
    uint32_t tickstart = 0U, temp_reg = 0U;

    /*------------------------------- RTC/LCD Configuration ------------------------*/ 
    if ((((PeriphClkInit->PeriphClockSelection) & RCMU_PERIPHCLK_RTC) == RCMU_PERIPHCLK_RTC))
    {

        FlagStatus       pwrclkchanged = RESET;

        /* As soon as function is called to change RTC clock source, activation of the 
            power domain is done. */
        /* Requires to enable write access to Backup Domain of necessary */
        if(__HAL_RCMU_PMU_CLK_DISABLE())
        {
            __HAL_RCMU_PMU_CLK_ENABLE();
            pwrclkchanged = SET;
        }

        if(HAL_IS_BIT_CLR(PMU->CR, PMU_CR_DBP))
        {
            /* Enable write access to Backup domain */
            SET_BIT(PMU->CR, PMU_CR_DBP);
            
            /* Wait for Backup domain Write protection disable */
            tickstart = HAL_GetTick();
            
            while(HAL_IS_BIT_CLR(PMU->CR, PMU_CR_DBP))
            {
                if((HAL_GetTick() - tickstart) > RCMU_DBP_TIMEOUT_VALUE)
                {
                    return HAL_TIMEOUT;
                }
            }
        }
            
        /* Reset the Backup domain only if the RTC Clock source selection is modified from reset value */ 
        temp_reg = (RCMU->BDCR & RCMU_BDCR_RTCSEL);
        if((temp_reg != 0x00000000U) && (temp_reg != (PeriphClkInit->RTCClockSelection & RCMU_BDCR_RTCSEL)))
        {
            /* Store the content of BDCR register before the reset of Backup Domain */
            temp_reg = (RCMU->BDCR & ~(RCMU_BDCR_RTCSEL));
            /* RTC Clock selection can be changed only if the Backup Domain is reset */
            __HAL_RCMU_BACKUPRESET_FORCE();
            __HAL_RCMU_BACKUPRESET_RELEASE();
            /* Restore the Content of BDCR register */
            RCMU->BDCR = temp_reg;

            /* Wait for LSERDY if LSE was enabled */
            if (HAL_IS_BIT_SET(temp_reg, RCMU_BDCR_LSEON))
            {
                /* Get Start Tick */
                tickstart = HAL_GetTick();
                
                /* Wait till LSE is ready */  
                while(__HAL_RCMU_GET_REG_FLAG(RCMU->BDCR, RCMU_BDCR_LSERDY, RCMU_BDCR_LSERDY_Pos) == RESET)
                {
                    if((HAL_GetTick() - tickstart) > RCMU_LSE_TIMEOUT_VALUE)
                    {
                        return HAL_TIMEOUT;
                    }
                }
            }
        }

        __HAL_RCMU_RTC_CONFIG(PeriphClkInit->RTCClockSelection); 

        /* Require to disable power clock if necessary */
        if(pwrclkchanged == SET)
        {
            __HAL_RCMU_PMU_CLK_DISABLE();
        }
    }

    /*------------------------------ ADC clock Configuration ------------------*/ 
    if(((PeriphClkInit->PeriphClockSelection) & RCMU_PERIPHCLK_ADC) == RCMU_PERIPHCLK_ADC)
    {
        /* Configure the ADC clock source */
        __HAL_RCMU_ADC_CONFIG(PeriphClkInit->AdcClockSelection);
    }

    return HAL_OK;
}

/**
  * @brief  Get the PeriphClkInit according to the internal
  * RCMU configuration registers.
  * @param  PeriphClkInit pointer to an RCMU_PeriphCLKInitTypeDef structure that 
  *         returns the configuration information for the Extended Peripherals clocks(RTC, I2S, ADC clocks).
  * @retval None
  */
void HAL_RCMUEx_GetPeriphCLKConfig(RCMU_PeriphCLKInitTypeDef  *PeriphClkInit)
{
    uint32_t srcclk = 0U;

    /* Set all possible values for the extended clock type parameter------------*/
    PeriphClkInit->PeriphClockSelection = RCMU_PERIPHCLK_RTC;

    /* Get the RTC configuration -----------------------------------------------*/
    srcclk = __HAL_RCMU_GET_RTC_SOURCE();
    /* Source clock is LSE or LSI*/
    PeriphClkInit->RTCClockSelection = srcclk;

    /* Get the ADC clock configuration -----------------------------------------*/
    PeriphClkInit->PeriphClockSelection |= RCMU_PERIPHCLK_ADC;
    PeriphClkInit->AdcClockSelection = __HAL_RCMU_GET_ADC_SOURCE();
}

/**
  * @brief  Returns the peripheral clock frequency
  * @note   Returns 0 if peripheral clock is unknown
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_PERIPHCLK_RTC  RTC peripheral clock
  *            @arg @ref RCMU_PERIPHCLK_ADC  ADC peripheral clock
  @endif
  * @retval Frequency in Hz (0: means that no available frequency for the peripheral)
  */
uint32_t HAL_RCMUEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
    uint32_t temp_reg = 0U, frequency = 0U;

    /* Check the parameters */

    switch (PeriphClk)
    {
        case RCMU_PERIPHCLK_RTC:
            /* Get RCMU BDCR configuration ------------------------------------------------------*/
            temp_reg = RCMU->BDCR;
            /* Check if LSE is ready if RTC clock selection is LSE */
            if (((temp_reg & RCMU_BDCR_RTCSEL) == RCMU_RTCCLKSOURCE_LSE) && (HAL_IS_BIT_SET(temp_reg, RCMU_BDCR_LSERDY)))
            {
                frequency = LSE_VALUE;
            }
            /* Check if LSI is ready if RTC clock selection is LSI */
            else if (((temp_reg & RCMU_BDCR_RTCSEL) == RCMU_RTCCLKSOURCE_LSI) && (HAL_IS_BIT_SET(RCMU->CSR, RCMU_CSR_LSIRDY)))
            {
                frequency = LSI_VALUE;
            }
            else if (((temp_reg & RCMU_BDCR_RTCSEL) == RCMU_RTCCLKSOURCE_HSE_DIV128) && (HAL_IS_BIT_SET(RCMU->CR, RCMU_CR_HSERDY)))
            {
                frequency = HSE_VALUE / 128U;
            }
            /* Clock not enabled for RTC*/
            else
            {
                frequency = 0U;
            }
            break;
        
        case RCMU_PERIPHCLK_ADC:
            frequency = HAL_RCMU_GetPCLK2Freq() / (((__HAL_RCMU_GET_ADC_SOURCE() >> RCMU_CFGR_PPRE2_Pos) + 1));
            break;
        
        default:
            break;        
    }
    return(frequency);
}

#endif /* HAL_RCMU_MODULE_ENABLED */
