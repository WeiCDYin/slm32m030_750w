#include "poke_task.h"
#include "user_config.h"
#include "foc.h"
#include "convert.h"
#include "cmdbus.h"

// static volatile uint8_t  g_duty_mode;
// static volatile uint8_t  g_vv_mode;
// static volatile uint8_t  g_vf_mode;
// static volatile uint8_t  g_cv_mode;
// static volatile uint8_t  g_if_mode;
// static volatile uint8_t  g_foc_mode;
// static volatile uint32_t g_poke_addr; /* the address written; set BEFORE the bump below */
// static volatile uint32_t g_poke_seq;  /* bumped last: the sequence is what commits the pair */

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
    volatile duties_t duty_q15;
} poke_t;

static poke_t g_poke;

// static volatile uint8_t *const g_bench_flag[] = {
//     &g_duty_mode, &g_vv_mode, &g_vf_mode, &g_cv_mode, &g_if_mode, &g_foc_mode,
// };
// static const state_t g_bench_state[] = {ST_DUTY, ST_VV, ST_VF, ST_CV, ST_IF, ST_IF_FOC};
// #define BENCH_N ((int)(sizeof g_bench_state / sizeof g_bench_state[0]))
// static void bench_mirror(void)
// {
//     for (int i = 0; i < BENCH_N; i++)
//         *g_bench_flag[i] = (g_hsm.state == g_bench_state[i]);
// }

// static state_t bench_request(void)
// {
//     for (int i = 0; i < BENCH_N; i++)
//         if (*g_bench_flag[i] != (g_hsm.state == g_bench_state[i]))
//             return *g_bench_flag[i] ? g_bench_state[i] : ST_IDLE;
//     return ST_COUNT;
// }

static int16_t bench_vec_mag_pct(void)
{
    return (g_hsm.state == ST_CV) ? g_poke.cv_mag_pct : g_poke.vv_mag_pct;
}

// static cmdbus_run_mode_t bench_run_mode(state_t s)
// {
//     switch (s)
//     {
//         case ST_DUTY:
//             return CMDBUS_RUN_MODE_DUTY;
//         case ST_VV:
//             return CMDBUS_RUN_MODE_VV;
//         case ST_VF:
//             return CMDBUS_RUN_MODE_VF;
//         case ST_CV:
//             return CMDBUS_RUN_MODE_CV;
//         case ST_IF:
//             return CMDBUS_RUN_MODE_IF;
//         case ST_IF_FOC:
//             return CMDBUS_RUN_MODE_IF_FOC;
//         default:
//             return CMDBUS_RUN_MODE_NONE;
//     }
// }

// static bool poked(const volatile void *knob)
// {
//     return g_poke_addr == (uint32_t)knob;
// }

// static void bench_emit(void)
// {
//     if (poked(&g_spd_rpm))
//     {
//         cmdbus_speed_t p = {.speed_rpm = g_spd_rpm};
//         cmdbus_post(CMDBUS_CMD_SPEED, &p, sizeof(p));
//     }
//     else if (poked(&g_vv_mag_pct) || poked(&g_cv_mag_pct) || poked(&g_vec_ang_deg))
//     {
//         /* Raw percent/degrees: mst_cmd_exec does the q15 conversion and the VV/CV
//          * angle correction (main_state.c vec_bam, by run mode). */
//         cmdbus_vec_t p = {.mag_pct = (uint16_t)bench_vec_mag_pct(), .ang_deg = (uint16_t)g_vec_ang_deg};
//         cmdbus_post(CMDBUS_CMD_VEC, &p, sizeof(p));
//     }
//     else if (poked(&g_da_pct) || poked(&g_db_pct) || poked(&g_dc_pct))
//     {
//         /* DUTY keeps the direct write -- the same destination CMDBUS_CMD_DUTY -> poke_task_set_duty
//          * would reach, but unconditional: mst_cmd_exec gates setpoints on RUNNING, and a bench
//          * duty poke must land whatever the machine state. */
//         g_poke_duty_q15.a = percent_to_q15(g_da_pct);
//         g_poke_duty_q15.b = percent_to_q15(g_db_pct);
//         g_poke_duty_q15.c = percent_to_q15(g_dc_pct);
//     }
// }

void poke_task_init(void)
{
    g_poke.spd_rpm = 100;
    g_poke.da_pct = 50;
    g_poke.db_pct = 50;
    g_poke.dc_pct = 50;
    g_poke.vv_mag_pct = 2;
    g_poke.cv_mag_pct = 5;
    g_poke.vec_ang_deg = 0;
    g_poke.duty_q15.a = Q15_ONE >> 1;
    g_poke.duty_q15.b = Q15_ONE >> 1;
    g_poke.duty_q15.c = Q15_ONE >> 1;
}

void poke_task_poll(void)
{
    // for poke process
    static uint8_t cur_mode    = CMDBUS_RUN_MODE_NONE;
    static int16_t spd_rpm     = 0;
    static int16_t vv_mag_pct  = 0;
    static int16_t cv_mag_pct  = 0;
    static int16_t vec_ang_deg = 0;
    static int16_t da_pct      = 0;
    static int16_t db_pct      = 0;
    static int16_t dc_pct      = 0;

    // static state_t  prev_state  = ST_ROOT; /* != the ST_IDLE above, so pass 1 publishes the mirrors */
    // static uint32_t poke_seq = 0;
    // if (g_hsm.state != prev_state)
    // { /* moved: our own event, or a FAULT */
    //     prev_state = g_hsm.state;
    //     bench_mirror(); /* the losers snap back to 0 */
    // }
    // if (g_poke_seq != poke_seq)
    // { /* a write landed; g_poke_addr says where */
    //     poke_seq    = g_poke_seq;
    //     state_t req = bench_request(); /* a flag disagreeing with the mirror */
    //     if (req != ST_COUNT)
    //     {
    //         if (req == ST_IDLE)
    //         { /* flag cleared: stop */
    //             cmdbus_ctrl_t p = {.ctrl_type = CMDBUS_CTRL_STOP};
    //             cmdbus_post(CMDBUS_CMD_CTRL, &p, sizeof(p));
    //         }
    //         else
    //         { /* flag set: the run mode, applied at the next START */
    //             cmdbus_mode_t p = {.run_mode = bench_run_mode(req)};
    //             cmdbus_post(CMDBUS_CMD_MODE, &p, sizeof(p));
    //             if (p.run_mode == CMDBUS_RUN_MODE_NONE)
    //             {
    //                 cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_STOP};
    //                 cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
    //             }
    //             else
    //             {
    //                 cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_START};
    //                 cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
    //             }
    //         }
    //     }
    //     bench_emit(); /* and what the armed leaf owns */
    // }

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
        g_poke.duty_q15.a = percent_to_q15(g_poke.da_pct);
        g_poke.duty_q15.b = percent_to_q15(g_poke.db_pct);
        g_poke.duty_q15.c = percent_to_q15(g_poke.dc_pct);
        da_pct            = g_poke.da_pct;
        db_pct            = g_poke.db_pct;
        dc_pct            = g_poke.dc_pct;
    }
}

void poke_task_isr(mc_in_t *in)
{
    in->duty_abc = g_poke.duty_q15;
    return;
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

void poke_task_set_duty(int16_t duty_a, int16_t duty_b, int16_t duty_c)
{
    g_poke.duty_q15.a = duty_a;
    g_poke.duty_q15.b = duty_b;
    g_poke.duty_q15.c = duty_c;
}
