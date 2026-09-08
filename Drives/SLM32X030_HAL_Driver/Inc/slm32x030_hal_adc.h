/**
  ******************************************************************************
  * @file    slm32x030_hal_adc.h
  * @author  MCU Software Team
  * @Version V1.0.0
  * @Date    8-Nov-2024
  * @brief   Header file of ADC HAL module.
  ******************************************************************************
 **/

#ifndef _SLM32X030_HAL_ADC_H_
#define _SLM32X030_HAL_ADC_H_

#ifdef __cplusplus
    extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "slm32x030_hal_conf.h"

/** @defgroup SEQ MAX CHANNEL
  * @{
  */
#define     ADC_SEQ1_CHANNEL_MAX            (18U) 
#define     ADC_SEQ2_CHANNEL_MAX            (4U)   
#define     ADC_SEQ1_DEF                    (0U) 
#define     ADC_SEQ2_DEF                    (1U)

/** @defgroup ADC_POLL_TIMEOUT
  * @{
  */
#define ADC_POLL_TIMEOUT                    (50U)

/** @defgroup ADC_SR cur_sta
  * @{
  */
typedef enum{
    ADC_IDLE,
    SEQ1_START,
    SEQ1_WAIT,
    SEQ1_READY,
    SEQ2_START,
    SEQ2_WAIT,
    SEQ2_READY,
    ADC_DONE
} ADC_CUR_STA;

/** 
  * @brief  Structure definition of ADC channel for regular group   
  * @note   The setting of these parameters with function HAL_ADC_ConfigChannel() is conditioned to ADC state.
  *         ADC can be either disabled or enabled without conversion on going on regular group.
  */ 
typedef struct 
{
    uint8_t *Seq1Ch;                    /*!< seq1 channel 1~6 configure.*/        
    uint8_t *Seq2Ch;                    /*!< seq2 channel 1~4 configure.*/              
}ADC_ChannelConfTypeDef;

/** 
  * @brief  Structure definition of ADC initialization 
  * @note   The setting of these parameters with function HAL_ADC_Init() is conditioned to ADC state.
  *         ADC can be either disabled or enabled without conversion on going on regular group.
  */
typedef struct
{
    uint32_t Seq1SampleCnt;             /*!< Specifies ADC sample time.
                                            This parameter can be a value of @ref ADC_SampleDuty */     

    uint32_t ClkSel;                    /*!< Specifies ADC clock select.
                                            This parameter can be a value of @ref ADC_ClkSel */

    uint32_t SingleContinueMode;        /*!< Specifies ADC single/continue converstion mode select.
                                            This parameter can be a value of @ref ADC_SingleContinueMode */     

    uint32_t AutoAccumulation;          /*!< Specifies ADC conversion result auto accumulation.
                                            This parameter can be a value of @ref ADC_AutoAccumulation */       

    uint32_t NbrOfConversion;           /*!< Specifies the number of conversions in continue mode.  
                                            This parameter can be a value between 0x00 to 0xff @ref ADC_Seq1CountinueMode*/     

    uint8_t Seq1Trig1;                  /*!< Selects the external interrupt 1 used to trigger the conversion.
                                            If set to ADC_SOFTWARE_START, external triggers are disabled.
                                            If set to external trigger source, triggering is on interrupt flag rising edge.
                                            This parameter can be a value of @ref ADC_External_trigger_source */
                                            
    uint8_t Seq1Trig2;                  /*!< Selects the external interrupt 2 used to trigger the conversion.
                                            If set to ADC_SOFTWARE_START, external triggers are disabled.
                                            If set to external trigger source, triggering is on interrupt flag rising edge.
                                            This parameter can be a value of @ref ADC_External_trigger_source */        
    uint8_t Seq2Trig1;                  /*!< Selects the external interrupt 1 used to trigger the conversion.
                                            If set to ADC_SOFTWARE_START, external triggers are disabled.
                                            If set to external trigger source, triggering is on interrupt flag rising edge.
                                            This parameter can be a value of @ref ADC_External_trigger_source */
                                            
    uint8_t Seq2Trig2;                  /*!< Selects the external interrupt 2 used to trigger the conversion.
                                            If set to ADC_SOFTWARE_START, external triggers are disabled.
                                            If set to external trigger source, triggering is on interrupt flag rising edge.
                                            This parameter can be a value of @ref ADC_External_trigger_source */   
    uint8_t Threshold_En;               /*!< Threshold enable @ThresholdEn*/      
    uint8_t DmaEn;                      /*!< DMA enable @DmaEn*/      
    uint8_t TemprEn;                    /*!< tempature conversion enable @ref TS_Enable*/
    uint8_t VolRefSel;                  /*!< VREFSEL reference voltage select，@ref VREFSEL */

    uint8_t Seq2_num;                   /*!< Seq2_num sample num @ref SeqNum*/
    uint8_t Seq2_awd_lt_int;            /*!< Seq2_awd_lt interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq2_awd_ht_int;            /*!< Seq2_awd_ht interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq2_awd_reg_int;           /*!< Seq2_awd_reg interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq2_1seq_int;              /*!< Seq2_1seq interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq2_1ch_int;               /*!< Seq2_1ch interrupt @Adc_Interrupt_Enable*/

    uint8_t Seq1_num;                   /*!< Seq1_num sample num @ref SeqNum*/  
    uint8_t Seq1_awd_lt_int;            /*!< Seq1_awd_lt interrupt @Adc_Interrupt_Enable*/  
    uint8_t Seq1_awd_ht_int;            /*!< Seq1_awd_ht interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq1_awd_reg_int;           /*!< Seq1_awd_reg interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq1_all_int;               /*!< Seq1_all interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq1_1seq_int;              /*!< Seq1_1seq interrupt @Adc_Interrupt_Enable*/
    uint8_t Seq1_1ch_int;               /*!< Seq1_1ch interrupt @Adc_Interrupt_Enable*/
    uint8_t Dma_overrun_int;            /*!< Dma_overrun @Adc_Interrupt_Enable*/
}ADC_InitTypeDef;

/**
  * @brief  ADC Configuration threshold definition
  * @note   The setting of these parameters with function is conditioned to ADC state.
  */
typedef struct
{
    uint32_t CompareMode;                       /*!< Specifies ADC threshold compare mode.
                                                      This parameter can be a value of @ref ADC_CompareMode */        

    uint32_t ITMode;                            /*!< Specifies whether out of threshold is configured in interrupt or polling mode.
                                                                                        This parameter can be set to ENABLE or DISABLE */

    uint32_t HighThreshold;                     /*!< Specifies ADC high level threshold.
                                                                                        This parameter can be a value between 0x0000 to 0x0fff */

    uint32_t LowThreshold;                      /*!< Specifies ADC low level threshold.
                                                                                        This parameter can be a value between 0x0000 to 0x0fff */   
}ADC_ThresholdConfTypeDef;

/** 
  * @brief  ADC handle Structure definition  
  * @note  
  *         
  */
typedef struct
{
    ADC_TypeDef                   *Instance;              /*!< Register base address */
    ADC_InitTypeDef               Init;                   /*!< ADC required parameters */
    ADC_ChannelConfTypeDef        ChCfg;                  /*!< ADC channel configure */  
    uint32_t                      *Seq1ChnValue;          /*!<  */        
    uint32_t                      *Seq2ChnValue;          /*!<  */   
    HAL_LockTypeDef               Lock;                   /*!< ADC locking object */
    __IO uint32_t                 State;                  /*!< ADC communication state (bitmap of ADC states) @Adc_State*/
    __IO uint32_t                 ErrorCode;              /*!< ADC Error code @Error_Code*/

    DMA_HandleTypeDef             *hdmaSeq1;              /*!< ADC seq1 DMA Handle parameters */
    DMA_HandleTypeDef             *hdmaSeq2;              /*!< ADC seq2 Rx DMA Handle parameters */
} ADC_HandleTypeDef;

/** @defgroup DMA Register address and offsets
  * @{
  */
#define ADC_OFFSET                      (ADC_BASE - PERIPH_BASE)
#define ADC_INTR_OFFSET                 (0x00U)    /*!< ADC Interrupt status Register offset*/
#define ADC_CR_OFFSET                   (0x04U)    /*!< ADC Control Register offset*/
#define ADC_DR_OFFSET                   (0x08U)    /*!< ADC Data Register offset*/
#define ADC_SR_OFFSET                   (0x0CU)    /*!< ADC Status Register offset*/
#define ADC_SEQ1CR1_OFFSET              (0x10U)    /*!< SEQ1 Control Register 1 offset*/
#define ADC_SEQ1CR2_OFFSET              (0x14U)    /*!< SEQ1 Control Register 2 offset*/
#define ADC_SEQ1CR3_OFFSET              (0x18U)    /*!< SEQ1 Control Register 3 offset*/
#define ADC_SEQ1CR4_OFFSET              (0x1CU)    /*!< SEQ1 Control Register 4 offset*/
#define ADC_SEQ1SR1_OFFSET              (0x20U)    /*!< SEQ1 Status Register 1  offset*/
#define ADC_SEQ1SR2_OFFSET              (0x24U)    /*!< SEQ1 Status Register 2  offset*/
#define ADC_SEQ1SR3_OFFSET              (0x28U)    /*!< SEQ1 Status Register 3  offset*/
#define ADC_SEQ1SR4_OFFSET              (0x2CU)    /*!< SEQ1 Status Register 4  offset*/
#define ADC_SEQ1SR5_OFFSET              (0x30U)    /*!< SEQ1 Status Register 5  offset*/
#define ADC_SEQ1SR6_OFFSET              (0x34U)    /*!< SEQ1 Status Register 6 offset*/
#define ADC_SEQ1SR7_OFFSET              (0x38U)    /*!< SEQ1 Status Register 7  offset*/
#define ADC_SEQ1SR8_OFFSET              (0x3CU)    /*!< SEQ1 Status Register 8  offset*/
#define ADC_SEQ1SR9_OFFSET              (0x40U)    /*!< SEQ1 Status Register 9  offset*/
#define ADC_SEQ1SR10_OFFSET             (0x44U)    /*!< SEQ1 Status Register 10 offset*/
#define ADC_SEQ1SR11_OFFSET             (0x48U)    /*!< SEQ1 Status Register 11 offset*/
#define ADC_SEQ1SR12_OFFSET             (0x4CU)    /*!< SEQ1 Status Register 12 offset*/
#define ADC_SEQ1SR13_OFFSET             (0x50U)    /*!< SEQ1 Status Register 13 offset*/
#define ADC_SEQ1SR14_OFFSET             (0x54U)    /*!< SEQ1 Status Register 14 offset*/
#define ADC_SEQ1SR15_OFFSET             (0x58U)    /*!< SEQ1 Status Register 15 offset*/
#define ADC_SEQ1SR16_OFFSET             (0x5CU)    /*!< SEQ1 Status Register 16 offset*/
#define ADC_SEQ1SR17_OFFSET             (0x60U)    /*!< SEQ1 Status Register 17 offset*/
#define ADC_SEQ1SR18_OFFSET             (0x64U)    /*!< SEQ1 Status Register 18 offset*/
#define ADC_SEQ2CR1_OFFSET              (0x68U)    /*!< SEQ1 Control Register 1 offset*/
#define ADC_SEQ2CR2_OFFSET              (0x6CU)    /*!< SEQ1 Control Register 2 offset*/
#define ADC_SEQ2SR1_OFFSET              (0x70U)    /*!< SEQ2 Status Register 1  offset*/
#define ADC_SEQ2SR2_OFFSET              (0x74U)    /*!< SEQ2 Status Register 2  offset*/
#define ADC_SEQ2SR3_OFFSET              (0x78U)    /*!< SEQ2 Status Register 3  offset*/
#define ADC_SEQ2SR4_OFFSET              (0x7CU)    /*!< SEQ2 Status Register 4  offset*/
#define ADC_ACCR_OFFSET                 (0x80U)    /*!< ADC Accumulative Register offset*/
#define ADC_AWDCR_OFFSET                (0x84U)    /*!< ADC Watchdog Control Register offset*/

#define ADC_INTR_ADDR                   (ADC_BASE + 0x00U)    /*!< ADC Interrupt status Register offset*/
#define ADC_CR_ADDR                     (ADC_BASE + 0x04U)    /*!< ADC Control Register offset*/
#define ADC_DR_ADDR                     (ADC_BASE + 0x08U)    /*!< ADC Data Register offset*/
#define ADC_SR_ADDR                     (ADC_BASE + 0x0CU)    /*!< ADC Status Register offset*/
#define ADC_SEQ1CR1_ADDR                (ADC_BASE + 0x10U)    /*!< SEQ1 Control Register 1 offset*/
#define ADC_SEQ1CR2_ADDR                (ADC_BASE + 0x14U)    /*!< SEQ1 Control Register 2 offset*/
#define ADC_SEQ1CR3_ADDR                (ADC_BASE + 0x18U)    /*!< SEQ1 Control Register 3 offset*/
#define ADC_SEQ1CR4_ADDR                (ADC_BASE + 0x1CU)    /*!< SEQ1 Control Register 4 offset*/
#define ADC_SEQ1SR1_ADDR                (ADC_BASE + 0x20U)    /*!< SEQ1 Status Register 1  offset*/
#define ADC_SEQ1SR2_ADDR                (ADC_BASE + 0x24U)    /*!< SEQ1 Status Register 2  offset*/
#define ADC_SEQ1SR3_ADDR                (ADC_BASE + 0x28U)    /*!< SEQ1 Status Register 3  offset*/
#define ADC_SEQ1SR4_ADDR                (ADC_BASE + 0x2CU)    /*!< SEQ1 Status Register 4  offset*/
#define ADC_SEQ1SR5_ADDR                (ADC_BASE + 0x30U)    /*!< SEQ1 Status Register 5  offset*/
#define ADC_SEQ1SR6_ADDR                (ADC_BASE + 0x34U)    /*!< SEQ1 Status Register 6 offset*/
#define ADC_SEQ1SR7_ADDR                (ADC_BASE + 0x38U)    /*!< SEQ1 Status Register 7  offset*/
#define ADC_SEQ1SR8_ADDR                (ADC_BASE + 0x3CU)    /*!< SEQ1 Status Register 8  offset*/
#define ADC_SEQ1SR9_ADDR                (ADC_BASE + 0x40U)    /*!< SEQ1 Status Register 9  offset*/
#define ADC_SEQ1SR10_ADDR               (ADC_BASE + 0x44U)    /*!< SEQ1 Status Register 10 offset*/
#define ADC_SEQ1SR11_ADDR               (ADC_BASE + 0x48U)    /*!< SEQ1 Status Register 11 offset*/
#define ADC_SEQ1SR12_ADDR               (ADC_BASE + 0x4CU)    /*!< SEQ1 Status Register 12 offset*/
#define ADC_SEQ1SR13_ADDR               (ADC_BASE + 0x50U)    /*!< SEQ1 Status Register 13 offset*/
#define ADC_SEQ1SR14_ADDR               (ADC_BASE + 0x54U)    /*!< SEQ1 Status Register 14 offset*/
#define ADC_SEQ1SR15_ADDR               (ADC_BASE + 0x58U)    /*!< SEQ1 Status Register 15 offset*/
#define ADC_SEQ1SR16_ADDR               (ADC_BASE + 0x5CU)    /*!< SEQ1 Status Register 16 offset*/
#define ADC_SEQ1SR17_ADDR               (ADC_BASE + 0x60U)    /*!< SEQ1 Status Register 17 offset*/
#define ADC_SEQ1SR18_ADDR               (ADC_BASE + 0x64U)    /*!< SEQ1 Status Register 18 offset*/
#define ADC_SEQ2CR1_ADDR                (ADC_BASE + 0x68U)    /*!< SEQ1 Control Register 1 offset*/
#define ADC_SEQ2CR2_ADDR                (ADC_BASE + 0x6CU)    /*!< SEQ1 Control Register 2 offset*/
#define ADC_SEQ2SR1_ADDR                (ADC_BASE + 0x70U)    /*!< SEQ2 Status Register 1  offset*/
#define ADC_SEQ2SR2_ADDR                (ADC_BASE + 0x74U)    /*!< SEQ2 Status Register 2  offset*/
#define ADC_SEQ2SR3_ADDR                (ADC_BASE + 0x78U)    /*!< SEQ2 Status Register 3  offset*/
#define ADC_SEQ2SR4_ADDR                (ADC_BASE + 0x7CU)    /*!< SEQ2 Status Register 4  offset*/
#define ADC_ACCR_ADDR                   (ADC_BASE + 0x80U)    /*!< ADC Accumulative Register offset*/
#define ADC_AWDCR_ADDR                  (ADC_BASE + 0x84U)    /*!< ADC Watchdog Control Register offset*/

/** 
  * @brief  Adc_State HAL ADC state machine: ADC states definition (bitfields)
  */ 
/* States of ADC global scope */
#define HAL_ADC_STATE_RESET             (0x00000000U)    /*!< ADC not yet initialized or disabled */
#define HAL_ADC_STATE_READY             (0x00000001U)    /*!< ADC peripheral ready for use */
#define HAL_ADC_STATE_BUSY_INTERNAL     (0x00000002U)    /*!< ADC is busy to internal process (initialization, calibration) */
#define HAL_ADC_STATE_TIMEOUT           (0x00000004U)    /*!< TimeOut occurrence */

/* States of ADC errors */
#define HAL_ADC_STATE_ERROR_INTERNAL    (0x00000010U)    /*!< Internal error occurrence */
#define HAL_ADC_STATE_ERROR_CONFIG      (0x00000020U)    /*!< Configuration error occurrence */

/* States of ADC conversion */
#define HAL_ADC_STATE_BUSY              (0x00000100U)    /*!< A conversion is ongoing (either by continuous mode, external trigger) */
#define HAL_ADC_STATE_EOC               (0x00000200U)    /*!< Conversion data available */

/* States of ADC value out of range */
#define HAL_ADC_STATE_OUTRANGE          (0x00010000U)    /*!< Out-of-range occurrence defined by user */

/** @defgroup ADC_INTR 
 * @{
 */
#define HAL_ADC_INTR_STA_NUM            (0x11U)

/** @defgroup SeqNum
 * @{
 */
#define ADC_SQE_SAMPLE_1                (0x00U)
#define ADC_SQE_SAMPLE_2                (0x01U)
#define ADC_SQE_SAMPLE_3                (0x02U)
#define ADC_SQE_SAMPLE_4                (0x03U)
#define ADC_SQE_SAMPLE_5                (0x04U)
#define ADC_SQE_SAMPLE_6                (0x05U)
#define ADC_SQE_SAMPLE_7                (0x06U)
#define ADC_SQE_SAMPLE_8                (0x07U)
#define ADC_SQE_SAMPLE_9                (0x08U)
#define ADC_SQE_SAMPLE_10               (0x09U)
#define ADC_SQE_SAMPLE_11               (0x0AU)
#define ADC_SQE_SAMPLE_12               (0x0BU)
#define ADC_SQE_SAMPLE_13               (0x0CU)
#define ADC_SQE_SAMPLE_14               (0x0DU)
#define ADC_SQE_SAMPLE_15               (0x0EU)
#define ADC_SQE_SAMPLE_16               (0x0FU)
#define ADC_SQE_SAMPLE_17               (0x10U)
#define ADC_SQE_SAMPLE_18               (0x11U)
#define ADC_SQE_SAMPLE_NULL             (0x20U)

/** @defgroup SEQ*_CH*_SEL config,16 external channel,2 internal channel
  * @{
  */
#define ADC_SQE_SEL_AIN0                (0x00U)
#define ADC_SQE_SEL_AIN1                (0x01U)
#define ADC_SQE_SEL_AIN2                (0x02U)
#define ADC_SQE_SEL_AIN3                (0x03U)
#define ADC_SQE_SEL_AIN4                (0x04U)
#define ADC_SQE_SEL_AIN5                (0x05U)
#define ADC_SQE_SEL_AIN6                (0x06U)
#define ADC_SQE_SEL_AIN7                (0x07U)
#define ADC_SQE_SEL_AIN8                (0x08U)
#define ADC_SQE_SEL_AIN9                (0x09U)
#define ADC_SQE_SEL_AIN10               (0x0AU)
#define ADC_SQE_SEL_AIN11               (0x0BU)
#define ADC_SQE_SEL_AIN12               (0x0CU)
#define ADC_SQE_SEL_AIN13               (0x0DU)
#define ADC_SQE_SEL_AIN14               (0x0EU)
#define ADC_SQE_SEL_AIN15               (0x0FU)
#define ADC_SQE_SEL_AIN16               (0x10U)
#define ADC_SQE_SEL_AIN17               (0x11U)
#define ADC_SQE_SEL_NULL                (0x1FU)         /*!< none channel select */

/** @defgroup DmaEn
  * @{
  */
#define ADC_DMA_ENABLE                     (0x01U) 
#define ADC_DMA_DISABLE                    (0x00U)

/** @defgroup TS_Enable tempature conversion enable
  * @{
  */
#define ADC_TS_ENABLE                      (0x01U) 
#define ADC_TS_DISABLE                     (0x00U)


/** @defgroup Adc_Interrupt_Enable
  * @{
  */
#define ADC_INTRPT_ENABLE                   (0x01U) 
#define ADC_INTRPT_DISABLE                  (0x00U)

/** @defgroup ADC_External_trigger_source   
  * @{
  */
#define ADC_TRIG_SEL_NULL                   (0x00U)   
#define ADC_TRIG_SEL_EXTI_LINE11            (0x01U) 
#define ADC_TRIG_SEL_EXTI_LINE15            (0x02U) 
#define ADC_TRIG_SEL_TIM1_TRGO              (0x03U)    
#define ADC_TRIG_SEL_TIM1_CC1               (0x04U) 
#define ADC_TRIG_SEL_TIM1_CC2               (0x05U)
#define ADC_TRIG_SEL_TIM1_CC3               (0x06U)
#define ADC_TRIG_SEL_TIM1_CC4               (0x07U)
#define ADC_TRIG_SEL_USART1_INT             (0x08U)
#define ADC_TRIG_SEL_USART2_INT             (0x09U)
#define ADC_TRIG_SEL_LPUART_INT             (0x0AU)
#define ADC_TRIG_SEL_TIM3_TRGO              (0x0BU)
#define ADC_TRIG_SEL_TIM3_CC1               (0x0CU)
#define ADC_TRIG_SEL_TIM3_CC4               (0x0DU)

/** @defgroup VREFSEL 
  * @{
  */
#define ADC_VREFSEL_INPUT                   (0x00U) 
#define ADC_VREFSEL_POWER                   (0x01U) 

/** @defgroup Error_Code 
  * @{
  */
#define HAL_ADC_ERROR_NONE                  (0x00U)   /*!< No error                                              */
#define HAL_ADC_ERROR_INTERNAL              (0x01U)   /*!< ADC IP internal error: if problem of clocking, enable/disable, erroneous state */

/** @defgroup ADC_SampleDuty 
  * @{
  */
#define ADC_SAMPLE_4CYCLE                   (0U)     /*!< ADC Sample 4 cycles           */
#define ADC_SAMPLE_8CYCLE                   (1U)     /*!< ADC Sample 8 cycles           */

/** @defgroup ADC_ClkSel 
  * @{
  */
#define ADC_CLOCK_PCLK_DIV1                 (0U)                 /*!< Select ADC clock PCLK          */
#define ADC_CLOCK_PCLK_DIV2                 (1U)                 /*!< Select ADC clock PCLK/2        */
#define ADC_CLOCK_PCLK_DIV4                 (2U)                 /*!< Select ADC clock PCLK/4        */
#define ADC_CLOCK_PCLK_DIV8                 (3U)                 /*!< Select ADC clock PCLK/8        */
#define ADC_CLOCK_PCLK_DIV16                (4U)                 /*!< Select ADC clock PCLK/16       */
#define ADC_CLOCK_PCLK_DIV32                (5U)                 /*!< Select ADC clock PCLK/32       */
#define ADC_CLOCK_PCLK_DIV64                (6U)                 /*!< Select ADC clock PCLK/64       */
#define ADC_CLOCK_PCLK_DIV128               (7U)                 /*!< Select ADC clock PCLK/128      */

/** @defgroup  ADC_SingleContinueMode
  * @{
  */
#define ADC_MODE_SINGLE                     (0U)                 /*!< Select ADC single mode          */
#define ADC_MODE_CONTINUE                   (1U)                 /*!< Select ADC continue mode            */

/** @defgroup ADC_AutoAccumulation ADC conversion results are auto accmulated in ADC_RESULT_ACC register
  * @{
  */
#define ADC_AUTOACC_DISABLE                 (0U) 
#define ADC_AUTOACC_ENABLE                  (1U)                 /*!< Auto accumulation enable       */

/* ADC conversion cycles (unit: ADC clock cycles)                                           */
/* (selected sampling time(4 or 8) + conversion time of 12 ADC clock cycles, with   */
/* resolution 12 bits)                                                                      */
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_4CYCLE                   (16U)
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_8CYCLE                   (20U)

/** @defgroup  ADC_Seq1CountinueMode
  * @{
  */
#define ADC_SEQ1_CONT_NUM_FOREVER           (0U)
#define ADC_SEQ1_CONT_NUM(x)                ((x <= 1) ? 1 : ((x > 256) ? 256 : (x-1)))     

/** @brief  Reset ADC handle state
  * @param  __HANDLE__: ADC handle
  * @retval None
  */
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__)                      ((__HANDLE__)->State = HAL_ADC_STATE_RESET)

/**
  * @brief Get the SEQ2SRx vaule 
  * @param __HANDLE__: ADC handle
  * @retval None
  */
#define __HAL_GET_SEQ2SR1_VAULE(__HANDLE__)                           (READ_BIT((__HANDLE__)->Instance->SEQ2SR1,0xffff))

#define __HAL_GET_SEQ2SR2_VAULE(__HANDLE__)                           ((__HANDLE__)->Instance->SEQ2SR2)

#define __HAL_GET_SEQ2SR3_VAULE(__HANDLE__)                           ((__HANDLE__)->Instance->SEQ2SR3)

#define __HAL_GET_SEQ2SR4_VAULE(__HANDLE__)                           ((__HANDLE__)->Instance->SEQ2SR4)

/**
  * @brief Enable the ADC peripheral
  * @param __HANDLE__: ADC handle
  * @retval None
  */
#define __HAL_ADC_ENABLE(__HANDLE__)                                  (SET_BIT((__HANDLE__)->Instance->CR, (ADC_CR_ADCEN)))
    
/**
  * @brief Disable the ADC peripheral
  * @param __HANDLE__: ADC handle
  * @retval None
  */
#define __HAL_ADC_DISABLE(__HANDLE__)                                 (CLEAR_BIT((__HANDLE__)->Instance->CR, (ADC_CR_ADCEN)))
    
/**
  * @brief Start the ADC conversion
  * @note  ADC module start by software, stop by hardware after conversion.     
  * @param __HANDLE__: ADC handle
  * @retval None
  */
#define __HAL_ADC_START(__HANDLE__)                                   (SET_BIT((__HANDLE__)->Instance->CR0, (ADC_CR0_START)))

#define __HAL_ADC_ENABLE_IT(__HANDLE__, __INTERRUPT__)                (SET_BIT((__HANDLE__)->Instance->INTEN, (__INTERRUPT__)))

#define __HAL_ADC_DISABLE_IT(__HANDLE__, __INTERRUPT__)               (CLEAR_BIT((__HANDLE__)->Instance->INTEN, (__INTERRUPT__)))

/** @brief  Checks if the specified ADC interrupt source is enabled or disabled.
  * @param __HANDLE__: ADC handle
  * @param __INTERRUPT__: ADC interrupt source to check
  *          This parameter can be any combination of the following values:
    *                       @arg    ADC_IT_CONTINUE:                        ADC End of Continue Conversion interrupt source 
    *                       @arg    ADC_IT_RANGE_THRESHOLD                  ADC conversion data in defined range interrupt source
    *                       @arg    ADC_IT_HIGH_THRESHOLD                   ADC conversion data above threshold interrupt source  
    *                       @arg    ADC_IT_LOW_THRESHOLD                    ADC conversion data below threshold interrupt source 
    *                       @arg    ADC_IT_CHANNEL15                        ADC End of channel15 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL14                        ADC End of channel14 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL13                        ADC End of channel13 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL12                        ADC End of channel12 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL11                        ADC End of channel11 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL10                        ADC End of channel10 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL9                         ADC End of channel9 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL8                         ADC End of channel8 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL7                         ADC End of channel7 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL6                         ADC End of channel6 conversion interrupt source 
    *                       @arg    ADC_IT_CHANNEL5                         ADC End of channel5 conversion interrupt source         
    *                       @arg    ADC_IT_CHANNEL4                         ADC End of channel4 conversion interrupt source     
    *                       @arg    ADC_IT_CHANNEL3                         ADC End of channel3 conversion interrupt source         
    *                       @arg    ADC_IT_CHANNEL2                         ADC End of channel2 conversion interrupt source         
    *                       @arg    ADC_IT_CHANNEL1                         ADC End of channel1 conversion interrupt source         
    *                       @arg    ADC_IT_CHANNEL0                         ADC End of channel0 conversion interrupt source 
  * @retval None
  */
#define __HAL_ADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)             (((__HANDLE__)->Instance->INTR & (__INTERRUPT__)) == (__INTERRUPT__))

#define __HAL_ADC_GET_FLAG(__HANDLE__, __FLAG__)                       ((((__HANDLE__)->Instance->INTR) & (__FLAG__)) == (__FLAG__))

#define __HAL_ADC_GET_STATE(__HANDLE__, __FLAG__)                      ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

#define __HAL_ADC_CLEAR_FLAG(__HANDLE__, __FLAG__)                     (ADC->INTR |= (__FLAG__) )
    
#define __HAL_ADC_GET_RAWFLAG(__HANDLE__, __FLAG__)                    ((((__HANDLE__)->Instance->RAWINTSR) & (__FLAG__)) == (__FLAG__))

/**
 * @brief Verification of ADC state: enabled or disabled
 * @param __HANDLE__: ADC handle
 * @retval SET (ADC enabled) or RESET (ADC disabled)
 */
#define ADC_IS_ENABLE(__HANDLE__)                                       (((((__HANDLE__)->Instance->CR & ADC_CR_ADCEN) == ADC_CR_ADCEN)) ? SET : RESET)

/**
  * @brief Test if conversion trigger is software start
  *        or external trigger.
  * @param __HANDLE__: ADC handle
  * @retval SET (software start) or RESET (external trigger)
  */
#define ADC_IS_SOFTWARE_START(__HANDLE__)                              ((READ_BIT((__HANDLE__)->Instance->CR1, ADC_CR1_TRIGS0) == ADC_SOFTWARE_START) && \
                                                                        (READ_BIT((__HANDLE__)->Instance->CR1, ADC_CR1_TRIGS1) == ADC_SOFTWARE_START))

/**
  * @brief Simultaneously clears and sets specific bits of the handle State
  * @note: ADC_STATE_CLR_SET() macro is merely aliased to generic macro MODIFY_REG(),
  *        the first parameter is the ADC handle State, the second parameter is the
  *        bit field to clear, the third and last parameter is the bit field to set.
  * @retval None
  */
#define ADC_STATE_CLR_SET                                               MODIFY_REG

/**
  * @brief Clear ADC error code (set it to error code: "no error")
  * @param __HANDLE__: ADC handle
  * @retval None
  */
#define ADC_CLEAR_ERRORCODE(__HANDLE__)                                 ((__HANDLE__)->ErrorCode = HAL_ADC_ERROR_NONE)

/**
 * @brief Get the maximum ADC conversion cycles on all channels.
 * Returns the selected sampling time + conversion time (12 ADC clock cycles)
 * Approximation of sampling time within 4 ranges, returns the highest value:
 *   4 sampling time: 4 + 12 = 16 cycles
 *   8 sampling time: 8 + 12 = 20 cycles
 * Unit: ADC clock cycles
 * @param __HANDLE__: ADC handle
 * @retval ADC conversion cycles on all channels
 */
#define ADC_CONVCYCLES_MAX_RANGE(__HANDLE__)                            (((((__HANDLE__)->Instance->CR0 & ADC_SAMPLE_8CYCLE) == ADC_SAMPLE_8CYCLE) ? \
                                                                            ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_8CYCLE : \
                                                                            ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_4CYCLE))

#define IS_ADC_ALL_INSTANCE(INSTANCE)                                   ((INSTANCE) == ADC)
#define IS_ADC_SEQ1_CHN_NUM(NUM)                                        ((NUM >= 0) && ((NUM <= ADC_SEQ1_CHANNEL_MAX)))
#define IS_ADC_SEQ2_CHN_NUM(NUM)                                        ((NUM >= 0) && ((NUM <= ADC_SEQ2_CHANNEL_MAX)))
#define IS_ADC_SEQ_TYPE(TYPE)                                           ((TYPE >= ADC_SQE_SEL_AIN0) && ((TYPE <= ADC_SQE_SEL_AIN17)) ||(TYPE == ADC_SQE_SEL_NULL))        
            
#define __HAL_ADC_AWDG_SET_LT(__HANDLE__,value)                         do{                                                                               \
                                                                            ((__HANDLE__)->Instance->AWDCR) &= ~ADC_AWDCR_AWD_LT_Msk;                     \
                                                                            ((__HANDLE__)->Instance->AWDCR) |= ((value & 0xFFF) << ADC_AWDCR_AWD_LT_Pos); \
                                                                        }while(0)

#define __HAL_ADC_AWDG_SET_HT(__HANDLE__,value)                         do{                                                                               \
                                                                            ((__HANDLE__)->Instance->AWDCR) &= ~ADC_AWDCR_AWD_HT_Msk;                     \
                                                                            ((__HANDLE__)->Instance->AWDCR) |= ((value & 0xFFF) << ADC_AWDCR_AWD_HT_Pos); \
                                                                        }while(0)

#define __HAL_ADC_AWDG_GET_LT(__HANDLE__)                               ((((__HANDLE__)->Instance->AWDCR) & ADC_AWDCR_AWD_LT_Msk) >> ADC_AWDCR_AWD_LT_Pos)
#define __HAL_ADC_AWDG_GET_HT(__HANDLE__)                               ((((__HANDLE__)->Instance->AWDCR) & ADC_AWDCR_AWD_HT_Msk) >> ADC_AWDCR_AWD_HT_Pos)

/* Initialization and de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc);

/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_PollForState(ADC_HandleTypeDef *hadc, ADC_CUR_STA State, uint16_t Timeout);

/* ADC retrieve conversion value intended to be used with polling or interruption */
void HAL_ADC_GetValue(ADC_HandleTypeDef * hadc, uint8_t seqindex);
uint32_t HAL_ADC_GetAccValue(ADC_HandleTypeDef* hadc);

/* ADC IRQHandler and Callbacks used in non-blocking modes (Interruption) */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);

/* ADC continuous mode, multi channel, all channels coversion complete interrupt */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_LevelOutOfRangeCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_Seq1_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_Seq2_ConvCpltCallback(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_ThresholdConfig(ADC_HandleTypeDef *hadc, ADC_ThresholdConfTypeDef *sConfig);
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef *hadc);
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc);
uint32_t HAL_ADC_GetDiv(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_Soft_Trigger(ADC_HandleTypeDef* hadc,uint8_t seqindex);
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef ADC_ConversionStatus_Reset(ADC_HandleTypeDef* hadc);
void HAL_ADC_GetLastValue(ADC_HandleTypeDef *hadc);
void ADC_DMACallbackCplt(DMA_HandleTypeDef *hdma);

#ifdef __cplusplus
    }
#endif

#endif /* _SLM32X030_HAL_ADC_H_ */

