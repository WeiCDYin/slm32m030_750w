/**
  ******************************************************************************
  * @file    slm32x030_hal_spi_ex.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   Extended SPI HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities SPI extension peripheral:
  *           + Extended Peripheral Control functions
  *  
  ******************************************************************************
 **/ 

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */
#ifdef HAL_SPI_MODULE_ENABLED

/** @defgroup SPI_Private_Variables SPI Private Variables
  * @{
  */
#if (USE_SPI_CRC != 0U)
/* Variable used to determine if device is impacted by implementation of workaround
   related to wrong CRC errors detection on SPI2. Conditions in which this workaround has to be applied, are:
    - Revision ID : Z
    - SPI2
    - In receive only mode, with CRC calculation enabled, at the end of the CRC reception,
      the software needs to check the CRCERR flag. If it is found set, read back the SPI_RXCRC:
        + If the value is 0, the complete data transfer is successful.
        + Otherwise, one or more errors have been detected during the data transfer by CPU or DMA.
      If CRCERR is found reset, the complete data transfer is considered successful.
*/
uint8_t uCRCErrorWorkaroundCheck = 0U;
#endif /* USE_SPI_CRC */
/**
  * @}
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup SPI_Exported_Functions
  * @{
  */

/** @addtogroup SPI_Exported_Functions_Group1
  *
  * @{
  */

/**
  * @brief  Initializes the SPI according to the specified parameters 
  *         in the SPI_InitTypeDef and create the associated handle.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
    /* Check the SPI handle allocation */
    if(hspi == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
    assert_param(IS_SPI_MODE(hspi->Init.Mode));
    assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));
    assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));
    assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));
    assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));
    assert_param(IS_SPI_NSS(hspi->Init.NSS));
    assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
    assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));

#if (USE_SPI_CRC != 0U)
    assert_param(IS_SPI_CRC_CALCULATION(hspi->Init.CRCCalculation));
    if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
        assert_param(IS_SPI_CRC_POLYNOMIAL(hspi->Init.CRCPolynomial));
    }
#else
    hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
#endif /* USE_SPI_CRC */

    if(hspi->State == HAL_SPI_STATE_RESET)
    {
        /* Init the low level hardware : GPIO, CLOCK, NVIC... */
        HAL_SPI_MspInit(hspi);
    }

    hspi->State = HAL_SPI_STATE_BUSY;

    /* Disble the selected SPI peripheral */
    __HAL_SPI_DISABLE(hspi);

    /*----------------------- SPIx CR1 & CR2 Configuration ---------------------*/
    /* Configure : SPI Mode, Communication Mode, Data size, Clock polarity and phase, NSS management,
    Communication speed, First bit and CRC calculation state */
    WRITE_REG(hspi->Instance->CR1, (hspi->Init.Mode | hspi->Init.Direction | hspi->Init.DataSize |
                                    hspi->Init.CLKPolarity | hspi->Init.CLKPhase | (hspi->Init.NSS & SPI_CR1_SSM) |
                                    hspi->Init.BaudRatePrescaler | hspi->Init.FirstBit  | hspi->Init.CRCCalculation) );

    /* Configure : NSS management */
    WRITE_REG(hspi->Instance->CR2, (((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE)));

    /*---------------------------- SPIx CRCPOLY Configuration ------------------*/
    /* Configure : CRC Polynomial */
    WRITE_REG(hspi->Instance->CRCPR, hspi->Init.CRCPolynomial);

#if defined(SPI_I2SCFGR_I2SMOD)
    /* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
    CLEAR_BIT(hspi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);
#endif /* SPI_I2SCFGR_I2SMOD */

#if (USE_SPI_CRC != 0U)
    uCRCErrorWorkaroundCheck = 0U;
#endif /* USE_SPI_CRC */

    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    hspi->State = HAL_SPI_STATE_READY;

    return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SPI_Private_Functions
  * @{
  */
#if (USE_SPI_CRC != 0U)
/**
  * @brief  Checks if encountered CRC error could be corresponding to wrongly detected errors 
  *         according to SPI instance, Device type, and revision ID.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval CRC error validity (SPI_INVALID_CRC_ERROR or SPI_VALID_CRC_ERROR).  
*/
uint8_t SPI_ISCRCErrorValid(SPI_HandleTypeDef *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    return (SPI_VALID_CRC_ERROR);
}
#endif /* USE_SPI_CRC */

/**
  * @}
  */

#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */
