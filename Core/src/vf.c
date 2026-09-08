#include "vf.h"
/* V/f scalar generator. Hot path is integer-only; see vf.h for the contract
 * and test_vf.cpp for the validated behavior. Scaling mirrors if_ref_gen:
 *   theta Q32 (full circle == 2^32), output angle_t = top 16 bits.
 *   spd_acc is Q15.16 (per-unit Q15 << 16) so a sub-LSB accel still progresses.
 *   d(theta)/step = spd_q15 * k_theta,  k_theta = w_base*Ts/(2pi) * 2^17,
 *   clamped to 2^16-1 (the |w|*Ts < pi limit). */

static const float TWO_PI = 6.2831853071795865f;

/* The STATIC VECTOR's start angle, in Q32 counts (DEG_Q32 is types.h; constant-folded, no 64-bit
 * maths runs). prod_vf puts the voltage on the CONTROLLER Q-AXIS ({.d = 0, .q = mag}, mc.c), so the
 * stator vector points at theta + 90 deg; starting at -90 lands it on the phase-a axis, and a
 * commanded angle then means what it says. Same constant, same reason, as if.c -- the difference is
 * that if.c's serves BOTH its leaves, while here only VV wants it (below). */
#define NEG_90_DEG    DEG_Q32(-90)

/* Cold-path float->Q15 with saturation (mirrors if.c). */
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

/* Reset the RUNTIME state to standstill: working speed 0, angle accumulator 0, the curve's boost
 * back to the CONFIGURED one, and the angle offset back to zero. Keeps the derived coefficients --
 * the config-less runtime half of vf_tune. Called on VF entry (hsm on_entry_vf) so entry starts a
 * clean ramp from standstill instead of resuming a stale working speed (which for the static
 * voltage-vector tool would otherwise let the angle drift), on a clean CURVE rather than the
 * magnitude some earlier static-vector session left in v_boost, and on the BARE angle rather than
 * one that session left rotated. NULL-safe.
 *
 * The two overrides need different mechanisms only because the boost HAS a configured value to go
 * back to (v_boost_cfg); the offset's configured value is zero, so clearing it is the whole restore.
 * if_init is the same pair, for the same reason. */
void vf_init(vf_t *g) {
    if(!g) return;
    g->theta   = 0;   /* the RAMP's start is arbitrary; VV overrides it via vf_set_init_theta */
    g->spd_acc = 0;
    g->v_boost = g->v_boost_cfg;
    g->theta_offset = 0;
}

void vf_tune(vf_t *g, const base_t *b, const vf_cfg_t *c, float Ts) {
    if(!g) return;
    g->k_theta = 0; g->d_spd = 0; g->v_boost_cfg = 0; g->k_v = 0;
    g->theta_offset = 0;
    vf_init(g);   /* runtime: angle 0, working speed 0, boost <- cfg */
    if(!b || !c || Ts <= 0.0f) return;
    g->k_theta = to_inc_sat(b->w_base * Ts / TWO_PI * PU_ONE_Q32, 65535); /* 2^17 */
    /* accel as full-pu-per-second: w_ramp_ms is the time to slew across 1.0 pu. */
    float accel = c->w_ramp_ms > 0.0f ? 1000.0f / c->w_ramp_ms : 0.0f;
    g->d_spd  = to_inc_sat(accel * Ts * PU_ONE_Q15_16, 1 << 30);
    g->v_boost_cfg = to_q15_sat(c->v_boost_pu);
    g->v_boost     = g->v_boost_cfg;                 /* live starts at the configured curve */
    g->k_v         = (int32_t)to_q15_sat(c->v_rated_pu) - (int32_t)g->v_boost_cfg;
}

void vf_set_theta_offset(vf_t *g, angle_t offset) {
    if(g) g->theta_offset = offset;
}

void vf_set_vv_mag(vf_t *g, q15_t v_pu) {
    if(g) g->v_boost = v_pu;                   /* == the curve output at |w| == 0. LIVE only:
                                                * v_boost_cfg is untouched, so the next vf_init
                                                * puts the configured curve back (vf.h). */
}

static int32_t ramp_to(int32_t acc, int32_t tgt, int32_t inc) {
    if(acc < tgt) { acc += inc; if(acc > tgt) acc = tgt; }
    else if(acc > tgt) { acc -= inc; if(acc < tgt) acc = tgt; }
    return acc;
}

polar_pu_t vf_step(vf_t *g, spd_pu_t spd_tgt) {
    polar_pu_t v = { 0, 0 };
    if(!g) return v;
    g->spd_acc = ramp_to(g->spd_acc, (int32_t)spd_tgt * Q15_16_SCALE, g->d_spd);  /* not << : signed (types.h) */
    g->theta +=(uint32_t)((g->spd_acc >> Q15_16_SHIFT) * g->k_theta);
    v.angle = (angle_t)((g->theta >> ANGLE_SHIFT) + g->theta_offset);
    int32_t w_q15 = g->spd_acc >> Q15_16_SHIFT;                  /* signed Q15 working speed */
    int32_t w_abs = w_q15 < 0 ? -w_q15 : w_q15;
    if(w_abs > Q15_ONE) w_abs = Q15_ONE;                         /* clamps at 1 pu */
    v.mag = (q15_t)(g->v_boost + ((g->k_v * w_abs) >> 15));
    return v;
}

spd_pu_t vf_get_spd(const vf_t *g) {
    return g ? (spd_pu_t)(g->spd_acc >> Q15_16_SHIFT) : 0;
}

/* VV built on top of VF */
void vv_init(vf_t *g) {
    vf_init(g);                  /* NULL-safe; everything else about the reset is identical */
    if(g) g->theta = NEG_90_DEG;
    vf_set_vv_mag(g, 0);         /* ENTERING IS NOT COMMANDING: the vector starts at zero volts */
}
