#ifndef __MODBUS_TASK_H__
#define __MODBUS_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/*
 * Modbus RTU slave on USART2 (RS485, PB6 direction). Holding registers (protocol addresses, 0-based).
 * Access flags match the rw/r annotations below and are enforced on the wire:
 * read-only registers reject writes (illegal data address, MB_ENOREG).
 *
 *   User registers:
 *     0x0000  CTRL         rw  0:Stop  1:Start  2:Fault  3:Recovery(clear faults + reset)
 *     0x0001  MST_STATE    r   main state machine: 0=INIT 1=IDLE(Stop)
 *                              2=RUNNING 3=CALI(offset calibration) 4=FAULT (latched, needs CTRL=3)
 *     0x0002  FAULT_LATCH1 r   sticky fault history, LOW 16 bits; every fault
 *                              id that ever tripped; NOT cleared when the
 *                              fault recovers, only by CTRL=3 (Recovery)
 *     0x0003  FAULT_LATCH2 r   sticky fault history, HIGH 16 bits (bits 16-31)
 *     0x0004  REF_SPEED    rw  speed setpoint, rpm (dec; +-1460 = +-1.0 pu)
 *     0x0005  ACT_SPEED    r   actual speed, rpm
 *     0x0006  REF_POWER    rw  power setpoint, W (consumed by the application)
 *     0x0007  ACT_POWER    r   actual power, W (DC bus V * I)
 *     0x0008  ACT_TEMP     r   NTC temperature, deg C (signed)
 *
 *   Debug registers (not part of the user API):
 *     0x0010  MODE         rw  run mode: 1=DUTY 2=VV 3=VF 4=CV 5=IF 6=IF_FOC
 *     0x0011  DUTY_A       rw  phase-A duty, percent 0~100
 *     0x0012  DUTY_B       rw  phase-B duty, percent 0~100
 *     0x0013  DUTY_C       rw  phase-C duty, percent 0~100
 *     0x0014  MAG          rw  vector magnitude, percent 0~100
 *     0x0015  ANG          rw  vector angle, deg 0~360 (VV/CV modes)
 *
 * Call modbus_task_init() once after uart2_init()/foc_init(); call
 * modbus_poll() from the main loop (it drains eMBPoll, which also feeds
 * the hsm command path -- see ctrl_apply in modbus.c).
 */
typedef enum
{
    MB_REG_CTRL         = 0x0000, // rw, 0:Stop; 1:Start; 2:Fault; 3:Recovery
    MB_REG_MST_STATE    = 0x0001, // r main state machine: 0:INIT 1:IDLE(Stop) 2:RUNNING 3:CALI 4:FAULT
    MB_REG_FAULT_LATCH1 = 0x0002, // r sticky fault history, low 16 bits; cleared only by CTRL=3 (Recovery)
    MB_REG_FAULT_LATCH2 = 0x0003, // r sticky fault history, high 16 bits
    MB_REG_REF_SPEED    = 0x0004, // rw
    MB_REG_ACT_SPEED    = 0x0005, // r
    MB_REG_REF_POWER    = 0x0006, // rw
    MB_REG_ACT_POWER    = 0x0007, // r
    MB_REG_ACT_TEMP     = 0x0008, // r signed NTC temperature [deg C]

    // debug register, not for user api
    MB_REG_MODE   = 0x0010,
    MB_REG_DUTY_A = 0x0011,
    MB_REG_DUTY_B = 0x0012,
    MB_REG_DUTY_C = 0x0013,
    MB_REG_MAG    = 0x0014,
    MB_REG_ANG    = 0x0015,

    MB_REG_MAX,
} mb_register_id;

void modbus_task_init(void);
void modbus_task_poll(void);

#ifdef __cplusplus
}
#endif
#endif
