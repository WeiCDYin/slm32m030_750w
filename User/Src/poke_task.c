#include "poke_task.h"
#include "user_control.h"
#include "foc.h"
#include "convert.h"
#include "cmdbus.h"

typedef struct
{
    volatile uint8_t  run_mode;
    volatile uint32_t resync_delay;
    volatile uint8_t  resync_flag;
    volatile int16_t  spd_rpm;
    volatile int16_t  da_pct;
    volatile int16_t  db_pct;
    volatile int16_t  dc_pct;
    volatile int16_t  vv_mag_pct;
    volatile int16_t  cv_mag_pct;
    volatile int16_t  vec_ang_deg;
} poke_t;

static poke_t g_poke;

static int16_t bench_vec_mag_pct(void)
{
    return (g_hsm.state == ST_CV) ? g_poke.cv_mag_pct : g_poke.vv_mag_pct;
}

void poke_task_init(void)
{
    g_poke.spd_rpm     = SPEED_REF_DEFAULT;
    g_poke.da_pct      = 50;
    g_poke.db_pct      = 50;
    g_poke.dc_pct      = 50;
    g_poke.vv_mag_pct  = 2;
    g_poke.cv_mag_pct  = 5;
    g_poke.vec_ang_deg = 0;
}

void poke_task_poll(void)
{
    // for poke process
    static uint8_t cur_mode    = CMDBUS_RUN_MODE_NONE;
    static int16_t spd_rpm     = SPEED_REF_DEFAULT;
    static int16_t vv_mag_pct  = 0;
    static int16_t cv_mag_pct  = 0;
    static int16_t vec_ang_deg = 0;
    static int16_t da_pct      = 0;
    static int16_t db_pct      = 0;
    static int16_t dc_pct      = 0;

    if (cur_mode != g_poke.run_mode)
    {
        cur_mode = g_poke.run_mode;
        /* A run mode is not a command to the leaf: mst_cmd_exec remembers it
         * and the main state machine applies it at the next START. */
        cmdbus_mode_t p = {.run_mode = cur_mode};
        cmdbus_post(CMDBUS_CMD_MODE, &p, sizeof(p));
        if (p.run_mode == CMDBUS_RUN_MODE_NONE)
        {
            cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_STOP};
            cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
        }
        else
        {
            cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_START};
            cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
        }
    }

    if (g_poke.resync_flag == 1)
    {
        g_poke.resync_flag = 2;
        cmdbus_speed_t p   = {.speed_rpm = 0};
        cmdbus_post(CMDBUS_CMD_SPEED, &p, sizeof(p));
    }

    if (spd_rpm != g_poke.spd_rpm)
    {
        spd_rpm          = g_poke.spd_rpm;
        cmdbus_speed_t p = {.speed_rpm = spd_rpm};
        cmdbus_post(CMDBUS_CMD_SPEED, &p, sizeof(p));
    }

    if (vv_mag_pct != g_poke.vv_mag_pct || cv_mag_pct != g_poke.cv_mag_pct || vec_ang_deg != g_poke.vec_ang_deg)
    {
        /* clamp the bench knobs themselves so the scope value cannot exceed the limit */
        if (g_poke.vv_mag_pct > (int16_t)VV_MAG_PCT_MAX)
            g_poke.vv_mag_pct = (int16_t)VV_MAG_PCT_MAX;
        if (g_poke.cv_mag_pct > (int16_t)CV_MAG_PCT_MAX)
            g_poke.cv_mag_pct = (int16_t)CV_MAG_PCT_MAX;

        vv_mag_pct  = g_poke.vv_mag_pct;
        cv_mag_pct  = g_poke.cv_mag_pct;
        vec_ang_deg = g_poke.vec_ang_deg;
        /* Raw percent/degrees here: mst_cmd_exec does the q15 conversion and
         * the VV/CV angle correction (main_state.c vec_bam, by run mode). */
        cmdbus_vec_t p = {.mag_pct = (uint16_t)bench_vec_mag_pct(), .ang_deg = (uint16_t)g_poke.vec_ang_deg};
        cmdbus_post(CMDBUS_CMD_VEC, &p, sizeof(p));
    }

    if (da_pct != g_poke.da_pct || db_pct != g_poke.db_pct || dc_pct != g_poke.dc_pct)
    {
        da_pct = g_poke.da_pct;
        db_pct = g_poke.db_pct;
        dc_pct = g_poke.dc_pct;

        cmdbus_duty_t p = {.duty_a = da_pct, .duty_b = db_pct, .duty_c = dc_pct};
        cmdbus_post(CMDBUS_CMD_DUTY, &p, sizeof(p));
    }
}

void poke_task_1ms(void)
{
    if (g_poke.resync_flag == 2)
    {
        if (g_poke.resync_delay == 0)
        {
            g_poke.resync_flag = 0;
            cmdbus_speed_t p   = {.speed_rpm = g_poke.spd_rpm};
            cmdbus_post(CMDBUS_CMD_SPEED, &p, sizeof(p));
        }
        else
        {
            g_poke.resync_delay--;
        }
    }
}
