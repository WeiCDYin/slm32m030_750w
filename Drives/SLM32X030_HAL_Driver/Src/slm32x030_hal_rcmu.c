/**
  ******************************************************************************
  * @file    slm32x030_hal_rcmu.c
  * @author  Application Team
  * @version V1.0.0
  * @date    8-11-2024
  * @brief   RCMU HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Reset and Clock Control (RCMU) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *
  @verbatim
  ==============================================================================
                      ##### RCMU specific features #####
  ==============================================================================
    [..]
      After reset the device is running from Internal High Speed oscillator
      (HSI 8MHz) with Flash 0 wait state, Flash prefetch buffer is enabled, 
      and all peripherals are off except internal SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) buses;
          all peripherals mapped on these buses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.
    [..] Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings  
      (+) Configure the AHB and APB buses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals whose clocks are not
          derived from the System clock (I2S, RTC, ADC, USB OTG FS) 

                      ##### RCMU Limitations #####
  ==============================================================================
    [..]
      A delay between an RCMU peripheral clock enable and the effective peripheral 
      enabling should be taken into account in order to manage the peripheral read/write 
      from/to registers.
      (+) This delay depends on the peripheral mapping.
        (++) AHB & APB peripherals, 1 dummy read is necessary

    [..]  
      Workarounds:
      (#) For AHB & APB peripherals, a dummy read to the peripheral register has been
          inserted in each __HAL_RCMU_PPP_CLK_ENABLE() macro.

  @endverbatim
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @defgroup RCMU RCMU
* @brief RCMU HAL module driver
  * @{
  */

#ifdef HAL_RCMU_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCMU_Private_Constants RCMU Private Constants
 * @{
 */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/** @defgroup RCMU_Private_Macros RCMU Private Macros  PA8~PA9可以作为mco的输出
  * @{
  */



/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup RCMU_Private_Variables RCMU Private Variables
  * @{
  */
static uint8_t pll_type = 0;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
static void RCMU_Delay(uint32_t mdelay) __attribute__((unused));

/* Exported functions --------------------------------------------------------*/

/** @defgroup RCMU_Exported_Functions RCMU Exported Functions
  * @{
  */

/** @defgroup RCMU_Exported_Functions_Group1 Initialization and de-initialization functions 
  *  @brief    Initialization and Configuration functions 
  *
  @verbatim    
  ===============================================================================
           ##### Initialization and de-initialization functions #####
  ===============================================================================
    [..]
      This section provides functions allowing to configure the internal/external oscillators
      (HSE, HSI, LSE, LSI, PLL, CSS and MCO) and the System buses clocks (SYSCLK, AHB, APB1
      and APB2).

    [..] Internal/external clock and PLL configuration
      (#) HSI (high-speed internal), 4 to 32 MHz factory-trimmed RC used directly or through
          the PLL as System clock source.
      (#) LSI (low-speed internal), ~40 KHz low consumption RC used as IWDG and/or RTC
          clock source.

      (#) HSE (high-speed external), 4 to 32 MHz  crystal oscillator used directly or
          through the PLL as System clock source. Can be used also as RTC clock source.

      (#) LSE (low-speed external), 32.768KHz oscillator used as RTC clock source.

      (#) PLL (clocked by HSI or HSE), featuring different output clocks:
        (++) The first output is used to generate the high speed system clock 

      (#) CSS (Clock security system), once enable using the macro __HAL_RCMU_CSS_ENABLE()
          and if a HSE clock failure occurs(HSE used directly or through PLL as System 
          clock source), the System clocks automatically switched to HSI and an interrupt
          is generated if enabled. The interrupt is linked to the Cortex-M3 NMI 
          (Non-Maskable Interrupt) exception vector.

      (#) MCO1 (microcontroller clock output), used to output SYSCLK, HSI,

    [..] System, AHB and APB buses clocks configuration
      (#) Several clock sources can be used to drive the System clock (SYSCLK): HSI,
          HSE and PLL.
          The AHB clock (HCLK) is derived from System clock through configurable
          prescaler and used to clock the CPU, memory and peripherals mapped
          on AHB bus (DMA, GPIO...). APB1 (PCLK1) and APB2 (PCLK2) clocks are derived
          from AHB clock through configurable prescalers and used to clock
          the peripherals mapped on these buses. You can use
          "@ref HAL_RCMU_GetSysClockFreq()" function to retrieve the frequencies of these clocks.

      -@- All the peripheral clocks are derived from the System clock (SYSCLK) except:
          (+@) RTC: RTC clock can be derived either from the LSI, LSE or HSE clock
              divided by 128. 
      
          (+@) IWDG clock which is always the LSI clock.

          Depending on the SYSCLK frequency, the flash latency should be adapted accordingly.
  @endverbatim
  * @{
  */
  
/*
  Additional consideration on the SYSCLK based on Latency settings:  (page 26)
        +-----------------------------------------------+
        | Latency       | SYSCLK clock frequency (MHz)  |
        |---------------|-------------------------------|
        |0WS(1CPU cycle)|       0 < SYSCLK <= 32        |
        |---------------|-------------------------------|
        |1WS(2CPU cycle)|      32 < SYSCLK <= 64        |
        +-----------------------------------------------+
  */

/**
  * @brief  Resets the RCMU clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, PLLare OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS and MCO1 OFF
  *            - All interrupts disabled
  *            - All flags are cleared
  * @note   This function does not modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef HAL_RCMU_DeInit(void)
{
    uint32_t tickstart;

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    /* Set HSION bit */
    SET_BIT(RCMU->CR, RCMU_CR_HSION);

    /* Wait till HSI is ready */
    while (READ_BIT(RCMU->CR, RCMU_CR_HSIRDY) == RESET)
    {
        if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    /* Update the SystemCoreClock global variable */
    SystemCoreClock = HSI_VALUE;

    /* Adapt Systick interrupt period */
    if(HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    /* Second step is to clear PLLON bit */
    CLEAR_BIT(RCMU->CR, RCMU_CR_PLLON);

    /* Wait till PLL is disabled */
    while (READ_BIT(RCMU->CR, RCMU_CR_PLLRDY) != RESET)
    {
        if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    /* Reset HSEON bits */
    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEON);

    /* Wait till HSE is disabled */
    while(READ_BIT(RCMU->CR, RCMU_CR_HSERDY) != RESET)
    {
        if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    /* Reset HSEBYP bit */
    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEBYP);

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    /* Reset CFGR register */
    CLEAR_REG(RCMU->CFGR);

    /* Wait till clock switch is ready */
    while (READ_BIT(RCMU->CFGR, RCMU_CFGR_SWS) != RESET)
    {
        if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }

    /* Reset all CSR flags */
    SET_BIT(RCMU->CSR, RCMU_CSR_RMVF);

    /* Disable all interrupts */
    CLEAR_REG(RCMU->CIR);

    return HAL_OK;
}

/**
  * @brief  Initializes the RCMU Oscillators according to the specified parameters in the
  *         RCMU_OscInitTypeDef.
  * @param  RCMU_OscInitStruct pointer to an RCMU_OscInitTypeDef structure that
  *         contains the configuration information for the RCMU Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   The PLL is not disabled when USB OTG FS clock is enabled (specific to devices with USB FS)
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this macro. App should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. App should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCMU_OscConfig(RCMU_OscInitTypeDef  *RCMU_OscInitStruct)
{
    uint32_t tickstart = 0U;
    uint32_t temp = 0U;
    uint32_t tmp_reg = 0U;
    
    /* Check the parameters */
    assert_param(RCMU_OscInitStruct != NULL);
    HAL_RCMU_PllTypeSet(RCMU_OscInitStruct);
    
    /*------------------------------- HSE Configuration ------------------------*/
    if(RCMU_OscInitStruct->HSEState == RCMU_HSE_ON)
    {
        /* Check the parameters */
        assert_param(IS_RCMU_HSE(RCMU_OscInitStruct->HSEState));
            
        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Set HSEON bit */
        SET_BIT(RCMU->CR, RCMU_CR_HSEON);

        /* Wait till HSE is ready */
        while (__HAL_RCMU_GET_REG_FLAG(RCMU->CR, RCMU_CR_HSERDY, RCMU_CR_HSERDY_Pos) == RESET)
        {
            if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }

    /*----------------------------- HSI Configuration --------------------------*/ 
    if(RCMU_OscInitStruct->HSIState == RCMU_HSI_ON)
    {       
        /* Check the parameters */
        assert_param(IS_RCMU_HSI(RCMU_OscInitStruct->HSIState));
        assert_param(IS_RCMU_HSI(RCMU_OscInitStruct->HSISelect));

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Set HSION bit */
        SET_BIT(RCMU->CR, RCMU_CR_HSION);

        /* select HSI mode */
        RCMU->CR &= ~(RCMU_CR_HSI_FREQ_SEL);
        RCMU->CR |= (RCMU_OscInitStruct->HSISelect << RCMU_CR_HSI_FREQ_SEL_Pos);

        /* Wait till HSI is ready */
        while (__HAL_RCMU_GET_REG_FLAG(RCMU->CR, RCMU_CR_HSIRDY, RCMU_CR_HSIRDY_Pos) == RESET)
        {
            if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }

    /*------------------------------ LSE Configuration -------------------------*/ 
    if(RCMU_OscInitStruct->LSEState == RCMU_LSE_ON)
    {
        /* Check the parameters */
        assert_param(IS_RCMU_LSE(RCMU_OscInitStruct->LSEState));

        /*PMU clock enable*/ 
        __HAL_RCMU_PMU_CLK_ENABLE();
        /*Enable access to RTC and backup registers*/  
        SET_BIT(PMU->CR, PMU_CR_DBP); 

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Set LSE bit */
        //SET_BIT(RCMU->BDCR, RCMU_BDCR_LSEON);
        RCMU->BDCR |= RCMU_BDCR_LSEON;

        /* Wait till LSE is ready */
        while (__HAL_RCMU_GET_REG_FLAG(RCMU->BDCR, RCMU_BDCR_LSERDY, RCMU_BDCR_LSERDY_Pos) == RESET)
        {
            if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }
        
    /*------------------------------ LSI Configuration -------------------------*/ 
    if(RCMU_OscInitStruct->LSIState == RCMU_LSI_ON)
    {
        /* Check the parameters */  
        assert_param(IS_RCMU_LSI(RCMU_OscInitStruct->LSIState));
        assert_param(IS_RCMU_HSI(RCMU_OscInitStruct->LSISelect));
        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Set LSI bit */
        SET_BIT(RCMU->CSR, RCMU_CSR_LSION);

        /* select LSI high or low*/
        if(RCMU_OscInitStruct->LSISelect == RCMU_LSI_HIGH)
        {
            /* Set LSI bit  1：high frequency*/
            SET_BIT(RCMU->CSR, RCMU_CSR_LSE_FREQ_SEL);
        }
        else if(RCMU_OscInitStruct->LSISelect == RCMU_LSI_LOW)
        {
            /* Set LSI bit  0：Low frequency*/
            CLEAR_BIT(RCMU->CSR,RCMU_CSR_LSE_FREQ_SEL);
        }

        /* Wait till LSI is ready */
        while (__HAL_RCMU_GET_REG_FLAG(RCMU->CSR, RCMU_CSR_LSIRDY, RCMU_CSR_LSIRDY_Pos) == RESET)
        {
            if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }
    /*-------------------------------- PLL Configuration -----------------------*/
    if ((RCMU_OscInitStruct->PLL.PLLState) == RCMU_PLL_ON)  
    {    
        /* Check the parameters */
        assert_param(IS_RCMU_PLL(RCMU_OscInitStruct->PLL.PLLState));
        assert_param(IS_RCMU_PLLSRC(RCMU_OscInitStruct->PLL.PLLType));
        assert_param(IS_RCMU_PLLSRC(RCMU_OscInitStruct->PLL.PLLSource));    
        assert_param(IS_RCMU_PLLPREDIV(RCMU_OscInitStruct->PLL.PLLPreDiv)); 
        assert_param(IS_RCMU_PLLFREQ(RCMU_OscInitStruct->PLL.PLLFreq)); 
        /* must be configure PLL,last enable PLL_ON */
        
        if((RCMU_OscInitStruct->PLL.PLLType) == CFG0)
        {

            temp = RCMU->PLLCR2;
            tmp_reg = RCMU->PLLCR;

            /* configure CFG0*/
            temp &= (~(RCMU_PLLCR2_SEL_M | RCMU_PLLCR2_SEL_N | RCMU_PLLCR2_SEL_OD));
            WRITE_REG(RCMU->PLLCR2, temp);

            /* Set PLLCR CFG0_N,CFG0_OD and CFG0_M bit*/
            tmp_reg &= (~(RCMU_PLLCR_CFG0_N | RCMU_PLLCR_CFG0_OD | RCMU_PLLCR_CFG0_M));
            tmp_reg |= ((RCMU_OscInitStruct->PLL.PLLPreDiv << RCMU_PLLCR_CFG0_N_Pos) | (RCMU_OscInitStruct->PLL.PLLFreq << RCMU_PLLCR_CFG0_OD_Pos) | (RCMU_OscInitStruct->PLL.PLLMUL << RCMU_PLLCR_CFG0_M_Pos));
            WRITE_REG(RCMU->PLLCR, tmp_reg);
        }
        else if ((RCMU_OscInitStruct->PLL.PLLType) == CFG1)
        {

            temp = RCMU->PLLCR2;

            /* configure CFG1 , Set PLLCR CFG1_N,CFG1_OD and CFG1_M bit*/
            temp &= (~(RCMU_PLLCR2_SEL_M | RCMU_PLLCR2_SEL_N | RCMU_PLLCR2_SEL_OD | RCMU_PLLCR2_CFG1_N | RCMU_PLLCR2_CFG1_OD | RCMU_PLLCR2_CFG1_M));
            temp |= (RCMU_PLLCR2_SEL_M | RCMU_PLLCR2_SEL_N | RCMU_PLLCR2_SEL_OD | (RCMU_OscInitStruct->PLL.PLLPreDiv << RCMU_PLLCR2_CFG1_N_Pos) | (RCMU_OscInitStruct->PLL.PLLFreq << RCMU_PLLCR2_CFG1_OD_Pos) | (RCMU_OscInitStruct->PLL.PLLMUL << RCMU_PLLCR2_CFG1_M_Pos));
            WRITE_REG(RCMU->PLLCR2, temp);
        }

        if(RCMU_OscInitStruct->PLL.PLLState == RCMU_PLL_ON)
        {
            /* Set PLL_SRC  PLL Input source selection:HSI or HSE */
            if(RCMU_OscInitStruct->PLL.PLLSource == RCMU_PLL_SRC_HSE)
            {
                SET_BIT(RCMU->PLLCR, RCMU_HSE_ON << RCMU_PLLCR_PLL_SRC_Pos);                                  
            } 
            else
            {
                CLEAR_BIT(RCMU->PLLCR, RCMU_HSE_OFF << RCMU_PLLCR_PLL_SRC_Pos);                
            }   
            /* Set PLL_ON bit on */
            SET_BIT(RCMU->CR, RCMU_CR_PLLON);       
        }
        else
        {
            /* Set PLL_ON bit off */
            CLEAR_BIT(RCMU->CR, RCMU_CR_PLLON);             
        }

        /* Get Start Tick */
        tickstart = HAL_GetTick();

        /* Wait till HSI is ready */
        while (__HAL_RCMU_GET_REG_FLAG(RCMU->CR, RCMU_CR_PLLRDY, RCMU_CR_PLLRDY_Pos) == RESET)
        {
            if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}
/**
  * @brief  Initializes the CPU, AHB and APB buses clocks according to the specified 
  *         parameters in the RCMU_ClkInitStruct.
  * @param  RCMU_ClkInitStruct pointer to an RCMU_OscInitTypeDef structure that
  *         contains the configuration information for the RCMU peripheral.
  * @param  FLatency FLASH Latency                   
  *          The value of this parameter depend on device used within the same series
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency 
  *         and updated by @ref HAL_RCMU_GetHCLKFreq() function called within this function
  *
  * @note   The HSI is used (enabled by hardware) as system clock source after
  *         start-up from Reset, wake-up from STOP and STANDBY mode, or in case
  *         of failure of the HSE used directly or indirectly as system clock
  *         (if the Clock Security System CSS is enabled).
  *           
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after start-up delay or PLL locked). 
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready. 
  *         You can use @ref HAL_RCMU_GetClockConfig() function to know which clock is
  *         currently used as system clock source.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCMU_ClockConfig(RCMU_ClkInitTypeDef  *RCMU_ClkInitStruct, uint32_t FLatency)
{
    uint32_t tickstart = 0U;

    /* To correctly read data from FLASH memory, the number of wait states (LATENCY) 
    must be correctly programmed according to the frequency of the CPU clock 
    (HCLK) of the device. */
    /* Check the parameters */

    assert_param(RCMU_ClkInitStruct != NULL);
    #if defined(FLASH_ACR_LATENCY)
    assert_param(IS_FLASH_LATENCY(FLatency));
    /* Increasing the number of wait states because of higher CPU frequency */

    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLatency)
    {
        return HAL_ERROR;
    }
    #endif /* FLASH_ACR_LATENCY */    

    /*------------------------- SYSCLK Configuration ---------------------------*/ 
    assert_param(IS_RCMU_SYSCLKSOURCE(RCMU_ClkInitStruct->SYSCLKSource));

    if(RCMU_ClkInitStruct->SYSCLKSource == RCMU_SYSCLKSOURCE_PLLCLK)
    {
        if(READ_BIT(RCMU->CR, RCMU_CR_PLLON) == RCMU_PLL_OFF)
        {
            return HAL_ERROR;
        }
    }
    else if(RCMU_ClkInitStruct->SYSCLKSource == RCMU_SYSCLKSOURCE_LSI)
    {
        CLEAR_BIT(RCMU->CFGR, RCMU_CFGR_SLOW_SEL);      
    }
    else if(RCMU_ClkInitStruct->SYSCLKSource == RCMU_SYSCLKSOURCE_LSE)
    {
        SET_BIT(RCMU->CFGR, RCMU_CFGR_SLOW_SEL);        
    }
    else
    {
        /* */
    }

    SET_BIT(RCMU->CFGR, RCMU_ClkInitStruct->SYSCLKSource << RCMU_CFGR_SW_Pos);    

    /* Get Start Tick */
    tickstart = HAL_GetTick();

    while (__HAL_RCMU_GET_REG_FLAG(RCMU->CFGR, RCMU_CFGR_SWS, RCMU_CFGR_SWS_Pos) != RCMU_ClkInitStruct->SYSCLKSource)
    {
        if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }
        
    /*-------------------------- HCLK Configuration ---------------------------*/ 
    assert_param(IS_RCMU_HCLK(RC MU_ClkInitStruct->AHBCLKDivider));
    SET_BIT(RCMU->CFGR, (RCMU_ClkInitStruct->AHBCLKDivider) << RCMU_CFGR_HPRE_Pos);
    /*-------------------------- PCLK1 Configuration ---------------------------*/ 

    assert_param(IS_RCMU_PCLK(RCMU_ClkInitStruct->APB1CLKDivider));
    SET_BIT(RCMU->CFGR, (RCMU_ClkInitStruct->APB1CLKDivider) << RCMU_CFGR_PPRE1_Pos);
    /*-------------------------- PCLK2 Configuration ---------------------------*/ 

    assert_param(IS_RCMU_PCLK(RCMU_ClkInitStruct->APB2CLKDivider));
    SET_BIT(RCMU->CFGR, (RCMU_ClkInitStruct->APB2CLKDivider) << RCMU_CFGR_PPRE2_Pos);

    /* Update the SystemCoreClock global variable */
    SystemCoreClock = HAL_RCMU_GetSysClockFreq() >> ((RCMU->CFGR & RCMU_CFGR_HPRE) >> RCMU_CFGR_HPRE_Pos);

    /* Configure the source of time base considering new system clocks settings*/
    HAL_InitTick (TICK_INT_PRIORITY);

    return HAL_OK;
}


/**
  * \brief          MCO_Output_Ctrl
  * \note           None
  * \param[in]      mcox,clksrc,clkdiv
  * \param[out]     None
  * \retval         None 
  */
void HAL_RCMU_MCO_Output_Ctrl(uint32_t mcox, uint32_t clksrc, uint32_t clkdiv)
{
    /* Check the parameters */
    assert_param(IS_RCMU_MCO(mcox));
    assert_param(IS_RCMU_MCODIV(clkdiv));
    assert_param(IS_RCMU_MCO1SOURCE(clksrc));

    /* Prevent unused argument(s) compilation warning */
    UNUSED(mcox);
    SET_BIT(RCMU->CFGR, clksrc << RCMU_CFGR_MCO_Pos);  
    SET_BIT(RCMU->CFGR, clkdiv << RCMU_CFGR_MCOPRE_Pos);       
}
/**
  * @brief  Returns the SYSCLK frequency     
  * @note   The system frequency computed by this function is not the real 
  *         frequency in the chip. It is calculated based on the predefined 
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns a value based on HSE_VALUE
  *           divided by PREDIV factor(**)
  * @note     If SYSCLK source is PLL, function returns a value based on HSE_VALUE
  *           divided by PREDIV factor(**) or HSI_VALUE(*) multiplied by the PLL factor.
  * @note     (*) HSI_VALUE is a constant defined in slm32x030_hal_conf.h file (default value
  *               8 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in slm32x030_hal_conf.h file (default value
  *                8 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *                  
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *           
  * @note   This function can be used by the user application to compute the 
  *         baud-rate for the communication peripherals or configure other parameters.
  *           
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *         
  * @retval SYSCLK frequency
  */

uint32_t HAL_RCMU_GetSysClockFreq(void)
{
    uint32_t prediv = 0U, div = 0U, pllclk = 0U, pllmul = 0U;
    uint32_t tmpreg = 0U;
    uint32_t sysclockfreq = 0U;
    uint8_t current_plltype = 0U;

    tmpreg = RCMU->CFGR;

    /* Get SYSCLK source -------------------------------------------------------*/
    switch ((tmpreg & RCMU_CFGR_SWS) >> RCMU_CFGR_SWS_Pos)
    {
        case RCMU_SYSCLKSOURCE_STATUS_HSI:  /* HSI used as system clock source */
            sysclockfreq = HSI_VALUE;
            break;

        case RCMU_SYSCLKSOURCE_STATUS_HSE:  /* HSE used as system clock */
            sysclockfreq = HSE_VALUE;
            break;

        case RCMU_SYSCLKSOURCE_STATUS_PLLCLK:  /* PLL2 used as system clock */
            current_plltype = HAL_RCMU_PllTypeGet();
            tmpreg =  RCMU->PLLCR;

            if(current_plltype == CFG0)
            {
                
                pllmul = (uint32_t)(RCMU->PLLCR & RCMU_PLLCR_CFG0_M) >> RCMU_PLLCR_CFG0_M_Pos;
                prediv = (uint32_t)(RCMU->PLLCR & RCMU_PLLCR_CFG0_N) >> RCMU_PLLCR_CFG0_N_Pos;
                div = (uint32_t)(RCMU->PLLCR & RCMU_PLLCR_CFG0_OD) >> RCMU_PLLCR_CFG0_OD_Pos;
                if ((tmpreg & RCMU_PLLCR_PLL_SRC) != 0)
                {
                    /* HSE used as PLL clock source, only for pllcr_cfg0 */
                    pllclk = (uint32_t)(HSE_VALUE * pllmul);
                }
                else
                {
                    /* HSE used as PLL clock source, only for pllcr_cfg0 */
                    pllclk = (uint32_t)(HSI_VALUE * pllmul);
                }
            }
            else if(current_plltype == CFG1)
            {

                pllmul = (uint32_t)(RCMU->PLLCR2 & RCMU_PLLCR2_CFG1_M) >> RCMU_PLLCR2_CFG1_M_Pos;
                prediv = (uint32_t)(RCMU->PLLCR2 & RCMU_PLLCR2_CFG1_N) >> RCMU_PLLCR2_CFG1_N_Pos;
                div = (uint32_t)(RCMU->PLLCR2 & RCMU_PLLCR2_CFG1_OD) >> RCMU_PLLCR2_CFG1_OD_Pos;
                if ((tmpreg & RCMU_PLLCR_PLL_SRC) != 0)
                {
                    /* HSE used as PLL clock source, only for pllcr_cfg1 */
                    pllclk = (uint32_t)((HSE_VALUE/(1+prediv))*pllmul/(1<<div));

                }
                else
                {
                    /* HSI used as PLL clock source, only for pllcr_cfg1 */ 
                    pllclk = (uint32_t)((HSI_VALUE/(1+prediv))*pllmul/(1<<div));
                }
            }
            else
            {
                /* */
            }
            sysclockfreq = pllclk;
            break;

        case RCMU_SYSCLKSOURCE_STATUS_LSI_LSE:  /* LSI used as system clock source */
            if((tmpreg & RCMU_CFGR_SLOW_SEL) != 0)
            {
                sysclockfreq = LSE_VALUE;
            }
            else
            {
                sysclockfreq = LSI_VALUE;
            }
            break;
    }
    return sysclockfreq;
}


/**
  * brief     set pll_type
  * param   pll_type
  * retval  None 
  */
void HAL_RCMU_PllTypeSet(RCMU_OscInitTypeDef* hrcmu)
{
    pll_type = hrcmu->PLL.PLLType;
}

/**
  * brief     get pll_type
  * param   None
  * retval  None 
  */
uint8_t HAL_RCMU_PllTypeGet(void)
{
    return pll_type;
}

/**
  * @brief  Returns the HCLK frequency
  * @note   Each time HCLK changes, this function must be called to update the
  *         right HCLK value. Otherwise, any configuration based on this function will be incorrect.
  * 
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency 
  *         and updated within this function
  * @retval HCLK frequency
  */
uint32_t HAL_RCMU_GetHCLKFreq(void)
{
    return SystemCoreClock;
}

/**
  * @brief  Returns the PCLK1 frequency
  * @note   Each time PCLK1 changes, this function must be called to update the
  *         right PCLK1 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK1 frequency
  */
uint32_t HAL_RCMU_GetPCLK1Freq(void)
{
    /* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
    return HAL_RCMU_GetHCLKFreq() / (((RCMU->CFGR & RCMU_CFGR_PPRE1) >> RCMU_CFGR_PPRE1_Pos) + 1);
}

/**
  * @brief  Returns the PCLK2 frequency
  * @note   Each time PCLK2 changes, this function must be called to update the
  *         right PCLK2 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK2 frequency
  */
uint32_t HAL_RCMU_GetPCLK2Freq(void)
{
    /* Get HCLK source and Compute PCLK2 frequency ---------------------------*/
    return HAL_RCMU_GetHCLKFreq() / (((RCMU->CFGR & RCMU_CFGR_PPRE2) >> RCMU_CFGR_PPRE2_Pos) + 1);
} 

/**
  * @brief  Configures the RCMU_OscInitStruct according to the internal 
  * RCMU configuration registers.
  * @param  RCMU_OscInitStruct pointer to an RCMU_OscInitTypeDef structure that 
  * will be configured.
  * @retval None
  */
void HAL_RCMU_GetOscConfig(RCMU_OscInitTypeDef  *RCMU_OscInitStruct)
{
    /* Check the parameters */
    assert_param(RCMU_OscInitStruct != NULL);

    /* Get the HSE configuration -----------------------------------------------*/
    if((RCMU->CR & RCMU_CR_HSEON) == RCMU_CR_HSEON)
    {
        RCMU_OscInitStruct->HSEState = RCMU_HSE_ON;
    }
    else
    {
        RCMU_OscInitStruct->HSEState = RCMU_HSE_OFF;
    }

    if((RCMU->CR & RCMU_CR_HSEBYP) == RCMU_CR_HSEBYP)
    {
        RCMU_OscInitStruct->HSEState = RCMU_HSEBYP_N;
    }
    else
    {
        RCMU_OscInitStruct->HSEState = RCMU_HSEBYP;
    }

    /* Get the HSI configuration -----------------------------------------------*/
    if((RCMU->CR &RCMU_CR_HSION) == RCMU_CR_HSION)
    {
        RCMU_OscInitStruct->HSIState = RCMU_HSI_ON;
    }
    else
    {
        RCMU_OscInitStruct->HSIState = RCMU_HSI_OFF;
    }

    /* Get the LSI configuration -----------------------------------------------*/
    if((RCMU->CSR &RCMU_CSR_LSION) == RCMU_CSR_LSION)
    {
        RCMU_OscInitStruct->LSIState = RCMU_LSI_ON;
    }
    else
    {
        RCMU_OscInitStruct->LSIState = RCMU_LSI_OFF;
    }

    /* Get the LSE configuration -----------------------------------------------*/
    if((RCMU->BDCR & RCMU_BDCR_LSEON) == RCMU_BDCR_LSEON)
    {
        RCMU_OscInitStruct->LSEState = RCMU_LSE_ON;
    }
    else
    {
        RCMU_OscInitStruct->LSEState = RCMU_LSE_OFF;
    }

    /* Get the PLL configuration -----------------------------------------------*/
    if((RCMU->CR &RCMU_CR_PLLON) == RCMU_CR_PLLON)
    {
        RCMU_OscInitStruct->PLL.PLLState = RCMU_PLL_ON;
    }
    else
    {
        RCMU_OscInitStruct->PLL.PLLState = RCMU_PLL_OFF;
    }

    RCMU_OscInitStruct->PLL.PLLSource = (uint32_t)(RCMU->PLLCR & RCMU_PLLCR_PLL_SRC);
    RCMU_OscInitStruct->PLL.PLLMUL = (uint32_t)(RCMU->PLLCR & RCMU_PLLCR_CFG0_M);
#if defined(RCMU_CR_PLL2ON)
    /* Get the PLL2 configuration -----------------------------------------------*/
    if((RCMU->CR &RCMU_CR_PLL2ON) == RCMU_CR_PLL2ON)
    {
        RCMU_OscInitStruct->PLL2.PLL2State = RCMU_PLL2_ON;
    }
    else
    {
        RCMU_OscInitStruct->PLL2.PLL2State = RCMU_PLL2_OFF;
    }
    RCMU_OscInitStruct->PLL2.HSEPrediv2Value = __HAL_RCMU_HSE_GET_PREDIV2();
    RCMU_OscInitStruct->PLL2.PLL2MUL = (uint32_t)(RCMU->CFGR2 & RCMU_CFGR2_PLL2MUL);
#endif /* RCMU_CR_PLL2ON */
}

/**
  * @brief  Get the RCMU_ClkInitStruct according to the internal 
  * RCMU configuration registers.
  * @param  RCMU_ClkInitStruct pointer to an RCMU_ClkInitTypeDef structure that 
  * contains the current clock configuration.
  * @param  pFLatency Pointer on the Flash Latency.
  * @retval None
  */
void HAL_RCMU_GetClockConfig(RCMU_ClkInitTypeDef  *RCMU_ClkInitStruct, uint32_t *pFLatency)
{
    /* Check the parameters */
    assert_param(RCMU_ClkInitStruct != NULL);
    assert_param(pFLatency != NULL);

    /* Set all possible values for the Clock type parameter --------------------*/
    RCMU_ClkInitStruct->ClockType = RCMU_CLOCKTYPE_SYSCLK | RCMU_CLOCKTYPE_HCLK | RCMU_CLOCKTYPE_PCLK1 | RCMU_CLOCKTYPE_PCLK2;

    /* Get the SYSCLK configuration --------------------------------------------*/ 
    RCMU_ClkInitStruct->SYSCLKSource = (uint32_t)((RCMU->CFGR & RCMU_CFGR_SWS) >> RCMU_CFGR_SWS_Pos);

    /* Get the HCLK configuration ----------------------------------------------*/ 
    RCMU_ClkInitStruct->AHBCLKDivider = (uint32_t)((RCMU->CFGR & RCMU_CFGR_HPRE) >> RCMU_CFGR_HPRE_Pos); 

    /* Get the APB1 configuration ----------------------------------------------*/ 
    RCMU_ClkInitStruct->APB1CLKDivider = (uint32_t)((RCMU->CFGR & RCMU_CFGR_PPRE1) >> RCMU_CFGR_PPRE1_Pos);   

    /* Get the APB2 configuration ----------------------------------------------*/ 
    RCMU_ClkInitStruct->APB2CLKDivider = (uint32_t)((RCMU->CFGR & RCMU_CFGR_PPRE2) >> RCMU_CFGR_PPRE2_Pos);

    #if   defined(FLASH_ACR_LATENCY)
        /* Get the Flash Wait State (Latency) configuration ------------------------*/   
        *pFLatency = (uint32_t)(FLASH->ACR & FLASH_ACR_LATENCY); 
    #else
        /* For VALUE lines devices, only LATENCY_0 can be set*/
        *pFLatency = (uint32_t)FLASH_LATENCY_0; 
    #endif
}

/**
  * @brief This function handles the RCMU CSS interrupt request.
  * @note This API should be called under the NMI_Handler().
  * @retval None
  */
void HAL_RCMU_NMI_IRQHandler(void)
{
    /* Check RCMU CSSF flag  */
    if(READ_BIT(RCMU->CIR,RCMU_CIR_CSSF))  
    {
        /* RCMU Clock Security System interrupt user callback */
        HAL_RCMU_CSSCallback();

        /* Clear RCMU CSS pending bit */
        CLEAR_BIT(RCMU->CIR,RCMU_CIR_CSSF);
    }
}

/**
  * @brief  This function provides delay (in milliseconds) based on CPU cycles method.
  * @param  mdelay: specifies the delay time length, in milliseconds.
  * @retval None
  */
static void RCMU_Delay(uint32_t mdelay)
{
    __IO uint32_t Delay = mdelay * (SystemCoreClock / 64U / 1000U);
    do
    {
        __NOP();
    } while (Delay--);
}

/**
  * @brief  RCMU Clock Security System interrupt callback
  * @retval none
  */
__weak void HAL_RCMU_CSSCallback(void)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_RCMU_CSSCallback could be implemented in the user file
    */ 
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCMU_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */
