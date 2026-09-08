#include "user_config.h"
#include "cmdbus.h"
#include "mc.h"
#include "mb.h"
#include "mbport.h"
#include "modbus_task.h"

/* Slave configuration. */
#define MB_SLAVE_ADDR (0x01u)
#define MB_BAUD       (9600u)
#define MB_PARITY     (MB_PAR_NONE)

static uint16_t g_modbus_hold_regs[MB_REG_MAX];

/* ------------------------------------------------------------------ */
/* Command queue helpers (modbus -> main state machine)                */
/* fill the per-command payload struct, then cmdbus_post(cmd, &struct, sizeof) */
/* ------------------------------------------------------------------ */

static void mb_post_ctrl(uint8_t v)
{
    cmdbus_ctrl_t p = {.ctrl_type = v};

    cmdbus_post(CMDBUS_CMD_CTRL, &p, sizeof(p));
}

static void mb_post_mode(uint8_t v)
{
    cmdbus_mode_t p = {.run_mode = v};

    cmdbus_post(CMDBUS_CMD_MODE, &p, sizeof(p));
}

static void mb_post_speed(int16_t v)
{
    cmdbus_speed_t p = {.speed_rpm = v};

    cmdbus_post(CMDBUS_CMD_SPEED, &p, sizeof(p));
}

static void mb_post_power(uint16_t v)
{
    cmdbus_power_t p = {.power_w = v};

    cmdbus_post(CMDBUS_CMD_POWER, &p, sizeof(p));
}

static void mb_post_duty(uint16_t a, uint16_t b, uint16_t c)
{
    cmdbus_duty_t p = {.duty_a = a, .duty_b = b, .duty_c = c};

    cmdbus_post(CMDBUS_CMD_DUTY, &p, sizeof(p));
}

static void mb_post_vec(uint16_t mag, uint16_t ang)
{
    cmdbus_vec_t p = {.mag_pct = mag, .ang_deg = ang};

    cmdbus_post(CMDBUS_CMD_VEC, &p, sizeof(p));
}

/* 0x0000 control word: 0 Stop / 1 Start / 2 Fault / 3 Recovery. State
 * transitions are driven by the main state machine; Recovery also restores
 * the register defaults here and pushes them into the machine. */
static void ctrl_apply(uint16_t val)
{
    switch (val)
    {
        case CMDBUS_CTRL_STOP:
            mb_post_ctrl(CMDBUS_CTRL_STOP);
            break;
        case CMDBUS_CTRL_START:
            mb_post_ctrl(CMDBUS_CTRL_START);
            break;
        case CMDBUS_CTRL_FAULT:
            mb_post_ctrl(CMDBUS_CTRL_FAULT);
            break;
        case CMDBUS_CTRL_RECOVERY:
            mb_post_ctrl(CMDBUS_CTRL_RECOVERY);
            break;
        default:
            break;
    }
}

/* Validate + apply a written register (addresses are the wire/protocol ones).
 * Read-only registers (MST_STATE / FAULT_LATCH / ACT_*) are rejected with
 * MB_ENOREG so the rw/r annotation in modbus.h matches the wire behaviour. */
static eMBErrorCode reg_write_apply(uint16_t addr, uint16_t val)
{
    switch (addr)
    {
        case MB_REG_CTRL:
            if (val <= CMDBUS_CTRL_RECOVERY)
            {
                g_modbus_hold_regs[addr] = val;
                ctrl_apply(val);
            }
            break;
        case MB_REG_REF_SPEED:
            if ((int16_t)val >= -SPEED_RATE_RPM && (int16_t)val <= SPEED_RATE_RPM)
            {
                g_modbus_hold_regs[addr] = val;
                mb_post_speed((int16_t)val);
            }
            break;
        case MB_REG_REF_POWER:
            g_modbus_hold_regs[addr] = val;
            mb_post_power(val);
            break;
        case MB_REG_MODE:
            if (val <= CMDBUS_RUN_MODE_IF_FOC)
            {
                g_modbus_hold_regs[addr] = val;
                mb_post_mode((uint8_t)val);
            }
            break;
        case MB_REG_DUTY_A:
        case MB_REG_DUTY_B:
        case MB_REG_DUTY_C:
            if (val <= 100)
            {
                g_modbus_hold_regs[addr] = val;
                mb_post_duty(g_modbus_hold_regs[MB_REG_DUTY_A], g_modbus_hold_regs[MB_REG_DUTY_B], g_modbus_hold_regs[MB_REG_DUTY_C]);
            }
            break;
        case MB_REG_MAG:
            if (val <= 100)
            {
                g_modbus_hold_regs[addr] = val;
                mb_post_vec(g_modbus_hold_regs[MB_REG_MAG], g_modbus_hold_regs[MB_REG_ANG]);
            }
            break;
        case MB_REG_ANG:
            if (val <= 360)
            {
                g_modbus_hold_regs[addr] = val;
                mb_post_vec(g_modbus_hold_regs[MB_REG_MAG], g_modbus_hold_regs[MB_REG_ANG]);
            }
            break;
        default: /* read-only: MST_STATE / FAULT_LATCH1/2 / ACT_SPEED / ACT_POWER */
            return MB_ENOREG;
    }
    return MB_ENOERR;
}

/* Refresh the read-only "live" registers just before a read. */
static void regs_refresh_live(void)
{
    /* Actual speed: mc->act_spd_fb (Q15 electrical pu) -> mechanical rpm.
     *   rpm = spd * W_BASE_HZ * 60 / MOTOR_NPP / Q15_ONE = spd * SPEED_RATE_RPM / Q15_ONE */
    g_modbus_hold_regs[MB_REG_ACT_SPEED] = (uint16_t)g_monitor_para.spd_rpm_fb;

    /* Actual power: DC bus V (mV) * I (mA) -> W (main.c g_udc_mv / g_idc_ma). */
    g_modbus_hold_regs[MB_REG_ACT_POWER] = (uint16_t)g_monitor_para.pwr_watt_fb;

    /* Actual temperature: NTC, signed deg C. */
    g_modbus_hold_regs[MB_REG_ACT_TEMP] = (uint16_t)(int16_t)g_monitor_para.temperature;

    /* Sticky fault history, 32-bit across two registers (cleared only by
     * CTRL=3 Recovery). */
    g_modbus_hold_regs[MB_REG_FAULT_LATCH1] = (uint16_t)(g_monitor_para.fault_latch & 0xFFFFu);
    g_modbus_hold_regs[MB_REG_FAULT_LATCH2] = (uint16_t)(g_monitor_para.fault_latch >> 16);

    /* Main state machine: 0=INIT 1=IDLE(Stop) 2=RUNNING 3=FAULT. */
    g_modbus_hold_regs[MB_REG_MST_STATE] = (uint16_t)g_monitor_para.state;
}

/* ------------------------------------------------------------------ */
/* FreeModbus holding-register callback                                */
/* ------------------------------------------------------------------ */

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    /* The stack hands out 1-based addresses (wire address + 1, see
     * mbfuncholding.c); our register map is 0-based (0x0000 = CTRL). */
    uint16_t     reg = (uint16_t)(usAddress - 1);
    uint16_t     i;
    eMBErrorCode err = MB_ENOERR;

    if (reg + usNRegs > MB_REG_MAX)
        return MB_ENOREG;

    if (eMode == MB_REG_READ)
        regs_refresh_live();

    for (i = 0; i < usNRegs && err == MB_ENOERR; i++)
    {
        switch (eMode)
        {
            case MB_REG_READ:
                *pucRegBuffer++ = (UCHAR)(g_modbus_hold_regs[reg + i] >> 8);
                *pucRegBuffer++ = (UCHAR)(g_modbus_hold_regs[reg + i] & 0xFF);
                break;
            case MB_REG_WRITE:
            {
                uint16_t val = (uint16_t)(((uint16_t)pucRegBuffer[0] << 8) | pucRegBuffer[1]);
                pucRegBuffer += 2;
                err = reg_write_apply((uint16_t)(reg + i), val);
                break;
            }
            default:
                break;
        }
    }
    return err;
}

/* ------------------------------------------------------------------ */
/* Task                                                                */
/* ------------------------------------------------------------------ */

void modbus_task_init(void)
{
    g_modbus_hold_regs[MB_REG_CTRL]      = 0;
    g_modbus_hold_regs[MB_REG_REF_SPEED] = SPEED_REF_DEFAULT;
    g_modbus_hold_regs[MB_REG_REF_POWER] = 0;
    g_modbus_hold_regs[MB_REG_MODE]      = CMDBUS_RUN_MODE_IF_FOC;
    g_modbus_hold_regs[MB_REG_DUTY_A]    = 50;
    g_modbus_hold_regs[MB_REG_DUTY_B]    = 50;
    g_modbus_hold_regs[MB_REG_DUTY_C]    = 50;
    g_modbus_hold_regs[MB_REG_MAG]       = 0;
    g_modbus_hold_regs[MB_REG_ANG]       = 0;
    /* ACT_SPEED / ACT_POWER / DEVICE_STATE are refreshed on read.
     * Setpoint defaults are restored by the machine's INIT stage; no re-post here. */

    eMBInit(MB_RTU, MB_SLAVE_ADDR, 0, MB_BAUD, MB_PARITY, 1);
    eMBEnable();
}

void modbus_task_poll(void)
{
    eMBPoll();
}
