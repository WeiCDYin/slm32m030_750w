#include "foc.h"
#include "port.h"
#include "conv.h"
#include "convert.h"
#include "cmdbus.h"
#include "fault.h"
#include "user_control.h"

// FOC loop periods [s], derived from the rates configured in main.h
#define TS_FAST (1.0f / FOC_FAST_HZ)
#define TS_SLOW (1.0f / FOC_SLOW_HZ)

static cc_t  g_cc;
static sc_t  g_sc;
static if_t  g_ifg;
static vf_t  g_vf;
static smo_t g_smo;
static ob_t  g_ob;
mc_t         g_mc;
hsm_t        g_hsm, *g_hsm_ptr;

static cc_cfg_t g_cc_cfg = {
    .alpha_c = CC_BW_RAD_S,
    .Ts      = TS_FAST,
};

static sc_cfg_t g_sc_cfg = {
    .alpha_s   = SC_BW_RAD_S,
    .i_max_pu  = IQ_MAX_PU,
    .ramp_pu_s = SC_ACCEL_PU_S,
    .Ts        = TS_SLOW,
};

static if_cfg_t g_if_cfg = {
    .i_mag_pu     = IF_I_MAG_PU,
    .i_ramp_ms    = IF_I_RAMP_MS,
    .i_hold_ms    = IF_I_HOLD_MS,
    .w_accel_pu_s = IF_W_ACCEL_PU_S,
    .Ts           = TS_FAST,
};

static smo_cfg_t g_smo_cfg = {
    .k_slide  = SMO_K_SLIDE,
    .sig_a    = SMO_SIG_A,
    .bw_pll   = SMO_BW_PLL_RAD_S,
    .zeta_pll = SMO_ZETA_PLL,
    .Ts       = TS_FAST,
};

static vf_cfg_t g_vf_cfg = {
    .v_boost_pu = VF_V_BOOST_PU,
    .v_rated_pu = VF_V_RATED_PU,
    .w_ramp_ms  = VF_W_RAMP_MS,
    .Ts         = TS_FAST,
};

static motor_cfg_t g_motor_cfg = {
    .Rs        = MOTOR_RS_OHM,
    .Ld        = MOTOR_LD_H,
    .Lq        = MOTOR_LQ_H,
    .lambda_pm = MOTOR_LAMBDA_PM_WB,
    .npp       = MOTOR_NPP,
    .J         = MOTOR_J_KGM2,
    .B         = MOTOR_B_NMS,
    .Udc       = DC_VOLTAGE_MAX,
};

static base_t g_pu_base = {
    .i_base = I_BASE_A,
    .u_base = U_BASE_V,
    .w_base = W_BASE_HZ * 2.0f * PI_F,
};

void hsm_crit_enter(void)
{
    // nothing to do
}
void hsm_crit_exit(void)
{
    // nothing to do
}

void foc_init(void)
{
    g_mc.cc  = &g_cc;
    g_mc.sc  = &g_sc;
    g_mc.ifg = &g_ifg;
    g_mc.vf  = &g_vf;
    g_mc.ob  = &g_ob;
    smo_bind(&g_ob, &g_smo);

    if_tune(&g_ifg, &g_pu_base, &g_if_cfg);
    vf_tune(&g_vf, &g_pu_base, &g_vf_cfg);
    smo_tune(&g_smo, &g_motor_cfg, &g_pu_base, &g_smo_cfg);
    cc_tune(&g_cc, &g_motor_cfg, &g_pu_base, &g_cc_cfg);
    sc_tune(&g_sc, &g_motor_cfg, &g_pu_base, &g_sc_cfg);
    sc_init(&g_sc);
    conv_tune(&g_mc.conv, &g_motor_cfg, &g_pu_base, CONV_FLOOR_SPD_PU);

    g_mc.tr_enable           = true;
    g_mc.if2foc_handover_spd = (spd_pu_t)(IF_HANDOVER_PU * (float)Q15_ONE);
    g_hsm.mc                 = &g_mc;
    g_hsm.hw                 = port_hw_if(); /* power-stage vtable from the port layer */
    g_hsm_ptr                = &g_hsm;
    foc_hsm_set(EV_TRAN, EV_FIELD_A, ST_IDLE, 0, 0);
}

void foc_1ms_proc(void)
{
    // sc_step or power_step
    mc_slow_step(g_hsm_ptr->mc);

    /* Startup timeout watchdog: while the IF->FOC sequence is still in
     * RESYNC/STARTUP it has FOC_STARTUP_TIMEOUT_MS to reach SENSORLESS_FOC.
     * If it does not hand over within that window, the observer never locked
     * and the run is latched as a startup fault. The counter clears the moment
     * the drive reaches SENSORLESS_FOC or falls back to IDLE. */

#if FAULT_ONE_SHOT_FOC_STARTUP_ERR_ENABLE
    static uint16_t startup_ms = 0;
    if_foc_stage_t  stage      = g_hsm_ptr->mc->if_foc_stage;
    if (stage == RESYNC || stage == STARTUP)
    {
        if (++startup_ms >= FOC_STARTUP_TIMEOUT_MS)
        {
            startup_ms = 0;
            fault_set(FAULT_ID_FOC_STARTUP_ERROR);
        }
    }
    else
    {
        startup_ms = 0;
    }
#endif
}

void foc_poll_proc(uint8_t state)
{
    static ctrl_mode_t pre_ctrl_mode = CTRL_MODE_NONE;
    // machine state
    hsm_run(g_hsm_ptr);

    if (g_hsm_ptr->state == ST_FAULT)
    {
        if (state == CMDBUS_IDLE || state == CMDBUS_RUNNING)
        {
            cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_FAULT};

            cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
        }
    }
    else if (g_hsm_ptr->mc->ctrl_mode == CTRL_MODE_NONE && pre_ctrl_mode >= CTRL_MODE_DUTY)
    {
        // stop by foc internal
        cmdbus_ctrl_t ctrl = {.ctrl_type = CMDBUS_CTRL_STOP};

        cmdbus_post(CMDBUS_CMD_CTRL, &ctrl, sizeof(ctrl));
    }

    pre_ctrl_mode = g_hsm_ptr->mc->ctrl_mode;
}

void foc_isr_proc(mc_in_t *in, volatile duties_t *dabc_pu)
{
    mc_fast_step(g_hsm_ptr->mc, in);

    dabc_pu->a = g_hsm_ptr->mc->act_duty_abc.a;
    dabc_pu->b = g_hsm_ptr->mc->act_duty_abc.b;
    dabc_pu->c = g_hsm_ptr->mc->act_duty_abc.c;
}
