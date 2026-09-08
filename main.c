#include "bsp_hal.h"
#include "gpio.h"
#include "ad.h"
#include "spi.h"
#include "beep.h"
#include "rf.h"
#include "tim.h"
#include "afe.h"
#include "iwdg.h"
#include "state_task.h"
#include "modbus_task.h"
#include "poke_task.h"
#include "cmdbus.h"

volatile uint32_t g_poll_cyc;     /* last loop run time [64 MHz ticks] */
volatile uint32_t g_poll_cyc_max; /* peak loop run time */
volatile uint32_t g_isr_cyc;
volatile uint32_t g_isr_cyc_max;
volatile uint32_t g_load_pct;     /* current CPU load [%], updated every 1 ms */
volatile uint32_t g_load_pct_max; /* peak CPU load [%] */

/* Q24 reciprocal scale factors for the load formula
 *   load% = (poll_cyc*1000 + isr_cyc*PWM_FREQ_HZ) * 100 / SYSCLK
 * kept as two separate addends so the constants stay 32-bit and every
 * runtime multiply is a single-cycle MULS on Cortex-M0+ (no 64-bit mul,
 * no division). The 1 ms task block runs up to 1000 times/s and the
 * carrier runs PWM_FREQ_HZ times/s. */
#define LOAD_Q      24u
#define LOAD_K_POLL ((uint32_t)(((uint64_t)1000u * 100u * (1ull << LOAD_Q) + SYSTEM_CORE_CLOCK_HZ / 2) / SYSTEM_CORE_CLOCK_HZ))
#define LOAD_K_ISR  ((uint32_t)(((uint64_t)(100u * PWM_FREQ_HZ) * (1ull << LOAD_Q) + SYSTEM_CORE_CLOCK_HZ / 2) / SYSTEM_CORE_CLOCK_HZ))

void SystemClock_Config(void)
{
#define SYSCLK_0_32MHZ    FLASH_LATENCY_0 /*!< FLASH Zero Latency cycle */
#define SYSCLK_32_64MHZ   FLASH_LATENCY_1 /*!< FLASH One Latency cycle */
#define FLASH_SYSCLK_TYPE SYSCLK_32_64MHZ
    __HAL_RCMU_SYSCFG_CLK_ENABLE();
    __HAL_RCMU_IEMU_CLK_ENABLE();
    __HAL_RCMU_SRAM_CLK_ENABLE();

    RCMU_OscInitTypeDef RCMU_OscInitStruct = {0};
    RCMU_ClkInitTypeDef RCMU_ClkInitStruct = {0};

    RCMU_OscInitStruct.HSIState      = RCMU_HSI_ON;
    RCMU_OscInitStruct.HSISelect     = RCMU_HSI_FREQ_SEL_8M;
    RCMU_OscInitStruct.PLL.PLLState  = RCMU_PLL_ON;
    RCMU_OscInitStruct.PLL.PLLType   = CFG0;
    RCMU_OscInitStruct.PLL.PLLSource = RCMU_PLL_SRC_HSI;
    RCMU_OscInitStruct.PLL.PLLPreDiv = RCMU_CFG0_N_LESS_8M;
    RCMU_OscInitStruct.PLL.PLLFreq   = RCMU_CFG0_OD_48M_TO_64M;
    RCMU_OscInitStruct.PLL.PLLMUL    = RCMU_CFG0_M_8_MUL;
    HAL_RCMU_OscConfig(&RCMU_OscInitStruct);

    RCMU_ClkInitStruct.SYSCLKSource   = RCMU_SYSCLKSOURCE_PLLCLK;
    RCMU_ClkInitStruct.AHBCLKDivider  = RCMU_SYSCLK_DIV1;
    RCMU_ClkInitStruct.APB1CLKDivider = RCMU_HCLK_DIV1;
    RCMU_ClkInitStruct.APB2CLKDivider = RCMU_HCLK_DIV1;
    HAL_RCMU_ClockConfig(&RCMU_ClkInitStruct, FLASH_SYSCLK_TYPE);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* BSP: hardware peripherals */
    gpio_init();
    adc_init();
    spi1_init();
    tim_pwm_init();
    tim_load_isr_init();
    tim_load_poll_init();
    afe_init();

    /* App tasks: state_task owns foc/fault/ntc/poke/iwdg; modbus posts onto cmdbus */
    modbus_task_init();
    state_task_init();
    poke_task_init();

    iwdg_init();

    uint32_t last_1ms        = systick_get();
    uint32_t restart_delay   = 0;
    uint8_t  auto_start_flag = 0;
    while (1)
    {
        uint32_t t0 = tim_load_poll_get(); /* poll load measurement (TIM14) */

        /* realtime task */
        modbus_task_poll();
        poke_task_poll();
        state_task_poll();

        /* 1 ms period task */
        uint32_t now = systick_get();
        if ((int32_t)(now - last_1ms) >= 1)
        {
            last_1ms = now;
            poke_task_1ms();
            state_task_1ms();

            iwdg_refresh();
#if AUTO_RUN_MODE == 1
            // free-running
            if (auto_start_flag == 0)
            {
                restart_delay++;

                if (restart_delay == 3000)
                {
                    cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_START};
                    cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
                    cmdbus_speed_t spd = {.speed_rpm = AUTO_RUN_SPD_RPM};
                    cmdbus_post(CMDBUS_CMD_SPEED, &spd, sizeof(spd));
                    auto_start_flag = 1;
                }
            }
#elif AUTO_RUN_MODE == 2
            restart_delay++;
            if (restart_delay == 15000)
            {
                cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_START};
                cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
                cmdbus_speed_t spd = {.speed_rpm = AUTO_RUN_SPD_RPM};
                cmdbus_post(CMDBUS_CMD_SPEED, &spd, sizeof(spd));
            }
            else if (restart_delay == 45000)
            {
                cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_STOP};
                cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
                restart_delay = 0;
            }
#endif

            g_load_pct = (g_poll_cyc * LOAD_K_POLL + g_isr_cyc * LOAD_K_ISR) >> LOAD_Q;
            if (g_load_pct > g_load_pct_max)
                g_load_pct_max = g_load_pct;
        }

        /* poll load: run time and peak, 64 MHz ticks */
        g_poll_cyc = tim_load_poll_get() - t0;
        if (g_poll_cyc > g_poll_cyc_max)
            g_poll_cyc_max = g_poll_cyc;
    }
}
