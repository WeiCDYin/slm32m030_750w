#include "if.h"
/* 
  * Scaling:
 *   theta Q32 (full circle == 2^32), output angle_t = top 16 bits.
 *   spd_acc / i_acc are Q15.16 (per-unit Q15 value << 16) so ramp increments
 *   far below 1 Q15 LSB per step accumulate without stalling.
 *   d(theta)/step = spd_q15 * k_theta,  k_theta = w_base*Ts/(2pi) * 2^17.
 *   k_theta is clamped to 2^16-1, so |spd_q15 * k_theta| < 2^31 cannot overflow
 *   (that clamp is the |w|*Ts < pi limit from the header). */

static const float TWO_PI = 6.2831853071795865f;

/* The startup angle, in Q32 counts (DEG_Q32 is types.h; constant-folded, no 64-bit maths runs). */
#define NEG_90_DEG    DEG_Q32(-90)

/* Cold-path float->Q15 with saturation (mirrors smo.c / vf.c). */
static q15_t to_q15_sat(float x) {
    float v = x * (float)Q15_ONE;
    if(v > (float)Q15_MAX) v = (float)Q15_MAX;
    if(v < (float)Q15_MIN) v = (float)Q15_MIN;
    return (q15_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

/* Round a per-step increment to int32, clamped to [1, hi]: a nonzero configured
 * rate must always progress, and products/sums must stay inside int32. */
static int32_t to_inc_sat(float v, int32_t hi) {
    if(v < 1.0f) return 1;
    if(v > (float)hi) return hi;
    return (int32_t)(v + 0.5f);
}

/* Reset the RUNTIME ramp state to the startup point: current 0, frequency 0, alignment
 * hold not yet counted, and the generated angle at the -90 deg start so the initial current
 * vector lands on the forced d-axis (alignment). Keeps the derived coefficients and the
 * offset back to zero and the current magnitude back to the CONFIGURED one -- the config-less
 * runtime half of if_tune. Called on every I-f entry (hsm on_entry_if /
 * on_entry_if_foc) so re-entry restarts the trajectory from standstill instead of resuming a stale
 * ramp. NULL-safe. theta is Q32 (full circle == 2^32); -90 deg == 3/4 turn.
 *
 * The cached SPEED target goes to 0 rather than to some configured value: the generator does not own
 * a speed target, it is handed one every tick (if.h), so there is nothing to restore and no way to
 * inherit where the previous run happened to be going.
 *
 * The CURRENT target is the opposite case -- the generator DOES own it, and if_set_cv_mag writes it
 * live. Restoring it from i_tgt_cfg is what keeps a static-vector session (ST_CV, which shares this
 * same if_t) from leaving the STARTUP ramp aiming at a diagnostic magnitude: CV at 0.10 pu followed
 * by ST_IF_FOC used to align and ramp at a fifth of the configured current -- with d_i still sized
 * for the full one, so the ramp also finished early -- and stayed that way until the next if_tune,
 * i.e. until reboot. vf_init restores v_boost from v_boost_cfg for exactly this reason.
 *
 * theta_offset is the SAME defect with a simpler answer: zero IS its configured value (if_tune sets
 * it and nothing else ever does), so there is nothing to shadow -- clearing it here is the whole
 * restore. Only the static-vector leaf wants it non-zero; plain I-f and IF_FOC integrate from the
 * -90 deg start and want no constant rotation on top. Live rotation goes through if_set_theta_offset
 * and never through here, so keeping it across a reset only ever served RE-ENTRY -- which is exactly
 * the case that must not inherit, because entering a mode is not a command. */
void if_init(if_t *g) {
    if(!g) return;
    g->spd_acc = 0; g->i_acc = 0; g->hold_cnt = 0;
    g->spd_tgt = 0;
    g->i_tgt = g->i_tgt_cfg;
    g->theta = NEG_90_DEG;   /* initial angle of IF reference frame */
    g->theta_offset = 0;
}

void if_tune(if_t *g, const base_t *b, const if_cfg_t *c, float Ts) {
    if(!g) return;
    g->theta =0; g->spd_acc = 0; g->i_acc = 0; g->hold_cnt = 0;
    g->k_theta = 0; g->d_spd = 0; g->d_i = 0;
    g->hold_steps = 0; g->spd_tgt = 0; g->i_tgt = 0; g->i_tgt_cfg = 0; g->theta_offset = 0;
    if(!b || !c || Ts <= 0.0f) return;   /* config-less: inert, angle stays 0 */
    g->k_theta    = to_inc_sat(b->w_base * Ts / TWO_PI * PU_ONE_Q32, 65535); /* 2^17 */
    /* The rate is already per-unit of w_base per second, so the speed math needs no w_base and
     * no target -- unlike the current ramp below, which is still quoted as a time to a magnitude. */
    float accel   = c->w_accel_pu_s < 0.0f ? -c->w_accel_pu_s : c->w_accel_pu_s;
    g->d_spd      = to_inc_sat(accel * Ts * PU_ONE_Q15_16, 1 << 30);
    /* i_mag_pu is already per-unit of i_base; i_ramp_ms is the 0 -> i_mag_pu ramp
     * time, so rate = i_mag_pu / i_ramp [pu/s] -- no i_base needed. */
    float i_mag   = c->i_mag_pu < 0.0f ? -c->i_mag_pu : c->i_mag_pu;
    float i_rate  = c->i_ramp_ms > 0.0f ? i_mag * 1000.0f / c->i_ramp_ms : 0.0f;
    g->d_i        = to_inc_sat(i_rate * Ts * PU_ONE_Q15_16, 1 << 30);
    g->i_tgt_cfg  = to_q15_sat(c->i_mag_pu);   /* the live i_tgt follows, via the if_init below */
    g->hold_steps = (uint32_t)(c->i_hold_ms * 0.001f / Ts + 0.5f);
    if_init(g);   /* set the runtime startup point (-90 deg, ramps at 0, i_tgt <- cfg) */
}

static int32_t ramp_to(int32_t acc, int32_t tgt, int32_t inc) {
    if(acc < tgt) { acc += inc; if(acc > tgt) acc = tgt; }
    else if(acc > tgt) { acc -= inc; if(acc < tgt) acc = tgt; }
    return acc;
}

polar_pu_t if_step(if_t *g, spd_pu_t spd_tgt) {
    polar_pu_t cur_vec = { 0, 0 };
    if(!g) return cur_vec;
    g->spd_tgt = spd_tgt;            /* cache this tick's target so if_ss_reached can answer on it */
    const int32_t i_tgt = (int32_t)g->i_tgt * Q15_16_SCALE;   /* multiply, not << : signed (types.h) */
    if(g->i_acc != i_tgt) {
        g->i_acc = ramp_to(g->i_acc, i_tgt, g->d_i);
    }else if(g->hold_cnt < g->hold_steps) {
        g->hold_cnt++;
    }else{
        g->spd_acc = ramp_to(g->spd_acc, (int32_t)g->spd_tgt * Q15_16_SCALE, g->d_spd);
    }
    g->theta +=(uint32_t)((g->spd_acc >> Q15_16_SHIFT) * g->k_theta);
    cur_vec.mag   = (q15_t)(g->i_acc >> Q15_16_SHIFT);
    cur_vec.angle = (angle_t)((g->theta >> ANGLE_SHIFT) + g->theta_offset);
    return cur_vec;
}

/* CV built on top of IF. No angle half: if_init already starts at -90, which both I-f leaves want.
 * The zero matters -- if_init restores the STARTUP current (0.50 pu == 1.5 A on BL4260), which would
 * otherwise park in a stationary winding on entry. */
void cv_init(if_t *g) {
    if_init(g);
    if_set_cv_mag(g, 0);         /* ENTERING IS NOT COMMANDING: the vector starts at zero amps */
}

void if_set_theta_offset(if_t *g, angle_t offset) {
    if(g) g->theta_offset = offset;
}

void if_set_cv_mag(if_t *g, q15_t i_pu) {
    if(g) g->i_tgt = i_pu;
}

dq_pu_t if_get_idq_ref(const if_t *g) {
    dq_pu_t r = { 0, 0 };
    if(g) r.q = (q15_t)(g->i_acc >> Q15_16_SHIFT);
    return r;
}

angle_t if_get_theta(const if_t *g) {
    return g ? (angle_t)((g->theta >> ANGLE_SHIFT) + g->theta_offset) : 0;
}

spd_pu_t if_get_spd(const if_t *g) {
    return g ? (spd_pu_t)(g->spd_acc >> Q15_16_SHIFT) : 0;
}

bool if_ss_reached(const if_t *g) {
    if(!g) return false;
    return g->i_acc == ((int32_t)g->i_tgt * Q15_16_SCALE)
        && g->spd_acc == ((int32_t)g->spd_tgt * Q15_16_SCALE);
}
