;******************** (C) COPYRIGHT 2023 	********************
;* File Name          : startup_slm32f030.s
;* Author             :  
;* Version            : V1.0.0
;* Date               : 20230628
;* Description        : SLM32F030 devices vector table for MDK-ARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
;*
;* Redistribution and use in source and binary forms, with or without modification,
;* are permitted provided that the following conditions are met:
;*   1. Redistributions of source code must retain the above copyright notice,
;*      this list of conditions and the following disclaimer.
;*   2. Redistributions in binary form must reproduce the above copyright notice,
;*      this list of conditions and the following disclaimer in the documentation
;*      and/or other materials provided with the distribution.
;*   3. Neither the name of SLM nor the names of its contributors
;*      may be used to endorse or promote products derived from this software
;*      without specific prior written permission.
;*
;* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; 
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
;                                                                                                               address																											ï¿½ï¿½Ö·		
__Vectors       DCD     __initial_sp                    ; Top of Stack                                          0x0000_0000
                DCD     Reset_Handler                   ; Reset Handler                                         0x0000_0004	
                DCD     NMI_Handler                     ; NMI Handler  RCMU Ê±ï¿½Ó°ï¿½È«ÏµÍ³(CSS)                    0x0000_0008
                DCD     HardFault_Handler               ; Hard Fault Handler Ó²ï¿½ï¿½Ê§Ð§ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Íµï¿½Ê§Ð§               0x0000_000c
                DCD     Nvm_Handler                     ; ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½
                DCD     BusErr_Handler                  ; ï¿½ï¿½ï¿½ß´ï¿½ï¿½ï¿½ Ô¤È¡Ö¸Ê§ï¿½Ü£ï¿½ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê§ï¿½ï¿½
                DCD     AppErr_Handler                  ; ï¿½ï¿½ï¿½ï¿½Ó¦ï¿½ï¿½
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     0                               ; Reserved
                DCD     SVC_Handler                     ; SVCall Handler        ÏµÍ³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?                      0x0000_002C
                DCD     DebugMonitor_Handler            ; ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ô¼ï¿½ï¿?                                                 0x0000_0030
                DCD     0                               ; Reserved
                DCD     PendSV_Handler                  ; PendSV Handler
                DCD     SysTick_Handler                 ; SysTick Handler						                 0x0000_003C

                ; External Interrupts
                DCD     WWDG_IRQHandler                 ; 0	Window Watchdog						                 0x0000_0040
                DCD     PVD_IRQHandler                  ; 1	PVD_IRQn	ï¿½ï¿½ï¿½ï¿½EXTI ï¿½Äµï¿½Ñ¹ï¿½ï¿½ï¿½ï¿½Ð¶ï¿?			
                DCD     RTC_IRQHandler                  ; 2	RTC through EXTI Line
                DCD     FLASH_IRQHandler                ; 3	FLASH
                DCD     RCMU_IRQHandler                 ; 4	ï¿½ï¿½Î»ï¿½ï¿½Ê±ï¿½Ó¿ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½
                DCD     EXTI0_1_IRQHandler              ; 5	EXTI Line 0 and 1
                DCD     EXTI2_3_IRQHandler              ; 6	EXTI Line 2 and 3
                DCD     EXTI4_15_IRQHandler             ; 7	EXTI Line 4 to 15
                DCD     AES_IRQHandler                  ; 8     AES AES È«ï¿½ï¿½ï¿½Ð¶ï¿½
                DCD     DMA1_Channel0_5_IRQHandler      ; 9	DMA1 Channel 1
                DCD     AWKTIM_IRQHandler	        ; 10	AWKTIM_IRQn            = 10,
                DCD     TAMP_IRQHandler                 ; 11    Tamper 
                DCD     ADC_IRQHandler                  ; 12	ADC 
                DCD     TIM1_BRK_UP_TRG_COM_IRQHandler  ; 13	TIM1 Break, Update, Trigger and Commutation
                DCD     TIM1_CC_IRQHandler              ; 14	TIM1 Capture Compare
                DCD     SRAM_IRQHandler	                ; 15	SRAM_CONTROL
                DCD     TIM3_IRQHandler                 ; 16	TIM3
                DCD     TIM6_IRQHandler                 ; 17	TIM6
                DCD     TIM7_IRQHandler                 ; 18	TIM7
                DCD     TIM14_IRQHandler                ; 19	TIM14
                DCD     TIM15_IRQHandler                ; 20	TIM15
                DCD     TIM16_IRQHandler                ; 21	TIM16
                DCD     TIM17_IRQHandler                ; 22	TIM17
                DCD     I2C1_IRQHandler                 ; 23	I2C1
                DCD     I2C2_IRQHandler                 ; 24	I2C2
                DCD     SPI1_IRQHandler                 ; 25	SPI1
                DCD     SPI2_IRQHandler                 ; 26	SPI2
                DCD     USART1_IRQHandler               ; 27	USART1
                DCD     USART2_IRQHandler               ; 28	USART2
                DCD     LPUART_IRQHandler               ; 29	LPUART_IRQn
                DCD     RNG_IRQHandler                  ; 30    TRNG È«ï¿½ï¿½ï¿½Ð¶ï¿½
                DCD     VC_IRQHandler             	; 31    VC_IRQn             =31,
                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
        IMPORT  __main
        IMPORT  SystemInit



        LDR     R0, =__initial_sp          ; set stack pointer 
        MSR     MSP, R0  

;;Check if boot space corresponds to test memory 

        LDR R0,=0x00000004
        LDR R1, [R0]
        LSRS R1, R1, #24
        LDR R2,=0x1F
        CMP R1, R2
        
        BNE ApplicationStart  
     
;; SYSCFG clock enable    
     
        LDR R0,=0x40021018 
        LDR R1,=0x00000001
        STR R1, [R0]
        
;; Set CFGR1 register with flash memory remap at address 0

        LDR R0,=0x40010000 
        LDR R1,=0x00000000
        STR R1, [R0]
ApplicationStart        
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler   PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
Nvm_Handler     PROC
                EXPORT  Nvm_Handler                    [WEAK]
                B       .
                ENDP
BusErr_Handler	PROC
                EXPORT  BusErr_Handler                 [WEAK]
                B       .
                ENDP    
AppErr_Handler	PROC
                EXPORT  AppErr_Handler                 [WEAK]
                B       .
                ENDP   
DebugMonitor_Handler    PROC
                        EXPORT  AppErr_Handler         [WEAK]
                        B       .
                        ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                [WEAK]
                EXPORT  PVD_IRQHandler                         [WEAK]
                EXPORT  RTC_IRQHandler                 [WEAK]
                EXPORT  FLASH_IRQHandler               [WEAK]
                EXPORT  RCMU_IRQHandler                [WEAK]
                EXPORT  EXTI0_1_IRQHandler             [WEAK]
                EXPORT  EXTI2_3_IRQHandler             [WEAK]
                EXPORT  EXTI4_15_IRQHandler            [WEAK]
                EXPORT  AES_IRQHandler                 [WEAK]
                EXPORT  DMA1_Channel0_5_IRQHandler     [WEAK]
                EXPORT  AWKTIM_IRQHandler              [WEAK]
                EXPORT  TAMP_IRQHandler                [WEAK]
                EXPORT  ADC_IRQHandler                 [WEAK]
                EXPORT  TIM1_BRK_UP_TRG_COM_IRQHandler [WEAK]
                EXPORT  TIM1_CC_IRQHandler             [WEAK]
                EXPORT  SRAM_IRQHandler                [WEAK]
                EXPORT  TIM3_IRQHandler                [WEAK]
                EXPORT  TIM6_IRQHandler                [WEAK]
                EXPORT  TIM7_IRQHandler                [WEAK]
                EXPORT  TIM14_IRQHandler               [WEAK]
                EXPORT  TIM15_IRQHandler               [WEAK]
                EXPORT  TIM16_IRQHandler               [WEAK]
                EXPORT  TIM17_IRQHandler               [WEAK]
                EXPORT  I2C1_IRQHandler                [WEAK]
                EXPORT  I2C2_IRQHandler                [WEAK]
                EXPORT  SPI1_IRQHandler                [WEAK]
                EXPORT  SPI2_IRQHandler                [WEAK]
                EXPORT  USART1_IRQHandler              [WEAK]
                EXPORT  USART2_IRQHandler              [WEAK]
                EXPORT  LPUART_IRQHandler              [WEAK]
                EXPORT  RNG_IRQHandler	               [WEAK]
                EXPORT  VC_IRQHandler	               [WEAK]



WWDG_IRQHandler
PVD_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
RCMU_IRQHandler
EXTI0_1_IRQHandler
EXTI2_3_IRQHandler
EXTI4_15_IRQHandler
AES_IRQHandler
DMA1_Channel0_5_IRQHandler
AWKTIM_IRQHandler
TAMP_IRQHandler
ADC_IRQHandler
TIM1_BRK_UP_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
SRAM_IRQHandler
TIM3_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
TIM14_IRQHandler
TIM15_IRQHandler
TIM16_IRQHandler
TIM17_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
LPUART_IRQHandler
RNG_IRQHandler
VC_IRQHandler


                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;************************ (C) COPYRIGHT SoulSemi *****END OF FILE*****
