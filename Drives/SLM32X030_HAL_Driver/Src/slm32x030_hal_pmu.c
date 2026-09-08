/**
  ******************************************************************************
  * @file    slm32x030_hal_pmu.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   PMU HAL module driver.
  *
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PMU) peripheral:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions 
  *
  ******************************************************************************

  */

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @defgroup PMU PMU
  * @brief    PMU HAL module driver
  * @{
  */

#ifdef HAL_PMU_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup PMU_Private_Constants PMU Private Constants
  * @{
  */
  
/** @defgroup PMU_PVD_Mode_Mask PMU PVD Mode Mask
  * @{
  */ 
#define PVD_MODE_IT               (0x00010000U)
#define PVD_MODE_EVT              (0x00020000U)
#define PVD_RISING_EDGE           (0x00000001U)
#define PVD_FALLING_EDGE          (0x00000002U)
/**
  * @}
  */


/** @defgroup PMU_register_alias_address PMU Register alias address
  * @{
  */ 
/* ------------- PMU registers bit address in the alias region ---------------*/
#define PMU_OFFSET               (PMU_BASE - PERIPH_BASE)
#define PMU_CR_OFFSET            (0x00U)
#define PMU_CSR_OFFSET           (0x04U)
#define PMU_CR_OFFSET_BB         (PMU_OFFSET + PMU_CR_OFFSET)
#define PMU_CSR_OFFSET_BB        (PMU_OFFSET + PMU_CSR_OFFSET)
/**
  * @}
  */
   
/** @defgroup PMU_CR_register_alias PMU CR Register alias address
  * @{
  */  
/* --- CR Register ---*/
/* Alias word address of LPSDSR bit */
#define LPSDSR_BIT_NUMBER        PMU_CR_LPDS_Pos
#define CR_LPSDSR_BB             ((uint32_t)(PERIPH_BB_BASE + (PMU_CR_OFFSET_BB * 32U) + (LPSDSR_BIT_NUMBER * 4U)))

/* Alias word address of DBP bit */
#define DBP_BIT_NUMBER            PMU_CR_DBP_Pos
#define CR_DBP_BB                ((uint32_t)(PERIPH_BB_BASE + (PMU_CR_OFFSET_BB * 32U) + (DBP_BIT_NUMBER * 4U)))

/* Alias word address of PDDS bit */
#define PDDS_BIT_NUMBER           PMU_CR_PDDS_Pos
#define CR_PDDS_BB               ((uint32_t)(PERIPH_BB_BASE + (PMU_CR_OFFSET_BB * 32U) + (PDDS_BIT_NUMBER * 4U)))

/**
  * @}
  */

/** @defgroup PMU_CSR_register_alias PMU CSR Register alias address
  * @{
  */

/* --- CSR Register ---*/
/* Alias word address of EWUP1 bit */
#define CSR_EWUP_BB(VAL)         ((uint32_t)(PERIPH_BB_BASE + (PMU_CSR_OFFSET_BB * 32U) + (POSITION_VAL(VAL) * 4U)))

/**
  * @}
  */
  
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup PMU_Private_Functions PMU Private Functions
 * brief   WFE cortex command overloaded for HAL_PMU_EnterSTOPMode usage only (see Workaround section)
 * @{
 */
static void PMU_OverloadWfe(void);

/* Private functions ---------------------------------------------------------*/
__NOINLINE
static void PMU_OverloadWfe(void)
{
    __asm volatile( "wfe" );
    __asm volatile( "nop" );
}

/**
  * @}
  */


/** @defgroup PMU_Exported_Functions PMU Exported Functions
  * @{
  */

/** @defgroup PMU_Exported_Functions_Group1 Initialization and de-initialization functions 
  *  @brief   Initialization and de-initialization functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      After reset, the backup domain (RTC registers, RTC backup data
      registers) is protected against possible unwanted
      write accesses.
      To enable access to the RTC Domain and RTC registers, proceed as follows:
        (+) Enable the Power Controller (PMU) APB1 interface clock using the
            __HAL_RCMU_PMU_CLK_ENABLE() macro.
        (+) Enable access to RTC domain using the HAL_PMU_EnableBkUpAccess() function.

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the PMU peripheral registers to their default reset values.  
  * @retval None
  */
void HAL_PMU_DeInit(void)
{

}

/**
  * @brief  Enables access to the backup domain (RTC registers, RTC
  *         backup data registers ).
  * @note   If the HSE divided by 128 is used as the RTC clock, the
  *         Backup Domain Access should be kept enabled.
  * @retval None
  */
void HAL_PMU_EnableBkUpAccess(void)
{
    /* Enable access to RTC and backup registers */
    *(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;
}

/**
  * @brief  Disables access to the backup domain (RTC registers, RTC
  *         backup data registers).
  * @note   If the HSE divided by 128 is used as the RTC clock, the
  *         Backup Domain Access should be kept enabled.
  * @retval None
  */
void HAL_PMU_DisableBkUpAccess(void)
{
    /* Disable access to RTC and backup registers */
    *(__IO uint32_t *) CR_DBP_BB = (uint32_t)DISABLE;
}

/**
  * @}
  */

/** @defgroup PMU_Exported_Functions_Group2 Peripheral Control functions 
  * @brief    Low Power modes configuration functions
  *
@verbatim
 ===============================================================================
                 ##### Peripheral Control functions #####
 ===============================================================================
     
    *** PVD configuration ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a
          threshold selected by the PVD Level (PLS[2:0] bits in the PMU_CR).

      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PVD_EXTI_ENABLE_IT() macro.
      (+) The PVD is stopped in Standby mode.

    *** WakeUp pin configuration ***
    ================================
    [..]
      (+) WakeUp pin is used to wake up the system from Standby mode. This pin is
          forced in input pull-down configuration and is active on rising edges.
      (+) There is one WakeUp pin:
          WakeUp Pin 1 on PA.00.

    [..]

    *** Low Power modes configuration ***
    =====================================
     [..]
      The device features 3 low-power modes:
      (+) Sleep mode: CPU clock off, all peripherals including Cortex-M3 core peripherals like 
                      NVIC, SysTick, etc. are kept running
      (+) Stop mode: All clocks are stopped
      (+) Standby mode: 1.8V domain powered off
  
  
   *** Sleep mode ***
   ==================
    [..]
      (+) Entry:
          The Sleep mode is entered by using the HAL_PMU_EnterSLEEPMode(PMU_MAINREGULATOR_ON, PMU_SLEEPENTRY_WFx)
              functions with
          (++) PMU_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
          (++) PMU_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
     
      (+) Exit:
        (++) WFI entry mode, Any peripheral interrupt acknowledged by the nested vectored interrupt
             controller (NVIC) can wake up the device from Sleep mode.
        (++) WFE entry mode, Any wakeup event can wake up the device from Sleep mode.
           (+++) Any peripheral interrupt w/o NVIC configuration & SEVONPEND bit set in the Cortex (HAL_PMU_EnableSEVOnPend)
           (+++) Any EXTI Line (Internal or External) configured in Event mode

   *** Stop mode ***
   =================
    [..]
      The Stop mode is based on the Cortex-M3 deepsleep mode combined with peripheral
      clock gating. The voltage regulator can be configured either in normal or low-power mode.
      In Stop mode, all clocks in the 1.8 V domain are stopped, the PLL, the HSI and the HSE RC 
      oscillators are disabled. SRAM and register contents are preserved.
      In Stop mode, all I/O pins keep the same state as in Run mode.

      (+) Entry:
           The Stop mode is entered using the HAL_PMU_EnterSTOPMode(PMU_REGULATOR_VALUE, PMU_SLEEPENTRY_WFx )
             function with:
          (++) PMU_REGULATOR_VALUE= PMU_MAINREGULATOR_ON: Main regulator ON.
          (++) PMU_REGULATOR_VALUE= PMU_LOWPOWERREGULATOR_ON: Low Power regulator ON.
          (++) PMU_SLEEPENTRY_WFx= PMU_SLEEPENTRY_WFI: enter STOP mode with WFI instruction
          (++) PMU_SLEEPENTRY_WFx= PMU_SLEEPENTRY_WFE: enter STOP mode with WFE instruction
      (+) Exit:
          (++) WFI entry mode, Any EXTI Line (Internal or External) configured in Interrupt mode with NVIC configured
          (++) WFE entry mode, Any EXTI Line (Internal or External) configured in Event mode.

   *** Standby mode ***
   ====================
     [..]
      The Standby mode allows to achieve the lowest power consumption. It is based on the
      Cortex-M3 deepsleep mode, with the voltage regulator disabled. The 1.8 V domain is 
      consequently powered off. The PLL, the HSI oscillator and the HSE oscillator are also 
      switched off. SRAM and register contents are lost except for registers in the Backup domain 
      and Standby circuitry
      
      (+) Entry:
        (++) The Standby mode is entered using the HAL_PMU_EnterSTANDBYMode() function.
      (+) Exit:
        (++) WKUP pin rising edge, RTC alarm event rising edge, external Reset in 
             NRSTpin, IWDG Reset

   *** Auto-wakeup (AWU) from low-power mode ***
       =============================================
       [..]
        
       (+) The MCU can be woken up from low-power mode by an RTC Alarm event, 
           without depending on an external interrupt (Auto-wakeup mode).
   
       (+) RTC auto-wakeup (AWU) from the Stop and Standby modes

           (++) To wake up from the Stop mode with an RTC alarm event, it is necessary to 
                configure the RTC to generate the RTC alarm using the HAL_RTC_SetAlarm_IT() function.

   *** PMU Workarounds linked to Silicon Limitation ***
       ====================================================
       [..]
       Below the list of all silicon limitations known on SLM32X030 prouct.

       (#)Workarounds Implemented inside PMU HAL Driver
          (##)Debugging Stop mode with WFE entry - overloaded the WFE by an internal function    
        
  @endverbatim
  ******************************************************************************
 **/

/**
  * @brief  Enables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PMU_EnablePVD(void)
{
    /* Enable the power voltage detector */
    *(__IO uint32_t *) CR_PDDS_BB = (uint32_t)DISABLE;
    *(__IO uint32_t *) CR_LPSDSR_BB = (uint32_t)DISABLE;
}

/**
  * @brief  Disables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PMU_DisablePVD(void)
{
    /* Disable the power voltage detector */
    *(__IO uint32_t *) CR_PDDS_BB = (uint32_t)ENABLE;
    *(__IO uint32_t *) CR_LPSDSR_BB = (uint32_t)ENABLE;
}

/**
  * @brief Enables the WakeUp PINx functionality.
  * @param WakeUpPinx: Specifies the Power Wake-Up pin to enable.
  *        This parameter can be one of the following values:
  *           @arg PMU_WAKEUP_PIN1
  * @retval None
  */
void HAL_PMU_EnableWakeUpPin(uint32_t WakeUpPinx)
{
    /* Check the parameter */
    assert_param(IS_PMU_WAKEUP_PIN(WakeUpPinx));

    SET_BIT(PMU->CSR, WakeUpPinx);
}
/**
  * @brief Disables the WakeUp PINx functionality.
  * @param WakeUpPinx: Specifies the Power Wake-Up pin to disable.
  *        This parameter can be one of the following values:
  *           @arg PMU_WAKEUP_PIN1
  * @retval None
  */
void HAL_PMU_DisableWakeUpPin(uint32_t WakeUpPinx)
{
    /* Check the parameter */
    assert_param(IS_PMU_WAKEUP_PIN(WakeUpPinx));
      
    CLEAR_BIT(PMU->CSR, WakeUpPinx);
}

/**
  * @brief Enters Sleep mode.
  * @note  In Sleep mode, all I/O pins keep the same state as in Run mode.
  * @param Regulator: Regulator state as no effect in SLEEP mode -  allows to support portability from legacy software
  * @param SLEEPEntry: Specifies if SLEEP mode is entered with WFI or WFE instruction.
  *           When WFI entry is used, tick interrupt have to be disabled if not desired as 
  *           the interrupt wake up source.
  *           This parameter can be one of the following values:
  *            @arg PMU_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
  *            @arg PMU_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void HAL_PMU_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
{
    /* Check the parameters */
    /* No check on Regulator because parameter not used in SLEEP mode */
    /* Prevent unused argument(s) compilation warning */
    UNUSED(Regulator);

    assert_param(IS_PMU_SLEEP_ENTRY(SLEEPEntry));

    /* Clear SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    
    /* clear CR PDDS bit  */ 
    CLEAR_BIT(PMU->CR,  PMU_CR_PDDS);

    /* Set CR LPDS bit  */ 
    SET_BIT(PMU->CR,  PMU_LOWPOWERREGULATOR_ON);

    /* Select SLEEP mode entry -------------------------------------------------*/
    if(SLEEPEntry == PMU_SLEEPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief Enters Stop mode. 
  * @note  In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note  When exiting Stop mode by using an interrupt or a wakeup event,
  *        HSI RC oscillator is selected as system clock.
  * @note  When the voltage regulator operates in low power mode, an additional
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption
  *         is higher although the startup time is reduced.    
  * @param Regulator: Specifies the regulator state in Stop mode.
  *          This parameter can be one of the following values:
  *            @arg PMU_MAINREGULATOR_ON: Stop mode with regulator ON
  *            @arg PMU_LOWPOWERREGULATOR_ON: Stop mode with low power regulator ON
  * @param STOPEntry: Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PMU_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PMU_STOPENTRY_WFE: Enter Stop mode with WFE instruction   
  * @retval None
  */
void HAL_PMU_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
    /* Check the parameters */
    assert_param(IS_PMU_REGULATOR(Regulator));
    assert_param(IS_PMU_STOP_ENTRY(STOPEntry));

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

    /* Clear PDDS bit in PMU register to specify entering in STOP mode when CPU enter in Deepsleep */ 
    CLEAR_BIT(PMU->CR,  PMU_CR_PDDS);

    /* Clear CSR WUF bit  */ 
    CLEAR_BIT(PMU->CSR,  PMU_CSR_WUF_PIN);

    /* Set CR LPDS bit  */ 
    SET_BIT(PMU->CR,  Regulator);

    /* Select Stop mode entry --------------------------------------------------*/
    if(STOPEntry == PMU_STOPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
        SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk)); 
    }
    else
    {
        // /* Request Wait For Event */
        PMU_OverloadWfe(); /* WFE redefine locally */
        SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    }
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
}

/**
  * @brief Enters Standby mode.
  * @note  In Standby mode, all I/O pins are high impedance except for:
  *          - Reset pad (still available) 
  *          - TAMPER pin if configured for tamper or calibration out.
  *          - WKUP pin (PA0) if enabled.
  * @retval None
  */
void HAL_PMU_EnterSTANDBYMode(void)
{
    /* Select Standby mode */
    SET_BIT(PMU->CR, PMU_CR_PDDS);

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
    __force_stores();
#endif
    /* Request Wait For interrupt */
    __WFI();
}


/**
  * @brief Indicates Sleep-On-Exit when returning from Handler mode to Thread mode. 
  * @note Set SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.
  *       Setting this bit is useful when the processor is expected to run only on
  *       interruptions handling.         
  * @retval None
  */
void HAL_PMU_EnableSleepOnExit(void)
{
    /* Set SLEEPONEXIT bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}


/**
  * @brief Disables Sleep-On-Exit feature when returning from Handler mode to Thread mode. 
  * @note Clears SLEEPONEXIT bit of SCR register. When this bit is set, the processor 
  *       re-enters SLEEP mode when an interruption handling is over.          
  * @retval None
  */
void HAL_PMU_DisableSleepOnExit(void)
{
    /* Clear SLEEPONEXIT bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}


/**
  * @brief Enables CORTEX M3 SEVONPEND bit. 
  * @note Sets SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.
  * @retval None
  */
void HAL_PMU_EnableSEVOnPend(void)
{
    /* Set SEVONPEND bit of Cortex System Control Register */
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @brief Disables CORTEX M3 SEVONPEND bit. 
  * @note Clears SEVONPEND bit of SCR register. When this bit is set, this causes 
  *       WFE to wake up when an interrupt moves from inactive to pended.         
  * @retval None
  */
void HAL_PMU_DisableSEVOnPend(void)
{
    /* Clear SEVONPEND bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @brief  This function handles the PMU PVD interrupt request.
  * @note   This API should be called under the PVD_IRQHandler().
  * @retval None
  */
void HAL_PMU_PVD_IRQHandler(void)
{
    /* Check PMU exti flag */
    if(__HAL_PMU_PVD_EXTI_GET_FLAG() != RESET)
    {
        /* PMU PVD interrupt user callback */
        HAL_PMU_PVDCallback();

        /* Clear PMU Exti pending bit */
        __HAL_PMU_PVD_EXTI_CLEAR_FLAG();
    }
}

/**
  * @brief  PMU PVD interrupt callback
  * @retval None
  */
__weak void HAL_PMU_PVDCallback(void)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PMU_PVDCallback could be implemented in the user file
    */ 
}

/**
  * @}
  */

#endif /* HAL_PMU_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */
