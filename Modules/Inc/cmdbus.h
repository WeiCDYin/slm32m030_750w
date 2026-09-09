#ifndef __CMDBUS_H__
#define __CMDBUS_H__

/* cmdbus.h -- neutral command + state bus for the application layer.
 *
 * This is the lowest application module: it knows nothing about the FOC, the
 * fault monitor, modbus or the state machine. Producers (modbus, the bench
 * poke adapter, the FOC supervisor reporting an internal trip) post commands
 * with cmdbus_post() and read the current machine state with cmdbus_get_state();
 * the consumer (main_state) registers one handler per command with
 * cmdbus_register() and drains the queue each poll with cmdbus_dispatch().
 *
 * Direction of dependency: every module -> cmdbus. Nothing here points back,
 * so the former main_state <-> foc / main_state <-> poke cycles are broken. */

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Machine states. */
typedef enum
{
    CMDBUS_INIT    = 0, /* power-on / recovery: auto -> IDLE */
    CMDBUS_IDLE    = 1, /* standby: waiting for START */
    CMDBUS_CALI    = 2, /* start calibration: two-phase current offset averaging */
    CMDBUS_CHARGE  = 3, /* pre-charge bootstrap */
    CMDBUS_RUNNING = 4, /* running: FOC holds the run command */
    CMDBUS_FAULT   = 5, /* fault: latched; RECOVERY or fault-clear returns to IDLE */
    CMDBUS_STATE_COUNT
} cmdbus_state_t;

/* Command types. Each command carries its own payload struct; cmdbus_post()
 * validates that the posted length matches the size the consumer registered. */
typedef enum
{
    CMDBUS_CMD_CTRL  = 0, /* payload: cmdbus_ctrl_t  (stop / start / fault / recovery) */
    CMDBUS_CMD_MODE  = 1, /* payload: cmdbus_mode_t  (run mode 1..6) */
    CMDBUS_CMD_SPEED = 2, /* payload: cmdbus_speed_t (rpm) */
    CMDBUS_CMD_POWER = 3, /* payload: cmdbus_power_t (W) */
    CMDBUS_CMD_DUTY  = 4, /* payload: cmdbus_duty_t  (percent 0..100 per phase) */
    CMDBUS_CMD_VEC   = 5, /* payload: cmdbus_vec_t   (vector magnitude % + angle deg) */
    CMDBUS_CMD_COUNT
} cmdbus_cmd_t;

typedef enum
{
    CMDBUS_CTRL_STOP     = 0,
    CMDBUS_CTRL_START    = 1,
    CMDBUS_CTRL_FAULT    = 2,
    CMDBUS_CTRL_RECOVERY = 3,
    CMDBUS_CTRL_NONE     = 0xFF,
} cmdbus_ctrl_type_t;

/* Run modes (aligned with modbus MB_CTRL_*). */
typedef enum
{
    CMDBUS_RUN_MODE_NONE   = 0,
    CMDBUS_RUN_MODE_DUTY   = 1,
    CMDBUS_RUN_MODE_VV     = 2,
    CMDBUS_RUN_MODE_VF     = 3,
    CMDBUS_RUN_MODE_CV     = 4,
    CMDBUS_RUN_MODE_IF     = 5,
    CMDBUS_RUN_MODE_IF_FOC = 6,

    CMDBUS_RUN_MODE_COUNT,
} cmdbus_run_mode_t;

/* ---- per-command payload structs ---- */
typedef struct
{
    uint8_t ctrl_type; /* CMDBUS_CTRL_STOP / START / FAULT / RECOVERY */
} cmdbus_ctrl_t;

typedef struct
{
    uint8_t run_mode; /* run mode 1..6 */
} cmdbus_mode_t;

typedef struct
{
    int16_t speed_rpm; /* signed speed setpoint, rpm */
} cmdbus_speed_t;

typedef struct
{
    uint16_t duty_a; /* phase-A duty, percent 0..100 */
    uint16_t duty_b;
    uint16_t duty_c;
} cmdbus_duty_t;

typedef struct
{
    uint16_t mag_pct; /* vector magnitude, percent 0..100 */
    uint16_t ang_deg; /* vector angle, deg 0..360          */
} cmdbus_vec_t;

typedef struct
{
    uint16_t power_w; /* power setpoint, W */
} cmdbus_power_t;

/* Consumer command handler: receives the command id and a pointer to its
 * payload (valid for the call only). Registered by main_state. */
typedef void (*cmdbus_handler_t)(const void *payload);

/* ---- consumer side (main_state) ---- */
/* Register the handler + expected payload size for one command id. */
void cmdbus_register(uint8_t cmd, cmdbus_handler_t handler, uint16_t payload_len);

/* Drain at most one queued command, calling its registered handler. */
void cmdbus_dispatch(void);

/* Reset the command queue + state storage to power-on defaults. */
void cmdbus_reset(void);

/* ---- producer side (modbus / poke / foc) ---- */
/* Post one command + its payload struct. `length` must equal the size
 * registered for `cmd` or the post is rejected. Returns false on a bad
 * payload or a full queue. */
bool cmdbus_post(uint8_t cmd, const void *payload, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
