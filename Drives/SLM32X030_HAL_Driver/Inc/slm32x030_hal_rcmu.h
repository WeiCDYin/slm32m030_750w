/**
  ******************************************************************************
  * @file    slm32x030_hal_rcmu.h
  * @author  Application Team
  * @version V1.0.0
  * @date    8-Nov-2024 
  * @brief   Header file of RCMU HAL module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SLM32X030_HAL_RCMU_H_
#define _SLM32X030_HAL_RCMU_H_

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @addtogroup SLM32X030_HAL_Driver
  * @{
  */

/** @addtogroup RCMU
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup RCMU_Exported_Types RCMU Exported Types
  * @{
  */

/**
  * @brief  pll type 
  */
typedef enum
{
    CFG0 = 0U,
    CFG1
}Pll_Type;
/** 
  * @brief  RCMU PLL configuration structure definition  
  */
typedef struct
{
    uint32_t PLLState;      /*!< PLLState: The new state of the PLL.
                              This parameter can be a value of @ref RCMU_PLL_Config */
    Pll_Type PLLType;       /*!< PLLType: select CFG0 or CFG1*/
    uint32_t PLLSource;     /*!< PLLSource: PLL entry clock source.
                              This parameter must be a value of @ref RCMU_PLL_Clock_Source */          
    uint32_t PLLMUL;        /*!< PLLMUL: Multiplication factor for PLL VCO input clock
                              This parameter must be a value of @ref RCCEx_PLL_Multiplication_Factor */
    uint32_t PLLPreDiv;               
    uint32_t PLLFreq;       /* PLL output frequency */                    
} RCMU_PLLInitTypeDef;
   
/**
  * @brief  RCMU System, AHB and APB busses clock configuration structure definition  
  */
typedef struct
{
    uint32_t ClockType;             /*!< unused */
    uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                       This parameter can be a value of @ref RCMU_System_Clock_Source */
    uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                       This parameter can be a value of @ref RCMU_AHB_Clock_Source */
    uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCMU_APB1_APB2_Clock_Source */
    uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCMU_APB1_APB2_Clock_Source */
} RCMU_ClkInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCMU_Exported_Constants RCMU Exported Constants
  * @{
  */

/** @defgroup RCMU_CR Constants
  * @{
  */

/**
  * @brief  RCMU_CR HSI status
  */
#define RCMU_HSI_OFF                        (0x00000000U)                                 /*!< LSE clock deactivation */
#define RCMU_HSI_ON                         (0x00000001U)                                 /*!< LSE clock activation */

/**
  * @brief  CMU_CR HSIRDY
  */
#define RCMU_HSIRDY_N                       (0x00000000U)                                 /*!< LSE clock activation */
#define RCMU_HSIRDY                         (0x00000001U)                                 /*!< LSE clock deactivation */

/**
  * @brief  CMU_CR HSI_FREQ_SEL HSI Frequency range selection£º
  */
#define RCMU_HSI_FREQ_SEL_NULL              (0x00000000U)                                 /*!< NULL */
#define RCMU_HSI_FREQ_SEL_4M                (0x00000001U)                                 /*!< 1:4MHz */          
#define RCMU_HSI_FREQ_SEL_8M                (0x00000002U)                                 /*!< 2:8MHz */




/**
  * @brief HSE_STAB_RST Reset the HSESTAB circuit
  */
#define RCMU_HSE_STAB_NRST                  (0x00000000U)                                 /*!< 0:no reset£» */
#define RCMU_HSE_STAB_RST                   (0x00000001U)                                 /*!< 1:reset£» */

/**
  * @brief HSE_EXT Bypassing the HSE analog circuit and directly using PAD to inject the clock
  */
#define RCMU_HSE_N_EXT                      (0x00000000U)                                 /*!< 0:no reset£» */
#define RCMU_HSE_EXT                        (0x00000001U)                                 /*!< 1:reset£» */

/**
  * @brief HSE_EXT HSE Start Enable
  */
#define RCMU_HSE_OFF                        (0x00000000U)                                 /*!< 0£ºclose HSE£» */
#define RCMU_HSE_ON                         (0x00000001U)                                 /*!< 1£ºstart HSE£» */

/**
  * @brief HSERDY HSE Stable enabling£»
  */
#define RCMU_HSERDY_N                       (0x00000000U)                                 /*!< 0£ºclose HSE£» */
#define RCMU_HSERDY                         (0x00000001U)                                 /*!< 1£ºstart HSE£» */

/**
  * @brief HSERDY HSE Direct enable£»
  */
#define RCMU_HSEBYP_N                       (0x00000000U)                                 /*!< 0£ºHSE Normal oscillation use */
#define RCMU_HSEBYP                         (0x00000001U)                                 /*!< 1£ºInject the clock directly through IN only */

/**
  * @brief RCMU_CR HSE_FORCE_OK 
  */
#define RCMU_HSE_FORCE_OK_PULLDN            (0x00000000U)                                 /*!< 0£ºNo forced lifting HSE_OK*/ 
#define RCMU_HSE_FORCE_OK_PULLUP            (0x00000001U)                                 /*!< 1£ºForced lifting HSE_OK*/

/**
  * @brief RCMU_CR PLL 
  */
#define RCMU_PLL_OFF                        (0x00000000U)                                 /*!< PLL deactivation */
#define RCMU_PLL_ON                         (0x00000001U)                                 /*!< PLL activation */

/**
  * @brief RCMU_CR PLL Locked state¡£
  */
#define RCMU_PLLRDY_LOCK                    (0x00000000U)                                /*!< 0£ºUnlocked*/
#define RCMU_PLLRDY_UNLOCK                  (0x00000001U)                                /*!< 1£ºLocked*/

/**
  * @brief RCMU_CR HSE Stable time configuration that takes effect after clock generation
  */
#define RCMU_HSE_STARTUP_1_CLK              (0x00000000U)
#define RCMU_HSE_STARTUP_128_CLK            (0x00000001U)
#define RCMU_HSE_STARTUP_256_CLK            (0x00000002U)
#define RCMU_HSE_STARTUP_512_CLK            (0x00000003U)
#define RCMU_HSE_STARTUP_1024_CLK           (0x00000004U)
#define RCMU_HSE_STARTUP_2048_CLK           (0x00000005U)
#define RCMU_HSE_STARTUP_4096_CLK           (0x00000006U)
#define RCMU_HSE_STARTUP_16384_CLK          (0x00000007U)

/** @defgroup RCMU_CFGR Clock configuration register£¨RCMU_CFGR£©
  * @{
  */

/**
  * @brief System clock source selection
  */
#define RCMU_SW_HSI                         (0x00000000U)
#define RCMU_SW_HSE                         (0x00000001U)
#define RCMU_SW_PLL                         (0x00000002U)
#define RCMU_SW_LSI                         (0x00000003U)
#define RCMU_SW_LSE                         (0x00000003U)

/**
  * @brief SWS Actual system clock selection source, updated by hardware
  */
#define RCMU_SWS_HSI                        (0x00000000U)
#define RCMU_SWS_HSE                        (0x00000001U)
#define RCMU_SWS_PLL                        (0x00000002U)
#define RCMU_SWS_LSI_LSE                    (0x00000003U)

/**
  * @brief HPRE  HCLK frequency configuration, HCLK is divided from SYSCLK
  */
#define RCMU_HPRE_1_DIV                     (0x00000001U)
#define RCMU_HPRE_2_DIV                     (0x00000002U)
#define RCMU_HPRE_512_DIV                   (0x000001FFU)

/**
  * @brief HPRE  The slow clock source selection signal of SYS-CLK is only effective when SW=3
  */
#define RCMU_SLOW_SEL_LSI                   (0x00000000U)                                /*!< 0£ºLSI; */                     
#define RCMU_SLOW_SEL_LSE                   (0x00000001U)                                /*!< 1£ºLSE£» */  

/**
  * @brief PPRE1  PCLK1 frequency configuration, PCLK1 is derived from HCLK frequency division
  */
#define RCMU_PPRE1_1_DIV                    (0x00000000U)
#define RCMU_PPRE1_2_DIV                    (0x00000001U)
#define RCMU_PPRE1_3_DIV                    (0x00000002U)
#define RCMU_PPRE1_4_DIV                    (0x00000003U)
#define RCMU_PPRE1_5_DIV                    (0x00000004U)
#define RCMU_PPRE1_6_DIV                    (0x00000005U)
#define RCMU_PPRE1_7_DIV                    (0x00000006U)
#define RCMU_PPRE1_8_DIV                    (0x00000007U)
#define RCMU_PPRE1_9_DIV                    (0x00000008U)
#define RCMU_PPRE1_10_DIV                   (0x00000009U)
#define RCMU_PPRE1_11_DIV                   (0x0000000AU)
#define RCMU_PPRE1_12_DIV                   (0x0000000BU)
#define RCMU_PPRE1_13_DIV                   (0x0000000CU)
#define RCMU_PPRE1_14_DIV                   (0x0000000DU)
#define RCMU_PPRE1_15_DIV                   (0x0000000EU)

/**
  * @brief PPRE2  PCLK2 frequency configuration, PCLK2 is derived from HCLK frequency division
  */
#define RCMU_PPRE2_1_DIV                    (0x00000000U)
#define RCMU_PPRE2_2_DIV                    (0x00000001U)
#define RCMU_PPRE2_3_DIV                    (0x00000002U)
#define RCMU_PPRE2_4_DIV                    (0x00000003U)
#define RCMU_PPRE2_5_DIV                    (0x00000004U)
#define RCMU_PPRE2_6_DIV                    (0x00000005U)
#define RCMU_PPRE2_7_DIV                    (0x00000006U)
#define RCMU_PPRE2_8_DIV                    (0x00000007U)
#define RCMU_PPRE2_9_DIV                    (0x00000008U)
#define RCMU_PPRE2_10_DIV                   (0x00000009U)
#define RCMU_PPRE2_11_DIV                   (0x0000000AU)
#define RCMU_PPRE2_12_DIV                   (0x0000000BU)
#define RCMU_PPRE2_13_DIV                   (0x0000000CU)
#define RCMU_PPRE2_14_DIV                   (0x0000000DU)
#define RCMU_PPRE2_15_DIV                   (0x0000000EU)

/**
  * @brief MCO  Output clock source selection
  */
#define RCMU_MCO_NULL                       (0x00000000U)
#define RCMU_MCO_RTC                        (0x00000001U)
#define RCMU_MCO_LSI                        (0x00000002U)
#define RCMU_MCO_LSE                        (0x00000003U)
#define RCMU_MCO_SYS_CLK                    (0x00000004U)
#define RCMU_MCO_HSI                        (0x00000005U)
#define RCMU_MCO_HSE                        (0x00000006U)
#define RCMU_MCO_PLL                        (0x00000007U)

/**
  * @brief MCO  Output clock source selection.
  */
#define RCMU_MCOPRE_1_DIV                   (0x00000000U)
#define RCMU_MCOPRE_2_DIV                   (0x00000001U)
#define RCMU_MCOPRE_4_DIV                   (0x00000002U)
#define RCMU_MCOPRE_8_DIV                   (0x00000003U)
#define RCMU_MCOPRE_16_DIV                  (0x00000004U)
#define RCMU_MCOPRE_32_DIV                  (0x00000005U)
#define RCMU_MCOPRE_64_DIV                  (0x00000006U)
#define RCMU_MCOPRE_128_DIV                 (0x00000007U)

#define RCMU_HSEBYP_NORMAL                  (0x00000000U)
#define RCMU_HSEBYP_IN_CLK                  (0x00000001U)



/** @defgroup RCMU_CR LSI status
  * @{
  */
#define RCMU_LSI_OFF                       (0x00000000U)                                 /*!< LSI clock deactivation */
#define RCMU_LSI_ON                        (0x00000001U)                                 /*!< LSI clock activation */

/** @defgroup RCMU_CR LSI select
  * @{
  */
#define RCMU_LSI_LOW                       (0x00000000U)                                 /*!< LSI low */
#define RCMU_LSI_HIGH                      (0x00000001U)                                 /*!< LSI high */

/**
  * @}
  */

/** @defgroup RCMU_Oscillator_Type Oscillator Type
  * @{
  */
#define RCMU_OSCILLATORTYPE_NONE            (0x00000000U)
#define RCMU_OSCILLATORTYPE_HSE             (0x00000001U)
#define RCMU_OSCILLATORTYPE_HSI             (0x00000002U)
#define RCMU_OSCILLATORTYPE_LSE             (0x00000004U)
#define RCMU_OSCILLATORTYPE_LSI             (0x00000008U)

#define RCMU_SW_SYSCLK_HSI                  (0x00000000U)
#define RCMU_SW_SYSCLK_HSE                  (0x00000001U)
#define RCMU_SW_SYSCLK_PLL                  (0x00000002U)
#define RCMU_SW_SYSCLK_LSE_LSI              (0x00000003U)
#define RCMU_SW_SYSCLK_NULL                 (0x00000004U)


/** @defgroup PLL configuration register（RCMU_PLLCR）
  * @{
  */

/**
  * @brief PLL_SRC  PLL input source selection
  */
#define RCMU_PLL_SRC_HSI                    (0x00000000U)             /*!< HSI clock as PLL entry clock source */ 
#define RCMU_PLL_SRC_HSE                    RCMU_PLLCR_PLL_SRC       /*!< HSE clock as PLL entry clock source */ 


/**
 * @brief CFG0_M    PLL倍频倍数：
 */
#define RCMU_CFG0_M_1_MUL                   (0x00000001U)  /*!< 1:1倍频； */
#define RCMU_CFG0_M_2_MUL                   (0x00000002U)  /*!< 2:2倍频； */
#define RCMU_CFG0_M_3_MUL                   (0x00000003U)  /*!< 3:3倍频； */
#define RCMU_CFG0_M_4_MUL                   (0x00000004U)  /*!< 4:4倍频； */
#define RCMU_CFG0_M_5_MUL                   (0x00000005U)  /*!< 5:5倍频； */
#define RCMU_CFG0_M_6_MUL                   (0x00000006U)  /*!< 6:6倍频； */
#define RCMU_CFG0_M_7_MUL                   (0x00000007U)  /*!< 7:7倍频； */
#define RCMU_CFG0_M_8_MUL                   (0x00000008U)  /*!< 8:8倍频； */
#define RCMU_CFG0_M_9_MUL                   (0x00000009U)  /*!< 9:9倍频； */
#define RCMU_CFG0_M_10_MUL                  (0x0000000AU)  /*!< 9:9倍频； */
#define RCMU_CFG0_M_16_MUL                  (0x00000010U)  /*!< 16:16倍频 */
#define RCMU_CFG0_M_32_MUL                  (0x00000020U)  /*!< 32:32倍频*/

/**
 * @brief CFG0_N    根据PLL输入源频率范围配置
 */
#define RCMU_CFG0_N_LESS_8M                 (0x00000000U)   /*!< 小于8MHz: 0； */
#define RCMU_CFG0_N_8M_TO_16M               (0x00000001U)   /*!< 8到16MHz: 1 */
#define RCMU_CFG0_N_16M_TO_24M              (0x00000002U)   /*!< 16到24MHz: 2 */
#define RCMU_CFG0_N_MORE_24M                (0x00000003U)   /*!< 大于24MHz: 3 */

/**
 * @brief CFG0_OD   根据PLL输出频率范围配置
 */
#define RCMU_CFG0_OD_LESS_16M               (0x00000000U) /*!< 小于16MHz: 0； */
#define RCMU_CFG0_OD_16M_TO_32M             (0x00000001U) /*!< 16到32MHz: 1 */
#define RCMU_CFG0_OD_32M_TO_48M             (0x00000002U) /*!< 32到48MHz: 2 */
#define RCMU_CFG0_OD_48M_TO_64M             (0x00000003U) /*!< 48到64MHz: 3 */

/**
 * @brief CFG1_M    PLL倍频倍数：
 */
#define RCMU_CFG1_M_1_MUL                   (0x00000001U)  /*!< 1:1倍频； */
#define RCMU_CFG1_M_2_MUL                   (0x00000002U)  /*!< 2:2倍频； */
#define RCMU_CFG1_M_3_MUL                   (0x00000003U)  /*!< 3:3倍频； */
#define RCMU_CFG1_M_4_MUL                   (0x00000004U)  /*!< 4:4倍频； */
#define RCMU_CFG1_M_5_MUL                   (0x00000005U)  /*!< 5:5倍频； */
#define RCMU_CFG1_M_6_MUL                   (0x00000006U)  /*!< 6:6倍频； */
#define RCMU_CFG1_M_7_MUL                   (0x00000007U)  /*!< 7:7倍频； */
#define RCMU_CFG1_M_8_MUL                   (0x00000008U)  /*!< 8:8倍频； */
#define RCMU_CFG1_M_9_MUL                   (0x00000009U)  /*!< 9:9倍频； */
#define RCMU_CFG1_M_10_MUL                  (0x0000000AU)  /*!< 9:9倍频； */
#define RCMU_CFG1_M_16_MUL                  (0x00000010U)  /*!< 16:16倍频 */
#define RCMU_CFG1_M_32_MUL                  (0x00000020U)  /*!< 32:32倍频*/

/**
 * @brief CFG1_N
 */
#define RCMU_CFG1_N_DVI1                    (0x00000000U)
#define RCMU_CFG1_N_DVI2                    (0x00000001U)
#define RCMU_CFG1_N_DVI3                    (0x00000002U)
#define RCMU_CFG1_N_DVI4                    (0x00000003U)

/**
 * @brief CFG1_OD
 */
#define RCMU_CFG1_OD_0                      (0x00000000U) /*!< 小于16MHz: 0； */
#define RCMU_CFG1_OD_1                      (0x00000001U) /*!< 16到32MHz: 1 */
#define RCMU_CFG1_OD_2                      (0x00000002U) /*!< 32到48MHz: 2 */
#define RCMU_CFG1_OD_3                      (0x00000003U) /*!< 48到64MHz: 3 */
#define RCMU_LSE_OFF                        (0x00000000U)
#define RCMU_LSE_ON                         (0x00000001U)


/** @defgroup 
  * @{
  */
/**
  * @brief LSION    
  */
#define RCMU_LSI_OFF                        (0x00000000U)
#define RCMU_LSI_ON                         (0x00000001U)

/**
  * @brief CFG1_M   
  */
#define RCMU_CFG1_M_1_MUL                   (0x00000001U)
#define RCMU_CFG1_M_2_MUL                   (0x00000002U)
#define RCMU_CFG1_M_3_MUL                   (0x00000003U)
#define RCMU_CFG1_M_4_MUL                   (0x00000004U)
#define RCMU_CFG1_M_5_MUL                   (0x00000005U)
#define RCMU_CFG1_M_6_MUL                   (0x00000006U)
#define RCMU_CFG1_M_7_MUL                   (0x00000007U)
#define RCMU_CFG1_M_8_MUL                   (0x00000008U)
#define RCMU_CFG1_M_9_MUL                   (0x00000009U)
#define RCMU_CFG1_M_10_MUL                  (0x0000000AU)

/**
  * @brief CFG1_N   
  */
#define RCMU_CFG1_N_8M_PRE_DVI1             (0x00000000U)
#define RCMU_CFG1_N_16M_PRE_DVI2            (0x00000001U)
#define RCMU_CFG1_N_24M_PRE_DVI3            (0x00000002U)
#define RCMU_CFG1_N_32M_PRE_DVI4            (0x00000003U)

/**
  * @brief CFG1_OD  
  */
#define RCMU_CFG1_OD_16M                    (0x00000000U)
#define RCMU_CFG1_OD_32M                    (0x00000001U)
#define RCMU_CFG1_OD_48M                    (0x00000002U)
#define RCMU_CFG1_OD_64M                    (0x00000003U)

/**
  * @}
  */

#define IS_RCMU_HSE(__HSE__)                (((__HSE__) == RCMU_HSE_OFF) || ((__HSE__) == RCMU_HSE_ON))

#define IS_RCMU_HSI(__HSI__)                (((__HSI__) == RCMU_HSI_OFF) || ((__HSI__) == RCMU_HSI_ON))

#define IS_RCMU_PLL(__PLL__)                (((__PLL__) == RCMU_PLL_OFF) || ((__PLL__) == RCMU_PLL_ON))

#define IS_RCMU_LSI(__LSI__)                (((__LSI__) == RCMU_LSI_OFF) || ((__LSI__) == RCMU_LSI_ON))

#define IS_RCMU_LSE(__LSE__)                (((__LSE__) == RCMU_LSE_OFF) || ((__LSE__) == RCMU_LSE_ON))

#define IS_RCMU_PLLSRC(__PLLSRC__)          (((__PLLSRC__) == RCMU_PLL_SRC_HSI) || ((__PLLSRC__) == RCMU_PLL_SRC_HSE))

#define IS_RCMU_PLLPREDIV(__PLLPREDIV__)    (((__PLLPREDIV__) == RCMU_CFG_N_8M_PRE_DVI1) || ((__PLLPREDIV__) == RCMU_CFG_N_16M_PRE_DVI2) ||  \
                                             ((__PLLPREDIV__) == RCMU_CFG_N_24M_PRE_DVI3) || ((__PLLPREDIV__) == RCMU_CFG_N_32M_PRE_DVI4)) 

#define IS_RCMU_PLLFREQ(__PLLFREQ__)        (((__PLLFREQ__) == RCMU_CFG_OD_16M) || ((__PLLFREQ__) == RCMU_CFG_OD_32M) || ((__PLLFREQ__) == RCMU_CFG_OD_48M) ||  \
                                             ((__PLLFREQ__) == RCMU_CFG_OD_64M)) 
/**
  * @}
  */



/**
  * @}
  */

/** @defgroup RCMU_System_Clock_Type System Clock Type
  * @{
  */
#define RCMU_CLOCKTYPE_SYSCLK               (0x00000001U)    /*!< SYSCLK to configure */
#define RCMU_CLOCKTYPE_HCLK                 (0x00000002U)    /*!< HCLK to configure */
#define RCMU_CLOCKTYPE_PCLK1                (0x00000004U)    /*!< PCLK1 to configure */
#define RCMU_CLOCKTYPE_PCLK2                (0x00000008U)    /*!< PCLK2 to configure */

/**
  * @}
  */

/** @defgroup RCMU_System_Clock_Source System Clock Source
  * @{
  */
#define RCMU_SYSCLKSOURCE_HSI               RCMU_SW_HSI     /*!< HSI selected as system clock */
#define RCMU_SYSCLKSOURCE_HSE               RCMU_SW_HSE     /*!< HSE selected as system clock */
#define RCMU_SYSCLKSOURCE_PLLCLK            RCMU_SW_PLL     /*!< PLL selected as system clock */
#define RCMU_SYSCLKSOURCE_LSI               RCMU_SW_LSI     /*!< LSI selected as system clock */
#define RCMU_SYSCLKSOURCE_LSE               RCMU_SW_LSE     /*!< LSE selected as system clock */
/**
  * @}
  */

/** @defgroup RCMU_System_Clock_Source_Status System Clock Source Status
  * @{
  */
#define RCMU_SYSCLKSOURCE_STATUS_HSI        RCMU_SWS_HSI            /*!< HSI used as system clock */
#define RCMU_SYSCLKSOURCE_STATUS_HSE        RCMU_SWS_HSE            /*!< HSE used as system clock */
#define RCMU_SYSCLKSOURCE_STATUS_PLLCLK     RCMU_SWS_PLL            /*!< PLL used as system clock */
#define RCMU_SYSCLKSOURCE_STATUS_LSI_LSE    RCMU_SWS_LSI_LSE        /*!< LSI or LSE used as system clock */


/**
  * @}
  */

/** @defgroup RCMU_AHB_Clock_Source AHB Clock Source
  * @{
  */
#define RCMU_CFGR_HPRE_DIV1                 (0x00000000U)           /*!< SYSCLK not divided */
#define RCMU_CFGR_HPRE_DIV2                 (0x00000001U)           /*!< SYSCLK divided by 2 */
#define RCMU_CFGR_HPRE_DIV4                 (0x00000003U)           /*!< SYSCLK divided by 4 */
#define RCMU_CFGR_HPRE_DIV8                 (0x00000007U)           /*!< SYSCLK divided by 8 */
#define RCMU_CFGR_HPRE_DIV16                (0x0000000FU)           /*!< SYSCLK divided by 16 */
#define RCMU_CFGR_HPRE_DIV64                (0x0000003FU)           /*!< SYSCLK divided by 64 */
#define RCMU_CFGR_HPRE_DIV128               (0x0000007FU)           /*!< SYSCLK divided by 128 */
#define RCMU_CFGR_HPRE_DIV256               (0x000000FFU)           /*!< SYSCLK divided by 256 */
#define RCMU_CFGR_HPRE_DIV512               (0x000001FFU)           /*!< SYSCLK divided by 512 */

#define RCMU_SYSCLK_DIV1                    RCMU_CFGR_HPRE_DIV1     /*!< SYSCLK not divided */
#define RCMU_SYSCLK_DIV2                    RCMU_CFGR_HPRE_DIV2     /*!< SYSCLK divided by 2 */
#define RCMU_SYSCLK_DIV4                    RCMU_CFGR_HPRE_DIV4     /*!< SYSCLK divided by 4 */
#define RCMU_SYSCLK_DIV8                    RCMU_CFGR_HPRE_DIV8     /*!< SYSCLK divided by 8 */
#define RCMU_SYSCLK_DIV16                   RCMU_CFGR_HPRE_DIV16    /*!< SYSCLK divided by 16 */
#define RCMU_SYSCLK_DIV64                   RCMU_CFGR_HPRE_DIV64    /*!< SYSCLK divided by 64 */
#define RCMU_SYSCLK_DIV128                  RCMU_CFGR_HPRE_DIV128   /*!< SYSCLK divided by 128 */
#define RCMU_SYSCLK_DIV256                  RCMU_CFGR_HPRE_DIV256   /*!< SYSCLK divided by 256 */
#define RCMU_SYSCLK_DIV512                  RCMU_CFGR_HPRE_DIV512   /*!< SYSCLK divided by 512 */

/**
  * @}
  */
  
/** @defgroup RCMU_APB1_APB2_Clock_Source APB1 APB2 Clock Source
  * @{
  */
#define RCMU_HCLK_DIV1                      (0x00000000U)  /*!< divided by 1 */
#define RCMU_HCLK_DIV2                      (0x00000001U)  /*!< divided by 2 */
#define RCMU_HCLK_DIV3                      (0x00000002U)  /*!< divided by 3 */
#define RCMU_HCLK_DIV4                      (0x00000003U)  /*!< divided by 4 */
#define RCMU_HCLK_DIV5                      (0x00000004U)  /*!< divided by 5 */
#define RCMU_HCLK_DIV6                      (0x00000005U)  /*!< divided by 6 */
#define RCMU_HCLK_DIV7                      (0x00000006U)  /*!< divided by 7 */
#define RCMU_HCLK_DIV8                      (0x00000007U)  /*!< divided by 8 */
#define RCMU_HCLK_DIV9                      (0x00000008U)  /*!< divided by 9 */
#define RCMU_HCLK_DIV10                     (0x00000009U)  /*!< divided by 10 */
#define RCMU_HCLK_DIV11                     (0x0000000AU)  /*!< divided by 11 */
#define RCMU_HCLK_DIV12                     (0x0000000BU)  /*!< divided by 12 */
#define RCMU_HCLK_DIV13                     (0x0000000CU)  /*!< divided by 13 */
#define RCMU_HCLK_DIV14                     (0x0000000DU)  /*!< divided by 14 */
#define RCMU_HCLK_DIV15                     (0x0000000EU)  /*!< divided by 15 */
#define RCMU_HCLK_DIV16                     (0x0000000FU)  /*!< divided by 16 */

/**
  * @}
  */

/** @defgroup RCMU_RTC_Clock_Source RTC Clock Source
  * @{
  */
#define RCMU_BDCR_RTCSEL_NOCLOCK            (0x00000000U)                       /*!< No clock */
#define RCMU_BDCR_RTCSEL_LSE                (0x00000100U)                       /*!< LSE oscillator clock used as RTC clock */
#define RCMU_BDCR_RTCSEL_LSI                (0x00000200U)                       /*!< LSI oscillator clock used as RTC clock */
#define RCMU_BDCR_RTCSEL_HSE                (0x00000300U)                       /*!< HSE oscillator clock divided by 128 used as RTC clock */

#define RCMU_RTCCLKSOURCE_NO_CLK            (0x00000000U)                       /*!< No clock */
#define RCMU_RTCCLKSOURCE_LSE               RCMU_BDCR_RTCSEL_LSE                /*!< LSE oscillator clock used as RTC clock */
#define RCMU_RTCCLKSOURCE_LSI               RCMU_BDCR_RTCSEL_LSI                /*!< LSI oscillator clock used as RTC clock */
#define RCMU_RTCCLKSOURCE_HSE_DIV128        RCMU_BDCR_RTCSEL_HSE                /*!< HSE oscillator clock divided by 128 used as RTC clock */
/**
  * @}
  */


/** @defgroup RCMU_MCO_Index MCO Index
  * @{
  */
#define RCMU_MCO1                           (0x00000000U)
#define RCMU_MCO                            RCMU_MCO1                            /*!< MCO1 to be compliant with other families with 2 MCOs*/

/** @defgroup RCMU_MCOx_Clock_Prescaler MCO Clock Prescaler
  * @{
  */
#define RCMU_MCODIV_1                       (0U)
#define RCMU_MCODIV_2                       (1U)
#define RCMU_MCODIV_4                       (2U)
#define RCMU_MCODIV_8                       (3U)
#define RCMU_MCODIV_16                      (4U)
#define RCMU_MCODIV_32                      (5U)
#define RCMU_MCODIV_64                      (6U)
#define RCMU_MCODIV_128                     (7U)
/**
  * @}
  */

/** @defgroup RCMU_CFGR MCO 
  * @{
  * 0:Nothing
  * 1:RTC;
  * 2:LSI;
  * 3:LSE;
  * 4:SYS_CLK;
  * 5:HSI;
  * 6:HSE;
  * 7:PLL/2
  */

#define RCMU_MCO_CLK_NULL                   (0x00000000U)
#define RCMU_MCO_CLK_RTC                    (0x00000001U)
#define RCMU_MCO_CLK_LSI                    (0x00000002U)
#define RCMU_MCO_CLK_LSE                    (0x00000003U)
#define RCMU_MCO_CLK_SYS_CLK                (0x00000004U)
#define RCMU_MCO_CLK_HSI                    (0x00000005U)
#define RCMU_MCO_CLK_HSE                    (0x00000006U)
#define RCMU_MCO_CLK_PLL                    (0x00000007U)
/**
  * @}
  */

/** @defgroup RCMU_Interrupt Interrupts
  * @{
  */
#define RCMU_IT_LSIRDY                    ((uint8_t)RCMU_CIR_LSIRDYF)   /*!< LSI Ready Interrupt flag */
#define RCMU_IT_LSERDY                    ((uint8_t)RCMU_CIR_LSERDYF)   /*!< LSE Ready Interrupt flag */
#define RCMU_IT_HSIRDY                    ((uint8_t)RCMU_CIR_HSIRDYF)   /*!< HSI Ready Interrupt flag */
#define RCMU_IT_HSERDY                    ((uint8_t)RCMU_CIR_HSERDYF)   /*!< HSE Ready Interrupt flag */
#define RCMU_IT_PLLRDY                    ((uint8_t)RCMU_CIR_PLLRDYF)   /*!< PLL Ready Interrupt flag */
#define RCMU_IT_CSS                       ((uint8_t)RCMU_CIR_CSSF)      /*!< Clock Security System Interrupt flag */
/**
  * @}
  */ 
  
/** @defgroup RCMU_Flag Flags
  *        Elements values convention: XXXYYYYYb
  *           - YYYYY  : Flag position in the register
  *           - XXX  : Register index
  *                 - 001: CR register
  *                 - 010: BDCR register
  *                 - 011: CSR register
  * @{
  */
/* Flags in the CR register */
#define RCMU_FLAG_HSIRDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCMU_CR_HSIRDY_Pos)) /*!< Internal High Speed clock ready flag */
#define RCMU_FLAG_HSERDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCMU_CR_HSERDY_Pos)) /*!< External High Speed clock ready flag */
#define RCMU_FLAG_PLLRDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCMU_CR_PLLRDY_Pos)) /*!< PLL clock ready flag */

/* Flags in the CSR register */
#define RCMU_FLAG_LSIRDY                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_LSIRDY_Pos))   /*!< Internal Low Speed oscillator Ready */
#define RCMU_FLAG_PINRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_PINRSTF_Pos))  /*!< PIN reset flag */
#define RCMU_FLAG_PORRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_PORRSTF_Pos))  /*!< POR/PDR reset flag */
#define RCMU_FLAG_SFTRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_SFTRSTF_Pos))  /*!< Software Reset flag */
#define RCMU_FLAG_IWDGRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_IWDGRSTF_Pos)) /*!< Independent Watchdog reset flag */
#define RCMU_FLAG_WWDGRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_WWDGRSTF_Pos)) /*!< Window watchdog reset flag */
#define RCMU_FLAG_LPWRRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCMU_CSR_LPWRRSTF_Pos)) /*!< Low-Power reset flag */

/* Flags in the BDCR register */
#define RCMU_FLAG_LSERDY                  ((uint8_t)((BDCR_REG_INDEX << 5U) | RCMU_BDCR_LSERDY_Pos)) /*!< External Low Speed oscillator Ready */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup RCMU_Exported_Macros RCMU Exported Macros
  * @{
  */

/** @defgroup RCMU_Peripheral_Clock_Enable_Disable Peripheral Clock Enable Disable
  * @brief  Enable or disable the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.   
  * @{
  */
#define __HAL_RCMU_DMA1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_DMA1EN); \
                                        /* Delay after an RCMU peripheral clock enabling */  \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_DMA1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_SRAM_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_SRAMEN); \
                                        /* Delay after an RCMU peripheral clock enabling */  \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_SRAMEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_FLITF_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_FLITFEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_FLITFEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_CRC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_CRCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_CRCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_DMA1_CLK_DISABLE()           (RCMU->AHBENR &= ~(RCMU_AHBENR_DMA1EN))
#define __HAL_RCMU_SRAM_CLK_DISABLE()           (RCMU->AHBENR &= ~(RCMU_AHBENR_SRAMEN))
#define __HAL_RCMU_FLITF_CLK_DISABLE()          (RCMU->AHBENR &= ~(RCMU_AHBENR_FLITFEN))
#define __HAL_RCMU_CRC_CLK_DISABLE()            (RCMU->AHBENR &= ~(RCMU_AHBENR_CRCEN))

/**
  * @}
  */

/** @defgroup RCMU_AHB_Peripheral_Clock_Enable_Disable_Status AHB Peripheral Clock Enable Disable Status
  * @brief  Get the enable or disable status of the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCMU_DMA1_IS_CLK_ENABLED()       ((RCMU->AHBENR & (RCMU_AHBENR_DMA1EN)) != RESET)
#define __HAL_RCMU_DMA1_IS_CLK_DISABLED()      ((RCMU->AHBENR & (RCMU_AHBENR_DMA1EN)) == RESET)
#define __HAL_RCMU_SRAM_IS_CLK_ENABLED()       ((RCMU->AHBENR & (RCMU_AHBENR_SRAMEN)) != RESET)
#define __HAL_RCMU_SRAM_IS_CLK_DISABLED()      ((RCMU->AHBENR & (RCMU_AHBENR_SRAMEN)) == RESET)
#define __HAL_RCMU_FLITF_IS_CLK_ENABLED()      ((RCMU->AHBENR & (RCMU_AHBENR_FLITFEN)) != RESET)
#define __HAL_RCMU_FLITF_IS_CLK_DISABLED()     ((RCMU->AHBENR & (RCMU_AHBENR_FLITFEN)) == RESET)
#define __HAL_RCMU_CRC_IS_CLK_ENABLED()        ((RCMU->AHBENR & (RCMU_AHBENR_CRCEN)) != RESET)
#define __HAL_RCMU_CRC_IS_CLK_DISABLED()       ((RCMU->AHBENR & (RCMU_AHBENR_CRCEN)) == RESET)

/**
  * @}
  */

/** @defgroup RCMU_APB1_Clock_Enable_Disable APB1 Clock Enable Disable
  * @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  * @{   
  */

/**
  * @}
  */

/** @defgroup RCMU_APB1_Peripheral_Clock_Enable_Disable_Status APB1 Peripheral Clock Enable Disable Status
  * @brief  Get the enable or disable status of the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */



/**
  * @}
  */

/** @defgroup RCMU_APB2_Clock_Enable_Disable APB2 Clock Enable Disable
  * @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @{   
  */
#define __HAL_RCMU_DMA_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_DMAEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_DMAEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_SRAM_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_SRAMEN); \
                                        /* Delay after an RCMU peripheral clock enabling */  \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_SRAMEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_FLITF_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_FLITFEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_FLITFEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_TRNG_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_TRNGEN); \
                                        /* Delay after an RCMU peripheral clock enabling */  \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_TRNGEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  
                                
#define __HAL_RCMU_CRC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_CRCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_CRCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_AES_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_AESEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_AESEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)     

#define __HAL_RCMU_GPIOA_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_IOAEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_IOAEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_GPIOB_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_IOBEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_IOBEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_GPIOC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_IOCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_IOCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_GPIOD_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->AHBENR, RCMU_AHBENR_IODEN); \
                                        /* Delay after an RCMU peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCMU->AHBENR, RCMU_AHBENR_IODEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_SYSCFG_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_SYSCFGEN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_SYSCFGEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_IEMU_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_IEMUEN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_IEMUEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_ADC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_ADCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_ADCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_AWKTIM_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_AWKTIMEN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_AWKTIMEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_TIM1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM1EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_SPI1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_SPI1EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_SPI1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_USART1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_USART1EN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_USART1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_LPTIM_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_LPTIMEN); \
                                        /* Delay after an RCMU peripheral clock enabling */     \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_LPTIMEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_TIM15_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM15EN); \
                                        /* Delay after an RCMU peripheral clock enabling */     \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM15EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_TIM16_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM16EN); \
                                        /* Delay after an RCMU peripheral clock enabling */     \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM16EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_TIM17_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM17EN); \
                                        /* Delay after an RCMU peripheral clock enabling */     \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_TIM17EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCMU_LVD_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_LVDEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_LVDEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)                                      

#define __HAL_RCMU_BEEP_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_BEEPEN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_BEEPEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_DBGMCU_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB2ENR, RCMU_APB2ENR_DBGMCUEN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB2ENR, RCMU_APB2ENR_DBGMCUEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_TIM3_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM3EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM3EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)                                        

#define __HAL_RCMU_TIM6_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM6EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM6EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_TIM7_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM7EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM7EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_TIM14_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM14EN); \
                                        /* Delay after an RCMU peripheral clock enabling */     \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_TIM14EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)  

#define __HAL_RCMU_RTC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_RTCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_RTCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)    

#define __HAL_RCMU_WWDG_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_WWDGEN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_WWDGEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)                                                                             

#define __HAL_RCMU_IWDG_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_IWDGEN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_IWDGEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)   

#define __HAL_RCMU_SPI2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI2EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_SPI2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0) 

#define __HAL_RCMU_USART2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USART2EN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_USART2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)     

#define __HAL_RCMU_LPUART_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_LPUARTEN); \
                                        /* Delay after an RCMU peripheral clock enabling */      \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_LPUARTEN); \
                                        UNUSED(tmpreg); \
                                      } while(0) 

#define __HAL_RCMU_I2C1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C1EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)                                       

#define __HAL_RCMU_I2C2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C2EN); \
                                        /* Delay after an RCMU peripheral clock enabling */    \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_I2C2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0)     

#define __HAL_RCMU_VC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_VCEN); \
                                        /* Delay after an RCMU peripheral clock enabling */  \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_VCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)     

#define __HAL_RCMU_PMU_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCMU->APB1ENR, RCMU_APB1ENR_PMUEN); \
                                        /* Delay after an RCMU peripheral clock enabling */   \
                                        tmpreg = READ_BIT(RCMU->APB1ENR, RCMU_APB1ENR_PMUEN); \
                                        UNUSED(tmpreg); \
                                      } while(0)   


#define __HAL_RCMU_DMA_CLK_DISABLE()                        (RCMU->AHBENR &= ~(RCMU_AHBENR_DMAEN))

#define __HAL_RCMU_SRAM_CLK_DISABLE()                       (RCMU->AHBENR &= ~(RCMU_AHBENR_SRAMEN))

#define __HAL_RCMU_FLITF_CLK_DISABLE()                      (RCMU->AHBENR &= ~(RCMU_AHBENR_FLITFEN))

#define __HAL_RCMU_TRNG_CLK_DISABLE()                       (RCMU->AHBENR &= ~(RCMU_AHBENR_TRNGEN)) 
                            
#define __HAL_RCMU_CRC_CLK_DISABLE()                        (RCMU->AHBENR &= ~(RCMU_AHBENR_CRCEN)) 

#define __HAL_RCMU_AES_CLK_DISABLE()                        (RCMU->AHBENR &= ~(RCMU_AHBENR_AESEN)) 

#define __HAL_RCMU_GPIOA_CLK_DISABLE()                      (RCMU->AHBENR &= ~(RCMU_AHBENR_IOAEN)) 

#define __HAL_RCMU_GPIOB_CLK_DISABLE()                      (RCMU->AHBENR &= ~(RCMU_AHBENR_IOBEN)) 

#define __HAL_RCMU_GPIOC_CLK_DISABLE()                      (RCMU->AHBENR &= ~(RCMU_AHBENR_IOCEN)) 

#define __HAL_RCMU_GPIOD_CLK_DISABLE()                      (RCMU->AHBENR &= ~(RCMU_AHBENR_IODEN)) 

#define __HAL_RCMU_SYSCFG_CLK_DISABLE()                     (RCMU->APB2ENR &= ~(RCMU_APB2ENR_SYSCFGEN)) 

#define __HAL_RCMU_IEMU_CLK_DISABLE()                       (RCMU->APB2ENR &= ~(RCMU_APB2ENR_IEMUEN)) 

#define __HAL_RCMU_ADC_CLK_DISABLE()                        (RCMU->APB2ENR &= ~(RCMU_APB2ENR_ADCEN)) 

#define __HAL_RCMU_AWKTIM_CLK_DISABLE()                     (RCMU->APB2ENR &= ~(RCMU_APB2ENR_AWKTIMEN)) 

#define __HAL_RCMU_TIM1_CLK_DISABLE()                       (RCMU->APB2ENR &= ~(RCMU_APB2ENR_TIM1EN)) 

#define __HAL_RCMU_SPI1_CLK_DISABLE()                       (RCMU->APB2ENR &= ~(RCMU_APB2ENR_SPI1EN)) 

#define __HAL_RCMU_USART1_CLK_DISABLE()                     (RCMU->APB2ENR &= ~(RCMU_APB2ENR_USART1EN)) 

#define __HAL_RCMU_LPTIM_CLK_DISABLE()                      (RCMU->APB2ENR &= ~(RCMU_APB2ENR_LPTIMEN)) 

#define __HAL_RCMU_TIM15_CLK_DISABLE()                      (RCMU->APB2ENR &= ~(RCMU_APB2ENR_TIM15EN)) 

#define __HAL_RCMU_TIM16_CLK_DISABLE()                      (RCMU->APB2ENR &= ~(RCMU_APB2ENR_TIM16EN)) 

#define __HAL_RCMU_TIM17_CLK_DISABLE()                      (RCMU->APB2ENR &= ~(RCMU_APB2ENR_TIM17EN)) 

#define __HAL_RCMU_LVD_CLK_DISABLE()                        (RCMU->APB2ENR &= ~(RCMU_APB2ENR_LVDEN))                                  

#define __HAL_RCMU_BEEP_CLK_DISABLE()                       (RCMU->APB2ENR &= ~(RCMU_APB2ENR_BEEPEN)) 

#define __HAL_RCMU_DBGMCU_CLK_DISABLE()                     (RCMU->APB2ENR &= ~(RCMU_APB2ENR_DBGMCUEN)) 

#define __HAL_RCMU_TIM3_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM3EN))                                     

#define __HAL_RCMU_TIM6_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM6EN))  

#define __HAL_RCMU_TIM7_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM7EN))  

#define __HAL_RCMU_TIM14_CLK_DISABLE()                      (RCMU->APB1ENR &= ~(RCMU_APB1ENR_TIM14EN))  

#define __HAL_RCMU_RTC_CLK_DISABLE()                        (RCMU->APB1ENR &= ~(RCMU_APB1ENR_RTCEN)) 

#define __HAL_RCMU_WWDG_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_WWDGEN)) 
                                                                              
#define __HAL_RCMU_IWDG_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_IWDGEN))  

#define __HAL_RCMU_SPI2_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_SPI2EN))

#define __HAL_RCMU_USART2_CLK_DISABLE()                     (RCMU->APB1ENR &= ~(RCMU_APB1ENR_USART2EN))

#define __HAL_RCMU_LPUART_CLK_DISABLE()                     (RCMU->APB1ENR &= ~(RCMU_APB1ENR_LPUARTEN))

#define __HAL_RCMU_I2C1_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_I2C1EN))                                  

#define __HAL_RCMU_I2C2_CLK_DISABLE()                       (RCMU->APB1ENR &= ~(RCMU_APB1ENR_I2C2EN))   

#define __HAL_RCMU_VC_CLK_DISABLE()                         (RCMU->APB1ENR &= ~(RCMU_APB1ENR_VCEN))  
#define __HAL_RCMU_PMU_CLK_DISABLE()                        (RCMU->APB1ENR &= ~(RCMU_APB1ENR_PMUEN))


/**
  * @}
  */
  
/** @defgroup RCMU_APB2_Peripheral_Clock_Enable_Disable_Status APB2 Peripheral Clock Enable Disable Status
  * @brief  Get the enable or disable status of the APB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCMU_AFIO_IS_CLK_ENABLED()                    ((RCMU->APB2ENR & (RCMU_APB2ENR_AFIOEN)) != RESET)
#define __HAL_RCMU_AFIO_IS_CLK_DISABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_AFIOEN)) == RESET)
#define __HAL_RCMU_GPIOA_IS_CLK_ENABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPAEN)) != RESET)
#define __HAL_RCMU_GPIOA_IS_CLK_DISABLED()                  ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPAEN)) == RESET)
#define __HAL_RCMU_GPIOB_IS_CLK_ENABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPBEN)) != RESET)
#define __HAL_RCMU_GPIOB_IS_CLK_DISABLED()                  ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPBEN)) == RESET)
#define __HAL_RCMU_GPIOC_IS_CLK_ENABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPCEN)) != RESET)
#define __HAL_RCMU_GPIOC_IS_CLK_DISABLED()                  ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPCEN)) == RESET)
#define __HAL_RCMU_GPIOD_IS_CLK_ENABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPDEN)) != RESET)
#define __HAL_RCMU_GPIOD_IS_CLK_DISABLED()                  ((RCMU->APB2ENR & (RCMU_APB2ENR_IOPDEN)) == RESET)
#define __HAL_RCMU_ADC1_IS_CLK_ENABLED()                    ((RCMU->APB2ENR & (RCMU_APB2ENR_ADC1EN)) != RESET)
#define __HAL_RCMU_ADC1_IS_CLK_DISABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_ADC1EN)) == RESET)
#define __HAL_RCMU_TIM1_IS_CLK_ENABLED()                    ((RCMU->APB2ENR & (RCMU_APB2ENR_TIM1EN)) != RESET)
#define __HAL_RCMU_TIM1_IS_CLK_DISABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_TIM1EN)) == RESET)
#define __HAL_RCMU_SPI1_IS_CLK_ENABLED()                    ((RCMU->APB2ENR & (RCMU_APB2ENR_SPI1EN)) != RESET)
#define __HAL_RCMU_SPI1_IS_CLK_DISABLED()                   ((RCMU->APB2ENR & (RCMU_APB2ENR_SPI1EN)) == RESET)
#define __HAL_RCMU_USART1_IS_CLK_ENABLED()                  ((RCMU->APB2ENR & (RCMU_APB2ENR_USART1EN)) != RESET)
#define __HAL_RCMU_USART1_IS_CLK_DISABLED()                 ((RCMU->APB2ENR & (RCMU_APB2ENR_USART1EN)) == RESET)

/**
  * @}
  */

/** @defgroup RCMU_APB1_Force_Release_Reset APB1 Force Release Reset
  * @brief  Force or release APB1 peripheral reset.
  * @{   
  */
#define __HAL_RCMU_APB1_FORCE_RESET()                       (RCMU->APB2RSTR = 0xFFFFFFFFU)  
#define __HAL_RCMU_TIM2_FORCE_RESET()                       (RCMU->APB1RSTR |= (RCMU_APB1RSTR_TIM2RST))
#define __HAL_RCMU_TIM3_FORCE_RESET()                       (RCMU->APB1RSTR |= (RCMU_APB1RSTR_TIM3RST))
#define __HAL_RCMU_WWDG_FORCE_RESET()                       (RCMU->APB1RSTR |= (RCMU_APB1RSTR_WWDGRST))
#define __HAL_RCMU_USART2_FORCE_RESET()                     (RCMU->APB1RSTR |= (RCMU_APB1RSTR_USART2RST))
#define __HAL_RCMU_I2C1_FORCE_RESET()                       (RCMU->APB1RSTR |= (RCMU_APB1RSTR_I2C1RST))

#define __HAL_RCMU_BKP_FORCE_RESET()                        (RCMU->APB1RSTR |= (RCMU_APB1RSTR_BKPRST))
#define __HAL_RCMU_PWR_FORCE_RESET()                        (RCMU->APB1RSTR |= (RCMU_APB1RSTR_PMU_RST))

#define __HAL_RCMU_APB1_RELEASE_RESET()                     (RCMU->APB1RSTR = 0x00)  
#define __HAL_RCMU_TIM2_RELEASE_RESET()                     (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_TIM2RST))
#define __HAL_RCMU_TIM3_RELEASE_RESET()                     (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_TIM3RST))
#define __HAL_RCMU_WWDG_RELEASE_RESET()                     (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_WWDGRST))
#define __HAL_RCMU_USART2_RELEASE_RESET()                   (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_USART2RST))
#define __HAL_RCMU_I2C1_RELEASE_RESET()                     (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_I2C1RST))

#define __HAL_RCMU_BKP_RELEASE_RESET()                      (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_BKPRST))
#define __HAL_RCMU_PWR_RELEASE_RESET()                      (RCMU->APB1RSTR &= ~(RCMU_APB1RSTR_PMU_RST))

/**
  * @}
  */

/** @defgroup RCMU_APB2_Force_Release_Reset APB2 Force Release Reset
  * @brief  Force or release APB2 peripheral reset.
  * @{   
  */
#define __HAL_RCMU_APB2_FORCE_RESET()                       (RCMU->APB2RSTR = 0xFFFFFFFFU)  
#define __HAL_RCMU_AFIO_FORCE_RESET()                       (RCMU->APB2RSTR |= (RCMU_APB2RSTR_AFIORST))
#define __HAL_RCMU_GPIOA_FORCE_RESET()                      (RCMU->APB2RSTR |= (RCMU_APB2RSTR_IOPARST))
#define __HAL_RCMU_GPIOB_FORCE_RESET()                      (RCMU->APB2RSTR |= (RCMU_APB2RSTR_IOPBRST))
#define __HAL_RCMU_GPIOC_FORCE_RESET()                      (RCMU->APB2RSTR |= (RCMU_APB2RSTR_IOPCRST))
#define __HAL_RCMU_GPIOD_FORCE_RESET()                      (RCMU->APB2RSTR |= (RCMU_APB2RSTR_IOPDRST))
#define __HAL_RCMU_ADC1_FORCE_RESET()                       (RCMU->APB2RSTR |= (RCMU_APB2RSTR_ADC1RST))

#define __HAL_RCMU_TIM1_FORCE_RESET()                       (RCMU->APB2RSTR |= (RCMU_APB2RSTR_TIM1RST))
#define __HAL_RCMU_SPI1_FORCE_RESET()                       (RCMU->APB2RSTR |= (RCMU_APB2RSTR_SPI1RST))
#define __HAL_RCMU_USART1_FORCE_RESET()                     (RCMU->APB2RSTR |= (RCMU_APB2RSTR_USART1RST))

#define __HAL_RCMU_APB2_RELEASE_RESET()                     (RCMU->APB2RSTR = 0x00)  
#define __HAL_RCMU_AFIO_RELEASE_RESET()                     (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_AFIORST))
#define __HAL_RCMU_GPIOA_RELEASE_RESET()                    (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_IOPARST))
#define __HAL_RCMU_GPIOB_RELEASE_RESET()                    (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_IOPBRST))
#define __HAL_RCMU_GPIOC_RELEASE_RESET()                    (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_IOPCRST))
#define __HAL_RCMU_GPIOD_RELEASE_RESET()                    (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_IOPDRST))
#define __HAL_RCMU_ADC1_RELEASE_RESET()                     (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_ADC1RST))

#define __HAL_RCMU_TIM1_RELEASE_RESET()                     (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_TIM1RST))
#define __HAL_RCMU_SPI1_RELEASE_RESET()                     (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_SPI1RST))
#define __HAL_RCMU_USART1_RELEASE_RESET()                   (RCMU->APB2RSTR &= ~(RCMU_APB2RSTR_USART1RST))

/**
  * @}
  */

/** @defgroup RCMU_HSI_Configuration HSI Configuration
  * @{   
  */

/** @brief  Macros to enable or disable the Internal High Speed oscillator (HSI).
  * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
  * @note   HSI can not be stopped if it is used as system clock source. In this case,
  *         you have to select another source of the system clock then stop the HSI.  
  * @note   After enabling the HSI, the application software should wait on HSIRDY
  *         flag to be set indicating that HSI clock is stable and can be used as
  *         system clock source.  
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles.  
  */
#define __HAL_RCMU_HSI_ENABLE()                             (*(__IO uint32_t *) RCMU_CR_HSION_BB = ENABLE)
#define __HAL_RCMU_HSI_DISABLE()                            (*(__IO uint32_t *) RCMU_CR_HSION_BB = DISABLE)

/** @brief  Macro to adjust the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  _HSICALIBRATIONVALUE_ specifies the calibration trimming value.
  *         (default is RCMU_HSICALIBRATION_DEFAULT).
  *         This parameter must be a number between 0 and 0x1F.
  */  
#define __HAL_RCMU_HSI_CALIBRATIONVALUE_ADJUST(_HSICALIBRATIONVALUE_)   \
                                                            MODIFY_REG(RCMU->CR, RCMU_CR_HSITRIM, (uint32_t)(_HSICALIBRATIONVALUE_) << RCMU_CR_HSITRIM_Pos)

/**
  * @}
  */

/** @defgroup RCMU_LSI_Configuration  LSI Configuration
  * @{   
  */

/** @brief Macro to enable the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on 
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  */
#define __HAL_RCMU_LSI_ENABLE()                             (*(__IO uint32_t *) RCMU_CSR_LSION_BB = ENABLE)

/** @brief Macro to disable the Internal Low Speed oscillator (LSI).
  * @note   LSI can not be disabled if the IWDG is running.  
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles. 
  */
#define __HAL_RCMU_LSI_DISABLE()                            (*(__IO uint32_t *) RCMU_CSR_LSION_BB = DISABLE)

/**
  * @}
  */

/** @defgroup RCMU_HSE_Configuration HSE Configuration
  * @{   
  */

/**
  * @brief  Macro to configure the External High Speed oscillator (HSE).
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @note   After enabling the HSE (RCMU_HSE_ON or RCMU_HSE_Bypass), the application
  *         software should wait on HSERDY flag to be set indicating that HSE clock
  *         is stable and can be used to clock the PLL and/or system clock.
  * @note   HSE state can not be changed if it is used directly or through the
  *         PLL as system clock. In this case, you have to select another source
  *         of the system clock then change the HSE state (ex. disable it).
  * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.
  * @note   This function reset the CSSON bit, so if the clock security system(CSS)
  *         was previously enabled you have to enable it again after calling this
  *         function.
  * @param  __STATE__ specifies the new state of the HSE.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_HSE_OFF turn OFF the HSE oscillator, HSERDY flag goes low after
  *                              6 HSE oscillator clock cycles.
  *            @arg @ref RCMU_HSE_ON turn ON the HSE oscillator
  *            @arg @ref RCMU_HSE_BYPASS HSE oscillator bypassed with external clock
  */
#define __HAL_RCMU_HSE_CONFIG(__STATE__)                    do{                                                       \
                                                                if ((__STATE__) == RCMU_HSE_ON)                       \
                                                                {                                                     \
                                                                    SET_BIT(RCMU->CR, RCMU_CR_HSEON);                 \
                                                                }                                                     \
                                                                else if ((__STATE__) == RCMU_HSE_OFF)                 \
                                                                {                                                     \
                                                                    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEON);               \
                                                                    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEBYP);              \
                                                                }                                                     \
                                                                else if ((__STATE__) == RCMU_CR_HSEBYP)               \
                                                                {                                                     \
                                                                    SET_BIT(RCMU->CR, RCMU_CR_HSEBYP);                \
                                                                    SET_BIT(RCMU->CR, RCMU_CR_HSEON);                 \
                                                                }                                                     \
                                                                else                                                  \
                                                                {                                                     \
                                                                    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEON);               \
                                                                    CLEAR_BIT(RCMU->CR, RCMU_CR_HSEBYP);              \
                                                                }                                                     \
															}while(0)

/**
  * @}
  */

/** @defgroup RCMU_LSE_Configuration LSE Configuration
  * @{   
  */

/**
  * @brief  Macro to configure the External Low Speed oscillator (LSE).
  * @note Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not supported by this macro. 
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using 
  *         @ref HAL_PWR_EnableBkUpAccess() function before to configure the LSE
  *         (to be done once after reset).  
  * @note   After enabling the LSE (RCMU_LSE_ON or RCMU_LSE_BYPASS), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  __STATE__ specifies the new state of the LSE.
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_LSE_OFF turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg @ref RCMU_LSE_ON turn ON the LSE oscillator.
  *            @arg @ref RCMU_LSE_BYPASS LSE oscillator bypassed with external clock.
  */
#define __HAL_RCMU_LSE_CONFIG(__STATE__)                    do{                                                     \
                                                                if ((__STATE__) == RCMU_LSE_ON)                     \
                                                                {                                                   \
                                                                    SET_BIT(RCMU->BDCR, RCMU_BDCR_LSEON);           \
                                                                }                                                   \
                                                                else if ((__STATE__) == RCMU_LSE_OFF)               \
                                                                {                                                   \
                                                                    CLEAR_BIT(RCMU->BDCR, RCMU_BDCR_LSEON);         \
                                                                    CLEAR_BIT(RCMU->BDCR, RCMU_BDCR_LSEBYP);        \
                                                                }                                                   \
                                                                else if ((__STATE__) == RCMU_BDCR_LSEBYP)           \
                                                                {                                                   \
                                                                    SET_BIT(RCMU->BDCR, RCMU_BDCR_LSEBYP);          \
                                                                    SET_BIT(RCMU->BDCR, RCMU_BDCR_LSEON);           \
                                                                }                                                   \
                                                                else                                                \
                                                                {                                                   \
                                                                    CLEAR_BIT(RCMU->BDCR, RCMU_BDCR_LSEON);         \
                                                                    CLEAR_BIT(RCMU->BDCR, RCMU_BDCR_LSEBYP);        \
                                                                }                                                   \
															} while(0)

/**
  * @}
  */

/** @defgroup RCMU_PLL_Configuration PLL Configuration
  * @{   
  */

/** @brief Macro to enable the main PLL.
  * @note   After enabling the main PLL, the application software should wait on 
  *         PLLRDY flag to be set indicating that PLL clock is stable and can
  *         be used as system clock source.
  * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCMU_PLL_ENABLE()                                 (*(__IO uint32_t *) RCMU_CR_PLLON_BB = ENABLE)

/** @brief Macro to disable the main PLL.
  * @note   The main PLL can not be disabled if it is used as system clock source
  */
#define __HAL_RCMU_PLL_DISABLE()                                (*(__IO uint32_t *) RCMU_CR_PLLON_BB = DISABLE)

/** @brief Macro to configure the main PLL clock source and multiplication factors.
  * @note   This function must be used only when the main PLL is disabled.
  *  
  * @param  __RCMU_PLLSOURCE__ specifies the PLL entry clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_PLLSOURCE_HSI_DIV2 HSI oscillator clock selected as PLL clock entry
  *            @arg @ref RCMU_PLLSOURCE_HSE HSE oscillator clock selected as PLL clock entry
  * @param  __PLLMUL__ specifies the multiplication factor for PLL VCO output clock
  *          This parameter can be one of the following values:
  *             @arg @ref RCMU_PLL_MUL4   PLLVCO = PLL clock entry x 4
  *             @arg @ref RCMU_PLL_MUL6   PLLVCO = PLL clock entry x 6
  *             @arg @ref RCMU_PLL_MUL2   PLLVCO = PLL clock entry x 2
  *             @arg @ref RCMU_PLL_MUL3   PLLVCO = PLL clock entry x 3
  *             @arg @ref RCMU_PLL_MUL10  PLLVCO = PLL clock entry x 10
  *             @arg @ref RCMU_PLL_MUL11  PLLVCO = PLL clock entry x 11
  *             @arg @ref RCMU_PLL_MUL12  PLLVCO = PLL clock entry x 12
  *             @arg @ref RCMU_PLL_MUL13  PLLVCO = PLL clock entry x 13
  *             @arg @ref RCMU_PLL_MUL14  PLLVCO = PLL clock entry x 14
  *             @arg @ref RCMU_PLL_MUL15  PLLVCO = PLL clock entry x 15
  *             @arg @ref RCMU_PLL_MUL16  PLLVCO = PLL clock entry x 16
  *             @arg @ref RCMU_PLL_MUL8   PLLVCO = PLL clock entry x 8
  *             @arg @ref RCMU_PLL_MUL9   PLLVCO = PLL clock entry x 9
  *   
  */
#define __HAL_RCMU_PLL_CONFIG(__RCMU_PLLSOURCE__, __PLLMUL__)   MODIFY_REG(RCMU->CFGR, (RCMU_PLLCR_PLL_SRC | RCMU_CFGR_PLLMULL),((__RCMU_PLLSOURCE__) | (__PLLMUL__) ))

/** @brief  Get oscillator clock selected as PLL input clock
  * @retval The clock source used for PLL entry. The returned value can be one
  *         of the following:
  *             @arg @ref RCMU_PLLSOURCE_HSI_DIV2 HSI oscillator clock selected as PLL input clock
  *             @arg @ref RCMU_PLLSOURCE_HSE HSE oscillator clock selected as PLL input clock
  */
#define __HAL_RCMU_GET_PLL_OSCSOURCE()                          ((uint32_t)(READ_BIT(RCMU->CFGR, RCMU_PLLCR_PLL_SRC)))

/**
  * @}
  */

/** @defgroup RCMU_Get_Clock_source Get Clock source
  * @{   
  */

/**
  * @brief  Macro to configure the system clock source.
  * @param  __SYSCLKSOURCE__ specifies the system clock source.
  *          This parameter can be one of the following values:
  *              @arg @ref RCMU_SYSCLKSOURCE_HSI HSI oscillator is used as system clock source.
  *              @arg @ref RCMU_SYSCLKSOURCE_HSE HSE oscillator is used as system clock source.
  *              @arg @ref RCMU_SYSCLKSOURCE_PLLCLK PLL output is used as system clock source.
  */
#define __HAL_RCMU_SYSCLK_CONFIG(__SYSCLKSOURCE__)              MODIFY_REG(RCMU->CFGR, RCMU_CFGR_SW, (__SYSCLKSOURCE__))

/** @brief  Macro to get the clock source used as system clock.
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following:
  *             @arg @ref RCMU_SYSCLKSOURCE_STATUS_HSI HSI used as system clock
  *             @arg @ref RCMU_SYSCLKSOURCE_STATUS_HSE HSE used as system clock
  *             @arg @ref RCMU_SYSCLKSOURCE_STATUS_PLLCLK PLL used as system clock
  */
#define __HAL_RCMU_GET_SYSCLK_SOURCE()                          ((uint32_t)(READ_BIT(RCMU->CFGR,RCMU_CFGR_SWS)))

/**
  * @}
  */

/** @defgroup RCCEx_MCOx_Clock_Config RCMU Extended MCOx Clock Config
  * @{   
  */ 

#if   defined(RCMU_CFGR_MCO_3)
/** @brief  Macro to configure the MCO clock.
  * @param  __MCOCLKSOURCE__ specifies the MCO clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_MCO1SOURCE_NOCLOCK      No clock selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_SYSCLK       System clock (SYSCLK) selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_HSI          HSI selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_HSE          HSE selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_PLLCLK       PLL clock divided by 2 selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_PLL2CLK      PLL2 clock selected by 2 selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_PLL3CLK_DIV2 PLL3 clock divided by 2 selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_EXT_HSE XT1  external 3-25 MHz oscillator clock selected (for Ethernet) as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_PLL3CLK      PLL3 clock selected (for Ethernet) as MCO clock
  * @param  __MCODIV__ specifies the MCO clock prescaler.
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_MCODIV_1 No division applied on MCO clock source
  */
#else
/** @brief  Macro to configure the MCO clock.
  * @param  __MCOCLKSOURCE__ specifies the MCO clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_MCO1SOURCE_NOCLOCK No clock selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_SYSCLK  System clock (SYSCLK) selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_HSI HSI selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_HSE HSE selected as MCO clock
  *            @arg @ref RCMU_MCO1SOURCE_PLLCLK  PLL clock divided by 2 selected as MCO clock
  * @param  __MCODIV__ specifies the MCO clock prescaler.
  *         This parameter can be one of the following values:
  *            @arg @ref RCMU_MCODIV_1 No division applied on MCO clock source
  */
#endif

#define __HAL_RCMU_MCO1_CONFIG(__MCOCLKSOURCE__, __MCODIV__)    do {                                                           \
                                                                    MODIFY_REG(RCMU->CFGR, RCMU_CFGR_MCO, (__MCOCLKSOURCE__)); \
                                                                    MODIFY_REG(RCMU->CFGR, RCMU_CFGR_MCOPRE, (__MCODIV__));    \
                                                                } while(0)


/**
  * @}
  */

  /** @defgroup RCMU_RTC_Clock_Configuration RCMU RTC Clock Configuration
  * @{   
  */

/** @brief Macro to configure the RTC clock (RTCCLK).
  * @note   As the RTC clock configuration bits are in the Backup domain and write
  *         access is denied to this domain after reset, you have to enable write
  *         access using the Power Backup Access macro before to configure
  *         the RTC clock source (to be done once after reset).    
  * @note   Once the RTC clock is configured it can't be changed unless the  
  *         Backup domain is reset using @ref __HAL_RCMU_BACKUPRESET_FORCE() macro, or by
  *         a Power On Reset (POR).
  *
  * @param  __RTC_CLKSOURCE__ specifies the RTC clock source.
  *          This parameter can be one of the following values:
  *             @arg @ref RCMU_RTCCLKSOURCE_NO_CLK No clock selected as RTC clock
  *             @arg @ref RCMU_RTCCLKSOURCE_LSE LSE selected as RTC clock
  *             @arg @ref RCMU_RTCCLKSOURCE_LSI LSI selected as RTC clock
  *             @arg @ref RCMU_RTCCLKSOURCE_HSE_DIV128 HSE divided by 128 selected as RTC clock
  * @note   If the LSE or LSI is used as RTC clock source, the RTC continues to
  *         work in STOP and STANDBY modes, and can be used as wakeup source.
  *         However, when the HSE clock is used as RTC clock source, the RTC
  *         cannot be used in STOP and STANDBY modes.    
  * @note   The maximum input clock frequency for RTC is 1MHz (when using HSE as
  *         RTC clock source).
  */
#define __HAL_RCMU_RTC_CONFIG(__RTC_CLKSOURCE__)                MODIFY_REG(RCMU->BDCR, RCMU_BDCR_RTCSEL, (__RTC_CLKSOURCE__))
                                                   
/** @brief Macro to get the RTC clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCMU_RTCCLKSOURCE_NO_CLK No clock selected as RTC clock
  *            @arg @ref RCMU_RTCCLKSOURCE_LSE LSE selected as RTC clock
  *            @arg @ref RCMU_RTCCLKSOURCE_LSI LSI selected as RTC clock
  *            @arg @ref RCMU_RTCCLKSOURCE_HSE_DIV128 HSE divided by 128 selected as RTC clock
  */
#define __HAL_RCMU_GET_RTC_SOURCE()                             READ_BIT(RCMU->BDCR, RCMU_BDCR_RTCSEL)

/** @brief Macro to enable the the RTC clock.
  * @note   These macros must be used only after the RTC clock source was selected.
  */
#define __HAL_RCMU_RTC_ENABLE()                                 (*(__IO uint32_t *) RCMU_BDCR_RTCEN_BB = ENABLE)

/** @brief Macro to disable the the RTC clock.
  * @note  These macros must be used only after the RTC clock source was selected.
  */
#define __HAL_RCMU_RTC_DISABLE()                                (*(__IO uint32_t *) RCMU_BDCR_RTCEN_BB = DISABLE)

/** @brief  Macro to force the Backup domain reset.
  * @note   This function resets the RTC peripheral (including the backup registers)
  *         and the RTC clock source selection in RCMU_BDCR register.
  */
#define __HAL_RCMU_BACKUPRESET_FORCE()                          (*(__IO uint32_t *) RCMU_BDCR_BDRST_BB = ENABLE)

/** @brief  Macros to release the Backup domain reset.
  */
#define __HAL_RCMU_BACKUPRESET_RELEASE()                        (*(__IO uint32_t *) RCMU_BDCR_BDRST_BB = DISABLE)

/**
  * @}
  */

/** @defgroup RCMU_Flags_Interrupts_Management Flags Interrupts Management
  * @brief macros to manage the specified RCMU Flags and interrupts.
  * @{
  */

/** @brief Enable RCMU interrupt.
  * @param  __INTERRUPT__ specifies the RCMU interrupt sources to be enabled.
  *          This parameter can be any combination of the following values:
  *            @arg @ref RCMU_IT_LSIRDY LSI ready interrupt
  *            @arg @ref RCMU_IT_LSERDY LSE ready interrupt
  *            @arg @ref RCMU_IT_HSIRDY HSI ready interrupt
  *            @arg @ref RCMU_IT_HSERDY HSE ready interrupt
  *            @arg @ref RCMU_IT_PLLRDY main PLL ready interrupt
  */
#define __HAL_RCMU_ENABLE_IT(__INTERRUPT__)                     (*(__IO uint8_t *) RCMU_CIR_BYTE1_ADDRESS |= (__INTERRUPT__))

/** @brief Disable RCMU interrupt.
  * @param  __INTERRUPT__ specifies the RCMU interrupt sources to be disabled.
  *          This parameter can be any combination of the following values:
  *            @arg @ref RCMU_IT_LSIRDY LSI ready interrupt
  *            @arg @ref RCMU_IT_LSERDY LSE ready interrupt
  *            @arg @ref RCMU_IT_HSIRDY HSI ready interrupt
  *            @arg @ref RCMU_IT_HSERDY HSE ready interrupt
  *            @arg @ref RCMU_IT_PLLRDY main PLL ready interrupt
  */
#define __HAL_RCMU_DISABLE_IT(__INTERRUPT__)                    (*(__IO uint8_t *) RCMU_CIR_BYTE1_ADDRESS &= (uint8_t)(~(__INTERRUPT__)))

/** @brief Clear the RCMU's interrupt pending bits.
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref RCMU_IT_LSIRDY LSI ready interrupt.
  *            @arg @ref RCMU_IT_LSERDY LSE ready interrupt.
  *            @arg @ref RCMU_IT_HSIRDY HSI ready interrupt.
  *            @arg @ref RCMU_IT_HSERDY HSE ready interrupt.
  *            @arg @ref RCMU_IT_PLLRDY Main PLL ready interrupt.
  *            @arg @ref RCMU_IT_CSS Clock Security System interrupt
  */
#define __HAL_RCMU_CLEAR_IT(__INTERRUPT__)                      (*(__IO uint8_t *) RCMU_CIR_BYTE2_ADDRESS = (__INTERRUPT__))

/** @brief Check the RCMU's interrupt has occurred or not.
  * @param  __INTERRUPT__ specifies the RCMU interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_IT_LSIRDY LSI ready interrupt.
  *            @arg @ref RCMU_IT_LSERDY LSE ready interrupt.
  *            @arg @ref RCMU_IT_HSIRDY HSI ready interrupt.
  *            @arg @ref RCMU_IT_HSERDY HSE ready interrupt.
  *            @arg @ref RCMU_IT_PLLRDY Main PLL ready interrupt.
  *            @arg @ref RCMU_IT_CSS Clock Security System interrupt
  * @retval The new state of __INTERRUPT__ (TRUE or FALSE).
  */
#define __HAL_RCMU_GET_IT(__INTERRUPT__)                        ((RCMU->CIR & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief Set RMVF bit to clear the reset flags.
  *         The reset flags are RCMU_FLAG_PINRST, RCMU_FLAG_PORRST, RCMU_FLAG_SFTRST,
  *         RCMU_FLAG_IWDGRST, RCMU_FLAG_WWDGRST, RCMU_FLAG_LPWRRST
  */
#define __HAL_RCMU_CLEAR_RESET_FLAGS()                          (*(__IO uint32_t *)RCMU_CSR_RMVF_BB = ENABLE)

/** @brief  Check RCMU flag is set or not.
  * @param  __FLAG__ specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg @ref RCMU_FLAG_HSIRDY HSI oscillator clock ready.
  *            @arg @ref RCMU_FLAG_HSERDY HSE oscillator clock ready.
  *            @arg @ref RCMU_FLAG_PLLRDY Main PLL clock ready.
  *            @arg @ref RCMU_FLAG_LSERDY LSE oscillator clock ready.
  *            @arg @ref RCMU_FLAG_LSIRDY LSI oscillator clock ready.
  *            @arg @ref RCMU_FLAG_PINRST  Pin reset.
  *            @arg @ref RCMU_FLAG_PORRST  POR/PDR reset.
  *            @arg @ref RCMU_FLAG_SFTRST  Software reset.
  *            @arg @ref RCMU_FLAG_IWDGRST Independent Watchdog reset.
  *            @arg @ref RCMU_FLAG_WWDGRST Window Watchdog reset.
  *            @arg @ref RCMU_FLAG_LPWRRST Low Power reset.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_RCMU_GET_FLAG(__FLAG__)                           (((((__FLAG__) >> 5U) == CR_REG_INDEX) ? RCMU->CR : \
                                                                  ((((__FLAG__) >> 5U) == BDCR_REG_INDEX) ? RCMU->BDCR : RCMU->CSR)) & (1U << ((__FLAG__) & RCMU_FLAG_MASK)))

#define __HAL_RCMU_GET_REG_FLAG(__REG__,__FLAG__,__FLAGPOS__)   (READ_BIT(__REG__,__FLAG__) >> __FLAGPOS__)

/**
  * @}
  */

/**
  * @}
  */


/* Include RCMU HAL Extension module */
#include "slm32x030_hal_rcmu_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCMU_Exported_Functions
  * @{
  */

/** @addtogroup RCMU_Exported_Functions_Group1
  * @{
  */

/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef HAL_RCMU_DeInit(void);
HAL_StatusTypeDef HAL_RCMU_OscConfig(RCMU_OscInitTypeDef  *RCMU_OscInitStruct);
HAL_StatusTypeDef HAL_RCMU_ClockConfig(RCMU_ClkInitTypeDef  *RCMU_ClkInitStruct, uint32_t FLatency);

/**
  * @}
  */

/** @addtogroup RCMU_Exported_Functions_Group2
  * @{
  */

/* Peripheral Control functions  ************************************************/
void              HAL_RCMU_MCO_Output_Ctrl(uint32_t mcox, uint32_t clksrc, uint32_t clkdiv);
uint32_t          HAL_RCMU_GetSysClockFreq(void);
uint32_t          HAL_RCMU_GetHCLKFreq(void);
uint32_t          HAL_RCMU_GetPCLK1Freq(void);
uint32_t          HAL_RCMU_GetPCLK2Freq(void);
void              HAL_RCMU_GetOscConfig(RCMU_OscInitTypeDef  *RCMU_OscInitStruct);
void              HAL_RCMU_GetClockConfig(RCMU_ClkInitTypeDef  *RCMU_ClkInitStruct, uint32_t *pFLatency);

/* CSS NMI IRQ handler */
void              HAL_RCMU_NMI_IRQHandler(void);

/* User Callbacks in non blocking mode (IT mode) */
void              HAL_RCMU_CSSCallback(void);


extern void HAL_RCMU_PllTypeSet(RCMU_OscInitTypeDef* hrcmu);
extern uint8_t HAL_RCMU_PllTypeGet(void);



/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCMU_Private_Constants
  * @{
  */

/** @defgroup RCMU_Timeout RCMU Timeout
  * @{
  */ 
  
/* Disable Backup domain write protection state change timeout */

#define RCMU_DBP_TIMEOUT_VALUE              (100U)      /* 100 ms */
/* LSE state change timeout */
#define RCMU_LSE_TIMEOUT_VALUE              LSE_STARTUP_TIMEOUT
#define CLOCKSWITCH_TIMEOUT_VALUE           (5000U)     /* 5 s    */
#define HSE_TIMEOUT_VALUE                   HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE                   (50U)       /* 50 ms (minimum Tick + 1) */
#define LSI_TIMEOUT_VALUE                   (800U)      /* 50 ms (minimum Tick + 1) */
#define PLL_TIMEOUT_VALUE                   (50U)       /* 50 ms (minimum Tick + 1) */

/**
  * @}
  */
  
/** @defgroup RCMU_Register_Offset Register offsets
  * @{
  */
#define RCMU_OFFSET                         (RCMU_BASE - PERIPH_BASE)
#define RCMU_CR_OFFSET                      (0x00UL)
#define RCMU_CFGR_OFFSET                    (0x04UL)
#define RCMU_CIR_OFFSET                     (0x08UL)
#define RCMU_APB2RSTR_OFFSET                (0x0CUL)
#define RCMU_APB1RSTR_OFFSET                (0x10UL)
#define RCMU_AHBENR_OFFSET                  (0x14UL)
#define RCMU_APB1ENR_OFFSET                 (0x18UL)
#define RCMU_APB2ENR_OFFSET                 (0x1CUL)
#define RCMU_BDCR_OFFSET                    (0x20UL)
#define RCMU_CSR_OFFSET                     (0x24UL)
#define RCMU_AHBRSTR_OFFSET                 (0x28UL)
#define RCMU_CFGR2_OFFSET                   (0x2CUL)
#define RCMU_PLLCR_OFFSET                   (0x40UL)
#define RCMU_STCR_OFFSET                    (0x5CUL)
#define RCMU_IRQLATENCY_OFFSET              (0x60UL)

/**
  * @}
  */

/** @defgroup RCMU_BitAddress_AliasRegion BitAddress AliasRegion
  * @brief RCMU registers bit address in the alias region
  * @{
  */
#define RCMU_CR_OFFSET_BB                   (RCMU_OFFSET + RCMU_CR_OFFSET)
#define RCMU_CFGR_OFFSET_BB                 (RCMU_OFFSET + RCMU_CFGR_OFFSET)
#define RCMU_CIR_OFFSET_BB                  (RCMU_OFFSET + RCMU_CIR_OFFSET)
#define RCMU_BDCR_OFFSET_BB                 (RCMU_OFFSET + RCMU_BDCR_OFFSET)
#define RCMU_CSR_OFFSET_BB                  (RCMU_OFFSET + RCMU_CSR_OFFSET)

/* --- CR Register ---*/
/* Alias word address of HSION bit */
#define RCMU_HSION_BIT_NUMBER               RCMU_CR_HSION_Pos
#define RCMU_CR_HSION_BB                    ((uint32_t)(PERIPH_BB_BASE + (RCMU_CR_OFFSET_BB * 32U) + (RCMU_HSION_BIT_NUMBER * 4U)))
/* Alias word address of HSEON bit */
#define RCMU_HSEON_BIT_NUMBER               RCMU_CR_HSEON_Pos
#define RCMU_CR_HSEON_BB                    ((uint32_t)(PERIPH_BB_BASE + (RCMU_CR_OFFSET_BB * 32U) + (RCMU_HSEON_BIT_NUMBER * 4U)))
/* Alias word address of CSSON bit */
#define RCMU_CSSON_BIT_NUMBER               RCMU_CR_CSSON_Pos
#define RCMU_CR_CSSON_BB                    ((uint32_t)(PERIPH_BB_BASE + (RCMU_CR_OFFSET_BB * 32U) + (RCMU_CSSON_BIT_NUMBER * 4U)))
/* Alias word address of PLLON bit */
#define RCMU_PLLON_BIT_NUMBER               RCMU_CR_PLLON_Pos
#define RCMU_CR_PLLON_BB                    ((uint32_t)(PERIPH_BB_BASE + (RCMU_CR_OFFSET_BB * 32U) + (RCMU_PLLON_BIT_NUMBER * 4U)))

/* --- CSR Register ---*/
/* Alias word address of LSION bit */
#define RCMU_LSION_BIT_NUMBER               RCMU_CSR_LSION_Pos
#define RCMU_CSR_LSION_BB                   ((uint32_t)(PERIPH_BB_BASE + (RCMU_CSR_OFFSET_BB * 32U) + (RCMU_LSION_BIT_NUMBER * 4U)))

/* Alias word address of RMVF bit */
#define RCMU_RMVF_BIT_NUMBER                RCMU_CSR_RMVF_Pos
#define RCMU_CSR_RMVF_BB                    ((uint32_t)(PERIPH_BB_BASE + (RCMU_CSR_OFFSET_BB * 32U) + (RCMU_RMVF_BIT_NUMBER * 4U)))

/* --- BDCR Registers ---*/
/* Alias word address of LSEON bit */
#define RCMU_LSEON_BIT_NUMBER               RCMU_BDCR_LSEON_Pos
#define RCMU_BDCR_LSEON_BB                  ((uint32_t)(PERIPH_BB_BASE + (RCMU_BDCR_OFFSET_BB * 32U) + (RCMU_LSEON_BIT_NUMBER * 4U)))

/* Alias word address of LSEON bit */
#define RCMU_LSEBYP_BIT_NUMBER              RCMU_BDCR_LSEBYP_Pos
#define RCMU_BDCR_LSEBYP_BB                 ((uint32_t)(PERIPH_BB_BASE + (RCMU_BDCR_OFFSET_BB * 32U) + (RCMU_LSEBYP_BIT_NUMBER * 4U)))

/* Alias word address of RTCEN bit */
#define RCMU_RTCEN_BIT_NUMBER               RCMU_BDCR_RTCEN_Pos
#define RCMU_BDCR_RTCEN_BB                  ((uint32_t)(PERIPH_BB_BASE + (RCMU_BDCR_OFFSET_BB * 32U) + (RCMU_RTCEN_BIT_NUMBER * 4U)))

/* Alias word address of BDRST bit */
#define RCMU_BDRST_BIT_NUMBER               RCMU_BDCR_BDRST_Pos
#define RCMU_BDCR_BDRST_BB                  ((uint32_t)(PERIPH_BB_BASE + (RCMU_BDCR_OFFSET_BB * 32U) + (RCMU_BDRST_BIT_NUMBER * 4U)))

  
/* CR register byte 2 (Bits[23:16]) base address */
#define RCMU_CR_BYTE2_ADDRESS               ((uint32_t)(RCMU_BASE + RCMU_CR_OFFSET + 0x02U))

/* CIR register byte 1 (Bits[15:8]) base address */
#define RCMU_CIR_BYTE1_ADDRESS              ((uint32_t)(RCMU_BASE + RCMU_CIR_OFFSET + 0x01U))

/* CIR register byte 2 (Bits[23:16]) base address */
#define RCMU_CIR_BYTE2_ADDRESS              ((uint32_t)(RCMU_BASE + RCMU_CIR_OFFSET + 0x02U))

/* Defines used for Flags */
#define CR_REG_INDEX                        ((uint8_t)1)
#define BDCR_REG_INDEX                      ((uint8_t)2)
#define CSR_REG_INDEX                       ((uint8_t)3)

#define RCMU_FLAG_MASK                      ((uint8_t)0x1F)

/**
  * @}
  */

/** @addtogroup RCMU_Private_Macros
  * @{
  */

#define IS_RCMU_PLLSOURCE(__SOURCE__)           (((__SOURCE__) == RCMU_PLLSOURCE_HSE) || ((__SOURCE__) == RCMU_PLLSOURCE_HSE))

#define IS_RCMU_SYSCLKTYPE(__OSCILLATOR__)      (((__OSCILLATOR__) == RCMU_OSCILLATORTYPE_NONE)                           || \
                                                 (((__OSCILLATOR__) & RCMU_SW_SYSCLK_HSE) == RCMU_SW_SYSCLK_HSE) || \
                                                 (((__OSCILLATOR__) & RCMU_SW_SYSCLK_HSI) == RCMU_SW_SYSCLK_HSI) || \
                                                 (((__OSCILLATOR__) & RCMU_SW_SYSCLK_PLL) == RCMU_SW_SYSCLK_PLL) || \
                                                 (((__OSCILLATOR__) & RCMU_SW_SYSCLK_NULL) == RCMU_SW_SYSCLK_NULL) || \
                                                 (((__OSCILLATOR__) & RCMU_SW_SYSCLK_LSE_LSI) == RCMU_SW_SYSCLK_LSE_LSI))


#define IS_RCMU_CLOCKTYPE(CLK)                  ((((CLK) & RCMU_CLOCKTYPE_SYSCLK) == RCMU_CLOCKTYPE_SYSCLK) || \
                                                 (((CLK) & RCMU_CLOCKTYPE_HCLK)   == RCMU_CLOCKTYPE_HCLK)   || \
                                                 (((CLK) & RCMU_CLOCKTYPE_PCLK1)  == RCMU_CLOCKTYPE_PCLK1)  || \
                                                 (((CLK) & RCMU_CLOCKTYPE_PCLK2)  == RCMU_CLOCKTYPE_PCLK2))

#define IS_RCMU_SYSCLKSOURCE(__SOURCE__)        (((__SOURCE__) == RCMU_SYSCLKSOURCE_HSI) || \
                                                 ((__SOURCE__) == RCMU_SYSCLKSOURCE_HSE) || \
                                                 ((__SOURCE__) == RCMU_SYSCLKSOURCE_LSI_LSE) || \
                                                 ((__SOURCE__) == RCMU_SYSCLKSOURCE_PLLCLK))

#define IS_RCMU_SYSCLKSOURCE_STATUS(__SOURCE__) (((__SOURCE__) == RCMU_SYSCLKSOURCE_STATUS_HSI) || \
                                                 ((__SOURCE__) == RCMU_SYSCLKSOURCE_STATUS_HSE) || \
                                                 ((__SOURCE__) == RCMU_SYSCLKSOURCE_STATUS_PLLCLK))

#define IS_RCMU_HCLK(__HCLK__)                  (((__HCLK__) >= RCMU_SYSCLK_DIV1) || ((__HCLK__) <= RCMU_SYSCLK_DIV512))

#define IS_RCMU_PCLK(__PCLK__)                  (((__PCLK__) >= RCMU_PCLK1_PCLK2_DIV1) || ((__PCLK__) <= RCMU_PCLK1_PCLK2_DIV16))

#define IS_RCMU_MCO(__MCO__)                    ((__MCO__) == RCMU_MCO)

#define IS_RCMU_MCODIV(__DIV__)                 (((__DIV__) >= RCMU_MCODIV_1) && ((__DIV__) <= RCMU_MCODIV_128)) 

#define IS_RCMU_MCO1SOURCE(__SOURCE__)          (((__SOURCE__) >= RCMU_MCO_CLK_NULL)  && ((__SOURCE__) <= RCMU_MCO_CLK_PLL))

#define IS_RCMU_RTCCLKSOURCE(__SOURCE__)        (((__SOURCE__) == RCMU_RTCCLKSOURCE_NO_CLK) || \
                                                    ((__SOURCE__) == RCMU_RTCCLKSOURCE_LSE) || \
                                                    ((__SOURCE__) == RCMU_RTCCLKSOURCE_LSI) || \
                                                    ((__SOURCE__) == RCMU_RTCCLKSOURCE_HSE_DIV128))

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
   
#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_RCMU_H_ */


