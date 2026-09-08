/**
  ******************************************************************************
    PLACEHOLDER for CMSIS-CORE Startup Device Assembler File 
    for the LPC1800 Series for the IAR Compiler Toolchain
  ******************************************************************************  
  */ 
  
        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                  ; Reset Handler
        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; External Interrupts
        DCD     WWDG_IRQHandler                 ; 0
        DCD     PVD_IRQHandler                  ; 1
        DCD     RTC_IRQHandler                  ; 2
        DCD     FLASH_IRQHandler                ; 3
        DCD     RCMU_IRQHandler                 ; 4
        DCD     EXTI0_1_IRQHandler              ; 5
        DCD     EXTI2_3_IRQHandler              ; 6
        DCD     EXTI4_15_IRQHandler             ; 7
        DCD     AES_IRQHandler                  ; 8
        DCD     DMA1_Channel0_5_IRQHandler      ; 9
        DCD     AWKTIM_IRQHandler               ; 10
        DCD     TAMP_IRQHandler                 ; 11
        DCD     ADC_IRQHandler                  ; 12
        DCD     TIM1_BRK_UP_TRG_COM_IRQHandler  ; 13
        DCD     TIM1_CC_IRQHandler              ; 14
        DCD     SRAM_IRQHandler	                ; 15
        DCD     TIM3_IRQHandler                 ; 16
        DCD     TIM6_IRQHandler                 ; 17
        DCD     TIM7_IRQHandler                 ; 18
        DCD     TIM14_IRQHandler                ; 19
        DCD     TIM15_IRQHandler                ; 20
        DCD     TIM16_IRQHandler                ; 21
        DCD     TIM17_IRQHandler                ; 22
        DCD     I2C1_IRQHandler                 ; 23
        DCD     I2C2_IRQHandler                 ; 24
        DCD     SPI1_IRQHandler                 ; 25
        DCD     SPI2_IRQHandler                 ; 26
        DCD     USART1_IRQHandler               ; 27
        DCD     USART2_IRQHandler               ; 28
        DCD     LPUART_IRQHandler               ; 29
        DCD     RNG_IRQHandler                  ; 30
        DCD     VC_IRQHandler             	    ; 31

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PVD_IRQHandler
        B PVD_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK RCMU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCMU_IRQHandler
        B RCMU_IRQHandler

        PUBWEAK EXTI0_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_1_IRQHandler
        B EXTI0_1_IRQHandler

        PUBWEAK EXTI2_3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_3_IRQHandler
        B EXTI2_3_IRQHandler

        PUBWEAK EXTI4_15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_15_IRQHandler
        B EXTI4_15_IRQHandler

        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AES_IRQHandler
        B AES_IRQHandler

        PUBWEAK DMA1_Channel0_5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel0_5_IRQHandler
        B DMA1_Channel0_5_IRQHandler

        PUBWEAK AWKTIM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AWKTIM_IRQHandler
        B AWKTIM_IRQHandler

        PUBWEAK TAMP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TAMP_IRQHandler
        B TAMP_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK TIM1_BRK_UP_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_BRK_UP_TRG_COM_IRQHandler
        B TIM1_BRK_UP_TRG_COM_IRQHandler

        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler

        PUBWEAK SRAM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SRAM_IRQHandler
        B SRAM_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM3_IRQHandler
        B TIM3_IRQHandler

        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM6_IRQHandler
        B TIM6_IRQHandler

        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM7_IRQHandler
        B TIM7_IRQHandler

        PUBWEAK TIM14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM14_IRQHandler
        B TIM14_IRQHandler

        PUBWEAK TIM15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM15_IRQHandler
        B TIM15_IRQHandler

        PUBWEAK TIM16_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM16_IRQHandler
        B TIM16_IRQHandler

        PUBWEAK TIM17_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM17_IRQHandler
        B TIM17_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_IRQHandler
        B I2C2_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK LPUART_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPUART_IRQHandler
        B LPUART_IRQHandler

        PUBWEAK RNG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RNG_IRQHandler
        B RNG_IRQHandler

        PUBWEAK VC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
VC_IRQHandler
        B VC_IRQHandler

        END
;************************ (C) COPYRIGHT SoulSemi *****END OF FILE*****
