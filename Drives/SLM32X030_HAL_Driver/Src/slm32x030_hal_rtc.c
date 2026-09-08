/**
  ******************************************************************************
  * @file    slm32x030_hal_rtc.c
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   RTC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Real Time Clock (RTC) peripheral:
  *           + Initialization and de-initialization functions
  *           + RTC Time and Date functions
  *           + RTC Alarm functions
  *           + Peripheral Control functions
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                  ##### How to use this driver #####
  ==================================================================
  [..]
    (+) Enable the RTC domain access (see description in the section above).
    (+) Configure the RTC Prescaler (Asynchronous prescaler to generate RTC 1Hz time base)
        using the HAL_RTC_Init() function.

  *** Time and Date configuration ***
  ===================================
  [..]
    (+) To configure the RTC Calendar (Time and Date) use the HAL_RTC_SetTime()
        and HAL_RTC_SetDate() functions.
    (+) To read the RTC Calendar, use the HAL_RTC_GetTime_Date() functions.

  *** Alarm configuration ***
  ===========================
  [..]
    (+) To configure the RTC Alarm use the HAL_RTC_Alarm1_SetTime() and  HAL_RTC_Alarm1_SetDate()function.
    (+) To read the RTC Alarm, use the HAL_RTC_Alarm1_GetTime() and HAL_RTC_Alarm1_GetDate() function.


                      ##### Backup Domain Operating Condition #####
  ==============================================================================
  [..] The real-time clock (RTC) and the RTC backup registers can be powered
       from the VBAT voltage when the main VDD supply is powered off.
       To retain the content of the RTC backup registers and supply the RTC
       when VDD is turned off, VBAT pin can be connected to an optional
       standby voltage supplied by a battery or by another source.

  [..] To allow the RTC operating even when the main digital supply (VDD) is turned
       off, the VBAT pin powers the following blocks:
    (+) The RTC
    (+) The LSE oscillator


                  ##### RTC and low power modes #####
  ==================================================================
  [..] The MCU can be woken up from a low power mode by an RTC alternate
       function.
  [..] The RTC alternate functions are the RTC alarms (Alarm 1),
       and RTC tamper event detection.
       These RTC alternate functions can wake up the system from the Stop and
       Standby low power modes.
  [..] The system can also wake up from low power modes without depending
       on an external interrupt (Auto-wakeup mode), by using the RTC alarm.

   @endverbatim
  ******************************************************************************
 **/

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"
/** @addtogroup SLM32X030_HAL_Driver
 * @{
 */

/** @defgroup RTC RTC
 * @brief RTC HAL module driver
 * @{
 */

#ifdef HAL_RTC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RTC_Private_Constants RTC Private Constants
 * @{
 */
#define RTC_READ_TIMEOUT 0x08U
#define RTC_TIMEOUT_VALUE 0X10000U
/**
 * @}
 */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup RTC_Private_Functions RTC Private Functions
 * @{
 */
static void RTC_Disable_Write_Protected(RTC_HandleTypeDef *hrtc);
static HAL_StatusTypeDef RTC_Enter_Init_Mode(RTC_HandleTypeDef *hrtc);
static HAL_StatusTypeDef RTC_Exit_Init_Mode(RTC_HandleTypeDef *hrtc);

/**
 * @}
 */

/* Private functions ---------------------------------------------------------*/
/** @defgroup RTC_Exported_Functions RTC Exported Functions
 * @{
 */

/** @defgroup RTC_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
 @endverbatim
  * @{
  */

/**
 * @brief  Initializes the RTC peripheral
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc)
{
    /* Disable write protection */
    __HAL_RTC_DISABLE_WRITE_PROTECT();

    /* Check input parameters */
    if (hrtc == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the parameters */
    assert_param(IS_RTC_ALL_INSTANCE(hrtc->Instance));
    assert_param(IS_RTC_CLOCK_SOURCE(hrtc->Init.ClockSource));
    assert_param(IS_RTC_HOUR_FORMAT(hrtc->Init.HourFormat));
    assert_param(IS_RTC_TIME_ADJUST_MODE(hrtc->Init.TimeAdjustMode));
    assert_param(IS_RTC_TIME_TRIM(hrtc->Init.TimeTrim));

    if (hrtc->State == HAL_RTC_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hrtc->Lock = HAL_UNLOCKED;
        /* Initialize RTC MSP */
        HAL_RTC_MspInit(hrtc);
        SET_BIT(PMU->CR, PMU_CR_DBP);
    }

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_BUSY;

    /* Clear Flags Bits */
    CLEAR_REG(hrtc->Instance->INTCLR);
    CLEAR_REG(hrtc->Instance->ISR);

    /* Select a clock source */
    RCMU->BDCR = ((RCMU->BDCR & (~RCMU_BDCR_RTCSEL)) | hrtc->Init.ClockSource);

    /* Enable the RTCEN of the RCMU_BDCR */
    SET_BIT(RCMU->BDCR, RCMU_BDCR_RTCEN);

    /* set hour format Bits */
    if (hrtc->Init.HourFormat != RTC_HOURFORMAT_12)
    {
        SET_BIT(hrtc->Instance->CR, RTC_HOURFORMAT_24);
    }
    else
    {
        CLEAR_BIT(hrtc->Instance->CR, RTC_HOURFORMAT_24);
    }

    /* RTC counter was enabled */
    __HAL_RTC_ENABLE(hrtc);

    /* Enable write protection */
    __HAL_RTC_ENABLE_WRITE_PROTECT();

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  DeInitializes the RTC peripheral
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @note   This function does not reset the RTC Backup Data registers.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_DeInit(RTC_HandleTypeDef *hrtc)
{
    /* Check input parameters */
    if (hrtc == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_RTC_ALL_INSTANCE(hrtc->Instance));

    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_BUSY;

    RTC_Disable_Write_Protected(hrtc);
    CLEAR_REG(hrtc->Instance->CR);
    CLEAR_REG(hrtc->Instance->TIME);
    CLEAR_REG(hrtc->Instance->DATE);
    CLEAR_REG(hrtc->Instance->ALM1TIME);
    CLEAR_REG(hrtc->Instance->ALM1DATE);
    CLEAR_REG(hrtc->Instance->ALM2PRD);
    CLEAR_REG(hrtc->Instance->CLKCALR);
    CLEAR_REG(hrtc->Instance->INTCLR);
    CLEAR_REG(hrtc->Instance->TSTR);
    CLEAR_REG(hrtc->Instance->TSDR);
    CLEAR_REG(hrtc->Instance->TAFCR);

    CLEAR_BIT(hrtc->Instance->ISR, (RTC_ISR_RSF | RTC_ISR_ALM1_F) | RTC_ISR_ALM2_F);

    /* De-Initialize RTC MSP */
    HAL_RTC_MspDeInit(hrtc);

    hrtc->State = HAL_RTC_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(hrtc);

    CLEAR_BIT(PMU->CR, PMU_CR_DBP);

    return HAL_OK;
}

/**
 * @brief  Initializes the RTC MSP.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_RTC_MspInit could be implemented in the user file
     */
}

/**
 * @brief  DeInitializes the RTC MSP.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_RTC_MspDeInit could be implemented in the user file
     */
}

/**
 * @}
 */

/** @defgroup RTC_Exported_Functions_Group2 Time and Date functions
 *  @brief   RTC Time and Date functions
 *
@verbatim
 ===============================================================================
                 ##### RTC Time and Date functions #####
 ===============================================================================

 [..] This section provides functions allowing to configure Time and Date features

@endverbatim
  * @{
  */

/**
 * @brief  Sets RTC current time.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sTime: Pointer to Time structure
 * @param  Format: Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *            @arg RTC_HOURFORMAT_12: 12 hour system
 *            @arg RTC_HOURFORMAT_24: 24 hour system
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Hour_Format)
{
    volatile uint32_t time_reg = 0;
    uint32_t temp;

    /* Check input parameters */
    if ((hrtc == NULL) || (sTime == NULL))
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_FORMAT(Hour_Format));

    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR24(sTime->Hours));
    assert_param(IS_RTC_MINUTES(sTime->Minutes));
    assert_param(IS_RTC_SECONDS(sTime->Seconds));

    if (Hour_Format != hrtc->Init.HourFormat)
    {
        hrtc->Init.HourFormat = Hour_Format;
        RTC_Disable_Write_Protected(hrtc);
        if (hrtc->Init.HourFormat != RTC_HOURFORMAT_12)
        {
            SET_BIT(hrtc->Instance->CR, RTC_HOURFORMAT_24);
        }
        else
        {
            CLEAR_BIT(hrtc->Instance->CR, RTC_HOURFORMAT_24);
        }
    }

    if (Hour_Format == RTC_HOURFORMAT_12)
    {
        if (sTime->Hours > 12)
        {
            time_reg |= ((sTime->Hours - 12) / 10 << RTC_TIME_HOURT_Pos) | (((sTime->Hours - 12) % 10) << RTC_TIME_HOURU_Pos);
        }
        else
        {
            time_reg |= ((sTime->Hours) / 10 << RTC_TIME_HOURT_Pos) | (((sTime->Hours) % 10) << RTC_TIME_HOURU_Pos);
        }
        time_reg |= RTC_HOURFORMAT_12;
    }
    else
    {
        time_reg |= ((sTime->Hours) / 10 << RTC_TIME_HOURT_Pos) | (((sTime->Hours) % 10) << RTC_TIME_HOURU_Pos);
    }

    time_reg |= sTime->AmPM << RTC_TIME_PM_Pos;

    time_reg |= ((sTime->Minutes) / 10 << RTC_TIME_MINT_Pos) | (((sTime->Minutes) % 10) << RTC_TIME_MINU_Pos);
    time_reg |= ((sTime->Seconds) / 10 << RTC_TIME_SECT_Pos) | (((sTime->Seconds) % 10) << RTC_TIME_SECU_Pos);
    time_reg |= sTime->WeekDay << RTC_TIME_WEEK_Pos;
    temp = hrtc->Instance->TIME & RTC_TIME_WEEK;
    time_reg |= temp;

    /* Set Initialization mode */
    if (RTC_Enter_Init_Mode(hrtc) != HAL_OK)
    {
        /* Set RTC state */
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Release Lock */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }

    RTC_Disable_Write_Protected(hrtc);
    hrtc->Instance->TIME = time_reg;

    /* Enable write protection */
    WRITE_REG(hrtc->Instance->WPR, 0XFF);

    /* Exit read/writ mode in RTC registers */
    if (RTC_Exit_Init_Mode(hrtc) != HAL_OK)
    {
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }

    hrtc->State = HAL_RTC_STATE_READY;
    __HAL_UNLOCK(hrtc);
    return HAL_OK;
}

/**
 * @brief  Sets RTC current date.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sDate: Pointer to date structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate)
{
    uint32_t years = 0U, data_reg = 0;

    /* Check input parameters */
    if ((hrtc == NULL) || (sDate == NULL))
    {
        return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;

    /* Check the parameters */
    assert_param(IS_RTC_YEAR(sDate->Year));
    assert_param(IS_RTC_MONTH(sDate->Month));
    assert_param(IS_RTC_DATE(sDate->Date));
    assert_param(IS_RTC_WEEKDAY(sDate->WeekDay));

    /* Change the current date */
    hrtc->DateToUpdate.Year = sDate->Year;
    hrtc->DateToUpdate.Month = sDate->Month;
    hrtc->DateToUpdate.Date = sDate->Date;

    /* Read the time counter*/
    if (sDate->Year >= 2000)
    {
        years = sDate->Year - 2000;
        data_reg |= RTC_DATE_CEN;
    }
    else
    {
        years = sDate->Year - 1900;
        data_reg &= (~RTC_DATE_CEN);
    }

    data_reg |= ((years) / 10 << RTC_DATE_YEART_Pos) | (((years) % 10) << RTC_DATE_YEARU_Pos);
    data_reg |= ((sDate->Month) / 10 << RTC_DATE_MONT_Pos) | (((sDate->Month) % 10) << RTC_DATE_MONU_Pos);
    data_reg |= ((sDate->Date) / 10 << RTC_DATE_DAYT_Pos) | (((sDate->Date) % 10) << RTC_DATE_DAYU_Pos);

    /* Set Initialization mode */
    if (RTC_Enter_Init_Mode(hrtc) != HAL_OK)
    {
        /* Set RTC state */
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Release Lock */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }
    RTC_Disable_Write_Protected(hrtc);
    hrtc->Instance->DATE = data_reg;

    /* Enable write protection */
    WRITE_REG(hrtc->Instance->WPR, 0XFF);

    if (RTC_Exit_Init_Mode(hrtc) != HAL_OK)
    {
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }

    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @brief  Gets RTC current time and date.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sTime: Pointer to Time structure
 * @param  sDate: Pointer to Date structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_GetTime_Date(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
    uint32_t counter_times = 0U, time1_reg = 0U, date1_reg = 0U, time2_reg = 0U, date2_reg = 0U;
    uint32_t reg_val0, reg_val1;
    HAL_StatusTypeDef read_ok = HAL_ERROR;
    /* Check input parameters */
    if ((hrtc == NULL) || (sTime == NULL))
    {
        return HAL_ERROR;
    }
    /* Check if Bypass Shadow register */
    if ((READ_BIT(hrtc->Instance->CR, RTC_CR_BYPSHAD) >> RTC_CR_BYPSHAD_Pos) != RESET)
    {
        counter_times = RTC_READ_TIMEOUT;
        do
        {
            time1_reg = hrtc->Instance->TIME;
            date1_reg = hrtc->Instance->DATE;
            time2_reg = hrtc->Instance->TIME;
            date2_reg = hrtc->Instance->DATE;

            if ((time1_reg == time2_reg) && (date1_reg == date2_reg))
            {
                read_ok = HAL_OK;
            }
        } while ((read_ok != HAL_OK) && (counter_times-- > 0));

        if (read_ok != HAL_OK)
        {
            /* Set RTC state */
            hrtc->State = HAL_RTC_STATE_ERROR;

            /* Release Lock */
            __HAL_UNLOCK(hrtc);

            return HAL_ERROR;
        }
    }
    else
    {
        while (RTC_Get_RSF_Flag() == RESET);
        time1_reg = hrtc->Instance->TIME;
        date1_reg = hrtc->Instance->DATE;

        RTC_Clear_RSF_Flag();
    }
    /* Fill the structure fields with the read parameters */
    reg_val0 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_HOURT);
    reg_val1 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_HOURU);
    sTime->Hours = (reg_val0 >> RTC_TIME_HOURT_Pos) * 10 + (reg_val1 >> RTC_TIME_HOURU_Pos);

    reg_val0 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_MINT);
    reg_val1 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_MINU);
    sTime->Minutes = (reg_val0 >> RTC_TIME_MINT_Pos) * 10 + (reg_val1 >> RTC_TIME_MINU_Pos);

    reg_val0 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_SECT);
    reg_val1 = READ_BIT(hrtc->Instance->TIME, RTC_TIME_SECU);
    sTime->Seconds = (reg_val0 >> RTC_TIME_SECT_Pos) * 10 + (reg_val1 >> RTC_TIME_SECU_Pos);

    sTime->WeekDay = (READ_BIT(hrtc->Instance->TIME, RTC_TIME_WEEK) >> RTC_TIME_WEEK_Pos);
    sTime->AmPM = (READ_BIT(hrtc->Instance->TIME, RTC_TIME_PM) >> RTC_TIME_PM_Pos);

    sDate->CEN = (READ_BIT(hrtc->Instance->DATE, RTC_DATE_CEN) >> RTC_DATE_CEN_Pos);
    
    reg_val0 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_MONT);
    reg_val1 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_MONU);
    sDate->Month = (reg_val0 >> RTC_DATE_MONT_Pos) * 10 + (reg_val1 >> RTC_DATE_MONU_Pos);

    reg_val0 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_DAYT);
    reg_val1 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_DAYU);
    sDate->Date = (reg_val0 >> RTC_DATE_DAYT_Pos) * 10 + (reg_val1 >> RTC_DATE_DAYU_Pos);

    reg_val0 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_YEART);
    reg_val1 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_YEARU);
    sDate->Year = (reg_val0 >> RTC_DATE_YEART_Pos) * 10 + (reg_val1 >> RTC_DATE_YEARU_Pos);

    reg_val0 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_YEART);
    reg_val1 = READ_BIT(hrtc->Instance->DATE, RTC_DATE_YEARU);
    if (sDate->CEN == HAL_RTC_YEAR_1900)
    {
        sDate->Year = 1900 + (reg_val0 >> RTC_DATE_YEART_Pos) * 10 + (reg_val1 >> RTC_DATE_YEARU_Pos);
    }
    else if ((sDate->CEN << RTC_DATE_CEN_Pos) == HAL_RTC_YEAR_2000)
    {
        sDate->Year = 2000 + (reg_val0 >> RTC_DATE_YEART_Pos) * 10 + (reg_val1 >> RTC_DATE_YEARU_Pos);
    }

    /* Update date */
    hrtc->DateToUpdate.Date = sDate->Date;
    hrtc->DateToUpdate.Month = sDate->Month;
    hrtc->DateToUpdate.Year = sDate->Year;
    hrtc->DateToUpdate.CEN = sDate->CEN;

    return HAL_OK;
}

/**
 * @brief  Sets RTC Alarm1 time.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sTime: Pointer to Time structure
 * @retval HAL status
 */

HAL_StatusTypeDef HAL_RTC_Alarm1_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime)
{
    uint32_t time_reg = 0;
    uint32_t alm1data_reg = 0;
    /* Check input parameters */
    if ((hrtc == NULL) || (sTime == NULL))
    {
        return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;
    /* Check the parameters */
    assert_param(IS_RTC_HOUR24(sTime->Hours));
    assert_param(IS_RTC_MINUTES(sTime->Minutes));
    assert_param(IS_RTC_SECONDS(sTime->Seconds));
    assert_param(IS_RTC_WEEKDAY(sTime->WeekDay));

    time_reg = hrtc->Instance->ALM1TIME;

    if (hrtc->Init.HourFormat == RTC_HOURFORMAT_12)
    {
        if (sTime->Hours > 12)
        {
            time_reg = (time_reg & (~(RTC_ALM1TIME_ALMHOURT | RTC_ALM1TIME_ALMHOURU))) | (((sTime->Hours - 12) / 10 << RTC_ALM1TIME_ALMHOURT_Pos) | (((sTime->Hours - 12) % 10) << RTC_ALM1TIME_ALMHOURU_Pos));
        }
        else
        {
            time_reg = (time_reg & (~(RTC_ALM1TIME_ALMHOURT | RTC_ALM1TIME_ALMHOURU))) | (((sTime->Hours) / 10 << RTC_ALM1TIME_ALMHOURT_Pos) | (((sTime->Hours) % 10) << RTC_ALM1TIME_ALMHOURU_Pos));
        }
        time_reg |= RTC_HOURFORMAT_12;
    }
    else
    {
        time_reg = (time_reg & (~(RTC_ALM1TIME_ALMHOURT | RTC_ALM1TIME_ALMHOURU))) | (((sTime->Hours) / 10 << RTC_ALM1TIME_ALMHOURT_Pos) | (((sTime->Hours) % 10) << RTC_ALM1TIME_ALMHOURU_Pos));
        time_reg |= RTC_HOURFORMAT_12;
    }

    alm1data_reg = hrtc->Instance->ALM1DATE;
    alm1data_reg |= RTC_ALM1DATE_ALMHOUREN;

    if (sTime->Hours > 12)
    {
        time_reg |= RTC_ALM1TIME_ALMPM;
    }

    time_reg = (time_reg & (~(RTC_ALM1TIME_ALMMINT | RTC_ALM1TIME_ALMMINU))) | (((sTime->Minutes) / 10 << RTC_ALM1TIME_ALMMINT_Pos) | (((sTime->Minutes) % 10) << RTC_ALM1TIME_ALMMINU_Pos));
    alm1data_reg |= RTC_ALM1DATE_ALMMINEN;

    time_reg = (time_reg & (~(RTC_ALM1TIME_ALMSECT | RTC_ALM1TIME_ALMSECU))) | (((sTime->Seconds) / 10 << RTC_ALM1TIME_ALMSECT_Pos) | (((sTime->Seconds) % 10) << RTC_ALM1TIME_ALMSECU_Pos));
    alm1data_reg |= RTC_ALM1DATE_ALMSECEN;

    time_reg |= sTime->WeekDay << RTC_ALM1TIME_ALMWEEK_Pos;
    alm1data_reg |= RTC_ALM1DATE_ALMWEEKEN;

    /* Set Initialization mode */
    if (RTC_Enter_Init_Mode(hrtc) != HAL_OK)
    {
        /* Set RTC state */
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Release Lock */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }

    RTC->ALM1DATE &= (~((RTC_ALM1DATE_ALMSECEN) | (RTC_ALM1DATE_ALMMINEN) | (RTC_ALM1DATE_ALMHOUREN) | (RTC_ALM1DATE_ALMWEEKEN)));
    RTC_Disable_Write_Protected(hrtc);
    CLEAR_REG(RTC->ALM1TIME);

    WRITE_REG(hrtc->Instance->ALM1TIME, time_reg);

    CLEAR_BIT(RTC->ALM1TIME, RTC_ALM1TIME_ALMPM);

    WRITE_REG(hrtc->Instance->ALM1DATE, alm1data_reg);

    /* Enable write protection */
    WRITE_REG(hrtc->Instance->WPR, 0XFF);

    /* Exit read/writ mode in RTC registers */
    if (RTC_Exit_Init_Mode(hrtc) != HAL_OK)
    {
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }

    hrtc->State = HAL_RTC_STATE_READY;

    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @brief  Sets RTC Alarm1 date.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sDate: Pointer to date structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm1_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate)
{
    uint32_t data_reg = 0;
    uint32_t years = 0U;

    /* Check input parameters */
    if ((hrtc == NULL) || (sDate == NULL))
    {
        return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;
    /* Check the parameters */
    assert_param(IS_RTC_YEAR(sDate->Year));
    assert_param(IS_RTC_MONTH(sDate->Month));
    assert_param(IS_RTC_DATE(sDate->Date));

    data_reg = hrtc->Instance->ALM1DATE;

    /* Read the time counter*/
    if (sDate->Year >= 2000)
    {
        years = sDate->Year - 2000;
        data_reg |= HAL_RTC_YEAR_2000;
    }
    else
    {
        years = sDate->Year - 1900;
        data_reg |= HAL_RTC_YEAR_1900;
    }

    data_reg |= (data_reg & (~(RTC_ALM1DATE_ALMYEART | RTC_ALM1DATE_ALMYEARU))) | (((years) / 10 << RTC_ALM1DATE_ALMYEART_Pos) | (((years) % 10) << RTC_ALM1DATE_ALMYEARU_Pos));
    data_reg |= RTC_ALM1DATE_ALMYEAREN;

    data_reg |= (data_reg & (~(RTC_ALM1DATE_ALMMONT | RTC_ALM1DATE_ALMMONU))) | (((sDate->Month) / 10 << RTC_ALM1DATE_ALMMONT_Pos) | (((sDate->Month) % 10) << RTC_ALM1DATE_ALMMONU_Pos));
    data_reg |= RTC_ALM1DATE_ALMMONEN;

    data_reg |= (data_reg & (~(RTC_ALM1DATE_DAYT | RTC_ALM1DATE_DAYU))) | (((sDate->Date) / 10 << RTC_ALM1DATE_DAYT_Pos) | (((sDate->Date) % 10) << RTC_DATE_DAYU_Pos));
    data_reg |= RTC_ALM1DATE_ALMDAYEN;

    /* Set Initialization mode */
    if (RTC_Enter_Init_Mode(hrtc) != HAL_OK)
    {
        /* Set RTC state */
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Release Lock */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }
    RTC_Disable_Write_Protected(hrtc);
    CLEAR_REG(RTC->ALM1DATE);
    WRITE_REG(hrtc->Instance->ALM1DATE, data_reg);

    /* Enable write protection */
    WRITE_REG(hrtc->Instance->WPR, 0XFF);

    if (RTC_Exit_Init_Mode(hrtc) != HAL_OK)
    {
        hrtc->State = HAL_RTC_STATE_ERROR;
        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);

        return HAL_ERROR;
    }
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @brief  Gets RTC Alarm1 time
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sTime: Pointer to Time structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm1_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime)
{
    uint32_t reg_val1, reg_val2;
    
    /* Check input parameters */
    if ((hrtc == NULL) || (sTime == NULL))
    {
        return HAL_ERROR;
    }
    
    sTime->AmPM = (READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMPM) >> RTC_ALM1TIME_ALMPM_Pos);

    reg_val1 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMHOURT);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMHOURU);
    sTime->Hours = (reg_val1 >> RTC_ALM1TIME_ALMHOURT_Pos) * 10 + (reg_val2 >> RTC_ALM1TIME_ALMHOURU_Pos);

    reg_val1 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMMINT);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMMINU);
    sTime->Minutes = (reg_val1 >> RTC_ALM1TIME_ALMMINT_Pos) * 10 + (reg_val2 >> RTC_ALM1TIME_ALMMINU_Pos);

    reg_val1 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMSECT);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMSECU);
    sTime->Seconds = (reg_val1 >> RTC_ALM1TIME_ALMSECT_Pos) * 10 + (reg_val2 >> RTC_ALM1TIME_ALMSECU_Pos);

    sTime->WeekDay = (READ_BIT(hrtc->Instance->ALM1TIME, RTC_ALM1TIME_ALMWEEK) >> RTC_ALM1TIME_ALMWEEK_Pos);

    return HAL_OK;
}

/**
 * @brief  Gets RTC Alarm1 date
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  sDate: Pointer to Date structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm1_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate)
{
    uint32_t date_reg = 0U;
    uint32_t reg_val1, reg_val2;
    /* Check input parameters */
    if ((hrtc == NULL) || (sDate == NULL))
    {
        return HAL_ERROR;
    }

    /* Fill the structure fields with the read parameters */
    reg_val1 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_ALMYEART);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_ALMYEARU);
    if (((date_reg & RTC_DATE_CEN) >> RTC_DATE_CEN_Pos) != RESET)
    {
        sDate->Year = 2100 + (reg_val1 >> RTC_ALM1DATE_ALMYEART_Pos) * 10 + (reg_val2 >> RTC_ALM1DATE_ALMYEARU_Pos);
    }
    else
    {
        sDate->Year = 2000 + (reg_val1 >> RTC_ALM1DATE_ALMYEART_Pos) * 10 + (reg_val2 >> RTC_ALM1DATE_ALMYEARU_Pos);
    }

    reg_val1 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_ALMMONT);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_ALMMONU);
    sDate->Month = (reg_val1 >> RTC_ALM1DATE_ALMMONT_Pos) * 10 + (reg_val2 >> RTC_ALM1DATE_ALMMONU_Pos);

    reg_val1 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_DAYT);
    reg_val2 = READ_BIT(hrtc->Instance->ALM1DATE, RTC_ALM1DATE_DAYU);
    sDate->Date = (reg_val1 >> RTC_ALM1DATE_DAYT_Pos) * 10 + (reg_val2 >> RTC_ALM1DATE_DAYU_Pos);

    return HAL_OK;
}
/**
 * @brief  Active or Deactive the specified RTC Alarm interrupt
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  Alarm: Specifies the Alarm.
 *          This parameter can be one of the following values:
 *            @arg RTC_ALARM_1:  Alarm1
 *           @arg RTC_ALARM_2:  Alarm2
 * @param  NewState: the Alarm enable  or disable.
 *          This parameter can be one of the following values:
 *            @arg DISABLE
 *            @arg ENABLE
 * @retval HAL status
 */

HAL_StatusTypeDef HAL_RTC_INT_Config(RTC_HandleTypeDef *hrtc, uint32_t Alarm, FunctionalState NewState)
{
    UNUSED(Alarm);

    /* Check the parameters */
    assert_param(IS_RTC_INT_TYPE(Alarm));

    RTC_Disable_Write_Protected(hrtc);
    if (Alarm != RTC_ALARM_2)
    {
        if (NewState != DISABLE)
        {
            SET_BIT(hrtc->Instance->CR, RTC_CR_ALM1_INTEN);
        }
        else
        {
            CLEAR_BIT(hrtc->Instance->CR, RTC_CR_ALM1_INTEN);
        }
    }
    else
    {
        if (NewState != DISABLE)
        {
            SET_BIT(hrtc->Instance->CR, RTC_CR_ALM2_INTEN);
        }
        else
        {
            CLEAR_BIT(hrtc->Instance->CR, RTC_CR_ALM2_INTEN);
        }
    }
    return HAL_OK;
}

/**
 * @brief  Active or Deactive the specified RTC Alarm1
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  NewState: the Alarm enable  or disable.
 *          This parameter can be one of the following values:
 *            @arg DISABLE
 *            @arg ENABLE
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm1_Config(RTC_HandleTypeDef *hrtc, FunctionalState NewState)
{
    /* Check input parameters */
    if (hrtc == NULL)
    {
        return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;
    RTC_Disable_Write_Protected(hrtc);
    /* In case of interrupt mode is used, the interrupt source must disabled */
    if (NewState != DISABLE)
    {
        SET_BIT(hrtc->Instance->CR, RTC_CR_ALM1EN);
    }
    else
    {
        CLEAR_BIT(hrtc->Instance->CR, RTC_CR_ALM1EN);
    }

    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @brief  Set the RTC Alarm1 interrupt source.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  AlarmEn  Alarm1 interrupt source
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm1_Set_INT_Source(RTC_HandleTypeDef *hrtc, RTC_AlarmEnableTypeDef *AlarmEn)
{
    uint32_t alarmen_reg = 0;

    alarmen_reg = (hrtc->Instance->ALM1DATE) & 0XFFFFFF;
    alarmen_reg |= (AlarmEn->Year_Enable | AlarmEn->Month_Enable | AlarmEn->Date_Enable | AlarmEn->WeekDay_Enable | AlarmEn->Hours_Enable | AlarmEn->Minutes_Enable | AlarmEn->Seconds_Enable);

    RTC_Disable_Write_Protected(hrtc);
    hrtc->Instance->ALM1DATE = alarmen_reg;

    /* Enable write protection */
    WRITE_REG(hrtc->Instance->WPR, 0XFF);

    return HAL_OK;
}

/**
 * @brief  Active or Deactive the specified RTC Alarm2
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  NewState: the Alarm2 enable  or disable.
 *          This parameter can be one of the following values:
 *            @arg DISABLE
 *            @arg ENABLE
 * @param  CycleTime: the Alarm2 cycle time set.
 *          This parameter can be a value of @ref HAL_RTCAlarm2TypeDef
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_Alarm2_Config(RTC_HandleTypeDef *hrtc, FunctionalState NewState, HAL_RTCAlarm2TypeDef CycleTime)
{
    /* Check input parameters */
    if (hrtc == NULL)
    {
        return HAL_ERROR;
    }
    UNUSED(CycleTime);

    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;
    RTC_Disable_Write_Protected(hrtc);
    /* In case of interrupt mode is used, the interrupt source must disabled */
    if (NewState != DISABLE)
    {
        WRITE_REG(hrtc->Instance->ALM2PRD, CycleTime);
    }
    else
    {
        WRITE_REG(hrtc->Instance->ALM2PRD, DISABLE);
    }
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @}
 */

/** @defgroup RTC_Exported_Functions_Group4 Peripheral State functions
 *  @brief   Peripheral State functions
 *
@verbatim
 ===============================================================================
                     ##### Peripheral State functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Get RTC state

@endverbatim
  * @{
  */
/**
 * @brief  Returns the RTC state.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval HAL state
 */
HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef *hrtc)
{
    return hrtc->State;
}

/**
 * @}
 */

/** @defgroup RTC_Exported_Functions_Group5 Peripheral Control functions
 *  @brief   Peripheral Control functions
 *
@verbatim
 ===============================================================================
                     ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Wait for RTC Time and Date Synchronization

@endverbatim
  * @{
  */

/**
 * @brief  Enters the RTC Initialization mode.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval HAL status
 */
static HAL_StatusTypeDef RTC_Enter_Init_Mode(RTC_HandleTypeDef *hrtc)
{
    uint32_t tickstart = 0U;

    tickstart = HAL_GetTick();

    __HAL_RTC_ENTER_RW(hrtc);
    /* Wait till RTC is in WAITF state and if Time out is reached exit */
    while ((__HAL_RTC_GET_WAITF_STATUS(hrtc)) == RESET)
    {
        if ((HAL_GetTick() - tickstart) > RTC_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Exit the RTC Initialization mode.
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval HAL status
 */
static HAL_StatusTypeDef RTC_Exit_Init_Mode(RTC_HandleTypeDef *hrtc)
{
    uint32_t tickstart = 0U;

    tickstart = HAL_GetTick();

    __HAL_RTC_EXIT_RW(hrtc);
    /* Wait till RTC is in INIT state and if Time out is reached exit */
    while ((__HAL_RTC_GET_WAITF_STATUS(hrtc)) != RESET)
    {
        if ((HAL_GetTick() - tickstart) > RTC_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Get RSF flag .
 * @param  None
 * @retval FlagStatus
 */

FlagStatus RTC_Get_RSF_Flag(void)
{
    if (READ_BIT(RTC->ISR, RTC_ISR_RSF) != 0x00)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}
/**
 * @brief  Clear RSF flag .
 * @param  None
 * @retval None
 */

void RTC_Clear_RSF_Flag(void)
{
    RTC->WPR = RTC_WRITE_PROTECT_KEY1;
    RTC->WPR = RTC_WRITE_PROTECT_KEY2;
    CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);
}
/**
 * @brief  Get Alarm  flag .
 * @param  Alarm: Specifies the Alarm.
 *          This parameter can be one of the following values:
 *            @arg RTC_ALARM_1:  Alarm1
 *           @arg RTC_ALARM_2:  Alarm2
 * @retval FlagStatus
 */

FlagStatus HAL_RTC_Get_Flag(uint32_t X)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT_TYPE(X));

    if ((X != RTC_ALARM_2) && (X != RTC_TAMPF) && (X != RTC_TAMPOVF))
    {
        if (READ_BIT(RTC->ISR, RTC_ISR_ALM1_F) != RESET)
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if ((X != RTC_ALARM_1) && (X != RTC_TAMPF) && (X != RTC_TAMPOVF))
    {
        if (READ_BIT(RTC->ISR, RTC_ISR_ALM2_F) != RESET)
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if ((X != RTC_ALARM_1) && (X != RTC_ALARM_2) && (X != RTC_TAMPOVF))
    {
        if (READ_BIT(RTC->ISR, RTC_ISR_TAMPF) != RESET)
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else
    {
        if (READ_BIT(RTC->ISR, RTC_ISR_TAMPOVF) != RESET)
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
}
/**
 * @brief  Clear Alarm flag .
 * @param  Alarm: Specifies the Alarm.
 *          This parameter can be one of the following values:
 *            @arg RTC_ALARM_1:  Alarm1
 *           @arg RTC_ALARM_2:  Alarm2
 * @retval FlagStatus
 */

void HAL_RTC_Clear_Flag(uint32_t X)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT_TYPE(X));

    RTC->WPR = RTC_WRITE_PROTECT_KEY1;
    RTC->WPR = RTC_WRITE_PROTECT_KEY2;
    if (X == RTC_ALARM_1)
    {
        SET_BIT(RTC->INTCLR, RTC_INTCLR_ALM1_CLR);
    }
    else if (X == RTC_ALARM_2)
    {
        SET_BIT(RTC->INTCLR, RTC_INTCLR_ALM2_CLR);
    }
    else if (X == RTC_TAMPF)
    {
        SET_BIT(RTC->INTCLR, RTC_INTCLR_TAMPF_CLR);
    }
    else
    {
        SET_BIT(RTC->INTCLR, RTC_INTCLR_TAMPOVF_CLR);
    }
}
/**
 * @brief  Deactive the specified 1HZ out
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @param  NewState: the 1HZ out enable  or disable.
 *          This parameter can be one of the following values:
 *            @arg DISABLE
 *            @arg ENABLE
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_RTC_1HZ_Config(RTC_HandleTypeDef *hrtc, FunctionalState NewState)
{
    /* Check input parameters */
    if (hrtc == NULL)
    {
        return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);

    hrtc->State = HAL_RTC_STATE_BUSY;

    RTC_Disable_Write_Protected(hrtc);
    /* In case of interrupt mode is used, the interrupt source must disabled */
    if (NewState != DISABLE)
    {
        SET_BIT(hrtc->Instance->CR, RTC_CR_RTC1HZOE);
    }
    else
    {
        CLEAR_BIT(hrtc->Instance->CR, RTC_CR_RTC1HZOE);
    }
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);

    return HAL_OK;
}

/**
 * @brief  Write adjust register
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval None
 */
void HAL_RTC_Adjust_Config(RTC_HandleTypeDef *hrtc)
{
    uint32_t temp;
    CLEAR_REG(RTC->CLKCALR);
    temp = hrtc->Init.TimeAdjustMode | (hrtc->Init.TimeTrim << RTC_CLKCALR_TRIM_Pos);
    WRITE_REG(RTC->CLKCALR, temp);
    SET_BIT(RTC->CLKCALR, RTC_CLKCALR_TRIM_EN);
}

/**
 * @brief  Prohibit RTC_Alarm and 1HZ output
 * @param  None
 * @retval None
 */
void HAL_RTC_Ban_Config(void)
{
    CLEAR_BIT(RTC->CR, RTC_CR_RTC1HZOE);
    RTC->CR |= (OSEL_0 << RTC_CR_OSEL_Pos);
}

/**
 * @brief  configure the intrusion interrupt register
 * @param  hrtcintrude   pointer to a RTC_IntrudeTypeDef structure that contains
 *                       the configuration information for RTC.
 * @retval None
 */
void HAL_RTC_Intrude_Config(RTC_IntrudeTypeDef *hrtcintrude)
{
    uint32_t temp = 0;
    temp = READ_REG(RTC->TAFCR);
    temp |= hrtcintrude->TAMP_DB | hrtcintrude->TAMP_MODE;
    WRITE_REG(RTC->TAFCR, temp);
    SET_BIT(RTC->TAFCR, hrtcintrude->TAMPE);
}

/**
 * @brief  Disable Write Protected
 * @param  hrtc   pointer to a RTC_HandleTypeDef structure that contains
 *                the configuration information for RTC.
 * @retval None
 */

static void RTC_Disable_Write_Protected(RTC_HandleTypeDef *hrtc)
{
    hrtc->Instance->WPR = RTC_WRITE_PROTECT_KEY1;
    hrtc->Instance->WPR = RTC_WRITE_PROTECT_KEY2;
}

/**
 * @brief  alarm interrupt and cycle interrupt
 * @param  None
 * @retval None
 */
void HAL_RTC_IRQHandler(RTC_HandleTypeDef *hrtc)
{

    if (HAL_RTC_Get_Flag(RTC_ALARM_1) == SET)
    {
        HAL_RTC_Clear_Flag(RTC_ALARM_1);

        if (hrtc->AlarmAEventCallback != NULL)
        {
            hrtc->AlarmAEventCallback(hrtc);
        }
    }

    if (HAL_RTC_Get_Flag(RTC_ALARM_2) == SET)
    {
        HAL_RTC_Clear_Flag(RTC_ALARM_2);

        if (hrtc->CycleEventCallback != NULL)
        {
            hrtc->CycleEventCallback(hrtc);
        }
    }
}

/**
 * @brief  intrusion interrupt
 * @param  None
 * @retval None
 */
void HAL_RTC_TAMP_IRQHandler(RTC_HandleTypeDef *hrtc)
{
    if (HAL_RTC_Get_Flag(RTC_TAMPF) == SET)
    {
        HAL_RTC_Clear_Flag(RTC_TAMPF);

        if (hrtc->TampfEventCallback != NULL)
        {
            hrtc->TampfEventCallback(hrtc);
        }
    }

    if (HAL_RTC_Get_Flag(RTC_TAMPOVF) == SET)
    {
        HAL_RTC_Clear_Flag(RTC_TAMPOVF);

        if (hrtc->TampovfEventCallback != NULL)
        {
            hrtc->TampovfEventCallback(hrtc);
        }
    }
}

/**
 * @}
 */
/**
 * @}
 */

#endif /* HAL_RTC_MODULE_ENABLED */

/**
 * @}
 */

/**
 * @}
 */
