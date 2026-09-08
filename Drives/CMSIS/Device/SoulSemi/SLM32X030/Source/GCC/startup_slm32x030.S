/******************************************************************************
 * @file     startup_slm32x030.s
 * @brief    SLM32X030 devices vector table for GCC toolchain
 * @version  V1.1.1
 * @date     20260731
 ******************************************************************************/
    .syntax unified
    .cpu cortex-m0plus
    .fpu softvfp
    .thumb

/* 导出符号 */
.global g_pfnVectors
.global __Vectors
.global __Vectors_End
.global __Vectors_Size
.global Default_Handler

/* 链接脚本定义符号 */
.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss
.extern _sheap
.extern _eheap

/* 外部C函数 */
.extern SystemInit
.extern main

/* ===== 栈/堆交由链接脚本管理，汇编内不再分配 ===== */
.equ    Stack_Size, 0x00000800
.equ    Heap_Size, 0x00000800

/* ===== 中断向量表 ===== */
.section .isr_vector, "a", %progbits
    .type   g_pfnVectors, %object
    .type   __Vectors, %object
    .type   __Vectors_End, %object

g_pfnVectors:
__Vectors:
    .word   _estack                           /* Top of Stack【修复】        0x00 */
    .word   Reset_Handler                     /* Reset Handler                0x04 */
    .word   NMI_Handler                       /* NMI Handler                  0x08 */
    .word   HardFault_Handler                 /* Hard Fault Handler           0x0C */
    .word   Nvm_Handler                       /* Nvm_Handler                  0x10 */
    .word   BusErr_Handler                    /* BusErr_Handler               0x14 */
    .word   AppErr_Handler                    /* AppErr_Handler               0x18 */
    .word   0                                 /* Reserved                     0x1C */
    .word   0                                 /* Reserved                     0x20 */
    .word   0                                 /* Reserved                     0x24 */
    .word   0                                 /* Reserved                     0x28 */
    .word   SVC_Handler                       /* SVCall Handler               0x2C */
    .word   DebugMonitor_Handler              /* DebugMonitor_Handler         0x30 */
    .word   0                                 /* Reserved                     0x34 */
    .word   PendSV_Handler                    /* PendSV Handler               0x38 */
    .word   SysTick_Handler                   /* SysTick Handler              0x3C */

    /* External Interrupts (32个) */
    .word   WWDG_IRQHandler                   /*  0 Window Watchdog           */
    .word   PVD_IRQHandler                    /*  1 PVD_IRQn                  */
    .word   RTC_IRQHandler                    /*  2 RTC through EXTI Line     */
    .word   FLASH_IRQHandler                  /*  3 FLASH_IRQHandler          */
    .word   RCMU_IRQHandler                   /*  4 RCMU_IRQHandler           */
    .word   EXTI0_1_IRQHandler                /*  5 EXTI Line 0 and 1         */
    .word   EXTI2_3_IRQHandler                /*  6 EXTI Line 2 and 3         */
    .word   EXTI4_15_IRQHandler               /*  7 EXTI Line 4 to 15         */
    .word   AES_IRQHandler                    /*  8 AES_IRQHandler            */
    .word   DMA1_Channel0_5_IRQHandler        /*  9 DMA1 Channel              */
    .word   AWKTIM_IRQHandler                 /* 10 AWKTIM_IRQn               */
    .word   TAMP_IRQHandler                   /* 11 Tamper                    */
    .word   ADC_IRQHandler                    /* 12 ADC                       */
    .word   TIM1_BRK_UP_TRG_COM_IRQHandler    /* 13 TIM1 Break/Update/Trig/Com*/
    .word   TIM1_CC_IRQHandler                /* 14 TIM1 Capture Compare      */
    .word   SRAM_IRQHandler                   /* 15 SRAM_CONTROL              */
    .word   TIM3_IRQHandler                   /* 16 TIM3                      */
    .word   TIM6_IRQHandler                   /* 17 TIM6                      */
    .word   TIM7_IRQHandler                   /* 18 TIM7                      */
    .word   TIM14_IRQHandler                  /* 19 TIM14                     */
    .word   TIM15_IRQHandler                  /* 20 TIM15                     */
    .word   TIM16_IRQHandler                  /* 21 TIM16                     */
    .word   TIM17_IRQHandler                  /* 22 TIM17                     */
    .word   I2C1_IRQHandler                   /* 23 I2C1                      */
    .word   I2C2_IRQHandler                   /* 24 I2C2                      */
    .word   SPI1_IRQHandler                   /* 25 SPI1                      */
    .word   SPI2_IRQHandler                   /* 26 SPI2                      */
    .word   USART1_IRQHandler                 /* 27 USART1                    */
    .word   USART2_IRQHandler                 /* 28 USART2                    */
    .word   LPUART_IRQHandler                 /* 29 LPUART                    */
    .word   TRNG_IRQHandler                   /* 30 TRNG                      */
    .word   VC_IRQHandler                     /* 31 VC_IRQn                   */

__Vectors_End:
.equ    __Vectors_Size, __Vectors_End - __Vectors
    .size   g_pfnVectors, . - g_pfnVectors

/* ===== Reset Handler：包含.data拷贝 + .bss清零 ===== */
.section .text.Reset_Handler, "ax", %progbits
    .weak   Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    ldr     r0, =_estack
    msr     msp, r0

    ldr     r0, =0x00000004
    ldr     r1, [r0]
    lsrs    r1, r1, #24
    ldr     r2, =0x1F
    cmp     r1, r2
    bne     ram_init_start

    /* SYSCFG clock enable */
    ldr     r0, =0x40021018
    ldr     r1, =0x00000001
    str     r1, [r0]

    /* Set CFGR1 register with flash memory remap at address 0 */
    ldr     r0, =0x40010000
    ldr     r1, =0x00000000
    str     r1, [r0]

ram_init_start:
    ldr     r1, =_sidata
    ldr     r2, =_sdata
    ldr     r3, =_edata
    cmp     r2, r3
    beq     bss_clear

data_copy_loop:
    ldr     r0, [r1]
    str     r0, [r2]
    adds    r1, r1, #4
    adds    r2, r2, #4
    cmp     r2, r3
    bcc     data_copy_loop

/* =================================== */

bss_clear:
    ldr     r1, =_sbss
    ldr     r2, =_ebss
    movs    r0, #0
    cmp     r1, r2
    beq     run_user_code

bss_clear_loop:
    str     r0, [r1]
    adds    r1, r1, #4
    cmp     r1, r2
    bcc     bss_clear_loop

run_user_code:
    bl      SystemInit
    bl      main

main_return_loop:
    b       main_return_loop

    .size   Reset_Handler, . - Reset_Handler

/* ===== 默认中断异常处理器 ===== */
.section .text.Default_Handler, "ax", %progbits
    .type   Default_Handler, %function
Default_Handler:
    b       .       /* 原地死循环，调试可打断点 */
    .size   Default_Handler, . - Default_Handler

/* 宏批量生成弱别名中断函数 */
.macro  def_irq_handler handler_name
    .weak   \handler_name
    .set    \handler_name, Default_Handler
.endm

/* 系统异常 */
def_irq_handler NMI_Handler
def_irq_handler HardFault_Handler
def_irq_handler Nvm_Handler
def_irq_handler BusErr_Handler
def_irq_handler AppErr_Handler
def_irq_handler DebugMonitor_Handler
def_irq_handler SVC_Handler
def_irq_handler PendSV_Handler
def_irq_handler SysTick_Handler

/* 外部中断 (32个) */
def_irq_handler WWDG_IRQHandler
def_irq_handler PVD_IRQHandler
def_irq_handler RTC_IRQHandler
def_irq_handler FLASH_IRQHandler
def_irq_handler RCMU_IRQHandler
def_irq_handler EXTI0_1_IRQHandler
def_irq_handler EXTI2_3_IRQHandler
def_irq_handler EXTI4_15_IRQHandler
def_irq_handler AES_IRQHandler
def_irq_handler DMA1_Channel0_5_IRQHandler
def_irq_handler AWKTIM_IRQHandler
def_irq_handler TAMP_IRQHandler
def_irq_handler ADC_IRQHandler
def_irq_handler TIM1_BRK_UP_TRG_COM_IRQHandler
def_irq_handler TIM1_CC_IRQHandler
def_irq_handler SRAM_IRQHandler
def_irq_handler TIM3_IRQHandler
def_irq_handler TIM6_IRQHandler
def_irq_handler TIM7_IRQHandler
def_irq_handler TIM14_IRQHandler
def_irq_handler TIM15_IRQHandler
def_irq_handler TIM16_IRQHandler
def_irq_handler TIM17_IRQHandler
def_irq_handler I2C1_IRQHandler
def_irq_handler I2C2_IRQHandler
def_irq_handler SPI1_IRQHandler
def_irq_handler SPI2_IRQHandler
def_irq_handler USART1_IRQHandler
def_irq_handler USART2_IRQHandler
def_irq_handler LPUART_IRQHandler
def_irq_handler TRNG_IRQHandler
def_irq_handler VC_IRQHandler

.end