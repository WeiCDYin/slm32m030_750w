/**
  ******************************************************************************
  * @file    slm32x030.h
  * @author  Software Team
  * @version V1.0.0
  * @date    8-Nov-2024
  * @brief   CMSIS Cortex-M+ Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for SLM32L0xx devices.            
  *            
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *  
  ******************************************************************************
  */


/** @addtogroup slm32x030
  * @{
  */

#ifndef _SLM32X030_H_
#define _SLM32X030_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum
{
    /* =======================================  ARM Cortex-M0+ Specific Interrupt Numbers  ======================================= */
    Reset_IRQn                  = -15,    /*!< -15  Reset Vector, invoked on Power up and warm reset                        */
    NonMaskableInt_IRQn         = -14,    /*!< -14  Non maskable Interrupt, cannot be stopped or preempted                  */
    HardFault_IRQn              = -13,    /*!< -13  Hard Fault, all classes of Fault                                        */
    Nvm_IRQn                    = -12,    /*!< -12  nvm                                                                     */
    BusErr_IRQn                 = -11,    /*!< -11  BusErr                                                                  */
    AppErr_IRQn                 = -10,    /*!< -10  AppErr                                                                  */
    SVCall_IRQn                 =  -5,    /*!< -5 System Service Call via SVC instruction                                   */
    DebugMonitor_IRQn           =  -4,    /*!< -4  DebugMonitor                                                             */      
    PendSV_IRQn                 =  -2,    /*!< -2 Pendable request for system service                                       */
    SysTick_IRQn                =  -1,    /*!< -1 System Tick Timer                                                         */
    /* ==========================================  slm32x030 Specific Interrupt Numbers  ========================================== */
    WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                                    */
    PVD_IRQn                    = 1,      /* PVD                                                                            */
    RTC_IRQn                    = 2,      /*!< RTC through EXTI Line Interrupt                                              */
    FLASH_IRQn                  = 3,      /*!< FLASH Interrupt                                                              */
    RCMU_IRQn                   = 4,      /*!< RCMU Interrupt                                                                   */
    EXTI0_1_IRQn                = 5,      /*!< EXTI Line 0 and 1 Interrupts                                                 */
    EXTI2_3_IRQn                = 6,      /*!< EXTI Line 2 and 3 Interrupts                                                 */
    EXTI4_15_IRQn               = 7,      /*!< EXTI Line 4 to 15 Interrupts                                                 */
    AES_IRQn                    = 8,      /*!< AES global Interrupt                                                         */
    DMA1_Channel0_5_IRQn        = 9,      /*!< DMA1 Channel 0-5 Interrupt                                                   */
    AWKTIM_IRQn                 = 10,     /*!< AWK Interrupts                                                               */
    TAMP_IRQn                   = 11,     /*!< TAMPER_IRQn Interrupts                                                       */
    ADC_IRQn                    = 12,     /*!< ADC1 Interrupt                                                               */
    TIM1_BRK_UP_TRG_COM_IRQn    = 13,     /*!< TIM1 Break, Update, Trigger and Commutation Interrupts                       */
    TIM1_CC_IRQn                = 14,     /*!< TIM1 Capture Compare Interrupt                                               */
    SRAM_IRQn                   = 15,     /*SRAM                                                                            */
    TIM3_IRQn                   = 16,     /*!< TIM3 Interrupt                                                               */
    TIM6_IRQn                   = 17,     /*!< TIM6 Interrupt                                                               */
    TIM7_IRQn                   = 18,     /*!< TIM7 Interrupt                                                               */
    TIM14_IRQn                  = 19,     /*!< TIM14 Interrupt                                                              */
    TIM15_IRQn                  = 20,     /*!< TIM15 Interrupt                                                              */
    TIM16_IRQn                  = 21,     /*!< TIM16 Interrupt                                                              */
    TIM17_IRQn                  = 22,     /*!< TIM17 Interrupt                                                              */
    I2C1_IRQn                   = 23,     /*!< I2C1 Interrupt                                                               */
    I2C2_IRQn                   = 24,     /*!< I2C2 Interrupt                                                               */
    SPI1_IRQn                   = 25,     /*!< SPI1 Interrupt                                                               */
    SPI2_IRQn                   = 26,     /*!< SPI2 Interrupt                                                               */
    USART1_IRQn                 = 27,     /*!< USART1 Interrupt                                                             */
    USART2_IRQn                 = 28,     /*!< USART2 Interrupt                                                             */
    LPUART_IRQn                 = 29,     /*!<   LPUART                                                                     */
    TRNG_IRQn                   = 30,     /*!< RNG global Interrupt                                                         */
    VC_IRQn                     = 31,     /*!<  VC                                                                          */
} IRQn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ==========================  Configuration of the ARM Cortex-M0+ Processor and Core Peripherals  =========================== */
#define __CM0PLUS_REV                  0x0201U  /*!< CM0PLUS Core Revision                                                     */
#define __NVIC_PRIO_BITS               2        /*!< Number of Bits used for Priority Levels                                   */
#define __Vendor_SysTickConfig         0        /*!< Set to 1 if different SysTick Config is used                              */
#define __VTOR_PRESENT                 1        /*!< Set to 1 if CPU supports Vector Table Offset Register                     */
#define __MPU_PRESENT                  0        /*!< MPU present or not                                                        */
#define __FPU_PRESENT                  0        /*!< FPU present or not                                                        */


/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm0plus.h"                       /*!< ARM Cortex-M0+ processor and core peripherals                             */

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
  * @{
  */

/* =========================================================================================================================== */
/* ================                                           FLASH                                           ================ */
/* =========================================================================================================================== */

/**
  * @brief Flash Controller (FLASH)
  */

/** 
  * @brief FLASH Registers
  */
typedef struct
{
    __IO uint32_t ACR;          /*!<FLASH access control register,                 Address offset: 0x00 */
    __IO uint32_t KEYR;         /*!<FLASH key register,                            Address offset: 0x04 */
    __IO uint32_t OPTKEYR;      /*!<FLASH OPT key register,                        Address offset: 0x08 */
    __IO uint32_t SR;           /*!<FLASH status register,                         Address offset: 0x0C */
    __IO uint32_t CR;           /*!<FLASH control register,                        Address offset: 0x10 */
    __IO uint32_t AR;           /*!<FLASH address register,                        Address offset: 0x14 */
    __IO uint32_t RESERVED;     /*!< Reserved,                                                     0x18 */
    __IO uint32_t OBR;          /*!<FLASH option bytes register,                   Address offset: 0x1C */
    __IO uint32_t WRPR;         /*!<FLASH option bytes register,                   Address offset: 0x20 */
    __IO uint32_t OP_MODE;      /*!<FLASH option bytes register,                   Address offset: 0x24 */  
} FLASH_TypeDef;

/** 
  * @brief Option Bytes Registers
  */
typedef struct
{
    __IO uint16_t RDP;
    __IO uint16_t USER;
    __IO uint16_t Data0;
    __IO uint16_t Data1;
    __IO uint16_t WRP0;
    __IO uint16_t WRP1;
    __IO uint16_t WRP2;
    __IO uint16_t WRP3;
} OB_TypeDef;  /* referent chat 2.6 */
/* =========================================================================================================================== */
/* ================                                            CRC                                            ================ */
/* =========================================================================================================================== */
/**
  * @brief CRC_Register (CRC)
  */

/** 
  * @brief CRC calculation unit 
  */

typedef struct
{
    __IO uint32_t DR;          /*!< CRC Data register,                           Address offset: 0x00 */
    __IO uint8_t  IDR;         /*!< CRC Independent data register,               Address offset: 0x04 */
    __IO uint32_t CR;          /*!< CRC Control register,                        Address offset: 0x08 */
    __IO uint32_t INIT;        /*!< Initial CRC value register,                  Address offset: 0x0C */
} CRC_TypeDef;


/* =========================================================================================================================== */
/* ================                                            CRC                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief Power Control Register
  */

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t CSR;
} PMU_TypeDef;

/* =========================================================================================================================== */
/* ================                                            RCMU                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief   RESET and clock  Register
  */
typedef struct
{
    __IO uint32_t CR;         /*!< RCMU clock control register,                                  Address offset: 0x00 */
    __IO uint32_t CFGR;       /*!< RCMU clock configuration register,                            Address offset: 0x04 */
    __IO uint32_t CIR;        /*!< RCMU clock interrupt register,                                Address offset: 0x08 */
    __IO uint32_t APB2RSTR;   /*!< RCMU APB2 peripheral reset register,                          Address offset: 0x0C */
    __IO uint32_t APB1RSTR;   /*!< RCMU APB1 peripheral reset register,                          Address offset: 0x10 */
    __IO uint32_t AHBENR;     /*!< RCMU AHB peripheral clock register,                           Address offset: 0x14 */
    __IO uint32_t APB2ENR;    /*!< RCMU APB2 peripheral clock enable register,                   Address offset: 0x18 */
    __IO uint32_t APB1ENR;    /*!< RCMU APB1 peripheral clock enable register,                   Address offset: 0x1C */
    __IO uint32_t BDCR;       /*!< RCMU Backup domain control register,                          Address offset: 0x20 */ 
    __IO uint32_t CSR;        /*!< RCMU clock control & status register,                         Address offset: 0x24 */
    __IO uint32_t AHBRSTR;    /*!< RCMU AHB peripheral reset register,                           Address offset: 0x28 */
    __IO uint32_t CFGR2;      /*!< RCMU clock configuration register 2,                          Address offset: 0x2C */
    __IO uint32_t RESERVED[4];/*!< Reserved,                                                                0x30~0X3F */
    __IO uint32_t PLLCR;      /*!< RCMU PLL,                                                     Address offset: 0x40 */
    __IO uint32_t PLLCR2;     /*!< RCMU PLL2 ,                                                   Address offset: 0x44 */
    __IO uint32_t RSVD[5];    /*!< Reserved,                                                              0x48~0X5B */
    __IO uint32_t STCR;       /*!< RCMU STCR,                                                    Address offset: 0x5C */
    __IO uint32_t IRQLATENCY; /*!< RCMU STCR,                                                    Address offset: 0x60 */
} RCMU_TypeDef;

/* =========================================================================================================================== */
/* ================                                            RTC                                            ================ */
/* =========================================================================================================================== */
/**
  * @brief RTC (RTC)
  */

typedef struct
{
    __IO uint32_t CR;                          /*!< (@ 0x00000000) CR                                            */
    __IO uint32_t TIME;                        /*!< (@ 0x00000004) TIME                                          */
    __IO uint32_t DATE;                        /*!< (@ 0x00000008) DATE                                          */
    __IO uint32_t ALM1TIME;                    /*!< (@ 0x0000000c) ALM                                           */
    __IO uint32_t ALM1DATE;                    /*!< (@ 0x00000010) ALM                                           */
    __IO uint32_t ALM2PRD;                     /*!< (@ 0x00000014) ALM PR                                        */
    __IO uint32_t CLKCALR;                     /*!< (@ 0x00000018) CAL                                           */
    __IO uint32_t ISR;                         /*!< (@ 0x0000001c)                                               */
    __IO uint32_t INTCLR;                      /*!< (@ 0x00000020)                                               */
    __IO uint32_t WPR;                         /*!< (@ 0x00000024)                                               */
    __IO uint32_t TSTR;                        /*!< (@ 0x00000028)                                               */
    __IO uint32_t TSDR;                        /*!< (@ 0x0000002c)                                               */
    __IO uint32_t TAFCR;                       /*!< (@ 0x00000030)                                               */
    __IO uint32_t RSVD[7];                     /*!< (@ 0x00000032~0x4f)                                          */
    __IO uint32_t BKP0R;                       /*!< (@ 0x00000050)                                               */
    __IO uint32_t BKP1R;                       /*!< (@ 0x00000054)                                               */
    __IO uint32_t BKP2R;                       /*!< (@ 0x00000058)                                               */
    __IO uint32_t BKP3R;                       /*!< (@ 0x0000005c)                                               */
    __IO uint32_t BKP4R;                       /*!< (@ 0x00000060)                                               */
} RTC_TypeDef;


/* =========================================================================================================================== */
/* ================                                           GPIO                                            ================ */
/* =========================================================================================================================== */

/** 
  * @brief GPIO
  */
typedef struct
{                  
    __IO uint32_t  MODER;           /*!< (@ 0x00000000) Input Output model Register                          */
    __IO uint32_t  OTYPER;          /*!< (@ 0x00000004) Output Type register, 0:push-pull, 1:open-drain      */
    __IO uint32_t  OSPEEDER;        /*!< (@ 0x00000008) Output Speed Register                                */
    __IO uint32_t  PUPDER;          /*!< (@ 0x0000000C) PullUp and PullDown Register                         */
    __IO uint32_t  IDR;             /*!< (@ 0x00000010) Input data Register                                  */
    __IO uint32_t  ODR;             /*!< (@ 0x00000014) Output data Register                                 */
    __IO uint32_t  BSRR;            /*!< (@ 0x00000018) Output logic bit config,bit[15:0]IOyS,bit[31:16]IOyC */
    __IO uint32_t  LCKR;            /*!< (@ 0x0000001C) IO config Lock Register                              */
    __IO uint32_t  AFRL;            /*!< (@ 0x00000020) Alternate Function Register Low                      */
    __IO uint32_t  AFRH;            /*!< (@ 0x00000024) Alternate Function Register High                     */
    __IO uint32_t  BRR;             /*!< (@ 0x00000028) Output logic bit config,bit[15:0]IOyC                */
    __IO uint32_t  SR;              /*!< (@ 0x0000002C) Driver Strength Config                               */
} GPIO_TypeDef;

/* =========================================================================================================================== */
/* ================                                           SYSCFG                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief SysTem Configuration
  */

typedef struct
{
    __IO uint32_t CFGR1;         /*!< SYSCFG configuration register 1,                   Address offset: 0x00 */
    __IO uint32_t CFGR2;         /*!<                                                    Address offset: 0x04 */
    __IO uint32_t EXTICR[4];     /*!< SYSCFG external interrupt configuration register,  Address offset: 0x08 */
    __IO uint32_t IRCR;          /*!<                                                    Address offset: 0x18 */  
}SYSCFG_TypeDef;

/* =========================================================================================================================== */
/* ================                                           IEMU                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief EXTI Register
  */

typedef struct
{
    __IO uint32_t IMR;          /*!< Address offset: 0x00 */
    __IO uint32_t EMR;          /*!< Address offset: 0x04 */
    __IO uint32_t RTSR;         /*!< Address offset: 0x08 */
    __IO uint32_t FTSR;         /*!< Address offset: 0x0c */
    __IO uint32_t SWIER;        /*!< Address offset: 0x10 */
    __IO uint32_t PR;           /*!< Address offset: 0x14 */
    __IO uint32_t IRCR;         /*!< Address offset: 0x18 */
}EXTI_TypeDef;

typedef EXTI_TypeDef IEMU_TypeDef;
/* =========================================================================================================================== */
/* ================                                            ADC                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief ADC
  */
typedef struct
{
    __IO uint32_t  INTR;                         /*!< (@ 0x00000000) ADC Interrupt status Register                            */
    __IO uint32_t  CR;                           /*!< (@ 0x00000004) ADC Control Register                                       */
    __IO uint32_t  DR;                           /*!< (@ 0x00000008) ADC Data Register                                          */
    __IO uint32_t  SR;                           /*!< (@ 0x0000000C) ADC Status Register                                        */
    __IO uint32_t  SEQ1CR1;                      /*!< (@ 0x00000010) SEQ1 Control Register 1                                    */
    __IO uint32_t  SEQ1CR2;                      /*!< (@ 0x00000014) SEQ1 Control Register 2                                    */
    __IO uint32_t  SEQ1CR3;                      /*!< (@ 0x00000018) SEQ1 Control Register 3                                    */
    __IO uint32_t  SEQ1CR4;                      /*!< (@ 0x0000001C) SEQ1 Control Register 4                                    */
    __IO uint32_t  SEQ1SR1;                      /*!< (@ 0x00000020) SEQ1 Status Register 1                                     */
    __IO uint32_t  SEQ1SR2;                      /*!< (@ 0x00000024) SEQ1 Status Register 2                                     */
    __IO uint32_t  SEQ1SR3;                      /*!< (@ 0x00000028) SEQ1 Status Register 3                                     */
    __IO uint32_t  SEQ1SR4;                      /*!< (@ 0x0000002C) SEQ1 Status Register 4                                     */
    __IO uint32_t  SEQ1SR5;                      /*!< (@ 0x00000030) SEQ1 Status Register 5                                     */
    __IO uint32_t  SEQ1SR6;                      /*!< (@ 0x00000034) SEQ1 Status Register 6                                     */
    __IO uint32_t  SEQ1SR7;                      /*!< (@ 0x00000038) SEQ1 Status Register 7                                     */
    __IO uint32_t  SEQ1SR8;                      /*!< (@ 0x0000003C) SEQ1 Status Register 8                                     */
    __IO uint32_t  SEQ1SR9;                      /*!< (@ 0x00000040) SEQ1 Status Register 9                                     */
    __IO uint32_t  SEQ1SR10;                     /*!< (@ 0x00000044) SEQ1 Status Register 10                                    */
    __IO uint32_t  SEQ1SR11;                     /*!< (@ 0x00000048) SEQ1 Status Register 11                                    */
    __IO uint32_t  SEQ1SR12;                     /*!< (@ 0x0000004C) SEQ1 Status Register 12                                    */
    __IO uint32_t  SEQ1SR13;                     /*!< (@ 0x00000050) SEQ1 Status Register 13                                    */
    __IO uint32_t  SEQ1SR14;                     /*!< (@ 0x00000054) SEQ1 Status Register 14                                    */
    __IO uint32_t  SEQ1SR15;                     /*!< (@ 0x00000058) SEQ1 Status Register 15                                    */
    __IO uint32_t  SEQ1SR16;                     /*!< (@ 0x0000005C) SEQ1 Status Register 16                                    */
    __IO uint32_t  SEQ1SR17;                     /*!< (@ 0x00000060) SEQ1 Status Register 17                                    */
    __IO uint32_t  SEQ1SR18;                     /*!< (@ 0x00000064) SEQ1 Status Register 18                                    */
    __IO uint32_t  SEQ2CR1;                      /*!< (@ 0x00000068) SEQ1 Control Register 1                                    */
    __IO uint32_t  SEQ2CR2;                      /*!< (@ 0x0000006C) SEQ1 Control Register 2                                    */
    __IO uint32_t  SEQ2SR1;                      /*!< (@ 0x00000070) SEQ2 Status Register 1                                     */
    __IO uint32_t  SEQ2SR2;                      /*!< (@ 0x00000074) SEQ2 Status Register 2                                     */
    __IO uint32_t  SEQ2SR3;                      /*!< (@ 0x00000078) SEQ2 Status Register 3                                     */
    __IO uint32_t  SEQ2SR4;                      /*!< (@ 0x0000007C) SEQ2 Status Register 4                                     */
    __IO uint32_t  ACCR;                         /*!< (@ 0x00000080) ADC Accumulative Register                                  */
    __IO uint32_t  AWDCR;                        /*!< (@ 0x00000084) ADC Watchdog Control Register                              */
} ADC_TypeDef;

/* =========================================================================================================================== */
/* ================                                           DMA                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief DMA
  */

typedef struct
{
    __IO uint32_t INTSTA;        /*!< 0x00 */
    __IO uint32_t INTTCSTA;      /*!< 0x04 */
    __IO uint32_t INTTCCLR;     /*!< 0x08 */
    __IO uint32_t INTERRSTA;     /*!< 0x0c */
    __IO uint32_t INTERRCLR;     /*!< 0x10 */
    __IO uint32_t RAWINTTCSTA;   /*!< 0x14 */
    __IO uint32_t RAWINTERRSTA;  /*!< 0x18 */
    __IO uint32_t ENBLDCHNSTA;   /*!< 0x1c */

    __IO uint32_t SOFTBREQ;      /*!< 0x20 */
    __IO uint32_t SOFTSREQ;      /*!< 0x24 */
    __IO uint32_t OVSTA;         /*!< 0x28 */ 
    __IO uint32_t OVCLR;         /*!< 0x2c */

    __IO uint32_t CONFIG;        /*!< 0x30 */
    __IO uint32_t PERIPH0_SEL;   /*!< 0x34 */
    __IO uint32_t PERIPH1_SEL;   /*!< 0x38 */
    __IO uint32_t PERIPH2_SEL;   /*!< 0x3c */
    __IO uint32_t PERIPH3_SEL;   /*!< 0x40 */
    __IO uint32_t PERIPH4_SEL;   /*!< 0x44 */
    __IO uint32_t PERIPH5_SEL;   /*!< 0x48 */

    __IO uint32_t RSVD[45];      /*!< 0x4c~ 0xff*/

    __IO uint32_t CH0_SRCADDR;   /*!< 0x100 */
    __IO uint32_t CH0_DESTADDR;  /*!< 0x104 */
    __IO uint32_t CH0_RSVD1;     /*!< 0x108 */
    __IO uint32_t CH0_CONTROL;   /*!< 0x10c */
    __IO uint32_t CH0_CONFIG;    /*!< 0x110 */
    __IO uint32_t CH0_RSVD2[3];  /*!< 0x114~0x1ff */

    __IO uint32_t CH1_SRCADDR;   /*!< 0x120 */
    __IO uint32_t CH1_DESTADDR;  /*!< 0x124 */
    __IO uint32_t CH1_RSVD1;     /*!< 0x128 */
    __IO uint32_t CH1_CONTROL;   /*!< 0x12c */
    __IO uint32_t CH1_CONFIG;    /*!< 0x130 */
    __IO uint32_t CH1_RSVD2[3];  /*!< 0x134~0x13f */

    __IO uint32_t CH2_SRCADDR;   /*!< 0x140 */
    __IO uint32_t CH2_DESTADDR;  /*!< 0x144 */
    __IO uint32_t CH2_RSVD1;     /*!< 0x148 */
    __IO uint32_t CH2_CONTROL;   /*!< 0x14c */
    __IO uint32_t CH2_CONFIG;    /*!< 0x150 */
    __IO uint32_t CH2_RSVD2[3];  /*!< 0x154~0x15f */

    __IO uint32_t CH3_SRCADDR;   /*!< 0x160 */
    __IO uint32_t CH3_DESTADDR;  /*!< 0x164 */
    __IO uint32_t CH3_RSVD1;     /*!< 0x168 */
    __IO uint32_t CH3_CONTROL;   /*!< 0x16c */
    __IO uint32_t CH3_CONFIG;    /*!< 0x170 */
    __IO uint32_t CH3_RSVD2[3];  /*!< 0x174~0x17f */

    __IO uint32_t CH4_SRCADDR;   /*!< 0x180 */
    __IO uint32_t CH4_DESTADDR;  /*!< 0x184 */
    __IO uint32_t CH4_RSVD1;     /*!< 0x188 */
    __IO uint32_t CH4_CONTROL;   /*!< 0x18c */
    __IO uint32_t CH4_CONFIG;    /*!< 0x190 */
    __IO uint32_t CH4_RSVD2[3];  /*!< 0x194~0x19f */

    __IO uint32_t CH5_SRCADDR;   /*!< 0x1A0 */
    __IO uint32_t CH5_DESTADDR;  /*!< 0x1A4 */
    __IO uint32_t CH5_RSVD1;     /*!< 0x1AA */
    __IO uint32_t CH5_CONTROL;   /*!< 0x1Ac */
    __IO uint32_t CH5_CONFIG;    /*!< 0x1B0 */
    __IO uint32_t CH5_RSVD2[3];  /*!< 0x1B4~0x1Bf */
}DMA_TypeDef;

/* =========================================================================================================================== */
/* ================                                 Advance Timer                                             ================ */
/* =========================================================================================================================== */
/**
  * @brief Advance Timer 
  */

/** 
  * @brief TIM
  */
typedef struct
{
    __IO uint32_t CR1;             /*!< TIM control register 1,                      Address offset: 0x00 */
    __IO uint32_t CR2;             /*!< TIM control register 2,                      Address offset: 0x04 */
    __IO uint32_t SMCR;            /*!< TIM slave Mode Control register,             Address offset: 0x08 */
    __IO uint32_t DIER;            /*!< TIM DMA/interrupt enable register,           Address offset: 0x0C */
    __IO uint32_t SR;              /*!< TIM status register,                         Address offset: 0x10 */
    __IO uint32_t EGR;             /*!< TIM event generation register,               Address offset: 0x14 */
    __IO uint32_t CCMR1;           /*!< TIM  capture/compare mode register 1,        Address offset: 0x18 */
    __IO uint32_t CCMR2;           /*!< TIM  capture/compare mode register 2,        Address offset: 0x1C */
    __IO uint32_t CCER;            /*!< TIM capture/compare enable register,         Address offset: 0x20 */
    __IO uint32_t CNT;             /*!< TIM counter register,                        Address offset: 0x24 */
    __IO uint32_t PSC;             /*!< TIM prescaler register,                      Address offset: 0x28 */
    __IO uint32_t ARR;             /*!< TIM auto-reload register,                    Address offset: 0x2C */
    __IO uint32_t RCR;             /*!< TIM  repetition counter register,            Address offset: 0x30 */
    __IO uint32_t CCR1;            /*!< TIM capture/compare register 1,              Address offset: 0x34 */
    __IO uint32_t CCR2;            /*!< TIM capture/compare register 2,              Address offset: 0x38 */  
    __IO uint32_t CCR3;            /*!< TIM capture/compare register 3,              Address offset: 0x3C */
    __IO uint32_t CCR4;            /*!< TIM capture/compare register 4,              Address offset: 0x40 */
    __IO uint32_t BDTR;            /*!< TIM break and dead-time register,            Address offset: 0x44 */
    __IO uint32_t DCR;             /*!< TIM DMA control register,                    Address offset: 0x48 */
    __IO uint32_t DMAR;            /*!< TIM DMA address for full transfer register,  Address offset: 0x4C */
} TIM_TypeDef;

/* =========================================================================================================================== */
/* ================                                           IWDG                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief Independ Watch Dog (IWDG)
  */

/** 
  * @brief Independent WATCHDOG
  */
typedef struct
{
    __IO uint32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
    __IO uint32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
    __IO uint32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
    __IO uint32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
} IWDG_TypeDef;

/* =========================================================================================================================== */
/* ================                                           WWDG                                            ================ */
/* =========================================================================================================================== */
/**
  * @brief Independ Watch Dog (WWDG)
  */

/** 
  * @brief Window WATCHDOG
  */
typedef struct
{
    __IO uint32_t CR;   /*!< WWDG Control register,       Address offset: 0x00 */
    __IO uint32_t CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
    __IO uint32_t SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;
/* =========================================================================================================================== */
/* ================                                            I2C                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief I2C Register (I2C)
  */

/** 
  * @brief Inter-integrated Circuit Interface
  */
typedef struct
{
    __IO uint32_t CON;                                      /*!< control register,                                              Address offset: 0x00 */
    __IO uint32_t TAR;                                      /*!< target address register,                                       Address offset: 0x04 */                   
    __IO uint32_t SAR;                                      /*!< slave address register,                                        Address offset: 0x08 */
    __IO uint32_t RSVD;                                     /*!< 0x0c~ 0x0f,                                                    Address offset: 0x0c */
    __IO uint32_t DATA_CMD;                                 /*!< i2c rx/tx data buffer and command register,                    Address offset: 0x10 */
    __IO uint32_t SS_SCL_HCNT;                              /*!< standard speed i2c clock scl high count register,              Address offset: 0x14 */
    __IO uint32_t SS_SCL_LCNT;                              /*!< standard speed i2c clock scl low count register,               Address offset: 0x18 */
    __IO uint32_t FS_SCL_HCNT;                              /*!< fast mode or fast mode plus i2c clock scl high count register, Address offset: 0x1C */
    __IO uint32_t FS_SCL_LCNT;                              /*!< fast mode or fast mode plus clock scl low count register,      Address offset: 0x20 */

    __IO uint32_t RSVD0[2];                                 /*!< 0x24~ 0x2B*/
    __IO uint32_t INTR_STAT;                                /*!< i2c interrupt status register,                                 Address offset: 0x2C */
    __IO uint32_t INTR_MASK;                                /*!< interrupt mask register,                                       Address offset: 0x30 */
    __IO uint32_t RAW_INTR_STAT;                            /*!< raw interrupt status register,                                 Address offset: 0x34 */
    __IO uint32_t RX_TL;                                    /*!< i2c receive FIFO threshold register,                           Address offset: 0x38 */
    __IO uint32_t TX_TL;                                    /*!< i2c transimit FIFO threshold register,                         Address offset: 0x3C */
    __IO uint32_t CLR_INTR;                                 /*!< clear combined and individual interrupt register,              Address offset: 0x40 */
    __IO uint32_t CLR_RX_UNDER;                             /*!< clear RX_UNDER interrupt register,                             Address offset: 0x44 */
    __IO uint32_t CLR_RX_OVER;                              /*!< clear RX_OVER interrupt register,                              Address offset: 0x48 */
    __IO uint32_t CLR_TX_OVER;                              /*!< clear TX_OVER interrupt register,                              Address offset: 0x4C */ 
    __IO uint32_t CLR_RD_REQ;                               /*!< clear RD_REQ interrupt register,                               Address offset: 0x50 */
    __IO uint32_t CLR_TX_ABRT;                              /*!< clear TX_ABRT interrupt register,                              Address offset: 0x54 */
    __IO uint32_t CLR_RX_DONE;                              /*!< clear RX_DONE interrupt register,                              Address offset: 0x58 */
    __IO uint32_t CLR_ACTIVITY;                             /*!< clear ACTIVITY interrupt register,                             Address offset: 0x5C */
    __IO uint32_t CLR_STOP_DET;                             /*!< clear STOP_DET interrupt register,                             Address offset: 0x60 */
    __IO uint32_t CLR_START_DET;                            /*!< clear START_DET interrupt register,                            Address offset: 0x64 */
    __IO uint32_t CLR_GEN_CALL;                             /*!< clear GEN_CALL interrupt register,                             Address offset: 0x68 */
    __IO uint32_t ENABLE;                                   /*!< i2c enable register,                                           Address offset: 0x6C */
    __IO uint32_t STATUS;                                   /*!< i2c status register,                                           Address offset: 0x70 */
    __IO uint32_t TXFLR;                                    /*!< i2c transmit FIFO level register,                              Address offset: 0x74 */
    __IO uint32_t RXFLR;                                    /*!< i2c receive FIFO level register,                               Address offset: 0x78 */
    __IO uint32_t SDA_HOLD;                                 /*!< i2c SDA hold time length register,                             Address offset: 0x7C */
    __IO uint32_t TX_ABRT_SOURCE;                           /*!< i2c transmit abort source register,                            Address offset: 0x80 */
    __IO uint32_t RSVD1[1];                                 /*!< 0x84~ 0x87*/
    __IO uint32_t DMA_CR;                                   /*!< DMA control register,                                          Address offset: 0x88 */
    __IO uint32_t DMA_TDLR;                                 /*!< DMA transmit data level register,                              Address offset: 0x8C */
    __IO uint32_t DMA_RDLR;                                 /*!< i2c receive data level register,                               Address offset: 0x90 */
    __IO uint32_t SDA_SETUP;                                /*!< i2c sda setup register,                                        Address offset: 0x94 */
    __IO uint32_t ACK_GENERAL_CALL;                         /*!< i2c ack general call register,                                 Address offset: 0x98 */
    __IO uint32_t ENABLE_STATUS;                            /*!< i2c enable status register,                                    Address offset: 0x9C */
    __IO uint32_t FS_SPKLEN;                                /*!< i2c SS and FS spike suppression limit register,                Address offset: 0xA0 */
    __IO uint32_t RSVD2[2];                                 /*!< 0xA4~ 0xAB*/
    __IO uint32_t SCL_STUCK_AT_LOW_TIMEOUT;                 /*!< SCL stuck at low timeout register,                             Address offset: 0xAC */
    __IO uint32_t SDA_STUCK_AT_LOW_TIMEOUT;                 /*!< SDA stuck at low timeout register,                             Address offset: 0xB0 */
    __IO uint32_t CLR_SCL_STUCK_DET;                        /*!< clear SCL stuck at low timeout register,                       Address offset: 0xB4 */   

    __IO uint32_t SMBUS_CLK_LOW_SEXT_TIMEOUT;               /*!< SMBUS Slave Clock Extend Timeout register,                     Address offset: 0xBC */
    __IO uint32_t SMBUS_CLK_LOW_MEXT_TIMEOUT;               /*!< SMBUS master extend clock timeout register,                    Address offset: 0xC0 */
    __IO uint32_t SMBUS_THIGH_MAX_IDLE_COUNT;               /*!< SMBUS Thigh Max Bus-Idle count register,                       Address offset: 0xC4 */
    __IO uint32_t SMBUS_INTR_STAT;                          /*!< i2c SMBUS Interrupt status register,                           Address offset: 0xC8 */
    __IO uint32_t SMBUS_INTR_MASK;                          /*!< i2c interrupt mask register,                                   Address offset: 0xCC */   
    __IO uint32_t SMBUS_INTR_RAW_STATUS;                    /*!< i2c smbus raw interrupt status register,                       Address offset: 0xD0 */   
    __IO uint32_t CLR_SMBUS_INTR;                           /*!< clear smbus interrupt register,                                Address offset: 0xD4 */   
    __IO uint32_t OPTIONAL_SAR;                             /*!< i2c optional slave address register,                           Address offset: 0xD8 */   
    __IO uint32_t SMBUS_UDID;                               /*!< smbus ARP UDID LSB register,                                       Address offset: 0xDC */   
}I2C_TypeDef;

/* =========================================================================================================================== */
/* ================                                            SPI                                            ================ */
/* =========================================================================================================================== */
/**
  * @brief SPI Register (SPI)
  */

/** 
  * @brief Serial Peripheral Interface
  */
  
typedef struct
{
    __IO uint16_t CR1;      /*!< SPI Control register 1 (not used in I2S mode),       Address offset: 0x00 */
    uint16_t  RESERVED0;    /*!< Reserved, 0x02                                                            */
    __IO uint16_t CR2;      /*!< SPI Control register 2,                              Address offset: 0x04 */
    uint16_t  RESERVED1;    /*!< Reserved, 0x06                                                            */
    __IO uint16_t SR;       /*!< SPI Status register,                                 Address offset: 0x08 */
    uint16_t  RESERVED2;    /*!< Reserved, 0x0A                                                            */
    __IO uint16_t DR;       /*!< SPI data register,                                   Address offset: 0x0C */
    uint16_t  RESERVED3;    /*!< Reserved, 0x0E                                                            */
    __IO uint16_t CRCPR;    /*!< SPI CRC polynomial register (not used in I2S mode),  Address offset: 0x10 */
    uint16_t  RESERVED4;    /*!< Reserved, 0x12                                                            */
    __IO uint16_t RXCRCR;   /*!< SPI Rx CRC register (not used in I2S mode),          Address offset: 0x14 */
    uint16_t  RESERVED5;    /*!< Reserved, 0x16                                                            */
    __IO uint16_t TXCRCR;   /*!< SPI Tx CRC register (not used in I2S mode),          Address offset: 0x18 */
    uint16_t  RESERVED6;    /*!< Reserved, 0x1A                                                            */ 
} SPI_TypeDef;

/* =========================================================================================================================== */
/* ================                                           UART                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief UART Register
  */

/** 
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
  
typedef struct
{
    __IO uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
    __IO uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
    __IO uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
    __IO uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
    __IO uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
    __IO uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
    __IO uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

/* =========================================================================================================================== */
/* ================                                          DBG                                              ================ */
/* =========================================================================================================================== */

/**
  * @brief Debug MCU Module
  */
/** 
  * @brief Debug MCU
  */

typedef struct
{
    __IO uint32_t ID;             /*!< Address offset: 0x00 */
    __IO uint32_t CTL;            /*!< Address offset: 0x04 */
    __IO uint32_t RESERVED;       /*!< Address offset: 0x08 */
    __IO uint32_t DBGEN;          /*!< Address offset: 0x0C */
}DBGMCU_TypeDef;


/* =========================================================================================================================== */
/* ================                                           LPTIM                                           ================ */
/* =========================================================================================================================== */

/**
  * @brief Low Power Timer (LPTIM)
  */

typedef struct
{
    __IO uint32_t  CNTVAL;                       /*!< (@ 0x00000000) Low Power Count Read_Only Register                         */
    __IO uint32_t  CR;                           /*!< (@ 0x00000004) LPTIM Control Register                                     */
    __IO uint32_t  LOAD;                         /*!< (@ 0x00000008) LPTIM Reload Register                                      */
    __IO uint32_t  INTSR;                        /*!< (@ 0x0000000C) LPTIM Interrupt Status                                     */
    __IO uint32_t  INTCLR;                       /*!< (@ 0x00000010) LPTIM Interrupt Clear Register                             */
    __IO uint32_t  BGLOAD;                       /*!< (@ 0x00000014) LPTIM Circle reload Register                               */
} LPTIM_TypeDef;                                   /*!< Size = 24 (0x18)    */


/* =========================================================================================================================== */
/* ================                                          LPUART                                           ================ */
/* =========================================================================================================================== */

/**
  * @brief Low Power UART Register (LPUART)
  */

typedef struct
{
    __IO uint32_t  SBUF;                         /*!< (@ 0x00000000) Data BUFF                                                  */
    __IO uint32_t  SCON;                         /*!< (@ 0x00000004) Low Power UART Control Register                            */
    __IO uint32_t  SADDR;                        /*!< (@ 0x00000008) UART0_Address Register                                     */
    __IO uint32_t  SADEN;                        /*!< (@ 0x0000000C) UART0_Address Mask Register                                */
    __IO uint32_t  INTSR;                        /*!< (@ 0x00000010) UART0_Interrupt flag Status Register                       */
    __IO uint32_t  INTCLR;                       /*!< (@ 0x00000014) UART0 Interruput flag Clear Register                       */
    __IO uint32_t  BAUDCR;                       /*!< (@ 0x00000018) UART0_ Baud Control Register                               */
} LPUART_TypeDef;

/* =========================================================================================================================== */
/* ================                                           BEEP                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief BEEP (BEEP)
  */

typedef struct
{
    __IO uint32_t  CSR;                          /*!< (@ 0x00000000) BEEP Control Status Register                               */
} BEEP_TypeDef;                                /*!< Size = 4 (0x4)  */   

/* =========================================================================================================================== */
/* ================                                            VC                                             ================ */
/* =========================================================================================================================== */

/**
  * @brief VC (VC)
  */

typedef struct
{
    __IO uint32_t  VCCR;                          /*!< (@ 0x00000000) VC Control 0                                               */
    __IO uint32_t  VCSR;                          /*!< (@ 0x00000004) VC Control 1                                               */
    __IO uint32_t  VC0CR;                         /*!< (@ 0x00000008) VC Output Config                                           */
    __IO uint32_t  RSVD;                          /*!< (@ 0x0000000C) VC Status Register                                         */  
    __IO uint32_t  VC1CR;                         /*!< (@ 0x0000000C) VC Status Register                                         */
} VC_TypeDef;

/* =========================================================================================================================== */
/* ================                                            LVD                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief LVD (LVD)
  */

typedef struct 
{
    __IO uint32_t  CR;                           /*!< (@ 0x00000000) Control Register                                           */
    __IO uint32_t  SR;                           /*!< (@ 0x00000004) LVD Status                                                 */
} LVD_TypeDef;

/* =========================================================================================================================== */
/* ================                                            RNG                                            ================ */
/* =========================================================================================================================== */
/** 
  * @brief RNG
  */
  
typedef struct 
{
    __IO uint32_t CR;  /*!< RNG control register, Address offset: 0x00 */
    __IO uint32_t SR;  /*!< RNG status register,  Address offset: 0x04 */
    __IO uint32_t DR;  /*!< RNG data register,    Address offset: 0x08 */
} TRNG_TypeDef;

/* =========================================================================================================================== */
/* ================                                            AES                                            ================ */
/* =========================================================================================================================== */

/** 
  * @brief AES hardware accelerator
  */
typedef struct
{
    __IO uint32_t CR;      /*!< AES control register,                        Address offset: 0x00 */
    __IO uint32_t SR;      /*!< AES status register,                         Address offset: 0x04 */
    __IO uint32_t DINR;    /*!< AES data input register,                     Address offset: 0x08 */
    __IO uint32_t DOUTR;   /*!< AES data output register,                    Address offset: 0x0C */
    __IO uint32_t KEYR0;   /*!< AES key register 0,                          Address offset: 0x10 */
    __IO uint32_t KEYR1;   /*!< AES key register 1,                          Address offset: 0x14 */
    __IO uint32_t KEYR2;   /*!< AES key register 2,                          Address offset: 0x18 */
    __IO uint32_t KEYR3;   /*!< AES key register 3,                          Address offset: 0x1C */
    __IO uint32_t IVR0;    /*!< AES initialization vector register 0,        Address offset: 0x20 */
    __IO uint32_t IVR1;    /*!< AES initialization vector register 1,        Address offset: 0x24 */
    __IO uint32_t IVR2;    /*!< AES initialization vector register 2,        Address offset: 0x28 */
    __IO uint32_t IVR3;    /*!< AES initialization vector register 3,        Address offset: 0x2C */
} AES_TypeDef;

/* =========================================================================================================================== */
/* ================                                            SRAM                                            ================ */
/* =========================================================================================================================== */

/** 
  * @brief SRAM 
  */
typedef struct
{
    __IO uint32_t CR;      /*!< Address offset: 0x00 */
    __IO uint32_t SR;      /*!< Address offset: 0x04 */
} SRAM_TypeDef;

/* =========================================================================================================================== */
/* ================                                            AWK                                            ================ */
/* =========================================================================================================================== */

/**
  * @brief Auto wake (AWK)
  */

typedef struct 
{
    __IO uint32_t  CR;                           /*!< (@ 0x00000000) Auto Wake Timer Control Register                           */
    __IO uint32_t  RLOAD;                        /*!< (@ 0x00000004) Auto Wake Timer ReLoad Data Register                       */
    __IO uint32_t  SR;                           /*!< (@ 0x00000008) Auto Wake Timer Status Register                            */
    __IO uint32_t  INTCLR;                       /*!< (@ 0x0000000C) Auto Wake Interrupt Clear Register                         */
} AWK_TypeDef;                                 /*!< Size = 16 (0x10)                                                          */

/** @} */ /* End of group Device_Peripheral_peripherals */


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */
#define FLASH_BASE            ((uint32_t)0x08000000) /*!< FLASH base address in the alias region */
#define OB_BASE               ((uint32_t)0x1FFFF800U)    /*!< Flash Option Bytes base address */
#define SRAM_BASE             ((uint32_t)0x20000000) /*!< SRAM base address in the alias region */

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

#define SRAM_BB_BASE          ((uint32_t)0x22000000U) /*!< SRAM base address in the bit-band region */
#define PERIPH_BB_BASE        ((uint32_t)0x42000000U) /*!< Peripheral base address in the bit-band region */

/*!< AHB2PERIPH memory map */
#define APBPERIPH_BASE         PERIPH_BASE
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000)    /*datasheet page24  GPIOA,B,C,D; FASTIOA,B,C,D*/

/*!< GPIOA,B,C,D; FASTIOA,B,C,D memory map */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x00000000)    /*!< 0x4800 0000 - 0x4800 03FF  1 KB    GPIOA    */
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x00000400)    /*!< 0x4800 0400 - 0x4800 07FF  1 KB    GPIOB    */
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x00000800)    /*!< 0x4800 0800 - 0x4800 0BFF  1 KB    GPIOC    */
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x00000C00)    /*!< 0x4800 0C00 - 0x4800 0FFF  1 KB    GPIOD  */
#define FASTIOA_BASE          (AHB2PERIPH_BASE + 0x00010000)    /*!< 0x4801 0000 - 0x4801 03FF  1 KB    FASTIOA */
#define FASTIOB_BASE          (AHB2PERIPH_BASE + 0x00010400)    /*!< 0x4801 0400 - 0x4801 07FF  1 KB    FASTIOB */
#define FASTIOC_BASE          (AHB2PERIPH_BASE + 0x00010800)    /*!< 0x4801 0800 - 0x4801 0BFF  1 KB    FASTIOC */
#define FASTIOD_BASE          (AHB2PERIPH_BASE + 0x00010C00)    /*!< 0x4801 0C00 - 0x4801 0FFF  1 KB    FASTIOD */

/*!< AHB1PERIPH memory map */
#define AHB1PERIPH_BASE       (APBPERIPH_BASE + 0x00020000)     /*!< datasheet page24  DMA RCMU FLASH Interface  SRAM Controller CRC TRNG AES*/

/*!< DMA RCMU FLASH Interface  SRAM Controller CRC TRNG AES memory map */
#define DMA_BASE              (AHB1PERIPH_BASE + 0x00000000)    /*!< 0x4002 0000 - 0x4002 03FF  1 KB    DMA*/
#define RCMU_BASE             (AHB1PERIPH_BASE + 0x00001000)    /*!< 0x4002 1000 - 0x4002_13FF  1 KB    RCMU*/
#define FLASH_IF_BASE         (AHB1PERIPH_BASE + 0x00002000)    /*!< 0x4002 2000 - 0x4002 23FF  1 KB    FLASH Interface*/
#define SRAM_CONTROL_BASE     (AHB1PERIPH_BASE + 0x00002800)    /*!< 0x4002 2800 - 0x4002 2BFF  1 KB    SRAM Controller*/
#define CRC_BASE              (AHB1PERIPH_BASE + 0x00003000)    /*!< 0x4002 3000 - 0x4002 33FF  1 KB    CRC*/
#define TRNG_BASE             (AHB1PERIPH_BASE + 0x00003400)    /*!< 0x4002 3400 - 0x4002 37FF  1 KB    TRNG*/
#define AES_BASE              (AHB1PERIPH_BASE + 0x00003800)    /*!< 0x4002 3800 - 0x4002 3BFF  1 KB    AES*/

/*!< APB2PERIPH memory map */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)      /*!< datasheet page24  SYSCFG EXTI ADC AWK TIM1 SPI1 USART1 LPTIMER TIM15 TIM16 TIM17 LVD BEEP DBGMCU*/

/*!< SYSCFG EXTI ADC AWK TIM1 SPI1 USART1 LPTIMER TIM15 TIM16 TIM17 LVD BEEP DBGMCU memory map */
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x00000000)    /*!< 0x4001 0000 - 0x4001 03FF  1 KB    SYSCFG*/
#define EXTI_BASE             (APB2PERIPH_BASE + 0x00000400)    /*!< 0x4001 0400 - 0x4001 07FF  1 KB    EXTI*/
#define ADC_BASE              (APB2PERIPH_BASE + 0x00002400)    /*!< 0x4001 2400 - 0x4001 27FF  1 KB    ADC*/
#define AWK_BASE              (APB2PERIPH_BASE + 0x00002800)    /*!< 0x4001 2800 - 0x4001 2BFF  1 KB    AWK*/
#define TIM1_BASE             (APB2PERIPH_BASE + 0x00002C00)    /*!< 0x4001 2C00 - 0x4001 2FFF  1 KB    TIM1*/
#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000)    /*!< 0x4001 3000 - 0x4001 33FF  1 KB    SPI1*/
#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800)    /*!< 0x4001 3800 - 0x4001 3BFF  1 KB    USART1*/
#define LPTIMER_BASE          (APB2PERIPH_BASE + 0x00003C00)    /*!< 0x4001 3C00 - 0x4001 3FFF  1 KB    LPTIMER*/
#define TIM15_BASE            (APB2PERIPH_BASE + 0x00004000)    /*!< 0x4001 4000 - 0x4001 43FF  1 KB    TIM15*/
#define TIM16_BASE            (APB2PERIPH_BASE + 0x00004400)    /*!< 0x4001 4400 - 0x4001 47FF  1 KB    TIM16*/
#define TIM17_BASE            (APB2PERIPH_BASE + 0x00004800)    /*!< 0x4001 4800 - 0x4001 4BFF  1 KB    TIM17*/
#define LVD_BASE              (APB2PERIPH_BASE + 0x00005000)    /*!< 0x4001 5000 - 0x4001 53FF  1KB LVD*/
#define BEEP_BASE             (APB2PERIPH_BASE + 0x00005400)    /*!< 0x4001 5400 - 0x4001 57FF  1 KB    BEEP*/
#define DBGMCU_BASE           (APB2PERIPH_BASE + 0x00005800)    /*!< 0x4001 5800 - 0x4001 5BFF  1 KB    DBGMCU*/

/*!< APB1PERIPH memory map */
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000)      /*!< datasheet page24  TIM3 TIM6 TIM7 TIM14 RTC WWDG IWDG SPI2 USART2 LPUART I2C1 I2C2 VC PMU*/

/*!< SYSCFG TIM3 TIM6 TIM7 TIM14 RTC WWDG IWDG SPI2 USART2 LPUART I2C1 I2C2 VC PMU memory map */
#define TIM3_BASE           (APB1PERIPH_BASE + 0x00000400)  /*!<    0x4000 0400 - 0x4000 07FF   1 KB    TIM3*/
#define TIM6_BASE           (APB1PERIPH_BASE + 0x00001000)  /*!<    0x4000 1000 - 0x4000 13FF   1 KB    TIM6*/
#define TIM7_BASE           (APB1PERIPH_BASE + 0x00001400)  /*!<    0x4000 1400 - 0x4000 17FF   1 KB    TIM7*/
#define TIM14_BASE          (APB1PERIPH_BASE + 0x00002000)  /*!<    0x4000 2000 - 0x4000 23FF   1 KB    TIM14*/
#define RTC_BASE            (APB1PERIPH_BASE + 0x00002800)  /*!<    0x4000 2800 - 0x4000 2BFF   1 KB    RTC*/
#define WWDG_BASE           (APB1PERIPH_BASE + 0x00002C00)  /*!<    0x4000 2C00 - 0x4000 2FFF   1 KB    WWDG*/
#define IWDG_BASE           (APB1PERIPH_BASE + 0x00003000)  /*!<    0x4000 3000 - 0x4000 33FF   1 KB    IWDG*/
#define SPI2_BASE           (APB1PERIPH_BASE + 0x00003800)  /*!<    0x4000 3800 - 0x4000 3BFF   1 KB    SPI2*/
#define USART2_BASE         (APB1PERIPH_BASE + 0x00004400)  /*!<    0x4000 4400 - 0x4000 47FF   1 KB    USART2*/
#define LPUART_BASE         (APB1PERIPH_BASE + 0x00005000)  /*!<    0x4000 5000 - 0x4000 53FF   1 KB    LPUART*/
#define I2C1_BASE           (APB1PERIPH_BASE + 0x00005400)  /*!<    0x4000 5400 - 0x4000 57FF   1 KB    I2C1*/
#define I2C2_BASE           (APB1PERIPH_BASE + 0x00005800)  /*!<    0x4000 5800 - 0x4000 5BFF   1 KB    I2C2*/
#define VC_BASE             (APB1PERIPH_BASE + 0x00005C00)  /*!<    0x4000 5C00 - 0x4000 5FFF   1 KB    VC*/
#define PMU_BASE            (APB1PERIPH_BASE + 0x00007000)  /*!<    0x4000 7000 - 0x4000 73FF   1 KB    PMU*/
  
/** @addtogroup Peripheral_declaration
  * @{
  */  
// #define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_IF_BASE)
#define OB                  ((OB_TypeDef *)OB_BASE)

#define CRC                 ((CRC_TypeDef *) CRC_BASE)

#define PMU                 ((PMU_TypeDef *) PMU_BASE) 

#define RCMU                ((RCMU_TypeDef *) RCMU_BASE)

#define RTC                 ((RTC_TypeDef *) RTC_BASE)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)

#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)

#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define IEMU                ((IEMU_TypeDef *) EXTI_BASE)

#define ADC                 ((ADC_TypeDef *) ADC_BASE)

#define DMA                 ((DMA_TypeDef *) DMA_BASE)

#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)

#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)

#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)

#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)

#define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)

#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)

#define LPTIM               ((LPTIM_TypeDef *) LPTIMER_BASE)

#define LPUART              ((LPUART_TypeDef *) LPUART_BASE)

#define BEEP                ((BEEP_TypeDef *) BEEP_BASE)

#define VC                  ((VC_TypeDef *) VC_BASE)
#define LVD                 ((LVD_TypeDef *) LVD_BASE)

#define TRNG                ((TRNG_TypeDef *) TRNG_BASE)

#define AES                 ((AES_TypeDef *) AES_BASE)

#define SRAM                ((SRAM_TypeDef *) SRAM_BASE)

#define AWK                 ((AWK_TypeDef*)AWK_BASE)

/** @} */ /* End of group Device_Peripheral_declaration */


/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */



/** @addtogroup PosMask_peripherals
  * @{
  */

 /*******************  Bit definition for FLASH_ACR register  ******************/
#define FLASH_ACR_LATENCY_Pos               (0U)                               
#define FLASH_ACR_LATENCY_Msk               (0x7U << FLASH_ACR_LATENCY_Pos)    /*!< 0x00000007 */
#define FLASH_ACR_LATENCY                   FLASH_ACR_LATENCY_Msk              /*!< LATENCY[2:0] bits (Latency) */
#define FLASH_ACR_ALL_Msk                    (FLASH_ACR_LATENCY_Msk)
/******************  Bit definition for FLASH_KEYR register ********************/
#define FLASH_KEYR_KEYR_Pos                    (0U)
#define FLASH_KEYR_KEYR_Msk                    (0xFFFFFFFFU << FLASH_KEYR_KEYR_Pos)        /*!<  */
#define FLASH_KEYR_KEYR                        FLASH_KEYR_KEYR_Msk                   /*!<  */

#define FLASH_KEYR_ALL_Msk                    (FLASH_KEYR_KEYR_Msk)
/******************  Bit definition for FLASH_OPTKEYR register ********************/
#define FLASH_OPTKEYR_OPTKEYR_Pos           (0U)                               
#define FLASH_OPTKEYR_OPTKEYR_Msk           (0xFFFFFFFFU << FLASH_OPTKEYR_OPTKEYR_Pos) /*!< 0xFFFFFFFF */
#define FLASH_OPTKEYR_OPTKEYR               FLASH_OPTKEYR_OPTKEYR_Msk          /*!< Option Byte Key */

#define FLASH_OPTKEYR_ALL_Msk                    (FLASH_OPTKEYR_OPTKEYR_Msk)


/******************  Bit definition for FLASH_SR register  ********************/
#define FLASH_SR_BSY_Pos                    (0U)                               
#define FLASH_SR_BSY_Msk                    (0x01U << FLASH_SR_BSY_Pos)         /*!< 0x00000001 */
#define FLASH_SR_BSY                        FLASH_SR_BSY_Msk                   /*!< Busy */
#define FLASH_SR_PGERR_Pos                  (2U)                               
#define FLASH_SR_PGERR_Msk                  (0x01U << FLASH_SR_PGERR_Pos)       /*!< 0x00000004 */
#define FLASH_SR_PGERR                      FLASH_SR_PGERR_Msk                 /*!< Programming Error */
#define FLASH_SR_WRPRTERR_Pos               (4U)                               
#define FLASH_SR_WRPRTERR_Msk               (0x01U << FLASH_SR_WRPRTERR_Pos)    /*!< 0x00000010 */
#define FLASH_SR_WRPRTERR                   FLASH_SR_WRPRTERR_Msk              /*!< Write Protection Error */
#define FLASH_SR_EOP_Pos                    (5U)                               
#define FLASH_SR_EOP_Msk                    (0x01U << FLASH_SR_EOP_Pos)         /*!< 0x00000020 */
#define FLASH_SR_EOP                        FLASH_SR_EOP_Msk                   /*!< End of operation */
#define FLASH_SR_ALL_Msk                    (FLASH_SR_EOP_Msk | FLASH_SR_WRPRTERR_Msk | FLASH_SR_PGERR_Msk | FLASH_SR_BSY_Msk)
/******************  Bit definition for FLASH_CR register ********************/
#define FLASH_CR_PG_Pos                    (0U)
#define FLASH_CR_PG_Msk                    (0x01U << FLASH_CR_PG_Pos)        /*!<  */
#define FLASH_CR_PG                        FLASH_CR_PG_Msk                   /*!<  */
 
#define FLASH_CR_PER_Pos                    (1U)
#define FLASH_CR_PER_Msk                    (0x01U << FLASH_CR_PER_Pos)        /*!<  */
#define FLASH_CR_PER                        FLASH_CR_PER_Msk                   /*!<  */
 
#define FLASH_CR_MER_Pos                    (2U)
#define FLASH_CR_MER_Msk                    (0x01U << FLASH_CR_MER_Pos)        /*!<  */
#define FLASH_CR_MER                        FLASH_CR_MER_Msk                   /*!<  */
 
#define FLASH_CR_OPTPG_Pos                    (4U)
#define FLASH_CR_OPTPG_Msk                    (0x01U << FLASH_CR_OPTPG_Pos)        /*!<  */
#define FLASH_CR_OPTPG                        FLASH_CR_OPTPG_Msk                   /*!<  */
 
#define FLASH_CR_OPTER_Pos                    (5U)
#define FLASH_CR_OPTER_Msk                    (0x01U << FLASH_CR_OPTER_Pos)        /*!<  */
#define FLASH_CR_OPTER                        FLASH_CR_OPTER_Msk                   /*!<  */
 
#define FLASH_CR_STRT_Pos                    (6U)
#define FLASH_CR_STRT_Msk                    (0x01U << FLASH_CR_STRT_Pos)        /*!<  */
#define FLASH_CR_STRT                        FLASH_CR_STRT_Msk                   /*!<  */
 
#define FLASH_CR_LOCK_Pos                    (7U)
#define FLASH_CR_LOCK_Msk                    (0x01U << FLASH_CR_LOCK_Pos)        /*!<  */
#define FLASH_CR_LOCK                        FLASH_CR_LOCK_Msk                   /*!<  */
 
#define FLASH_CR_OPTWRE_Pos                    (9U)
#define FLASH_CR_OPTWRE_Msk                    (0x01U << FLASH_CR_OPTWRE_Pos)        /*!<  */
#define FLASH_CR_OPTWRE                        FLASH_CR_OPTWRE_Msk                   /*!<  */
 
#define FLASH_CR_ERRIE_Pos                    (10U)
#define FLASH_CR_ERRIE_Msk                    (0x01U << FLASH_CR_ERRIE_Pos)        /*!<  */
#define FLASH_CR_ERRIE                        FLASH_CR_ERRIE_Msk                   /*!<  */
 
#define FLASH_CR_EOPIE_Pos                    (12U)
#define FLASH_CR_EOPIE_Msk                    (0x01U << FLASH_CR_EOPIE_Pos)        /*!<  */
#define FLASH_CR_EOPIE                        FLASH_CR_EOPIE_Msk                   /*!<  */
 
#define FLASH_CR_OBL_LAUNCH_Pos                    (13U)
#define FLASH_CR_OBL_LAUNCH_Msk                    (0x01U << FLASH_CR_OBL_LAUNCH_Pos)        /*!<  */
#define FLASH_CR_OBL_LAUNCH                        FLASH_CR_OBL_LAUNCH_Msk                   /*!<  */
 
#define FLASH_CR_ALL_Msk                    (FLASH_CR_OBL_LAUNCH_Msk | FLASH_CR_EOPIE_Msk | FLASH_CR_ERRIE_Msk | FLASH_CR_OPTWRE_Msk | FLASH_CR_LOCK_Msk | FLASH_CR_STRT_Msk | FLASH_CR_OPTER_Msk | FLASH_CR_OPTPG_Msk | FLASH_CR_MER_Msk | FLASH_CR_PER_Msk | FLASH_CR_PG_Msk)
 
/******************  Bit definition for FLASH_AR register ********************/
#define FLASH_AR_FAR_Pos                    (0U)                               
#define FLASH_AR_FAR_Msk                    (0xFFFFFFFFU << FLASH_AR_FAR_Pos)  /*!< 0xFFFFFFFF */
#define FLASH_AR_FAR                        FLASH_AR_FAR_Msk                   /*!< Flash Address */
#define FLASH_AR_ALL_Msk                    (FLASH_AR_FAR_Msk)
/******************  Bit definition for FLASH_OBR register  *******************/
#define FLASH_OBR_OPTERR_Pos                (0U)                               
#define FLASH_OBR_OPTERR_Msk                (0x01U << FLASH_OBR_OPTERR_Pos)     /*!< 0x00000001 */
#define FLASH_OBR_OPTERR                    FLASH_OBR_OPTERR_Msk               /*!< Option Byte Error */
#define FLASH_OBR_RDPRT_Pos                 (1U)                               
#define FLASH_OBR_RDPRT_Msk                 (0x3U << FLASH_OBR_RDPRT_Pos)      /*!< 0x00000002 */
#define FLASH_OBR_RDPRT                     FLASH_OBR_RDPRT_Msk                /*!< Read protection */

#define FLASH_OBR_WDG_SW_Pos                    (8U)
#define FLASH_OBR_WDG_SW_Msk                    (0x01U << FLASH_OBR_WDG_SW_Pos)        /*!<  */
#define FLASH_OBR_WDG_SW                        FLASH_OBR_WDG_SW_Msk                   /*!<  */
 
#define FLASH_OBR_NRST_STOP_Pos                    (9U)
#define FLASH_OBR_NRST_STOP_Msk                    (0x01U << FLASH_OBR_NRST_STOP_Pos)        /*!<  */
#define FLASH_OBR_NRST_STOP                        FLASH_OBR_NRST_STOP_Msk                   /*!<  */
 
#define FLASH_OBR_NRST_STDBY_Pos                    (10U)
#define FLASH_OBR_NRST_STDBY_Msk                    (0x01U << FLASH_OBR_NRST_STDBY_Pos)        /*!<  */
#define FLASH_OBR_NRST_STDBY                        FLASH_OBR_NRST_STDBY_Msk                   /*!<  */
 
#define FLASH_OBR_NBOOT1_Pos                    (11U)
#define FLASH_OBR_NBOOT1_Msk                    (0x01U << FLASH_OBR_NBOOT1_Pos)        /*!<  */
#define FLASH_OBR_NBOOT1                        FLASH_OBR_NBOOT1_Msk                   /*!<  */
 
#define FLASH_OBR_RAM_PARITY_CHECK_Pos                    (13U)
#define FLASH_OBR_RAM_PARITY_CHECK_Msk                    (0x01U << FLASH_OBR_RAM_PARITY_CHECK_Pos)        /*!<  */
#define FLASH_OBR_RAM_PARITY_CHECK                        FLASH_OBR_RAM_PARITY_CHECK_Msk                   /*!<  */
 
#define FLASH_OBR_DATA0_Pos                    (16U)
#define FLASH_OBR_DATA0_Msk                    (0xFFU << FLASH_OBR_DATA0_Pos)        /*!<  */
#define FLASH_OBR_DATA0                        FLASH_OBR_DATA0_Msk                   /*!<  */
 
#define FLASH_OBR_DATA1_Pos                    (24U)
#define FLASH_OBR_DATA1_Msk                    (0xFFU << FLASH_OBR_DATA1_Pos)        /*!<  */
#define FLASH_OBR_DATA1                        FLASH_OBR_DATA1_Msk                   /*!<  */
 
#define FLASH_OBR_ALL_Msk                    (FLASH_OBR_DATA1_Msk | FLASH_OBR_DATA0_Msk | FLASH_OBR_RAM_PARITY_CHECK_Msk | FLASH_OBR_NBOOT1_Msk | FLASH_OBR_NRST_STDBY_Msk | FLASH_OBR_NRST_STOP_Msk | FLASH_OBR_WDG_SW_Msk | FLASH_OBR_RDPRT_Msk | FLASH_OBR_OPTERR_Msk)
 
/******************  Bit definition for FLASH_WRPR register ********************/
#define FLASH_WRPR_WRP0_Pos                    (0U)
#define FLASH_WRPR_WRP0_Msk                    (0xFFU << FLASH_WRPR_WRP0_Pos)        /*!<  */
#define FLASH_WRPR_WRP0                        FLASH_WRPR_WRP0_Msk                   /*!<  */
 
#define FLASH_WRPR_WRP1_Pos                    (8U)
#define FLASH_WRPR_WRP1_Msk                    (0xFFU << FLASH_WRPR_WRP1_Pos)        /*!<  */
#define FLASH_WRPR_WRP1                        FLASH_WRPR_WRP1_Msk                   /*!<  */
 
#define FLASH_WRPR_WRP2_Pos                    (16U)
#define FLASH_WRPR_WRP2_Msk                    (0xFFU << FLASH_WRPR_WRP2_Pos)        /*!<  */
#define FLASH_WRPR_WRP2                        FLASH_WRPR_WRP2_Msk                   /*!<  */
 
#define FLASH_WRPR_WRP3_Pos                    (24U)
#define FLASH_WRPR_WRP3_Msk                    (0xFFU << FLASH_WRPR_WRP3_Pos)        /*!<  */
#define FLASH_WRPR_WRP3                        FLASH_WRPR_WRP3_Msk                   /*!<  */
 
#define FLASH_WRPR_ALL_Msk                    (FLASH_WRPR_WRP3_Msk | FLASH_WRPR_WRP2_Msk | FLASH_WRPR_WRP1_Msk | FLASH_WRPR_WRP0_Msk)
 
/******************  Bit definition for FLASH_OP_MODE register ********************/
#define FLASH_OP_MODE_ERASE_MODE_Pos                    (0U)
#define FLASH_OP_MODE_ERASE_MODE_Msk                    (0x01U << FLASH_OP_MODE_ERASE_MODE_Pos)        /*!<  */
#define FLASH_OP_MODE_ERASE_MODE                        FLASH_OP_MODE_ERASE_MODE_Msk                   /*!<  */
 
#define FLASH_OP_MODE_PREPG_Pos                    (1U)
#define FLASH_OP_MODE_PREPG_Msk                    (0x01U << FLASH_OP_MODE_PREPG_Pos)        /*!<  */
#define FLASH_OP_MODE_PREPG                        FLASH_OP_MODE_PREPG_Msk                   /*!<  */
 
#define FLASH_OP_MODE_ALL_Msk                    (FLASH_OP_MODE_PREPG_Msk | FLASH_OP_MODE_ERASE_MODE_Msk)

/*-----------------------------2.6  Option byte area description, a relatively special register-----------------------------------------------*/
/******************  Bit definition for FLASH_RDP register  *******************/
#define FLASH_RDP_RDP_Pos                   (0U)                               
#define FLASH_RDP_RDP_Msk                   (0xFFU << FLASH_RDP_RDP_Pos)       /*!< 0x000000FF */
#define FLASH_RDP_RDP                       FLASH_RDP_RDP_Msk                  /*!< Read protection option byte */
#define FLASH_RDP_nRDP_Pos                  (8U)                               
#define FLASH_RDP_nRDP_Msk                  (0xFFU << FLASH_RDP_nRDP_Pos)      /*!< 0x0000FF00 */
#define FLASH_RDP_nRDP                      FLASH_RDP_nRDP_Msk                 /*!< Read protection complemented option byte */

/******************  Bit definition for FLASH_USER register  ******************/
#define FLASH_USER_USER_Pos                 (16U)                              
#define FLASH_USER_USER_Msk                 (0xFFU << FLASH_USER_USER_Pos)     /*!< 0x00FF0000 */
#define FLASH_USER_USER                     FLASH_USER_USER_Msk                /*!< user option byte */
#define FLASH_USER_nUSER_Pos                (24U)                              
#define FLASH_USER_nUSER_Msk                (0xFFU << FLASH_USER_nUSER_Pos)    /*!< 0xFF000000 */
#define FLASH_USER_nUSER                    FLASH_USER_nUSER_Msk               /*!< user complemented option byte */

/******************  Bit definition for FLASH_Data0 register  *****************/
#define FLASH_DATA0_DATA0_Pos               (0U)                               
#define FLASH_DATA0_DATA0_Msk               (0xFFU << FLASH_DATA0_DATA0_Pos)   /*!< 0x000000FF */
#define FLASH_DATA0_DATA0                   FLASH_DATA0_DATA0_Msk              /*!< user data storage option byte */
#define FLASH_DATA0_nDATA0_Pos              (8U)                               
#define FLASH_DATA0_nDATA0_Msk              (0xFFU << FLASH_DATA0_nDATA0_Pos)  /*!< 0x0000FF00 */
#define FLASH_DATA0_nDATA0                  FLASH_DATA0_nDATA0_Msk             /*!< user data storage complemented option byte */

/******************  Bit definition for FLASH_Data1 register  *****************/
#define FLASH_DATA1_DATA1_Pos               (16U)                              
#define FLASH_DATA1_DATA1_Msk               (0xFFU << FLASH_DATA1_DATA1_Pos)   /*!< 0x00FF0000 */
#define FLASH_DATA1_DATA1                   FLASH_DATA1_DATA1_Msk              /*!< user data storage option byte */
#define FLASH_DATA1_nDATA1_Pos              (24U)                              
#define FLASH_DATA1_nDATA1_Msk              (0xFFU << FLASH_DATA1_nDATA1_Pos)  /*!< 0xFF000000 */
#define FLASH_DATA1_nDATA1                  FLASH_DATA1_nDATA1_Msk             /*!< user data storage complemented option byte */

/******************  Bit definition for FLASH_WRP0 register  ******************/
#define FLASH_WRP0_WRP0_Pos                 (0U)                               
#define FLASH_WRP0_WRP0_Msk                 (0xFFU << FLASH_WRP0_WRP0_Pos)     /*!< 0x000000FF */
#define FLASH_WRPR0_WRP0                     FLASH_WRP0_WRP0_Msk                /*!< Flash memory write protection option bytes */
#define FLASH_WRP0_nWRP0_Pos                (8U)                               
#define FLASH_WRP0_nWRP0_Msk                (0xFFU << FLASH_WRP0_nWRP0_Pos)    /*!< 0x0000FF00 */
#define FLASH_WRP0_nWRP0                    FLASH_WRP0_nWRP0_Msk               /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP1 register  ******************/
#define FLASH_WRP1_WRP1_Pos                 (16U)                              
#define FLASH_WRP1_WRP1_Msk                 (0xFFU << FLASH_WRP1_WRP1_Pos)     /*!< 0x00FF0000 */
#define FLASH_WRP1_WRP1                     FLASH_WRP1_WRP1_Msk                /*!< Flash memory write protection option bytes */
#define FLASH_WRP1_nWRP1_Pos                (24U)                              
#define FLASH_WRP1_nWRP1_Msk                (0xFFU << FLASH_WRP1_nWRP1_Pos)    /*!< 0xFF000000 */
#define FLASH_WRP1_nWRP1                    FLASH_WRP1_nWRP1_Msk               /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP2 register  ******************/
#define FLASH_WRP2_WRP2_Pos                 (0U)                               
#define FLASH_WRP2_WRP2_Msk                 (0xFFU << FLASH_WRP2_WRP2_Pos)     /*!< 0x000000FF */
#define FLASH_WRP2_WRP2                     FLASH_WRP2_WRP2_Msk                /*!< Flash memory write protection option bytes */
#define FLASH_WRP2_nWRP2_Pos                (8U)                               
#define FLASH_WRP2_nWRP2_Msk                (0xFFU << FLASH_WRP2_nWRP2_Pos)    /*!< 0x0000FF00 */
#define FLASH_WRP2_nWRP2                    FLASH_WRP2_nWRP2_Msk               /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for FLASH_WRP3 register  ******************/
#define FLASH_WRP3_WRP3_Pos                 (16U)                              
#define FLASH_WRP3_WRP3_Msk                 (0xFFU << FLASH_WRP3_WRP3_Pos)     /*!< 0x00FF0000 */
#define FLASH_WRP3_WRP3                     FLASH_WRP3_WRP3_Msk                /*!< Flash memory write protection option bytes */
#define FLASH_WRP3_nWRP3_Pos                (24U)                              
#define FLASH_WRP3_nWRP3_Msk                (0xFFU << FLASH_WRP3_nWRP3_Pos)    /*!< 0xFF000000 */
#define FLASH_WRP3_nWRP3                    FLASH_WRP3_nWRP3_Msk               /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for CRC_DR register ********************/
#define CRC_DR_DR_Pos                    (0U)
#define CRC_DR_DR_Msk                    (0xFFFFFFFFU << CRC_DR_DR_Pos)        /*!<  */
#define CRC_DR_DR                        CRC_DR_DR_Msk                   /*!<  */
 
#define CRC_DR_ALL_Msk                    (CRC_DR_DR_Msk)
 
/******************  Bit definition for CRC_IDR register ********************/
#define CRC_IDR_IDR_Pos                    (0U)
#define CRC_IDR_IDR_Msk                    (0xFFFFFFFFU << CRC_IDR_IDR_Pos)        /*!<  */
#define CRC_IDR_IDR                        CRC_IDR_IDR_Msk                   /*!<  */
 
#define CRC_IDR_ALL_Msk                    (CRC_IDR_IDR_Msk)
 
/******************  Bit definition for CRC_CR register ********************/
#define CRC_CR_RST_Pos                    (0U)
#define CRC_CR_RST_Msk                    (0x01U << CRC_CR_RST_Pos)        /*!<  */
#define CRC_CR_RST                        CRC_CR_RST_Msk                   /*!<  */
 
#define CRC_CR_INV_IN_Pos                    (1U)
#define CRC_CR_INV_IN_Msk                    (0x01U << CRC_CR_INV_IN_Pos)        /*!<  */
#define CRC_CR_INV_IN                        CRC_CR_INV_IN_Msk                   /*!<  */
 
#define CRC_CR_INV_OUT_Pos                    (2U)
#define CRC_CR_INV_OUT_Msk                    (0x01U << CRC_CR_INV_OUT_Pos)        /*!<  */
#define CRC_CR_INV_OUT                        CRC_CR_INV_OUT_Msk                   /*!<  */
 
#define CRC_CR_MODE_Pos                    (3U)
#define CRC_CR_MODE_Msk8                    (0x3U << CRC_CR_MODE_Pos)        /*!<  */
#define CRC_CR_MODE_CRC_8                        CRC_CR_MODE_Msk8                   /*!<  */
 

#define CRC_CR_MODE_Msk_CCITT                    (0x2U << CRC_CR_MODE_Pos)        /*!<  */
#define CRC_CR_MODE_CRC_CCITT                        CRC_CR_MODE_Msk_CCITT 

#define CRC_CR_MODE_Msk16                    (0x1U << CRC_CR_MODE_Pos)        /*!<  */
#define CRC_CR_MODE_CRC16                        CRC_CR_MODE_Msk16 
 
#define CRC_CR_MODE_Msk32                    (0x0U << CRC_CR_MODE_Pos)        /*!<  */
#define CRC_CR_MODE_CRC32                        CRC_CR_MODE_Msk32 
 

#define CRC_CR_REV_IN_Pos                    (5U)
#define CRC_CR_REV_IN_Msk                    (0x01U << CRC_CR_REV_IN_Pos)        /*!<  */
#define CRC_CR_REV_IN                        CRC_CR_REV_IN_Msk                   /*!<  */
 
#define CRC_CR_REV_OUT_Pos                    (7U)
#define CRC_CR_REV_OUT_Msk                    (0x01U << CRC_CR_REV_OUT_Pos)        /*!<  */
#define CRC_CR_REV_OUT                        CRC_CR_REV_OUT_Msk                   /*!<  */
 
#define CRC_CR_ALL_Msk                    (CRC_CR_REV_OUT_Msk | CRC_CR_REV_IN_Msk | CRC_CR_MODE_Msk | CRC_CR_INV_OUT_Msk | CRC_CR_INV_IN_Msk | CRC_CR_RST_Msk)
 
/******************  Bit definition for CRC_INIT register ********************/
#define CRC_INIT_CRC_INIT_Pos                    (0U)
#define CRC_INIT_CRC_INIT_Msk                    (0xFFFFFFFFU << CRC_INIT_CRC_INIT_Pos)        /*!<  */
#define CRC_INIT_CRC_INIT                        CRC_INIT_CRC_INIT_Msk                   /*!<  */
 
#define CRC_INIT_ALL_Msk                    (CRC_INIT_CRC_INIT_Msk)
 
/******************  Bit definition for PMU_CR register ********************/
#define PMU_CR_LPDS_Pos                     (0U)                               
#define PMU_CR_LPDS_Msk                     (0x01U << PMU_CR_LPDS_Pos)          /*!< 0x00000001 */
#define PMU_CR_LPDS                         PMU_CR_LPDS_Msk                    /*!< Low-Power Deepsleep */
#define PMU_CR_PDDS_Pos                     (1U)                               
#define PMU_CR_PDDS_Msk                     (0x01U << PMU_CR_PDDS_Pos)          /*!< 0x00000002 */
#define PMU_CR_PDDS                         PMU_CR_PDDS_Msk                    /*!< Power Down Deepsleep */
#define PMU_CR_CWUF_Pos                     (2U)                               
#define PMU_CR_CWUF_Msk                     (0x01U << PMU_CR_CWUF_Pos)          /*!< 0x00000004 */
#define PMU_CR_CWUF                         PMU_CR_CWUF_Msk                    /*!< Clear Wakeup Flag */
#define PMU_CR_CSBF_Pos                     (3U)                               
#define PMU_CR_CSBF_Msk                     (0x01U << PMU_CR_CSBF_Pos)          /*!< 0x00000008 */
#define PMU_CR_CSBF                         PMU_CR_CSBF_Msk                    /*!< Clear Standby Flag */
#define PMU_CR_DBP_Pos                    (8U)
#define PMU_CR_DBP_Msk                    (0x01U << PMU_CR_DBP_Pos)        /*!<  */
#define PMU_CR_DBP                        PMU_CR_DBP_Msk                   /*!<  */
 
#define PMU_CR_ALL_Msk                    (PMU_CR_DBP_Msk | PMU_CR_CSBF_Msk | PMU_CR_CWUF_Msk | PMU_CR_PDDS_Msk | PMU_CR_LPDS_Msk)
 
/******************  Bit definition for PMU_CSR register ********************/
#define PMU_CSR_SBF_Pos                    (0U)
#define PMU_CSR_SBF_Msk                    (0x01U << PMU_CSR_SBF_Pos)        /*!<  */
#define PMU_CSR_SBF                        PMU_CSR_SBF_Msk                   /*!<  */
 
#define PMU_CSR_WUF_PIN_Pos                    (1U)
#define PMU_CSR_WUF_PIN_Msk                    (0x01U << PMU_CSR_WUF_PIN_Pos)        /*!<  */
#define PMU_CSR_WUF_PIN                        PMU_CSR_WUF_PIN_Msk                   /*!<  */
 
#define PMU_CSR_WUF_RTC_Pos                    (2U)
#define PMU_CSR_WUF_RTC_Msk                    (0x01U << PMU_CSR_WUF_RTC_Pos)        /*!<  */
#define PMU_CSR_WUF_RTC                        PMU_CSR_WUF_RTC_Msk                   /*!<  */
 
#define PMU_CSR_WUF_IWDG_Pos                    (3U)
#define PMU_CSR_WUF_IWDG_Msk                    (0x01U << PMU_CSR_WUF_IWDG_Pos)        /*!<  */
#define PMU_CSR_WUF_IWDG                        PMU_CSR_WUF_IWDG_Msk                   /*!<  */
 
#define PMU_CSR_WUF_EXTI_Pos                    (4U)
#define PMU_CSR_WUF_EXTI_Msk                    (0x01U << PMU_CSR_WUF_EXTI_Pos)        /*!<  */
#define PMU_CSR_WUF_EXTI                        PMU_CSR_WUF_EXTI_Msk                   /*!<  */
 
#define PMU_CSR_WUF_RSTP_Pos                    (5U)
#define PMU_CSR_WUF_RSTP_Msk                    (0x01U << PMU_CSR_WUF_RSTP_Pos)        /*!<  */
#define PMU_CSR_WUF_RSTP                        PMU_CSR_WUF_RSTP_Msk                   /*!<  */
 
#define PMU_CSR_EWUP1_Pos                    (8U)
#define PMU_CSR_EWUP1_Msk                    (0x01U << PMU_CSR_EWUP1_Pos)        /*!<  */
#define PMU_CSR_EWUP1                        PMU_CSR_EWUP1_Msk                   /*!<  */
 
#define PMU_CSR_EWUP2_Pos                    (9U)
#define PMU_CSR_EWUP2_Msk                    (0x01U << PMU_CSR_EWUP2_Pos)        /*!<  */
#define PMU_CSR_EWUP2                        PMU_CSR_EWUP2_Msk                   /*!<  */
 
#define PMU_CSR_EWUP4_Pos                    (11U)
#define PMU_CSR_EWUP4_Msk                    (0x01U << PMU_CSR_EWUP4_Pos)        /*!<  */
#define PMU_CSR_EWUP4                        PMU_CSR_EWUP4_Msk                   /*!<  */
 
#define PMU_CSR_EWUP7_Pos                    (14U)
#define PMU_CSR_EWUP7_Msk                    (0x01U << PMU_CSR_EWUP7_Pos)        /*!<  */
#define PMU_CSR_EWUP7                        PMU_CSR_EWUP7_Msk                   /*!<  */
 
#define PMU_CSR_WUF_PIN1_Pos                    (16U)
#define PMU_CSR_WUF_PIN1_Msk                    (0x01U << PMU_CSR_WUF_PIN1_Pos)        /*!<  */
#define PMU_CSR_WUF_PIN1                        PMU_CSR_WUF_PIN1_Msk                   /*!<  */
 
#define PMU_CSR_WUF_PIN2_Pos                    (17U)
#define PMU_CSR_WUF_PIN2_Msk                    (0x01U << PMU_CSR_WUF_PIN2_Pos)        /*!<  */
#define PMU_CSR_WUF_PIN2                        PMU_CSR_WUF_PIN2_Msk                   /*!<  */
 
#define PMU_CSR_WUF_PIN4_Pos                    (19U)
#define PMU_CSR_WUF_PIN4_Msk                    (0x01U << PMU_CSR_WUF_PIN4_Pos)        /*!<  */
#define PMU_CSR_WUF_PIN4                        PMU_CSR_WUF_PIN4_Msk                   /*!<  */
 
#define PMU_CSR_WUF_PIN7_Pos                    (22U)
#define PMU_CSR_WUF_PIN7_Msk                    (0x01U << PMU_CSR_WUF_PIN7_Pos)        /*!<  */
#define PMU_CSR_WUF_PIN7                        PMU_CSR_WUF_PIN7_Msk                   /*!<  */
 
#define PMU_CSR_ALL_Msk                    (PMU_CSR_WUF_PIN7_Msk | PMU_CSR_WUF_PIN4_Msk | PMU_CSR_WUF_PIN2_Msk | PMU_CSR_WUF_PIN1_Msk | PMU_CSR_EWUP7_Msk | PMU_CSR_EWUP4_Msk | PMU_CSR_EWUP2_Msk | PMU_CSR_EWUP1_Msk | PMU_CSR_WUF_RSTP_Msk | PMU_CSR_WUF_EXTI_Msk | PMU_CSR_WUF_IWDG_Msk | PMU_CSR_WUF_RTC_Msk | PMU_CSR_WUF_PIN_Msk | PMU_CSR_SBF_Msk)
 
/******************  Bit definition for RCMU_CR register ********************/
#define RCMU_CR_HSION_Pos                    (0U)
#define RCMU_CR_HSION_Msk                    (0x01U << RCMU_CR_HSION_Pos)        /*!<  */
#define RCMU_CR_HSION                        RCMU_CR_HSION_Msk                   /*!<  */
 
#define RCMU_CR_HSIRDY_Pos                    (1U)
#define RCMU_CR_HSIRDY_Msk                    (0x01U << RCMU_CR_HSIRDY_Pos)        /*!<  */
#define RCMU_CR_HSIRDY                        RCMU_CR_HSIRDY_Msk                   /*!<  */
 
#define RCMU_CR_HSI_FREQ_SEL_Pos                    (4U)
#define RCMU_CR_HSI_FREQ_SEL_Msk                    (0x7U << RCMU_CR_HSI_FREQ_SEL_Pos)        /*!<  */
#define RCMU_CR_HSI_FREQ_SEL                        RCMU_CR_HSI_FREQ_SEL_Msk                   /*!<  */
 
#define RCMU_CR_HSE_STAB_RST_Pos                    (14U)
#define RCMU_CR_HSE_STAB_RST_Msk                    (0x01U << RCMU_CR_HSE_STAB_RST_Pos)        /*!<  */
#define RCMU_CR_HSE_STAB_RST                        RCMU_CR_HSE_STAB_RST_Msk                   /*!<  */
 
#define RCMU_CR_HSE_EXT_Pos                    (15U)
#define RCMU_CR_HSE_EXT_Msk                    (0x01U << RCMU_CR_HSE_EXT_Pos)        /*!<  */
#define RCMU_CR_HSE_EXT                        RCMU_CR_HSE_EXT_Msk                   /*!<  */
 
#define RCMU_CR_HSEON_Pos                    (16U)
#define RCMU_CR_HSEON_Msk                    (0x01U << RCMU_CR_HSEON_Pos)        /*!<  */
#define RCMU_CR_HSEON                        RCMU_CR_HSEON_Msk                   /*!<  */
 
#define RCMU_CR_HSERDY_Pos                    (17U)
#define RCMU_CR_HSERDY_Msk                    (0x01U << RCMU_CR_HSERDY_Pos)        /*!<  */
#define RCMU_CR_HSERDY                        RCMU_CR_HSERDY_Msk                   /*!<  */
 
#define RCMU_CR_HSEBYP_Pos                    (18U)
#define RCMU_CR_HSEBYP_Msk                    (0x01U << RCMU_CR_HSEBYP_Pos)        /*!<  */
#define RCMU_CR_HSEBYP                        RCMU_CR_HSEBYP_Msk                   /*!<  */
 
#define RCMU_CR_HSE_DL_Pos                    (20U)
#define RCMU_CR_HSE_DL_Msk                    (0x7U << RCMU_CR_HSE_DL_Pos)        /*!<  */
#define RCMU_CR_HSE_DL                        RCMU_CR_HSE_DL_Msk                   /*!<  */
 
#define RCMU_CR_HSE_FORCE_OK_Pos                    (23U)
#define RCMU_CR_HSE_FORCE_OK_Msk                    (0x01U << RCMU_CR_HSE_FORCE_OK_Pos)        /*!<  */
#define RCMU_CR_HSE_FORCE_OK                        RCMU_CR_HSE_FORCE_OK_Msk                   /*!<  */
 
#define RCMU_CR_PLLON_Pos                    (24U)
#define RCMU_CR_PLLON_Msk                    (0x01U << RCMU_CR_PLLON_Pos)        /*!<  */
#define RCMU_CR_PLLON                        RCMU_CR_PLLON_Msk                   /*!<  */
 
#define RCMU_CR_PLLRDY_Pos                    (25U)
#define RCMU_CR_PLLRDY_Msk                    (0x01U << RCMU_CR_PLLRDY_Pos)        /*!<  */
#define RCMU_CR_PLLRDY                        RCMU_CR_PLLRDY_Msk                   /*!<  */
 
#define RCMU_CR_HSE_STARTUP_Pos                    (28U)
#define RCMU_CR_HSE_STARTUP_Msk                    (0x7U << RCMU_CR_HSE_STARTUP_Pos)        /*!<  */
#define RCMU_CR_HSE_STARTUP                        RCMU_CR_HSE_STARTUP_Msk                   /*!<  */
 
#define RCMU_CR_ALL_Msk                    (RCMU_CR_HSE_STARTUP_Msk | RCMU_CR_PLLRDY_Msk | RCMU_CR_PLLON_Msk | RCMU_CR_HSE_FORCE_OK_Msk | RCMU_CR_HSE_DL_Msk | RCMU_CR_HSEBYP_Msk | RCMU_CR_HSERDY_Msk | RCMU_CR_HSEON_Msk | RCMU_CR_HSE_EXT_Msk | RCMU_CR_HSE_STAB_RST_Msk | RCMU_CR_HSI_FREQ_SEL_Msk | RCMU_CR_HSIRDY_Msk | RCMU_CR_HSION_Msk)
 
/******************  Bit definition for RCMU_CFGR register ********************/
#define RCMU_CFGR_SW_Pos                    (0U)
#define RCMU_CFGR_SW_Msk                    (0x3U << RCMU_CFGR_SW_Pos)        /*!<  */
#define RCMU_CFGR_SW                        RCMU_CFGR_SW_Msk                   /*!<  */
 
#define RCMU_CFGR_SWS_Pos                    (2U)
#define RCMU_CFGR_SWS_Msk                    (0x3U << RCMU_CFGR_SWS_Pos)        /*!<  */
#define RCMU_CFGR_SWS                        RCMU_CFGR_SWS_Msk                   /*!<  */
 
#define RCMU_CFGR_HPRE_Pos                    (4U)
#define RCMU_CFGR_HPRE_Msk                    (0x1FFU << RCMU_CFGR_HPRE_Pos)        /*!<  */
#define RCMU_CFGR_HPRE                        RCMU_CFGR_HPRE_Msk                   /*!<  */
 
#define RCMU_CFGR_SLOW_SEL_Pos                    (14U)
#define RCMU_CFGR_SLOW_SEL_Msk                    (0x01U << RCMU_CFGR_SLOW_SEL_Pos)        /*!<  */
#define RCMU_CFGR_SLOW_SEL                        RCMU_CFGR_SLOW_SEL_Msk                   /*!<  */
 
#define RCMU_CFGR_PPRE1_Pos                    (16U)
#define RCMU_CFGR_PPRE1_Msk                    (0xFU << RCMU_CFGR_PPRE1_Pos)        /*!<  */
#define RCMU_CFGR_PPRE1                        RCMU_CFGR_PPRE1_Msk                   /*!<  */
 
#define RCMU_CFGR_PPRE2_Pos                    (20U)
#define RCMU_CFGR_PPRE2_Msk                    (0xFU << RCMU_CFGR_PPRE2_Pos)        /*!<  */
#define RCMU_CFGR_PPRE2                        RCMU_CFGR_PPRE2_Msk                   /*!<  */
 
#define RCMU_CFGR_MCO_Pos                    (24U)
#define RCMU_CFGR_MCO_Msk                    (0x7U << RCMU_CFGR_MCO_Pos)        /*!<  */
#define RCMU_CFGR_MCO                        RCMU_CFGR_MCO_Msk                   /*!<  */
 
#define RCMU_CFGR_MCOPRE_Pos                    (28U)
#define RCMU_CFGR_MCOPRE_Msk                    (0x7U << RCMU_CFGR_MCOPRE_Pos)        /*!<  */
#define RCMU_CFGR_MCOPRE                        RCMU_CFGR_MCOPRE_Msk                   /*!<  */
 
#define RCMU_CFGR_ALL_Msk                    (RCMU_CFGR_MCOPRE_Msk | RCMU_CFGR_MCO_Msk | RCMU_CFGR_PPRE2_Msk | RCMU_CFGR_PPRE1_Msk | RCMU_CFGR_SLOW_SEL_Msk | RCMU_CFGR_HPRE_Msk | RCMU_CFGR_SWS_Msk | RCMU_CFGR_SW_Msk)
 
/******************  Bit definition for RCMU_CIR register ********************/
#define RCMU_CIR_LSIRDYF_Pos                    (0U)
#define RCMU_CIR_LSIRDYF_Msk                    (0x01U << RCMU_CIR_LSIRDYF_Pos)        /*!<  */
#define RCMU_CIR_LSIRDYF                        RCMU_CIR_LSIRDYF_Msk                   /*!<  */
 
#define RCMU_CIR_LSERDYF_Pos                    (1U)
#define RCMU_CIR_LSERDYF_Msk                    (0x01U << RCMU_CIR_LSERDYF_Pos)        /*!<  */
#define RCMU_CIR_LSERDYF                        RCMU_CIR_LSERDYF_Msk                   /*!<  */
 
#define RCMU_CIR_HSIRDYF_Pos                    (2U)
#define RCMU_CIR_HSIRDYF_Msk                    (0x01U << RCMU_CIR_HSIRDYF_Pos)        /*!<  */
#define RCMU_CIR_HSIRDYF                        RCMU_CIR_HSIRDYF_Msk                   /*!<  */
 
#define RCMU_CIR_HSERDYF_Pos                    (3U)
#define RCMU_CIR_HSERDYF_Msk                    (0x01U << RCMU_CIR_HSERDYF_Pos)        /*!<  */
#define RCMU_CIR_HSERDYF                        RCMU_CIR_HSERDYF_Msk                   /*!<  */
 
#define RCMU_CIR_PLLRDYF_Pos                    (4U)
#define RCMU_CIR_PLLRDYF_Msk                    (0x01U << RCMU_CIR_PLLRDYF_Pos)        /*!<  */
#define RCMU_CIR_PLLRDYF                        RCMU_CIR_PLLRDYF_Msk                   /*!<  */
 
#define RCMU_CIR_CSSF_Pos                    (7U)
#define RCMU_CIR_CSSF_Msk                    (0x01U << RCMU_CIR_CSSF_Pos)        /*!<  */
#define RCMU_CIR_CSSF                        RCMU_CIR_CSSF_Msk                   /*!<  */
 
#define RCMU_CIR_LSIRDYIE_Pos                    (8U)
#define RCMU_CIR_LSIRDYIE_Msk                    (0x01U << RCMU_CIR_LSIRDYIE_Pos)        /*!<  */
#define RCMU_CIR_LSIRDYIE                        RCMU_CIR_LSIRDYIE_Msk                   /*!<  */
 
#define RCMU_CIR_LSERDYIE_Pos                    (9U)
#define RCMU_CIR_LSERDYIE_Msk                    (0x01U << RCMU_CIR_LSERDYIE_Pos)        /*!<  */
#define RCMU_CIR_LSERDYIE                        RCMU_CIR_LSERDYIE_Msk                   /*!<  */
 
#define RCMU_CIR_HSIRDYIE_Pos                    (10U)
#define RCMU_CIR_HSIRDYIE_Msk                    (0x01U << RCMU_CIR_HSIRDYIE_Pos)        /*!<  */
#define RCMU_CIR_HSIRDYIE                        RCMU_CIR_HSIRDYIE_Msk                   /*!<  */
 
#define RCMU_CIR_HSERDYIE_Pos                    (11U)
#define RCMU_CIR_HSERDYIE_Msk                    (0x01U << RCMU_CIR_HSERDYIE_Pos)        /*!<  */
#define RCMU_CIR_HSERDYIE                        RCMU_CIR_HSERDYIE_Msk                   /*!<  */
 
#define RCMU_CIR_PLLRDYIE_Pos                    (12U)
#define RCMU_CIR_PLLRDYIE_Msk                    (0x01U << RCMU_CIR_PLLRDYIE_Pos)        /*!<  */
#define RCMU_CIR_PLLRDYIE                        RCMU_CIR_PLLRDYIE_Msk                   /*!<  */
 
#define RCMU_CIR_LSIRDYC_Pos                    (16U)
#define RCMU_CIR_LSIRDYC_Msk                    (0x01U << RCMU_CIR_LSIRDYC_Pos)        /*!<  */
#define RCMU_CIR_LSIRDYC                        RCMU_CIR_LSIRDYC_Msk                   /*!<  */
 
#define RCMU_CIR_LSERDYC_Pos                    (17U)
#define RCMU_CIR_LSERDYC_Msk                    (0x01U << RCMU_CIR_LSERDYC_Pos)        /*!<  */
#define RCMU_CIR_LSERDYC                        RCMU_CIR_LSERDYC_Msk                   /*!<  */
 
#define RCMU_CIR_HSIRDYC_Pos                    (18U)
#define RCMU_CIR_HSIRDYC_Msk                    (0x01U << RCMU_CIR_HSIRDYC_Pos)        /*!<  */
#define RCMU_CIR_HSIRDYC                        RCMU_CIR_HSIRDYC_Msk                   /*!<  */
 
#define RCMU_CIR_HSERDYC_Pos                    (19U)
#define RCMU_CIR_HSERDYC_Msk                    (0x01U << RCMU_CIR_HSERDYC_Pos)        /*!<  */
#define RCMU_CIR_HSERDYC                        RCMU_CIR_HSERDYC_Msk                   /*!<  */
 
#define RCMU_CIR_PLLRDYC_Pos                    (20U)
#define RCMU_CIR_PLLRDYC_Msk                    (0x01U << RCMU_CIR_PLLRDYC_Pos)        /*!<  */
#define RCMU_CIR_PLLRDYC                        RCMU_CIR_PLLRDYC_Msk                   /*!<  */
 
#define RCMU_CIR_CSSC_Pos                    (23U)
#define RCMU_CIR_CSSC_Msk                    (0x01U << RCMU_CIR_CSSC_Pos)        /*!<  */
#define RCMU_CIR_CSSC                        RCMU_CIR_CSSC_Msk                   /*!<  */
 
#define RCMU_CIR_ALL_Msk                    (RCMU_CIR_CSSC_Msk | RCMU_CIR_PLLRDYC_Msk | RCMU_CIR_HSERDYC_Msk | RCMU_CIR_HSIRDYC_Msk | RCMU_CIR_LSERDYC_Msk | RCMU_CIR_LSIRDYC_Msk | RCMU_CIR_PLLRDYIE_Msk | RCMU_CIR_HSERDYIE_Msk | RCMU_CIR_HSIRDYIE_Msk | RCMU_CIR_LSERDYIE_Msk | RCMU_CIR_LSIRDYIE_Msk | RCMU_CIR_CSSF_Msk | RCMU_CIR_PLLRDYF_Msk | RCMU_CIR_HSERDYF_Msk | RCMU_CIR_HSIRDYF_Msk | RCMU_CIR_LSERDYF_Msk | RCMU_CIR_LSIRDYF_Msk)
 
/******************  Bit definition for RCMU_APB2RSTR register ********************/
#define RCMU_APB2RSTR_SYSCFG_RST_Pos                    (0U)
#define RCMU_APB2RSTR_SYSCFG_RST_Msk                    (0x01U << RCMU_APB2RSTR_SYSCFG_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_SYSCFG_RST                        RCMU_APB2RSTR_SYSCFG_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_IEMU_RST_Pos                    (1U)
#define RCMU_APB2RSTR_IEMU_RST_Msk                    (0x01U << RCMU_APB2RSTR_IEMU_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_IEMU_RST                        RCMU_APB2RSTR_IEMU_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_ADC_RST_Pos                    (9U)
#define RCMU_APB2RSTR_ADC_RST_Msk                    (0x01U << RCMU_APB2RSTR_ADC_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_ADC_RST                        RCMU_APB2RSTR_ADC_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_AWKTIM_RST_Pos                    (10U)
#define RCMU_APB2RSTR_AWKTIM_RST_Msk                    (0x01U << RCMU_APB2RSTR_AWKTIM_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_AWKTIM_RST                        RCMU_APB2RSTR_AWKTIM_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_TIM1RST_Pos                    (11U)
#define RCMU_APB2RSTR_TIM1RST_Msk                    (0x01U << RCMU_APB2RSTR_TIM1RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_TIM1RST                        RCMU_APB2RSTR_TIM1RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_SPI1RST_Pos                    (12U)
#define RCMU_APB2RSTR_SPI1RST_Msk                    (0x01U << RCMU_APB2RSTR_SPI1RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_SPI1RST                        RCMU_APB2RSTR_SPI1RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_USART1_RST_Pos                    (14U)
#define RCMU_APB2RSTR_USART1_RST_Msk                    (0x01U << RCMU_APB2RSTR_USART1_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_USART1_RST                        RCMU_APB2RSTR_USART1_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_LPTIM_RST_Pos                    (15U)
#define RCMU_APB2RSTR_LPTIM_RST_Msk                    (0x01U << RCMU_APB2RSTR_LPTIM_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_LPTIM_RST                        RCMU_APB2RSTR_LPTIM_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_TIM15_RST_Pos                    (16U)
#define RCMU_APB2RSTR_TIM15_RST_Msk                    (0x01U << RCMU_APB2RSTR_TIM15_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_TIM15_RST                        RCMU_APB2RSTR_TIM15_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_TIM16_RST_Pos                    (17U)
#define RCMU_APB2RSTR_TIM16_RST_Msk                    (0x01U << RCMU_APB2RSTR_TIM16_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_TIM16_RST                        RCMU_APB2RSTR_TIM16_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_TIM17_RST_Pos                    (18U)
#define RCMU_APB2RSTR_TIM17_RST_Msk                    (0x01U << RCMU_APB2RSTR_TIM17_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_TIM17_RST                        RCMU_APB2RSTR_TIM17_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_LVD_RST_Pos                    (20U)
#define RCMU_APB2RSTR_LVD_RST_Msk                    (0x01U << RCMU_APB2RSTR_LVD_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_LVD_RST                        RCMU_APB2RSTR_LVD_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_BEEP_RST_Pos                    (21U)
#define RCMU_APB2RSTR_BEEP_RST_Msk                    (0x01U << RCMU_APB2RSTR_BEEP_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_BEEP_RST                        RCMU_APB2RSTR_BEEP_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_DBGMCU_RST_Pos                    (22U)
#define RCMU_APB2RSTR_DBGMCU_RST_Msk                    (0x01U << RCMU_APB2RSTR_DBGMCU_RST_Pos)        /*!<  */
#define RCMU_APB2RSTR_DBGMCU_RST                        RCMU_APB2RSTR_DBGMCU_RST_Msk                   /*!<  */
 
#define RCMU_APB2RSTR_ALL_Msk                    (RCMU_APB2RSTR_DBGMCU_RST_Msk | RCMU_APB2RSTR_BEEP_RST_Msk | RCMU_APB2RSTR_LVD_RST_Msk | RCMU_APB2RSTR_TIM17_RST_Msk | RCMU_APB2RSTR_TIM16_RST_Msk | RCMU_APB2RSTR_TIM15_RST_Msk | RCMU_APB2RSTR_LPTIM_RST_Msk | RCMU_APB2RSTR_USART1_RST_Msk | RCMU_APB2RSTR_SPI1RST_Msk | RCMU_APB2RSTR_TIM1RST_Msk | RCMU_APB2RSTR_AWKTIM_RST_Msk | RCMU_APB2RSTR_ADC_RST_Msk | RCMU_APB2RSTR_IEMU_RST_Msk | RCMU_APB2RSTR_SYSCFG_RST_Msk)
 
/******************  Bit definition for RCMU_APB1RSTR register ********************/
#define RCMU_APB1RSTR_TIM3_RST_Pos                    (1U)
#define RCMU_APB1RSTR_TIM3_RST_Msk                    (0x01U << RCMU_APB1RSTR_TIM3_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_TIM3_RST                        RCMU_APB1RSTR_TIM3_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_TIM6_RST_Pos                    (4U)
#define RCMU_APB1RSTR_TIM6_RST_Msk                    (0x01U << RCMU_APB1RSTR_TIM6_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_TIM6_RST                        RCMU_APB1RSTR_TIM6_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_TIM7_RST_Pos                    (5U)
#define RCMU_APB1RSTR_TIM7_RST_Msk                    (0x01U << RCMU_APB1RSTR_TIM7_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_TIM7_RST                        RCMU_APB1RSTR_TIM7_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_TIM14_RST_Pos                    (8U)
#define RCMU_APB1RSTR_TIM14_RST_Msk                    (0x01U << RCMU_APB1RSTR_TIM14_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_TIM14_RST                        RCMU_APB1RSTR_TIM14_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_RTC_RST_Pos                    (10U)
#define RCMU_APB1RSTR_RTC_RST_Msk                    (0x01U << RCMU_APB1RSTR_RTC_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_RTC_RST                        RCMU_APB1RSTR_RTC_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_WWDGRST_Pos                    (11U)
#define RCMU_APB1RSTR_WWDGRST_Msk                    (0x01U << RCMU_APB1RSTR_WWDGRST_Pos)        /*!<  */
#define RCMU_APB1RSTR_WWDGRST                        RCMU_APB1RSTR_WWDGRST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_IWDGRST_Pos                    (12U)
#define RCMU_APB1RSTR_IWDGRST_Msk                    (0x01U << RCMU_APB1RSTR_IWDGRST_Pos)        /*!<  */
#define RCMU_APB1RSTR_IWDGRST                        RCMU_APB1RSTR_IWDGRST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_SPI2RST_Pos                    (14U)
#define RCMU_APB1RSTR_SPI2RST_Msk                    (0x01U << RCMU_APB1RSTR_SPI2RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_SPI2RST                        RCMU_APB1RSTR_SPI2RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_USART2_RST_Pos                    (17U)
#define RCMU_APB1RSTR_USART2_RST_Msk                    (0x01U << RCMU_APB1RSTR_USART2_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_USART2_RST                        RCMU_APB1RSTR_USART2_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_LPUART_RST_Pos                    (20U)
#define RCMU_APB1RSTR_LPUART_RST_Msk                    (0x01U << RCMU_APB1RSTR_LPUART_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_LPUART_RST                        RCMU_APB1RSTR_LPUART_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_I2C1_RST_Pos                    (21U)
#define RCMU_APB1RSTR_I2C1_RST_Msk                    (0x01U << RCMU_APB1RSTR_I2C1_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_I2C1_RST                        RCMU_APB1RSTR_I2C1_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_I2C2_RST_Pos                    (22U)
#define RCMU_APB1RSTR_I2C2_RST_Msk                    (0x01U << RCMU_APB1RSTR_I2C2_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_I2C2_RST                        RCMU_APB1RSTR_I2C2_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_VC_RST_Pos                    (23U)
#define RCMU_APB1RSTR_VC_RST_Msk                    (0x01U << RCMU_APB1RSTR_VC_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_VC_RST                        RCMU_APB1RSTR_VC_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_PMU_RST_Pos                    (28U)
#define RCMU_APB1RSTR_PMU_RST_Msk                    (0x01U << RCMU_APB1RSTR_PMU_RST_Pos)        /*!<  */
#define RCMU_APB1RSTR_PMU_RST                        RCMU_APB1RSTR_PMU_RST_Msk                   /*!<  */
 
#define RCMU_APB1RSTR_ALL_Msk                    (RCMU_APB1RSTR_PMU_RST_Msk | RCMU_APB1RSTR_VC_RST_Msk | RCMU_APB1RSTR_I2C2_RST_Msk | RCMU_APB1RSTR_I2C1_RST_Msk | RCMU_APB1RSTR_LPUART_RST_Msk | RCMU_APB1RSTR_USART2_RST_Msk | RCMU_APB1RSTR_SPI2RST_Msk | RCMU_APB1RSTR_IWDGRST_Msk | RCMU_APB1RSTR_WWDGRST_Msk | RCMU_APB1RSTR_RTC_RST_Msk | RCMU_APB1RSTR_TIM14_RST_Msk | RCMU_APB1RSTR_TIM7_RST_Msk | RCMU_APB1RSTR_TIM6_RST_Msk | RCMU_APB1RSTR_TIM3_RST_Msk)
 
/******************  Bit definition for RCMU_AHBENR register ********************/
#define RCMU_AHBENR_DMAEN_Pos                    (0U)
#define RCMU_AHBENR_DMAEN_Msk                    (0x01U << RCMU_AHBENR_DMAEN_Pos)        /*!<  */
#define RCMU_AHBENR_DMAEN                        RCMU_AHBENR_DMAEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_SRAMEN_Pos                    (2U)
#define RCMU_AHBENR_SRAMEN_Msk                    (0x01U << RCMU_AHBENR_SRAMEN_Pos)        /*!<  */
#define RCMU_AHBENR_SRAMEN                        RCMU_AHBENR_SRAMEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_FLITFEN_Pos                    (4U)
#define RCMU_AHBENR_FLITFEN_Msk                    (0x01U << RCMU_AHBENR_FLITFEN_Pos)        /*!<  */
#define RCMU_AHBENR_FLITFEN                        RCMU_AHBENR_FLITFEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_TRNGEN_Pos                    (5U)
#define RCMU_AHBENR_TRNGEN_Msk                    (0x01U << RCMU_AHBENR_TRNGEN_Pos)        /*!<  */
#define RCMU_AHBENR_TRNGEN                        RCMU_AHBENR_TRNGEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_CRCEN_Pos                    (6U)
#define RCMU_AHBENR_CRCEN_Msk                    (0x01U << RCMU_AHBENR_CRCEN_Pos)        /*!<  */
#define RCMU_AHBENR_CRCEN                        RCMU_AHBENR_CRCEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_AESEN_Pos                    (7U)
#define RCMU_AHBENR_AESEN_Msk                    (0x01U << RCMU_AHBENR_AESEN_Pos)        /*!<  */
#define RCMU_AHBENR_AESEN                        RCMU_AHBENR_AESEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_IOAEN_Pos                    (16U)
#define RCMU_AHBENR_IOAEN_Msk                    (0x01U << RCMU_AHBENR_IOAEN_Pos)        /*!<  */
#define RCMU_AHBENR_IOAEN                        RCMU_AHBENR_IOAEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_IOBEN_Pos                    (17U)
#define RCMU_AHBENR_IOBEN_Msk                    (0x01U << RCMU_AHBENR_IOBEN_Pos)        /*!<  */
#define RCMU_AHBENR_IOBEN                        RCMU_AHBENR_IOBEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_IOCEN_Pos                    (18U)
#define RCMU_AHBENR_IOCEN_Msk                    (0x01U << RCMU_AHBENR_IOCEN_Pos)        /*!<  */
#define RCMU_AHBENR_IOCEN                        RCMU_AHBENR_IOCEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_IODEN_Pos                    (19U)
#define RCMU_AHBENR_IODEN_Msk                    (0x01U << RCMU_AHBENR_IODEN_Pos)        /*!<  */
#define RCMU_AHBENR_IODEN                        RCMU_AHBENR_IODEN_Msk                   /*!<  */
 
#define RCMU_AHBENR_ALL_Msk                    (RCMU_AHBENR_IODEN_Msk | RCMU_AHBENR_IOCEN_Msk | RCMU_AHBENR_IOBEN_Msk | RCMU_AHBENR_IOAEN_Msk | RCMU_AHBENR_AESEN_Msk | RCMU_AHBENR_CRCEN_Msk | RCMU_AHBENR_TRNGEN_Msk | RCMU_AHBENR_FLITFEN_Msk | RCMU_AHBENR_SRAMEN_Msk | RCMU_AHBENR_DMAEN_Msk)
 
/******************  Bit definition for RCMU_APB2ENR register ********************/
#define RCMU_APB2ENR_SYSCFGEN_Pos                    (0U)
#define RCMU_APB2ENR_SYSCFGEN_Msk                    (0x01U << RCMU_APB2ENR_SYSCFGEN_Pos)        /*!<  */
#define RCMU_APB2ENR_SYSCFGEN                        RCMU_APB2ENR_SYSCFGEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_IEMUEN_Pos                    (1U)
#define RCMU_APB2ENR_IEMUEN_Msk                    (0x01U << RCMU_APB2ENR_IEMUEN_Pos)        /*!<  */
#define RCMU_APB2ENR_IEMUEN                        RCMU_APB2ENR_IEMUEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_ADCEN_Pos                    (9U)
#define RCMU_APB2ENR_ADCEN_Msk                    (0x01U << RCMU_APB2ENR_ADCEN_Pos)        /*!<  */
#define RCMU_APB2ENR_ADCEN                        RCMU_APB2ENR_ADCEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_AWKTIMEN_Pos                    (10U)
#define RCMU_APB2ENR_AWKTIMEN_Msk                    (0x01U << RCMU_APB2ENR_AWKTIMEN_Pos)        /*!<  */
#define RCMU_APB2ENR_AWKTIMEN                        RCMU_APB2ENR_AWKTIMEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_TIM1EN_Pos                    (11U)
#define RCMU_APB2ENR_TIM1EN_Msk                    (0x01U << RCMU_APB2ENR_TIM1EN_Pos)        /*!<  */
#define RCMU_APB2ENR_TIM1EN                        RCMU_APB2ENR_TIM1EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_SPI1EN_Pos                    (12U)
#define RCMU_APB2ENR_SPI1EN_Msk                    (0x01U << RCMU_APB2ENR_SPI1EN_Pos)        /*!<  */
#define RCMU_APB2ENR_SPI1EN                        RCMU_APB2ENR_SPI1EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_USART1EN_Pos                    (14U)
#define RCMU_APB2ENR_USART1EN_Msk                    (0x01U << RCMU_APB2ENR_USART1EN_Pos)        /*!<  */
#define RCMU_APB2ENR_USART1EN                        RCMU_APB2ENR_USART1EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_LPTIMEN_Pos                    (15U)
#define RCMU_APB2ENR_LPTIMEN_Msk                    (0x01U << RCMU_APB2ENR_LPTIMEN_Pos)        /*!<  */
#define RCMU_APB2ENR_LPTIMEN                        RCMU_APB2ENR_LPTIMEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_TIM15EN_Pos                    (16U)
#define RCMU_APB2ENR_TIM15EN_Msk                    (0x01U << RCMU_APB2ENR_TIM15EN_Pos)        /*!<  */
#define RCMU_APB2ENR_TIM15EN                        RCMU_APB2ENR_TIM15EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_TIM16EN_Pos                    (17U)
#define RCMU_APB2ENR_TIM16EN_Msk                    (0x01U << RCMU_APB2ENR_TIM16EN_Pos)        /*!<  */
#define RCMU_APB2ENR_TIM16EN                        RCMU_APB2ENR_TIM16EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_TIM17EN_Pos                    (18U)
#define RCMU_APB2ENR_TIM17EN_Msk                    (0x01U << RCMU_APB2ENR_TIM17EN_Pos)        /*!<  */
#define RCMU_APB2ENR_TIM17EN                        RCMU_APB2ENR_TIM17EN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_LVDEN_Pos                    (20U)
#define RCMU_APB2ENR_LVDEN_Msk                    (0x01U << RCMU_APB2ENR_LVDEN_Pos)        /*!<  */
#define RCMU_APB2ENR_LVDEN                        RCMU_APB2ENR_LVDEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_BEEPEN_Pos                    (21U)
#define RCMU_APB2ENR_BEEPEN_Msk                    (0x01U << RCMU_APB2ENR_BEEPEN_Pos)        /*!<  */
#define RCMU_APB2ENR_BEEPEN                        RCMU_APB2ENR_BEEPEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_DBGMCUEN_Pos                    (22U)
#define RCMU_APB2ENR_DBGMCUEN_Msk                    (0x01U << RCMU_APB2ENR_DBGMCUEN_Pos)        /*!<  */
#define RCMU_APB2ENR_DBGMCUEN                        RCMU_APB2ENR_DBGMCUEN_Msk                   /*!<  */
 
#define RCMU_APB2ENR_ALL_Msk                    (RCMU_APB2ENR_DBGMCUEN_Msk | RCMU_APB2ENR_BEEPEN_Msk | RCMU_APB2ENR_LVDEN_Msk | RCMU_APB2ENR_TIM17EN_Msk | RCMU_APB2ENR_TIM16EN_Msk | RCMU_APB2ENR_TIM15EN_Msk | RCMU_APB2ENR_LPTIMEN_Msk | RCMU_APB2ENR_USART1EN_Msk | RCMU_APB2ENR_SPI1EN_Msk | RCMU_APB2ENR_TIM1EN_Msk | RCMU_APB2ENR_AWKTIMEN_Msk | RCMU_APB2ENR_ADCEN_Msk | RCMU_APB2ENR_IEMUEN_Msk | RCMU_APB2ENR_SYSCFGEN_Msk)
 
/******************  Bit definition for RCMU_APB1ENR register ********************/
#define RCMU_APB1ENR_TIM3EN_Pos                    (1U)
#define RCMU_APB1ENR_TIM3EN_Msk                    (0x01U << RCMU_APB1ENR_TIM3EN_Pos)        /*!<  */
#define RCMU_APB1ENR_TIM3EN                        RCMU_APB1ENR_TIM3EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_TIM6EN_Pos                    (4U)
#define RCMU_APB1ENR_TIM6EN_Msk                    (0x01U << RCMU_APB1ENR_TIM6EN_Pos)        /*!<  */
#define RCMU_APB1ENR_TIM6EN                        RCMU_APB1ENR_TIM6EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_TIM7EN_Pos                    (5U)
#define RCMU_APB1ENR_TIM7EN_Msk                    (0x01U << RCMU_APB1ENR_TIM7EN_Pos)        /*!<  */
#define RCMU_APB1ENR_TIM7EN                        RCMU_APB1ENR_TIM7EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_TIM14EN_Pos                    (8U)
#define RCMU_APB1ENR_TIM14EN_Msk                    (0x01U << RCMU_APB1ENR_TIM14EN_Pos)        /*!<  */
#define RCMU_APB1ENR_TIM14EN                        RCMU_APB1ENR_TIM14EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_RTCEN_Pos                    (10U)
#define RCMU_APB1ENR_RTCEN_Msk                    (0x01U << RCMU_APB1ENR_RTCEN_Pos)        /*!<  */
#define RCMU_APB1ENR_RTCEN                        RCMU_APB1ENR_RTCEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_WWDGEN_Pos                    (11U)
#define RCMU_APB1ENR_WWDGEN_Msk                    (0x01U << RCMU_APB1ENR_WWDGEN_Pos)        /*!<  */
#define RCMU_APB1ENR_WWDGEN                        RCMU_APB1ENR_WWDGEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_IWDGEN_Pos                    (12U)
#define RCMU_APB1ENR_IWDGEN_Msk                    (0x01U << RCMU_APB1ENR_IWDGEN_Pos)        /*!<  */
#define RCMU_APB1ENR_IWDGEN                        RCMU_APB1ENR_IWDGEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_SPI2EN_Pos                    (14U)
#define RCMU_APB1ENR_SPI2EN_Msk                    (0x01U << RCMU_APB1ENR_SPI2EN_Pos)        /*!<  */
#define RCMU_APB1ENR_SPI2EN                        RCMU_APB1ENR_SPI2EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_USART2EN_Pos                    (17U)
#define RCMU_APB1ENR_USART2EN_Msk                    (0x01U << RCMU_APB1ENR_USART2EN_Pos)        /*!<  */
#define RCMU_APB1ENR_USART2EN                        RCMU_APB1ENR_USART2EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_LPUARTEN_Pos                    (20U)
#define RCMU_APB1ENR_LPUARTEN_Msk                    (0x01U << RCMU_APB1ENR_LPUARTEN_Pos)        /*!<  */
#define RCMU_APB1ENR_LPUARTEN                        RCMU_APB1ENR_LPUARTEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_I2C1EN_Pos                    (21U)
#define RCMU_APB1ENR_I2C1EN_Msk                    (0x01U << RCMU_APB1ENR_I2C1EN_Pos)        /*!<  */
#define RCMU_APB1ENR_I2C1EN                        RCMU_APB1ENR_I2C1EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_I2C2EN_Pos                    (22U)
#define RCMU_APB1ENR_I2C2EN_Msk                    (0x01U << RCMU_APB1ENR_I2C2EN_Pos)        /*!<  */
#define RCMU_APB1ENR_I2C2EN                        RCMU_APB1ENR_I2C2EN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_VCEN_Pos                    (23U)
#define RCMU_APB1ENR_VCEN_Msk                    (0x01U << RCMU_APB1ENR_VCEN_Pos)        /*!<  */
#define RCMU_APB1ENR_VCEN                        RCMU_APB1ENR_VCEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_PMUEN_Pos                    (28U)
#define RCMU_APB1ENR_PMUEN_Msk                    (0x01U << RCMU_APB1ENR_PMUEN_Pos)        /*!<  */
#define RCMU_APB1ENR_PMUEN                        RCMU_APB1ENR_PMUEN_Msk                   /*!<  */
 
#define RCMU_APB1ENR_ALL_Msk                    (RCMU_APB1ENR_PMUEN_Msk | RCMU_APB1ENR_VCEN_Msk | RCMU_APB1ENR_I2C2EN_Msk | RCMU_APB1ENR_I2C1EN_Msk | RCMU_APB1ENR_LPUARTEN_Msk | RCMU_APB1ENR_USART2EN_Msk | RCMU_APB1ENR_SPI2EN_Msk | RCMU_APB1ENR_IWDGEN_Msk | RCMU_APB1ENR_WWDGEN_Msk | RCMU_APB1ENR_RTCEN_Msk | RCMU_APB1ENR_TIM14EN_Msk | RCMU_APB1ENR_TIM7EN_Msk | RCMU_APB1ENR_TIM6EN_Msk | RCMU_APB1ENR_TIM3EN_Msk)
 
/******************  Bit definition for RCMU_BDCR register ********************/
#define RCMU_BDCR_LSEON_Pos                    (0U)
#define RCMU_BDCR_LSEON_Msk                    (0x01U << RCMU_BDCR_LSEON_Pos)        /*!<  */
#define RCMU_BDCR_LSEON                        RCMU_BDCR_LSEON_Msk                   /*!<  */
 
#define RCMU_BDCR_LSERDY_Pos                    (1U)
#define RCMU_BDCR_LSERDY_Msk                    (0x01U << RCMU_BDCR_LSERDY_Pos)        /*!<  */
#define RCMU_BDCR_LSERDY                        RCMU_BDCR_LSERDY_Msk                   /*!<  */
 
#define RCMU_BDCR_LSEBYP_Pos                    (2U)
#define RCMU_BDCR_LSEBYP_Msk                    (0x01U << RCMU_BDCR_LSEBYP_Pos)        /*!<  */
#define RCMU_BDCR_LSEBYP                        RCMU_BDCR_LSEBYP_Msk                   /*!<  */
 
#define RCMU_BDCR_LSE_DL_Pos                    (3U)
#define RCMU_BDCR_LSE_DL_Msk                    (0xFU << RCMU_BDCR_LSE_DL_Pos)        /*!<  */
#define RCMU_BDCR_LSE_DL                        RCMU_BDCR_LSE_DL_Msk                   /*!<  */
 
#define RCMU_BDCR_RTCSEL_Pos                    (8U)
#define RCMU_BDCR_RTCSEL_Msk                    (0x3U << RCMU_BDCR_RTCSEL_Pos)        /*!<  */
#define RCMU_BDCR_RTCSEL                        RCMU_BDCR_RTCSEL_Msk                   /*!<  */
 
#define RCMU_BDCR_LSE_EXT_Pos                    (13U)
#define RCMU_BDCR_LSE_EXT_Msk                    (0x01U << RCMU_BDCR_LSE_EXT_Pos)        /*!<  */
#define RCMU_BDCR_LSE_EXT                        RCMU_BDCR_LSE_EXT_Msk                   /*!<  */
 
#define RCMU_BDCR_LSE_FORCE_OK_Pos                    (14U)
#define RCMU_BDCR_LSE_FORCE_OK_Msk                    (0x01U << RCMU_BDCR_LSE_FORCE_OK_Pos)        /*!<  */
#define RCMU_BDCR_LSE_FORCE_OK                        RCMU_BDCR_LSE_FORCE_OK_Msk                   /*!<  */
 
#define RCMU_BDCR_RTCEN_Pos                    (15U)
#define RCMU_BDCR_RTCEN_Msk                    (0x01U << RCMU_BDCR_RTCEN_Pos)        /*!<  */
#define RCMU_BDCR_RTCEN                        RCMU_BDCR_RTCEN_Msk                   /*!<  */
 
#define RCMU_BDCR_BDRST_Pos                    (16U)
#define RCMU_BDCR_BDRST_Msk                    (0x01U << RCMU_BDCR_BDRST_Pos)        /*!<  */
#define RCMU_BDCR_BDRST                        RCMU_BDCR_BDRST_Msk                   /*!<  */
 
#define RCMU_BDCR_ALL_Msk                    (RCMU_BDCR_BDRST_Msk | RCMU_BDCR_RTCEN_Msk | RCMU_BDCR_LSE_FORCE_OK_Msk | RCMU_BDCR_LSE_EXT_Msk | RCMU_BDCR_RTCSEL_Msk | RCMU_BDCR_LSE_DL_Msk | RCMU_BDCR_LSEBYP_Msk | RCMU_BDCR_LSERDY_Msk | RCMU_BDCR_LSEON_Msk)
 
/******************  Bit definition for RCMU_CSR register ********************/
#define RCMU_CSR_LSION_Pos                    (0U)
#define RCMU_CSR_LSION_Msk                    (0x01U << RCMU_CSR_LSION_Pos)        /*!<  */
#define RCMU_CSR_LSION                        RCMU_CSR_LSION_Msk                   /*!<  */
 
#define RCMU_CSR_LSIRDY_Pos                    (1U)
#define RCMU_CSR_LSIRDY_Msk                    (0x01U << RCMU_CSR_LSIRDY_Pos)        /*!<  */
#define RCMU_CSR_LSIRDY                        RCMU_CSR_LSIRDY_Msk                   /*!<  */
 
#define RCMU_CSR_LSE_FREQ_SEL_Pos                    (2U)
#define RCMU_CSR_LSE_FREQ_SEL_Msk                    (0x01U << RCMU_CSR_LSE_FREQ_SEL_Pos)        /*!<  */
#define RCMU_CSR_LSE_FREQ_SEL                        RCMU_CSR_LSE_FREQ_SEL_Msk                   /*!<  */
 
#define RCMU_CSR_LVDSTF_Pos                    (23U)
#define RCMU_CSR_LVDSTF_Msk                    (0x01U << RCMU_CSR_LVDSTF_Pos)        /*!<  */
#define RCMU_CSR_LVDSTF                        RCMU_CSR_LVDSTF_Msk                   /*!<  */
 
#define RCMU_CSR_RMVF_Pos                    (24U)
#define RCMU_CSR_RMVF_Msk                    (0x01U << RCMU_CSR_RMVF_Pos)        /*!<  */
#define RCMU_CSR_RMVF                        RCMU_CSR_RMVF_Msk                   /*!<  */
 
#define RCMU_CSR_LOCKUPSTF_Pos                    (25U)
#define RCMU_CSR_LOCKUPSTF_Msk                    (0x01U << RCMU_CSR_LOCKUPSTF_Pos)        /*!<  */
#define RCMU_CSR_LOCKUPSTF                        RCMU_CSR_LOCKUPSTF_Msk                   /*!<  */
 
#define RCMU_CSR_PINRSTF_Pos                    (26U)
#define RCMU_CSR_PINRSTF_Msk                    (0x01U << RCMU_CSR_PINRSTF_Pos)        /*!<  */
#define RCMU_CSR_PINRSTF                        RCMU_CSR_PINRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_PWRRSTF_Pos                    (27U)
#define RCMU_CSR_PWRRSTF_Msk                    (0x01U << RCMU_CSR_PWRRSTF_Pos)        /*!<  */
#define RCMU_CSR_PWRRSTF                        RCMU_CSR_PWRRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_SFTRSTF_Pos                    (28U)
#define RCMU_CSR_SFTRSTF_Msk                    (0x01U << RCMU_CSR_SFTRSTF_Pos)        /*!<  */
#define RCMU_CSR_SFTRSTF                        RCMU_CSR_SFTRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_IWDGRSTF_Pos                    (29U)
#define RCMU_CSR_IWDGRSTF_Msk                    (0x01U << RCMU_CSR_IWDGRSTF_Pos)        /*!<  */
#define RCMU_CSR_IWDGRSTF                        RCMU_CSR_IWDGRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_WWDGRSTF_Pos                    (30U)
#define RCMU_CSR_WWDGRSTF_Msk                    (0x01U << RCMU_CSR_WWDGRSTF_Pos)        /*!<  */
#define RCMU_CSR_WWDGRSTF                        RCMU_CSR_WWDGRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_LPWRRSTF_Pos                    (31U)
#define RCMU_CSR_LPWRRSTF_Msk                    (0x01U << RCMU_CSR_LPWRRSTF_Pos)        /*!<  */
#define RCMU_CSR_LPWRRSTF                        RCMU_CSR_LPWRRSTF_Msk                   /*!<  */
 
#define RCMU_CSR_ALL_Msk                    (RCMU_CSR_LPWRRSTF_Msk | RCMU_CSR_WWDGRSTF_Msk | RCMU_CSR_IWDGRSTF_Msk | RCMU_CSR_SFTRSTF_Msk | RCMU_CSR_PWRRSTF_Msk | RCMU_CSR_PINRSTF_Msk | RCMU_CSR_LOCKUPSTF_Msk | RCMU_CSR_RMVF_Msk | RCMU_CSR_LVDSTF_Msk | RCMU_CSR_LSE_FREQ_SEL_Msk | RCMU_CSR_LSIRDY_Msk | RCMU_CSR_LSION_Msk)
 
/******************  Bit definition for RCMU_AHBRSTR register ********************/
#define RCMU_AHBRSTR_DMARST_Pos                    (0U)
#define RCMU_AHBRSTR_DMARST_Msk                    (0x01U << RCMU_AHBRSTR_DMARST_Pos)        /*!<  */
#define RCMU_AHBRSTR_DMARST                        RCMU_AHBRSTR_DMARST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_SRAMRST_Pos                    (2U)
#define RCMU_AHBRSTR_SRAMRST_Msk                    (0x01U << RCMU_AHBRSTR_SRAMRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_SRAMRST                        RCMU_AHBRSTR_SRAMRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_FLITFRST_Pos                    (4U)
#define RCMU_AHBRSTR_FLITFRST_Msk                    (0x01U << RCMU_AHBRSTR_FLITFRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_FLITFRST                        RCMU_AHBRSTR_FLITFRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_TRNGRST_Pos                    (5U)
#define RCMU_AHBRSTR_TRNGRST_Msk                    (0x01U << RCMU_AHBRSTR_TRNGRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_TRNGRST                        RCMU_AHBRSTR_TRNGRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_CRCRST_Pos                    (6U)
#define RCMU_AHBRSTR_CRCRST_Msk                    (0x01U << RCMU_AHBRSTR_CRCRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_CRCRST                        RCMU_AHBRSTR_CRCRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_AESRST_Pos                    (7U)
#define RCMU_AHBRSTR_AESRST_Msk                    (0x01U << RCMU_AHBRSTR_AESRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_AESRST                        RCMU_AHBRSTR_AESRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_IOARST_Pos                    (16U)
#define RCMU_AHBRSTR_IOARST_Msk                    (0x01U << RCMU_AHBRSTR_IOARST_Pos)        /*!<  */
#define RCMU_AHBRSTR_IOARST                        RCMU_AHBRSTR_IOARST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_IOBRST_Pos                    (17U)
#define RCMU_AHBRSTR_IOBRST_Msk                    (0x01U << RCMU_AHBRSTR_IOBRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_IOBRST                        RCMU_AHBRSTR_IOBRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_IOCRST_Pos                    (18U)
#define RCMU_AHBRSTR_IOCRST_Msk                    (0x01U << RCMU_AHBRSTR_IOCRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_IOCRST                        RCMU_AHBRSTR_IOCRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_IODRST_Pos                    (19U)
#define RCMU_AHBRSTR_IODRST_Msk                    (0x01U << RCMU_AHBRSTR_IODRST_Pos)        /*!<  */
#define RCMU_AHBRSTR_IODRST                        RCMU_AHBRSTR_IODRST_Msk                   /*!<  */
 
#define RCMU_AHBRSTR_ALL_Msk                    (RCMU_AHBRSTR_IODRST_Msk | RCMU_AHBRSTR_IOCRST_Msk | RCMU_AHBRSTR_IOBRST_Msk | RCMU_AHBRSTR_IOARST_Msk | RCMU_AHBRSTR_AESRST_Msk | RCMU_AHBRSTR_CRCRST_Msk | RCMU_AHBRSTR_TRNGRST_Msk | RCMU_AHBRSTR_FLITFRST_Msk | RCMU_AHBRSTR_SRAMRST_Msk | RCMU_AHBRSTR_DMARST_Msk)
 
/******************  Bit definition for RCMU_CFGR2 register ********************/
#define RCMU_CFGR2_TIMPRE1_Pos                    (8U)
#define RCMU_CFGR2_TIMPRE1_Msk                    (0xFU << RCMU_CFGR2_TIMPRE1_Pos)        /*!<  */
#define RCMU_CFGR2_TIMPRE1                        RCMU_CFGR2_TIMPRE1_Msk                   /*!<  */
 
#define RCMU_CFGR2_TIMPRE2_Pos                    (12U)
#define RCMU_CFGR2_TIMPRE2_Msk                    (0xFU << RCMU_CFGR2_TIMPRE2_Pos)        /*!<  */
#define RCMU_CFGR2_TIMPRE2                        RCMU_CFGR2_TIMPRE2_Msk                   /*!<  */
 
#define RCMU_CFGR2_ALL_Msk                    (RCMU_CFGR2_TIMPRE2_Msk | RCMU_CFGR2_TIMPRE1_Msk)
 
/******************  Bit definition for RCMU_PLLCR register ********************/
#define RCMU_PLLCR_CFG0_M_Pos                    (0U)
#define RCMU_PLLCR_CFG0_M_Msk                    (0x1FU << RCMU_PLLCR_CFG0_M_Pos)        /*!<  */
#define RCMU_PLLCR_CFG0_M                        RCMU_PLLCR_CFG0_M_Msk                   /*!<  */
 
#define RCMU_PLLCR_CFG0_N_Pos                    (8U)
#define RCMU_PLLCR_CFG0_N_Msk                    (0x3U << RCMU_PLLCR_CFG0_N_Pos)        /*!<  */
#define RCMU_PLLCR_CFG0_N                        RCMU_PLLCR_CFG0_N_Msk                   /*!<  */
 
#define RCMU_PLLCR_CFG0_OD_Pos                    (11U)
#define RCMU_PLLCR_CFG0_OD_Msk                    (0x3U << RCMU_PLLCR_CFG0_OD_Pos)        /*!<  */
#define RCMU_PLLCR_CFG0_OD                        RCMU_PLLCR_CFG0_OD_Msk                   /*!<  */
 
#define RCMU_PLLCR_PLL_SRC_Pos                    (16U)
#define RCMU_PLLCR_PLL_SRC_Msk                    (0x01U << RCMU_PLLCR_PLL_SRC_Pos)        /*!<  */
#define RCMU_PLLCR_PLL_SRC                        RCMU_PLLCR_PLL_SRC_Msk                   /*!<  */
 
#define RCMU_PLLCR_PLL_BYP_Pos                    (17U)
#define RCMU_PLLCR_PLL_BYP_Msk                    (0x01U << RCMU_PLLCR_PLL_BYP_Pos)        /*!<  */
#define RCMU_PLLCR_PLL_BYP                        RCMU_PLLCR_PLL_BYP_Msk                   /*!<  */
 
#define RCMU_PLLCR_PLL_STARTUP_Pos                    (20U)
#define RCMU_PLLCR_PLL_STARTUP_Msk                    (0x7U << RCMU_PLLCR_PLL_STARTUP_Pos)        /*!<  */
#define RCMU_PLLCR_PLL_STARTUP                        RCMU_PLLCR_PLL_STARTUP_Msk                   /*!<  */
 
#define RCMU_PLLCR_ALL_Msk                    (RCMU_PLLCR_PLL_STARTUP_Msk | RCMU_PLLCR_PLL_BYP_Msk | RCMU_PLLCR_PLL_SRC_Msk | RCMU_PLLCR_CFG0_OD_Msk | RCMU_PLLCR_CFG0_N_Msk | RCMU_PLLCR_CFG0_M_Msk)
 
/******************  Bit definition for RCMU_PLLCR2 register ********************/
#define RCMU_PLLCR2_CFG1_M_Pos                    (0U)
#define RCMU_PLLCR2_CFG1_M_Msk                    (0x7FU << RCMU_PLLCR2_CFG1_M_Pos)        /*!<  */
#define RCMU_PLLCR2_CFG1_M                        RCMU_PLLCR2_CFG1_M_Msk                   /*!<  */
 
#define RCMU_PLLCR2_CFG1_N_Pos                    (8U)
#define RCMU_PLLCR2_CFG1_N_Msk                    (0x7U << RCMU_PLLCR2_CFG1_N_Pos)        /*!<  */
#define RCMU_PLLCR2_CFG1_N                        RCMU_PLLCR2_CFG1_N_Msk                   /*!<  */
 
#define RCMU_PLLCR2_CFG1_OD_Pos                    (11U)
#define RCMU_PLLCR2_CFG1_OD_Msk                    (0x7U << RCMU_PLLCR2_CFG1_OD_Pos)        /*!<  */
#define RCMU_PLLCR2_CFG1_OD                        RCMU_PLLCR2_CFG1_OD_Msk                   /*!<  */
 
#define RCMU_PLLCR2_RST_TH_Pos                    (16U)
#define RCMU_PLLCR2_RST_TH_Msk                    (0xFFU << RCMU_PLLCR2_RST_TH_Pos)        /*!<  */
#define RCMU_PLLCR2_RST_TH                        RCMU_PLLCR2_RST_TH_Msk                   /*!<  */
 
#define RCMU_PLLCR2_SEL_RST_Pos                    (24U)
#define RCMU_PLLCR2_SEL_RST_Msk                    (0x01U << RCMU_PLLCR2_SEL_RST_Pos)        /*!<  */
#define RCMU_PLLCR2_SEL_RST                        RCMU_PLLCR2_SEL_RST_Msk                   /*!<  */
 
#define RCMU_PLLCR2_SEL_M_Pos                    (25U)
#define RCMU_PLLCR2_SEL_M_Msk                    (0x01U << RCMU_PLLCR2_SEL_M_Pos)        /*!<  */
#define RCMU_PLLCR2_SEL_M                        RCMU_PLLCR2_SEL_M_Msk                   /*!<  */
 
#define RCMU_PLLCR2_SEL_N_Pos                    (26U)
#define RCMU_PLLCR2_SEL_N_Msk                    (0x01U << RCMU_PLLCR2_SEL_N_Pos)        /*!<  */
#define RCMU_PLLCR2_SEL_N                        RCMU_PLLCR2_SEL_N_Msk                   /*!<  */
 
#define RCMU_PLLCR2_SEL_OD_Pos                    (27U)
#define RCMU_PLLCR2_SEL_OD_Msk                    (0x01U << RCMU_PLLCR2_SEL_OD_Pos)        /*!<  */
#define RCMU_PLLCR2_SEL_OD                        RCMU_PLLCR2_SEL_OD_Msk                   /*!<  */
 
#define RCMU_PLLCR2_STAB_RST_Pos                    (28U)
#define RCMU_PLLCR2_STAB_RST_Msk                    (0x01U << RCMU_PLLCR2_STAB_RST_Pos)        /*!<  */
#define RCMU_PLLCR2_STAB_RST                        RCMU_PLLCR2_STAB_RST_Msk                   /*!<  */
 
#define RCMU_PLLCR2_ALL_Msk                    (RCMU_PLLCR2_STAB_RST_Msk | RCMU_PLLCR2_SEL_OD_Msk | RCMU_PLLCR2_SEL_N_Msk | RCMU_PLLCR2_SEL_M_Msk | RCMU_PLLCR2_SEL_RST_Msk | RCMU_PLLCR2_RST_TH_Msk | RCMU_PLLCR2_CFG1_OD_Msk | RCMU_PLLCR2_CFG1_N_Msk | RCMU_PLLCR2_CFG1_M_Msk)
 
/******************  Bit definition for RCMU_STCR register ********************/
#define RCMU_STCR_STCALIB_Pos                    (0U)
#define RCMU_STCR_STCALIB_Msk                    (0x3FFFFFFU << RCMU_STCR_STCALIB_Pos)        /*!<  */
#define RCMU_STCR_STCALIB                        RCMU_STCR_STCALIB_Msk                   /*!<  */
 
#define RCMU_STCR_STCLK_SEL_Pos                    (26U)
#define RCMU_STCR_STCLK_SEL_Msk                    (0x01U << RCMU_STCR_STCLK_SEL_Pos)        /*!<  */
#define RCMU_STCR_STCLK_SEL                        RCMU_STCR_STCLK_SEL_Msk                   /*!<  */
 
#define RCMU_STCR_STCLK_ON_Pos                    (27U)
#define RCMU_STCR_STCLK_ON_Msk                    (0x01U << RCMU_STCR_STCLK_ON_Pos)        /*!<  */
#define RCMU_STCR_STCLK_ON                        RCMU_STCR_STCLK_ON_Msk                   /*!<  */
 
#define RCMU_STCR_ALL_Msk                    (RCMU_STCR_STCLK_ON_Msk | RCMU_STCR_STCLK_SEL_Msk | RCMU_STCR_STCALIB_Msk)
 
/******************  Bit definition for RCMU_IRQLATENCY register ********************/
#define RCMU_IRQLATENCY_IRQLATENCY_Pos                    (0U)
#define RCMU_IRQLATENCY_IRQLATENCY_Msk                    (0xFFU << RCMU_IRQLATENCY_IRQLATENCY_Pos)        /*!<  */
#define RCMU_IRQLATENCY_IRQLATENCY                        RCMU_IRQLATENCY_IRQLATENCY_Msk                   /*!<  */
 
#define RCMU_IRQLATENCY_ALL_Msk                    (RCMU_IRQLATENCY_IRQLATENCY_Msk)
 
/******************  Bit definition for RTC_CR register ********************/
#define RTC_CR_BYPSHAD_Pos                    (0U)
#define RTC_CR_BYPSHAD_Msk                    (0x01U << RTC_CR_BYPSHAD_Pos)        /*!<  */
#define RTC_CR_BYPSHAD                        RTC_CR_BYPSHAD_Msk                   /*!<  */
 
#define RTC_CR_RTC1HZOE_Pos                    (1U)
#define RTC_CR_RTC1HZOE_Msk                    (0x01U << RTC_CR_RTC1HZOE_Pos)        /*!<  */
#define RTC_CR_RTC1HZOE                        RTC_CR_RTC1HZOE_Msk                   /*!<  */
 
#define RTC_CR_FMT_Pos                    (2U)
#define RTC_CR_FMT_Msk                    (0x01U << RTC_CR_FMT_Pos)        /*!<  */
#define RTC_CR_FMT                        RTC_CR_FMT_Msk                   /*!<  */
 
#define RTC_CR_ALM1_INTEN_Pos                    (4U)
#define RTC_CR_ALM1_INTEN_Msk                    (0x01U << RTC_CR_ALM1_INTEN_Pos)        /*!<  */
#define RTC_CR_ALM1_INTEN                        RTC_CR_ALM1_INTEN_Msk                   /*!<  */
 
#define RTC_CR_ALM2_INTEN_Pos                    (5U)
#define RTC_CR_ALM2_INTEN_Msk                    (0x01U << RTC_CR_ALM2_INTEN_Pos)        /*!<  */
#define RTC_CR_ALM2_INTEN                        RTC_CR_ALM2_INTEN_Msk                   /*!<  */
 
#define RTC_CR_ALM1EN_Pos                    (6U)
#define RTC_CR_ALM1EN_Msk                    (0x01U << RTC_CR_ALM1EN_Pos)        /*!<  */
#define RTC_CR_ALM1EN                        RTC_CR_ALM1EN_Msk                   /*!<  */
 
#define RTC_CR_ALM2EN_Pos                    (7U)
#define RTC_CR_ALM2EN_Msk                    (0x01U << RTC_CR_ALM2EN_Pos)        /*!<  */
#define RTC_CR_ALM2EN                        RTC_CR_ALM2EN_Msk                   /*!<  */
 
#define RTC_CR_START_Pos                    (8U)
#define RTC_CR_START_Msk                    (0x01U << RTC_CR_START_Pos)        /*!<  */
#define RTC_CR_START                        RTC_CR_START_Msk                   /*!<  */
 
#define RTC_CR_POL_Pos                    (20U)
#define RTC_CR_POL_Msk                    (0x01U << RTC_CR_POL_Pos)        /*!<  */
#define RTC_CR_POL                        RTC_CR_POL_Msk                   /*!<  */
 
#define RTC_CR_OSEL_Pos                    (21U)
#define RTC_CR_OSEL_Msk                    (0x3U << RTC_CR_OSEL_Pos)        /*!<  */
#define RTC_CR_OSEL                        RTC_CR_OSEL_Msk                   /*!<  */
 
#define RTC_CR_ALL_Msk                    (RTC_CR_OSEL_Msk | RTC_CR_POL_Msk | RTC_CR_START_Msk | RTC_CR_ALM2EN_Msk | RTC_CR_ALM1EN_Msk | RTC_CR_ALM2_INTEN_Msk | RTC_CR_ALM1_INTEN_Msk | RTC_CR_FMT_Msk | RTC_CR_RTC1HZOE_Msk | RTC_CR_BYPSHAD_Msk)
 
/******************  Bit definition for RTC_TIME register ********************/
#define RTC_TIME_SECU_Pos                    (0U)
#define RTC_TIME_SECU_Msk                    (0xFU << RTC_TIME_SECU_Pos)        /*!<  */
#define RTC_TIME_SECU                        RTC_TIME_SECU_Msk                   /*!<  */
 
#define RTC_TIME_SECT_Pos                    (4U)
#define RTC_TIME_SECT_Msk                    (0x7U << RTC_TIME_SECT_Pos)        /*!<  */
#define RTC_TIME_SECT                        RTC_TIME_SECT_Msk                   /*!<  */
 
#define RTC_TIME_MINU_Pos                    (8U)
#define RTC_TIME_MINU_Msk                    (0xFU << RTC_TIME_MINU_Pos)        /*!<  */
#define RTC_TIME_MINU                        RTC_TIME_MINU_Msk                   /*!<  */
 
#define RTC_TIME_MINT_Pos                    (12U)
#define RTC_TIME_MINT_Msk                    (0x7U << RTC_TIME_MINT_Pos)        /*!<  */
#define RTC_TIME_MINT                        RTC_TIME_MINT_Msk                   /*!<  */
 
#define RTC_TIME_HOURU_Pos                    (16U)
#define RTC_TIME_HOURU_Msk                    (0xFU << RTC_TIME_HOURU_Pos)        /*!<  */
#define RTC_TIME_HOURU                        RTC_TIME_HOURU_Msk                   /*!<  */
 
#define RTC_TIME_HOURT_Pos                    (20U)
#define RTC_TIME_HOURT_Msk                    (0x3U << RTC_TIME_HOURT_Pos)        /*!<  */
#define RTC_TIME_HOURT                        RTC_TIME_HOURT_Msk                   /*!<  */
 
#define RTC_TIME_PM_Pos                    (22U)
#define RTC_TIME_PM_Msk                    (0x01U << RTC_TIME_PM_Pos)        /*!<  */
#define RTC_TIME_PM                        RTC_TIME_PM_Msk                   /*!<  */
 
#define RTC_TIME_WEEK_Pos                    (24U)
#define RTC_TIME_WEEK_Msk                    (0x7U << RTC_TIME_WEEK_Pos)        /*!<  */
#define RTC_TIME_WEEK                        RTC_TIME_WEEK_Msk                   /*!<  */
 
#define RTC_TIME_ALL_Msk                    (RTC_TIME_WEEK_Msk | RTC_TIME_PM_Msk | RTC_TIME_HOURT_Msk | RTC_TIME_HOURU_Msk | RTC_TIME_MINT_Msk | RTC_TIME_MINU_Msk | RTC_TIME_SECT_Msk | RTC_TIME_SECU_Msk)
 
/******************  Bit definition for RTC_DATE register ********************/
#define RTC_DATE_DAYU_Pos                    (0U)
#define RTC_DATE_DAYU_Msk                    (0xFU << RTC_DATE_DAYU_Pos)        /*!<  */
#define RTC_DATE_DAYU                        RTC_DATE_DAYU_Msk                   /*!<  */
 
#define RTC_DATE_DAYT_Pos                    (4U)
#define RTC_DATE_DAYT_Msk                    (0x3U << RTC_DATE_DAYT_Pos)        /*!<  */
#define RTC_DATE_DAYT                        RTC_DATE_DAYT_Msk                   /*!<  */
 
#define RTC_DATE_MONU_Pos                    (8U)
#define RTC_DATE_MONU_Msk                    (0xFU << RTC_DATE_MONU_Pos)        /*!<  */
#define RTC_DATE_MONU                        RTC_DATE_MONU_Msk                   /*!<  */
 
#define RTC_DATE_MONT_Pos                    (12U)
#define RTC_DATE_MONT_Msk                    (0x01U << RTC_DATE_MONT_Pos)        /*!<  */
#define RTC_DATE_MONT                        RTC_DATE_MONT_Msk                   /*!<  */
 
#define RTC_DATE_CEN_Pos                    (15U)
#define RTC_DATE_CEN_Msk                    (0x01U << RTC_DATE_CEN_Pos)        /*!<  */
#define RTC_DATE_CEN                        RTC_DATE_CEN_Msk                   /*!<  */
 
#define RTC_DATE_YEARU_Pos                    (16U)
#define RTC_DATE_YEARU_Msk                    (0xFU << RTC_DATE_YEARU_Pos)        /*!<  */
#define RTC_DATE_YEARU                        RTC_DATE_YEARU_Msk                   /*!<  */
 
#define RTC_DATE_YEART_Pos                    (20U)
#define RTC_DATE_YEART_Msk                    (0xFU << RTC_DATE_YEART_Pos)        /*!<  */
#define RTC_DATE_YEART                        RTC_DATE_YEART_Msk                   /*!<  */
 
#define RTC_DATE_ALL_Msk                    (RTC_DATE_YEART_Msk | RTC_DATE_YEARU_Msk | RTC_DATE_CEN_Msk | RTC_DATE_MONT_Msk | RTC_DATE_MONU_Msk | RTC_DATE_DAYT_Msk | RTC_DATE_DAYU_Msk)
 
/******************  Bit definition for RTC_ALM1TIME register ********************/
#define RTC_ALM1TIME_ALMSECU_Pos                    (0U)
#define RTC_ALM1TIME_ALMSECU_Msk                    (0xFU << RTC_ALM1TIME_ALMSECU_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMSECU                        RTC_ALM1TIME_ALMSECU_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMSECT_Pos                    (4U)
#define RTC_ALM1TIME_ALMSECT_Msk                    (0x7U << RTC_ALM1TIME_ALMSECT_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMSECT                        RTC_ALM1TIME_ALMSECT_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMMINU_Pos                    (8U)
#define RTC_ALM1TIME_ALMMINU_Msk                    (0xFU << RTC_ALM1TIME_ALMMINU_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMMINU                        RTC_ALM1TIME_ALMMINU_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMMINT_Pos                    (12U)
#define RTC_ALM1TIME_ALMMINT_Msk                    (0x7U << RTC_ALM1TIME_ALMMINT_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMMINT                        RTC_ALM1TIME_ALMMINT_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMHOURU_Pos                    (16U)
#define RTC_ALM1TIME_ALMHOURU_Msk                    (0xFU << RTC_ALM1TIME_ALMHOURU_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMHOURU                        RTC_ALM1TIME_ALMHOURU_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMHOURT_Pos                    (20U)
#define RTC_ALM1TIME_ALMHOURT_Msk                    (0x3U << RTC_ALM1TIME_ALMHOURT_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMHOURT                        RTC_ALM1TIME_ALMHOURT_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMPM_Pos                    (22U)
#define RTC_ALM1TIME_ALMPM_Msk                    (0x01U << RTC_ALM1TIME_ALMPM_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMPM                        RTC_ALM1TIME_ALMPM_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALMWEEK_Pos                    (24U)
#define RTC_ALM1TIME_ALMWEEK_Msk                    (0x7U << RTC_ALM1TIME_ALMWEEK_Pos)        /*!<  */
#define RTC_ALM1TIME_ALMWEEK                        RTC_ALM1TIME_ALMWEEK_Msk                   /*!<  */
 
#define RTC_ALM1TIME_ALL_Msk                    (RTC_ALM1TIME_ALMWEEK_Msk | RTC_ALM1TIME_ALMPM_Msk | RTC_ALM1TIME_ALMHOURT_Msk | RTC_ALM1TIME_ALMHOURU_Msk | RTC_ALM1TIME_ALMMINT_Msk | RTC_ALM1TIME_ALMMINU_Msk | RTC_ALM1TIME_ALMSECT_Msk | RTC_ALM1TIME_ALMSECU_Msk)
 
/******************  Bit definition for RTC_ALM1DATE register ********************/
#define RTC_ALM1DATE_DAYU_Pos                    (0U)
#define RTC_ALM1DATE_DAYU_Msk                    (0xFU << RTC_ALM1DATE_DAYU_Pos)        /*!<  */
#define RTC_ALM1DATE_DAYU                        RTC_ALM1DATE_DAYU_Msk                   /*!<  */
 
#define RTC_ALM1DATE_DAYT_Pos                    (4U)
#define RTC_ALM1DATE_DAYT_Msk                    (0x3U << RTC_ALM1DATE_DAYT_Pos)        /*!<  */
#define RTC_ALM1DATE_DAYT                        RTC_ALM1DATE_DAYT_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMMONU_Pos                    (8U)
#define RTC_ALM1DATE_ALMMONU_Msk                    (0xFU << RTC_ALM1DATE_ALMMONU_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMMONU                        RTC_ALM1DATE_ALMMONU_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMMONT_Pos                    (12U)
#define RTC_ALM1DATE_ALMMONT_Msk                    (0x01U << RTC_ALM1DATE_ALMMONT_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMMONT                        RTC_ALM1DATE_ALMMONT_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMCEN_Pos                    (15U)
#define RTC_ALM1DATE_ALMCEN_Msk                    (0x01U << RTC_ALM1DATE_ALMCEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMCEN                        RTC_ALM1DATE_ALMCEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMYEARU_Pos                    (16U)
#define RTC_ALM1DATE_ALMYEARU_Msk                    (0xFU << RTC_ALM1DATE_ALMYEARU_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMYEARU                        RTC_ALM1DATE_ALMYEARU_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMYEART_Pos                    (20U)
#define RTC_ALM1DATE_ALMYEART_Msk                    (0xFU << RTC_ALM1DATE_ALMYEART_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMYEART                        RTC_ALM1DATE_ALMYEART_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMSECEN_Pos                    (24U)
#define RTC_ALM1DATE_ALMSECEN_Msk                    (0x01U << RTC_ALM1DATE_ALMSECEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMSECEN                        RTC_ALM1DATE_ALMSECEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMMINEN_Pos                    (25U)
#define RTC_ALM1DATE_ALMMINEN_Msk                    (0x01U << RTC_ALM1DATE_ALMMINEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMMINEN                        RTC_ALM1DATE_ALMMINEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMHOUREN_Pos                    (26U)
#define RTC_ALM1DATE_ALMHOUREN_Msk                    (0x01U << RTC_ALM1DATE_ALMHOUREN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMHOUREN                        RTC_ALM1DATE_ALMHOUREN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMWEEKEN_Pos                    (27U)
#define RTC_ALM1DATE_ALMWEEKEN_Msk                    (0x01U << RTC_ALM1DATE_ALMWEEKEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMWEEKEN                        RTC_ALM1DATE_ALMWEEKEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMDAYEN_Pos                    (28U)
#define RTC_ALM1DATE_ALMDAYEN_Msk                    (0x01U << RTC_ALM1DATE_ALMDAYEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMDAYEN                        RTC_ALM1DATE_ALMDAYEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMMONEN_Pos                    (29U)
#define RTC_ALM1DATE_ALMMONEN_Msk                    (0x01U << RTC_ALM1DATE_ALMMONEN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMMONEN                        RTC_ALM1DATE_ALMMONEN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALMYEAREN_Pos                    (30U)
#define RTC_ALM1DATE_ALMYEAREN_Msk                    (0x01U << RTC_ALM1DATE_ALMYEAREN_Pos)        /*!<  */
#define RTC_ALM1DATE_ALMYEAREN                        RTC_ALM1DATE_ALMYEAREN_Msk                   /*!<  */
 
#define RTC_ALM1DATE_ALL_Msk                    (RTC_ALM1DATE_ALMYEAREN_Msk | RTC_ALM1DATE_ALMMONEN_Msk | RTC_ALM1DATE_ALMDAYEN_Msk | RTC_ALM1DATE_ALMWEEKEN_Msk | RTC_ALM1DATE_ALMHOUREN_Msk | RTC_ALM1DATE_ALMMINEN_Msk | RTC_ALM1DATE_ALMSECEN_Msk | RTC_ALM1DATE_ALMYEART_Msk | RTC_ALM1DATE_ALMYEARU_Msk | RTC_ALM1DATE_ALMCEN_Msk | RTC_ALM1DATE_ALMMONT_Msk | RTC_ALM1DATE_ALMMONU_Msk | RTC_ALM1DATE_DAYT_Msk | RTC_ALM1DATE_DAYU_Msk)
 
/******************  Bit definition for RTC_ALM2PRD register ********************/
#define RTC_ALM2PRD_ALM2PR_CNT_Pos                    (0U)
#define RTC_ALM2PRD_ALM2PR_CNT_Msk                    (0xFU << RTC_ALM2PRD_ALM2PR_CNT_Pos)        /*!<  */
#define RTC_ALM2PRD_ALM2PR_CNT                        RTC_ALM2PRD_ALM2PR_CNT_Msk                   /*!<  */
 
#define RTC_ALM2PRD_ALL_Msk                    (RTC_ALM2PRD_ALM2PR_CNT_Msk)
 
/******************  Bit definition for RTC_CLKCALR register ********************/
#define RTC_CLKCALR_TRIM_Pos                    (0U)
#define RTC_CLKCALR_TRIM_Msk                    (0xFFU << RTC_CLKCALR_TRIM_Pos)        /*!<  */
#define RTC_CLKCALR_TRIM                        RTC_CLKCALR_TRIM_Msk                   /*!<  */
 
#define RTC_CLKCALR_MODE_Pos                    (8U)
#define RTC_CLKCALR_MODE_Msk                    (0x3U << RTC_CLKCALR_MODE_Pos)        /*!<  */
#define RTC_CLKCALR_MODE                        RTC_CLKCALR_MODE_Msk                   /*!<  */
 
#define RTC_CLKCALR_TRIM_EN_Pos                    (16U)
#define RTC_CLKCALR_TRIM_EN_Msk                    (0x01U << RTC_CLKCALR_TRIM_EN_Pos)        /*!<  */
#define RTC_CLKCALR_TRIM_EN                        RTC_CLKCALR_TRIM_EN_Msk                   /*!<  */
 
#define RTC_CLKCALR_ALL_Msk                    (RTC_CLKCALR_TRIM_EN_Msk | RTC_CLKCALR_MODE_Msk | RTC_CLKCALR_TRIM_Msk)
 
/******************  Bit definition for RTC_ISR register ********************/
#define RTC_ISR_WAIT_Pos                    (0U)
#define RTC_ISR_WAIT_Msk                    (0x01U << RTC_ISR_WAIT_Pos)        /*!<  */
#define RTC_ISR_WAIT                        RTC_ISR_WAIT_Msk                   /*!<  */
 
#define RTC_ISR_WAITF_Pos                    (1U)
#define RTC_ISR_WAITF_Msk                    (0x01U << RTC_ISR_WAITF_Pos)        /*!<  */
#define RTC_ISR_WAITF                        RTC_ISR_WAITF_Msk                   /*!<  */
 
#define RTC_ISR_RSF_Pos                    (2U)
#define RTC_ISR_RSF_Msk                    (0x01U << RTC_ISR_RSF_Pos)        /*!<  */
#define RTC_ISR_RSF                        RTC_ISR_RSF_Msk                   /*!<  */
 
#define RTC_ISR_ALM1_F_Pos                    (4U)
#define RTC_ISR_ALM1_F_Msk                    (0x01U << RTC_ISR_ALM1_F_Pos)        /*!<  */
#define RTC_ISR_ALM1_F                        RTC_ISR_ALM1_F_Msk                   /*!<  */
 
#define RTC_ISR_ALM2_F_Pos                    (5U)
#define RTC_ISR_ALM2_F_Msk                    (0x01U << RTC_ISR_ALM2_F_Pos)        /*!<  */
#define RTC_ISR_ALM2_F                        RTC_ISR_ALM2_F_Msk                   /*!<  */
 
#define RTC_ISR_TAMPF_Pos                    (12U)
#define RTC_ISR_TAMPF_Msk                    (0x01U << RTC_ISR_TAMPF_Pos)        /*!<  */
#define RTC_ISR_TAMPF                        RTC_ISR_TAMPF_Msk                   /*!<  */
 
#define RTC_ISR_TAMPOVF_Pos                    (13U)
#define RTC_ISR_TAMPOVF_Msk                    (0x01U << RTC_ISR_TAMPOVF_Pos)        /*!<  */
#define RTC_ISR_TAMPOVF                        RTC_ISR_TAMPOVF_Msk                   /*!<  */
 
#define RTC_ISR_ALL_Msk                    (RTC_ISR_TAMPOVF_Msk | RTC_ISR_TAMPF_Msk | RTC_ISR_ALM2_F_Msk | RTC_ISR_ALM1_F_Msk | RTC_ISR_RSF_Msk | RTC_ISR_WAITF_Msk | RTC_ISR_WAIT_Msk)
 
/******************  Bit definition for RTC_INTCLR register ********************/
#define RTC_INTCLR_ALM1_CLR_Pos                    (4U)
#define RTC_INTCLR_ALM1_CLR_Msk                    (0x01U << RTC_INTCLR_ALM1_CLR_Pos)        /*!<  */
#define RTC_INTCLR_ALM1_CLR                        RTC_INTCLR_ALM1_CLR_Msk                   /*!<  */
 
#define RTC_INTCLR_ALM2_CLR_Pos                    (5U)
#define RTC_INTCLR_ALM2_CLR_Msk                    (0x01U << RTC_INTCLR_ALM2_CLR_Pos)        /*!<  */
#define RTC_INTCLR_ALM2_CLR                        RTC_INTCLR_ALM2_CLR_Msk                   /*!<  */
 
#define RTC_INTCLR_TAMPF_CLR_Pos                    (12U)
#define RTC_INTCLR_TAMPF_CLR_Msk                    (0x01U << RTC_INTCLR_TAMPF_CLR_Pos)        /*!<  */
#define RTC_INTCLR_TAMPF_CLR                        RTC_INTCLR_TAMPF_CLR_Msk                   /*!<  */
 
#define RTC_INTCLR_TAMPOVF_CLR_Pos                    (13U)
#define RTC_INTCLR_TAMPOVF_CLR_Msk                    (0x01U << RTC_INTCLR_TAMPOVF_CLR_Pos)        /*!<  */
#define RTC_INTCLR_TAMPOVF_CLR                        RTC_INTCLR_TAMPOVF_CLR_Msk                   /*!<  */
 
#define RTC_INTCLR_ALL_Msk                    (RTC_INTCLR_TAMPOVF_CLR_Msk | RTC_INTCLR_TAMPF_CLR_Msk | RTC_INTCLR_ALM2_CLR_Msk | RTC_INTCLR_ALM1_CLR_Msk)
 
/******************  Bit definition for RTC_WPR register ********************/
#define RTC_WPR_WPR_Pos                    (0U)
#define RTC_WPR_WPR_Msk                    (0xFFU << RTC_WPR_WPR_Pos)        /*!<  */
#define RTC_WPR_WPR                        RTC_WPR_WPR_Msk                   /*!<  */
 
#define RTC_WPR_ALL_Msk                    (RTC_WPR_WPR_Msk)
 
/******************  Bit definition for RTC_TSTR register ********************/
#define RTC_TSTR_SECT_0_Pos                    (0U)
#define RTC_TSTR_SECT_0_Msk                    (0xFU << RTC_TSTR_SECT_0_Pos)        /*!<  */
#define RTC_TSTR_SECT_0                        RTC_TSTR_SECT_0_Msk                   /*!<  */
 
#define RTC_TSTR_SECT_1_Pos                    (4U)
#define RTC_TSTR_SECT_1_Msk                    (0x7U << RTC_TSTR_SECT_1_Pos)        /*!<  */
#define RTC_TSTR_SECT_1                        RTC_TSTR_SECT_1_Msk                   /*!<  */
 
#define RTC_TSTR_MINU_Pos                    (8U)
#define RTC_TSTR_MINU_Msk                    (0xFU << RTC_TSTR_MINU_Pos)        /*!<  */
#define RTC_TSTR_MINU                        RTC_TSTR_MINU_Msk                   /*!<  */
 
#define RTC_TSTR_MINT_Pos                    (12U)
#define RTC_TSTR_MINT_Msk                    (0x7U << RTC_TSTR_MINT_Pos)        /*!<  */
#define RTC_TSTR_MINT                        RTC_TSTR_MINT_Msk                   /*!<  */
 
#define RTC_TSTR_HOURU_Pos                    (16U)
#define RTC_TSTR_HOURU_Msk                    (0xFU << RTC_TSTR_HOURU_Pos)        /*!<  */
#define RTC_TSTR_HOURU                        RTC_TSTR_HOURU_Msk                   /*!<  */
 
#define RTC_TSTR_HOURT_Pos                    (20U)
#define RTC_TSTR_HOURT_Msk                    (0x3U << RTC_TSTR_HOURT_Pos)        /*!<  */
#define RTC_TSTR_HOURT                        RTC_TSTR_HOURT_Msk                   /*!<  */
 
#define RTC_TSTR_PM_Pos                    (22U)
#define RTC_TSTR_PM_Msk                    (0x01U << RTC_TSTR_PM_Pos)        /*!<  */
#define RTC_TSTR_PM                        RTC_TSTR_PM_Msk                   /*!<  */
 
#define RTC_TSTR_WEEK_Pos                    (24U)
#define RTC_TSTR_WEEK_Msk                    (0x7U << RTC_TSTR_WEEK_Pos)        /*!<  */
#define RTC_TSTR_WEEK                        RTC_TSTR_WEEK_Msk                   /*!<  */
 
#define RTC_TSTR_ALL_Msk                    (RTC_TSTR_WEEK_Msk | RTC_TSTR_PM_Msk | RTC_TSTR_HOURT_Msk | RTC_TSTR_HOURU_Msk | RTC_TSTR_MINT_Msk | RTC_TSTR_MINU_Msk | RTC_TSTR_SECT_1_Msk | RTC_TSTR_SECT_0_Msk)
 
/******************  Bit definition for RTC_TSDR register ********************/
#define RTC_TSDR_DAYU_Pos                    (0U)
#define RTC_TSDR_DAYU_Msk                    (0xFU << RTC_TSDR_DAYU_Pos)        /*!<  */
#define RTC_TSDR_DAYU                        RTC_TSDR_DAYU_Msk                   /*!<  */
 
#define RTC_TSDR_DAYT_Pos                    (4U)
#define RTC_TSDR_DAYT_Msk                    (0x3U << RTC_TSDR_DAYT_Pos)        /*!<  */
#define RTC_TSDR_DAYT                        RTC_TSDR_DAYT_Msk                   /*!<  */
 
#define RTC_TSDR_MONU_Pos                    (8U)
#define RTC_TSDR_MONU_Msk                    (0xFU << RTC_TSDR_MONU_Pos)        /*!<  */
#define RTC_TSDR_MONU                        RTC_TSDR_MONU_Msk                   /*!<  */
 
#define RTC_TSDR_MONT_Pos                    (12U)
#define RTC_TSDR_MONT_Msk                    (0x01U << RTC_TSDR_MONT_Pos)        /*!<  */
#define RTC_TSDR_MONT                        RTC_TSDR_MONT_Msk                   /*!<  */
 
#define RTC_TSDR_CEN_Pos                    (15U)
#define RTC_TSDR_CEN_Msk                    (0x01U << RTC_TSDR_CEN_Pos)        /*!<  */
#define RTC_TSDR_CEN                        RTC_TSDR_CEN_Msk                   /*!<  */
 
#define RTC_TSDR_YEARU_Pos                    (16U)
#define RTC_TSDR_YEARU_Msk                    (0xFU << RTC_TSDR_YEARU_Pos)        /*!<  */
#define RTC_TSDR_YEARU                        RTC_TSDR_YEARU_Msk                   /*!<  */
 
#define RTC_TSDR_YEART_Pos                    (20U)
#define RTC_TSDR_YEART_Msk                    (0xFU << RTC_TSDR_YEART_Pos)        /*!<  */
#define RTC_TSDR_YEART                        RTC_TSDR_YEART_Msk                   /*!<  */
 
#define RTC_TSDR_ALL_Msk                    (RTC_TSDR_YEART_Msk | RTC_TSDR_YEARU_Msk | RTC_TSDR_CEN_Msk | RTC_TSDR_MONT_Msk | RTC_TSDR_MONU_Msk | RTC_TSDR_DAYT_Msk | RTC_TSDR_DAYU_Msk)
 
/******************  Bit definition for RTC_TAFCR register ********************/
#define RTC_TAFCR_TMAPE_Pos                    (0U)
#define RTC_TAFCR_TMAPE_Msk                    (0x01U << RTC_TAFCR_TMAPE_Pos)        /*!<  */
#define RTC_TAFCR_TMAPE                        RTC_TAFCR_TMAPE_Msk                   /*!<  */
 
#define RTC_TAFCR_TAMPIE_Pos                    (2U)
#define RTC_TAFCR_TAMPIE_Msk                    (0x01U << RTC_TAFCR_TAMPIE_Pos)        /*!<  */
#define RTC_TAFCR_TAMPIE                        RTC_TAFCR_TAMPIE_Msk                   /*!<  */
 
#define RTC_TAFCR_TAMP_MODE_Pos                    (8U)
#define RTC_TAFCR_TAMP_MODE_Msk                    (0x7U << RTC_TAFCR_TAMP_MODE_Pos)        /*!<  */
#define RTC_TAFCR_TAMP_MODE                        RTC_TAFCR_TAMP_MODE_Msk                   /*!<  */
 
#define RTC_TAFCR_TAMP_DB_Pos                    (12U)
#define RTC_TAFCR_TAMP_DB_Msk                    (0x3U << RTC_TAFCR_TAMP_DB_Pos)        /*!<  */
#define RTC_TAFCR_TAMP_DB                        RTC_TAFCR_TAMP_DB_Msk                   /*!<  */
 
#define RTC_TAFCR_PC13VALUE_Pos                    (18U)
#define RTC_TAFCR_PC13VALUE_Msk                    (0x01U << RTC_TAFCR_PC13VALUE_Pos)        /*!<  */
#define RTC_TAFCR_PC13VALUE                        RTC_TAFCR_PC13VALUE_Msk                   /*!<  */
 
#define RTC_TAFCR_PC13MODE_Pos                    (19U)
#define RTC_TAFCR_PC13MODE_Msk                    (0x01U << RTC_TAFCR_PC13MODE_Pos)        /*!<  */
#define RTC_TAFCR_PC13MODE                        RTC_TAFCR_PC13MODE_Msk                   /*!<  */
 
#define RTC_TAFCR_PC14VALUE_Pos                    (20U)
#define RTC_TAFCR_PC14VALUE_Msk                    (0x01U << RTC_TAFCR_PC14VALUE_Pos)        /*!<  */
#define RTC_TAFCR_PC14VALUE                        RTC_TAFCR_PC14VALUE_Msk                   /*!<  */
 
#define RTC_TAFCR_PC14MODE_Pos                    (21U)
#define RTC_TAFCR_PC14MODE_Msk                    (0x01U << RTC_TAFCR_PC14MODE_Pos)        /*!<  */
#define RTC_TAFCR_PC14MODE                        RTC_TAFCR_PC14MODE_Msk                   /*!<  */
 
#define RTC_TAFCR_PC15VALUE_Pos                    (22U)
#define RTC_TAFCR_PC15VALUE_Msk                    (0x01U << RTC_TAFCR_PC15VALUE_Pos)        /*!<  */
#define RTC_TAFCR_PC15VALUE                        RTC_TAFCR_PC15VALUE_Msk                   /*!<  */
 
#define RTC_TAFCR_PC15MODE_Pos                    (23U)
#define RTC_TAFCR_PC15MODE_Msk                    (0x01U << RTC_TAFCR_PC15MODE_Pos)        /*!<  */
#define RTC_TAFCR_PC15MODE                        RTC_TAFCR_PC15MODE_Msk                   /*!<  */
 
#define RTC_TAFCR_ALL_Msk                    (RTC_TAFCR_PC15MODE_Msk | RTC_TAFCR_PC15VALUE_Msk | RTC_TAFCR_PC14MODE_Msk | RTC_TAFCR_PC14VALUE_Msk | RTC_TAFCR_PC13MODE_Msk | RTC_TAFCR_PC13VALUE_Msk | RTC_TAFCR_TAMP_DB_Msk | RTC_TAFCR_TAMP_MODE_Msk | RTC_TAFCR_TAMPIE_Msk | RTC_TAFCR_TMAPE_Msk)
 
/******************  Bit definition for RTC_BKPxR register ********************/
#define RTC_BKPXR_BKP_Pos                    (0U)
#define RTC_BKPXR_BKP_Msk                    (0xFFFFFFFFU << RTC_BKPXR_BKP_Pos)        /*!<  */
#define RTC_BKPXR_BKP                        RTC_BKPXR_BKP_Msk                   /*!<  */
 
#define RTC_BKPXR_ALL_Msk                    (RTC_BKPXR_BKP_Msk)
 
/******************  Bit definition for GPRx_MODER register ********************/
#define GPRX_MODER_MODE0_Pos                    (0U)
#define GPRX_MODER_MODE0_Msk                    (0x3U << GPRX_MODER_MODE0_Pos)        /*!<  */
#define GPRX_MODER_MODE0                        GPRX_MODER_MODE0_Msk                   /*!<  */
 
#define GPRX_MODER_MODE1_Pos                    (2U)
#define GPRX_MODER_MODE1_Msk                    (0x3U << GPRX_MODER_MODE1_Pos)        /*!<  */
#define GPRX_MODER_MODE1                        GPRX_MODER_MODE1_Msk                   /*!<  */
 
#define GPRX_MODER_MODE2_Pos                    (4U)
#define GPRX_MODER_MODE2_Msk                    (0x3U << GPRX_MODER_MODE2_Pos)        /*!<  */
#define GPRX_MODER_MODE2                        GPRX_MODER_MODE2_Msk                   /*!<  */
 
#define GPRX_MODER_MODE3_Pos                    (6U)
#define GPRX_MODER_MODE3_Msk                    (0x3U << GPRX_MODER_MODE3_Pos)        /*!<  */
#define GPRX_MODER_MODE3                        GPRX_MODER_MODE3_Msk                   /*!<  */
 
#define GPRX_MODER_MODE4_Pos                    (8U)
#define GPRX_MODER_MODE4_Msk                    (0x3U << GPRX_MODER_MODE4_Pos)        /*!<  */
#define GPRX_MODER_MODE4                        GPRX_MODER_MODE4_Msk                   /*!<  */
 
#define GPRX_MODER_MODE5_Pos                    (10U)
#define GPRX_MODER_MODE5_Msk                    (0x3U << GPRX_MODER_MODE5_Pos)        /*!<  */
#define GPRX_MODER_MODE5                        GPRX_MODER_MODE5_Msk                   /*!<  */
 
#define GPRX_MODER_MODE6_Pos                    (12U)
#define GPRX_MODER_MODE6_Msk                    (0x3U << GPRX_MODER_MODE6_Pos)        /*!<  */
#define GPRX_MODER_MODE6                        GPRX_MODER_MODE6_Msk                   /*!<  */
 
#define GPRX_MODER_MODE7_Pos                    (14U)
#define GPRX_MODER_MODE7_Msk                    (0x3U << GPRX_MODER_MODE7_Pos)        /*!<  */
#define GPRX_MODER_MODE7                        GPRX_MODER_MODE7_Msk                   /*!<  */
 
#define GPRX_MODER_MODE8_Pos                    (16U)
#define GPRX_MODER_MODE8_Msk                    (0x3U << GPRX_MODER_MODE8_Pos)        /*!<  */
#define GPRX_MODER_MODE8                        GPRX_MODER_MODE8_Msk                   /*!<  */
 
#define GPRX_MODER_MODE9_Pos                    (18U)
#define GPRX_MODER_MODE9_Msk                    (0x3U << GPRX_MODER_MODE9_Pos)        /*!<  */
#define GPRX_MODER_MODE9                        GPRX_MODER_MODE9_Msk                   /*!<  */
 
#define GPRX_MODER_MODE10_Pos                    (20U)
#define GPRX_MODER_MODE10_Msk                    (0x3U << GPRX_MODER_MODE10_Pos)        /*!<  */
#define GPRX_MODER_MODE10                        GPRX_MODER_MODE10_Msk                   /*!<  */
 
#define GPRX_MODER_MODE11_Pos                    (22U)
#define GPRX_MODER_MODE11_Msk                    (0x3U << GPRX_MODER_MODE11_Pos)        /*!<  */
#define GPRX_MODER_MODE11                        GPRX_MODER_MODE11_Msk                   /*!<  */
 
#define GPRX_MODER_MODE12_Pos                    (24U)
#define GPRX_MODER_MODE12_Msk                    (0x3U << GPRX_MODER_MODE12_Pos)        /*!<  */
#define GPRX_MODER_MODE12                        GPRX_MODER_MODE12_Msk                   /*!<  */
 
#define GPRX_MODER_MODE13_Pos                    (26U)
#define GPRX_MODER_MODE13_Msk                    (0x3U << GPRX_MODER_MODE13_Pos)        /*!<  */
#define GPRX_MODER_MODE13                        GPRX_MODER_MODE13_Msk                   /*!<  */
 
#define GPRX_MODER_MODE14_Pos                    (28U)
#define GPRX_MODER_MODE14_Msk                    (0x3U << GPRX_MODER_MODE14_Pos)        /*!<  */
#define GPRX_MODER_MODE14                        GPRX_MODER_MODE14_Msk                   /*!<  */
 
#define GPRX_MODER_MODE15_Pos                    (30U)
#define GPRX_MODER_MODE15_Msk                    (0x3U << GPRX_MODER_MODE15_Pos)        /*!<  */
#define GPRX_MODER_MODE15                        GPRX_MODER_MODE15_Msk                   /*!<  */
 
#define GPRX_MODER_ALL_Msk                    (GPRX_MODER_MODE15_Msk | GPRX_MODER_MODE14_Msk | GPRX_MODER_MODE13_Msk | GPRX_MODER_MODE12_Msk | GPRX_MODER_MODE11_Msk | GPRX_MODER_MODE10_Msk | GPRX_MODER_MODE9_Msk | GPRX_MODER_MODE8_Msk | GPRX_MODER_MODE7_Msk | GPRX_MODER_MODE6_Msk | GPRX_MODER_MODE5_Msk | GPRX_MODER_MODE4_Msk | GPRX_MODER_MODE3_Msk | GPRX_MODER_MODE2_Msk | GPRX_MODER_MODE1_Msk | GPRX_MODER_MODE0_Msk)
 
/******************  Bit definition for GPRx_OTYPER register ********************/
#define GPRX_OTYPER_OT0_Pos                    (0U)
#define GPRX_OTYPER_OT0_Msk                    (0x01U << GPRX_OTYPER_OT0_Pos)        /*!<  */
#define GPRX_OTYPER_OT0                        GPRX_OTYPER_OT0_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT1_Pos                    (1U)
#define GPRX_OTYPER_OT1_Msk                    (0x01U << GPRX_OTYPER_OT1_Pos)        /*!<  */
#define GPRX_OTYPER_OT1                        GPRX_OTYPER_OT1_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT2_Pos                    (2U)
#define GPRX_OTYPER_OT2_Msk                    (0x01U << GPRX_OTYPER_OT2_Pos)        /*!<  */
#define GPRX_OTYPER_OT2                        GPRX_OTYPER_OT2_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT3_Pos                    (3U)
#define GPRX_OTYPER_OT3_Msk                    (0x01U << GPRX_OTYPER_OT3_Pos)        /*!<  */
#define GPRX_OTYPER_OT3                        GPRX_OTYPER_OT3_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT4_Pos                    (4U)
#define GPRX_OTYPER_OT4_Msk                    (0x01U << GPRX_OTYPER_OT4_Pos)        /*!<  */
#define GPRX_OTYPER_OT4                        GPRX_OTYPER_OT4_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT5_Pos                    (5U)
#define GPRX_OTYPER_OT5_Msk                    (0x01U << GPRX_OTYPER_OT5_Pos)        /*!<  */
#define GPRX_OTYPER_OT5                        GPRX_OTYPER_OT5_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT6_Pos                    (6U)
#define GPRX_OTYPER_OT6_Msk                    (0x01U << GPRX_OTYPER_OT6_Pos)        /*!<  */
#define GPRX_OTYPER_OT6                        GPRX_OTYPER_OT6_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT7_Pos                    (7U)
#define GPRX_OTYPER_OT7_Msk                    (0x01U << GPRX_OTYPER_OT7_Pos)        /*!<  */
#define GPRX_OTYPER_OT7                        GPRX_OTYPER_OT7_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT8_Pos                    (8U)
#define GPRX_OTYPER_OT8_Msk                    (0x01U << GPRX_OTYPER_OT8_Pos)        /*!<  */
#define GPRX_OTYPER_OT8                        GPRX_OTYPER_OT8_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT9_Pos                    (9U)
#define GPRX_OTYPER_OT9_Msk                    (0x01U << GPRX_OTYPER_OT9_Pos)        /*!<  */
#define GPRX_OTYPER_OT9                        GPRX_OTYPER_OT9_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT10_Pos                    (10U)
#define GPRX_OTYPER_OT10_Msk                    (0x01U << GPRX_OTYPER_OT10_Pos)        /*!<  */
#define GPRX_OTYPER_OT10                        GPRX_OTYPER_OT10_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT11_Pos                    (11U)
#define GPRX_OTYPER_OT11_Msk                    (0x01U << GPRX_OTYPER_OT11_Pos)        /*!<  */
#define GPRX_OTYPER_OT11                        GPRX_OTYPER_OT11_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT12_Pos                    (12U)
#define GPRX_OTYPER_OT12_Msk                    (0x01U << GPRX_OTYPER_OT12_Pos)        /*!<  */
#define GPRX_OTYPER_OT12                        GPRX_OTYPER_OT12_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT13_Pos                    (13U)
#define GPRX_OTYPER_OT13_Msk                    (0x01U << GPRX_OTYPER_OT13_Pos)        /*!<  */
#define GPRX_OTYPER_OT13                        GPRX_OTYPER_OT13_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT14_Pos                    (14U)
#define GPRX_OTYPER_OT14_Msk                    (0x01U << GPRX_OTYPER_OT14_Pos)        /*!<  */
#define GPRX_OTYPER_OT14                        GPRX_OTYPER_OT14_Msk                   /*!<  */
 
#define GPRX_OTYPER_OT15_Pos                    (15U)
#define GPRX_OTYPER_OT15_Msk                    (0x01U << GPRX_OTYPER_OT15_Pos)        /*!<  */
#define GPRX_OTYPER_OT15                        GPRX_OTYPER_OT15_Msk                   /*!<  */
 
#define GPRX_OTYPER_ALL_Msk                    (GPRX_OTYPER_OT15_Msk | GPRX_OTYPER_OT14_Msk | GPRX_OTYPER_OT13_Msk | GPRX_OTYPER_OT12_Msk | GPRX_OTYPER_OT11_Msk | GPRX_OTYPER_OT10_Msk | GPRX_OTYPER_OT9_Msk | GPRX_OTYPER_OT8_Msk | GPRX_OTYPER_OT7_Msk | GPRX_OTYPER_OT6_Msk | GPRX_OTYPER_OT5_Msk | GPRX_OTYPER_OT4_Msk | GPRX_OTYPER_OT3_Msk | GPRX_OTYPER_OT2_Msk | GPRX_OTYPER_OT1_Msk | GPRX_OTYPER_OT0_Msk)
 
/******************  Bit definition for GPRx_OSPEEDER register ********************/
#define GPRX_OSPEEDER_OS0_Pos                    (0U)
#define GPRX_OSPEEDER_OS0_Msk                    (0x01U << GPRX_OSPEEDER_OS0_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS0                        GPRX_OSPEEDER_OS0_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS1_Pos                    (1U)
#define GPRX_OSPEEDER_OS1_Msk                    (0x01U << GPRX_OSPEEDER_OS1_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS1                        GPRX_OSPEEDER_OS1_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS2_Pos                    (2U)
#define GPRX_OSPEEDER_OS2_Msk                    (0x01U << GPRX_OSPEEDER_OS2_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS2                        GPRX_OSPEEDER_OS2_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS3_Pos                    (3U)
#define GPRX_OSPEEDER_OS3_Msk                    (0x01U << GPRX_OSPEEDER_OS3_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS3                        GPRX_OSPEEDER_OS3_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS4_Pos                    (4U)
#define GPRX_OSPEEDER_OS4_Msk                    (0x01U << GPRX_OSPEEDER_OS4_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS4                        GPRX_OSPEEDER_OS4_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS5_Pos                    (5U)
#define GPRX_OSPEEDER_OS5_Msk                    (0x01U << GPRX_OSPEEDER_OS5_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS5                        GPRX_OSPEEDER_OS5_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS6_Pos                    (6U)
#define GPRX_OSPEEDER_OS6_Msk                    (0x01U << GPRX_OSPEEDER_OS6_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS6                        GPRX_OSPEEDER_OS6_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS7_Pos                    (7U)
#define GPRX_OSPEEDER_OS7_Msk                    (0x01U << GPRX_OSPEEDER_OS7_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS7                        GPRX_OSPEEDER_OS7_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS8_Pos                    (8U)
#define GPRX_OSPEEDER_OS8_Msk                    (0x01U << GPRX_OSPEEDER_OS8_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS8                        GPRX_OSPEEDER_OS8_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS9_Pos                    (9U)
#define GPRX_OSPEEDER_OS9_Msk                    (0x01U << GPRX_OSPEEDER_OS9_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS9                        GPRX_OSPEEDER_OS9_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS10_Pos                    (10U)
#define GPRX_OSPEEDER_OS10_Msk                    (0x01U << GPRX_OSPEEDER_OS10_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS10                        GPRX_OSPEEDER_OS10_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS11_Pos                    (11U)
#define GPRX_OSPEEDER_OS11_Msk                    (0x01U << GPRX_OSPEEDER_OS11_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS11                        GPRX_OSPEEDER_OS11_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS12_Pos                    (12U)
#define GPRX_OSPEEDER_OS12_Msk                    (0x01U << GPRX_OSPEEDER_OS12_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS12                        GPRX_OSPEEDER_OS12_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS13_Pos                    (13U)
#define GPRX_OSPEEDER_OS13_Msk                    (0x01U << GPRX_OSPEEDER_OS13_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS13                        GPRX_OSPEEDER_OS13_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS14_Pos                    (14U)
#define GPRX_OSPEEDER_OS14_Msk                    (0x01U << GPRX_OSPEEDER_OS14_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS14                        GPRX_OSPEEDER_OS14_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_OS15_Pos                    (15U)
#define GPRX_OSPEEDER_OS15_Msk                    (0x01U << GPRX_OSPEEDER_OS15_Pos)        /*!<  */
#define GPRX_OSPEEDER_OS15                        GPRX_OSPEEDER_OS15_Msk                   /*!<  */
 
#define GPRX_OSPEEDER_ALL_Msk                    (GPRX_OSPEEDER_OS15_Msk | GPRX_OSPEEDER_OS14_Msk | GPRX_OSPEEDER_OS13_Msk | GPRX_OSPEEDER_OS12_Msk | GPRX_OSPEEDER_OS11_Msk | GPRX_OSPEEDER_OS10_Msk | GPRX_OSPEEDER_OS9_Msk | GPRX_OSPEEDER_OS8_Msk | GPRX_OSPEEDER_OS7_Msk | GPRX_OSPEEDER_OS6_Msk | GPRX_OSPEEDER_OS5_Msk | GPRX_OSPEEDER_OS4_Msk | GPRX_OSPEEDER_OS3_Msk | GPRX_OSPEEDER_OS2_Msk | GPRX_OSPEEDER_OS1_Msk | GPRX_OSPEEDER_OS0_Msk)
 
/******************  Bit definition for GPRx_PUPDER register ********************/
#define GPRX_PUPDER_PUPD0_Pos                    (0U)
#define GPRX_PUPDER_PUPD0_Msk                    (0x3U << GPRX_PUPDER_PUPD0_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD0                        GPRX_PUPDER_PUPD0_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD1_Pos                    (2U)
#define GPRX_PUPDER_PUPD1_Msk                    (0x3U << GPRX_PUPDER_PUPD1_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD1                        GPRX_PUPDER_PUPD1_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD2_Pos                    (4U)
#define GPRX_PUPDER_PUPD2_Msk                    (0x3U << GPRX_PUPDER_PUPD2_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD2                        GPRX_PUPDER_PUPD2_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD3_Pos                    (6U)
#define GPRX_PUPDER_PUPD3_Msk                    (0x3U << GPRX_PUPDER_PUPD3_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD3                        GPRX_PUPDER_PUPD3_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD4_Pos                    (8U)
#define GPRX_PUPDER_PUPD4_Msk                    (0x3U << GPRX_PUPDER_PUPD4_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD4                        GPRX_PUPDER_PUPD4_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD5_Pos                    (10U)
#define GPRX_PUPDER_PUPD5_Msk                    (0x3U << GPRX_PUPDER_PUPD5_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD5                        GPRX_PUPDER_PUPD5_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD6_Pos                    (12U)
#define GPRX_PUPDER_PUPD6_Msk                    (0x3U << GPRX_PUPDER_PUPD6_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD6                        GPRX_PUPDER_PUPD6_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD7_Pos                    (14U)
#define GPRX_PUPDER_PUPD7_Msk                    (0x3U << GPRX_PUPDER_PUPD7_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD7                        GPRX_PUPDER_PUPD7_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD8_Pos                    (16U)
#define GPRX_PUPDER_PUPD8_Msk                    (0x3U << GPRX_PUPDER_PUPD8_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD8                        GPRX_PUPDER_PUPD8_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD9_Pos                    (18U)
#define GPRX_PUPDER_PUPD9_Msk                    (0x3U << GPRX_PUPDER_PUPD9_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD9                        GPRX_PUPDER_PUPD9_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD10_Pos                    (20U)
#define GPRX_PUPDER_PUPD10_Msk                    (0x3U << GPRX_PUPDER_PUPD10_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD10                        GPRX_PUPDER_PUPD10_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD11_Pos                    (22U)
#define GPRX_PUPDER_PUPD11_Msk                    (0x3U << GPRX_PUPDER_PUPD11_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD11                        GPRX_PUPDER_PUPD11_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD12_Pos                    (24U)
#define GPRX_PUPDER_PUPD12_Msk                    (0x3U << GPRX_PUPDER_PUPD12_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD12                        GPRX_PUPDER_PUPD12_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD13_Pos                    (26U)
#define GPRX_PUPDER_PUPD13_Msk                    (0x3U << GPRX_PUPDER_PUPD13_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD13                        GPRX_PUPDER_PUPD13_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD14_Pos                    (28U)
#define GPRX_PUPDER_PUPD14_Msk                    (0x3U << GPRX_PUPDER_PUPD14_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD14                        GPRX_PUPDER_PUPD14_Msk                   /*!<  */
 
#define GPRX_PUPDER_PUPD15_Pos                    (30U)
#define GPRX_PUPDER_PUPD15_Msk                    (0x3U << GPRX_PUPDER_PUPD15_Pos)        /*!<  */
#define GPRX_PUPDER_PUPD15                        GPRX_PUPDER_PUPD15_Msk                   /*!<  */
 
#define GPRX_PUPDER_ALL_Msk                    (GPRX_PUPDER_PUPD15_Msk | GPRX_PUPDER_PUPD14_Msk | GPRX_PUPDER_PUPD13_Msk | GPRX_PUPDER_PUPD12_Msk | GPRX_PUPDER_PUPD11_Msk | GPRX_PUPDER_PUPD10_Msk | GPRX_PUPDER_PUPD9_Msk | GPRX_PUPDER_PUPD8_Msk | GPRX_PUPDER_PUPD7_Msk | GPRX_PUPDER_PUPD6_Msk | GPRX_PUPDER_PUPD5_Msk | GPRX_PUPDER_PUPD4_Msk | GPRX_PUPDER_PUPD3_Msk | GPRX_PUPDER_PUPD2_Msk | GPRX_PUPDER_PUPD1_Msk | GPRX_PUPDER_PUPD0_Msk)
 
/******************  Bit definition for GPRx_IDR register ********************/
#define GPRX_IDR_IDR0_Pos                    (0U)
#define GPRX_IDR_IDR0_Msk                    (0x01U << GPRX_IDR_IDR0_Pos)        /*!<  */
#define GPRX_IDR_IDR0                        GPRX_IDR_IDR0_Msk                   /*!<  */
 
#define GPRX_IDR_IDR1_Pos                    (1U)
#define GPRX_IDR_IDR1_Msk                    (0x01U << GPRX_IDR_IDR1_Pos)        /*!<  */
#define GPRX_IDR_IDR1                        GPRX_IDR_IDR1_Msk                   /*!<  */
 
#define GPRX_IDR_IDR2_Pos                    (2U)
#define GPRX_IDR_IDR2_Msk                    (0x01U << GPRX_IDR_IDR2_Pos)        /*!<  */
#define GPRX_IDR_IDR2                        GPRX_IDR_IDR2_Msk                   /*!<  */
 
#define GPRX_IDR_IDR3_Pos                    (3U)
#define GPRX_IDR_IDR3_Msk                    (0x01U << GPRX_IDR_IDR3_Pos)        /*!<  */
#define GPRX_IDR_IDR3                        GPRX_IDR_IDR3_Msk                   /*!<  */
 
#define GPRX_IDR_IDR4_Pos                    (4U)
#define GPRX_IDR_IDR4_Msk                    (0x01U << GPRX_IDR_IDR4_Pos)        /*!<  */
#define GPRX_IDR_IDR4                        GPRX_IDR_IDR4_Msk                   /*!<  */
 
#define GPRX_IDR_IDR5_Pos                    (5U)
#define GPRX_IDR_IDR5_Msk                    (0x01U << GPRX_IDR_IDR5_Pos)        /*!<  */
#define GPRX_IDR_IDR5                        GPRX_IDR_IDR5_Msk                   /*!<  */
 
#define GPRX_IDR_IDR6_Pos                    (6U)
#define GPRX_IDR_IDR6_Msk                    (0x01U << GPRX_IDR_IDR6_Pos)        /*!<  */
#define GPRX_IDR_IDR6                        GPRX_IDR_IDR6_Msk                   /*!<  */
 
#define GPRX_IDR_IDR7_Pos                    (7U)
#define GPRX_IDR_IDR7_Msk                    (0x01U << GPRX_IDR_IDR7_Pos)        /*!<  */
#define GPRX_IDR_IDR7                        GPRX_IDR_IDR7_Msk                   /*!<  */
 
#define GPRX_IDR_IDR8_Pos                    (8U)
#define GPRX_IDR_IDR8_Msk                    (0x01U << GPRX_IDR_IDR8_Pos)        /*!<  */
#define GPRX_IDR_IDR8                        GPRX_IDR_IDR8_Msk                   /*!<  */
 
#define GPRX_IDR_IDR9_Pos                    (9U)
#define GPRX_IDR_IDR9_Msk                    (0x01U << GPRX_IDR_IDR9_Pos)        /*!<  */
#define GPRX_IDR_IDR9                        GPRX_IDR_IDR9_Msk                   /*!<  */
 
#define GPRX_IDR_IDR10_Pos                    (10U)
#define GPRX_IDR_IDR10_Msk                    (0x01U << GPRX_IDR_IDR10_Pos)        /*!<  */
#define GPRX_IDR_IDR10                        GPRX_IDR_IDR10_Msk                   /*!<  */
 
#define GPRX_IDR_IDR11_Pos                    (11U)
#define GPRX_IDR_IDR11_Msk                    (0x01U << GPRX_IDR_IDR11_Pos)        /*!<  */
#define GPRX_IDR_IDR11                        GPRX_IDR_IDR11_Msk                   /*!<  */
 
#define GPRX_IDR_IDR12_Pos                    (12U)
#define GPRX_IDR_IDR12_Msk                    (0x01U << GPRX_IDR_IDR12_Pos)        /*!<  */
#define GPRX_IDR_IDR12                        GPRX_IDR_IDR12_Msk                   /*!<  */
 
#define GPRX_IDR_IDR13_Pos                    (13U)
#define GPRX_IDR_IDR13_Msk                    (0x01U << GPRX_IDR_IDR13_Pos)        /*!<  */
#define GPRX_IDR_IDR13                        GPRX_IDR_IDR13_Msk                   /*!<  */
 
#define GPRX_IDR_IDR14_Pos                    (14U)
#define GPRX_IDR_IDR14_Msk                    (0x01U << GPRX_IDR_IDR14_Pos)        /*!<  */
#define GPRX_IDR_IDR14                        GPRX_IDR_IDR14_Msk                   /*!<  */
 
#define GPRX_IDR_IDR15_Pos                    (15U)
#define GPRX_IDR_IDR15_Msk                    (0x01U << GPRX_IDR_IDR15_Pos)        /*!<  */
#define GPRX_IDR_IDR15                        GPRX_IDR_IDR15_Msk                   /*!<  */
 
#define GPRX_IDR_ALL_Msk                    (GPRX_IDR_IDR15_Msk | GPRX_IDR_IDR14_Msk | GPRX_IDR_IDR13_Msk | GPRX_IDR_IDR12_Msk | GPRX_IDR_IDR11_Msk | GPRX_IDR_IDR10_Msk | GPRX_IDR_IDR9_Msk | GPRX_IDR_IDR8_Msk | GPRX_IDR_IDR7_Msk | GPRX_IDR_IDR6_Msk | GPRX_IDR_IDR5_Msk | GPRX_IDR_IDR4_Msk | GPRX_IDR_IDR3_Msk | GPRX_IDR_IDR2_Msk | GPRX_IDR_IDR1_Msk | GPRX_IDR_IDR0_Msk)
 
/******************  Bit definition for GPRx_ODR register ********************/
#define GPRX_ODR_ODR0_Pos                    (0U)
#define GPRX_ODR_ODR0_Msk                    (0x01U << GPRX_ODR_ODR0_Pos)        /*!<  */
#define GPRX_ODR_ODR0                        GPRX_ODR_ODR0_Msk                   /*!<  */
 
#define GPRX_ODR_ODR1_Pos                    (1U)
#define GPRX_ODR_ODR1_Msk                    (0x01U << GPRX_ODR_ODR1_Pos)        /*!<  */
#define GPRX_ODR_ODR1                        GPRX_ODR_ODR1_Msk                   /*!<  */
 
#define GPRX_ODR_ODR2_Pos                    (2U)
#define GPRX_ODR_ODR2_Msk                    (0x01U << GPRX_ODR_ODR2_Pos)        /*!<  */
#define GPRX_ODR_ODR2                        GPRX_ODR_ODR2_Msk                   /*!<  */
 
#define GPRX_ODR_ODR3_Pos                    (3U)
#define GPRX_ODR_ODR3_Msk                    (0x01U << GPRX_ODR_ODR3_Pos)        /*!<  */
#define GPRX_ODR_ODR3                        GPRX_ODR_ODR3_Msk                   /*!<  */
 
#define GPRX_ODR_ODR4_Pos                    (4U)
#define GPRX_ODR_ODR4_Msk                    (0x01U << GPRX_ODR_ODR4_Pos)        /*!<  */
#define GPRX_ODR_ODR4                        GPRX_ODR_ODR4_Msk                   /*!<  */
 
#define GPRX_ODR_ODR5_Pos                    (5U)
#define GPRX_ODR_ODR5_Msk                    (0x01U << GPRX_ODR_ODR5_Pos)        /*!<  */
#define GPRX_ODR_ODR5                        GPRX_ODR_ODR5_Msk                   /*!<  */
 
#define GPRX_ODR_ODR6_Pos                    (6U)
#define GPRX_ODR_ODR6_Msk                    (0x01U << GPRX_ODR_ODR6_Pos)        /*!<  */
#define GPRX_ODR_ODR6                        GPRX_ODR_ODR6_Msk                   /*!<  */
 
#define GPRX_ODR_ODR7_Pos                    (7U)
#define GPRX_ODR_ODR7_Msk                    (0x01U << GPRX_ODR_ODR7_Pos)        /*!<  */
#define GPRX_ODR_ODR7                        GPRX_ODR_ODR7_Msk                   /*!<  */
 
#define GPRX_ODR_ODR8_Pos                    (8U)
#define GPRX_ODR_ODR8_Msk                    (0x01U << GPRX_ODR_ODR8_Pos)        /*!<  */
#define GPRX_ODR_ODR8                        GPRX_ODR_ODR8_Msk                   /*!<  */
 
#define GPRX_ODR_ODR9_Pos                    (9U)
#define GPRX_ODR_ODR9_Msk                    (0x01U << GPRX_ODR_ODR9_Pos)        /*!<  */
#define GPRX_ODR_ODR9                        GPRX_ODR_ODR9_Msk                   /*!<  */
 
#define GPRX_ODR_ODR10_Pos                    (10U)
#define GPRX_ODR_ODR10_Msk                    (0x01U << GPRX_ODR_ODR10_Pos)        /*!<  */
#define GPRX_ODR_ODR10                        GPRX_ODR_ODR10_Msk                   /*!<  */
 
#define GPRX_ODR_ODR11_Pos                    (11U)
#define GPRX_ODR_ODR11_Msk                    (0x01U << GPRX_ODR_ODR11_Pos)        /*!<  */
#define GPRX_ODR_ODR11                        GPRX_ODR_ODR11_Msk                   /*!<  */
 
#define GPRX_ODR_ODR12_Pos                    (12U)
#define GPRX_ODR_ODR12_Msk                    (0x01U << GPRX_ODR_ODR12_Pos)        /*!<  */
#define GPRX_ODR_ODR12                        GPRX_ODR_ODR12_Msk                   /*!<  */
 
#define GPRX_ODR_ODR13_Pos                    (13U)
#define GPRX_ODR_ODR13_Msk                    (0x01U << GPRX_ODR_ODR13_Pos)        /*!<  */
#define GPRX_ODR_ODR13                        GPRX_ODR_ODR13_Msk                   /*!<  */
 
#define GPRX_ODR_ODR14_Pos                    (14U)
#define GPRX_ODR_ODR14_Msk                    (0x01U << GPRX_ODR_ODR14_Pos)        /*!<  */
#define GPRX_ODR_ODR14                        GPRX_ODR_ODR14_Msk                   /*!<  */
 
#define GPRX_ODR_ODR15_Pos                    (15U)
#define GPRX_ODR_ODR15_Msk                    (0x01U << GPRX_ODR_ODR15_Pos)        /*!<  */
#define GPRX_ODR_ODR15                        GPRX_ODR_ODR15_Msk                   /*!<  */
 
#define GPRX_ODR_ALL_Msk                    (GPRX_ODR_ODR15_Msk | GPRX_ODR_ODR14_Msk | GPRX_ODR_ODR13_Msk | GPRX_ODR_ODR12_Msk | GPRX_ODR_ODR11_Msk | GPRX_ODR_ODR10_Msk | GPRX_ODR_ODR9_Msk | GPRX_ODR_ODR8_Msk | GPRX_ODR_ODR7_Msk | GPRX_ODR_ODR6_Msk | GPRX_ODR_ODR5_Msk | GPRX_ODR_ODR4_Msk | GPRX_ODR_ODR3_Msk | GPRX_ODR_ODR2_Msk | GPRX_ODR_ODR1_Msk | GPRX_ODR_ODR0_Msk)
 
/******************  Bit definition for GPRx_BSRR register ********************/
#define GPRX_BSRR_IOS0_Pos                    (0U)
#define GPRX_BSRR_IOS0_Msk                    (0x01U << GPRX_BSRR_IOS0_Pos)        /*!<  */
#define GPRX_BSRR_IOS0                        GPRX_BSRR_IOS0_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS1_Pos                    (1U)
#define GPRX_BSRR_IOS1_Msk                    (0x01U << GPRX_BSRR_IOS1_Pos)        /*!<  */
#define GPRX_BSRR_IOS1                        GPRX_BSRR_IOS1_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS2_Pos                    (2U)
#define GPRX_BSRR_IOS2_Msk                    (0x01U << GPRX_BSRR_IOS2_Pos)        /*!<  */
#define GPRX_BSRR_IOS2                        GPRX_BSRR_IOS2_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS3_Pos                    (3U)
#define GPRX_BSRR_IOS3_Msk                    (0x01U << GPRX_BSRR_IOS3_Pos)        /*!<  */
#define GPRX_BSRR_IOS3                        GPRX_BSRR_IOS3_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS4_Pos                    (4U)
#define GPRX_BSRR_IOS4_Msk                    (0x01U << GPRX_BSRR_IOS4_Pos)        /*!<  */
#define GPRX_BSRR_IOS4                        GPRX_BSRR_IOS4_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS5_Pos                    (5U)
#define GPRX_BSRR_IOS5_Msk                    (0x01U << GPRX_BSRR_IOS5_Pos)        /*!<  */
#define GPRX_BSRR_IOS5                        GPRX_BSRR_IOS5_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS6_Pos                    (6U)
#define GPRX_BSRR_IOS6_Msk                    (0x01U << GPRX_BSRR_IOS6_Pos)        /*!<  */
#define GPRX_BSRR_IOS6                        GPRX_BSRR_IOS6_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS7_Pos                    (7U)
#define GPRX_BSRR_IOS7_Msk                    (0x01U << GPRX_BSRR_IOS7_Pos)        /*!<  */
#define GPRX_BSRR_IOS7                        GPRX_BSRR_IOS7_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS8_Pos                    (8U)
#define GPRX_BSRR_IOS8_Msk                    (0x01U << GPRX_BSRR_IOS8_Pos)        /*!<  */
#define GPRX_BSRR_IOS8                        GPRX_BSRR_IOS8_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS9_Pos                    (9U)
#define GPRX_BSRR_IOS9_Msk                    (0x01U << GPRX_BSRR_IOS9_Pos)        /*!<  */
#define GPRX_BSRR_IOS9                        GPRX_BSRR_IOS9_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS10_Pos                    (10U)
#define GPRX_BSRR_IOS10_Msk                    (0x01U << GPRX_BSRR_IOS10_Pos)        /*!<  */
#define GPRX_BSRR_IOS10                        GPRX_BSRR_IOS10_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS11_Pos                    (11U)
#define GPRX_BSRR_IOS11_Msk                    (0x01U << GPRX_BSRR_IOS11_Pos)        /*!<  */
#define GPRX_BSRR_IOS11                        GPRX_BSRR_IOS11_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS12_Pos                    (12U)
#define GPRX_BSRR_IOS12_Msk                    (0x01U << GPRX_BSRR_IOS12_Pos)        /*!<  */
#define GPRX_BSRR_IOS12                        GPRX_BSRR_IOS12_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS13_Pos                    (13U)
#define GPRX_BSRR_IOS13_Msk                    (0x01U << GPRX_BSRR_IOS13_Pos)        /*!<  */
#define GPRX_BSRR_IOS13                        GPRX_BSRR_IOS13_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS14_Pos                    (14U)
#define GPRX_BSRR_IOS14_Msk                    (0x01U << GPRX_BSRR_IOS14_Pos)        /*!<  */
#define GPRX_BSRR_IOS14                        GPRX_BSRR_IOS14_Msk                   /*!<  */
 
#define GPRX_BSRR_IOS15_Pos                    (15U)
#define GPRX_BSRR_IOS15_Msk                    (0x01U << GPRX_BSRR_IOS15_Pos)        /*!<  */
#define GPRX_BSRR_IOS15                        GPRX_BSRR_IOS15_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC0_Pos                    (16U)
#define GPRX_BSRR_IOC0_Msk                    (0x01U << GPRX_BSRR_IOC0_Pos)        /*!<  */
#define GPRX_BSRR_IOC0                        GPRX_BSRR_IOC0_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC1_Pos                    (17U)
#define GPRX_BSRR_IOC1_Msk                    (0x01U << GPRX_BSRR_IOC1_Pos)        /*!<  */
#define GPRX_BSRR_IOC1                        GPRX_BSRR_IOC1_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC2_Pos                    (18U)
#define GPRX_BSRR_IOC2_Msk                    (0x01U << GPRX_BSRR_IOC2_Pos)        /*!<  */
#define GPRX_BSRR_IOC2                        GPRX_BSRR_IOC2_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC3_Pos                    (19U)
#define GPRX_BSRR_IOC3_Msk                    (0x01U << GPRX_BSRR_IOC3_Pos)        /*!<  */
#define GPRX_BSRR_IOC3                        GPRX_BSRR_IOC3_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC4_Pos                    (20U)
#define GPRX_BSRR_IOC4_Msk                    (0x01U << GPRX_BSRR_IOC4_Pos)        /*!<  */
#define GPRX_BSRR_IOC4                        GPRX_BSRR_IOC4_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC5_Pos                    (21U)
#define GPRX_BSRR_IOC5_Msk                    (0x01U << GPRX_BSRR_IOC5_Pos)        /*!<  */
#define GPRX_BSRR_IOC5                        GPRX_BSRR_IOC5_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC6_Pos                    (22U)
#define GPRX_BSRR_IOC6_Msk                    (0x01U << GPRX_BSRR_IOC6_Pos)        /*!<  */
#define GPRX_BSRR_IOC6                        GPRX_BSRR_IOC6_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC7_Pos                    (23U)
#define GPRX_BSRR_IOC7_Msk                    (0x01U << GPRX_BSRR_IOC7_Pos)        /*!<  */
#define GPRX_BSRR_IOC7                        GPRX_BSRR_IOC7_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC8_Pos                    (24U)
#define GPRX_BSRR_IOC8_Msk                    (0x01U << GPRX_BSRR_IOC8_Pos)        /*!<  */
#define GPRX_BSRR_IOC8                        GPRX_BSRR_IOC8_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC9_Pos                    (25U)
#define GPRX_BSRR_IOC9_Msk                    (0x01U << GPRX_BSRR_IOC9_Pos)        /*!<  */
#define GPRX_BSRR_IOC9                        GPRX_BSRR_IOC9_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC10_Pos                    (26U)
#define GPRX_BSRR_IOC10_Msk                    (0x01U << GPRX_BSRR_IOC10_Pos)        /*!<  */
#define GPRX_BSRR_IOC10                        GPRX_BSRR_IOC10_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC11_Pos                    (27U)
#define GPRX_BSRR_IOC11_Msk                    (0x01U << GPRX_BSRR_IOC11_Pos)        /*!<  */
#define GPRX_BSRR_IOC11                        GPRX_BSRR_IOC11_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC12_Pos                    (28U)
#define GPRX_BSRR_IOC12_Msk                    (0x01U << GPRX_BSRR_IOC12_Pos)        /*!<  */
#define GPRX_BSRR_IOC12                        GPRX_BSRR_IOC12_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC13_Pos                    (29U)
#define GPRX_BSRR_IOC13_Msk                    (0x01U << GPRX_BSRR_IOC13_Pos)        /*!<  */
#define GPRX_BSRR_IOC13                        GPRX_BSRR_IOC13_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC14_Pos                    (30U)
#define GPRX_BSRR_IOC14_Msk                    (0x01U << GPRX_BSRR_IOC14_Pos)        /*!<  */
#define GPRX_BSRR_IOC14                        GPRX_BSRR_IOC14_Msk                   /*!<  */
 
#define GPRX_BSRR_IOC15_Pos                    (31U)
#define GPRX_BSRR_IOC15_Msk                    (0x01U << GPRX_BSRR_IOC15_Pos)        /*!<  */
#define GPRX_BSRR_IOC15                        GPRX_BSRR_IOC15_Msk                   /*!<  */
 
#define GPRX_BSRR_ALL_Msk                    (GPRX_BSRR_IOC15_Msk | GPRX_BSRR_IOC14_Msk | GPRX_BSRR_IOC13_Msk | GPRX_BSRR_IOC12_Msk | GPRX_BSRR_IOC11_Msk | GPRX_BSRR_IOC10_Msk | GPRX_BSRR_IOC9_Msk | GPRX_BSRR_IOC8_Msk | GPRX_BSRR_IOC7_Msk | GPRX_BSRR_IOC6_Msk | GPRX_BSRR_IOC5_Msk | GPRX_BSRR_IOC4_Msk | GPRX_BSRR_IOC3_Msk | GPRX_BSRR_IOC2_Msk | GPRX_BSRR_IOC1_Msk | GPRX_BSRR_IOC0_Msk | GPRX_BSRR_IOS15_Msk | GPRX_BSRR_IOS14_Msk | GPRX_BSRR_IOS13_Msk | GPRX_BSRR_IOS12_Msk | GPRX_BSRR_IOS11_Msk | GPRX_BSRR_IOS10_Msk | GPRX_BSRR_IOS9_Msk | GPRX_BSRR_IOS8_Msk | GPRX_BSRR_IOS7_Msk | GPRX_BSRR_IOS6_Msk | GPRX_BSRR_IOS5_Msk | GPRX_BSRR_IOS4_Msk | GPRX_BSRR_IOS3_Msk | GPRX_BSRR_IOS2_Msk | GPRX_BSRR_IOS1_Msk | GPRX_BSRR_IOS0_Msk)
 
/******************  Bit definition for GPRx_LCKR register ********************/
#define GPRX_LCKR_LCK0_Pos                    (0U)
#define GPRX_LCKR_LCK0_Msk                    (0x01U << GPRX_LCKR_LCK0_Pos)        /*!<  */
#define GPRX_LCKR_LCK0                        GPRX_LCKR_LCK0_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK1_Pos                    (1U)
#define GPRX_LCKR_LCK1_Msk                    (0x01U << GPRX_LCKR_LCK1_Pos)        /*!<  */
#define GPRX_LCKR_LCK1                        GPRX_LCKR_LCK1_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK2_Pos                    (2U)
#define GPRX_LCKR_LCK2_Msk                    (0x01U << GPRX_LCKR_LCK2_Pos)        /*!<  */
#define GPRX_LCKR_LCK2                        GPRX_LCKR_LCK2_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK3_Pos                    (3U)
#define GPRX_LCKR_LCK3_Msk                    (0x01U << GPRX_LCKR_LCK3_Pos)        /*!<  */
#define GPRX_LCKR_LCK3                        GPRX_LCKR_LCK3_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK4_Pos                    (4U)
#define GPRX_LCKR_LCK4_Msk                    (0x01U << GPRX_LCKR_LCK4_Pos)        /*!<  */
#define GPRX_LCKR_LCK4                        GPRX_LCKR_LCK4_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK5_Pos                    (5U)
#define GPRX_LCKR_LCK5_Msk                    (0x01U << GPRX_LCKR_LCK5_Pos)        /*!<  */
#define GPRX_LCKR_LCK5                        GPRX_LCKR_LCK5_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK6_Pos                    (6U)
#define GPRX_LCKR_LCK6_Msk                    (0x01U << GPRX_LCKR_LCK6_Pos)        /*!<  */
#define GPRX_LCKR_LCK6                        GPRX_LCKR_LCK6_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK7_Pos                    (7U)
#define GPRX_LCKR_LCK7_Msk                    (0x01U << GPRX_LCKR_LCK7_Pos)        /*!<  */
#define GPRX_LCKR_LCK7                        GPRX_LCKR_LCK7_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK8_Pos                    (8U)
#define GPRX_LCKR_LCK8_Msk                    (0x01U << GPRX_LCKR_LCK8_Pos)        /*!<  */
#define GPRX_LCKR_LCK8                        GPRX_LCKR_LCK8_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK9_Pos                    (9U)
#define GPRX_LCKR_LCK9_Msk                    (0x01U << GPRX_LCKR_LCK9_Pos)        /*!<  */
#define GPRX_LCKR_LCK9                        GPRX_LCKR_LCK9_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK10_Pos                    (10U)
#define GPRX_LCKR_LCK10_Msk                    (0x01U << GPRX_LCKR_LCK10_Pos)        /*!<  */
#define GPRX_LCKR_LCK10                        GPRX_LCKR_LCK10_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK11_Pos                    (11U)
#define GPRX_LCKR_LCK11_Msk                    (0x01U << GPRX_LCKR_LCK11_Pos)        /*!<  */
#define GPRX_LCKR_LCK11                        GPRX_LCKR_LCK11_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK12_Pos                    (12U)
#define GPRX_LCKR_LCK12_Msk                    (0x01U << GPRX_LCKR_LCK12_Pos)        /*!<  */
#define GPRX_LCKR_LCK12                        GPRX_LCKR_LCK12_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK13_Pos                    (13U)
#define GPRX_LCKR_LCK13_Msk                    (0x01U << GPRX_LCKR_LCK13_Pos)        /*!<  */
#define GPRX_LCKR_LCK13                        GPRX_LCKR_LCK13_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK14_Pos                    (14U)
#define GPRX_LCKR_LCK14_Msk                    (0x01U << GPRX_LCKR_LCK14_Pos)        /*!<  */
#define GPRX_LCKR_LCK14                        GPRX_LCKR_LCK14_Msk                   /*!<  */
 
#define GPRX_LCKR_LCK15_Pos                    (15U)
#define GPRX_LCKR_LCK15_Msk                    (0x01U << GPRX_LCKR_LCK15_Pos)        /*!<  */
#define GPRX_LCKR_LCK15                        GPRX_LCKR_LCK15_Msk                   /*!<  */
 
#define GPRX_LCKR_LOCK_Pos                    (16U)
#define GPRX_LCKR_LOCK_Msk                    (0x01U << GPRX_LCKR_LOCK_Pos)        /*!<  */
#define GPRX_LCKR_LOCK                        GPRX_LCKR_LOCK_Msk                   /*!<  */
 
#define GPRX_LCKR_ALL_Msk                    (GPRX_LCKR_LOCK_Msk | GPRX_LCKR_LCK15_Msk | GPRX_LCKR_LCK14_Msk | GPRX_LCKR_LCK13_Msk | GPRX_LCKR_LCK12_Msk | GPRX_LCKR_LCK11_Msk | GPRX_LCKR_LCK10_Msk | GPRX_LCKR_LCK9_Msk | GPRX_LCKR_LCK8_Msk | GPRX_LCKR_LCK7_Msk | GPRX_LCKR_LCK6_Msk | GPRX_LCKR_LCK5_Msk | GPRX_LCKR_LCK4_Msk | GPRX_LCKR_LCK3_Msk | GPRX_LCKR_LCK2_Msk | GPRX_LCKR_LCK1_Msk | GPRX_LCKR_LCK0_Msk)
 
/******************  Bit definition for GPRx_AFRL register ********************/
#define GPRX_AFRL_AFR0_Pos                    (0U)
#define GPRX_AFRL_AFR0_Msk                    (0xFU << GPRX_AFRL_AFR0_Pos)        /*!<  */
#define GPRX_AFRL_AFR0                        GPRX_AFRL_AFR0_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR1_Pos                    (4U)
#define GPRX_AFRL_AFR1_Msk                    (0xFU << GPRX_AFRL_AFR1_Pos)        /*!<  */
#define GPRX_AFRL_AFR1                        GPRX_AFRL_AFR1_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR2_Pos                    (8U)
#define GPRX_AFRL_AFR2_Msk                    (0xFU << GPRX_AFRL_AFR2_Pos)        /*!<  */
#define GPRX_AFRL_AFR2                        GPRX_AFRL_AFR2_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR3_Pos                    (12U)
#define GPRX_AFRL_AFR3_Msk                    (0xFU << GPRX_AFRL_AFR3_Pos)        /*!<  */
#define GPRX_AFRL_AFR3                        GPRX_AFRL_AFR3_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR4_Pos                    (16U)
#define GPRX_AFRL_AFR4_Msk                    (0xFU << GPRX_AFRL_AFR4_Pos)        /*!<  */
#define GPRX_AFRL_AFR4                        GPRX_AFRL_AFR4_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR5_Pos                    (20U)
#define GPRX_AFRL_AFR5_Msk                    (0xFU << GPRX_AFRL_AFR5_Pos)        /*!<  */
#define GPRX_AFRL_AFR5                        GPRX_AFRL_AFR5_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR6_Pos                    (24U)
#define GPRX_AFRL_AFR6_Msk                    (0xFU << GPRX_AFRL_AFR6_Pos)        /*!<  */
#define GPRX_AFRL_AFR6                        GPRX_AFRL_AFR6_Msk                   /*!<  */
 
#define GPRX_AFRL_AFR7_Pos                    (28U)
#define GPRX_AFRL_AFR7_Msk                    (0xFU << GPRX_AFRL_AFR7_Pos)        /*!<  */
#define GPRX_AFRL_AFR7                        GPRX_AFRL_AFR7_Msk                   /*!<  */
 
#define GPRX_AFRL_ALL_Msk                    (GPRX_AFRL_AFR7_Msk | GPRX_AFRL_AFR6_Msk | GPRX_AFRL_AFR5_Msk | GPRX_AFRL_AFR4_Msk | GPRX_AFRL_AFR3_Msk | GPRX_AFRL_AFR2_Msk | GPRX_AFRL_AFR1_Msk | GPRX_AFRL_AFR0_Msk)
 
/******************  Bit definition for GPRx_AFRH register ********************/
#define GPRX_AFRH_AFR8_Pos                    (0U)
#define GPRX_AFRH_AFR8_Msk                    (0xFU << GPRX_AFRH_AFR8_Pos)        /*!<  */
#define GPRX_AFRH_AFR8                        GPRX_AFRH_AFR8_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR9_Pos                    (4U)
#define GPRX_AFRH_AFR9_Msk                    (0xFU << GPRX_AFRH_AFR9_Pos)        /*!<  */
#define GPRX_AFRH_AFR9                        GPRX_AFRH_AFR9_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR10_Pos                    (8U)
#define GPRX_AFRH_AFR10_Msk                    (0xFU << GPRX_AFRH_AFR10_Pos)        /*!<  */
#define GPRX_AFRH_AFR10                        GPRX_AFRH_AFR10_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR11_Pos                    (12U)
#define GPRX_AFRH_AFR11_Msk                    (0xFU << GPRX_AFRH_AFR11_Pos)        /*!<  */
#define GPRX_AFRH_AFR11                        GPRX_AFRH_AFR11_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR12_Pos                    (16U)
#define GPRX_AFRH_AFR12_Msk                    (0xFU << GPRX_AFRH_AFR12_Pos)        /*!<  */
#define GPRX_AFRH_AFR12                        GPRX_AFRH_AFR12_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR13_Pos                    (20U)
#define GPRX_AFRH_AFR13_Msk                    (0xFU << GPRX_AFRH_AFR13_Pos)        /*!<  */
#define GPRX_AFRH_AFR13                        GPRX_AFRH_AFR13_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR14_Pos                    (24U)
#define GPRX_AFRH_AFR14_Msk                    (0xFU << GPRX_AFRH_AFR14_Pos)        /*!<  */
#define GPRX_AFRH_AFR14                        GPRX_AFRH_AFR14_Msk                   /*!<  */
 
#define GPRX_AFRH_AFR15_Pos                    (28U)
#define GPRX_AFRH_AFR15_Msk                    (0xFU << GPRX_AFRH_AFR15_Pos)        /*!<  */
#define GPRX_AFRH_AFR15                        GPRX_AFRH_AFR15_Msk                   /*!<  */
 
#define GPRX_AFRH_ALL_Msk                    (GPRX_AFRH_AFR15_Msk | GPRX_AFRH_AFR14_Msk | GPRX_AFRH_AFR13_Msk | GPRX_AFRH_AFR12_Msk | GPRX_AFRH_AFR11_Msk | GPRX_AFRH_AFR10_Msk | GPRX_AFRH_AFR9_Msk | GPRX_AFRH_AFR8_Msk)
 
/******************  Bit definition for GPRx_BRR register ********************/
#define GPRX_BRR_IOC0_Pos                    (0U)
#define GPRX_BRR_IOC0_Msk                    (0x01U << GPRX_BRR_IOC0_Pos)        /*!<  */
#define GPRX_BRR_IOC0                        GPRX_BRR_IOC0_Msk                   /*!<  */
 
#define GPRX_BRR_IOC1_Pos                    (1U)
#define GPRX_BRR_IOC1_Msk                    (0x01U << GPRX_BRR_IOC1_Pos)        /*!<  */
#define GPRX_BRR_IOC1                        GPRX_BRR_IOC1_Msk                   /*!<  */
 
#define GPRX_BRR_IOC2_Pos                    (2U)
#define GPRX_BRR_IOC2_Msk                    (0x01U << GPRX_BRR_IOC2_Pos)        /*!<  */
#define GPRX_BRR_IOC2                        GPRX_BRR_IOC2_Msk                   /*!<  */
 
#define GPRX_BRR_IOC3_Pos                    (3U)
#define GPRX_BRR_IOC3_Msk                    (0x01U << GPRX_BRR_IOC3_Pos)        /*!<  */
#define GPRX_BRR_IOC3                        GPRX_BRR_IOC3_Msk                   /*!<  */
 
#define GPRX_BRR_IOC4_Pos                    (4U)
#define GPRX_BRR_IOC4_Msk                    (0x01U << GPRX_BRR_IOC4_Pos)        /*!<  */
#define GPRX_BRR_IOC4                        GPRX_BRR_IOC4_Msk                   /*!<  */
 
#define GPRX_BRR_IOC5_Pos                    (5U)
#define GPRX_BRR_IOC5_Msk                    (0x01U << GPRX_BRR_IOC5_Pos)        /*!<  */
#define GPRX_BRR_IOC5                        GPRX_BRR_IOC5_Msk                   /*!<  */
 
#define GPRX_BRR_IOC6_Pos                    (6U)
#define GPRX_BRR_IOC6_Msk                    (0x01U << GPRX_BRR_IOC6_Pos)        /*!<  */
#define GPRX_BRR_IOC6                        GPRX_BRR_IOC6_Msk                   /*!<  */
 
#define GPRX_BRR_IOC7_Pos                    (7U)
#define GPRX_BRR_IOC7_Msk                    (0x01U << GPRX_BRR_IOC7_Pos)        /*!<  */
#define GPRX_BRR_IOC7                        GPRX_BRR_IOC7_Msk                   /*!<  */
 
#define GPRX_BRR_IOC8_Pos                    (8U)
#define GPRX_BRR_IOC8_Msk                    (0x01U << GPRX_BRR_IOC8_Pos)        /*!<  */
#define GPRX_BRR_IOC8                        GPRX_BRR_IOC8_Msk                   /*!<  */
 
#define GPRX_BRR_IOC9_Pos                    (9U)
#define GPRX_BRR_IOC9_Msk                    (0x01U << GPRX_BRR_IOC9_Pos)        /*!<  */
#define GPRX_BRR_IOC9                        GPRX_BRR_IOC9_Msk                   /*!<  */
 
#define GPRX_BRR_IOC10_Pos                    (10U)
#define GPRX_BRR_IOC10_Msk                    (0x01U << GPRX_BRR_IOC10_Pos)        /*!<  */
#define GPRX_BRR_IOC10                        GPRX_BRR_IOC10_Msk                   /*!<  */
 
#define GPRX_BRR_IOC11_Pos                    (11U)
#define GPRX_BRR_IOC11_Msk                    (0x01U << GPRX_BRR_IOC11_Pos)        /*!<  */
#define GPRX_BRR_IOC11                        GPRX_BRR_IOC11_Msk                   /*!<  */
 
#define GPRX_BRR_IOC12_Pos                    (12U)
#define GPRX_BRR_IOC12_Msk                    (0x01U << GPRX_BRR_IOC12_Pos)        /*!<  */
#define GPRX_BRR_IOC12                        GPRX_BRR_IOC12_Msk                   /*!<  */
 
#define GPRX_BRR_IOC13_Pos                    (13U)
#define GPRX_BRR_IOC13_Msk                    (0x01U << GPRX_BRR_IOC13_Pos)        /*!<  */
#define GPRX_BRR_IOC13                        GPRX_BRR_IOC13_Msk                   /*!<  */
 
#define GPRX_BRR_IOC14_Pos                    (14U)
#define GPRX_BRR_IOC14_Msk                    (0x01U << GPRX_BRR_IOC14_Pos)        /*!<  */
#define GPRX_BRR_IOC14                        GPRX_BRR_IOC14_Msk                   /*!<  */
 
#define GPRX_BRR_IOC15_Pos                    (15U)
#define GPRX_BRR_IOC15_Msk                    (0x01U << GPRX_BRR_IOC15_Pos)        /*!<  */
#define GPRX_BRR_IOC15                        GPRX_BRR_IOC15_Msk                   /*!<  */
 
#define GPRX_BRR_ALL_Msk                    (GPRX_BRR_IOC15_Msk | GPRX_BRR_IOC14_Msk | GPRX_BRR_IOC13_Msk | GPRX_BRR_IOC12_Msk | GPRX_BRR_IOC11_Msk | GPRX_BRR_IOC10_Msk | GPRX_BRR_IOC9_Msk | GPRX_BRR_IOC8_Msk | GPRX_BRR_IOC7_Msk | GPRX_BRR_IOC6_Msk | GPRX_BRR_IOC5_Msk | GPRX_BRR_IOC4_Msk | GPRX_BRR_IOC3_Msk | GPRX_BRR_IOC2_Msk | GPRX_BRR_IOC1_Msk | GPRX_BRR_IOC0_Msk)
 
/******************  Bit definition for GPRx_SR register ********************/
#define GPRX_SR_SR0_Pos                    (0U)
#define GPRX_SR_SR0_Msk                    (0x01U << GPRX_SR_SR0_Pos)        /*!<  */
#define GPRX_SR_SR0                        GPRX_SR_SR0_Msk                   /*!<  */
 
#define GPRX_SR_SR1_Pos                    (1U)
#define GPRX_SR_SR1_Msk                    (0x01U << GPRX_SR_SR1_Pos)        /*!<  */
#define GPRX_SR_SR1                        GPRX_SR_SR1_Msk                   /*!<  */
 
#define GPRX_SR_SR2_Pos                    (2U)
#define GPRX_SR_SR2_Msk                    (0x01U << GPRX_SR_SR2_Pos)        /*!<  */
#define GPRX_SR_SR2                        GPRX_SR_SR2_Msk                   /*!<  */
 
#define GPRX_SR_SR3_Pos                    (3U)
#define GPRX_SR_SR3_Msk                    (0x01U << GPRX_SR_SR3_Pos)        /*!<  */
#define GPRX_SR_SR3                        GPRX_SR_SR3_Msk                   /*!<  */
 
#define GPRX_SR_SR4_Pos                    (4U)
#define GPRX_SR_SR4_Msk                    (0x01U << GPRX_SR_SR4_Pos)        /*!<  */
#define GPRX_SR_SR4                        GPRX_SR_SR4_Msk                   /*!<  */
 
#define GPRX_SR_SR5_Pos                    (5U)
#define GPRX_SR_SR5_Msk                    (0x01U << GPRX_SR_SR5_Pos)        /*!<  */
#define GPRX_SR_SR5                        GPRX_SR_SR5_Msk                   /*!<  */
 
#define GPRX_SR_SR6_Pos                    (6U)
#define GPRX_SR_SR6_Msk                    (0x01U << GPRX_SR_SR6_Pos)        /*!<  */
#define GPRX_SR_SR6                        GPRX_SR_SR6_Msk                   /*!<  */
 
#define GPRX_SR_SR7_Pos                    (7U)
#define GPRX_SR_SR7_Msk                    (0x01U << GPRX_SR_SR7_Pos)        /*!<  */
#define GPRX_SR_SR7                        GPRX_SR_SR7_Msk                   /*!<  */
 
#define GPRX_SR_SR8_Pos                    (8U)
#define GPRX_SR_SR8_Msk                    (0x01U << GPRX_SR_SR8_Pos)        /*!<  */
#define GPRX_SR_SR8                        GPRX_SR_SR8_Msk                   /*!<  */
 
#define GPRX_SR_SR9_Pos                    (9U)
#define GPRX_SR_SR9_Msk                    (0x01U << GPRX_SR_SR9_Pos)        /*!<  */
#define GPRX_SR_SR9                        GPRX_SR_SR9_Msk                   /*!<  */
 
#define GPRX_SR_SR10_Pos                    (10U)
#define GPRX_SR_SR10_Msk                    (0x01U << GPRX_SR_SR10_Pos)        /*!<  */
#define GPRX_SR_SR10                        GPRX_SR_SR10_Msk                   /*!<  */
 
#define GPRX_SR_SR11_Pos                    (11U)
#define GPRX_SR_SR11_Msk                    (0x01U << GPRX_SR_SR11_Pos)        /*!<  */
#define GPRX_SR_SR11                        GPRX_SR_SR11_Msk                   /*!<  */
 
#define GPRX_SR_SR12_Pos                    (12U)
#define GPRX_SR_SR12_Msk                    (0x01U << GPRX_SR_SR12_Pos)        /*!<  */
#define GPRX_SR_SR12                        GPRX_SR_SR12_Msk                   /*!<  */
 
#define GPRX_SR_SR13_Pos                    (13U)
#define GPRX_SR_SR13_Msk                    (0x01U << GPRX_SR_SR13_Pos)        /*!<  */
#define GPRX_SR_SR13                        GPRX_SR_SR13_Msk                   /*!<  */
 
#define GPRX_SR_SR14_Pos                    (14U)
#define GPRX_SR_SR14_Msk                    (0x01U << GPRX_SR_SR14_Pos)        /*!<  */
#define GPRX_SR_SR14                        GPRX_SR_SR14_Msk                   /*!<  */
 
#define GPRX_SR_SR15_Pos                    (15U)
#define GPRX_SR_SR15_Msk                    (0x01U << GPRX_SR_SR15_Pos)        /*!<  */
#define GPRX_SR_SR15                        GPRX_SR_SR15_Msk                   /*!<  */
 
#define GPRX_SR_ALL_Msk                    (GPRX_SR_SR15_Msk | GPRX_SR_SR14_Msk | GPRX_SR_SR13_Msk | GPRX_SR_SR12_Msk | GPRX_SR_SR11_Msk | GPRX_SR_SR10_Msk | GPRX_SR_SR9_Msk | GPRX_SR_SR8_Msk | GPRX_SR_SR7_Msk | GPRX_SR_SR6_Msk | GPRX_SR_SR5_Msk | GPRX_SR_SR4_Msk | GPRX_SR_SR3_Msk | GPRX_SR_SR2_Msk | GPRX_SR_SR1_Msk | GPRX_SR_SR0_Msk)
 
/******************  Bit definition for SYSCFG_CFGR1 register ********************/
#define SYSCFG_CFGR1_REMAP_Pos                    (0U)
#define SYSCFG_CFGR1_REMAP_Msk                    (0x7U << SYSCFG_CFGR1_REMAP_Pos)        /*!<  */
#define SYSCFG_CFGR1_REMAP                        SYSCFG_CFGR1_REMAP_Msk                   /*!<  */
 
#define SYSCFG_CFGR1_LOCKUP_RSTEN_Pos                    (16U)
#define SYSCFG_CFGR1_LOCKUP_RSTEN_Msk                    (0x01U << SYSCFG_CFGR1_LOCKUP_RSTEN_Pos)        /*!<  */
#define SYSCFG_CFGR1_LOCKUP_RSTEN                        SYSCFG_CFGR1_LOCKUP_RSTEN_Msk                   /*!<  */
 
#define SYSCFG_CFGR1_ALL_Msk                    (SYSCFG_CFGR1_LOCKUP_RSTEN_Msk | SYSCFG_CFGR1_REMAP_Msk)
 
/******************  Bit definition for SYSCFG_CFGR2 register ********************/
#define SYSCFG_CFGR2_BREAKEN_Pos                    (0U)
#define SYSCFG_CFGR2_BREAKEN_Msk                    (0x7U << SYSCFG_CFGR2_BREAKEN_Pos)        /*!<  */
#define SYSCFG_CFGR2_BREAKEN                        SYSCFG_CFGR2_BREAKEN_Msk                   /*!<  */
 
#define SYSCFG_CFGR2_MASTER_CHK_EN_Pos                    (4U)
#define SYSCFG_CFGR2_MASTER_CHK_EN_Msk                    (0xFU << SYSCFG_CFGR2_MASTER_CHK_EN_Pos)        /*!<  */
#define SYSCFG_CFGR2_MASTER_CHK_EN                        SYSCFG_CFGR2_MASTER_CHK_EN_Msk                   /*!<  */
 
#define SYSCFG_CFGR2_ALL_Msk                    (SYSCFG_CFGR2_MASTER_CHK_EN_Msk | SYSCFG_CFGR2_BREAKEN_Msk)
 
/******************  Bit definition for SYSCFG_EXTICR1 register ********************/
#define SYSCFG_EXTICR1_EXTI0_SEL_Pos                    (0U)
#define SYSCFG_EXTICR1_EXTI0_SEL_Msk                    (0xFU << SYSCFG_EXTICR1_EXTI0_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR1_EXTI0_SEL                        SYSCFG_EXTICR1_EXTI0_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR1_EXTI1_SEL_Pos                    (4U)
#define SYSCFG_EXTICR1_EXTI1_SEL_Msk                    (0xFU << SYSCFG_EXTICR1_EXTI1_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR1_EXTI1_SEL                        SYSCFG_EXTICR1_EXTI1_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR1_EXTI2_SEL_Pos                    (8U)
#define SYSCFG_EXTICR1_EXTI2_SEL_Msk                    (0xFU << SYSCFG_EXTICR1_EXTI2_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR1_EXTI2_SEL                        SYSCFG_EXTICR1_EXTI2_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR1_EXTI3_SEL_Pos                    (12U)
#define SYSCFG_EXTICR1_EXTI3_SEL_Msk                    (0xFU << SYSCFG_EXTICR1_EXTI3_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR1_EXTI3_SEL                        SYSCFG_EXTICR1_EXTI3_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR1_ALL_Msk                    (SYSCFG_EXTICR1_EXTI3_SEL_Msk | SYSCFG_EXTICR1_EXTI2_SEL_Msk | SYSCFG_EXTICR1_EXTI1_SEL_Msk | SYSCFG_EXTICR1_EXTI0_SEL_Msk)
 
/******************  Bit definition for SYSCFG_EXTICR2 register ********************/
#define SYSCFG_EXTICR2_EXTI4_SEL_Pos                    (0U)
#define SYSCFG_EXTICR2_EXTI4_SEL_Msk                    (0xFU << SYSCFG_EXTICR2_EXTI4_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR2_EXTI4_SEL                        SYSCFG_EXTICR2_EXTI4_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR2_EXTI5_SEL_Pos                    (4U)
#define SYSCFG_EXTICR2_EXTI5_SEL_Msk                    (0xFU << SYSCFG_EXTICR2_EXTI5_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR2_EXTI5_SEL                        SYSCFG_EXTICR2_EXTI5_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR2_EXTI6_SEL_Pos                    (8U)
#define SYSCFG_EXTICR2_EXTI6_SEL_Msk                    (0xFU << SYSCFG_EXTICR2_EXTI6_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR2_EXTI6_SEL                        SYSCFG_EXTICR2_EXTI6_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR2_EXTI7_SEL_Pos                    (12U)
#define SYSCFG_EXTICR2_EXTI7_SEL_Msk                    (0xFU << SYSCFG_EXTICR2_EXTI7_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR2_EXTI7_SEL                        SYSCFG_EXTICR2_EXTI7_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR2_ALL_Msk                    (SYSCFG_EXTICR2_EXTI7_SEL_Msk | SYSCFG_EXTICR2_EXTI6_SEL_Msk | SYSCFG_EXTICR2_EXTI5_SEL_Msk | SYSCFG_EXTICR2_EXTI4_SEL_Msk)
 
/******************  Bit definition for SYSCFG_EXTICR3 register ********************/
#define SYSCFG_EXTICR3_EXTI8_SEL_Pos                    (0U)
#define SYSCFG_EXTICR3_EXTI8_SEL_Msk                    (0xFU << SYSCFG_EXTICR3_EXTI8_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR3_EXTI8_SEL                        SYSCFG_EXTICR3_EXTI8_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR3_EXTI9_SEL_Pos                    (4U)
#define SYSCFG_EXTICR3_EXTI9_SEL_Msk                    (0xFU << SYSCFG_EXTICR3_EXTI9_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR3_EXTI9_SEL                        SYSCFG_EXTICR3_EXTI9_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR3_EXTI10_SEL_Pos                    (8U)
#define SYSCFG_EXTICR3_EXTI10_SEL_Msk                    (0xFU << SYSCFG_EXTICR3_EXTI10_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR3_EXTI10_SEL                        SYSCFG_EXTICR3_EXTI10_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR3_EXTI11_SEL_Pos                    (12U)
#define SYSCFG_EXTICR3_EXTI11_SEL_Msk                    (0xFU << SYSCFG_EXTICR3_EXTI11_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR3_EXTI11_SEL                        SYSCFG_EXTICR3_EXTI11_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR3_ALL_Msk                    (SYSCFG_EXTICR3_EXTI11_SEL_Msk | SYSCFG_EXTICR3_EXTI10_SEL_Msk | SYSCFG_EXTICR3_EXTI9_SEL_Msk | SYSCFG_EXTICR3_EXTI8_SEL_Msk)
 
/******************  Bit definition for SYSCFG_EXTICR4 register ********************/
#define SYSCFG_EXTICR4_EXTI12_SEL_Pos                    (0U)
#define SYSCFG_EXTICR4_EXTI12_SEL_Msk                    (0xFU << SYSCFG_EXTICR4_EXTI12_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR4_EXTI12_SEL                        SYSCFG_EXTICR4_EXTI12_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR4_EXTI13_SEL_Pos                    (4U)
#define SYSCFG_EXTICR4_EXTI13_SEL_Msk                    (0xFU << SYSCFG_EXTICR4_EXTI13_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR4_EXTI13_SEL                        SYSCFG_EXTICR4_EXTI13_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR4_EXTI14_SEL_Pos                    (8U)
#define SYSCFG_EXTICR4_EXTI14_SEL_Msk                    (0xFU << SYSCFG_EXTICR4_EXTI14_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR4_EXTI14_SEL                        SYSCFG_EXTICR4_EXTI14_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR4_EXTI15_SEL_Pos                    (12U)
#define SYSCFG_EXTICR4_EXTI15_SEL_Msk                    (0xFU << SYSCFG_EXTICR4_EXTI15_SEL_Pos)        /*!<  */
#define SYSCFG_EXTICR4_EXTI15_SEL                        SYSCFG_EXTICR4_EXTI15_SEL_Msk                   /*!<  */
 
#define SYSCFG_EXTICR4_ALL_Msk                    (SYSCFG_EXTICR4_EXTI15_SEL_Msk | SYSCFG_EXTICR4_EXTI14_SEL_Msk | SYSCFG_EXTICR4_EXTI13_SEL_Msk | SYSCFG_EXTICR4_EXTI12_SEL_Msk)
 
/******************  Bit definition for SYSCFG_IRCR register ********************/
#define SYSCFG_IRCR_SIG_SEL_Pos                    (0U)
#define SYSCFG_IRCR_SIG_SEL_Msk                    (0x3U << SYSCFG_IRCR_SIG_SEL_Pos)        /*!<  */
#define SYSCFG_IRCR_SIG_SEL                        SYSCFG_IRCR_SIG_SEL_Msk                   /*!<  */
 
#define SYSCFG_IRCR_CAR_SEL_Pos                    (2U)
#define SYSCFG_IRCR_CAR_SEL_Msk                    (0x3U << SYSCFG_IRCR_CAR_SEL_Pos)        /*!<  */
#define SYSCFG_IRCR_CAR_SEL                        SYSCFG_IRCR_CAR_SEL_Msk                   /*!<  */
 
#define SYSCFG_IRCR_SIG_INV_Pos                    (4U)
#define SYSCFG_IRCR_SIG_INV_Msk                    (0x01U << SYSCFG_IRCR_SIG_INV_Pos)        /*!<  */
#define SYSCFG_IRCR_SIG_INV                        SYSCFG_IRCR_SIG_INV_Msk                   /*!<  */
 
#define SYSCFG_IRCR_CAR_INV_Pos                    (5U)
#define SYSCFG_IRCR_CAR_INV_Msk                    (0x01U << SYSCFG_IRCR_CAR_INV_Pos)        /*!<  */
#define SYSCFG_IRCR_CAR_INV                        SYSCFG_IRCR_CAR_INV_Msk                   /*!<  */
 
#define SYSCFG_IRCR_OUT_SEL_Pos                    (8U)
#define SYSCFG_IRCR_OUT_SEL_Msk                    (0x3U << SYSCFG_IRCR_OUT_SEL_Pos)        /*!<  */
#define SYSCFG_IRCR_OUT_SEL                        SYSCFG_IRCR_OUT_SEL_Msk                   /*!<  */
 
#define SYSCFG_IRCR_ALL_Msk                    (SYSCFG_IRCR_OUT_SEL_Msk | SYSCFG_IRCR_CAR_INV_Msk | SYSCFG_IRCR_SIG_INV_Msk | SYSCFG_IRCR_CAR_SEL_Msk | SYSCFG_IRCR_SIG_SEL_Msk)
 
/******************  Bit definition for EXTI_IMR register ********************/
#define EXTI_IMR_MRX_Pos                    (0U)
#define EXTI_IMR_MRX_Msk                    (0xFFFFFFU << EXTI_IMR_MRX_Pos)        /*!<  */
#define EXTI_IMR_MRX                        EXTI_IMR_MRX_Msk                   /*!<  */
 
#define EXTI_IMR_ALL_Msk                    (EXTI_IMR_MRX_Msk)
 
/******************  Bit definition for EXTI_EMR register ********************/
#define EXTI_EMR_EMRX_Pos                    (0U)
#define EXTI_EMR_EMRX_Msk                    (0xFFFFFFU << EXTI_EMR_EMRX_Pos)        /*!<  */
#define EXTI_EMR_EMRX                        EXTI_EMR_EMRX_Msk                   /*!<  */
 
#define EXTI_EMR_ALL_Msk                    (EXTI_EMR_EMRX_Msk)
 
/******************  Bit definition for EXTI_RTSR register ********************/
#define EXTI_RTSR_TRX_Pos                    (0U)
#define EXTI_RTSR_TRX_Msk                    (0xFFFFFFU << EXTI_RTSR_TRX_Pos)        /*!<  */
#define EXTI_RTSR_TRX                        EXTI_RTSR_TRX_Msk                   /*!<  */
 
#define EXTI_RTSR_ALL_Msk                    (EXTI_RTSR_TRX_Msk)
 
/******************  Bit definition for EXTI_FTSR register ********************/
#define EXTI_FTSR_TRX_Pos                    (0U)
#define EXTI_FTSR_TRX_Msk                    (0xFFFFFFU << EXTI_FTSR_TRX_Pos)        /*!<  */
#define EXTI_FTSR_TRX                        EXTI_FTSR_TRX_Msk                   /*!<  */
 
#define EXTI_FTSR_ALL_Msk                    (EXTI_FTSR_TRX_Msk)
 
/******************  Bit definition for EXTI_SWIER register ********************/
#define EXTI_SWIER_SWIERX_Pos                    (0U)
#define EXTI_SWIER_SWIERX_Msk                    (0xFFFFFFU << EXTI_SWIER_SWIERX_Pos)        /*!<  */
#define EXTI_SWIER_SWIERX                        EXTI_SWIER_SWIERX_Msk                   /*!<  */
 
#define EXTI_SWIER_ALL_Msk                    (EXTI_SWIER_SWIERX_Msk)
 
/******************  Bit definition for EXTI_PR register ********************/
#define EXTI_PR_PRX_Pos                    (0U)
#define EXTI_PR_PRX_Msk                    (0xFFFFFFU << EXTI_PR_PRX_Pos)        /*!<  */
#define EXTI_PR_PRX                        EXTI_PR_PRX_Msk                   /*!<  */
 
#define EXTI_PR_ALL_Msk                    (EXTI_PR_PRX_Msk)
 
/******************  Bit definition for ADC_INTR register ********************/
#define ADC_INTR_SEQ2_AWD_LT_STS_Pos                    (0U)
#define ADC_INTR_SEQ2_AWD_LT_STS_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_LT_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_LT_STS                        ADC_INTR_SEQ2_AWD_LT_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_AWD_HT_STS_Pos                    (1U)
#define ADC_INTR_SEQ2_AWD_HT_STS_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_HT_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_HT_STS                        ADC_INTR_SEQ2_AWD_HT_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_AWD_REG_STS_Pos                    (2U)
#define ADC_INTR_SEQ2_AWD_REG_STS_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_REG_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_REG_STS                        ADC_INTR_SEQ2_AWD_REG_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_1SEQ_STS_Pos                    (3U)
#define ADC_INTR_SEQ2_1SEQ_STS_Msk                    (0x01U << ADC_INTR_SEQ2_1SEQ_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ2_1SEQ_STS                        ADC_INTR_SEQ2_1SEQ_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_1CH_STS_Pos                    (4U)
#define ADC_INTR_SEQ2_1CH_STS_Msk                    (0x01U << ADC_INTR_SEQ2_1CH_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ2_1CH_STS                        ADC_INTR_SEQ2_1CH_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_LT_STS_Pos                    (5U)
#define ADC_INTR_SEQ1_AWD_LT_STS_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_LT_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_LT_STS                        ADC_INTR_SEQ1_AWD_LT_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_HT_STS_Pos                    (6U)
#define ADC_INTR_SEQ1_AWD_HT_STS_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_HT_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_HT_STS                        ADC_INTR_SEQ1_AWD_HT_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_REG_STS_Pos                    (7U)
#define ADC_INTR_SEQ1_AWD_REG_STS_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_REG_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_REG_STS                        ADC_INTR_SEQ1_AWD_REG_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_ALL_STS_Pos                    (8U)
#define ADC_INTR_SEQ1_ALL_STS_Msk                    (0x01U << ADC_INTR_SEQ1_ALL_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_ALL_STS                        ADC_INTR_SEQ1_ALL_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_1SEQ_STS_Pos                    (9U)
#define ADC_INTR_SEQ1_1SEQ_STS_Msk                    (0x01U << ADC_INTR_SEQ1_1SEQ_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_1SEQ_STS                        ADC_INTR_SEQ1_1SEQ_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_1CH_STS_Pos                    (10U)
#define ADC_INTR_SEQ1_1CH_STS_Msk                    (0x01U << ADC_INTR_SEQ1_1CH_STS_Pos)        /*!<  */
#define ADC_INTR_SEQ1_1CH_STS                        ADC_INTR_SEQ1_1CH_STS_Msk                   /*!<  */
 
#define ADC_INTR_DMA_OVERRUN_STS_Pos                    (11U)
#define ADC_INTR_DMA_OVERRUN_STS_Msk                    (0x01U << ADC_INTR_DMA_OVERRUN_STS_Pos)        /*!<  */
#define ADC_INTR_DMA_OVERRUN_STS                        ADC_INTR_DMA_OVERRUN_STS_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_AWD_LT_Pos                    (16U)
#define ADC_INTR_SEQ2_AWD_LT_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_LT_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_LT                        ADC_INTR_SEQ2_AWD_LT_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_AWD_HT_Pos                    (17U)
#define ADC_INTR_SEQ2_AWD_HT_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_HT_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_HT                        ADC_INTR_SEQ2_AWD_HT_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_AWD_REG_Pos                    (18U)
#define ADC_INTR_SEQ2_AWD_REG_Msk                    (0x01U << ADC_INTR_SEQ2_AWD_REG_Pos)        /*!<  */
#define ADC_INTR_SEQ2_AWD_REG                        ADC_INTR_SEQ2_AWD_REG_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_1SEQ_Pos                    (19U)
#define ADC_INTR_SEQ2_1SEQ_Msk                    (0x01U << ADC_INTR_SEQ2_1SEQ_Pos)        /*!<  */
#define ADC_INTR_SEQ2_1SEQ                        ADC_INTR_SEQ2_1SEQ_Msk                   /*!<  */
 
#define ADC_INTR_SEQ2_1CH_Pos                    (20U)
#define ADC_INTR_SEQ2_1CH_Msk                    (0x01U << ADC_INTR_SEQ2_1CH_Pos)        /*!<  */
#define ADC_INTR_SEQ2_1CH                        ADC_INTR_SEQ2_1CH_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_LT_Pos                    (21U)
#define ADC_INTR_SEQ1_AWD_LT_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_LT_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_LT                        ADC_INTR_SEQ1_AWD_LT_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_HT_Pos                    (22U)
#define ADC_INTR_SEQ1_AWD_HT_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_HT_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_HT                        ADC_INTR_SEQ1_AWD_HT_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_AWD_REG_Pos                    (23U)
#define ADC_INTR_SEQ1_AWD_REG_Msk                    (0x01U << ADC_INTR_SEQ1_AWD_REG_Pos)        /*!<  */
#define ADC_INTR_SEQ1_AWD_REG                        ADC_INTR_SEQ1_AWD_REG_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_ALL_Pos                    (24U)
#define ADC_INTR_SEQ1_ALL_Msk                    (0x01U << ADC_INTR_SEQ1_ALL_Pos)        /*!<  */
#define ADC_INTR_SEQ1_ALL                        ADC_INTR_SEQ1_ALL_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_1SEQ_Pos                    (25U)
#define ADC_INTR_SEQ1_1SEQ_Msk                    (0x01U << ADC_INTR_SEQ1_1SEQ_Pos)        /*!<  */
#define ADC_INTR_SEQ1_1SEQ                        ADC_INTR_SEQ1_1SEQ_Msk                   /*!<  */
 
#define ADC_INTR_SEQ1_1CH_Pos                    (26U)
#define ADC_INTR_SEQ1_1CH_Msk                    (0x01U << ADC_INTR_SEQ1_1CH_Pos)        /*!<  */
#define ADC_INTR_SEQ1_1CH                        ADC_INTR_SEQ1_1CH_Msk                   /*!<  */
 
#define ADC_INTR_DMA_OVERRUN_Pos                    (27U)
#define ADC_INTR_DMA_OVERRUN_Msk                    (0x01U << ADC_INTR_DMA_OVERRUN_Pos)        /*!<  */
#define ADC_INTR_DMA_OVERRUN                        ADC_INTR_DMA_OVERRUN_Msk                   /*!<  */
 
#define ADC_INTR_ALL_Msk                    (ADC_INTR_DMA_OVERRUN_Msk | ADC_INTR_SEQ1_1CH_Msk | ADC_INTR_SEQ1_1SEQ_Msk | ADC_INTR_SEQ1_ALL_Msk | ADC_INTR_SEQ1_AWD_REG_Msk | ADC_INTR_SEQ1_AWD_HT_Msk | ADC_INTR_SEQ1_AWD_LT_Msk | ADC_INTR_SEQ2_1CH_Msk | ADC_INTR_SEQ2_1SEQ_Msk | ADC_INTR_SEQ2_AWD_REG_Msk | ADC_INTR_SEQ2_AWD_HT_Msk | ADC_INTR_SEQ2_AWD_LT_Msk | ADC_INTR_DMA_OVERRUN_STS_Msk | ADC_INTR_SEQ1_1CH_STS_Msk | ADC_INTR_SEQ1_1SEQ_STS_Msk | ADC_INTR_SEQ1_ALL_STS_Msk | ADC_INTR_SEQ1_AWD_REG_STS_Msk | ADC_INTR_SEQ1_AWD_HT_STS_Msk | ADC_INTR_SEQ1_AWD_LT_STS_Msk | ADC_INTR_SEQ2_1CH_STS_Msk | ADC_INTR_SEQ2_1SEQ_STS_Msk | ADC_INTR_SEQ2_AWD_REG_STS_Msk | ADC_INTR_SEQ2_AWD_HT_STS_Msk | ADC_INTR_SEQ2_AWD_LT_STS_Msk)
 
/******************  Bit definition for ADC_CR register ********************/
#define ADC_CR_ADCEN_Pos                    (0U)
#define ADC_CR_ADCEN_Msk                    (0x01U << ADC_CR_ADCEN_Pos)        /*!<  */
#define ADC_CR_ADCEN                        ADC_CR_ADCEN_Msk                   /*!<  */
 
#define ADC_CR_SEQ1_START_Pos                    (1U)
#define ADC_CR_SEQ1_START_Msk                    (0x01U << ADC_CR_SEQ1_START_Pos)        /*!<  */
#define ADC_CR_SEQ1_START                        ADC_CR_SEQ1_START_Msk                   /*!<  */
 
#define ADC_CR_SEQ2_START_Pos                    (2U)
#define ADC_CR_SEQ2_START_Msk                    (0x01U << ADC_CR_SEQ2_START_Pos)        /*!<  */
#define ADC_CR_SEQ2_START                        ADC_CR_SEQ2_START_Msk                   /*!<  */
 
#define ADC_CR_VREFSEL_Pos                    (3U)
#define ADC_CR_VREFSEL_Msk                    (0x01U << ADC_CR_VREFSEL_Pos)        /*!<  */
#define ADC_CR_VREFSEL                        ADC_CR_VREFSEL_Msk                   /*!<  */
 
#define ADC_CR_CLK_SEL_Pos                    (4U)
#define ADC_CR_CLK_SEL_Msk                    (0x7U << ADC_CR_CLK_SEL_Pos)        /*!<  */
#define ADC_CR_CLK_SEL                        ADC_CR_CLK_SEL_Msk                   /*!<  */
 
#define ADC_CR_SAMP_Pos                    (7U)
#define ADC_CR_SAMP_Msk                    (0x01U << ADC_CR_SAMP_Pos)        /*!<  */
#define ADC_CR_SAMP                        ADC_CR_SAMP_Msk                   /*!<  */
 
#define ADC_CR_SEQ1_CONT_EN_Pos                    (8U)
#define ADC_CR_SEQ1_CONT_EN_Msk                    (0x01U << ADC_CR_SEQ1_CONT_EN_Pos)        /*!<  */
#define ADC_CR_SEQ1_CONT_EN                        ADC_CR_SEQ1_CONT_EN_Msk                   /*!<  */
 
#define ADC_CR_SEQ1_ACC_EN_Pos                    (9U)
#define ADC_CR_SEQ1_ACC_EN_Msk                    (0x01U << ADC_CR_SEQ1_ACC_EN_Pos)        /*!<  */
#define ADC_CR_SEQ1_ACC_EN                        ADC_CR_SEQ1_ACC_EN_Msk                   /*!<  */
 
#define ADC_CR_DMA_EN_Pos                    (11U)
#define ADC_CR_DMA_EN_Msk                    (0x01U << ADC_CR_DMA_EN_Pos)        /*!<  */
#define ADC_CR_DMA_EN                        ADC_CR_DMA_EN_Msk                   /*!<  */
 
#define ADC_CR_TS_EN_Pos                    (12U)
#define ADC_CR_TS_EN_Msk                    (0x01U << ADC_CR_TS_EN_Pos)        /*!<  */
#define ADC_CR_TS_EN                        ADC_CR_TS_EN_Msk                   /*!<  */
 
#define ADC_CR_STATE_RST_Pos                    (15U)
#define ADC_CR_STATE_RST_Msk                    (0x01U << ADC_CR_STATE_RST_Pos)        /*!<  */
#define ADC_CR_STATE_RST                        ADC_CR_STATE_RST_Msk                   /*!<  */
 
#define ADC_CR_SEQ1_CONT_NUM_Pos                    (16U)
#define ADC_CR_SEQ1_CONT_NUM_Msk                    (0xFFU << ADC_CR_SEQ1_CONT_NUM_Pos)        /*!<  */
#define ADC_CR_SEQ1_CONT_NUM                        ADC_CR_SEQ1_CONT_NUM_Msk                   /*!<  */
 
#define ADC_CR_ALL_Msk                    (ADC_CR_SEQ1_CONT_NUM_Msk | ADC_CR_STATE_RST_Msk | ADC_CR_TS_EN_Msk | ADC_CR_DMA_EN_Msk | ADC_CR_SEQ1_ACC_EN_Msk | ADC_CR_SEQ1_CONT_EN_Msk | ADC_CR_SAMP_Msk | ADC_CR_CLK_SEL_Msk | ADC_CR_VREFSEL_Msk | ADC_CR_SEQ2_START_Msk | ADC_CR_SEQ1_START_Msk | ADC_CR_ADCEN_Msk)
 
/******************  Bit definition for ADC_DR register ********************/
#define ADC_DR_SEQ1_RESULT_Pos                    (0U)
#define ADC_DR_SEQ1_RESULT_Msk                    (0xFFFU << ADC_DR_SEQ1_RESULT_Pos)        /*!<  */
#define ADC_DR_SEQ1_RESULT                        ADC_DR_SEQ1_RESULT_Msk                   /*!<  */
 
#define ADC_DR_SEQ2_RESULT_Pos                    (16U)
#define ADC_DR_SEQ2_RESULT_Msk                    (0xFFFU << ADC_DR_SEQ2_RESULT_Pos)        /*!<  */
#define ADC_DR_SEQ2_RESULT                        ADC_DR_SEQ2_RESULT_Msk                   /*!<  */
 
#define ADC_DR_ALL_Msk                    (ADC_DR_SEQ2_RESULT_Msk | ADC_DR_SEQ1_RESULT_Msk)
 
/******************  Bit definition for ADC_SR register ********************/
#define ADC_SR_ADC_CH_Pos                    (0U)
#define ADC_SR_ADC_CH_Msk                    (0x1FU << ADC_SR_ADC_CH_Pos)        /*!<  */
#define ADC_SR_ADC_CH                        ADC_SR_ADC_CH_Msk                   /*!<  */
 
#define ADC_SR_SEQ1_CH_CNT_Pos                    (5U)
#define ADC_SR_SEQ1_CH_CNT_Msk                    (0x1FU << ADC_SR_SEQ1_CH_CNT_Pos)        /*!<  */
#define ADC_SR_SEQ1_CH_CNT                        ADC_SR_SEQ1_CH_CNT_Msk                   /*!<  */
 
#define ADC_SR_SEQ2_CH_CNT_Pos                    (10U)
#define ADC_SR_SEQ2_CH_CNT_Msk                    (0x3U << ADC_SR_SEQ2_CH_CNT_Pos)        /*!<  */
#define ADC_SR_SEQ2_CH_CNT                        ADC_SR_SEQ2_CH_CNT_Msk                   /*!<  */
 
#define ADC_SR_SEQ1_CONT_Pos                    (12U)
#define ADC_SR_SEQ1_CONT_Msk                    (0xFFU << ADC_SR_SEQ1_CONT_Pos)        /*!<  */
#define ADC_SR_SEQ1_CONT                        ADC_SR_SEQ1_CONT_Msk                   /*!<  */
 
#define ADC_SR_CUR_STATE_Pos                    (20U)
#define ADC_SR_CUR_STATE_Msk                    (0x7U << ADC_SR_CUR_STATE_Pos)        /*!<  */
#define ADC_SR_CUR_STATE                        ADC_SR_CUR_STATE_Msk                   /*!<  */
 
#define ADC_SR_SEQ2_WORK_Pos                    (23U)
#define ADC_SR_SEQ2_WORK_Msk                    (0x01U << ADC_SR_SEQ2_WORK_Pos)        /*!<  */
#define ADC_SR_SEQ2_WORK                        ADC_SR_SEQ2_WORK_Msk                   /*!<  */
 
#define ADC_SR_SEQ1_WORK_Pos                    (24U)
#define ADC_SR_SEQ1_WORK_Msk                    (0x01U << ADC_SR_SEQ1_WORK_Pos)        /*!<  */
#define ADC_SR_SEQ1_WORK                        ADC_SR_SEQ1_WORK_Msk                   /*!<  */
 
#define ADC_SR_ALL_Msk                    (ADC_SR_SEQ1_WORK_Msk | ADC_SR_SEQ2_WORK_Msk | ADC_SR_CUR_STATE_Msk | ADC_SR_SEQ1_CONT_Msk | ADC_SR_SEQ2_CH_CNT_Msk | ADC_SR_SEQ1_CH_CNT_Msk | ADC_SR_ADC_CH_Msk)
 
/******************  Bit definition for ADC_SEQ1CR1 register ********************/
#define ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Pos                    (0U)
#define ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Msk                    (0x3FU << ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Pos)        /*!<  */
#define ADC_SEQ1CR1_SEQ1_TRIG1_SEL                        ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Msk                   /*!<  */
 
#define ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Pos                    (8U)
#define ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Msk                    (0x3FU << ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Pos)        /*!<  */
#define ADC_SEQ1CR1_SEQ1_TRIG2_SEL                        ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Msk                   /*!<  */
 
#define ADC_SEQ1CR1_SEQ1_NUM_Pos                    (16U)
#define ADC_SEQ1CR1_SEQ1_NUM_Msk                    (0x1FU << ADC_SEQ1CR1_SEQ1_NUM_Pos)        /*!<  */
#define ADC_SEQ1CR1_SEQ1_NUM                        ADC_SEQ1CR1_SEQ1_NUM_Msk                   /*!<  */
 
#define ADC_SEQ1CR1_ALL_Msk                    (ADC_SEQ1CR1_SEQ1_NUM_Msk | ADC_SEQ1CR1_SEQ1_TRIG2_SEL_Msk | ADC_SEQ1CR1_SEQ1_TRIG1_SEL_Msk)
 
/******************  Bit definition for ADC_SEQ1CR2 register ********************/
#define ADC_SEQ1CR2_SEQ1_SEL1_Pos                    (0U)
#define ADC_SEQ1CR2_SEQ1_SEL1_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL1_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL1                        ADC_SEQ1CR2_SEQ1_SEL1_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_SEQ1_SEL2_Pos                    (5U)
#define ADC_SEQ1CR2_SEQ1_SEL2_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL2_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL2                        ADC_SEQ1CR2_SEQ1_SEL2_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_SEQ1_SEL3_Pos                    (10U)
#define ADC_SEQ1CR2_SEQ1_SEL3_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL3_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL3                        ADC_SEQ1CR2_SEQ1_SEL3_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_SEQ1_SEL4_Pos                    (15U)
#define ADC_SEQ1CR2_SEQ1_SEL4_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL4_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL4                        ADC_SEQ1CR2_SEQ1_SEL4_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_SEQ1_SEL5_Pos                    (20U)
#define ADC_SEQ1CR2_SEQ1_SEL5_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL5_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL5                        ADC_SEQ1CR2_SEQ1_SEL5_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_SEQ1_SEL6_Pos                    (25U)
#define ADC_SEQ1CR2_SEQ1_SEL6_Msk                    (0x1FU << ADC_SEQ1CR2_SEQ1_SEL6_Pos)        /*!<  */
#define ADC_SEQ1CR2_SEQ1_SEL6                        ADC_SEQ1CR2_SEQ1_SEL6_Msk                   /*!<  */
 
#define ADC_SEQ1CR2_ALL_Msk                    (ADC_SEQ1CR2_SEQ1_SEL6_Msk | ADC_SEQ1CR2_SEQ1_SEL5_Msk | ADC_SEQ1CR2_SEQ1_SEL4_Msk | ADC_SEQ1CR2_SEQ1_SEL3_Msk | ADC_SEQ1CR2_SEQ1_SEL2_Msk | ADC_SEQ1CR2_SEQ1_SEL1_Msk)
 
/******************  Bit definition for ADC_SEQ1CR3 register ********************/
#define ADC_SEQ1CR3_SEQ1_SEL7_Pos                    (0U)
#define ADC_SEQ1CR3_SEQ1_SEL7_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL7_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL7                        ADC_SEQ1CR3_SEQ1_SEL7_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_SEQ1_SEL8_Pos                    (5U)
#define ADC_SEQ1CR3_SEQ1_SEL8_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL8_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL8                        ADC_SEQ1CR3_SEQ1_SEL8_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_SEQ1_SEL9_Pos                    (10U)
#define ADC_SEQ1CR3_SEQ1_SEL9_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL9_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL9                        ADC_SEQ1CR3_SEQ1_SEL9_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_SEQ1_SEL10_Pos                    (15U)
#define ADC_SEQ1CR3_SEQ1_SEL10_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL10_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL10                        ADC_SEQ1CR3_SEQ1_SEL10_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_SEQ1_SEL11_Pos                    (20U)
#define ADC_SEQ1CR3_SEQ1_SEL11_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL11_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL11                        ADC_SEQ1CR3_SEQ1_SEL11_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_SEQ1_SEL12_Pos                    (25U)
#define ADC_SEQ1CR3_SEQ1_SEL12_Msk                    (0x1FU << ADC_SEQ1CR3_SEQ1_SEL12_Pos)        /*!<  */
#define ADC_SEQ1CR3_SEQ1_SEL12                        ADC_SEQ1CR3_SEQ1_SEL12_Msk                   /*!<  */
 
#define ADC_SEQ1CR3_ALL_Msk                    (ADC_SEQ1CR3_SEQ1_SEL12_Msk | ADC_SEQ1CR3_SEQ1_SEL11_Msk | ADC_SEQ1CR3_SEQ1_SEL10_Msk | ADC_SEQ1CR3_SEQ1_SEL9_Msk | ADC_SEQ1CR3_SEQ1_SEL8_Msk | ADC_SEQ1CR3_SEQ1_SEL7_Msk)
 
/******************  Bit definition for ADC_SEQ1CR4 register ********************/
#define ADC_SEQ1CR4_SEQ1_SEL13_Pos                    (0U)
#define ADC_SEQ1CR4_SEQ1_SEL13_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL13_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL13                        ADC_SEQ1CR4_SEQ1_SEL13_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_SEQ1_SEL14_Pos                    (5U)
#define ADC_SEQ1CR4_SEQ1_SEL14_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL14_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL14                        ADC_SEQ1CR4_SEQ1_SEL14_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_SEQ1_SEL15_Pos                    (10U)
#define ADC_SEQ1CR4_SEQ1_SEL15_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL15_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL15                        ADC_SEQ1CR4_SEQ1_SEL15_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_SEQ1_SEL16_Pos                    (15U)
#define ADC_SEQ1CR4_SEQ1_SEL16_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL16_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL16                        ADC_SEQ1CR4_SEQ1_SEL16_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_SEQ1_SEL17_Pos                    (20U)
#define ADC_SEQ1CR4_SEQ1_SEL17_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL17_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL17                        ADC_SEQ1CR4_SEQ1_SEL17_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_SEQ1_SEL18_Pos                    (25U)
#define ADC_SEQ1CR4_SEQ1_SEL18_Msk                    (0x1FU << ADC_SEQ1CR4_SEQ1_SEL18_Pos)        /*!<  */
#define ADC_SEQ1CR4_SEQ1_SEL18                        ADC_SEQ1CR4_SEQ1_SEL18_Msk                   /*!<  */
 
#define ADC_SEQ1CR4_ALL_Msk                    (ADC_SEQ1CR4_SEQ1_SEL18_Msk | ADC_SEQ1CR4_SEQ1_SEL17_Msk | ADC_SEQ1CR4_SEQ1_SEL16_Msk | ADC_SEQ1CR4_SEQ1_SEL15_Msk | ADC_SEQ1CR4_SEQ1_SEL14_Msk | ADC_SEQ1CR4_SEQ1_SEL13_Msk)
 
/******************  Bit definition for ADC_SEQ1SRx register ********************/
#define ADC_SEQ1SRX_SEQ1_RESULT_Pos                    (0U)
#define ADC_SEQ1SRX_SEQ1_RESULT_Msk                    (0xFFFU << ADC_SEQ1SRX_SEQ1_RESULT_Pos)        /*!<  */
#define ADC_SEQ1SRX_SEQ1_RESULT                        ADC_SEQ1SRX_SEQ1_RESULT_Msk                   /*!<  */
 
#define ADC_SEQ1SRX_ALL_Msk                    (ADC_SEQ1SRX_SEQ1_RESULT_Msk)
 
/******************  Bit definition for ADC_SEQ2CR1 register ********************/
#define ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Pos                    (0U)
#define ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Msk                    (0x3FU << ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Pos)        /*!<  */
#define ADC_SEQ2CR1_SEQ2_TRIG1_SEL                        ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Msk                   /*!<  */
 
#define ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Pos                    (8U)
#define ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Msk                    (0x3FU << ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Pos)        /*!<  */
#define ADC_SEQ2CR1_SEQ2_TRIG2_SEL                        ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Msk                   /*!<  */
 
#define ADC_SEQ2CR1_SEQ2_NUM_Pos                    (16U)
#define ADC_SEQ2CR1_SEQ2_NUM_Msk                    (0x3U << ADC_SEQ2CR1_SEQ2_NUM_Pos)        /*!<  */
#define ADC_SEQ2CR1_SEQ2_NUM                        ADC_SEQ2CR1_SEQ2_NUM_Msk                   /*!<  */
 
#define ADC_SEQ2CR1_ALL_Msk                    (ADC_SEQ2CR1_SEQ2_NUM_Msk | ADC_SEQ2CR1_SEQ2_TRIG2_SEL_Msk | ADC_SEQ2CR1_SEQ2_TRIG1_SEL_Msk)
 
/******************  Bit definition for ADC_SEQ2CR2 register ********************/
#define ADC_SEQ2CR2_SEQ2_SEL1_Pos                    (0U)
#define ADC_SEQ2CR2_SEQ2_SEL1_Msk                    (0x1FU << ADC_SEQ2CR2_SEQ2_SEL1_Pos)        /*!<  */
#define ADC_SEQ2CR2_SEQ2_SEL1                        ADC_SEQ2CR2_SEQ2_SEL1_Msk                   /*!<  */
 
#define ADC_SEQ2CR2_SEQ2_SEL2_Pos                    (5U)
#define ADC_SEQ2CR2_SEQ2_SEL2_Msk                    (0x1FU << ADC_SEQ2CR2_SEQ2_SEL2_Pos)        /*!<  */
#define ADC_SEQ2CR2_SEQ2_SEL2                        ADC_SEQ2CR2_SEQ2_SEL2_Msk                   /*!<  */
 
#define ADC_SEQ2CR2_SEQ2_SEL3_Pos                    (10U)
#define ADC_SEQ2CR2_SEQ2_SEL3_Msk                    (0x1FU << ADC_SEQ2CR2_SEQ2_SEL3_Pos)        /*!<  */
#define ADC_SEQ2CR2_SEQ2_SEL3                        ADC_SEQ2CR2_SEQ2_SEL3_Msk                   /*!<  */
 
#define ADC_SEQ2CR2_SEQ2_SEL4_Pos                    (15U)
#define ADC_SEQ2CR2_SEQ2_SEL4_Msk                    (0x1FU << ADC_SEQ2CR2_SEQ2_SEL4_Pos)        /*!<  */
#define ADC_SEQ2CR2_SEQ2_SEL4                        ADC_SEQ2CR2_SEQ2_SEL4_Msk                   /*!<  */
 
#define ADC_SEQ2CR2_ALL_Msk                    (ADC_SEQ2CR2_SEQ2_SEL4_Msk | ADC_SEQ2CR2_SEQ2_SEL3_Msk | ADC_SEQ2CR2_SEQ2_SEL2_Msk | ADC_SEQ2CR2_SEQ2_SEL1_Msk)
 
/******************  Bit definition for ADC_SEQ2SRx register ********************/
#define ADC_SEQ2SRX_SEQ2_RESULT_Pos                    (0U)
#define ADC_SEQ2SRX_SEQ2_RESULT_Msk                    (0xFFFU << ADC_SEQ2SRX_SEQ2_RESULT_Pos)        /*!<  */
#define ADC_SEQ2SRX_SEQ2_RESULT                        ADC_SEQ2SRX_SEQ2_RESULT_Msk                   /*!<  */
 
#define ADC_SEQ2SRX_ALL_Msk                    (ADC_SEQ2SRX_SEQ2_RESULT_Msk)
 
/******************  Bit definition for ADC_ACCR register ********************/
#define ADC_ACCR_ACC_Pos                    (0U)
#define ADC_ACCR_ACC_Msk                    (0xFFFFFU << ADC_ACCR_ACC_Pos)        /*!<  */
#define ADC_ACCR_ACC                        ADC_ACCR_ACC_Msk                   /*!<  */
 
#define ADC_ACCR_ALL_Msk                    (ADC_ACCR_ACC_Msk)
 
/******************  Bit definition for ADC_AWDCR register ********************/
#define ADC_AWDCR_AWD_LT_Pos                    (0U)
#define ADC_AWDCR_AWD_LT_Msk                    (0xFFFU << ADC_AWDCR_AWD_LT_Pos)        /*!<  */
#define ADC_AWDCR_AWD_LT                        ADC_AWDCR_AWD_LT_Msk                   /*!<  */
 
#define ADC_AWDCR_AWD_HT_Pos                    (12U)
#define ADC_AWDCR_AWD_HT_Msk                    (0xFFFU << ADC_AWDCR_AWD_HT_Pos)        /*!<  */
#define ADC_AWDCR_AWD_HT                        ADC_AWDCR_AWD_HT_Msk                   /*!<  */
 
#define ADC_AWDCR_ALL_Msk                    (ADC_AWDCR_AWD_HT_Msk | ADC_AWDCR_AWD_LT_Msk)
 
/******************  Bit definition for DMA_INTSTA register ********************/
#define DMA_INTSTA_INT_STA_Pos                    (0U)
#define DMA_INTSTA_INT_STA_Msk                    (0x3FU << DMA_INTSTA_INT_STA_Pos)        /*!<  */
#define DMA_INTSTA_INT_STA                        DMA_INTSTA_INT_STA_Msk                   /*!<  */
 
#define DMA_INTSTA_ALL_Msk                    (DMA_INTSTA_INT_STA_Msk)
 
/******************  Bit definition for DMA_INTTCSTA register ********************/
#define DMA_INTTCSTA_INT_TC_STA_Pos                    (0U)
#define DMA_INTTCSTA_INT_TC_STA_Msk                    (0x3FU << DMA_INTTCSTA_INT_TC_STA_Pos)        /*!<  */
#define DMA_INTTCSTA_INT_TC_STA                        DMA_INTTCSTA_INT_TC_STA_Msk                   /*!<  */
 
#define DMA_INTTCSTA_ALL_Msk                    (DMA_INTTCSTA_INT_TC_STA_Msk)
 
/******************  Bit definition for DMA_INCTTCCLR register ********************/
#define DMA_INCTTCCLR_INT_TC_CLR_Pos                    (0U)
#define DMA_INCTTCCLR_INT_TC_CLR_Msk                    (0x3FU << DMA_INCTTCCLR_INT_TC_CLR_Pos)        /*!<  */
#define DMA_INCTTCCLR_INT_TC_CLR                        DMA_INCTTCCLR_INT_TC_CLR_Msk                   /*!<  */
 
#define DMA_INCTTCCLR_ALL_Msk                    (DMA_INCTTCCLR_INT_TC_CLR_Msk)
 
/******************  Bit definition for DMA_INTERRSTA register ********************/
#define DMA_INTERRSTA_INT_ERR_STA_Pos                    (0U)
#define DMA_INTERRSTA_INT_ERR_STA_Msk                    (0x3FU << DMA_INTERRSTA_INT_ERR_STA_Pos)        /*!<  */
#define DMA_INTERRSTA_INT_ERR_STA                        DMA_INTERRSTA_INT_ERR_STA_Msk                   /*!<  */
 
#define DMA_INTERRSTA_ALL_Msk                    (DMA_INTERRSTA_INT_ERR_STA_Msk)
 
/******************  Bit definition for DMA_INTERRCLR register ********************/
#define DMA_INTERRCLR_INT_ERR_CLR_Pos                    (0U)
#define DMA_INTERRCLR_INT_ERR_CLR_Msk                    (0x3FU << DMA_INTERRCLR_INT_ERR_CLR_Pos)        /*!<  */
#define DMA_INTERRCLR_INT_ERR_CLR                        DMA_INTERRCLR_INT_ERR_CLR_Msk                   /*!<  */
 
#define DMA_INTERRCLR_ALL_Msk                    (DMA_INTERRCLR_INT_ERR_CLR_Msk)
 
/******************  Bit definition for DMA_RAWINTTCSTA register ********************/
#define DMA_RAWINTTCSTA_INT_TC_RAW_STA_Pos                    (0U)
#define DMA_RAWINTTCSTA_INT_TC_RAW_STA_Msk                    (0x3FU << DMA_RAWINTTCSTA_INT_TC_RAW_STA_Pos)        /*!<  */
#define DMA_RAWINTTCSTA_INT_TC_RAW_STA                        DMA_RAWINTTCSTA_INT_TC_RAW_STA_Msk                   /*!<  */
 
#define DMA_RAWINTTCSTA_ALL_Msk                    (DMA_RAWINTTCSTA_INT_TC_RAW_STA_Msk)
 
/******************  Bit definition for DMA_RAWINTERRSTA register ********************/
#define DMA_RAWINTERRSTA_INT_ERR_RAW_STA_Pos                    (0U)
#define DMA_RAWINTERRSTA_INT_ERR_RAW_STA_Msk                    (0x3FU << DMA_RAWINTERRSTA_INT_ERR_RAW_STA_Pos)        /*!<  */
#define DMA_RAWINTERRSTA_INT_ERR_RAW_STA                        DMA_RAWINTERRSTA_INT_ERR_RAW_STA_Msk                   /*!<  */
 
#define DMA_RAWINTERRSTA_ALL_Msk                    (DMA_RAWINTERRSTA_INT_ERR_RAW_STA_Msk)
 
/******************  Bit definition for DMA_ENBLDCHNSTA register ********************/
#define DMA_ENBLDCHNSTA_CH_WORK_Pos                    (0U)
#define DMA_ENBLDCHNSTA_CH_WORK_Msk                    (0x3FU << DMA_ENBLDCHNSTA_CH_WORK_Pos)        /*!<  */
#define DMA_ENBLDCHNSTA_CH_WORK                        DMA_ENBLDCHNSTA_CH_WORK_Msk                   /*!<  */
 
#define DMA_ENBLDCHNSTA_ALL_Msk                    (DMA_ENBLDCHNSTA_CH_WORK_Msk)
 
/******************  Bit definition for DMA_SOFTSREQ register ********************/
#define DMA_SOFTSREQ_SOFT_SREQ_Pos                    (0U)
#define DMA_SOFTSREQ_SOFT_SREQ_Msk                    (0x3FU << DMA_SOFTSREQ_SOFT_SREQ_Pos)        /*!<  */
#define DMA_SOFTSREQ_SOFT_SREQ                        DMA_SOFTSREQ_SOFT_SREQ_Msk                   /*!<  */
 
#define DMA_SOFTSREQ_ALL_Msk                    (DMA_SOFTSREQ_SOFT_SREQ_Msk)
 
/******************  Bit definition for DMA_OVSTA register ********************/
#define DMA_OVSTA_OVER_RUN_STA_Pos                    (0U)
#define DMA_OVSTA_OVER_RUN_STA_Msk                    (0x3FU << DMA_OVSTA_OVER_RUN_STA_Pos)        /*!<  */
#define DMA_OVSTA_OVER_RUN_STA                        DMA_OVSTA_OVER_RUN_STA_Msk                   /*!<  */
 
#define DMA_OVSTA_ALL_Msk                    (DMA_OVSTA_OVER_RUN_STA_Msk)
 
/******************  Bit definition for DMA_OVCLR register ********************/
#define DMA_OVCLR_OVER_RUN_CLR_Pos                    (0U)
#define DMA_OVCLR_OVER_RUN_CLR_Msk                    (0x3FU << DMA_OVCLR_OVER_RUN_CLR_Pos)        /*!<  */
#define DMA_OVCLR_OVER_RUN_CLR                        DMA_OVCLR_OVER_RUN_CLR_Msk                   /*!<  */
 
#define DMA_OVCLR_ALL_Msk                    (DMA_OVCLR_OVER_RUN_CLR_Msk)
 
/******************  Bit definition for DMA_CONFIG register ********************/
#define DMA_CONFIG_DMA_EN_Pos                    (0U)
#define DMA_CONFIG_DMA_EN_Msk                    (0x01U << DMA_CONFIG_DMA_EN_Pos)        /*!<  */
#define DMA_CONFIG_DMA_EN                        DMA_CONFIG_DMA_EN_Msk                   /*!<  */
 
#define DMA_CONFIG_RELEASE_PMU_Pos                    (1U)
#define DMA_CONFIG_RELEASE_PMU_Msk                    (0x01U << DMA_CONFIG_RELEASE_PMU_Pos)        /*!<  */
#define DMA_CONFIG_RELEASE_PMU                        DMA_CONFIG_RELEASE_PMU_Msk                   /*!<  */
 
#define DMA_CONFIG_ALL_Msk                    (DMA_CONFIG_RELEASE_PMU_Msk | DMA_CONFIG_DMA_EN_Msk)
 
/******************  Bit definition for DMA_PERIPHx_SEL register ********************/
#define DMA_PERIPHX_SEL_PERIPH_SELX_Pos                    (0U)
#define DMA_PERIPHX_SEL_PERIPH_SELX_Msk                    (0x3FU << DMA_PERIPHX_SEL_PERIPH_SELX_Pos)        /*!<  */
#define DMA_PERIPHX_SEL_PERIPH_SELX                        DMA_PERIPHX_SEL_PERIPH_SELX_Msk                   /*!<  */
 
#define DMA_PERIPHX_SEL_ALL_Msk                    (DMA_PERIPHX_SEL_PERIPH_SELX_Msk)
 
/******************  Bit definition for DMA_CHx_SRCADDR register ********************/
#define DMA_CHX_SRCADDR_SRC_ADDR_Pos                    (0U)
#define DMA_CHX_SRCADDR_SRC_ADDR_Msk                    (0xFFFFFFFFU << DMA_CHX_SRCADDR_SRC_ADDR_Pos)        /*!<  */
#define DMA_CHX_SRCADDR_SRC_ADDR                        DMA_CHX_SRCADDR_SRC_ADDR_Msk                   /*!<  */
 
#define DMA_CHX_SRCADDR_ALL_Msk                    (DMA_CHX_SRCADDR_SRC_ADDR_Msk)
 
/******************  Bit definition for DMA_CHx_DESTADDR register ********************/
#define DMA_CHX_DESTADDR_DEST_ADDR_Pos                    (0U)
#define DMA_CHX_DESTADDR_DEST_ADDR_Msk                    (0xFFFFFFFFU << DMA_CHX_DESTADDR_DEST_ADDR_Pos)        /*!<  */
#define DMA_CHX_DESTADDR_DEST_ADDR                        DMA_CHX_DESTADDR_DEST_ADDR_Msk                   /*!<  */
 
#define DMA_CHX_DESTADDR_ALL_Msk                    (DMA_CHX_DESTADDR_DEST_ADDR_Msk)
 
/******************  Bit definition for DMA_CHx_CONTROL register ********************/
#define DMA_CHX_CONTROL_TS_Pos                    (0U)
#define DMA_CHX_CONTROL_TS_Msk                    (0xFFFU << DMA_CHX_CONTROL_TS_Pos)        /*!<  */
#define DMA_CHX_CONTROL_TS                        DMA_CHX_CONTROL_TS_Msk                   /*!<  */
 
#define DMA_CHX_CONTROL_CONTI_Pos                    (24U)
#define DMA_CHX_CONTROL_CONTI_Msk                    (0x01U << DMA_CHX_CONTROL_CONTI_Pos)        /*!<  */
#define DMA_CHX_CONTROL_CONTI                        DMA_CHX_CONTROL_CONTI_Msk                   /*!<  */
 
#define DMA_CHX_CONTROL_BRKEN_Pos                    (25U)
#define DMA_CHX_CONTROL_BRKEN_Msk                    (0x01U << DMA_CHX_CONTROL_BRKEN_Pos)        /*!<  */
#define DMA_CHX_CONTROL_BRKEN                        DMA_CHX_CONTROL_BRKEN_Msk                   /*!<  */
 
#define DMA_CHX_CONTROL_SINC_Pos                    (26U)
#define DMA_CHX_CONTROL_SINC_Msk                    (0x01U << DMA_CHX_CONTROL_SINC_Pos)        /*!<  */
#define DMA_CHX_CONTROL_SINC                        DMA_CHX_CONTROL_SINC_Msk                   /*!<  */
 
#define DMA_CHX_CONTROL_DINC_Pos                    (27U)
#define DMA_CHX_CONTROL_DINC_Msk                    (0x01U << DMA_CHX_CONTROL_DINC_Pos)        /*!<  */
#define DMA_CHX_CONTROL_DINC                        DMA_CHX_CONTROL_DINC_Msk                   /*!<  */
 
#define DMA_CHX_CONTROL_ALL_Msk                    (DMA_CHX_CONTROL_DINC_Msk | DMA_CHX_CONTROL_SINC_Msk | DMA_CHX_CONTROL_BRKEN_Msk | DMA_CHX_CONTROL_CONTI_Msk | DMA_CHX_CONTROL_TS_Msk)
 
/******************  Bit definition for DMA_CHx_CONFIG register ********************/
#define DMA_CHX_CONFIG_CH_EN_Pos                    (0U)
#define DMA_CHX_CONFIG_CH_EN_Msk                    (0x01U << DMA_CHX_CONFIG_CH_EN_Pos)        /*!<  */
#define DMA_CHX_CONFIG_CH_EN                        DMA_CHX_CONFIG_CH_EN_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_SRC_PERIPH_Pos                    (1U)
#define DMA_CHX_CONFIG_SRC_PERIPH_Msk                    (0xFU << DMA_CHX_CONFIG_SRC_PERIPH_Pos)        /*!<  */
#define DMA_CHX_CONFIG_SRC_PERIPH                        DMA_CHX_CONFIG_SRC_PERIPH_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_DEST_PERIPH_Pos                    (6U)
#define DMA_CHX_CONFIG_DEST_PERIPH_Msk                    (0xFU << DMA_CHX_CONFIG_DEST_PERIPH_Pos)        /*!<  */
#define DMA_CHX_CONFIG_DEST_PERIPH                        DMA_CHX_CONFIG_DEST_PERIPH_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_MODE_Pos                    (11U)
#define DMA_CHX_CONFIG_MODE_Msk                    (0x7U << DMA_CHX_CONFIG_MODE_Pos)        /*!<  */
#define DMA_CHX_CONFIG_MODE                        DMA_CHX_CONFIG_MODE_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_ERR_INT_MASK_Pos                    (14U)
#define DMA_CHX_CONFIG_ERR_INT_MASK_Msk                    (0x01U << DMA_CHX_CONFIG_ERR_INT_MASK_Pos)        /*!<  */
#define DMA_CHX_CONFIG_ERR_INT_MASK                        DMA_CHX_CONFIG_ERR_INT_MASK_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_TC_INT_MASK_Pos                    (15U)
#define DMA_CHX_CONFIG_TC_INT_MASK_Msk                    (0x01U << DMA_CHX_CONFIG_TC_INT_MASK_Pos)        /*!<  */
#define DMA_CHX_CONFIG_TC_INT_MASK                        DMA_CHX_CONFIG_TC_INT_MASK_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_ACTIVE_Pos                    (17U)
#define DMA_CHX_CONFIG_ACTIVE_Msk                    (0x01U << DMA_CHX_CONFIG_ACTIVE_Pos)        /*!<  */
#define DMA_CHX_CONFIG_ACTIVE                        DMA_CHX_CONFIG_ACTIVE_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_HALT_Pos                    (18U)
#define DMA_CHX_CONFIG_HALT_Msk                    (0x01U << DMA_CHX_CONFIG_HALT_Pos)        /*!<  */
#define DMA_CHX_CONFIG_HALT                        DMA_CHX_CONFIG_HALT_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_SRCCONTI_Pos                    (20U)
#define DMA_CHX_CONFIG_SRCCONTI_Msk                    (0x01U << DMA_CHX_CONFIG_SRCCONTI_Pos)        /*!<  */
#define DMA_CHX_CONFIG_SRCCONTI                        DMA_CHX_CONFIG_SRCCONTI_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_DESTCONTI_Pos                    (21U)
#define DMA_CHX_CONFIG_DESTCONTI_Msk                    (0x01U << DMA_CHX_CONFIG_DESTCONTI_Pos)        /*!<  */
#define DMA_CHX_CONFIG_DESTCONTI                        DMA_CHX_CONFIG_DESTCONTI_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_CONTI_CLR_Pos                    (23U)
#define DMA_CHX_CONFIG_CONTI_CLR_Msk                    (0x01U << DMA_CHX_CONFIG_CONTI_CLR_Pos)        /*!<  */
#define DMA_CHX_CONFIG_CONTI_CLR                        DMA_CHX_CONFIG_CONTI_CLR_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_CONTI_CNT_Pos                    (24U)
#define DMA_CHX_CONFIG_CONTI_CNT_Msk                    (0xFFU << DMA_CHX_CONFIG_CONTI_CNT_Pos)        /*!<  */
#define DMA_CHX_CONFIG_CONTI_CNT                        DMA_CHX_CONFIG_CONTI_CNT_Msk                   /*!<  */
 
#define DMA_CHX_CONFIG_ALL_Msk                    (DMA_CHX_CONFIG_CONTI_CNT_Msk | DMA_CHX_CONFIG_CONTI_CLR_Msk | DMA_CHX_CONFIG_DESTCONTI_Msk | DMA_CHX_CONFIG_SRCCONTI_Msk | DMA_CHX_CONFIG_HALT_Msk | DMA_CHX_CONFIG_ACTIVE_Msk | DMA_CHX_CONFIG_TC_INT_MASK_Msk | DMA_CHX_CONFIG_ERR_INT_MASK_Msk | DMA_CHX_CONFIG_MODE_Msk | DMA_CHX_CONFIG_DEST_PERIPH_Msk | DMA_CHX_CONFIG_SRC_PERIPH_Msk | DMA_CHX_CONFIG_CH_EN_Msk)
 
/******************  Bit definition for TIMx_CR1 register ********************/
#define TIMX_CR1_CEN_Pos                    (0U)
#define TIMX_CR1_CEN_Msk                    (0x01U << TIMX_CR1_CEN_Pos)        /*!<  */
#define TIMX_CR1_CEN                        TIMX_CR1_CEN_Msk                   /*!<  */
 
#define TIMX_CR1_UDIS_Pos                    (1U)
#define TIMX_CR1_UDIS_Msk                    (0x01U << TIMX_CR1_UDIS_Pos)        /*!<  */
#define TIMX_CR1_UDIS                        TIMX_CR1_UDIS_Msk                   /*!<  */
 
#define TIMX_CR1_URS_Pos                    (2U)
#define TIMX_CR1_URS_Msk                    (0x01U << TIMX_CR1_URS_Pos)        /*!<  */
#define TIMX_CR1_URS                        TIMX_CR1_URS_Msk                   /*!<  */
 
#define TIMX_CR1_OPM_Pos                    (3U)
#define TIMX_CR1_OPM_Msk                    (0x01U << TIMX_CR1_OPM_Pos)        /*!<  */
#define TIMX_CR1_OPM                        TIMX_CR1_OPM_Msk                   /*!<  */
 
#define TIMX_CR1_DIR_Pos                    (4U)
#define TIMX_CR1_DIR_Msk                    (0x01U << TIMX_CR1_DIR_Pos)        /*!<  */
#define TIMX_CR1_DIR                        TIMX_CR1_DIR_Msk                   /*!<  */
 
#define TIMX_CR1_CMS_Pos                    (5U)
#define TIMX_CR1_CMS_Msk                    (0x3U << TIMX_CR1_CMS_Pos)        /*!<  */
#define TIMX_CR1_CMS                        TIMX_CR1_CMS_Msk                   /*!<  */
 
#define TIMX_CR1_ARPE_Pos                    (7U)
#define TIMX_CR1_ARPE_Msk                    (0x01U << TIMX_CR1_ARPE_Pos)        /*!<  */
#define TIMX_CR1_ARPE                        TIMX_CR1_ARPE_Msk                   /*!<  */
 
#define TIMX_CR1_CKD_Pos                    (8U)
#define TIMX_CR1_CKD_Msk                    (0x3U << TIMX_CR1_CKD_Pos)        /*!<  */
#define TIMX_CR1_CKD                        TIMX_CR1_CKD_Msk                   /*!<  */
 
#define TIMX_CR1_ALL_Msk                    (TIMX_CR1_CKD_Msk | TIMX_CR1_ARPE_Msk | TIMX_CR1_CMS_Msk | TIMX_CR1_DIR_Msk | TIMX_CR1_OPM_Msk | TIMX_CR1_URS_Msk | TIMX_CR1_UDIS_Msk | TIMX_CR1_CEN_Msk)
 
/******************  Bit definition for TIMx_CR2 register ********************/
#define TIMX_CR2_CCPC_Pos                    (0U)
#define TIMX_CR2_CCPC_Msk                    (0x01U << TIMX_CR2_CCPC_Pos)        /*!<  */
#define TIMX_CR2_CCPC                        TIMX_CR2_CCPC_Msk                   /*!<  */
 
#define TIMX_CR2_CCUS_Pos                    (2U)
#define TIMX_CR2_CCUS_Msk                    (0x01U << TIMX_CR2_CCUS_Pos)        /*!<  */
#define TIMX_CR2_CCUS                        TIMX_CR2_CCUS_Msk                   /*!<  */
 
#define TIMX_CR2_CCDS_Pos                    (3U)
#define TIMX_CR2_CCDS_Msk                    (0x01U << TIMX_CR2_CCDS_Pos)        /*!<  */
#define TIMX_CR2_CCDS                        TIMX_CR2_CCDS_Msk                   /*!<  */
 
#define TIMX_CR2_MIMS_Pos                    (4U)
#define TIMX_CR2_MIMS_Msk                    (0x7U << TIMX_CR2_MIMS_Pos)        /*!<  */
#define TIMX_CR2_MIMS                        TIMX_CR2_MIMS_Msk                   /*!<  */
 
#define TIMX_CR2_TI1S_Pos                    (7U)
#define TIMX_CR2_TI1S_Msk                    (0x01U << TIMX_CR2_TI1S_Pos)        /*!<  */
#define TIMX_CR2_TI1S                        TIMX_CR2_TI1S_Msk                   /*!<  */
 
#define TIMX_CR2_OIS1_Pos                    (8U)
#define TIMX_CR2_OIS1_Msk                    (0x01U << TIMX_CR2_OIS1_Pos)        /*!<  */
#define TIMX_CR2_OIS1                        TIMX_CR2_OIS1_Msk                   /*!<  */
 
#define TIMX_CR2_OIS1N_Pos                    (9U)
#define TIMX_CR2_OIS1N_Msk                    (0x01U << TIMX_CR2_OIS1N_Pos)        /*!<  */
#define TIMX_CR2_OIS1N                        TIMX_CR2_OIS1N_Msk                   /*!<  */
 
#define TIMX_CR2_OIS2_Pos                    (10U)
#define TIMX_CR2_OIS2_Msk                    (0x01U << TIMX_CR2_OIS2_Pos)        /*!<  */
#define TIMX_CR2_OIS2                        TIMX_CR2_OIS2_Msk                   /*!<  */
 
#define TIMX_CR2_OIS2N_Pos                    (11U)
#define TIMX_CR2_OIS2N_Msk                    (0x01U << TIMX_CR2_OIS2N_Pos)        /*!<  */
#define TIMX_CR2_OIS2N                        TIMX_CR2_OIS2N_Msk                   /*!<  */
 
#define TIMX_CR2_OIS3_Pos                    (12U)
#define TIMX_CR2_OIS3_Msk                    (0x01U << TIMX_CR2_OIS3_Pos)        /*!<  */
#define TIMX_CR2_OIS3                        TIMX_CR2_OIS3_Msk                   /*!<  */
 
#define TIMX_CR2_OIS3N_Pos                    (13U)
#define TIMX_CR2_OIS3N_Msk                    (0x01U << TIMX_CR2_OIS3N_Pos)        /*!<  */
#define TIMX_CR2_OIS3N                        TIMX_CR2_OIS3N_Msk                   /*!<  */
 
#define TIMX_CR2_OIS4_Pos                    (14U)
#define TIMX_CR2_OIS4_Msk                    (0x01U << TIMX_CR2_OIS4_Pos)        /*!<  */
#define TIMX_CR2_OIS4                        TIMX_CR2_OIS4_Msk                   /*!<  */
 
#define TIMX_CR2_ALL_Msk                    (TIMX_CR2_OIS4_Msk | TIMX_CR2_OIS3N_Msk | TIMX_CR2_OIS3_Msk | TIMX_CR2_OIS2N_Msk | TIMX_CR2_OIS2_Msk | TIMX_CR2_OIS1N_Msk | TIMX_CR2_OIS1_Msk | TIMX_CR2_TI1S_Msk | TIMX_CR2_MIMS_Msk | TIMX_CR2_CCDS_Msk | TIMX_CR2_CCUS_Msk | TIMX_CR2_CCPC_Msk)
 
/******************  Bit definition for TIMx_SMCR register ********************/
#define TIMX_SMCR_SMS_Pos                    (0U)
#define TIMX_SMCR_SMS_Msk                    (0x7U << TIMX_SMCR_SMS_Pos)        /*!<  */
#define TIMX_SMCR_SMS                        TIMX_SMCR_SMS_Msk                   /*!<  */
 
#define TIMX_SMCR_TS_Pos                    (4U)
#define TIMX_SMCR_TS_Msk                    (0x7U << TIMX_SMCR_TS_Pos)        /*!<  */
#define TIMX_SMCR_TS                        TIMX_SMCR_TS_Msk                   /*!<  */
 
#define TIMX_SMCR_MSM_Pos                    (7U)
#define TIMX_SMCR_MSM_Msk                    (0x01U << TIMX_SMCR_MSM_Pos)        /*!<  */
#define TIMX_SMCR_MSM                        TIMX_SMCR_MSM_Msk                   /*!<  */
 
#define TIMX_SMCR_ETF_Pos                    (8U)
#define TIMX_SMCR_ETF_Msk                    (0xFU << TIMX_SMCR_ETF_Pos)        /*!<  */
#define TIMX_SMCR_ETF                        TIMX_SMCR_ETF_Msk                   /*!<  */
 
#define TIMX_SMCR_ETPS_Pos                    (12U)
#define TIMX_SMCR_ETPS_Msk                    (0x3U << TIMX_SMCR_ETPS_Pos)        /*!<  */
#define TIMX_SMCR_ETPS                        TIMX_SMCR_ETPS_Msk                   /*!<  */
 
#define TIMX_SMCR_ECE_Pos                    (14U)
#define TIMX_SMCR_ECE_Msk                    (0x01U << TIMX_SMCR_ECE_Pos)        /*!<  */
#define TIMX_SMCR_ECE                        TIMX_SMCR_ECE_Msk                   /*!<  */
 
#define TIMX_SMCR_ETP_Pos                    (15U)
#define TIMX_SMCR_ETP_Msk                    (0x01U << TIMX_SMCR_ETP_Pos)        /*!<  */
#define TIMX_SMCR_ETP                        TIMX_SMCR_ETP_Msk                   /*!<  */
 
#define TIMX_SMCR_ALL_Msk                    (TIMX_SMCR_ETP_Msk | TIMX_SMCR_ECE_Msk | TIMX_SMCR_ETPS_Msk | TIMX_SMCR_ETF_Msk | TIMX_SMCR_MSM_Msk | TIMX_SMCR_TS_Msk | TIMX_SMCR_SMS_Msk)
 
/******************  Bit definition for TIMx_DIER register ********************/
#define TIMX_DIER_UIE_Pos                    (0U)
#define TIMX_DIER_UIE_Msk                    (0x01U << TIMX_DIER_UIE_Pos)        /*!<  */
#define TIMX_DIER_UIE                        TIMX_DIER_UIE_Msk                   /*!<  */
 
#define TIMX_DIER_CC1IE_Pos                    (1U)
#define TIMX_DIER_CC1IE_Msk                    (0x01U << TIMX_DIER_CC1IE_Pos)        /*!<  */
#define TIMX_DIER_CC1IE                        TIMX_DIER_CC1IE_Msk                   /*!<  */
 
#define TIMX_DIER_CC2IE_Pos                    (2U)
#define TIMX_DIER_CC2IE_Msk                    (0x01U << TIMX_DIER_CC2IE_Pos)        /*!<  */
#define TIMX_DIER_CC2IE                        TIMX_DIER_CC2IE_Msk                   /*!<  */
 
#define TIMX_DIER_CC3IE_Pos                    (3U)
#define TIMX_DIER_CC3IE_Msk                    (0x01U << TIMX_DIER_CC3IE_Pos)        /*!<  */
#define TIMX_DIER_CC3IE                        TIMX_DIER_CC3IE_Msk                   /*!<  */
 
#define TIMX_DIER_CC4IE_Pos                    (4U)
#define TIMX_DIER_CC4IE_Msk                    (0x01U << TIMX_DIER_CC4IE_Pos)        /*!<  */
#define TIMX_DIER_CC4IE                        TIMX_DIER_CC4IE_Msk                   /*!<  */
 
#define TIMX_DIER_COMIE_Pos                    (5U)
#define TIMX_DIER_COMIE_Msk                    (0x01U << TIMX_DIER_COMIE_Pos)        /*!<  */
#define TIMX_DIER_COMIE                        TIMX_DIER_COMIE_Msk                   /*!<  */
 
#define TIMX_DIER_TIE_Pos                    (6U)
#define TIMX_DIER_TIE_Msk                    (0x01U << TIMX_DIER_TIE_Pos)        /*!<  */
#define TIMX_DIER_TIE                        TIMX_DIER_TIE_Msk                   /*!<  */
 
#define TIMX_DIER_BIE_Pos                    (7U)
#define TIMX_DIER_BIE_Msk                    (0x01U << TIMX_DIER_BIE_Pos)        /*!<  */
#define TIMX_DIER_BIE                        TIMX_DIER_BIE_Msk                   /*!<  */
 
#define TIMX_DIER_UDE_Pos                    (8U)
#define TIMX_DIER_UDE_Msk                    (0x01U << TIMX_DIER_UDE_Pos)        /*!<  */
#define TIMX_DIER_UDE                        TIMX_DIER_UDE_Msk                   /*!<  */
 
#define TIMX_DIER_CC1DE_Pos                    (9U)
#define TIMX_DIER_CC1DE_Msk                    (0x01U << TIMX_DIER_CC1DE_Pos)        /*!<  */
#define TIMX_DIER_CC1DE                        TIMX_DIER_CC1DE_Msk                   /*!<  */
 
#define TIMX_DIER_CC2DE_Pos                    (10U)
#define TIMX_DIER_CC2DE_Msk                    (0x01U << TIMX_DIER_CC2DE_Pos)        /*!<  */
#define TIMX_DIER_CC2DE                        TIMX_DIER_CC2DE_Msk                   /*!<  */
 
#define TIMX_DIER_CC3DE_Pos                    (11U)
#define TIMX_DIER_CC3DE_Msk                    (0x01U << TIMX_DIER_CC3DE_Pos)        /*!<  */
#define TIMX_DIER_CC3DE                        TIMX_DIER_CC3DE_Msk                   /*!<  */
 
#define TIMX_DIER_CC4DE_Pos                    (12U)
#define TIMX_DIER_CC4DE_Msk                    (0x01U << TIMX_DIER_CC4DE_Pos)        /*!<  */
#define TIMX_DIER_CC4DE                        TIMX_DIER_CC4DE_Msk                   /*!<  */
 
#define TIMX_DIER_COMDE_Pos                    (13U)
#define TIMX_DIER_COMDE_Msk                    (0x01U << TIMX_DIER_COMDE_Pos)        /*!<  */
#define TIMX_DIER_COMDE                        TIMX_DIER_COMDE_Msk                   /*!<  */
 
#define TIMX_DIER_TDE_Pos                    (14U)
#define TIMX_DIER_TDE_Msk                    (0x01U << TIMX_DIER_TDE_Pos)        /*!<  */
#define TIMX_DIER_TDE                        TIMX_DIER_TDE_Msk                   /*!<  */
 
#define TIMX_DIER_ALL_Msk                    (TIMX_DIER_TDE_Msk | TIMX_DIER_COMDE_Msk | TIMX_DIER_CC4DE_Msk | TIMX_DIER_CC3DE_Msk | TIMX_DIER_CC2DE_Msk | TIMX_DIER_CC1DE_Msk | TIMX_DIER_UDE_Msk | TIMX_DIER_BIE_Msk | TIMX_DIER_TIE_Msk | TIMX_DIER_COMIE_Msk | TIMX_DIER_CC4IE_Msk | TIMX_DIER_CC3IE_Msk | TIMX_DIER_CC2IE_Msk | TIMX_DIER_CC1IE_Msk | TIMX_DIER_UIE_Msk)
 
/******************  Bit definition for TIMx_SR register ********************/
#define TIMX_SR_UIF_Pos                    (0U)
#define TIMX_SR_UIF_Msk                    (0x01U << TIMX_SR_UIF_Pos)        /*!<  */
#define TIMX_SR_UIF                        TIMX_SR_UIF_Msk                   /*!<  */
 
#define TIMX_SR_CC1IF_Pos                    (1U)
#define TIMX_SR_CC1IF_Msk                    (0x01U << TIMX_SR_CC1IF_Pos)        /*!<  */
#define TIMX_SR_CC1IF                        TIMX_SR_CC1IF_Msk                   /*!<  */
 
#define TIMX_SR_CC2IF_Pos                    (2U)
#define TIMX_SR_CC2IF_Msk                    (0x01U << TIMX_SR_CC2IF_Pos)        /*!<  */
#define TIMX_SR_CC2IF                        TIMX_SR_CC2IF_Msk                   /*!<  */
 
#define TIMX_SR_CC3IF_Pos                    (3U)
#define TIMX_SR_CC3IF_Msk                    (0x01U << TIMX_SR_CC3IF_Pos)        /*!<  */
#define TIMX_SR_CC3IF                        TIMX_SR_CC3IF_Msk                   /*!<  */
 
#define TIMX_SR_CC4IF_Pos                    (4U)
#define TIMX_SR_CC4IF_Msk                    (0x01U << TIMX_SR_CC4IF_Pos)        /*!<  */
#define TIMX_SR_CC4IF                        TIMX_SR_CC4IF_Msk                   /*!<  */
 
#define TIMX_SR_COMIF_Pos                    (5U)
#define TIMX_SR_COMIF_Msk                    (0x01U << TIMX_SR_COMIF_Pos)        /*!<  */
#define TIMX_SR_COMIF                        TIMX_SR_COMIF_Msk                   /*!<  */
 
#define TIMX_SR_TIF_Pos                    (6U)
#define TIMX_SR_TIF_Msk                    (0x01U << TIMX_SR_TIF_Pos)        /*!<  */
#define TIMX_SR_TIF                        TIMX_SR_TIF_Msk                   /*!<  */
 
#define TIMX_SR_BIF_Pos                    (7U)
#define TIMX_SR_BIF_Msk                    (0x01U << TIMX_SR_BIF_Pos)        /*!<  */
#define TIMX_SR_BIF                        TIMX_SR_BIF_Msk                   /*!<  */
 
#define TIMX_SR_CC1OF_Pos                    (9U)
#define TIMX_SR_CC1OF_Msk                    (0x01U << TIMX_SR_CC1OF_Pos)        /*!<  */
#define TIMX_SR_CC1OF                        TIMX_SR_CC1OF_Msk                   /*!<  */
 
#define TIMX_SR_CC2OF_Pos                    (10U)
#define TIMX_SR_CC2OF_Msk                    (0x01U << TIMX_SR_CC2OF_Pos)        /*!<  */
#define TIMX_SR_CC2OF                        TIMX_SR_CC2OF_Msk                   /*!<  */
 
#define TIMX_SR_CC3OF_Pos                    (11U)
#define TIMX_SR_CC3OF_Msk                    (0x01U << TIMX_SR_CC3OF_Pos)        /*!<  */
#define TIMX_SR_CC3OF                        TIMX_SR_CC3OF_Msk                   /*!<  */
 
#define TIMX_SR_CC4OF_Pos                    (12U)
#define TIMX_SR_CC4OF_Msk                    (0x01U << TIMX_SR_CC4OF_Pos)        /*!<  */
#define TIMX_SR_CC4OF                        TIMX_SR_CC4OF_Msk                   /*!<  */
 
#define TIMX_SR_ALL_Msk                    (TIMX_SR_CC4OF_Msk | TIMX_SR_CC3OF_Msk | TIMX_SR_CC2OF_Msk | TIMX_SR_CC1OF_Msk | TIMX_SR_BIF_Msk | TIMX_SR_TIF_Msk | TIMX_SR_COMIF_Msk | TIMX_SR_CC4IF_Msk | TIMX_SR_CC3IF_Msk | TIMX_SR_CC2IF_Msk | TIMX_SR_CC1IF_Msk | TIMX_SR_UIF_Msk)
 
/******************  Bit definition for TIMx_EGR register ********************/
#define TIMX_EGR_UG_Pos                    (0U)
#define TIMX_EGR_UG_Msk                    (0x01U << TIMX_EGR_UG_Pos)        /*!<  */
#define TIMX_EGR_UG                        TIMX_EGR_UG_Msk                   /*!<  */
 
#define TIMX_EGR_CC1G_Pos                    (1U)
#define TIMX_EGR_CC1G_Msk                    (0x01U << TIMX_EGR_CC1G_Pos)        /*!<  */
#define TIMX_EGR_CC1G                        TIMX_EGR_CC1G_Msk                   /*!<  */
 
#define TIMX_EGR_CC2G_Pos                    (2U)
#define TIMX_EGR_CC2G_Msk                    (0x01U << TIMX_EGR_CC2G_Pos)        /*!<  */
#define TIMX_EGR_CC2G                        TIMX_EGR_CC2G_Msk                   /*!<  */
 
#define TIMX_EGR_CC3G_Pos                    (3U)
#define TIMX_EGR_CC3G_Msk                    (0x01U << TIMX_EGR_CC3G_Pos)        /*!<  */
#define TIMX_EGR_CC3G                        TIMX_EGR_CC3G_Msk                   /*!<  */
 
#define TIMX_EGR_CC4G_Pos                    (4U)
#define TIMX_EGR_CC4G_Msk                    (0x01U << TIMX_EGR_CC4G_Pos)        /*!<  */
#define TIMX_EGR_CC4G                        TIMX_EGR_CC4G_Msk                   /*!<  */
 
#define TIMX_EGR_COMG_Pos                    (5U)
#define TIMX_EGR_COMG_Msk                    (0x01U << TIMX_EGR_COMG_Pos)        /*!<  */
#define TIMX_EGR_COMG                        TIMX_EGR_COMG_Msk                   /*!<  */
 
#define TIMX_EGR_TG_Pos                    (6U)
#define TIMX_EGR_TG_Msk                    (0x01U << TIMX_EGR_TG_Pos)        /*!<  */
#define TIMX_EGR_TG                        TIMX_EGR_TG_Msk                   /*!<  */
 
#define TIMX_EGR_BG_Pos                    (7U)
#define TIMX_EGR_BG_Msk                    (0x01U << TIMX_EGR_BG_Pos)        /*!<  */
#define TIMX_EGR_BG                        TIMX_EGR_BG_Msk                   /*!<  */
 
#define TIMX_EGR_ALL_Msk                    (TIMX_EGR_BG_Msk | TIMX_EGR_TG_Msk | TIMX_EGR_COMG_Msk | TIMX_EGR_CC4G_Msk | TIMX_EGR_CC3G_Msk | TIMX_EGR_CC2G_Msk | TIMX_EGR_CC1G_Msk | TIMX_EGR_UG_Msk)
 
/******************  Bit definition for TIMx_CCMR1 register ********************/
#define TIMX_CCMR1_CC1S_Pos        (0U)
#define TIMX_CCMR1_CC1S_Msk        (0X3U << TIMX_CCMR1_CC1S_Pos)        /*!<  */   
#define TIMX_CCMR1_CC1S            TIMX_CCMR1_CC1S_Msk        /*!<  */ 

#define TIMX_CCMR1_IC1PS_Pos        (2U)
#define TIMX_CCMR1_IC1PS_Msk        (0X3U << TIMX_CCMR1_IC1PS_Pos)        /*!<  */   
#define TIMX_CCMR1_IC1PS            TIMX_CCMR1_IC1PS_Msk        /*!<  */ 

#define TIMX_CCMR1_IC1F_Pos        (4U)
#define TIMX_CCMR1_IC1F_Msk        (0XFU << TIMX_CCMR1_IC1F_Pos)        /*!<  */   
#define TIMX_CCMR1_IC1F            TIMX_CCMR1_IC1F_Msk        /*!<  */ 

#define TIMX_CCMR1_CC2S_Pos        (8U)
#define TIMX_CCMR1_CC2S_Msk        (0X3U << TIMX_CCMR1_CC2S_Pos)        /*!<  */   
#define TIMX_CCMR1_CC2S            TIMX_CCMR1_CC2S_Msk        /*!<  */ 

#define TIMX_CCMR1_IC2PS_Pos        (10U)
#define TIMX_CCMR1_IC2PS_Msk        (0X3U << TIMX_CCMR1_IC2PS_Pos)        /*!<  */   
#define TIMX_CCMR1_IC2PS            TIMX_CCMR1_IC2PS_Msk        /*!<  */ 

#define TIMX_CCMR1_IC2F_Pos        (12U)
#define TIMX_CCMR1_IC2F_Msk        (0XFU << TIMX_CCMR1_IC2F_Pos)        /*!<  */   
#define TIMX_CCMR1_IC2F            TIMX_CCMR1_IC2F_Msk        /*!<  */ 

#define TIMX_CCMR1_CC1S_Pos        (0U)
#define TIMX_CCMR1_CC1S_Msk        (0X3U << TIMX_CCMR1_CC1S_Pos)        /*!<  */   
#define TIMX_CCMR1_CC1S            TIMX_CCMR1_CC1S_Msk        /*!<  */ 

#define TIMX_CCMR1_OC1FE_Pos        (2U)
#define TIMX_CCMR1_OC1FE_Msk        (0X1U << TIMX_CCMR1_OC1FE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC1FE            TIMX_CCMR1_OC1FE_Msk        /*!<  */ 

#define TIMX_CCMR1_OC1PE_Pos        (3U)
#define TIMX_CCMR1_OC1PE_Msk        (0X1U << TIMX_CCMR1_OC1PE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC1PE            TIMX_CCMR1_OC1PE_Msk        /*!<  */ 

#define TIMX_CCMR1_OC1M_Pos        (4U)
#define TIMX_CCMR1_OC1M_Msk        (0X7U << TIMX_CCMR1_OC1M_Pos)        /*!<  */   
#define TIMX_CCMR1_OC1M            TIMX_CCMR1_OC1M_Msk        /*!<  */ 

#define TIMX_CCMR1_OC1CE_Pos        (7U)
#define TIMX_CCMR1_OC1CE_Msk        (0X1U << TIMX_CCMR1_OC1CE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC1CE            TIMX_CCMR1_OC1CE_Msk        /*!<  */ 

#define TIMX_CCMR1_CC2S_Pos        (8U)
#define TIMX_CCMR1_CC2S_Msk        (0X3U << TIMX_CCMR1_CC2S_Pos)        /*!<  */   
#define TIMX_CCMR1_CC2S            TIMX_CCMR1_CC2S_Msk        /*!<  */ 

#define TIMX_CCMR1_OC2FE_Pos        (10U)
#define TIMX_CCMR1_OC2FE_Msk        (0X1U << TIMX_CCMR1_OC2FE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC2FE            TIMX_CCMR1_OC2FE_Msk        /*!<  */ 

#define TIMX_CCMR1_OC2PE_Pos        (11U)
#define TIMX_CCMR1_OC2PE_Msk        (0X1U << TIMX_CCMR1_OC2PE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC2PE            TIMX_CCMR1_OC2PE_Msk        /*!<  */ 

#define TIMX_CCMR1_OC2M_Pos        (12U)
#define TIMX_CCMR1_OC2M_Msk        (0X7U << TIMX_CCMR1_OC2M_Pos)        /*!<  */   
#define TIMX_CCMR1_OC2M            TIMX_CCMR1_OC2M_Msk        /*!<  */ 

#define TIMX_CCMR1_OC2CE_Pos        (15U)
#define TIMX_CCMR1_OC2CE_Msk        (0X1U << TIMX_CCMR1_OC2CE_Pos)        /*!<  */   
#define TIMX_CCMR1_OC2CE            TIMX_CCMR1_OC2CE_Msk        /*!<  */ 

#define TIMX_CCMR1_ALL_Msk        (TIMX_CCMR1CC1S_Msk | TIMX_CCMR1IC1PS_Msk | TIMX_CCMR1IC1F_Msk | TIMX_CCMR1CC2S_Msk | TIMX_CCMR1IC2PS_Msk | TIMX_CCMR1IC2F_Msk | TIMX_CCMR1CC1S_Msk | TIMX_CCMR1OC1FE_Msk | TIMX_CCMR1OC1PE_Msk | TIMX_CCMR1OC1M_Msk | TIMX_CCMR1OC1CE_Msk | TIMX_CCMR1CC2S_Msk | TIMX_CCMR1OC2FE_Msk | TIMX_CCMR1OC2PE_Msk | TIMX_CCMR1OC2M_Msk | TIMX_CCMR1OC2CE_Msk ) 

/******************  Bit definition for TIMx_CCMR2 register ********************/
#define TIMX_CCMR2_CC3S_Pos        (0U)
#define TIMX_CCMR2_CC3S_Msk        (0X3U << TIMX_CCMR2_CC3S_Pos)        /*!<  */   
#define TIMX_CCMR2_CC3S            TIMX_CCMR2_CC3S_Msk        /*!<  */ 

#define TIMX_CCMR2_IC3PSC_Pos        (2U)
#define TIMX_CCMR2_IC3PSC_Msk        (0X3U << TIMX_CCMR2_IC3PSC_Pos)        /*!<  */   
#define TIMX_CCMR2_IC3PSC            TIMX_CCMR2_IC3PSC_Msk        /*!<  */ 

#define TIMX_CCMR2_IC3F_Pos        (4U)
#define TIMX_CCMR2_IC3F_Msk        (0XFU << TIMX_CCMR2_IC3F_Pos)        /*!<  */   
#define TIMX_CCMR2_IC3F            TIMX_CCMR2_IC3F_Msk        /*!<  */ 

#define TIMX_CCMR2_CC4S_Pos        (8U)
#define TIMX_CCMR2_CC4S_Msk        (0X3U << TIMX_CCMR2_CC4S_Pos)        /*!<  */   
#define TIMX_CCMR2_CC4S            TIMX_CCMR2_CC4S_Msk        /*!<  */ 

#define TIMX_CCMR2_IC4PSC_Pos        (10U)
#define TIMX_CCMR2_IC4PSC_Msk        (0X3U << TIMX_CCMR2_IC4PSC_Pos)        /*!<  */   
#define TIMX_CCMR2_IC4PSC            TIMX_CCMR2_IC4PSC_Msk        /*!<  */ 

#define TIMX_CCMR2_IC4F_Pos        (12U)
#define TIMX_CCMR2_IC4F_Msk        (0XFU << TIMX_CCMR2_IC4F_Pos)        /*!<  */   
#define TIMX_CCMR2_IC4F            TIMX_CCMR2_IC4F_Msk        /*!<  */ 

#define TIMX_CCMR2_CC3S_Pos        (0U)
#define TIMX_CCMR2_CC3S_Msk        (0X3U << TIMX_CCMR2_CC3S_Pos)        /*!<  */   
#define TIMX_CCMR2_CC3S            TIMX_CCMR2_CC3S_Msk        /*!<  */ 

#define TIMX_CCMR2_OC3FE_Pos        (2U)
#define TIMX_CCMR2_OC3FE_Msk        (0X1U << TIMX_CCMR2_OC3FE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC3FE            TIMX_CCMR2_OC3FE_Msk        /*!<  */ 

#define TIMX_CCMR2_OC3PE_Pos        (3U)
#define TIMX_CCMR2_OC3PE_Msk        (0X1U << TIMX_CCMR2_OC3PE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC3PE            TIMX_CCMR2_OC3PE_Msk        /*!<  */ 

#define TIMX_CCMR2_OC3M_Pos        (4U)
#define TIMX_CCMR2_OC3M_Msk        (0X7U << TIMX_CCMR2_OC3M_Pos)        /*!<  */   
#define TIMX_CCMR2_OC3M            TIMX_CCMR2_OC3M_Msk        /*!<  */ 

#define TIMX_CCMR2_OC3CE_Pos        (7U)
#define TIMX_CCMR2_OC3CE_Msk        (0X1U << TIMX_CCMR2_OC3CE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC3CE            TIMX_CCMR2_OC3CE_Msk        /*!<  */ 

#define TIMX_CCMR2_CC4S_Pos        (8U)
#define TIMX_CCMR2_CC4S_Msk        (0X3U << TIMX_CCMR2_CC4S_Pos)        /*!<  */   
#define TIMX_CCMR2_CC4S            TIMX_CCMR2_CC4S_Msk        /*!<  */ 

#define TIMX_CCMR2_OC4FE_Pos        (10U)
#define TIMX_CCMR2_OC4FE_Msk        (0X1U << TIMX_CCMR2_OC4FE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC4FE            TIMX_CCMR2_OC4FE_Msk        /*!<  */ 

#define TIMX_CCMR2_OC4PE_Pos        (11U)
#define TIMX_CCMR2_OC4PE_Msk        (0X1U << TIMX_CCMR2_OC4PE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC4PE            TIMX_CCMR2_OC4PE_Msk        /*!<  */ 

#define TIMX_CCMR2_OC4M_Pos        (12U)
#define TIMX_CCMR2_OC4M_Msk        (0X7U << TIMX_CCMR2_OC4M_Pos)        /*!<  */   
#define TIMX_CCMR2_OC4M            TIMX_CCMR2_OC4M_Msk        /*!<  */ 

#define TIMX_CCMR2_OC4CE_Pos        (15U)
#define TIMX_CCMR2_OC4CE_Msk        (0X1U << TIMX_CCMR2_OC4CE_Pos)        /*!<  */   
#define TIMX_CCMR2_OC4CE            TIMX_CCMR2_OC4CE_Msk        /*!<  */ 

#define TIMX_CCMR2_ALL_Msk        (TIMX_CCMR2CC3S_Msk | TIMX_CCMR2IC3PSC_Msk | TIMX_CCMR2IC3F_Msk | TIMX_CCMR2CC4S_Msk | TIMX_CCMR2IC4PSC_Msk | TIMX_CCMR2IC4F_Msk | TIMX_CCMR2CC3S_Msk | TIMX_CCMR2OC3FE_Msk | TIMX_CCMR2OC3PE_Msk | TIMX_CCMR2OC3M_Msk | TIMX_CCMR2OC3CE_Msk | TIMX_CCMR2CC4S_Msk | TIMX_CCMR2OC4FE_Msk | TIMX_CCMR2OC4PE_Msk | TIMX_CCMR2OC4M_Msk | TIMX_CCMR2OC4CE_Msk ) 

/******************  Bit definition for TIMx_CCER register ********************/
#define TIMX_CCER_CC1E_Pos                    (0U)
#define TIMX_CCER_CC1E_Msk                    (0x01U << TIMX_CCER_CC1E_Pos)        /*!<  */
#define TIMX_CCER_CC1E                        TIMX_CCER_CC1E_Msk                   /*!<  */
 
#define TIMX_CCER_CC1P_Pos                    (1U)
#define TIMX_CCER_CC1P_Msk                    (0x01U << TIMX_CCER_CC1P_Pos)        /*!<  */
#define TIMX_CCER_CC1P                        TIMX_CCER_CC1P_Msk                   /*!<  */
 
#define TIMX_CCER_CC1NE_Pos                    (2U)
#define TIMX_CCER_CC1NE_Msk                    (0x01U << TIMX_CCER_CC1NE_Pos)        /*!<  */
#define TIMX_CCER_CC1NE                        TIMX_CCER_CC1NE_Msk                   /*!<  */
 
#define TIMX_CCER_CC1NP_Pos                    (3U)
#define TIMX_CCER_CC1NP_Msk                    (0x01U << TIMX_CCER_CC1NP_Pos)        /*!<  */
#define TIMX_CCER_CC1NP                        TIMX_CCER_CC1NP_Msk                   /*!<  */
 
#define TIMX_CCER_CC2E_Pos                    (4U)
#define TIMX_CCER_CC2E_Msk                    (0x01U << TIMX_CCER_CC2E_Pos)        /*!<  */
#define TIMX_CCER_CC2E                        TIMX_CCER_CC2E_Msk                   /*!<  */
 
#define TIMX_CCER_CC2P_Pos                    (5U)
#define TIMX_CCER_CC2P_Msk                    (0x01U << TIMX_CCER_CC2P_Pos)        /*!<  */
#define TIMX_CCER_CC2P                        TIMX_CCER_CC2P_Msk                   /*!<  */
 
#define TIMX_CCER_CC2NE_Pos                    (6U)
#define TIMX_CCER_CC2NE_Msk                    (0x01U << TIMX_CCER_CC2NE_Pos)        /*!<  */
#define TIMX_CCER_CC2NE                        TIMX_CCER_CC2NE_Msk                   /*!<  */
 
#define TIMX_CCER_CC2NP_Pos                    (7U)
#define TIMX_CCER_CC2NP_Msk                    (0x01U << TIMX_CCER_CC2NP_Pos)        /*!<  */
#define TIMX_CCER_CC2NP                        TIMX_CCER_CC2NP_Msk                   /*!<  */
 
#define TIMX_CCER_CC3E_Pos                    (8U)
#define TIMX_CCER_CC3E_Msk                    (0x01U << TIMX_CCER_CC3E_Pos)        /*!<  */
#define TIMX_CCER_CC3E                        TIMX_CCER_CC3E_Msk                   /*!<  */
 
#define TIMX_CCER_CC3P_Pos                    (9U)
#define TIMX_CCER_CC3P_Msk                    (0x01U << TIMX_CCER_CC3P_Pos)        /*!<  */
#define TIMX_CCER_CC3P                        TIMX_CCER_CC3P_Msk                   /*!<  */
 
#define TIMX_CCER_CC3NE_Pos                    (10U)
#define TIMX_CCER_CC3NE_Msk                    (0x01U << TIMX_CCER_CC3NE_Pos)        /*!<  */
#define TIMX_CCER_CC3NE                        TIMX_CCER_CC3NE_Msk                   /*!<  */
 
#define TIMX_CCER_CC3NP_Pos                    (11U)
#define TIMX_CCER_CC3NP_Msk                    (0x01U << TIMX_CCER_CC3NP_Pos)        /*!<  */
#define TIMX_CCER_CC3NP                        TIMX_CCER_CC3NP_Msk                   /*!<  */
 
#define TIMX_CCER_CC4E_Pos                    (12U)
#define TIMX_CCER_CC4E_Msk                    (0x01U << TIMX_CCER_CC4E_Pos)        /*!<  */
#define TIMX_CCER_CC4E                        TIMX_CCER_CC4E_Msk                   /*!<  */
 
#define TIMX_CCER_CC4P_Pos                    (13U)
#define TIMX_CCER_CC4P_Msk                    (0x01U << TIMX_CCER_CC4P_Pos)        /*!<  */
#define TIMX_CCER_CC4P                        TIMX_CCER_CC4P_Msk                   /*!<  */
 
#define TIMX_CCER_ALL_Msk                    (TIMX_CCER_CC4P_Msk | TIMX_CCER_CC4E_Msk | TIMX_CCER_CC3NP_Msk | TIMX_CCER_CC3NE_Msk | TIMX_CCER_CC3P_Msk | TIMX_CCER_CC3E_Msk | TIMX_CCER_CC2NP_Msk | TIMX_CCER_CC2NE_Msk | TIMX_CCER_CC2P_Msk | TIMX_CCER_CC2E_Msk | TIMX_CCER_CC1NP_Msk | TIMX_CCER_CC1NE_Msk | TIMX_CCER_CC1P_Msk | TIMX_CCER_CC1E_Msk)
 
/******************  Bit definition for TIMx_CNT register ********************/
#define TIMX_CNT_CNT_Pos                    (0U)
#define TIMX_CNT_CNT_Msk                    (0xFFFFU << TIMX_CNT_CNT_Pos)        /*!<  */
#define TIMX_CNT_CNT                        TIMX_CNT_CNT_Msk                   /*!<  */
 
#define TIMX_CNT_ALL_Msk                    (TIMX_CNT_CNT_Msk)
 
/******************  Bit definition for TIMx_PSC register ********************/
#define TIMX_PSC_PSC_Pos                    (0U)
#define TIMX_PSC_PSC_Msk                    (0xFFFFU << TIMX_PSC_PSC_Pos)        /*!<  */
#define TIMX_PSC_PSC                        TIMX_PSC_PSC_Msk                   /*!<  */
 
#define TIMX_PSC_ALL_Msk                    (TIMX_PSC_PSC_Msk)
 
/******************  Bit definition for TIMx_ARR register ********************/
#define TIMX_ARR_ARR_Pos                    (0U)
#define TIMX_ARR_ARR_Msk                    (0xFFFFU << TIMX_ARR_ARR_Pos)        /*!<  */
#define TIMX_ARR_ARR                        TIMX_ARR_ARR_Msk                   /*!<  */
 
#define TIMX_ARR_ALL_Msk                    (TIMX_ARR_ARR_Msk)
 
/******************  Bit definition for TIMx_RCR register ********************/
#define TIMX_RCR_REP_Pos                    (0U)
#define TIMX_RCR_REP_Msk                    (0xFFU << TIMX_RCR_REP_Pos)        /*!<  */
#define TIMX_RCR_REP                        TIMX_RCR_REP_Msk                   /*!<  */
 
#define TIMX_RCR_ALL_Msk                    (TIMX_RCR_REP_Msk)
 
/******************  Bit definition for TIMx_CCR1 register ********************/
#define TIMX_CCR1_CCR1_Pos                    (0U)
#define TIMX_CCR1_CCR1_Msk                    (0xFFFFU << TIMX_CCR1_CCR1_Pos)        /*!<  */
#define TIMX_CCR1_CCR1                        TIMX_CCR1_CCR1_Msk                   /*!<  */
 
#define TIMX_CCR1_ALL_Msk                    (TIMX_CCR1_CCR1_Msk)
 
/******************  Bit definition for TIMx_CCR2 register ********************/
#define TIMX_CCR2_CCR2_Pos                    (0U)
#define TIMX_CCR2_CCR2_Msk                    (0xFFFFU << TIMX_CCR2_CCR2_Pos)        /*!<  */
#define TIMX_CCR2_CCR2                        TIMX_CCR2_CCR2_Msk                   /*!<  */
 
#define TIMX_CCR2_ALL_Msk                    (TIMX_CCR2_CCR2_Msk)
 
/******************  Bit definition for TIMx_CCR3 register ********************/
#define TIMX_CCR3_CCR3_Pos                    (0U)
#define TIMX_CCR3_CCR3_Msk                    (0xFFFFU << TIMX_CCR3_CCR3_Pos)        /*!<  */
#define TIMX_CCR3_CCR3                        TIMX_CCR3_CCR3_Msk                   /*!<  */
 
#define TIMX_CCR3_ALL_Msk                    (TIMX_CCR3_CCR3_Msk)
 
/******************  Bit definition for TIMx_CCR4 register ********************/
#define TIMX_CCR4_CCR4_Pos                    (0U)
#define TIMX_CCR4_CCR4_Msk                    (0xFFFFU << TIMX_CCR4_CCR4_Pos)        /*!<  */
#define TIMX_CCR4_CCR4                        TIMX_CCR4_CCR4_Msk                   /*!<  */
 
#define TIMX_CCR4_ALL_Msk                    (TIMX_CCR4_CCR4_Msk)
 
/******************  Bit definition for TIMx_BDTR4 register ********************/
#define TIMX_BDTR4_DTG_Pos                    (0U)
#define TIMX_BDTR4_DTG_Msk                    (0xFFU << TIMX_BDTR4_DTG_Pos)        /*!<  */
#define TIMX_BDTR4_DTG                        TIMX_BDTR4_DTG_Msk                   /*!<  */
 
#define TIMX_BDTR4_LOCK_Pos                    (8U)
#define TIMX_BDTR4_LOCK_Msk                    (0x3U << TIMX_BDTR4_LOCK_Pos)        /*!<  */
#define TIMX_BDTR4_LOCK                        TIMX_BDTR4_LOCK_Msk                   /*!<  */
 
#define TIMX_BDTR4_OSSI_Pos                    (10U)
#define TIMX_BDTR4_OSSI_Msk                    (0x01U << TIMX_BDTR4_OSSI_Pos)        /*!<  */
#define TIMX_BDTR4_OSSI                        TIMX_BDTR4_OSSI_Msk                   /*!<  */
 
#define TIMX_BDTR4_OSSR_Pos                    (11U)
#define TIMX_BDTR4_OSSR_Msk                    (0x01U << TIMX_BDTR4_OSSR_Pos)        /*!<  */
#define TIMX_BDTR4_OSSR                        TIMX_BDTR4_OSSR_Msk                   /*!<  */
 
#define TIMX_BDTR4_BKE_Pos                    (12U)
#define TIMX_BDTR4_BKE_Msk                    (0x01U << TIMX_BDTR4_BKE_Pos)        /*!<  */
#define TIMX_BDTR4_BKE                        TIMX_BDTR4_BKE_Msk                   /*!<  */
 
#define TIMX_BDTR4_BKP_Pos                    (13U)
#define TIMX_BDTR4_BKP_Msk                    (0x01U << TIMX_BDTR4_BKP_Pos)        /*!<  */
#define TIMX_BDTR4_BKP                        TIMX_BDTR4_BKP_Msk                   /*!<  */
 
#define TIMX_BDTR4_AOE_Pos                    (14U)
#define TIMX_BDTR4_AOE_Msk                    (0x01U << TIMX_BDTR4_AOE_Pos)        /*!<  */
#define TIMX_BDTR4_AOE                        TIMX_BDTR4_AOE_Msk                   /*!<  */
 
#define TIMX_BDTR4_MOE_Pos                    (15U)
#define TIMX_BDTR4_MOE_Msk                    (0x01U << TIMX_BDTR4_MOE_Pos)        /*!<  */
#define TIMX_BDTR4_MOE                        TIMX_BDTR4_MOE_Msk                   /*!<  */
 
#define TIMX_BDTR4_ALL_Msk                    (TIMX_BDTR4_MOE_Msk | TIMX_BDTR4_AOE_Msk | TIMX_BDTR4_BKP_Msk | TIMX_BDTR4_BKE_Msk | TIMX_BDTR4_OSSR_Msk | TIMX_BDTR4_OSSI_Msk | TIMX_BDTR4_LOCK_Msk | TIMX_BDTR4_DTG_Msk)
 
/******************  Bit definition for TIMx_DCR register ********************/
#define TIMX_DCR_DBA_Pos                    (0U)
#define TIMX_DCR_DBA_Msk                    (0x1FU << TIMX_DCR_DBA_Pos)        /*!<  */
#define TIMX_DCR_DBA                        TIMX_DCR_DBA_Msk                   /*!<  */
 
#define TIMX_DCR_DBL_Pos                    (8U)
#define TIMX_DCR_DBL_Msk                    (0x1FU << TIMX_DCR_DBL_Pos)        /*!<  */
#define TIMX_DCR_DBL                        TIMX_DCR_DBL_Msk                   /*!<  */
 
#define TIMX_DCR_ALL_Msk                    (TIMX_DCR_DBL_Msk | TIMX_DCR_DBA_Msk)
 
/******************  Bit definition for TIMx_DMAR register ********************/
#define TIMX_DMAR_DMAB_Pos                    (0U)
#define TIMX_DMAR_DMAB_Msk                    (0xFFFFFFFFU << TIMX_DMAR_DMAB_Pos)        /*!<  */
#define TIMX_DMAR_DMAB                        TIMX_DMAR_DMAB_Msk                   /*!<  */
 
#define TIMX_DMAR_ALL_Msk                    (TIMX_DMAR_DMAB_Msk)
 
/******************  Bit definition for IWDG_KR register ********************/
#define IWDG_KR_KEY_Pos                    (0U)
#define IWDG_KR_KEY_Msk                    (0xFFFFU << IWDG_KR_KEY_Pos)        /*!<  */
#define IWDG_KR_KEY                        IWDG_KR_KEY_Msk                   /*!<  */
 
#define IWDG_KR_ALL_Msk                    (IWDG_KR_KEY_Msk)
 
/******************  Bit definition for IWDG_PR register ********************/
#define IWDG_PR_PR_Pos                    (0U)
#define IWDG_PR_PR_Msk                    (0x7U << IWDG_PR_PR_Pos)        /*!<  */
#define IWDG_PR_PR                        IWDG_PR_PR_Msk                   /*!<  */
 
#define IWDG_PR_ALL_Msk                    (IWDG_PR_PR_Msk)
 
/******************  Bit definition for IWDG_RLR register ********************/
#define IWDG_RLR_RL_Pos                    (0U)
#define IWDG_RLR_RL_Msk                    (0xFFFU << IWDG_RLR_RL_Pos)        /*!<  */
#define IWDG_RLR_RL                        IWDG_RLR_RL_Msk                   /*!<  */
 
#define IWDG_RLR_ALL_Msk                    (IWDG_RLR_RL_Msk)
 
/******************  Bit definition for IWDG_SR register ********************/
#define IWDG_SR_PVU_Pos                    (0U)
#define IWDG_SR_PVU_Msk                    (0x01U << IWDG_SR_PVU_Pos)        /*!<  */
#define IWDG_SR_PVU                        IWDG_SR_PVU_Msk                   /*!<  */
 
#define IWDG_SR_RVU_Pos                    (1U)
#define IWDG_SR_RVU_Msk                    (0x01U << IWDG_SR_RVU_Pos)        /*!<  */
#define IWDG_SR_RVU                        IWDG_SR_RVU_Msk                   /*!<  */
 
#define IWDG_SR_ALL_Msk                    (IWDG_SR_RVU_Msk | IWDG_SR_PVU_Msk)
 
/******************  Bit definition for WWDG_CR register ********************/
#define WWDG_CR_T_Pos                    (0U)
#define WWDG_CR_T_Msk                    (0x7FU << WWDG_CR_T_Pos)        /*!<  */
#define WWDG_CR_T                        WWDG_CR_T_Msk                   /*!<  */
 
#define WWDG_CR_WDGA_Pos                    (7U)
#define WWDG_CR_WDGA_Msk                    (0x01U << WWDG_CR_WDGA_Pos)        /*!<  */
#define WWDG_CR_WDGA                        WWDG_CR_WDGA_Msk                   /*!<  */
 
#define WWDG_CR_ALL_Msk                    (WWDG_CR_WDGA_Msk | WWDG_CR_T_Msk)
 
/******************  Bit definition for WWDG_CFR register ********************/
#define WWDG_CFR_W_Pos                    (0U)
#define WWDG_CFR_W_Msk                    (0x7FU << WWDG_CFR_W_Pos)        /*!<  */
#define WWDG_CFR_W                        WWDG_CFR_W_Msk                   /*!<  */
 
#define WWDG_CFR_WDGTB_Pos                    (7U)
#define WWDG_CFR_WDGTB_Msk                    (0x3U << WWDG_CFR_WDGTB_Pos)        /*!<  */
#define WWDG_CFR_WDGTB                        WWDG_CFR_WDGTB_Msk                   /*!<  */
 
#define WWDG_CFR_EWI_Pos                    (9U)
#define WWDG_CFR_EWI_Msk                    (0x01U << WWDG_CFR_EWI_Pos)        /*!<  */
#define WWDG_CFR_EWI                        WWDG_CFR_EWI_Msk                   /*!<  */
 
#define WWDG_CFR_ALL_Msk                    (WWDG_CFR_EWI_Msk | WWDG_CFR_WDGTB_Msk | WWDG_CFR_W_Msk)
 
/******************  Bit definition for WWDG_SR register ********************/
#define WWDG_SR_EWIF_Pos                    (0U)
#define WWDG_SR_EWIF_Msk                    (0x01U << WWDG_SR_EWIF_Pos)        /*!<  */
#define WWDG_SR_EWIF                        WWDG_SR_EWIF_Msk                   /*!<  */
 
#define WWDG_SR_ALL_Msk                    (WWDG_SR_EWIF_Msk)
 
/******************  Bit definition for I2C_CON register ********************/
#define I2C_CON_MASTER_MODE_Pos                    (0U)
#define I2C_CON_MASTER_MODE_Msk                    (0x01U << I2C_CON_MASTER_MODE_Pos)        /*!<  */
#define I2C_CON_MASTER_MODE                        I2C_CON_MASTER_MODE_Msk                   /*!<  */
 
#define I2C_CON_SPEED_Pos                    (1U)
#define I2C_CON_SPEED_Msk                    (0x3U << I2C_CON_SPEED_Pos)        /*!<  */
#define I2C_CON_SPEED                        I2C_CON_SPEED_Msk                   /*!<  */
 
#define I2C_CON_10BITADDR_SLAVE_Pos                    (3U)
#define I2C_CON_10BITADDR_SLAVE_Msk                    (0x01U << I2C_CON_10BITADDR_SLAVE_Pos)        /*!<  */
#define I2C_CON_10BITADDR_SLAVE                        I2C_CON_10BITADDR_SLAVE_Msk                   /*!<  */
 
#define I2C_CON_10BITADDR_MASTER_Pos                    (4U)
#define I2C_CON_10BITADDR_MASTER_Msk                    (0x01U << I2C_CON_10BITADDR_MASTER_Pos)        /*!<  */
#define I2C_CON_10BITADDR_MASTER                        I2C_CON_10BITADDR_MASTER_Msk                   /*!<  */
 
#define I2C_CON_RESTART_EN_Pos                    (5U)
#define I2C_CON_RESTART_EN_Msk                    (0x01U << I2C_CON_RESTART_EN_Pos)        /*!<  */
#define I2C_CON_RESTART_EN                        I2C_CON_RESTART_EN_Msk                   /*!<  */
 
#define I2C_CON_SLAVE_DISABLE_Pos                    (6U)
#define I2C_CON_SLAVE_DISABLE_Msk                    (0x01U << I2C_CON_SLAVE_DISABLE_Pos)        /*!<  */
#define I2C_CON_SLAVE_DISABLE                        I2C_CON_SLAVE_DISABLE_Msk                   /*!<  */
 
#define I2C_CON_STOP_DET_IFADDRESSED_Pos                    (7U)
#define I2C_CON_STOP_DET_IFADDRESSED_Msk                    (0x01U << I2C_CON_STOP_DET_IFADDRESSED_Pos)        /*!<  */
#define I2C_CON_STOP_DET_IFADDRESSED                        I2C_CON_STOP_DET_IFADDRESSED_Msk                   /*!<  */
 
#define I2C_CON_TX_EMPTY_CTRL_Pos                    (8U)
#define I2C_CON_TX_EMPTY_CTRL_Msk                    (0x01U << I2C_CON_TX_EMPTY_CTRL_Pos)        /*!<  */
#define I2C_CON_TX_EMPTY_CTRL                        I2C_CON_TX_EMPTY_CTRL_Msk                   /*!<  */
 
#define I2C_CON_RX_FIFO_FULL_HLD_CTRL_Pos                    (9U)
#define I2C_CON_RX_FIFO_FULL_HLD_CTRL_Msk                    (0x01U << I2C_CON_RX_FIFO_FULL_HLD_CTRL_Pos)        /*!<  */
#define I2C_CON_RX_FIFO_FULL_HLD_CTRL                        I2C_CON_RX_FIFO_FULL_HLD_CTRL_Msk                   /*!<  */
 
#define I2C_CON_BUS_CLEAR_FEATURE_CTRL_Pos                    (11U)
#define I2C_CON_BUS_CLEAR_FEATURE_CTRL_Msk                    (0x01U << I2C_CON_BUS_CLEAR_FEATURE_CTRL_Pos)        /*!<  */
#define I2C_CON_BUS_CLEAR_FEATURE_CTRL                        I2C_CON_BUS_CLEAR_FEATURE_CTRL_Msk                   /*!<  */
 
#define I2C_CON_SAR_DUAL_EN_Pos                    (12U)
#define I2C_CON_SAR_DUAL_EN_Msk                    (0x01U << I2C_CON_SAR_DUAL_EN_Pos)        /*!<  */
#define I2C_CON_SAR_DUAL_EN                        I2C_CON_SAR_DUAL_EN_Msk                   /*!<  */
 
#define I2C_CON_SMBUS_ARP_EN_Pos                    (18U)
#define I2C_CON_SMBUS_ARP_EN_Msk                    (0x01U << I2C_CON_SMBUS_ARP_EN_Pos)        /*!<  */
#define I2C_CON_SMBUS_ARP_EN                        I2C_CON_SMBUS_ARP_EN_Msk                   /*!<  */
 
#define I2C_CON_ALL_Msk                    (I2C_CON_SMBUS_ARP_EN_Msk | I2C_CON_SAR_DUAL_EN_Msk | I2C_CON_BUS_CLEAR_FEATURE_CTRL_Msk | I2C_CON_RX_FIFO_FULL_HLD_CTRL_Msk | I2C_CON_TX_EMPTY_CTRL_Msk | I2C_CON_STOP_DET_IFADDRESSED_Msk | I2C_CON_SLAVE_DISABLE_Msk | I2C_CON_RESTART_EN_Msk | I2C_CON_10BITADDR_MASTER_Msk | I2C_CON_10BITADDR_SLAVE_Msk | I2C_CON_SPEED_Msk | I2C_CON_MASTER_MODE_Msk)
 
/******************  Bit definition for I2C_TAR register ********************/
#define I2C_TAR_TAR_Pos                    (0U)
#define I2C_TAR_TAR_Msk                    (0x3FFU << I2C_TAR_TAR_Pos)        /*!<  */
#define I2C_TAR_TAR                        I2C_TAR_TAR_Msk                   /*!<  */
 
#define I2C_TAR_GC_OR_START_Pos                    (10U)
#define I2C_TAR_GC_OR_START_Msk                    (0x01U << I2C_TAR_GC_OR_START_Pos)        /*!<  */
#define I2C_TAR_GC_OR_START                        I2C_TAR_GC_OR_START_Msk                   /*!<  */
 
#define I2C_TAR_SPECIAL_Pos                    (11U)
#define I2C_TAR_SPECIAL_Msk                    (0x01U << I2C_TAR_SPECIAL_Pos)        /*!<  */
#define I2C_TAR_SPECIAL                        I2C_TAR_SPECIAL_Msk                   /*!<  */
 
#define I2C_TAR_SMBUS_QUICK_CMD_Pos                    (16U)
#define I2C_TAR_SMBUS_QUICK_CMD_Msk                    (0x01U << I2C_TAR_SMBUS_QUICK_CMD_Pos)        /*!<  */
#define I2C_TAR_SMBUS_QUICK_CMD                        I2C_TAR_SMBUS_QUICK_CMD_Msk                   /*!<  */
 
#define I2C_TAR_ALL_Msk                    (I2C_TAR_SMBUS_QUICK_CMD_Msk | I2C_TAR_SPECIAL_Msk | I2C_TAR_GC_OR_START_Msk | I2C_TAR_TAR_Msk)
 
/******************  Bit definition for I2C_SAR register ********************/
#define I2C_SAR_IC_SAR_Pos                    (0U)
#define I2C_SAR_IC_SAR_Msk                    (0x3FFU << I2C_SAR_IC_SAR_Pos)        /*!<  */
#define I2C_SAR_IC_SAR                        I2C_SAR_IC_SAR_Msk                   /*!<  */
 
#define I2C_SAR_IC_SAR_DUAL_Pos                    (16U)
#define I2C_SAR_IC_SAR_DUAL_Msk                    (0x7FU << I2C_SAR_IC_SAR_DUAL_Pos)        /*!<  */
#define I2C_SAR_IC_SAR_DUAL                        I2C_SAR_IC_SAR_DUAL_Msk                   /*!<  */
 
#define I2C_SAR_ALL_Msk                    (I2C_SAR_IC_SAR_DUAL_Msk | I2C_SAR_IC_SAR_Msk)
 
/******************  Bit definition for I2C_DATA_CMD register ********************/
#define I2C_DATA_CMD_DAT_Pos                    (0U)
#define I2C_DATA_CMD_DAT_Msk                    (0xFFU << I2C_DATA_CMD_DAT_Pos)        /*!<  */
#define I2C_DATA_CMD_DAT                        I2C_DATA_CMD_DAT_Msk                   /*!<  */
 
#define I2C_DATA_CMD_CMD_Pos                    (8U)
#define I2C_DATA_CMD_CMD_Msk                    (0x01U << I2C_DATA_CMD_CMD_Pos)        /*!<  */
#define I2C_DATA_CMD_CMD                        I2C_DATA_CMD_CMD_Msk                   /*!<  */
 
#define I2C_DATA_CMD_STOP_Pos                    (9U)
#define I2C_DATA_CMD_STOP_Msk                    (0x01U << I2C_DATA_CMD_STOP_Pos)        /*!<  */
#define I2C_DATA_CMD_STOP                        I2C_DATA_CMD_STOP_Msk                   /*!<  */
 
#define I2C_DATA_CMD_RESTART_Pos                    (10U)
#define I2C_DATA_CMD_RESTART_Msk                    (0x01U << I2C_DATA_CMD_RESTART_Pos)        /*!<  */
#define I2C_DATA_CMD_RESTART                        I2C_DATA_CMD_RESTART_Msk                   /*!<  */
 
#define I2C_DATA_CMD_FIRST_DATA_BYTE_Pos                    (11U)
#define I2C_DATA_CMD_FIRST_DATA_BYTE_Msk                    (0x01U << I2C_DATA_CMD_FIRST_DATA_BYTE_Pos)        /*!<  */
#define I2C_DATA_CMD_FIRST_DATA_BYTE                        I2C_DATA_CMD_FIRST_DATA_BYTE_Msk                   /*!<  */
 
#define I2C_DATA_CMD_ALL_Msk                    (I2C_DATA_CMD_FIRST_DATA_BYTE_Msk | I2C_DATA_CMD_RESTART_Msk | I2C_DATA_CMD_STOP_Msk | I2C_DATA_CMD_CMD_Msk | I2C_DATA_CMD_DAT_Msk)
 
/******************  Bit definition for I2C_SS_SCL_HCNT register ********************/
#define I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT_Pos                    (0U)
#define I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT_Msk                    (0xFFFFU << I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT_Pos)        /*!<  */
#define I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT                        I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT_Msk                   /*!<  */
 
#define I2C_SS_SCL_HCNT_ALL_Msk                    (I2C_SS_SCL_HCNT_IC_SS_SCL_HCNT_Msk)
 
/******************  Bit definition for I2C_SS_SCL_SCNT register ********************/
#define I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT_Pos                    (0U)
#define I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT_Msk                    (0xFFFFU << I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT_Pos)        /*!<  */
#define I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT                        I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT_Msk                   /*!<  */
 
#define I2C_SS_SCL_SCNT_ALL_Msk                    (I2C_SS_SCL_SCNT_IC_SS_SCL_LCNT_Msk)
 
/******************  Bit definition for I2C_FS_SCL_HCNT register ********************/
#define I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT_Pos                    (0U)
#define I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT_Msk                    (0xFFFFU << I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT_Pos)        /*!<  */
#define I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT                        I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT_Msk                   /*!<  */
 
#define I2C_FS_SCL_HCNT_ALL_Msk                    (I2C_FS_SCL_HCNT_IC_FS_SCL_HCNT_Msk)
 
/******************  Bit definition for I2C_FS_SCL_LCNT register ********************/
#define I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT_Pos                    (0U)
#define I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT_Msk                    (0xFFFFU << I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT_Pos)        /*!<  */
#define I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT                        I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT_Msk                   /*!<  */
 
#define I2C_FS_SCL_LCNT_ALL_Msk                    (I2C_FS_SCL_LCNT_IC_FS_SCL_LCNT_Msk)
 
/******************  Bit definition for I2C_INTR_STAT register ********************/
#define I2C_INTR_STAT_R_RX_UNDER_Pos                    (0U)
#define I2C_INTR_STAT_R_RX_UNDER_Msk                    (0x01U << I2C_INTR_STAT_R_RX_UNDER_Pos)        /*!<  */
#define I2C_INTR_STAT_R_RX_UNDER                        I2C_INTR_STAT_R_RX_UNDER_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_RX_OVER_Pos                    (1U)
#define I2C_INTR_STAT_R_RX_OVER_Msk                    (0x01U << I2C_INTR_STAT_R_RX_OVER_Pos)        /*!<  */
#define I2C_INTR_STAT_R_RX_OVER                        I2C_INTR_STAT_R_RX_OVER_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_RX_FULL_Pos                    (2U)
#define I2C_INTR_STAT_R_RX_FULL_Msk                    (0x01U << I2C_INTR_STAT_R_RX_FULL_Pos)        /*!<  */
#define I2C_INTR_STAT_R_RX_FULL                        I2C_INTR_STAT_R_RX_FULL_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_TX_OVER_Pos                    (3U)
#define I2C_INTR_STAT_R_TX_OVER_Msk                    (0x01U << I2C_INTR_STAT_R_TX_OVER_Pos)        /*!<  */
#define I2C_INTR_STAT_R_TX_OVER                        I2C_INTR_STAT_R_TX_OVER_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_TX_EMPTY_Pos                    (4U)
#define I2C_INTR_STAT_R_TX_EMPTY_Msk                    (0x01U << I2C_INTR_STAT_R_TX_EMPTY_Pos)        /*!<  */
#define I2C_INTR_STAT_R_TX_EMPTY                        I2C_INTR_STAT_R_TX_EMPTY_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_RD_REQ_Pos                    (5U)
#define I2C_INTR_STAT_R_RD_REQ_Msk                    (0x01U << I2C_INTR_STAT_R_RD_REQ_Pos)        /*!<  */
#define I2C_INTR_STAT_R_RD_REQ                        I2C_INTR_STAT_R_RD_REQ_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_TX_ABRT_Pos                    (6U)
#define I2C_INTR_STAT_R_TX_ABRT_Msk                    (0x01U << I2C_INTR_STAT_R_TX_ABRT_Pos)        /*!<  */
#define I2C_INTR_STAT_R_TX_ABRT                        I2C_INTR_STAT_R_TX_ABRT_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_RX_DONE_Pos                    (7U)
#define I2C_INTR_STAT_R_RX_DONE_Msk                    (0x01U << I2C_INTR_STAT_R_RX_DONE_Pos)        /*!<  */
#define I2C_INTR_STAT_R_RX_DONE                        I2C_INTR_STAT_R_RX_DONE_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_ACTIVITY_Pos                    (8U)
#define I2C_INTR_STAT_R_ACTIVITY_Msk                    (0x01U << I2C_INTR_STAT_R_ACTIVITY_Pos)        /*!<  */
#define I2C_INTR_STAT_R_ACTIVITY                        I2C_INTR_STAT_R_ACTIVITY_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_STOP_DET_Pos                    (9U)
#define I2C_INTR_STAT_R_STOP_DET_Msk                    (0x01U << I2C_INTR_STAT_R_STOP_DET_Pos)        /*!<  */
#define I2C_INTR_STAT_R_STOP_DET                        I2C_INTR_STAT_R_STOP_DET_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_START_DET_Pos                    (10U)
#define I2C_INTR_STAT_R_START_DET_Msk                    (0x01U << I2C_INTR_STAT_R_START_DET_Pos)        /*!<  */
#define I2C_INTR_STAT_R_START_DET                        I2C_INTR_STAT_R_START_DET_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_GEN_CALL_Pos                    (11U)
#define I2C_INTR_STAT_R_GEN_CALL_Msk                    (0x01U << I2C_INTR_STAT_R_GEN_CALL_Pos)        /*!<  */
#define I2C_INTR_STAT_R_GEN_CALL                        I2C_INTR_STAT_R_GEN_CALL_Msk                   /*!<  */
 
#define I2C_INTR_STAT_R_SCL_STUCK_AT_LOW_Pos                    (14U)
#define I2C_INTR_STAT_R_SCL_STUCK_AT_LOW_Msk                    (0x01U << I2C_INTR_STAT_R_SCL_STUCK_AT_LOW_Pos)        /*!<  */
#define I2C_INTR_STAT_R_SCL_STUCK_AT_LOW                        I2C_INTR_STAT_R_SCL_STUCK_AT_LOW_Msk                   /*!<  */
 
#define I2C_INTR_STAT_ALL_Msk                    (I2C_INTR_STAT_R_SCL_STUCK_AT_LOW_Msk | I2C_INTR_STAT_R_GEN_CALL_Msk | I2C_INTR_STAT_R_START_DET_Msk | I2C_INTR_STAT_R_STOP_DET_Msk | I2C_INTR_STAT_R_ACTIVITY_Msk | I2C_INTR_STAT_R_RX_DONE_Msk | I2C_INTR_STAT_R_TX_ABRT_Msk | I2C_INTR_STAT_R_RD_REQ_Msk | I2C_INTR_STAT_R_TX_EMPTY_Msk | I2C_INTR_STAT_R_TX_OVER_Msk | I2C_INTR_STAT_R_RX_FULL_Msk | I2C_INTR_STAT_R_RX_OVER_Msk | I2C_INTR_STAT_R_RX_UNDER_Msk)
 
/******************  Bit definition for I2C_INTR_MASK register ********************/
#define I2C_INTR_MASK_M_RX_UNDER_Pos                    (0U)
#define I2C_INTR_MASK_M_RX_UNDER_Msk                    (0x01U << I2C_INTR_MASK_M_RX_UNDER_Pos)        /*!<  */
#define I2C_INTR_MASK_M_RX_UNDER                        I2C_INTR_MASK_M_RX_UNDER_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_RX_OVER_Pos                    (1U)
#define I2C_INTR_MASK_M_RX_OVER_Msk                    (0x01U << I2C_INTR_MASK_M_RX_OVER_Pos)        /*!<  */
#define I2C_INTR_MASK_M_RX_OVER                        I2C_INTR_MASK_M_RX_OVER_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_RX_FULL_Pos                    (2U)
#define I2C_INTR_MASK_M_RX_FULL_Msk                    (0x01U << I2C_INTR_MASK_M_RX_FULL_Pos)        /*!<  */
#define I2C_INTR_MASK_M_RX_FULL                        I2C_INTR_MASK_M_RX_FULL_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_TX_OVER_Pos                    (3U)
#define I2C_INTR_MASK_M_TX_OVER_Msk                    (0x01U << I2C_INTR_MASK_M_TX_OVER_Pos)        /*!<  */
#define I2C_INTR_MASK_M_TX_OVER                        I2C_INTR_MASK_M_TX_OVER_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_TX_EMPTY_Pos                    (4U)
#define I2C_INTR_MASK_M_TX_EMPTY_Msk                    (0x01U << I2C_INTR_MASK_M_TX_EMPTY_Pos)        /*!<  */
#define I2C_INTR_MASK_M_TX_EMPTY                        I2C_INTR_MASK_M_TX_EMPTY_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_RD_REQ_Pos                    (5U)
#define I2C_INTR_MASK_M_RD_REQ_Msk                    (0x01U << I2C_INTR_MASK_M_RD_REQ_Pos)        /*!<  */
#define I2C_INTR_MASK_M_RD_REQ                        I2C_INTR_MASK_M_RD_REQ_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_TX_ABRT_Pos                    (6U)
#define I2C_INTR_MASK_M_TX_ABRT_Msk                    (0x01U << I2C_INTR_MASK_M_TX_ABRT_Pos)        /*!<  */
#define I2C_INTR_MASK_M_TX_ABRT                        I2C_INTR_MASK_M_TX_ABRT_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_RX_DONE_Pos                    (7U)
#define I2C_INTR_MASK_M_RX_DONE_Msk                    (0x01U << I2C_INTR_MASK_M_RX_DONE_Pos)        /*!<  */
#define I2C_INTR_MASK_M_RX_DONE                        I2C_INTR_MASK_M_RX_DONE_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_ACTIVITY_Pos                    (8U)
#define I2C_INTR_MASK_M_ACTIVITY_Msk                    (0x01U << I2C_INTR_MASK_M_ACTIVITY_Pos)        /*!<  */
#define I2C_INTR_MASK_M_ACTIVITY                        I2C_INTR_MASK_M_ACTIVITY_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_STOP_DET_Pos                    (9U)
#define I2C_INTR_MASK_M_STOP_DET_Msk                    (0x01U << I2C_INTR_MASK_M_STOP_DET_Pos)        /*!<  */
#define I2C_INTR_MASK_M_STOP_DET                        I2C_INTR_MASK_M_STOP_DET_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_START_DET_Pos                    (10U)
#define I2C_INTR_MASK_M_START_DET_Msk                    (0x01U << I2C_INTR_MASK_M_START_DET_Pos)        /*!<  */
#define I2C_INTR_MASK_M_START_DET                        I2C_INTR_MASK_M_START_DET_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_GEN_CALL_Pos                    (11U)
#define I2C_INTR_MASK_M_GEN_CALL_Msk                    (0x01U << I2C_INTR_MASK_M_GEN_CALL_Pos)        /*!<  */
#define I2C_INTR_MASK_M_GEN_CALL                        I2C_INTR_MASK_M_GEN_CALL_Msk                   /*!<  */
 
#define I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_Pos                    (14U)
#define I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_Msk                    (0x01U << I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_Pos)        /*!<  */
#define I2C_INTR_MASK_M_SCL_STUCK_AT_LOW                        I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_Msk                   /*!<  */
 
#define I2C_INTR_MASK_ALL_Msk                    (I2C_INTR_MASK_M_SCL_STUCK_AT_LOW_Msk | I2C_INTR_MASK_M_GEN_CALL_Msk | I2C_INTR_MASK_M_START_DET_Msk | I2C_INTR_MASK_M_STOP_DET_Msk | I2C_INTR_MASK_M_ACTIVITY_Msk | I2C_INTR_MASK_M_RX_DONE_Msk | I2C_INTR_MASK_M_TX_ABRT_Msk | I2C_INTR_MASK_M_RD_REQ_Msk | I2C_INTR_MASK_M_TX_EMPTY_Msk | I2C_INTR_MASK_M_TX_OVER_Msk | I2C_INTR_MASK_M_RX_FULL_Msk | I2C_INTR_MASK_M_RX_OVER_Msk | I2C_INTR_MASK_M_RX_UNDER_Msk)
 
/******************  Bit definition for I2C_RAW_INTR_STAT register ********************/
#define I2C_RAW_INTR_STAT_RX_UNDER_Pos                    (0U)
#define I2C_RAW_INTR_STAT_RX_UNDER_Msk                    (0x01U << I2C_RAW_INTR_STAT_RX_UNDER_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_RX_UNDER                        I2C_RAW_INTR_STAT_RX_UNDER_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_RX_OVER_Pos                    (1U)
#define I2C_RAW_INTR_STAT_RX_OVER_Msk                    (0x01U << I2C_RAW_INTR_STAT_RX_OVER_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_RX_OVER                        I2C_RAW_INTR_STAT_RX_OVER_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_RX_FULL_Pos                    (2U)
#define I2C_RAW_INTR_STAT_RX_FULL_Msk                    (0x01U << I2C_RAW_INTR_STAT_RX_FULL_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_RX_FULL                        I2C_RAW_INTR_STAT_RX_FULL_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_TX_OVER_Pos                    (3U)
#define I2C_RAW_INTR_STAT_TX_OVER_Msk                    (0x01U << I2C_RAW_INTR_STAT_TX_OVER_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_TX_OVER                        I2C_RAW_INTR_STAT_TX_OVER_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_TX_EMPTY_Pos                    (4U)
#define I2C_RAW_INTR_STAT_TX_EMPTY_Msk                    (0x01U << I2C_RAW_INTR_STAT_TX_EMPTY_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_TX_EMPTY                        I2C_RAW_INTR_STAT_TX_EMPTY_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_RD_REQ_Pos                    (5U)
#define I2C_RAW_INTR_STAT_RD_REQ_Msk                    (0x01U << I2C_RAW_INTR_STAT_RD_REQ_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_RD_REQ                        I2C_RAW_INTR_STAT_RD_REQ_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_TX_ABRT_Pos                    (6U)
#define I2C_RAW_INTR_STAT_TX_ABRT_Msk                    (0x01U << I2C_RAW_INTR_STAT_TX_ABRT_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_TX_ABRT                        I2C_RAW_INTR_STAT_TX_ABRT_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_RX_DONE_Pos                    (7U)
#define I2C_RAW_INTR_STAT_RX_DONE_Msk                    (0x01U << I2C_RAW_INTR_STAT_RX_DONE_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_RX_DONE                        I2C_RAW_INTR_STAT_RX_DONE_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_ACTIVITY_Pos                    (8U)
#define I2C_RAW_INTR_STAT_ACTIVITY_Msk                    (0x01U << I2C_RAW_INTR_STAT_ACTIVITY_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_ACTIVITY                        I2C_RAW_INTR_STAT_ACTIVITY_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_STOP_DET_Pos                    (9U)
#define I2C_RAW_INTR_STAT_STOP_DET_Msk                    (0x01U << I2C_RAW_INTR_STAT_STOP_DET_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_STOP_DET                        I2C_RAW_INTR_STAT_STOP_DET_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_START_DET_Pos                    (10U)
#define I2C_RAW_INTR_STAT_START_DET_Msk                    (0x01U << I2C_RAW_INTR_STAT_START_DET_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_START_DET                        I2C_RAW_INTR_STAT_START_DET_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_GEN_CALL_Pos                    (11U)
#define I2C_RAW_INTR_STAT_GEN_CALL_Msk                    (0x01U << I2C_RAW_INTR_STAT_GEN_CALL_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_GEN_CALL                        I2C_RAW_INTR_STAT_GEN_CALL_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_Pos                    (14U)
#define I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_Msk                    (0x01U << I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_Pos)        /*!<  */
#define I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW                        I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_Msk                   /*!<  */
 
#define I2C_RAW_INTR_STAT_ALL_Msk                    (I2C_RAW_INTR_STAT_SCL_STUCK_AT_LOW_Msk | I2C_RAW_INTR_STAT_GEN_CALL_Msk | I2C_RAW_INTR_STAT_START_DET_Msk | I2C_RAW_INTR_STAT_STOP_DET_Msk | I2C_RAW_INTR_STAT_ACTIVITY_Msk | I2C_RAW_INTR_STAT_RX_DONE_Msk | I2C_RAW_INTR_STAT_TX_ABRT_Msk | I2C_RAW_INTR_STAT_RD_REQ_Msk | I2C_RAW_INTR_STAT_TX_EMPTY_Msk | I2C_RAW_INTR_STAT_TX_OVER_Msk | I2C_RAW_INTR_STAT_RX_FULL_Msk | I2C_RAW_INTR_STAT_RX_OVER_Msk | I2C_RAW_INTR_STAT_RX_UNDER_Msk)
 
/******************  Bit definition for I2C_RX_TL register ********************/
#define I2C_RX_TL_RX_TL_Pos                    (0U)
#define I2C_RX_TL_RX_TL_Msk                    (0xFFU << I2C_RX_TL_RX_TL_Pos)        /*!<  */
#define I2C_RX_TL_RX_TL                        I2C_RX_TL_RX_TL_Msk                   /*!<  */
 
#define I2C_RX_TL_ALL_Msk                    (I2C_RX_TL_RX_TL_Msk)
 
/******************  Bit definition for I2C_TX_TL register ********************/
#define I2C_TX_TL_TX_TL_Pos                    (0U)
#define I2C_TX_TL_TX_TL_Msk                    (0xFFU << I2C_TX_TL_TX_TL_Pos)        /*!<  */
#define I2C_TX_TL_TX_TL                        I2C_TX_TL_TX_TL_Msk                   /*!<  */
 
#define I2C_TX_TL_ALL_Msk                    (I2C_TX_TL_TX_TL_Msk)
 
/******************  Bit definition for I2C_CLR_INTR register ********************/
#define I2C_CLR_INTR_CLR_INTR_Pos                    (0U)
#define I2C_CLR_INTR_CLR_INTR_Msk                    (0x01U << I2C_CLR_INTR_CLR_INTR_Pos)        /*!<  */
#define I2C_CLR_INTR_CLR_INTR                        I2C_CLR_INTR_CLR_INTR_Msk                   /*!<  */
 
#define I2C_CLR_INTR_ALL_Msk                    (I2C_CLR_INTR_CLR_INTR_Msk)
 
/******************  Bit definition for I2C_CLR_RX_UNDER register ********************/
#define I2C_CLR_RX_UNDER_CLR_RX_UNDER_Pos                    (0U)
#define I2C_CLR_RX_UNDER_CLR_RX_UNDER_Msk                    (0x01U << I2C_CLR_RX_UNDER_CLR_RX_UNDER_Pos)        /*!<  */
#define I2C_CLR_RX_UNDER_CLR_RX_UNDER                        I2C_CLR_RX_UNDER_CLR_RX_UNDER_Msk                   /*!<  */
 
#define I2C_CLR_RX_UNDER_ALL_Msk                    (I2C_CLR_RX_UNDER_CLR_RX_UNDER_Msk)
 
/******************  Bit definition for I2C_CLR_RX_OVER register ********************/
#define I2C_CLR_RX_OVER_CLR_RX_OVER_Pos                    (0U)
#define I2C_CLR_RX_OVER_CLR_RX_OVER_Msk                    (0x01U << I2C_CLR_RX_OVER_CLR_RX_OVER_Pos)        /*!<  */
#define I2C_CLR_RX_OVER_CLR_RX_OVER                        I2C_CLR_RX_OVER_CLR_RX_OVER_Msk                   /*!<  */
 
#define I2C_CLR_RX_OVER_ALL_Msk                    (I2C_CLR_RX_OVER_CLR_RX_OVER_Msk)
 
/******************  Bit definition for I2C_CLR_TX_OVER register ********************/
#define I2C_CLR_TX_OVER_CLR_TX_OVER_Pos                    (0U)
#define I2C_CLR_TX_OVER_CLR_TX_OVER_Msk                    (0x01U << I2C_CLR_TX_OVER_CLR_TX_OVER_Pos)        /*!<  */
#define I2C_CLR_TX_OVER_CLR_TX_OVER                        I2C_CLR_TX_OVER_CLR_TX_OVER_Msk                   /*!<  */
 
#define I2C_CLR_TX_OVER_ALL_Msk                    (I2C_CLR_TX_OVER_CLR_TX_OVER_Msk)
 
/******************  Bit definition for I2C_CLR_RD_REQ register ********************/
#define I2C_CLR_RD_REQ_CLR_RD_REQ_Pos                    (0U)
#define I2C_CLR_RD_REQ_CLR_RD_REQ_Msk                    (0x01U << I2C_CLR_RD_REQ_CLR_RD_REQ_Pos)        /*!<  */
#define I2C_CLR_RD_REQ_CLR_RD_REQ                        I2C_CLR_RD_REQ_CLR_RD_REQ_Msk                   /*!<  */
 
#define I2C_CLR_RD_REQ_ALL_Msk                    (I2C_CLR_RD_REQ_CLR_RD_REQ_Msk)
 
/******************  Bit definition for I2C_CLR_TX_ABRT register ********************/
#define I2C_CLR_TX_ABRT_CLR_TX_ABRT_Pos                    (0U)
#define I2C_CLR_TX_ABRT_CLR_TX_ABRT_Msk                    (0x01U << I2C_CLR_TX_ABRT_CLR_TX_ABRT_Pos)        /*!<  */
#define I2C_CLR_TX_ABRT_CLR_TX_ABRT                        I2C_CLR_TX_ABRT_CLR_TX_ABRT_Msk                   /*!<  */
 
#define I2C_CLR_TX_ABRT_ALL_Msk                    (I2C_CLR_TX_ABRT_CLR_TX_ABRT_Msk)
 
/******************  Bit definition for I2C_CLR_RX_DONE register ********************/
#define I2C_CLR_RX_DONE_CLR_RX_DONE_Pos                    (0U)
#define I2C_CLR_RX_DONE_CLR_RX_DONE_Msk                    (0x01U << I2C_CLR_RX_DONE_CLR_RX_DONE_Pos)        /*!<  */
#define I2C_CLR_RX_DONE_CLR_RX_DONE                        I2C_CLR_RX_DONE_CLR_RX_DONE_Msk                   /*!<  */
 
#define I2C_CLR_RX_DONE_ALL_Msk                    (I2C_CLR_RX_DONE_CLR_RX_DONE_Msk)
 
/******************  Bit definition for I2C_CLR_ACTIVITY register ********************/
#define I2C_CLR_ACTIVITY_CLR_ACTIVITY_Pos                    (0U)
#define I2C_CLR_ACTIVITY_CLR_ACTIVITY_Msk                    (0x01U << I2C_CLR_ACTIVITY_CLR_ACTIVITY_Pos)        /*!<  */
#define I2C_CLR_ACTIVITY_CLR_ACTIVITY                        I2C_CLR_ACTIVITY_CLR_ACTIVITY_Msk                   /*!<  */
 
#define I2C_CLR_ACTIVITY_ALL_Msk                    (I2C_CLR_ACTIVITY_CLR_ACTIVITY_Msk)
 
/******************  Bit definition for I2C_CLR_STOP_DET register ********************/
#define I2C_CLR_STOP_DET_CLR_STOP_DET_Pos                    (0U)
#define I2C_CLR_STOP_DET_CLR_STOP_DET_Msk                    (0x01U << I2C_CLR_STOP_DET_CLR_STOP_DET_Pos)        /*!<  */
#define I2C_CLR_STOP_DET_CLR_STOP_DET                        I2C_CLR_STOP_DET_CLR_STOP_DET_Msk                   /*!<  */
 
#define I2C_CLR_STOP_DET_ALL_Msk                    (I2C_CLR_STOP_DET_CLR_STOP_DET_Msk)
 
/******************  Bit definition for I2C_CLR_START_DET register ********************/
#define I2C_CLR_START_DET_CLR_START_DET_Pos                    (0U)
#define I2C_CLR_START_DET_CLR_START_DET_Msk                    (0x01U << I2C_CLR_START_DET_CLR_START_DET_Pos)        /*!<  */
#define I2C_CLR_START_DET_CLR_START_DET                        I2C_CLR_START_DET_CLR_START_DET_Msk                   /*!<  */
 
#define I2C_CLR_START_DET_ALL_Msk                    (I2C_CLR_START_DET_CLR_START_DET_Msk)
 
/******************  Bit definition for I2C_CLR_GEN_CALL register ********************/
#define I2C_CLR_GEN_CALL_CLR_START_DET_Pos                    (0U)
#define I2C_CLR_GEN_CALL_CLR_START_DET_Msk                    (0x01U << I2C_CLR_GEN_CALL_CLR_START_DET_Pos)        /*!<  */
#define I2C_CLR_GEN_CALL_CLR_START_DET                        I2C_CLR_GEN_CALL_CLR_START_DET_Msk                   /*!<  */
 
#define I2C_CLR_GEN_CALL_ALL_Msk                    (I2C_CLR_GEN_CALL_CLR_START_DET_Msk)
 
/******************  Bit definition for I2C_ENABLE register ********************/
#define I2C_ENABLE_ENABLE_Pos                    (0U)
#define I2C_ENABLE_ENABLE_Msk                    (0x01U << I2C_ENABLE_ENABLE_Pos)        /*!<  */
#define I2C_ENABLE_ENABLE                        I2C_ENABLE_ENABLE_Msk                   /*!<  */
 
#define I2C_ENABLE_ABORT_Pos                    (1U)
#define I2C_ENABLE_ABORT_Msk                    (0x01U << I2C_ENABLE_ABORT_Pos)        /*!<  */
#define I2C_ENABLE_ABORT                        I2C_ENABLE_ABORT_Msk                   /*!<  */
 
#define I2C_ENABLE_TX_CMD_BLOCK_Pos                    (2U)
#define I2C_ENABLE_TX_CMD_BLOCK_Msk                    (0x01U << I2C_ENABLE_TX_CMD_BLOCK_Pos)        /*!<  */
#define I2C_ENABLE_TX_CMD_BLOCK                        I2C_ENABLE_TX_CMD_BLOCK_Msk                   /*!<  */
 
#define I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE_Pos                    (3U)
#define I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE_Msk                    (0x01U << I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE_Pos)        /*!<  */
#define I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE                        I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE_Msk                   /*!<  */
 
#define I2C_ENABLE_SMBUS_CLK_RESET_Pos                    (16U)
#define I2C_ENABLE_SMBUS_CLK_RESET_Msk                    (0x01U << I2C_ENABLE_SMBUS_CLK_RESET_Pos)        /*!<  */
#define I2C_ENABLE_SMBUS_CLK_RESET                        I2C_ENABLE_SMBUS_CLK_RESET_Msk                   /*!<  */
 
#define I2C_ENABLE_SMBUS_SUSPEND_EN_Pos                    (17U)
#define I2C_ENABLE_SMBUS_SUSPEND_EN_Msk                    (0x01U << I2C_ENABLE_SMBUS_SUSPEND_EN_Pos)        /*!<  */
#define I2C_ENABLE_SMBUS_SUSPEND_EN                        I2C_ENABLE_SMBUS_SUSPEND_EN_Msk                   /*!<  */
 
#define I2C_ENABLE_SMBUS_ALERT_EN_Pos                    (18U)
#define I2C_ENABLE_SMBUS_ALERT_EN_Msk                    (0x01U << I2C_ENABLE_SMBUS_ALERT_EN_Pos)        /*!<  */
#define I2C_ENABLE_SMBUS_ALERT_EN                        I2C_ENABLE_SMBUS_ALERT_EN_Msk                   /*!<  */
 
#define I2C_ENABLE_ALL_Msk                    (I2C_ENABLE_SMBUS_ALERT_EN_Msk | I2C_ENABLE_SMBUS_SUSPEND_EN_Msk | I2C_ENABLE_SMBUS_CLK_RESET_Msk | I2C_ENABLE_SDA_STUCK_RECOVERY_ENABLE_Msk | I2C_ENABLE_TX_CMD_BLOCK_Msk | I2C_ENABLE_ABORT_Msk | I2C_ENABLE_ENABLE_Msk)
 
/******************  Bit definition for I2C_STATUS register ********************/
#define I2C_STATUS_ACTIVITY_Pos                    (0U)
#define I2C_STATUS_ACTIVITY_Msk                    (0x01U << I2C_STATUS_ACTIVITY_Pos)        /*!<  */
#define I2C_STATUS_ACTIVITY                        I2C_STATUS_ACTIVITY_Msk                   /*!<  */
 
#define I2C_STATUS_TFNF_Pos                    (1U)
#define I2C_STATUS_TFNF_Msk                    (0x01U << I2C_STATUS_TFNF_Pos)        /*!<  */
#define I2C_STATUS_TFNF                        I2C_STATUS_TFNF_Msk                   /*!<  */
 
#define I2C_STATUS_TFE_Pos                    (2U)
#define I2C_STATUS_TFE_Msk                    (0x01U << I2C_STATUS_TFE_Pos)        /*!<  */
#define I2C_STATUS_TFE                        I2C_STATUS_TFE_Msk                   /*!<  */
 
#define I2C_STATUS_RFNE_Pos                    (3U)
#define I2C_STATUS_RFNE_Msk                    (0x01U << I2C_STATUS_RFNE_Pos)        /*!<  */
#define I2C_STATUS_RFNE                        I2C_STATUS_RFNE_Msk                   /*!<  */
 
#define I2C_STATUS_RFF_Pos                    (4U)
#define I2C_STATUS_RFF_Msk                    (0x01U << I2C_STATUS_RFF_Pos)        /*!<  */
#define I2C_STATUS_RFF                        I2C_STATUS_RFF_Msk                   /*!<  */
 
#define I2C_STATUS_MST_ACTIVITY_Pos                    (5U)
#define I2C_STATUS_MST_ACTIVITY_Msk                    (0x01U << I2C_STATUS_MST_ACTIVITY_Pos)        /*!<  */
#define I2C_STATUS_MST_ACTIVITY                        I2C_STATUS_MST_ACTIVITY_Msk                   /*!<  */
 
#define I2C_STATUS_SLV_ACTIVITY_Pos                    (6U)
#define I2C_STATUS_SLV_ACTIVITY_Msk                    (0x01U << I2C_STATUS_SLV_ACTIVITY_Pos)        /*!<  */
#define I2C_STATUS_SLV_ACTIVITY                        I2C_STATUS_SLV_ACTIVITY_Msk                   /*!<  */
 
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_Pos                    (7U)
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_Msk                    (0x01U << I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_Pos)        /*!<  */
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY                        I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_Msk                   /*!<  */
 
#define I2C_STATUS_MST_HODE_RX_FIFO_FULL_Pos                    (8U)
#define I2C_STATUS_MST_HODE_RX_FIFO_FULL_Msk                    (0x01U << I2C_STATUS_MST_HODE_RX_FIFO_FULL_Pos)        /*!<  */
#define I2C_STATUS_MST_HODE_RX_FIFO_FULL                        I2C_STATUS_MST_HODE_RX_FIFO_FULL_Msk                   /*!<  */
 
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_Pos                    (9U)
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_Msk                    (0x01U << I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_Pos)        /*!<  */
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY                        I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_Msk                   /*!<  */
 
#define I2C_STATUS_SLV_HODE_RX_FIFO_FULL_Pos                    (10U)
#define I2C_STATUS_SLV_HODE_RX_FIFO_FULL_Msk                    (0x01U << I2C_STATUS_SLV_HODE_RX_FIFO_FULL_Pos)        /*!<  */
#define I2C_STATUS_SLV_HODE_RX_FIFO_FULL                        I2C_STATUS_SLV_HODE_RX_FIFO_FULL_Msk                   /*!<  */
 
#define I2C_STATUS_SDA_STUCK_NOT_RECOVERED_Pos                    (11U)
#define I2C_STATUS_SDA_STUCK_NOT_RECOVERED_Msk                    (0x01U << I2C_STATUS_SDA_STUCK_NOT_RECOVERED_Pos)        /*!<  */
#define I2C_STATUS_SDA_STUCK_NOT_RECOVERED                        I2C_STATUS_SDA_STUCK_NOT_RECOVERED_Msk                   /*!<  */
 
#define I2C_STATUS_SAR_DUAL_FLAG_Pos                    (12U)
#define I2C_STATUS_SAR_DUAL_FLAG_Msk                    (0x01U << I2C_STATUS_SAR_DUAL_FLAG_Pos)        /*!<  */
#define I2C_STATUS_SAR_DUAL_FLAG                        I2C_STATUS_SAR_DUAL_FLAG_Msk                   /*!<  */
 
#define I2C_STATUS_SMBUS_QUICK_CMD_BIT_Pos                    (16U)
#define I2C_STATUS_SMBUS_QUICK_CMD_BIT_Msk                    (0x01U << I2C_STATUS_SMBUS_QUICK_CMD_BIT_Pos)        /*!<  */
#define I2C_STATUS_SMBUS_QUICK_CMD_BIT                        I2C_STATUS_SMBUS_QUICK_CMD_BIT_Msk                   /*!<  */
 
#define I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_Pos                    (17U)
#define I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_Msk                    (0x01U << I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_Pos)        /*!<  */
#define I2C_STATUS_SMBUS_SLAVE_ADDR_VALID                        I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_Msk                   /*!<  */
 
#define I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_Pos                    (18U)
#define I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_Msk                    (0x01U << I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_Pos)        /*!<  */
#define I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED                        I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_Msk                   /*!<  */
 
#define I2C_STATUS_SMBUS_SUSPEND_STATUS_Pos                    (19U)
#define I2C_STATUS_SMBUS_SUSPEND_STATUS_Msk                    (0x01U << I2C_STATUS_SMBUS_SUSPEND_STATUS_Pos)        /*!<  */
#define I2C_STATUS_SMBUS_SUSPEND_STATUS                        I2C_STATUS_SMBUS_SUSPEND_STATUS_Msk                   /*!<  */
 
#define I2C_STATUS_SMBUS_ALERT_STATUS_Pos                    (20U)
#define I2C_STATUS_SMBUS_ALERT_STATUS_Msk                    (0x01U << I2C_STATUS_SMBUS_ALERT_STATUS_Pos)        /*!<  */
#define I2C_STATUS_SMBUS_ALERT_STATUS                        I2C_STATUS_SMBUS_ALERT_STATUS_Msk                   /*!<  */
 
#define I2C_STATUS_ALL_Msk                    (I2C_STATUS_SMBUS_ALERT_STATUS_Msk | I2C_STATUS_SMBUS_SUSPEND_STATUS_Msk | I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED_Msk | I2C_STATUS_SMBUS_SLAVE_ADDR_VALID_Msk | I2C_STATUS_SMBUS_QUICK_CMD_BIT_Msk | I2C_STATUS_SAR_DUAL_FLAG_Msk | I2C_STATUS_SDA_STUCK_NOT_RECOVERED_Msk | I2C_STATUS_SLV_HODE_RX_FIFO_FULL_Msk | I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY_Msk | I2C_STATUS_MST_HODE_RX_FIFO_FULL_Msk | I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY_Msk | I2C_STATUS_SLV_ACTIVITY_Msk | I2C_STATUS_MST_ACTIVITY_Msk | I2C_STATUS_RFF_Msk | I2C_STATUS_RFNE_Msk | I2C_STATUS_TFE_Msk | I2C_STATUS_TFNF_Msk | I2C_STATUS_ACTIVITY_Msk)
 
/******************  Bit definition for I2C_TXFLR register ********************/
#define I2C_TXFLR_TXFLR_Pos                    (0U)
#define I2C_TXFLR_TXFLR_Msk                    (0x7U << I2C_TXFLR_TXFLR_Pos)        /*!<  */
#define I2C_TXFLR_TXFLR                        I2C_TXFLR_TXFLR_Msk                   /*!<  */
 
#define I2C_TXFLR_ALL_Msk                    (I2C_TXFLR_TXFLR_Msk)
 
/******************  Bit definition for I2C_RXFLR register ********************/
#define I2C_RXFLR_RXFLR_Pos                    (0U)
#define I2C_RXFLR_RXFLR_Msk                    (0x7U << I2C_RXFLR_RXFLR_Pos)        /*!<  */
#define I2C_RXFLR_RXFLR                        I2C_RXFLR_RXFLR_Msk                   /*!<  */
 
#define I2C_RXFLR_ALL_Msk                    (I2C_RXFLR_RXFLR_Msk)
 
/******************  Bit definition for I2C_SDA_HOLD register ********************/
#define I2C_SDA_HOLD_IC_SDA_TX_HOLD_Pos                    (0U)
#define I2C_SDA_HOLD_IC_SDA_TX_HOLD_Msk                    (0xFFFFU << I2C_SDA_HOLD_IC_SDA_TX_HOLD_Pos)        /*!<  */
#define I2C_SDA_HOLD_IC_SDA_TX_HOLD                        I2C_SDA_HOLD_IC_SDA_TX_HOLD_Msk                   /*!<  */
 
#define I2C_SDA_HOLD_IC_SDA_RX_HOLD_Pos                    (16U)
#define I2C_SDA_HOLD_IC_SDA_RX_HOLD_Msk                    (0xFFU << I2C_SDA_HOLD_IC_SDA_RX_HOLD_Pos)        /*!<  */
#define I2C_SDA_HOLD_IC_SDA_RX_HOLD                        I2C_SDA_HOLD_IC_SDA_RX_HOLD_Msk                   /*!<  */
 
#define I2C_SDA_HOLD_ALL_Msk                    (I2C_SDA_HOLD_IC_SDA_RX_HOLD_Msk | I2C_SDA_HOLD_IC_SDA_TX_HOLD_Msk)
 
/******************  Bit definition for I2C_TX_ABRT_SOURCE register ********************/
#define I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_Pos                    (0U)
#define I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK                        I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_Pos                    (1U)
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK                        I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_Pos                    (2U)
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK                        I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_Pos                    (3U)
#define I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK                        I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_Pos                    (4U)
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK                        I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ_Pos                    (5U)
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ                        I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_Pos                    (7U)
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET                        I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_Pos                    (9U)
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT                        I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_Pos                    (10U)
#define I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT                        I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS_Pos                    (11U)
#define I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS                        I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ARB_LOST_Pos                    (12U)
#define I2C_TX_ABRT_SOURCE_ARB_LOST_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ARB_LOST_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ARB_LOST                        I2C_TX_ABRT_SOURCE_ARB_LOST_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_Pos                    (13U)
#define I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO                        I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_Pos                    (14U)
#define I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST                        I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_Pos                    (15U)
#define I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX                        I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT_Pos                    (16U)
#define I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT                        I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW_Pos                    (17U)
#define I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW_Msk                    (0x01U << I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW                        I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Pos                    (23U)
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Msk                    (0x7U << I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Pos)        /*!<  */
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT                        I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Msk                   /*!<  */
 
#define I2C_TX_ABRT_SOURCE_ALL_Msk                    (I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Msk | I2C_TX_ABRT_SOURCE_ABRT_SDA_STUCK_AT_LOW_Msk | I2C_TX_ABRT_SOURCE_ABRT_USER_ABRT_Msk | I2C_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_Msk | I2C_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_Msk | I2C_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_Msk | I2C_TX_ABRT_SOURCE_ARB_LOST_Msk | I2C_TX_ABRT_SOURCE_ABRT_MASTER_DIS_Msk | I2C_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_Msk | I2C_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_Msk | I2C_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_Msk | I2C_TX_ABRT_SOURCE_ABRT_GCALL_READ_Msk | I2C_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_Msk | I2C_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_Msk | I2C_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_Msk | I2C_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_Msk | I2C_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_Msk)
 
/******************  Bit definition for I2C_DMA_CR register ********************/
#define I2C_DMA_CR_RDMAE_Pos                    (0U)
#define I2C_DMA_CR_RDMAE_Msk                    (0x01U << I2C_DMA_CR_RDMAE_Pos)        /*!<  */
#define I2C_DMA_CR_RDMAE                        I2C_DMA_CR_RDMAE_Msk                   /*!<  */
 
#define I2C_DMA_CR_TDMAE_Pos                    (1U)
#define I2C_DMA_CR_TDMAE_Msk                    (0x01U << I2C_DMA_CR_TDMAE_Pos)        /*!<  */
#define I2C_DMA_CR_TDMAE                        I2C_DMA_CR_TDMAE_Msk                   /*!<  */
 
#define I2C_DMA_CR_ALL_Msk                    (I2C_DMA_CR_TDMAE_Msk | I2C_DMA_CR_RDMAE_Msk)
 
/******************  Bit definition for I2C_DMA_TDLR register ********************/
#define I2C_DMA_TDLR_DMATDL_Pos                    (0U)
#define I2C_DMA_TDLR_DMATDL_Msk                    (0x7U << I2C_DMA_TDLR_DMATDL_Pos)        /*!<  */
#define I2C_DMA_TDLR_DMATDL                        I2C_DMA_TDLR_DMATDL_Msk                   /*!<  */
 
#define I2C_DMA_TDLR_ALL_Msk                    (I2C_DMA_TDLR_DMATDL_Msk)
 
/******************  Bit definition for I2C_DMA_RDLR register ********************/
#define I2C_DMA_RDLR_DMARDL_Pos                    (0U)
#define I2C_DMA_RDLR_DMARDL_Msk                    (0x7U << I2C_DMA_RDLR_DMARDL_Pos)        /*!<  */
#define I2C_DMA_RDLR_DMARDL                        I2C_DMA_RDLR_DMARDL_Msk                   /*!<  */
 
#define I2C_DMA_RDLR_ALL_Msk                    (I2C_DMA_RDLR_DMARDL_Msk)
 
/******************  Bit definition for I2C_SDA_SETUP register ********************/
#define I2C_SDA_SETUP_SDA_SETUP_Pos                    (0U)
#define I2C_SDA_SETUP_SDA_SETUP_Msk                    (0xFFU << I2C_SDA_SETUP_SDA_SETUP_Pos)        /*!<  */
#define I2C_SDA_SETUP_SDA_SETUP                        I2C_SDA_SETUP_SDA_SETUP_Msk                   /*!<  */
 
#define I2C_SDA_SETUP_ALL_Msk                    (I2C_SDA_SETUP_SDA_SETUP_Msk)
 
/******************  Bit definition for I2C_ACK_GENERAL_CALL register ********************/
#define I2C_ACK_GENERAL_CALL_ACK_GEN_CALL_Pos                    (0U)
#define I2C_ACK_GENERAL_CALL_ACK_GEN_CALL_Msk                    (0x01U << I2C_ACK_GENERAL_CALL_ACK_GEN_CALL_Pos)        /*!<  */
#define I2C_ACK_GENERAL_CALL_ACK_GEN_CALL                        I2C_ACK_GENERAL_CALL_ACK_GEN_CALL_Msk                   /*!<  */
 
#define I2C_ACK_GENERAL_CALL_ALL_Msk                    (I2C_ACK_GENERAL_CALL_ACK_GEN_CALL_Msk)
 
/******************  Bit definition for I2C_ENABLE_STATUS register ********************/
#define I2C_ENABLE_STATUS_I2C_EN_Pos                    (0U)
#define I2C_ENABLE_STATUS_I2C_EN_Msk                    (0x01U << I2C_ENABLE_STATUS_I2C_EN_Pos)        /*!<  */
#define I2C_ENABLE_STATUS_I2C_EN                        I2C_ENABLE_STATUS_I2C_EN_Msk                   /*!<  */
 
#define I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos                    (1U)
#define I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk                    (0x01U << I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos)        /*!<  */
#define I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY                        I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk                   /*!<  */
 
#define I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos                    (2U)
#define I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk                    (0x01U << I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos)        /*!<  */
#define I2C_ENABLE_STATUS_SLV_RX_DATA_LOST                        I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk                   /*!<  */
 
#define I2C_ENABLE_STATUS_ALL_Msk                    (I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk | I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk | I2C_ENABLE_STATUS_I2C_EN_Msk)
 
/******************  Bit definition for I2C_FS_SPKLEN register ********************/
#define I2C_FS_SPKLEN_I2C_FS_SPKLEN_Pos                    (0U)
#define I2C_FS_SPKLEN_I2C_FS_SPKLEN_Msk                    (0xFFU << I2C_FS_SPKLEN_I2C_FS_SPKLEN_Pos)        /*!<  */
#define I2C_FS_SPKLEN_I2C_FS_SPKLEN                        I2C_FS_SPKLEN_I2C_FS_SPKLEN_Msk                   /*!<  */
 
#define I2C_FS_SPKLEN_ALL_Msk                    (I2C_FS_SPKLEN_I2C_FS_SPKLEN_Msk)
 
/******************  Bit definition for I2C_SCL_STUCK_AT_LOW_TIMEOUT register ********************/
#define I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT_Pos                    (0U)
#define I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT_Msk                    (0xFFFFFFFFU << I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT_Pos)        /*!<  */
#define I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT                        I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SCL_STUCK_AT_LOW_TIMEOUT_ALL_Msk                    (I2C_SCL_STUCK_AT_LOW_TIMEOUT_I2C_SCL_STUCK_AT_LOW_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SDA_STUCK_AT_LOW_TIMEOUT register ********************/
#define I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT_Pos                    (0U)
#define I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT_Msk                    (0xFFFFFFFFU << I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT_Pos)        /*!<  */
#define I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT                        I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SDA_STUCK_AT_LOW_TIMEOUT_ALL_Msk                    (I2C_SDA_STUCK_AT_LOW_TIMEOUT_I2C_SDA_STUCK_AT_LOW_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_CLR_SCL_STUCK_DET register ********************/
#define I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK_Pos                    (0U)
#define I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK_Msk                    (0x01U << I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK_Pos)        /*!<  */
#define I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK                        I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK_Msk                   /*!<  */
 
#define I2C_CLR_SCL_STUCK_DET_ALL_Msk                    (I2C_CLR_SCL_STUCK_DET_CLR_SCL_STUCK_Msk)
 
/******************  Bit definition for I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT register ********************/
#define I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT_Pos                    (0U)
#define I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT_Msk                    (0xFFFFFFFFU << I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT                        I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_ALL_Msk                    (I2C_SMBUS_CLK_LOW_SEXT_TIMEOUT_SMBUS_CLK_LOW_SEXT_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT register ********************/
#define I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT_Pos                    (0U)
#define I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT_Msk                    (0xFFFFFFFFU << I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT                        I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_ALL_Msk                    (I2C_SMBUS_CLK_LOW_MEXT_TIMEOUT_SMBUS_CLK_LOW_MEXT_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SMBUS_THIGH_MAX_IDLE_COUNT register ********************/
#define I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT_Pos                    (0U)
#define I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT_Msk                    (0xFFFFU << I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT_Pos)        /*!<  */
#define I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT                        I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT_Msk                   /*!<  */
 
#define I2C_SMBUS_THIGH_MAX_IDLE_COUNT_ALL_Msk                    (I2C_SMBUS_THIGH_MAX_IDLE_COUNT_SMBUS_THIGH_MAX_IDLE_COUNT_Msk)
 
/******************  Bit definition for I2C_SMBUS_INTR_STAT register ********************/
#define I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT_Pos                    (0U)
#define I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT_Pos                    (1U)
#define I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET_Pos                    (2U)
#define I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET                        I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET_Pos                    (3U)
#define I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET                        I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET_Pos                    (4U)
#define I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET                        I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET_Pos                    (5U)
#define I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET                        I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET_Pos                    (6U)
#define I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET                        I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET_Pos                    (7U)
#define I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET                        I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK_Pos                    (8U)
#define I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK                        I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET_Pos                    (9U)
#define I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET                        I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET_Pos                    (10U)
#define I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET_Msk                    (0x01U << I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET                        I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_STAT_ALL_Msk                    (I2C_SMBUS_INTR_STAT_R_SMBUS_ALERT_DET_Msk | I2C_SMBUS_INTR_STAT_R_SMBUS_SUSPEND_DET_Msk | I2C_SMBUS_INTR_STAT_R_SLV_RX_PEC_NACK_Msk | I2C_SMBUS_INTR_STAT_R_ARP_ASSGN_ADDR_CMD_DET_Msk | I2C_SMBUS_INTR_STAT_R_ARP_GET_UDID_CMD_DET_Msk | I2C_SMBUS_INTR_STAT_R_ARP_RST_CMD_DET_Msk | I2C_SMBUS_INTR_STAT_R_ARP_PREPARE_CMD_DET_Msk | I2C_SMBUS_INTR_STAT_R_HOST_NOTIFY_MST_DET_Msk | I2C_SMBUS_INTR_STAT_R_QUICK_CMD_DET_Msk | I2C_SMBUS_INTR_STAT_R_MST_CLOCK_EXTND_TIMEOUT_Msk | I2C_SMBUS_INTR_STAT_R_SLV_CLOCK_EXTND_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SMBUS_INTR_MASK register ********************/
#define I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT_Pos                    (0U)
#define I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT_Pos                    (1U)
#define I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET_Pos                    (2U)
#define I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET                        I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET_Pos                    (3U)
#define I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET                        I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET_Pos                    (4U)
#define I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET                        I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET_Pos                    (5U)
#define I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET                        I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET_Pos                    (6U)
#define I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET                        I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET_Pos                    (7U)
#define I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET                        I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK_Pos                    (8U)
#define I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK                        I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET_Pos                    (9U)
#define I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET                        I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET_Pos                    (10U)
#define I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET_Msk                    (0x01U << I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET                        I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_MASK_ALL_Msk                    (I2C_SMBUS_INTR_MASK_M_SMBUS_ALERT_DET_Msk | I2C_SMBUS_INTR_MASK_M_SMBUS_SUSPEND_DET_Msk | I2C_SMBUS_INTR_MASK_M_SLV_RX_PEC_NACK_Msk | I2C_SMBUS_INTR_MASK_M_ARP_ASSGN_ADDR_CMD_DET_Msk | I2C_SMBUS_INTR_MASK_M_ARP_GET_UDID_CMD_DET_Msk | I2C_SMBUS_INTR_MASK_M_ARP_RST_CMD_DET_Msk | I2C_SMBUS_INTR_MASK_M_ARP_PREPARE_CMD_DET_Msk | I2C_SMBUS_INTR_MASK_M_HOST_NOTIFY_MST_DET_Msk | I2C_SMBUS_INTR_MASK_M_QUICK_CMD_DET_Msk | I2C_SMBUS_INTR_MASK_M_MST_CLOCK_EXTND_TIMEOUT_Msk | I2C_SMBUS_INTR_MASK_M_SLV_CLOCK_EXTND_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SMBUS_INTR_RAW_STATUS register ********************/
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT_Pos                    (0U)
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT_Pos                    (1U)
#define I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT                        I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET_Pos                    (2U)
#define I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET                        I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET_Pos                    (3U)
#define I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET                        I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET_Pos                    (4U)
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET                        I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET_Pos                    (5U)
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET                        I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET_Pos                    (6U)
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET                        I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET_Pos                    (7U)
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET                        I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK_Pos                    (8U)
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK                        I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET_Pos                    (9U)
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET                        I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET_Pos                    (10U)
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET_Msk                    (0x01U << I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET_Pos)        /*!<  */
#define I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET                        I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET_Msk                   /*!<  */
 
#define I2C_SMBUS_INTR_RAW_STATUS_ALL_Msk                    (I2C_SMBUS_INTR_RAW_STATUS_SMBUS_ALERT_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_SMBUS_SUSPEND_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_SLV_RX_PEC_NACK_Msk | I2C_SMBUS_INTR_RAW_STATUS_ARP_ASSGN_ADDR_CMD_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_ARP_GET_UDID_CMD_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_ARP_RST_CMD_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_ARP_PREPARE_CMD_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_HOST_NOTIFY_MST_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_QUICK_CMD_DET_Msk | I2C_SMBUS_INTR_RAW_STATUS_MST_CLOCK_EXTND_TIMEOUT_Msk | I2C_SMBUS_INTR_RAW_STATUS_SLV_CLOCK_EXTND_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_CLR_SMBUS_INTR register ********************/
#define I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT_Pos                    (0U)
#define I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT                        I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT_Pos                    (1U)
#define I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT                        I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET_Pos                    (2U)
#define I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET                        I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET_Pos                    (3U)
#define I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET                        I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET_Pos                    (4U)
#define I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET                        I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET_Pos                    (5U)
#define I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET                        I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET_Pos                    (6U)
#define I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET                        I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET_Pos                    (7U)
#define I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET                        I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK_Pos                    (8U)
#define I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK                        I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET_Pos                    (9U)
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET                        I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET_Pos                    (10U)
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET_Msk                    (0x01U << I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET_Pos)        /*!<  */
#define I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET                        I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET_Msk                   /*!<  */
 
#define I2C_CLR_SMBUS_INTR_ALL_Msk                    (I2C_CLR_SMBUS_INTR_CLR_SMBUS_ALERT_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_SMBUS_SUSPEND_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_SLV_RX_PEC_NACK_Msk | I2C_CLR_SMBUS_INTR_CLR_ARP_ASSGN_ADDR_CMD_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_ARP_GET_UDID_CMD_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_ARP_RST_CMD_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_ARP_PREPARE_CMD_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_HOST_NOTIFY_MST_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_QUICK_CMD_DET_Msk | I2C_CLR_SMBUS_INTR_CLR_MST_CLOCK_EXTND_TIMEOUT_Msk | I2C_CLR_SMBUS_INTR_CLR_SLV_CLOCK_EXTND_TIMEOUT_Msk)
 
/******************  Bit definition for I2C_SAR_OPT register ********************/
#define I2C_SAR_OPT_SAR_OPT_Pos                    (0U)
#define I2C_SAR_OPT_SAR_OPT_Msk                    (0xFFU << I2C_SAR_OPT_SAR_OPT_Pos)        /*!<  */
#define I2C_SAR_OPT_SAR_OPT                        I2C_SAR_OPT_SAR_OPT_Msk                   /*!<  */
 
#define I2C_SAR_OPT_ALL_Msk                    (I2C_SAR_OPT_SAR_OPT_Msk)
 
/******************  Bit definition for SMBUS_UDID register ********************/
#define SMBUS_UDID_SMBUS_UDID_Pos                    (0U)
#define SMBUS_UDID_SMBUS_UDID_Msk                    (0xFFFFFFFFU << SMBUS_UDID_SMBUS_UDID_Pos)        /*!<  */
#define SMBUS_UDID_SMBUS_UDID                        SMBUS_UDID_SMBUS_UDID_Msk                   /*!<  */
 
#define SMBUS_UDID_ALL_Msk                    (SMBUS_UDID_SMBUS_UDID_Msk)
 
/******************  Bit definition for SPI_CR1 register ********************/
#define SPI_CR1_CPHA_Pos                    (0U)
#define SPI_CR1_CPHA_Msk                    (0x01U << SPI_CR1_CPHA_Pos)        /*!<  */
#define SPI_CR1_CPHA                        SPI_CR1_CPHA_Msk                   /*!<  */
 
#define SPI_CR1_CPOL_Pos                    (1U)
#define SPI_CR1_CPOL_Msk                    (0x01U << SPI_CR1_CPOL_Pos)        /*!<  */
#define SPI_CR1_CPOL                        SPI_CR1_CPOL_Msk                   /*!<  */
 
#define SPI_CR1_MST_Pos                    (2U)
#define SPI_CR1_MST_Msk                    (0x01U << SPI_CR1_MST_Pos)        /*!<  */
#define SPI_CR1_MST                        SPI_CR1_MST_Msk                   /*!<  */
 
#define SPI_CR1_BR_Pos                    (3U)
#define SPI_CR1_BR_Msk                    (0x7U << SPI_CR1_BR_Pos)        /*!<  */
#define SPI_CR1_BR                        SPI_CR1_BR_Msk                   /*!<  */
 
#define SPI_CR1_SPE_Pos                    (6U)
#define SPI_CR1_SPE_Msk                    (0x01U << SPI_CR1_SPE_Pos)        /*!<  */
#define SPI_CR1_SPE                        SPI_CR1_SPE_Msk                   /*!<  */
 
#define SPI_CR1_LSBFIRST_Pos                    (7U)
#define SPI_CR1_LSBFIRST_Msk                    (0x01U << SPI_CR1_LSBFIRST_Pos)        /*!<  */
#define SPI_CR1_LSBFIRST                        SPI_CR1_LSBFIRST_Msk                   /*!<  */
 
#define SPI_CR1_SSI_Pos                    (8U)
#define SPI_CR1_SSI_Msk                    (0x01U << SPI_CR1_SSI_Pos)        /*!<  */
#define SPI_CR1_SSI                        SPI_CR1_SSI_Msk                   /*!<  */
 
#define SPI_CR1_SSM_Pos                    (9U)
#define SPI_CR1_SSM_Msk                    (0x01U << SPI_CR1_SSM_Pos)        /*!<  */
#define SPI_CR1_SSM                        SPI_CR1_SSM_Msk                   /*!<  */
 
#define SPI_CR1_RXONLY_Pos                    (10U)
#define SPI_CR1_RXONLY_Msk                    (0x01U << SPI_CR1_RXONLY_Pos)        /*!<  */
#define SPI_CR1_RXONLY                        SPI_CR1_RXONLY_Msk                   /*!<  */
 
#define SPI_CR1_DFF_Pos                    (11U)
#define SPI_CR1_DFF_Msk                    (0x01U << SPI_CR1_DFF_Pos)        /*!<  */
#define SPI_CR1_DFF                        SPI_CR1_DFF_Msk                   /*!<  */
 
#define SPI_CR1_CRCNEXT_Pos                    (12U)
#define SPI_CR1_CRCNEXT_Msk                    (0x01U << SPI_CR1_CRCNEXT_Pos)        /*!<  */
#define SPI_CR1_CRCNEXT                        SPI_CR1_CRCNEXT_Msk                   /*!<  */
 
#define SPI_CR1_CRCEN_Pos                    (13U)
#define SPI_CR1_CRCEN_Msk                    (0x01U << SPI_CR1_CRCEN_Pos)        /*!<  */
#define SPI_CR1_CRCEN                        SPI_CR1_CRCEN_Msk                   /*!<  */
 
#define SPI_CR1_BIDIOE_Pos                    (14U)
#define SPI_CR1_BIDIOE_Msk                    (0x01U << SPI_CR1_BIDIOE_Pos)        /*!<  */
#define SPI_CR1_BIDIOE                        SPI_CR1_BIDIOE_Msk                   /*!<  */
 
#define SPI_CR1_BIDIMODE_Pos                    (15U)
#define SPI_CR1_BIDIMODE_Msk                    (0x01U << SPI_CR1_BIDIMODE_Pos)        /*!<  */
#define SPI_CR1_BIDIMODE                        SPI_CR1_BIDIMODE_Msk                   /*!<  */
 
#define SPI_CR1_ALL_Msk                    (SPI_CR1_BIDIMODE_Msk | SPI_CR1_BIDIOE_Msk | SPI_CR1_CRCEN_Msk | SPI_CR1_CRCNEXT_Msk | SPI_CR1_DFF_Msk | SPI_CR1_RXONLY_Msk | SPI_CR1_SSM_Msk | SPI_CR1_SSI_Msk | SPI_CR1_LSBFIRST_Msk | SPI_CR1_SPE_Msk | SPI_CR1_BR_Msk | SPI_CR1_MST_Msk | SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk)
 
/******************  Bit definition for SPI_CR2 register ********************/
#define SPI_CR2_RXDMAEN_Pos                    (0U)
#define SPI_CR2_RXDMAEN_Msk                    (0x01U << SPI_CR2_RXDMAEN_Pos)        /*!<  */
#define SPI_CR2_RXDMAEN                        SPI_CR2_RXDMAEN_Msk                   /*!<  */
 
#define SPI_CR2_TXDMAEN_Pos                    (1U)
#define SPI_CR2_TXDMAEN_Msk                    (0x01U << SPI_CR2_TXDMAEN_Pos)        /*!<  */
#define SPI_CR2_TXDMAEN                        SPI_CR2_TXDMAEN_Msk                   /*!<  */
 
#define SPI_CR2_SSOE_Pos                    (2U)
#define SPI_CR2_SSOE_Msk                    (0x01U << SPI_CR2_SSOE_Pos)        /*!<  */
#define SPI_CR2_SSOE                        SPI_CR2_SSOE_Msk                   /*!<  */
 
#define SPI_CR2_ERRIE_Pos                    (5U)
#define SPI_CR2_ERRIE_Msk                    (0x01U << SPI_CR2_ERRIE_Pos)        /*!<  */
#define SPI_CR2_ERRIE                        SPI_CR2_ERRIE_Msk                   /*!<  */
 
#define SPI_CR2_RXNEIE_Pos                    (6U)
#define SPI_CR2_RXNEIE_Msk                    (0x01U << SPI_CR2_RXNEIE_Pos)        /*!<  */
#define SPI_CR2_RXNEIE                        SPI_CR2_RXNEIE_Msk                   /*!<  */
 
#define SPI_CR2_TXEIE_Pos                    (7U)
#define SPI_CR2_TXEIE_Msk                    (0x01U << SPI_CR2_TXEIE_Pos)        /*!<  */
#define SPI_CR2_TXEIE                        SPI_CR2_TXEIE_Msk                   /*!<  */
 
#define SPI_CR2_ALL_Msk                    (SPI_CR2_TXEIE_Msk | SPI_CR2_RXNEIE_Msk | SPI_CR2_ERRIE_Msk | SPI_CR2_SSOE_Msk | SPI_CR2_TXDMAEN_Msk | SPI_CR2_RXDMAEN_Msk)
 
/******************  Bit definition for SPI_SR register ********************/
#define SPI_SR_RXNE_Pos                    (0U)
#define SPI_SR_RXNE_Msk                    (0x01U << SPI_SR_RXNE_Pos)        /*!<  */
#define SPI_SR_RXNE                        SPI_SR_RXNE_Msk                   /*!<  */
 
#define SPI_SR_TXE_Pos                    (1U)
#define SPI_SR_TXE_Msk                    (0x01U << SPI_SR_TXE_Pos)        /*!<  */
#define SPI_SR_TXE                        SPI_SR_TXE_Msk                   /*!<  */
 
#define SPI_SR_CHSIDE_Pos                    (2U)
#define SPI_SR_CHSIDE_Msk                    (0x01U << SPI_SR_CHSIDE_Pos)        /*!<  */
#define SPI_SR_CHSIDE                        SPI_SR_CHSIDE_Msk                   /*!<  */
 
#define SPI_SR_UDR_Pos                    (3U)
#define SPI_SR_UDR_Msk                    (0x01U << SPI_SR_UDR_Pos)        /*!<  */
#define SPI_SR_UDR                        SPI_SR_UDR_Msk                   /*!<  */
 
#define SPI_SR_CRCERR_Pos                    (4U)
#define SPI_SR_CRCERR_Msk                    (0x01U << SPI_SR_CRCERR_Pos)        /*!<  */
#define SPI_SR_CRCERR                        SPI_SR_CRCERR_Msk                   /*!<  */
 
#define SPI_SR_MODF_Pos                    (5U)
#define SPI_SR_MODF_Msk                    (0x01U << SPI_SR_MODF_Pos)        /*!<  */
#define SPI_SR_MODF                        SPI_SR_MODF_Msk                   /*!<  */
 
#define SPI_SR_OVR_Pos                    (6U)
#define SPI_SR_OVR_Msk                    (0x01U << SPI_SR_OVR_Pos)        /*!<  */
#define SPI_SR_OVR                        SPI_SR_OVR_Msk                   /*!<  */
 
#define SPI_SR_BSY_Pos                    (7U)
#define SPI_SR_BSY_Msk                    (0x01U << SPI_SR_BSY_Pos)        /*!<  */
#define SPI_SR_BSY                        SPI_SR_BSY_Msk                   /*!<  */
 
#define SPI_SR_ALL_Msk                    (SPI_SR_BSY_Msk | SPI_SR_OVR_Msk | SPI_SR_MODF_Msk | SPI_SR_CRCERR_Msk | SPI_SR_UDR_Msk | SPI_SR_CHSIDE_Msk | SPI_SR_TXE_Msk | SPI_SR_RXNE_Msk)
 
/******************  Bit definition for SPI_DR register ********************/
#define SPI_DR_DR_Pos                    (0U)
#define SPI_DR_DR_Msk                    (0xFFFFU << SPI_DR_DR_Pos)        /*!<  */
#define SPI_DR_DR                        SPI_DR_DR_Msk                   /*!<  */
 
#define SPI_DR_ALL_Msk                    (SPI_DR_DR_Msk)
 
/******************  Bit definition for SPI_CRCPR register ********************/
#define SPI_CRCPR_CRCPOLY_Pos                    (0U)
#define SPI_CRCPR_CRCPOLY_Msk                    (0xFFFFU << SPI_CRCPR_CRCPOLY_Pos)        /*!<  */
#define SPI_CRCPR_CRCPOLY                        SPI_CRCPR_CRCPOLY_Msk                   /*!<  */
 
#define SPI_CRCPR_ALL_Msk                    (SPI_CRCPR_CRCPOLY_Msk)
 
/******************  Bit definition for SPI_RXCRCR register ********************/
#define SPI_RXCRCR_RXCRC_Pos                    (0U)
#define SPI_RXCRCR_RXCRC_Msk                    (0xFFFFU << SPI_RXCRCR_RXCRC_Pos)        /*!<  */
#define SPI_RXCRCR_RXCRC                        SPI_RXCRCR_RXCRC_Msk                   /*!<  */
 
#define SPI_RXCRCR_ALL_Msk                    (SPI_RXCRCR_RXCRC_Msk)
 
/******************  Bit definition for SPI_TXCRCR register ********************/
#define SPI_TXCRCR_TXCRC_Pos                    (0U)
#define SPI_TXCRCR_TXCRC_Msk                    (0xFFFFU << SPI_TXCRCR_TXCRC_Pos)        /*!<  */
#define SPI_TXCRCR_TXCRC                        SPI_TXCRCR_TXCRC_Msk                   /*!<  */
 
#define SPI_TXCRCR_ALL_Msk                    (SPI_TXCRCR_TXCRC_Msk)
 
/******************  Bit definition for USART_SR register ********************/
#define USART_SR_PE_Pos                    (0U)
#define USART_SR_PE_Msk                    (0x01U << USART_SR_PE_Pos)        /*!<  */
#define USART_SR_PE                        USART_SR_PE_Msk                   /*!<  */
 
#define USART_SR_FE_Pos                    (1U)
#define USART_SR_FE_Msk                    (0x01U << USART_SR_FE_Pos)        /*!<  */
#define USART_SR_FE                        USART_SR_FE_Msk                   /*!<  */
 
#define USART_SR_NE_Pos                    (2U)
#define USART_SR_NE_Msk                    (0x01U << USART_SR_NE_Pos)        /*!<  */
#define USART_SR_NE                        USART_SR_NE_Msk                   /*!<  */
 
#define USART_SR_ORE_Pos                    (3U)
#define USART_SR_ORE_Msk                    (0x01U << USART_SR_ORE_Pos)        /*!<  */
#define USART_SR_ORE                        USART_SR_ORE_Msk                   /*!<  */
 
#define USART_SR_IDLE_Pos                    (4U)
#define USART_SR_IDLE_Msk                    (0x01U << USART_SR_IDLE_Pos)        /*!<  */
#define USART_SR_IDLE                        USART_SR_IDLE_Msk                   /*!<  */
 
#define USART_SR_RXNE_Pos                    (5U)
#define USART_SR_RXNE_Msk                    (0x01U << USART_SR_RXNE_Pos)        /*!<  */
#define USART_SR_RXNE                        USART_SR_RXNE_Msk                   /*!<  */
 
#define USART_SR_TC_Pos                    (6U)
#define USART_SR_TC_Msk                    (0x01U << USART_SR_TC_Pos)        /*!<  */
#define USART_SR_TC                        USART_SR_TC_Msk                   /*!<  */
 
#define USART_SR_TXE_Pos                    (7U)
#define USART_SR_TXE_Msk                    (0x01U << USART_SR_TXE_Pos)        /*!<  */
#define USART_SR_TXE                        USART_SR_TXE_Msk                   /*!<  */
 
#define USART_SR_LBD_Pos                    (8U)
#define USART_SR_LBD_Msk                    (0x01U << USART_SR_LBD_Pos)        /*!<  */
#define USART_SR_LBD                        USART_SR_LBD_Msk                   /*!<  */
 
#define USART_SR_CTS_Pos                    (9U)
#define USART_SR_CTS_Msk                    (0x01U << USART_SR_CTS_Pos)        /*!<  */
#define USART_SR_CTS                        USART_SR_CTS_Msk                   /*!<  */
 
#define USART_SR_ALL_Msk                    (USART_SR_CTS_Msk | USART_SR_LBD_Msk | USART_SR_TXE_Msk | USART_SR_TC_Msk | USART_SR_RXNE_Msk | USART_SR_IDLE_Msk | USART_SR_ORE_Msk | USART_SR_NE_Msk | USART_SR_FE_Msk | USART_SR_PE_Msk)
 
/******************  Bit definition for USART_DR register ********************/
#define USART_DR_DR_Pos                    (0U)
#define USART_DR_DR_Msk                    (0x1FFU << USART_DR_DR_Pos)        /*!<  */
#define USART_DR_DR                        USART_DR_DR_Msk                   /*!<  */
 
#define USART_DR_ALL_Msk                    (USART_DR_DR_Msk)
 
/******************  Bit definition for USART_BRR register ********************/
#define USART_BRR_DIV_FRACITON_Pos                    (0U)
#define USART_BRR_DIV_FRACITON_Msk                    (0xFU << USART_BRR_DIV_FRACITON_Pos)        /*!<  */
#define USART_BRR_DIV_FRACITON                        USART_BRR_DIV_FRACITON_Msk                   /*!<  */
 
#define USART_BRR_DIV_MANTISSA_Pos                    (4U)
#define USART_BRR_DIV_MANTISSA_Msk                    (0xFFFU << USART_BRR_DIV_MANTISSA_Pos)        /*!<  */
#define USART_BRR_DIV_MANTISSA                        USART_BRR_DIV_MANTISSA_Msk                   /*!<  */
 
#define USART_BRR_ALL_Msk                    (USART_BRR_DIV_MANTISSA_Msk | USART_BRR_DIV_FRACITON_Msk)
 
/******************  Bit definition for USART_CR1 register ********************/
#define USART_CR1_SBK_Pos                    (0U)
#define USART_CR1_SBK_Msk                    (0x01U << USART_CR1_SBK_Pos)        /*!<  */
#define USART_CR1_SBK                        USART_CR1_SBK_Msk                   /*!<  */
 
#define USART_CR1_RWU_Pos                    (1U)
#define USART_CR1_RWU_Msk                    (0x01U << USART_CR1_RWU_Pos)        /*!<  */
#define USART_CR1_RWU                        USART_CR1_RWU_Msk                   /*!<  */
 
#define USART_CR1_RE_Pos                    (2U)
#define USART_CR1_RE_Msk                    (0x01U << USART_CR1_RE_Pos)        /*!<  */
#define USART_CR1_RE                        USART_CR1_RE_Msk                   /*!<  */
 
#define USART_CR1_TE_Pos                    (3U)
#define USART_CR1_TE_Msk                    (0x01U << USART_CR1_TE_Pos)        /*!<  */
#define USART_CR1_TE                        USART_CR1_TE_Msk                   /*!<  */
 
#define USART_CR1_IDLEIE_Pos                    (4U)
#define USART_CR1_IDLEIE_Msk                    (0x01U << USART_CR1_IDLEIE_Pos)        /*!<  */
#define USART_CR1_IDLEIE                        USART_CR1_IDLEIE_Msk                   /*!<  */
 
#define USART_CR1_RXNEIE_Pos                    (5U)
#define USART_CR1_RXNEIE_Msk                    (0x01U << USART_CR1_RXNEIE_Pos)        /*!<  */
#define USART_CR1_RXNEIE                        USART_CR1_RXNEIE_Msk                   /*!<  */
 
#define USART_CR1_TCIE_Pos                    (6U)
#define USART_CR1_TCIE_Msk                    (0x01U << USART_CR1_TCIE_Pos)        /*!<  */
#define USART_CR1_TCIE                        USART_CR1_TCIE_Msk                   /*!<  */
 
#define USART_CR1_TXEIE_Pos                    (7U)
#define USART_CR1_TXEIE_Msk                    (0x01U << USART_CR1_TXEIE_Pos)        /*!<  */
#define USART_CR1_TXEIE                        USART_CR1_TXEIE_Msk                   /*!<  */
 
#define USART_CR1_PEIE_Pos                    (8U)
#define USART_CR1_PEIE_Msk                    (0x01U << USART_CR1_PEIE_Pos)        /*!<  */
#define USART_CR1_PEIE                        USART_CR1_PEIE_Msk                   /*!<  */
 
#define USART_CR1_PS_Pos                    (9U)
#define USART_CR1_PS_Msk                    (0x01U << USART_CR1_PS_Pos)        /*!<  */
#define USART_CR1_PS                        USART_CR1_PS_Msk                   /*!<  */
 
#define USART_CR1_PCE_Pos                    (10U)
#define USART_CR1_PCE_Msk                    (0x01U << USART_CR1_PCE_Pos)        /*!<  */
#define USART_CR1_PCE                        USART_CR1_PCE_Msk                   /*!<  */
 
#define USART_CR1_WAKE_Pos                    (11U)
#define USART_CR1_WAKE_Msk                    (0x01U << USART_CR1_WAKE_Pos)        /*!<  */
#define USART_CR1_WAKE                        USART_CR1_WAKE_Msk                   /*!<  */
 
#define USART_CR1_WD_LEN_Pos                    (12U)
#define USART_CR1_WD_LEN_Msk                    (0x01U << USART_CR1_WD_LEN_Pos)        /*!<  */
#define USART_CR1_WD_LEN                        USART_CR1_WD_LEN_Msk                   /*!<  */
 
#define USART_CR1_UE_Pos                    (13U)
#define USART_CR1_UE_Msk                    (0x01U << USART_CR1_UE_Pos)        /*!<  */
#define USART_CR1_UE                        USART_CR1_UE_Msk                   /*!<  */
 
#define USART_CR1_ALL_Msk                    (USART_CR1_UE_Msk | USART_CR1_WD_LEN_Msk | USART_CR1_WAKE_Msk | USART_CR1_PCE_Msk | USART_CR1_PS_Msk | USART_CR1_PEIE_Msk | USART_CR1_TXEIE_Msk | USART_CR1_TCIE_Msk | USART_CR1_RXNEIE_Msk | USART_CR1_IDLEIE_Msk | USART_CR1_TE_Msk | USART_CR1_RE_Msk | USART_CR1_RWU_Msk | USART_CR1_SBK_Msk)
 
/******************  Bit definition for USART_CR2 register ********************/
#define USART_CR2_ADD_Pos                    (0U)
#define USART_CR2_ADD_Msk                    (0xFU << USART_CR2_ADD_Pos)        /*!<  */
#define USART_CR2_ADD                        USART_CR2_ADD_Msk                   /*!<  */
 
#define USART_CR2_LBDL_Pos                    (5U)
#define USART_CR2_LBDL_Msk                    (0x01U << USART_CR2_LBDL_Pos)        /*!<  */
#define USART_CR2_LBDL                        USART_CR2_LBDL_Msk                   /*!<  */
 
#define USART_CR2_LBDIE_Pos                    (6U)
#define USART_CR2_LBDIE_Msk                    (0x01U << USART_CR2_LBDIE_Pos)        /*!<  */
#define USART_CR2_LBDIE                        USART_CR2_LBDIE_Msk                   /*!<  */
 
#define USART_CR2_LBCL_Pos                    (8U)
#define USART_CR2_LBCL_Msk                    (0x01U << USART_CR2_LBCL_Pos)        /*!<  */
#define USART_CR2_LBCL                        USART_CR2_LBCL_Msk                   /*!<  */
 
#define USART_CR2_CPHA_Pos                    (9U)
#define USART_CR2_CPHA_Msk                    (0x01U << USART_CR2_CPHA_Pos)        /*!<  */
#define USART_CR2_CPHA                        USART_CR2_CPHA_Msk                   /*!<  */
 
#define USART_CR2_CPOL_Pos                    (10U)
#define USART_CR2_CPOL_Msk                    (0x01U << USART_CR2_CPOL_Pos)        /*!<  */
#define USART_CR2_CPOL                        USART_CR2_CPOL_Msk                   /*!<  */
 
#define USART_CR2_CLKEN_Pos                    (11U)
#define USART_CR2_CLKEN_Msk                    (0x01U << USART_CR2_CLKEN_Pos)        /*!<  */
#define USART_CR2_CLKEN                        USART_CR2_CLKEN_Msk                   /*!<  */
 
#define USART_CR2_STOP_Pos                    (12U)
#define USART_CR2_STOP_Msk                    (0x3U << USART_CR2_STOP_Pos)        /*!<  */
#define USART_CR2_STOP                        USART_CR2_STOP_Msk                   /*!<  */
 
#define USART_CR2_LINEN_Pos                    (14U)
#define USART_CR2_LINEN_Msk                    (0x01U << USART_CR2_LINEN_Pos)        /*!<  */
#define USART_CR2_LINEN                        USART_CR2_LINEN_Msk                   /*!<  */
 
#define USART_CR2_ALL_Msk                    (USART_CR2_LINEN_Msk | USART_CR2_STOP_Msk | USART_CR2_CLKEN_Msk | USART_CR2_CPOL_Msk | USART_CR2_CPHA_Msk | USART_CR2_LBCL_Msk | USART_CR2_LBDIE_Msk | USART_CR2_LBDL_Msk | USART_CR2_ADD_Msk)
 
/******************  Bit definition for USART_CR3 register ********************/
#define USART_CR3_EIE_Pos                    (0U)
#define USART_CR3_EIE_Msk                    (0x01U << USART_CR3_EIE_Pos)        /*!<  */
#define USART_CR3_EIE                        USART_CR3_EIE_Msk                   /*!<  */
 
#define USART_CR3_IREN_Pos                    (1U)
#define USART_CR3_IREN_Msk                    (0x01U << USART_CR3_IREN_Pos)        /*!<  */
#define USART_CR3_IREN                        USART_CR3_IREN_Msk                   /*!<  */
 
#define USART_CR3_IRLP_Pos                    (2U)
#define USART_CR3_IRLP_Msk                    (0x01U << USART_CR3_IRLP_Pos)        /*!<  */
#define USART_CR3_IRLP                        USART_CR3_IRLP_Msk                   /*!<  */
 
#define USART_CR3_HDSEL_Pos                    (3U)
#define USART_CR3_HDSEL_Msk                    (0x01U << USART_CR3_HDSEL_Pos)        /*!<  */
#define USART_CR3_HDSEL                        USART_CR3_HDSEL_Msk                   /*!<  */
 
#define USART_CR3_NACK_Pos                    (4U)
#define USART_CR3_NACK_Msk                    (0x01U << USART_CR3_NACK_Pos)        /*!<  */
#define USART_CR3_NACK                        USART_CR3_NACK_Msk                   /*!<  */
 
#define USART_CR3_SCEN_Pos                    (5U)
#define USART_CR3_SCEN_Msk                    (0x01U << USART_CR3_SCEN_Pos)        /*!<  */
#define USART_CR3_SCEN                        USART_CR3_SCEN_Msk                   /*!<  */
 
#define USART_CR3_DMAR_Pos                    (6U)
#define USART_CR3_DMAR_Msk                    (0x01U << USART_CR3_DMAR_Pos)        /*!<  */
#define USART_CR3_DMAR                        USART_CR3_DMAR_Msk                   /*!<  */
 
#define USART_CR3_DMAT_Pos                    (7U)
#define USART_CR3_DMAT_Msk                    (0x01U << USART_CR3_DMAT_Pos)        /*!<  */
#define USART_CR3_DMAT                        USART_CR3_DMAT_Msk                   /*!<  */
 
#define USART_CR3_RTSE_Pos                    (8U)
#define USART_CR3_RTSE_Msk                    (0x01U << USART_CR3_RTSE_Pos)        /*!<  */
#define USART_CR3_RTSE                        USART_CR3_RTSE_Msk                   /*!<  */
 
#define USART_CR3_CTSE_Pos                    (9U)
#define USART_CR3_CTSE_Msk                    (0x01U << USART_CR3_CTSE_Pos)        /*!<  */
#define USART_CR3_CTSE                        USART_CR3_CTSE_Msk                   /*!<  */
 
#define USART_CR3_CTSIE_Pos                    (10U)
#define USART_CR3_CTSIE_Msk                    (0x01U << USART_CR3_CTSIE_Pos)        /*!<  */
#define USART_CR3_CTSIE                        USART_CR3_CTSIE_Msk                   /*!<  */
 
#define USART_CR3_ALL_Msk                    (USART_CR3_CTSIE_Msk | USART_CR3_CTSE_Msk | USART_CR3_RTSE_Msk | USART_CR3_DMAT_Msk | USART_CR3_DMAR_Msk | USART_CR3_SCEN_Msk | USART_CR3_NACK_Msk | USART_CR3_HDSEL_Msk | USART_CR3_IRLP_Msk | USART_CR3_IREN_Msk | USART_CR3_EIE_Msk)
 
/******************  Bit definition for USART_GTPR register ********************/
#define USART_GTPR_PSC_Pos                    (0U)
#define USART_GTPR_PSC_Msk                    (0xFFU << USART_GTPR_PSC_Pos)        /*!<  */
#define USART_GTPR_PSC                        USART_GTPR_PSC_Msk                   /*!<  */
 
#define USART_GTPR_GT_Pos                    (8U)
#define USART_GTPR_GT_Msk                    (0xFFU << USART_GTPR_GT_Pos)        /*!<  */
#define USART_GTPR_GT                        USART_GTPR_GT_Msk                   /*!<  */
 
#define USART_GTPR_ALL_Msk                    (USART_GTPR_GT_Msk | USART_GTPR_PSC_Msk)
 
/******************  Bit definition for DBGMCU_ID register ********************/
#define DBGMCU_ID_ID_Pos                    (0U)
#define DBGMCU_ID_ID_Msk                    (0xFFFFFFFFU << DBGMCU_ID_ID_Pos)        /*!<  */
#define DBGMCU_ID_ID                        DBGMCU_ID_ID_Msk                   /*!<  */
 
#define DBGMCU_ID_ALL_Msk                    (DBGMCU_ID_ID_Msk)
 
/******************  Bit definition for DBGMCU_CTL register ********************/
#define DBGMCU_CTL_EN_SLEEP_Pos                    (0U)
#define DBGMCU_CTL_EN_SLEEP_Msk                    (0x01U << DBGMCU_CTL_EN_SLEEP_Pos)        /*!<  */
#define DBGMCU_CTL_EN_SLEEP                        DBGMCU_CTL_EN_SLEEP_Msk                   /*!<  */
 
#define DBGMCU_CTL_EN_STOP_Pos                    (1U)
#define DBGMCU_CTL_EN_STOP_Msk                    (0x01U << DBGMCU_CTL_EN_STOP_Pos)        /*!<  */
#define DBGMCU_CTL_EN_STOP                        DBGMCU_CTL_EN_STOP_Msk                   /*!<  */
 
#define DBGMCU_CTL_EN_STANDBY_Pos                    (2U)
#define DBGMCU_CTL_EN_STANDBY_Msk                    (0x01U << DBGMCU_CTL_EN_STANDBY_Pos)        /*!<  */
#define DBGMCU_CTL_EN_STANDBY                        DBGMCU_CTL_EN_STANDBY_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_RTC_Pos                    (3U)
#define DBGMCU_CTL_STOP_RTC_Msk                    (0x01U << DBGMCU_CTL_STOP_RTC_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_RTC                        DBGMCU_CTL_STOP_RTC_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_WWDG_Pos                    (4U)
#define DBGMCU_CTL_STOP_WWDG_Msk                    (0x01U << DBGMCU_CTL_STOP_WWDG_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_WWDG                        DBGMCU_CTL_STOP_WWDG_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_IWDG_Pos                    (5U)
#define DBGMCU_CTL_STOP_IWDG_Msk                    (0x01U << DBGMCU_CTL_STOP_IWDG_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_IWDG                        DBGMCU_CTL_STOP_IWDG_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_I2C1_Pos                    (6U)
#define DBGMCU_CTL_STOP_I2C1_Msk                    (0x01U << DBGMCU_CTL_STOP_I2C1_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_I2C1                        DBGMCU_CTL_STOP_I2C1_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_I2C2_Pos                    (7U)
#define DBGMCU_CTL_STOP_I2C2_Msk                    (0x01U << DBGMCU_CTL_STOP_I2C2_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_I2C2                        DBGMCU_CTL_STOP_I2C2_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_BEEP_Pos                    (8U)
#define DBGMCU_CTL_STOP_BEEP_Msk                    (0x01U << DBGMCU_CTL_STOP_BEEP_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_BEEP                        DBGMCU_CTL_STOP_BEEP_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_AWKTIM_Pos                    (9U)
#define DBGMCU_CTL_STOP_AWKTIM_Msk                    (0x01U << DBGMCU_CTL_STOP_AWKTIM_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_AWKTIM                        DBGMCU_CTL_STOP_AWKTIM_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM1_Pos                    (10U)
#define DBGMCU_CTL_STOP_TIM1_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM1_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM1                        DBGMCU_CTL_STOP_TIM1_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM3_Pos                    (11U)
#define DBGMCU_CTL_STOP_TIM3_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM3_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM3                        DBGMCU_CTL_STOP_TIM3_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM6_Pos                    (12U)
#define DBGMCU_CTL_STOP_TIM6_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM6_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM6                        DBGMCU_CTL_STOP_TIM6_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM7_Pos                    (13U)
#define DBGMCU_CTL_STOP_TIM7_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM7_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM7                        DBGMCU_CTL_STOP_TIM7_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM14_Pos                    (14U)
#define DBGMCU_CTL_STOP_TIM14_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM14_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM14                        DBGMCU_CTL_STOP_TIM14_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM15_Pos                    (15U)
#define DBGMCU_CTL_STOP_TIM15_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM15_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM15                        DBGMCU_CTL_STOP_TIM15_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM16_Pos                    (16U)
#define DBGMCU_CTL_STOP_TIM16_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM16_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM16                        DBGMCU_CTL_STOP_TIM16_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_TIM17_Pos                    (17U)
#define DBGMCU_CTL_STOP_TIM17_Msk                    (0x01U << DBGMCU_CTL_STOP_TIM17_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_TIM17                        DBGMCU_CTL_STOP_TIM17_Msk                   /*!<  */
 
#define DBGMCU_CTL_STOP_LPTIM_Pos                    (19U)
#define DBGMCU_CTL_STOP_LPTIM_Msk                    (0x01U << DBGMCU_CTL_STOP_LPTIM_Pos)        /*!<  */
#define DBGMCU_CTL_STOP_LPTIM                        DBGMCU_CTL_STOP_LPTIM_Msk                   /*!<  */
 
#define DBGMCU_CTL_ALL_Msk                    (DBGMCU_CTL_STOP_LPTIM_Msk | DBGMCU_CTL_STOP_TIM17_Msk | DBGMCU_CTL_STOP_TIM16_Msk | DBGMCU_CTL_STOP_TIM15_Msk | DBGMCU_CTL_STOP_TIM14_Msk | DBGMCU_CTL_STOP_TIM7_Msk | DBGMCU_CTL_STOP_TIM6_Msk | DBGMCU_CTL_STOP_TIM3_Msk | DBGMCU_CTL_STOP_TIM1_Msk | DBGMCU_CTL_STOP_AWKTIM_Msk | DBGMCU_CTL_STOP_BEEP_Msk | DBGMCU_CTL_STOP_I2C2_Msk | DBGMCU_CTL_STOP_I2C1_Msk | DBGMCU_CTL_STOP_IWDG_Msk | DBGMCU_CTL_STOP_WWDG_Msk | DBGMCU_CTL_STOP_RTC_Msk | DBGMCU_CTL_EN_STANDBY_Msk | DBGMCU_CTL_EN_STOP_Msk | DBGMCU_CTL_EN_SLEEP_Msk)
 
/******************  Bit definition for DBGMCU_RESERVED register ********************/
#define DBGMCU_RESERVED_SCLK_AO_Pos                    (0U)
#define DBGMCU_RESERVED_SCLK_AO_Msk                    (0x01U << DBGMCU_RESERVED_SCLK_AO_Pos)        /*!<  */
#define DBGMCU_RESERVED_SCLK_AO                        DBGMCU_RESERVED_SCLK_AO_Msk                   /*!<  */
 
#define DBGMCU_RESERVED_ALL_Msk                    (DBGMCU_RESERVED_SCLK_AO_Msk)
 
/******************  Bit definition for DBGMCU_DBGEN register ********************/
#define DBGMCU_DBGEN_DBGEN_Pos                    (0U)
#define DBGMCU_DBGEN_DBGEN_Msk                    (0x01U << DBGMCU_DBGEN_DBGEN_Pos)        /*!<  */
#define DBGMCU_DBGEN_DBGEN                        DBGMCU_DBGEN_DBGEN_Msk                   /*!<  */
 
#define DBGMCU_DBGEN_ALL_Msk                    (DBGMCU_DBGEN_DBGEN_Msk)
 
/******************  Bit definition for LPTIM_CNTVAL register ********************/
#define LPTIM_CNTVAL_LPT_CNT_Pos                    (0U)
#define LPTIM_CNTVAL_LPT_CNT_Msk                    (0xFFFFU << LPTIM_CNTVAL_LPT_CNT_Pos)        /*!<  */
#define LPTIM_CNTVAL_LPT_CNT                        LPTIM_CNTVAL_LPT_CNT_Msk                   /*!<  */
 
#define LPTIM_CNTVAL_ALL_Msk                    (LPTIM_CNTVAL_LPT_CNT_Msk)
 
/******************  Bit definition for LPTIM_CR register ********************/
#define LPTIM_CR_TIM_RUN_Pos                    (0U)
#define LPTIM_CR_TIM_RUN_Msk                    (0x01U << LPTIM_CR_TIM_RUN_Pos)        /*!<  */
#define LPTIM_CR_TIM_RUN                        LPTIM_CR_TIM_RUN_Msk                   /*!<  */
 
#define LPTIM_CR_MODE_Pos                    (1U)
#define LPTIM_CR_MODE_Msk                    (0x01U << LPTIM_CR_MODE_Pos)        /*!<  */
#define LPTIM_CR_MODE                        LPTIM_CR_MODE_Msk                   /*!<  */
 
#define LPTIM_CR_CT_SEL_Pos                    (2U)
#define LPTIM_CR_CT_SEL_Msk                    (0x01U << LPTIM_CR_CT_SEL_Pos)        /*!<  */
#define LPTIM_CR_CT_SEL                        LPTIM_CR_CT_SEL_Msk                   /*!<  */
 
#define LPTIM_CR_TOG_EN_Pos                    (3U)
#define LPTIM_CR_TOG_EN_Msk                    (0x01U << LPTIM_CR_TOG_EN_Pos)        /*!<  */
#define LPTIM_CR_TOG_EN                        LPTIM_CR_TOG_EN_Msk                   /*!<  */
 
#define LPTIM_CR_TCK_SEL_Pos                    (4U)
#define LPTIM_CR_TCK_SEL_Msk                    (0x3U << LPTIM_CR_TCK_SEL_Pos)        /*!<  */
#define LPTIM_CR_TCK_SEL                        LPTIM_CR_TCK_SEL_Msk                   /*!<  */
 
#define LPTIM_CR_GATE_EN_Pos                    (6U)
#define LPTIM_CR_GATE_EN_Msk                    (0x01U << LPTIM_CR_GATE_EN_Pos)        /*!<  */
#define LPTIM_CR_GATE_EN                        LPTIM_CR_GATE_EN_Msk                   /*!<  */
 
#define LPTIM_CR_GATE_P_Pos                    (7U)
#define LPTIM_CR_GATE_P_Msk                    (0x01U << LPTIM_CR_GATE_P_Pos)        /*!<  */
#define LPTIM_CR_GATE_P                        LPTIM_CR_GATE_P_Msk                   /*!<  */
 
#define LPTIM_CR_INT_EN_Pos                    (8U)
#define LPTIM_CR_INT_EN_Msk                    (0x01U << LPTIM_CR_INT_EN_Pos)        /*!<  */
#define LPTIM_CR_INT_EN                        LPTIM_CR_INT_EN_Msk                   /*!<  */
 
#define LPTIM_CR_TCK_EN_Pos                    (9U)
#define LPTIM_CR_TCK_EN_Msk                    (0x01U << LPTIM_CR_TCK_EN_Pos)        /*!<  */
#define LPTIM_CR_TCK_EN                        LPTIM_CR_TCK_EN_Msk                   /*!<  */
 
#define LPTIM_CR_WT_FLAG_Pos                    (16U)
#define LPTIM_CR_WT_FLAG_Msk                    (0x01U << LPTIM_CR_WT_FLAG_Pos)        /*!<  */
#define LPTIM_CR_WT_FLAG                        LPTIM_CR_WT_FLAG_Msk                   /*!<  */
 
#define LPTIM_CR_ALL_Msk                    (LPTIM_CR_WT_FLAG_Msk | LPTIM_CR_TCK_EN_Msk | LPTIM_CR_INT_EN_Msk | LPTIM_CR_GATE_P_Msk | LPTIM_CR_GATE_EN_Msk | LPTIM_CR_TCK_SEL_Msk | LPTIM_CR_TOG_EN_Msk | LPTIM_CR_CT_SEL_Msk | LPTIM_CR_MODE_Msk | LPTIM_CR_TIM_RUN_Msk)
 
/******************  Bit definition for LPTIM_LOAD register ********************/
#define LPTIM_LOAD_LOAD_Pos                    (0U)
#define LPTIM_LOAD_LOAD_Msk                    (0xFFFFU << LPTIM_LOAD_LOAD_Pos)        /*!<  */
#define LPTIM_LOAD_LOAD                        LPTIM_LOAD_LOAD_Msk                   /*!<  */
 
#define LPTIM_LOAD_ALL_Msk                    (LPTIM_LOAD_LOAD_Msk)
 
/******************  Bit definition for LPTIM_INTSR register ********************/
#define LPTIM_INTSR_INTF_Pos                    (0U)
#define LPTIM_INTSR_INTF_Msk                    (0x01U << LPTIM_INTSR_INTF_Pos)        /*!<  */
#define LPTIM_INTSR_INTF                        LPTIM_INTSR_INTF_Msk                   /*!<  */
 
#define LPTIM_INTSR_ALL_Msk                    (LPTIM_INTSR_INTF_Msk)
 
/******************  Bit definition for LPTIM_INTCLR register ********************/
#define LPTIM_INTCLR_ICLR_Pos                    (0U)
#define LPTIM_INTCLR_ICLR_Msk                    (0x01U << LPTIM_INTCLR_ICLR_Pos)        /*!<  */
#define LPTIM_INTCLR_ICLR                        LPTIM_INTCLR_ICLR_Msk                   /*!<  */
 
#define LPTIM_INTCLR_ALL_Msk                    (LPTIM_INTCLR_ICLR_Msk)
 
/******************  Bit definition for LPTIM_BGLOAD register ********************/
#define LPTIM_BGLOAD_BGLOAD_Pos                    (0U)
#define LPTIM_BGLOAD_BGLOAD_Msk                    (0xFFFFU << LPTIM_BGLOAD_BGLOAD_Pos)        /*!<  */
#define LPTIM_BGLOAD_BGLOAD                        LPTIM_BGLOAD_BGLOAD_Msk                   /*!<  */
 
#define LPTIM_BGLOAD_ALL_Msk                    (LPTIM_BGLOAD_BGLOAD_Msk)
 
/******************  Bit definition for LPUART_SBUF register ********************/
#define LPUART_SBUF_SBUF_Pos                    (0U)
#define LPUART_SBUF_SBUF_Msk                    (0xFFU << LPUART_SBUF_SBUF_Pos)        /*!<  */
#define LPUART_SBUF_SBUF                        LPUART_SBUF_SBUF_Msk                   /*!<  */
 
#define LPUART_SBUF_ALL_Msk                    (LPUART_SBUF_SBUF_Msk)
 
/******************  Bit definition for LPUART_SCON register ********************/
#define LPUART_SCON_RIEN_Pos                    (0U)
#define LPUART_SCON_RIEN_Msk                    (0x01U << LPUART_SCON_RIEN_Pos)        /*!<  */
#define LPUART_SCON_RIEN                        LPUART_SCON_RIEN_Msk                   /*!<  */
 
#define LPUART_SCON_TIEN_Pos                    (1U)
#define LPUART_SCON_TIEN_Msk                    (0x01U << LPUART_SCON_TIEN_Pos)        /*!<  */
#define LPUART_SCON_TIEN                        LPUART_SCON_TIEN_Msk                   /*!<  */
 
#define LPUART_SCON_RB8_Pos                    (2U)
#define LPUART_SCON_RB8_Msk                    (0x01U << LPUART_SCON_RB8_Pos)        /*!<  */
#define LPUART_SCON_RB8                        LPUART_SCON_RB8_Msk                   /*!<  */
 
#define LPUART_SCON_TB8_Pos                    (3U)
#define LPUART_SCON_TB8_Msk                    (0x01U << LPUART_SCON_TB8_Pos)        /*!<  */
#define LPUART_SCON_TB8                        LPUART_SCON_TB8_Msk                   /*!<  */
 
#define LPUART_SCON_REN_Pos                    (4U)
#define LPUART_SCON_REN_Msk                    (0x01U << LPUART_SCON_REN_Pos)        /*!<  */
#define LPUART_SCON_REN                        LPUART_SCON_REN_Msk                   /*!<  */
 
#define LPUART_SCON_SM2_Pos                    (5U)
#define LPUART_SCON_SM2_Msk                    (0x01U << LPUART_SCON_SM2_Pos)        /*!<  */
#define LPUART_SCON_SM2                        LPUART_SCON_SM2_Msk                   /*!<  */
 
#define LPUART_SCON_SM0_SM1_Pos                    (6U)
#define LPUART_SCON_SM0_SM1_Msk                    (0x3U << LPUART_SCON_SM0_SM1_Pos)        /*!<  */
#define LPUART_SCON_SM0_SM1                        LPUART_SCON_SM0_SM1_Msk                   /*!<  */
 
#define LPUART_SCON_TEEN_Pos                    (8U)
#define LPUART_SCON_TEEN_Msk                    (0x01U << LPUART_SCON_TEEN_Pos)        /*!<  */
#define LPUART_SCON_TEEN                        LPUART_SCON_TEEN_Msk                   /*!<  */
 
#define LPUART_SCON_DBAUD_Pos                    (9U)
#define LPUART_SCON_DBAUD_Msk                    (0x01U << LPUART_SCON_DBAUD_Pos)        /*!<  */
#define LPUART_SCON_DBAUD                        LPUART_SCON_DBAUD_Msk                   /*!<  */
 
#define LPUART_SCON_LPMODE_Pos                    (10U)
#define LPUART_SCON_LPMODE_Msk                    (0x01U << LPUART_SCON_LPMODE_Pos)        /*!<  */
#define LPUART_SCON_LPMODE                        LPUART_SCON_LPMODE_Msk                   /*!<  */
 
#define LPUART_SCON_SCLKSEL_Pos                    (11U)
#define LPUART_SCON_SCLKSEL_Msk                    (0x3U << LPUART_SCON_SCLKSEL_Pos)        /*!<  */
#define LPUART_SCON_SCLKSEL                        LPUART_SCON_SCLKSEL_Msk                   /*!<  */
 
#define LPUART_SCON_PRSC_Pos                    (13U)
#define LPUART_SCON_PRSC_Msk                    (0x7U << LPUART_SCON_PRSC_Pos)        /*!<  */
#define LPUART_SCON_PRSC                        LPUART_SCON_PRSC_Msk                   /*!<  */
 
#define LPUART_SCON_EN_Pos                    (16U)
#define LPUART_SCON_EN_Msk                    (0x01U << LPUART_SCON_EN_Pos)        /*!<  */
#define LPUART_SCON_EN                        LPUART_SCON_EN_Msk                   /*!<  */
 
#define LPUART_SCON_FEEN_Pos                    (17U)
#define LPUART_SCON_FEEN_Msk                    (0x01U << LPUART_SCON_FEEN_Pos)        /*!<  */
#define LPUART_SCON_FEEN                        LPUART_SCON_FEEN_Msk                   /*!<  */
 
#define LPUART_SCON_ALL_Msk                    (LPUART_SCON_FEEN_Msk | LPUART_SCON_EN_Msk | LPUART_SCON_PRSC_Msk | LPUART_SCON_SCLKSEL_Msk | LPUART_SCON_LPMODE_Msk | LPUART_SCON_DBAUD_Msk | LPUART_SCON_TEEN_Msk | LPUART_SCON_SM0_SM1_Msk | LPUART_SCON_SM2_Msk | LPUART_SCON_REN_Msk | LPUART_SCON_TB8_Msk | LPUART_SCON_RB8_Msk | LPUART_SCON_TIEN_Msk | LPUART_SCON_RIEN_Msk)
 
/******************  Bit definition for LPUART_SADDR register ********************/
#define LPUART_SADDR_SADDR_Pos                    (0U)
#define LPUART_SADDR_SADDR_Msk                    (0xFFU << LPUART_SADDR_SADDR_Pos)        /*!<  */
#define LPUART_SADDR_SADDR                        LPUART_SADDR_SADDR_Msk                   /*!<  */
 
#define LPUART_SADDR_ALL_Msk                    (LPUART_SADDR_SADDR_Msk)
 
/******************  Bit definition for LPUART_SADEN register ********************/
#define LPUART_SADEN_SADEN_Pos                    (0U)
#define LPUART_SADEN_SADEN_Msk                    (0xFFU << LPUART_SADEN_SADEN_Pos)        /*!<  */
#define LPUART_SADEN_SADEN                        LPUART_SADEN_SADEN_Msk                   /*!<  */
 
#define LPUART_SADEN_ALL_Msk                    (LPUART_SADEN_SADEN_Msk)
 
/******************  Bit definition for LPUART_INTSR register ********************/
#define LPUART_INTSR_RI_Pos                    (0U)
#define LPUART_INTSR_RI_Msk                    (0x01U << LPUART_INTSR_RI_Pos)        /*!<  */
#define LPUART_INTSR_RI                        LPUART_INTSR_RI_Msk                   /*!<  */
 
#define LPUART_INTSR_TI_Pos                    (1U)
#define LPUART_INTSR_TI_Msk                    (0x01U << LPUART_INTSR_TI_Pos)        /*!<  */
#define LPUART_INTSR_TI                        LPUART_INTSR_TI_Msk                   /*!<  */
 
#define LPUART_INTSR_FE_Pos                    (2U)
#define LPUART_INTSR_FE_Msk                    (0x01U << LPUART_INTSR_FE_Pos)        /*!<  */
#define LPUART_INTSR_FE                        LPUART_INTSR_FE_Msk                   /*!<  */
 
#define LPUART_INTSR_TE_Pos                    (3U)
#define LPUART_INTSR_TE_Msk                    (0x01U << LPUART_INTSR_TE_Pos)        /*!<  */
#define LPUART_INTSR_TE                        LPUART_INTSR_TE_Msk                   /*!<  */
 
#define LPUART_INTSR_ALL_Msk                    (LPUART_INTSR_TE_Msk | LPUART_INTSR_FE_Msk | LPUART_INTSR_TI_Msk | LPUART_INTSR_RI_Msk)
 
/******************  Bit definition for LPUART_INTCLR register ********************/
#define LPUART_INTCLR_RICLR_Pos                    (0U)
#define LPUART_INTCLR_RICLR_Msk                    (0x01U << LPUART_INTCLR_RICLR_Pos)        /*!<  */
#define LPUART_INTCLR_RICLR                        LPUART_INTCLR_RICLR_Msk                   /*!<  */
 
#define LPUART_INTCLR_TICLR_Pos                    (1U)
#define LPUART_INTCLR_TICLR_Msk                    (0x01U << LPUART_INTCLR_TICLR_Pos)        /*!<  */
#define LPUART_INTCLR_TICLR                        LPUART_INTCLR_TICLR_Msk                   /*!<  */
 
#define LPUART_INTCLR_FECLR_Pos                    (2U)
#define LPUART_INTCLR_FECLR_Msk                    (0x01U << LPUART_INTCLR_FECLR_Pos)        /*!<  */
#define LPUART_INTCLR_FECLR                        LPUART_INTCLR_FECLR_Msk                   /*!<  */
 
#define LPUART_INTCLR_TECLR_Pos                    (3U)
#define LPUART_INTCLR_TECLR_Msk                    (0x01U << LPUART_INTCLR_TECLR_Pos)        /*!<  */
#define LPUART_INTCLR_TECLR                        LPUART_INTCLR_TECLR_Msk                   /*!<  */
 
#define LPUART_INTCLR_ALL_Msk                    (LPUART_INTCLR_TECLR_Msk | LPUART_INTCLR_FECLR_Msk | LPUART_INTCLR_TICLR_Msk | LPUART_INTCLR_RICLR_Msk)
 
/******************  Bit definition for LPUART_BAUDCR register ********************/
#define LPUART_BAUDCR_BRG_Pos                    (0U)
#define LPUART_BAUDCR_BRG_Msk                    (0xFFFFU << LPUART_BAUDCR_BRG_Pos)        /*!<  */
#define LPUART_BAUDCR_BRG                        LPUART_BAUDCR_BRG_Msk                   /*!<  */
 
#define LPUART_BAUDCR_SELF_BRG_Pos                    (16U)
#define LPUART_BAUDCR_SELF_BRG_Msk                    (0x01U << LPUART_BAUDCR_SELF_BRG_Pos)        /*!<  */
#define LPUART_BAUDCR_SELF_BRG                        LPUART_BAUDCR_SELF_BRG_Msk                   /*!<  */
 
#define LPUART_BAUDCR_ALL_Msk                    (LPUART_BAUDCR_SELF_BRG_Msk | LPUART_BAUDCR_BRG_Msk)
 
/******************  Bit definition for BEEP_CSR register ********************/
#define BEEP_CSR_BEEPDIV_Pos                    (0U)
#define BEEP_CSR_BEEPDIV_Msk                    (0xFFFU << BEEP_CSR_BEEPDIV_Pos)        /*!<  */
#define BEEP_CSR_BEEPDIV                        BEEP_CSR_BEEPDIV_Msk                   /*!<  */
 
#define BEEP_CSR_BEEPSEL_Pos                    (16U)
#define BEEP_CSR_BEEPSEL_Msk                    (0x3U << BEEP_CSR_BEEPSEL_Pos)        /*!<  */
#define BEEP_CSR_BEEPSEL                        BEEP_CSR_BEEPSEL_Msk                   /*!<  */
 
#define BEEP_CSR_BEEPEN_Pos                    (18U)
#define BEEP_CSR_BEEPEN_Msk                    (0x01U << BEEP_CSR_BEEPEN_Pos)        /*!<  */
#define BEEP_CSR_BEEPEN                        BEEP_CSR_BEEPEN_Msk                   /*!<  */
 
#define BEEP_CSR_CLKSEL_Pos                    (20U)
#define BEEP_CSR_CLKSEL_Msk                    (0x3U << BEEP_CSR_CLKSEL_Pos)        /*!<  */
#define BEEP_CSR_CLKSEL                        BEEP_CSR_CLKSEL_Msk                   /*!<  */
 
#define BEEP_CSR_ALL_Msk                    (BEEP_CSR_CLKSEL_Msk | BEEP_CSR_BEEPEN_Msk | BEEP_CSR_BEEPSEL_Msk | BEEP_CSR_BEEPDIV_Msk)
 
/******************  Bit definition for VCCR register ********************/
#define VCCR_VREF_RSEL_Pos                    (0U)
#define VCCR_VREF_RSEL_Msk                    (0x7FU << VCCR_VREF_RSEL_Pos)        /*!<  */
#define VCCR_VREF_RSEL                        VCCR_VREF_RSEL_Msk                   /*!<  */
 
#define VCCR_VREFSEL_Pos                    (7U)
#define VCCR_VREFSEL_Msk                    (0x01U << VCCR_VREFSEL_Pos)        /*!<  */
#define VCCR_VREFSEL                        VCCR_VREFSEL_Msk                   /*!<  */
 
#define VCCR_VC0_HYS_SEL_Pos                    (8U)
#define VCCR_VC0_HYS_SEL_Msk                    (0x3U << VCCR_VC0_HYS_SEL_Pos)        /*!<  */
#define VCCR_VC0_HYS_SEL                        VCCR_VC0_HYS_SEL_Msk                   /*!<  */
 
#define VCCR_VC0_P_SEL_Pos                    (10U)
#define VCCR_VC0_P_SEL_Msk                    (0xFU << VCCR_VC0_P_SEL_Pos)        /*!<  */
#define VCCR_VC0_P_SEL                        VCCR_VC0_P_SEL_Msk                   /*!<  */
 
#define VCCR_VC0_N_SEL_Pos                    (14U)
#define VCCR_VC0_N_SEL_Msk                    (0xFU << VCCR_VC0_N_SEL_Pos)        /*!<  */
#define VCCR_VC0_N_SEL                        VCCR_VC0_N_SEL_Msk                   /*!<  */
 
#define VCCR_VC1_HYS_SEL_Pos                    (18U)
#define VCCR_VC1_HYS_SEL_Msk                    (0x3U << VCCR_VC1_HYS_SEL_Pos)        /*!<  */
#define VCCR_VC1_HYS_SEL                        VCCR_VC1_HYS_SEL_Msk                   /*!<  */
 
#define VCCR_VC1_P_SEL_Pos                    (20U)
#define VCCR_VC1_P_SEL_Msk                    (0xFU << VCCR_VC1_P_SEL_Pos)        /*!<  */
#define VCCR_VC1_P_SEL                        VCCR_VC1_P_SEL_Msk                   /*!<  */
 
#define VCCR_VC1_N_SEL_Pos                    (24U)
#define VCCR_VC1_N_SEL_Msk                    (0xFU << VCCR_VC1_N_SEL_Pos)        /*!<  */
#define VCCR_VC1_N_SEL                        VCCR_VC1_N_SEL_Msk                   /*!<  */
 
#define VCCR_ALL_Msk                    (VCCR_VC1_N_SEL_Msk | VCCR_VC1_P_SEL_Msk | VCCR_VC1_HYS_SEL_Msk | VCCR_VC0_N_SEL_Msk | VCCR_VC0_P_SEL_Msk | VCCR_VC0_HYS_SEL_Msk | VCCR_VREFSEL_Msk | VCCR_VREF_RSEL_Msk)
 
/******************  Bit definition for VCSR register ********************/
#define VCSR_VC0INT_Pos                    (0U)
#define VCSR_VC0INT_Msk                    (0x01U << VCSR_VC0INT_Pos)        /*!<  */
#define VCSR_VC0INT                        VCSR_VC0INT_Msk                   /*!<  */
 
#define VCSR_VC1INT_Pos                    (1U)
#define VCSR_VC1INT_Msk                    (0x01U << VCSR_VC1INT_Pos)        /*!<  */
#define VCSR_VC1INT                        VCSR_VC1INT_Msk                   /*!<  */
 
#define VCSR_VC0OUT_Pos                    (2U)
#define VCSR_VC0OUT_Msk                    (0x01U << VCSR_VC0OUT_Pos)        /*!<  */
#define VCSR_VC0OUT                        VCSR_VC0OUT_Msk                   /*!<  */
 
#define VCSR_VC1OUT_Pos                    (3U)
#define VCSR_VC1OUT_Msk                    (0x01U << VCSR_VC1OUT_Pos)        /*!<  */
#define VCSR_VC1OUT                        VCSR_VC1OUT_Msk                   /*!<  */
 
#define VCSR_ALL_Msk                    (VCSR_VC1OUT_Msk | VCSR_VC0OUT_Msk | VCSR_VC1INT_Msk | VCSR_VC0INT_Msk)
 
/******************  Bit definition for VC0CR register ********************/
#define VC0CR_VC0_EN_Pos                    (5U)
#define VC0CR_VC0_EN_Msk                    (0x01U << VC0CR_VC0_EN_Pos)        /*!<  */
#define VC0CR_VC0_EN                        VC0CR_VC0_EN_Msk                   /*!<  */
 
#define VC0CR_FILTER_EN_Pos                    (7U)
#define VC0CR_FILTER_EN_Msk                    (0x01U << VC0CR_FILTER_EN_Pos)        /*!<  */
#define VC0CR_FILTER_EN                        VC0CR_FILTER_EN_Msk                   /*!<  */
 
#define VC0CR_CLK_SEL_Pos                    (8U)
#define VC0CR_CLK_SEL_Msk                    (0x3U << VC0CR_CLK_SEL_Pos)        /*!<  */
#define VC0CR_CLK_SEL                        VC0CR_CLK_SEL_Msk                   /*!<  */
 
#define VC0CR_MIX_EN_Pos                    (10U)
#define VC0CR_MIX_EN_Msk                    (0x01U << VC0CR_MIX_EN_Pos)        /*!<  */
#define VC0CR_MIX_EN                        VC0CR_MIX_EN_Msk                   /*!<  */
 
#define VC0CR_INV_EN_Pos                    (11U)
#define VC0CR_INV_EN_Msk                    (0x01U << VC0CR_INV_EN_Pos)        /*!<  */
#define VC0CR_INV_EN                        VC0CR_INV_EN_Msk                   /*!<  */
 
#define VC0CR_DET_FALL_EN_Pos                    (12U)
#define VC0CR_DET_FALL_EN_Msk                    (0x01U << VC0CR_DET_FALL_EN_Pos)        /*!<  */
#define VC0CR_DET_FALL_EN                        VC0CR_DET_FALL_EN_Msk                   /*!<  */
 
#define VC0CR_DET_RISE_EN_Pos                    (13U)
#define VC0CR_DET_RISE_EN_Msk                    (0x01U << VC0CR_DET_RISE_EN_Pos)        /*!<  */
#define VC0CR_DET_RISE_EN                        VC0CR_DET_RISE_EN_Msk                   /*!<  */
 
#define VC0CR_DET_HI_EN_Pos                    (14U)
#define VC0CR_DET_HI_EN_Msk                    (0x01U << VC0CR_DET_HI_EN_Pos)        /*!<  */
#define VC0CR_DET_HI_EN                        VC0CR_DET_HI_EN_Msk                   /*!<  */
 
#define VC0CR_INT_EN_Pos                    (15U)
#define VC0CR_INT_EN_Msk                    (0x01U << VC0CR_INT_EN_Pos)        /*!<  */
#define VC0CR_INT_EN                        VC0CR_INT_EN_Msk                   /*!<  */
 
#define VC0CR_FILTER_NUM_Pos                    (16U)
#define VC0CR_FILTER_NUM_Msk                    (0xFFFFU << VC0CR_FILTER_NUM_Pos)        /*!<  */
#define VC0CR_FILTER_NUM                        VC0CR_FILTER_NUM_Msk                   /*!<  */
 
#define VC0CR_ALL_Msk                    (VC0CR_FILTER_NUM_Msk | VC0CR_INT_EN_Msk | VC0CR_DET_HI_EN_Msk | VC0CR_DET_RISE_EN_Msk | VC0CR_DET_FALL_EN_Msk | VC0CR_INV_EN_Msk | VC0CR_MIX_EN_Msk | VC0CR_CLK_SEL_Msk | VC0CR_FILTER_EN_Msk | VC0CR_VC0_EN_Msk)
 
/******************  Bit definition for VC1CR register ********************/
#define VC1CR_VC1_EN_Pos                    (5U)
#define VC1CR_VC1_EN_Msk                    (0x01U << VC1CR_VC1_EN_Pos)        /*!<  */
#define VC1CR_VC1_EN                        VC1CR_VC1_EN_Msk                   /*!<  */
 
#define VC1CR_FILTER_EN_Pos                    (7U)
#define VC1CR_FILTER_EN_Msk                    (0x01U << VC1CR_FILTER_EN_Pos)        /*!<  */
#define VC1CR_FILTER_EN                        VC1CR_FILTER_EN_Msk                   /*!<  */
 
#define VC1CR_CLK_SEL_Pos                    (8U)
#define VC1CR_CLK_SEL_Msk                    (0x3U << VC1CR_CLK_SEL_Pos)        /*!<  */
#define VC1CR_CLK_SEL                        VC1CR_CLK_SEL_Msk                   /*!<  */
 
#define VC1CR_MIX_EN_Pos                    (10U)
#define VC1CR_MIX_EN_Msk                    (0x01U << VC1CR_MIX_EN_Pos)        /*!<  */
#define VC1CR_MIX_EN                        VC1CR_MIX_EN_Msk                   /*!<  */
 
#define VC1CR_INV_EN_Pos                    (11U)
#define VC1CR_INV_EN_Msk                    (0x01U << VC1CR_INV_EN_Pos)        /*!<  */
#define VC1CR_INV_EN                        VC1CR_INV_EN_Msk                   /*!<  */
 
#define VC1CR_DET_FALL_EN_Pos                    (12U)
#define VC1CR_DET_FALL_EN_Msk                    (0x01U << VC1CR_DET_FALL_EN_Pos)        /*!<  */
#define VC1CR_DET_FALL_EN                        VC1CR_DET_FALL_EN_Msk                   /*!<  */
 
#define VC1CR_DET_RISE_EN_Pos                    (13U)
#define VC1CR_DET_RISE_EN_Msk                    (0x01U << VC1CR_DET_RISE_EN_Pos)        /*!<  */
#define VC1CR_DET_RISE_EN                        VC1CR_DET_RISE_EN_Msk                   /*!<  */
 
#define VC1CR_DET_HI_EN_Pos                    (14U)
#define VC1CR_DET_HI_EN_Msk                    (0x01U << VC1CR_DET_HI_EN_Pos)        /*!<  */
#define VC1CR_DET_HI_EN                        VC1CR_DET_HI_EN_Msk                   /*!<  */
 
#define VC1CR_INT_EN_Pos                    (15U)
#define VC1CR_INT_EN_Msk                    (0x01U << VC1CR_INT_EN_Pos)        /*!<  */
#define VC1CR_INT_EN                        VC1CR_INT_EN_Msk                   /*!<  */
 
#define VC1CR_FILTER_NUM_Pos                    (16U)
#define VC1CR_FILTER_NUM_Msk                    (0xFFFFU << VC1CR_FILTER_NUM_Pos)        /*!<  */
#define VC1CR_FILTER_NUM                        VC1CR_FILTER_NUM_Msk                   /*!<  */
 
#define VC1CR_ALL_Msk                    (VC1CR_FILTER_NUM_Msk | VC1CR_INT_EN_Msk | VC1CR_DET_HI_EN_Msk | VC1CR_DET_RISE_EN_Msk | VC1CR_DET_FALL_EN_Msk | VC1CR_INV_EN_Msk | VC1CR_MIX_EN_Msk | VC1CR_CLK_SEL_Msk | VC1CR_FILTER_EN_Msk | VC1CR_VC1_EN_Msk)
 
/******************  Bit definition for LVD_CR register ********************/
#define LVD_CR_DIV_SEL_Pos                    (0U)
#define LVD_CR_DIV_SEL_Msk                    (0xFU << LVD_CR_DIV_SEL_Pos)        /*!<  */
#define LVD_CR_DIV_SEL                        LVD_CR_DIV_SEL_Msk                   /*!<  */
 
#define LVD_CR_LVDEN_Pos                    (5U)
#define LVD_CR_LVDEN_Msk                    (0x01U << LVD_CR_LVDEN_Pos)        /*!<  */
#define LVD_CR_LVDEN                        LVD_CR_LVDEN_Msk                   /*!<  */
 
#define LVD_CR_ACT_Pos                    (6U)
#define LVD_CR_ACT_Msk                    (0x01U << LVD_CR_ACT_Pos)        /*!<  */
#define LVD_CR_ACT                        LVD_CR_ACT_Msk                   /*!<  */
 
#define LVD_CR_FLTEN_Pos                    (7U)
#define LVD_CR_FLTEN_Msk                    (0x01U << LVD_CR_FLTEN_Pos)        /*!<  */
#define LVD_CR_FLTEN                        LVD_CR_FLTEN_Msk                   /*!<  */
 
#define LVD_CR_FLTCLK_SEL_Pos                    (8U)
#define LVD_CR_FLTCLK_SEL_Msk                    (0x3U << LVD_CR_FLTCLK_SEL_Pos)        /*!<  */
#define LVD_CR_FLTCLK_SEL                        LVD_CR_FLTCLK_SEL_Msk                   /*!<  */
 
#define LVD_CR_FALLINTEN_Pos                    (12U)
#define LVD_CR_FALLINTEN_Msk                    (0x01U << LVD_CR_FALLINTEN_Pos)        /*!<  */
#define LVD_CR_FALLINTEN                        LVD_CR_FALLINTEN_Msk                   /*!<  */
 
#define LVD_CR_RISEINTEN_Pos                    (13U)
#define LVD_CR_RISEINTEN_Msk                    (0x01U << LVD_CR_RISEINTEN_Pos)        /*!<  */
#define LVD_CR_RISEINTEN                        LVD_CR_RISEINTEN_Msk                   /*!<  */
 
#define LVD_CR_HIGHINTEN_Pos                    (14U)
#define LVD_CR_HIGHINTEN_Msk                    (0x01U << LVD_CR_HIGHINTEN_Pos)        /*!<  */
#define LVD_CR_HIGHINTEN                        LVD_CR_HIGHINTEN_Msk                   /*!<  */
 
#define LVD_CR_INT_EN_Pos                    (15U)
#define LVD_CR_INT_EN_Msk                    (0x01U << LVD_CR_INT_EN_Pos)        /*!<  */
#define LVD_CR_INT_EN                        LVD_CR_INT_EN_Msk                   /*!<  */
 
#define LVD_CR_FLT_NUM_Pos                    (16U)
#define LVD_CR_FLT_NUM_Msk                    (0xFFFFU << LVD_CR_FLT_NUM_Pos)        /*!<  */
#define LVD_CR_FLT_NUM                        LVD_CR_FLT_NUM_Msk                   /*!<  */
 
#define LVD_CR_ALL_Msk                    (LVD_CR_FLT_NUM_Msk | LVD_CR_INT_EN_Msk | LVD_CR_HIGHINTEN_Msk | LVD_CR_RISEINTEN_Msk | LVD_CR_FALLINTEN_Msk | LVD_CR_FLTCLK_SEL_Msk | LVD_CR_FLTEN_Msk | LVD_CR_ACT_Msk | LVD_CR_LVDEN_Msk | LVD_CR_DIV_SEL_Msk)
 
/******************  Bit definition for LVD_SR register ********************/
#define LVD_SR_INTF_Pos                    (0U)
#define LVD_SR_INTF_Msk                    (0x01U << LVD_SR_INTF_Pos)        /*!<  */
#define LVD_SR_INTF                        LVD_SR_INTF_Msk                   /*!<  */
 
#define LVD_SR_LVDOUT_Pos                    (1U)
#define LVD_SR_LVDOUT_Msk                    (0x01U << LVD_SR_LVDOUT_Pos)        /*!<  */
#define LVD_SR_LVDOUT                        LVD_SR_LVDOUT_Msk                   /*!<  */
 
#define LVD_SR_ALL_Msk                    (LVD_SR_LVDOUT_Msk | LVD_SR_INTF_Msk)
 
/******************  Bit definition for TRNG_CR register ********************/
#define TRNG_CR_TRNGEN_Pos                    (2U)
#define TRNG_CR_TRNGEN_Msk                    (0x01U << TRNG_CR_TRNGEN_Pos)        /*!<  */
#define TRNG_CR_TRNGEN                        TRNG_CR_TRNGEN_Msk                   /*!<  */
 
#define TRNG_CR_IE_Pos                    (3U)
#define TRNG_CR_IE_Msk                    (0x01U << TRNG_CR_IE_Pos)        /*!<  */
#define TRNG_CR_IE                        TRNG_CR_IE_Msk                   /*!<  */
 
#define TRNG_CR_CED_Pos                    (5U)
#define TRNG_CR_CED_Msk                    (0x01U << TRNG_CR_CED_Pos)        /*!<  */
#define TRNG_CR_CED                        TRNG_CR_CED_Msk                   /*!<  */
 
#define TRNG_CR_ALL_Msk                    (TRNG_CR_CED_Msk | TRNG_CR_IE_Msk | TRNG_CR_RNGEN_Msk)
 
/******************  Bit definition for TRNG_SR register ********************/
#define TRNG_SR_DRDY_Pos                    (0U)
#define TRNG_SR_DRDY_Msk                    (0x01U << TRNG_SR_DRDY_Pos)        /*!<  */
#define TRNG_SR_DRDY                        TRNG_SR_DRDY_Msk                   /*!<  */
 
#define TRNG_SR_CECS_Pos                    (1U)
#define TRNG_SR_CECS_Msk                    (0x01U << TRNG_SR_CECS_Pos)        /*!<  */
#define TRNG_SR_CECS                        TRNG_SR_CECS_Msk                   /*!<  */
 
#define TRNG_SR_SECS_Pos                    (2U)
#define TRNG_SR_SECS_Msk                    (0x01U << TRNG_SR_SECS_Pos)        /*!<  */
#define TRNG_SR_SECS                        TRNG_SR_SECS_Msk                   /*!<  */
 
#define TRNG_SR_CEIS_Pos                    (5U)
#define TRNG_SR_CEIS_Msk                    (0x01U << TRNG_SR_CEIS_Pos)        /*!<  */
#define TRNG_SR_CEIS                        TRNG_SR_CEIS_Msk                   /*!<  */
 
#define TRNG_SR_SEIS_Pos                    (6U)
#define TRNG_SR_SEIS_Msk                    (0x01U << TRNG_SR_SEIS_Pos)        /*!<  */
#define TRNG_SR_SEIS                        TRNG_SR_SEIS_Msk                   /*!<  */
 
#define TRNG_SR_ALL_Msk                    (TRNG_SR_SEIS_Msk | TRNG_SR_CEIS_Msk | TRNG_SR_SECS_Msk | TRNG_SR_CECS_Msk | TRNG_SR_DRDY_Msk)
 
/******************  Bit definition for TRNG_DR register ********************/
#define TRNG_DR_TRNDATA_Pos                    (0U)
#define TRNG_DR_TRNDATA_Msk                    (0xFFFFFFFFU << TRNG_DR_TRNDATA_Pos)        /*!<  */
#define TRNG_DR_TRNDATA                        TRNG_DR_TRNDATA_Msk                   /*!<  */
 
#define TRNG_DR_ALL_Msk                    (TRNG_DR_TRNDATA_Msk)
 
/******************  Bit definition for AES_CR register ********************/
#define AES_CR_EN_Pos                    (0U)
#define AES_CR_EN_Msk                    (0x01U << AES_CR_EN_Pos)        /*!<  */
#define AES_CR_EN                        AES_CR_EN_Msk                   /*!<  */
 
#define AES_CR_DATATYPE_Pos                    (1U)
#define AES_CR_DATATYPE_Msk                    (0x3U << AES_CR_DATATYPE_Pos)        /*!<  */
#define AES_CR_DATATYPE                        AES_CR_DATATYPE_Msk                   /*!<  */
 
#define AES_CR_MODE_Pos                    (3U)
#define AES_CR_MODE_Msk                    (0x3U << AES_CR_MODE_Pos)        /*!<  */
#define AES_CR_MODE                        AES_CR_MODE_Msk                   /*!<  */
 
#define AES_CR_CHMOD_Pos                    (5U)
#define AES_CR_CHMOD_Msk                    (0x3U << AES_CR_CHMOD_Pos)        /*!<  */
#define AES_CR_CHMOD                        AES_CR_CHMOD_Msk                   /*!<  */
 
#define AES_CR_CCFC_Pos                    (7U)
#define AES_CR_CCFC_Msk                    (0x01U << AES_CR_CCFC_Pos)        /*!<  */
#define AES_CR_CCFC                        AES_CR_CCFC_Msk                   /*!<  */
 
#define AES_CR_ERRC_Pos                    (8U)
#define AES_CR_ERRC_Msk                    (0x01U << AES_CR_ERRC_Pos)        /*!<  */
#define AES_CR_ERRC                        AES_CR_ERRC_Msk                   /*!<  */
 
#define AES_CR_CCFIE_Pos                    (9U)
#define AES_CR_CCFIE_Msk                    (0x01U << AES_CR_CCFIE_Pos)        /*!<  */
#define AES_CR_CCFIE                        AES_CR_CCFIE_Msk                   /*!<  */
 
#define AES_CR_ERRIE_Pos                    (10U)
#define AES_CR_ERRIE_Msk                    (0x01U << AES_CR_ERRIE_Pos)        /*!<  */
#define AES_CR_ERRIE                        AES_CR_ERRIE_Msk                   /*!<  */
 
#define AES_CR_DMAINEN_Pos                    (11U)
#define AES_CR_DMAINEN_Msk                    (0x01U << AES_CR_DMAINEN_Pos)        /*!<  */
#define AES_CR_DMAINEN                        AES_CR_DMAINEN_Msk                   /*!<  */
 
#define AES_CR_DMAOUTEN_Pos                    (12U)
#define AES_CR_DMAOUTEN_Msk                    (0x01U << AES_CR_DMAOUTEN_Pos)        /*!<  */
#define AES_CR_DMAOUTEN                        AES_CR_DMAOUTEN_Msk                   /*!<  */
 
#define AES_CR_ALL_Msk                    (AES_CR_DMAOUTEN_Msk | AES_CR_DMAINEN_Msk | AES_CR_ERRIE_Msk | AES_CR_CCFIE_Msk | AES_CR_ERRC_Msk | AES_CR_CCFC_Msk | AES_CR_CHMOD_Msk | AES_CR_MODE_Msk | AES_CR_DATATYPE_Msk | AES_CR_EN_Msk)
 
/******************  Bit definition for AES_SR register ********************/
#define AES_SR_CCF_Pos                    (0U)
#define AES_SR_CCF_Msk                    (0x01U << AES_SR_CCF_Pos)        /*!<  */
#define AES_SR_CCF                        AES_SR_CCF_Msk                   /*!<  */
 
#define AES_SR_RDERR_Pos                    (1U)
#define AES_SR_RDERR_Msk                    (0x01U << AES_SR_RDERR_Pos)        /*!<  */
#define AES_SR_RDERR                        AES_SR_RDERR_Msk                   /*!<  */
 
#define AES_SR_WRERR_Pos                    (2U)
#define AES_SR_WRERR_Msk                    (0x01U << AES_SR_WRERR_Pos)        /*!<  */
#define AES_SR_WRERR                        AES_SR_WRERR_Msk                   /*!<  */
 
#define AES_SR_ALL_Msk                    (AES_SR_WRERR_Msk | AES_SR_RDERR_Msk | AES_SR_CCF_Msk)
 
/******************  Bit definition for AES_DINR register ********************/
#define AES_DINR_DINR_Pos                    (0U)
#define AES_DINR_DINR_Msk                    (0xFFFFFFFFU << AES_DINR_DINR_Pos)        /*!<  */
#define AES_DINR_DINR                        AES_DINR_DINR_Msk                   /*!<  */
 
#define AES_DINR_ALL_Msk                    (AES_DINR_DINR_Msk)
 
/******************  Bit definition for AES_OUTR register ********************/
#define AES_OUTR_DOUTR_Pos                    (0U)
#define AES_OUTR_DOUTR_Msk                    (0xFFFFFFFFU << AES_OUTR_DOUTR_Pos)        /*!<  */
#define AES_OUTR_DOUTR                        AES_OUTR_DOUTR_Msk                   /*!<  */
 
#define AES_OUTR_ALL_Msk                    (AES_OUTR_DOUTR_Msk)
 
/******************  Bit definition for AES_KEYR0 register ********************/
#define AES_KEYR0_KEYR0_Pos                    (0U)
#define AES_KEYR0_KEYR0_Msk                    (0xFFFFFFFFU << AES_KEYR0_KEYR0_Pos)        /*!<  */
#define AES_KEYR0_KEYR0                        AES_KEYR0_KEYR0_Msk                   /*!<  */
 
#define AES_KEYR0_ALL_Msk                    (AES_KEYR0_KEYR0_Msk)
 
/******************  Bit definition for AES_KEYR1 register ********************/
#define AES_KEYR1_KEYR1_Pos                    (0U)
#define AES_KEYR1_KEYR1_Msk                    (0xFFFFFFFFU << AES_KEYR1_KEYR1_Pos)        /*!<  */
#define AES_KEYR1_KEYR1                        AES_KEYR1_KEYR1_Msk                   /*!<  */
 
#define AES_KEYR1_ALL_Msk                    (AES_KEYR1_KEYR1_Msk)
 
/******************  Bit definition for AES_KEYR2 register ********************/
#define AES_KEYR2_KEYR2_Pos                    (0U)
#define AES_KEYR2_KEYR2_Msk                    (0xFFFFFFFFU << AES_KEYR2_KEYR2_Pos)        /*!<  */
#define AES_KEYR2_KEYR2                        AES_KEYR2_KEYR2_Msk                   /*!<  */
 
#define AES_KEYR2_ALL_Msk                    (AES_KEYR2_KEYR2_Msk)
 
/******************  Bit definition for AES_KEYR3 register ********************/
#define AES_KEYR3_KEYR3_Pos                    (0U)
#define AES_KEYR3_KEYR3_Msk                    (0xFFFFFFFFU << AES_KEYR3_KEYR3_Pos)        /*!<  */
#define AES_KEYR3_KEYR3                        AES_KEYR3_KEYR3_Msk                   /*!<  */
 
#define AES_KEYR3_ALL_Msk                    (AES_KEYR3_KEYR3_Msk)
 
/******************  Bit definition for AES_IVR0 register ********************/
#define AES_IVR0_IVR0_Pos                    (0U)
#define AES_IVR0_IVR0_Msk                    (0xFFFFFFFFU << AES_IVR0_IVR0_Pos)        /*!<  */
#define AES_IVR0_IVR0                        AES_IVR0_IVR0_Msk                   /*!<  */
 
#define AES_IVR0_ALL_Msk                    (AES_IVR0_IVR0_Msk)
 
/******************  Bit definition for AES_IVR1 register ********************/
#define AES_IVR1_IVR0_Pos                    (0U)
#define AES_IVR1_IVR0_Msk                    (0xFFFFFFFFU << AES_IVR1_IVR0_Pos)        /*!<  */
#define AES_IVR1_IVR0                        AES_IVR1_IVR0_Msk                   /*!<  */
 
#define AES_IVR1_ALL_Msk                    (AES_IVR1_IVR0_Msk)
 
/******************  Bit definition for AES_IVR2 register ********************/
#define AES_IVR2_IVR2_Pos                    (0U)
#define AES_IVR2_IVR2_Msk                    (0xFFFFFFFFU << AES_IVR2_IVR2_Pos)        /*!<  */
#define AES_IVR2_IVR2                        AES_IVR2_IVR2_Msk                   /*!<  */
 
#define AES_IVR2_ALL_Msk                    (AES_IVR2_IVR2_Msk)
 
/******************  Bit definition for AES_IVR3 register ********************/
#define AES_IVR3_IVR3_Pos                    (0U)
#define AES_IVR3_IVR3_Msk                    (0xFFFFFFFFU << AES_IVR3_IVR3_Pos)        /*!<  */
#define AES_IVR3_IVR3                        AES_IVR3_IVR3_Msk                   /*!<  */
 
#define AES_IVR3_ALL_Msk                    (AES_IVR3_IVR3_Msk)
 
/******************  Bit definition for SRAM_CR register ********************/
#define SRAM_CR_INTEN_Pos                    (0U)
#define SRAM_CR_INTEN_Msk                    (0x01U << SRAM_CR_INTEN_Pos)        /*!<  */
#define SRAM_CR_INTEN                        SRAM_CR_INTEN_Msk                   /*!<  */
 
#define SRAM_CR_ALL_Msk                    (SRAM_CR_INTEN_Msk)
 
/******************  Bit definition for SRAM_SR register ********************/
#define SRAM_SR_ERR_Pos                    (0U)
#define SRAM_SR_ERR_Msk                    (0x01U << SRAM_SR_ERR_Pos)        /*!<  */
#define SRAM_SR_ERR                        SRAM_SR_ERR_Msk                   /*!<  */
 
#define SRAM_SR_ERRADDR_Pos                    (16U)
#define SRAM_SR_ERRADDR_Msk                    (0x7FFFU << SRAM_SR_ERRADDR_Pos)        /*!<  */
#define SRAM_SR_ERRADDR                        SRAM_SR_ERRADDR_Msk                   /*!<  */
 
#define SRAM_SR_ALL_Msk                    (SRAM_SR_ERRADDR_Msk | SRAM_SR_ERR_Msk)
 
/******************  Bit definition for AWK_CR register ********************/
#define AWK_CR_DIVSEL_Pos                    (0U)
#define AWK_CR_DIVSEL_Msk                    (0xFU << AWK_CR_DIVSEL_Pos)        /*!<  */
#define AWK_CR_DIVSEL                        AWK_CR_DIVSEL_Msk                   /*!<  */
 
#define AWK_CR_AWKEN_Pos                    (4U)
#define AWK_CR_AWKEN_Msk                    (0x01U << AWK_CR_AWKEN_Pos)        /*!<  */
#define AWK_CR_AWKEN                        AWK_CR_AWKEN_Msk                   /*!<  */
 
#define AWK_CR_TCLKSEL_Pos                    (5U)
#define AWK_CR_TCLKSEL_Msk                    (0x3U << AWK_CR_TCLKSEL_Pos)        /*!<  */
#define AWK_CR_TCLKSEL                        AWK_CR_TCLKSEL_Msk                   /*!<  */
 
#define AWK_CR_HXTPRSC_Pos                    (8U)
#define AWK_CR_HXTPRSC_Msk                    (0xFFU << AWK_CR_HXTPRSC_Pos)        /*!<  */
#define AWK_CR_HXTPRSC                        AWK_CR_HXTPRSC_Msk                   /*!<  */
#define AWK_CR_ALL_Msk                        (AWK_CR_HXTPRSC_Msk | AWK_CR_TCLKSEL_Msk | AWK_CR_AWKEN_Msk | AWK_CR_DIVSEL_Msk)
 
/******************  Bit definition for AWK_RLOAD register ********************/
#define AWK_RLOAD_RLDVAL_Pos                 (0U)
#define AWK_RLOAD_RLDVAL_Msk                 (0xFFU << AWK_RLOAD_RLDVAL_Pos)        /*!<  */
#define AWK_RLOAD_RLDVAL                     AWK_RLOAD_RLDVAL_Msk                   /*!<  */
#define AWK_RLOAD_ALL_Msk                    (AWK_RLOAD_RLDVAL_Msk)
 
/******************  Bit definition for AWK_SR register ********************/
#define AWK_SR_AWUF_Pos                     (0U)
#define AWK_SR_AWUF_Msk                     (0x01U << AWK_SR_AWUF_Pos)        /*!<  */
#define AWK_SR_AWUF                         AWK_SR_AWUF_Msk                   /*!<  */
#define AWK_SR_ALL_Msk                      (AWK_SR_AWUF_Msk)
 
/******************  Bit definition for AWK_INTCLR register ********************/
#define AWK_INTCLR_INTCLR_Pos               (0U)
#define AWK_INTCLR_INTCLR_Msk               (0x01U << AWK_INTCLR_INTCLR_Pos)        /*!<  */
#define AWK_INTCLR_INTCLR                   AWK_INTCLR_INTCLR_Msk                   /*!<  */
#define AWK_INTCLR_ALL_Msk                  (AWK_INTCLR_INTCLR_Msk) 

/****************************** END TIM Instances *****************************/

typedef enum
{
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

 typedef enum
 {
     HAL_UNLOCKED = 0x00U,
     HAL_LOCKED   = 0x01U
 } HAL_LockTypeDef;
/** @addtogroup Exported_types
  * @{
  */  
typedef enum 
{
    RESET = 0, 
    SET = !RESET
} FlagStatus, ITStatus;


typedef enum 
{
    FALSE = 0, 
    TRUE = !FALSE
}STATUS_TYPEDEF;

typedef enum 
{
    DISABLE = 0, 
    ENABLE = !DISABLE
} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum 
{
    ERROR = 0, 
    SUCCESS = !ERROR
} ErrorStatus;
/** @} */ /* End of group PosMask_peripherals */

#ifdef __cplusplus
}
#endif

#endif /* _SLM32X030_H_ */


