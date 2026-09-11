#include "cmdbus.h"

/* ---- command queue: a ring of (cmd + payload), producers -> consumer ---- */
#define CMDBUS_Q_MAX 32 /* command queue depth */

/* Largest payload of any command (cmdbus_duty_t). */
typedef struct
{
    uint8_t  cmd;
    uint8_t  len;
    union
    {
        cmdbus_ctrl_t  ctrl;
        cmdbus_mode_t  mode;
        cmdbus_speed_t speed;
        cmdbus_duty_t  duty;
        cmdbus_vec_t   vec;
        cmdbus_power_t power;
        uint8_t        raw[6];
    } data;
} cmdbus_slot_t;

typedef struct
{
    // cmdbus queue
    cmdbus_slot_t    queue[CMDBUS_Q_MAX];
    volatile uint8_t head;    /* next free slot (producer) */
    volatile uint8_t tail;    /* next unread slot (consumer) */
    volatile uint8_t counter; /* entries currently queued */

    // cmdbus callbacl
    cmdbus_handler_t handler[CMDBUS_CMD_COUNT];
    uint16_t         payload_len[CMDBUS_CMD_COUNT];
} cmdbus_t;

static cmdbus_t g_cmdbus;

void cmdbus_register(uint8_t cmd, cmdbus_handler_t handler, uint16_t payload_len)
{
    if (cmd >= CMDBUS_CMD_COUNT)
        return;
    g_cmdbus.handler[cmd]     = handler;
    g_cmdbus.payload_len[cmd] = payload_len;
}

void cmdbus_reset(void)
{
    g_cmdbus.head    = 0;
    g_cmdbus.tail    = 0;
    g_cmdbus.counter = 0;
}

bool cmdbus_post(uint8_t cmd, const void *payload, uint16_t length)
{
    if (cmd >= CMDBUS_CMD_COUNT || payload == NULL || g_cmdbus.handler[cmd] == NULL || length != g_cmdbus.payload_len[cmd])
        return false;

    if (g_cmdbus.counter >= CMDBUS_Q_MAX)
        return false; /* queue full: drop; a live fault is still caught by fault_get polling */

    cmdbus_slot_t *s = &g_cmdbus.queue[g_cmdbus.head];
    s->cmd           = cmd;
    s->len           = length;
    for (uint16_t i = 0; i < length; i++)
        s->data.raw[i] = ((const uint8_t *)payload)[i];

    g_cmdbus.head = (uint8_t)((g_cmdbus.head + 1u) % CMDBUS_Q_MAX);
    g_cmdbus.counter++;
    return true;
}

void cmdbus_dispatch(void)
{
    /* Consume at most ONE command per call: main_state runs one state-machine
     * step after each dispatch, so a freshly latched CTRL is consumed/cleared
     * before the next command is applied (no overwrite of the control req). */
    if (g_cmdbus.counter == 0)
        return;

    cmdbus_slot_t *s   = &g_cmdbus.queue[g_cmdbus.tail];
    uint8_t        cmd = s->cmd;
    g_cmdbus.tail      = (uint8_t)((g_cmdbus.tail + 1u) % CMDBUS_Q_MAX);
    g_cmdbus.counter--;

    if (cmd < CMDBUS_CMD_COUNT && g_cmdbus.handler[cmd])
        g_cmdbus.handler[cmd](&s->data);
}
