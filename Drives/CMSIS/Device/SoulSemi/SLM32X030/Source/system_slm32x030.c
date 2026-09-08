/**
  ******************************************************************************
  * @file    system_slm32x030.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  * 
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      factors, AHB/APBx prescalers and Flash settings). 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_slm32x030_xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the HSI (8 MHz) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_slm32x030_xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 4. The default value of HSE crystal is set to 8 MHz (or 25 MHz, depending on
  *    the product used), refer to "HSE_VALUE". 
  *    When HSE is used as system clock source, directly or through PLL, and you
  *    are using different crystal you have to adapt the HSE value to your own
  *    configuration.
  *        
  ******************************************************************************
**/
#include "slm32x030.h"
#include "system_slm32x030.h"

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
uint32_t   SystemCoreClock;        /*!< System Clock Frequency (Core Clock) */

#ifdef DATA_IN_ExtSRAM
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @retval None
  */
void SystemInit (void)
{
    /* Reset the RCMU clock configuration to the default reset state(for debug purpose) */
    /* Set HSION bit */
    RCMU->CR |= RCMU_CR_HSION;

    /* Reset SW, HPRE, PPRE1, PPRE2 and MCO bits */
    RCMU->CFGR &= ~(RCMU_CFGR_SW|RCMU_CFGR_HPRE|RCMU_CFGR_PPRE1|RCMU_CFGR_PPRE2);
    RCMU->CFGR &= ~(RCMU_CFGR_MCO|RCMU_CFGR_MCOPRE);
    
    /* Reset TIMPPRE1 and TIMPPRE2 bits */
    RCMU->CFGR2 &= ~(RCMU_CFGR2_TIMPRE1|RCMU_CFGR2_TIMPRE2);
    
    /* Reset HSEON and PLLON bits */
    RCMU->CR &= ~(RCMU_CR_HSEON|RCMU_CR_PLLON);

    /* Reset HSEBYP bit */
    RCMU->CR &= ~RCMU_CR_HSEBYP;

    /* Reset PLLSRC, PLLBYP, PLLMUL bits */
    RCMU->PLLCR &= ~(RCMU_PLLCR_PLL_SRC|RCMU_PLLCR_PLL_BYP|RCMU_PLLCR_CFG0_M|RCMU_PLLCR_CFG0_N|RCMU_PLLCR_CFG0_OD);
    RCMU->PLLCR2 &= ~(RCMU_PLLCR2_CFG1_M|RCMU_PLLCR2_CFG1_N|RCMU_PLLCR2_CFG1_OD);

    /* Disable all interrupts and clear pending bits  */
    RCMU->CIR = 0x00000000U;

    #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
    #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
    #endif 
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in slm32x030.h file (default value
  *             8 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in slm32x030.h file (default value
  *              8 MHz or 25 MHz, depending on the product used), user has to ensure
  *              that HSE_VALUE is same as the real frequency of the crystal used.
  *              Otherwise, this function may have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
    uint32_t tmp = 0U, pllmull = 0U, pllsource = 0U;
        
    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCMU->CFGR & RCMU_CFGR_SWS;

    switch (tmp)
    {
        case 0x00U:  /* HSI used as system clock */
            SystemCoreClock = HSI_VALUE;
        break;

        case 0x01U:  /* HSE used as system clock */
            SystemCoreClock = HSE_VALUE;
        break;

        case 0x02U:  /* PLL used as system clock */
            /* Get PLL clock source and multiplication factor ----------------------*/
            pllmull = RCMU->PLLCR & RCMU_PLLCR_CFG0_M;
            pllsource = RCMU->PLLCR & RCMU_PLLCR_PLL_SRC;  
            
            if (pllsource == 0x00)
            {
                /* HSI oscillator clock divided by 2 selected as PLL clock entry */
                SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
            }	
            else
            {
                        /* HSE selected as PLL clock entry */
                if ((RCMU->PLLCR & RCMU_PLLCR_PLL_SRC) != (uint32_t)RESET)
                {/* HSE oscillator clock divided by 2 */
                SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
                }
                else
                {
                SystemCoreClock = HSE_VALUE * pllmull;
                }
            }				
        break;

        case 0x03U:  /* PLL used as system clock */

        break;

        default:
            SystemCoreClock = HSI_VALUE;
        break;
    }
    /* Compute HCLK clock frequency ----------------*/
    /* Get HCLK prescaler */
    SystemCoreClock >>= ((RCMU->CFGR & RCMU_CFGR_HPRE) >> RCMU_CFGR_HPRE_Pos);
    /* HCLK clock frequency */
}


