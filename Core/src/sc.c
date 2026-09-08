#include "sc.h"
/* Speed controller: the MECHANICAL-plant tuning of pi2dof (the regulator math lives there,
 * shared with cc). See sc.h for the contract. */

void sc_init(sc_t *sc) {
    if(!sc) return;
    pi2dof_init(&sc->pi);
}

/* Cold-path float -> Q15 gain in int32 (no cap at 1.0; the effective gain may exceed unity).
 * Defensively saturated to int32 range, which realistic gains never approach. */
#define GAIN_Q15_MAX ((float)((int32_t)65535 << 15))    /* 65535.0 pu; see cc.c */
static int32_t gain_q15(float x) {
    float v = x * (float)Q15_ONE;
    if(v >  GAIN_Q15_MAX) v =  GAIN_Q15_MAX;
    if(v < -GAIN_Q15_MAX) v = -GAIN_Q15_MAX;
    return (int32_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}
/* Anti-windup weight, saturated below 1.0 -- mirrors cc.c. */
static int32_t aw_q15(float x) {
    int32_t v = gain_q15(x);
    if(v >  Q15_ONE) v =  Q15_ONE;
    if(v < -Q15_ONE) v = -Q15_ONE;
    return v;
}
/* Cold-path float -> Q15 with saturation (for the current-limit clamp; mirrors cc.c). */
static q15_t to_q15_sat(float x) {
    float v = x * (float)Q15_ONE;
    if(v > (float)Q15_MAX) v = (float)Q15_MAX;
    if(v < (float)Q15_MIN) v = (float)Q15_MIN;
    return (q15_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

void sc_tune(sc_t *sc, const motor_cfg_t *m, const base_t *b, float alpha_s, float i_max_pu, float Ts) {
    if(!sc) return;
    sc->pi = (pi2dof_t){0};                                  /* inert until proven tunable */
    sc->i_max = 0;                                            /* clamp 0 -> no current */
    if(!m || !b || i_max_pu <= 0.0f || alpha_s <= 0.0f) return;
    float k = 1.5f * (float)m->npp * (float)m->npp * m->lambda_pm;    /* pp*Kt = 1.5*pp^2*lam (w_e torque gain) */
    if(k <= 0.0f || m->J <= 0.0f) return;                     /* no plant -> inert */
    float g  = b->w_base / b->i_base;                         /* SI gain -> pu (w_pu in, iq_pu out) */
    float kp = (2.0f * alpha_s * m->J - m->B) / k;            /* state feedback: k_p = 2*alpha_s*J - B */
    float ki = alpha_s * alpha_s * m->J / k;                  /* integral:       k_i = alpha_s^2 * J   */
    float kt = alpha_s * m->J / k;                            /* reference FF:   k_t = alpha_s*J        */
    float kb = (kt > 0.0f) ? ki / kt : 0.0f;                  /* realizable-form anti-windup: k_i/k_t = alpha_s (T_t = k_t/k_i = 1/alpha_s) */
    sc->pi.kp      = gain_q15(kp * g);
    sc->pi.ki      = gain_q15(ki * g * Ts);                   /* Ts folded in (pi2dof.c convention) */
    sc->pi.kt      = gain_q15(kt * g);
    sc->pi.k_aw    = aw_q15(kb * Ts);                         /* per-step anti-windup weight (dimensionless) */
    sc->i_max      = to_q15_sat(i_max_pu);                    /* sc's own limit; pi2dof never clamps */
}

void sc_trans_set_I(sc_t *sc, q15_t iq, spd_pu_t w_ref, spd_pu_t w_meas) {
    if(!sc) return;
    pi2dof_set_I(&sc->pi, iq, w_ref, w_meas);
}

/* The regulator asks; sc decides. Compute the ask, clamp it to the current limit, then feed the
 * clamp back SAME tick so the anti-windup drains against the limit that really happened
 * (pi2dof.h). */
q15_t sc_step(sc_t *sc, spd_pu_t w_ref, spd_pu_t w_meas) {
    if(!sc) return 0;
    int32_t ask = pi2dof_output(&sc->pi, w_ref, w_meas);
    if(ask >  sc->i_max) ask =  sc->i_max;
    if(ask < -sc->i_max) ask = -sc->i_max;
    q15_t   lim = (q15_t)ask;
    pi2dof_update_I(&sc->pi, w_ref, w_meas, lim);
    return lim;
}
