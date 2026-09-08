#include "stsmo.h"
#include "qmath.h"
/* Super-twisting sliding-mode observer (active flux). Shares the current model of smo.c
 * and, by value, the tracking PLL of pll.c -- only the SWITCH differs, so an A/B against
 * smo.c measures the switch alone. See stsmo.h for the derivation and gain sizing.
 *
 * Per-unit surface dynamics behind the sizing rule. With the same current model both
 * for the estimate and the truth,
 *   s[k+1] = F*s[k] - G*(z - e_a),   s = i_est - i_meas,
 * so in continuous time  s' = -alpha*(z - e_a),  alpha = G/Ts = u_base/(Lq*i_base).
 * Substituting the super-twisting law and matching Levant's canonical form gives an
 * effective sqrt-gain alpha*k1 and integral gain alpha*k2 against a disturbance
 * derivative alpha*|de_a/dt| -- the alphas cancel in the integral condition and leave
 *   k2 > C,   k1 > 1.5*sqrt(C/alpha),   C >= |de_a/dt| in pu-V/s.
 *
 * FIXED-POINT FORMATS (the awkward part, all forced by int32 products):
 *   k1     Q12, not Q15: useful k1 exceed 1.0, and k1(Q12,<=32767) * sqrt(Q15,<=32767)
 *          stays under 2^30. Q15 would overflow. Resolution 2.4e-4, range < 8.0.
 *   k2*Ts  Q15, folded at tune time (cf. pi2dof.c folding Ts into ki). The increment
 *          k2_ts(Q15) * sgn(Q15) is then exactly Q30 and <= 2^30.
 *   w      Q30 accumulator per axis, mirroring pll_t.integ: the per-step increment is
 *          tiny (k2*Ts ~ 1e-1 pu at most) so a Q15 state would quantize it away.
 *   sgn    Q15 in [-1, 1], from sat(s/eps) via inv_eps in Q8 -- the same trick smo.c's
 *          sig_a uses to get a >1 multiplier through an int32 product. */

/* Cold-path float->Q15 with saturation (mirrors if.c / smo.c). */
static q15_t to_q15_sat(float x) {
    float v = x * (float)Q15_ONE;
    if(v > (float)Q15_MAX) v = (float)Q15_MAX;
    if(v < (float)Q15_MIN) v = (float)Q15_MIN;
    return (q15_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

void stsmo_tune(stsmo_t *o, const motor_cfg_t *m, const base_t *b,
                const stsmo_cfg_t *c, float Ts) {
    if(!o) return;
    *o = (stsmo_t){0};                             /* clear coefficients + state */
    if(!m || !b || !c || Ts <= 0.0f) return;
    const float L = m->Lq;                         /* active-flux series L = Lq (Ls SPM) */
    if(L <= 0.0f) return;
    o->f_decay = to_q15_sat(1.0f - Ts * m->Rs / L);
    o->g_volt  = to_q15_sat(Ts * b->u_base / (L * b->i_base));
    /* k1 in Q12, clamped to the format's range (see the format note above). */
    float k1 = c->k1 * 4096.0f + 0.5f;
    if(k1 < 0.0f)          k1 = 0.0f;
    if(k1 > 32767.0f)      k1 = 32767.0f;
    o->k1    = (int32_t)k1;
    o->k2_ts = to_q15_sat(c->k2 * Ts);             /* Ts folded in, cf. pi2dof.c's ki */
    /* Smoothed sign: sgn(s) -> sat(s/eps), inv_eps in Q8. eps <= 0 (or so small that
     * 1/eps overflows the format) means the exact sign, which inv_eps == 0 encodes. */
    o->inv_eps = 0;
    if(c->eps > 0.0f) {
        float inv = 256.0f / c->eps;
        o->inv_eps = (inv > 8388608.0f) ? 0 : (int32_t)(inv + 0.5f);   /* 2^23 cap */
    }
    o->pll_on_w = c->pll_on_w;
    pll_tune(&o->pll, b, c->bw_pll, c->zeta_pll, Ts);   /* Stage 2, shared (pll.c) */
}

/* STATE only -- keep control parameters */
void stsmo_init(stsmo_t *o) {
    if(!o) return;
    o->iab_est = (ab_pu_t){0, 0};
    o->eab_est = (ab_pu_t){0, 0};
    o->wab_est = (ab_pu_t){0, 0};
    o->w_afa   = 0;
    o->w_bet   = 0;
    pll_init(&o->pll);            /* Stage-2 state; its coefficients survive */
}

/* sqrt(x_pu) in Q15 == isqrt(x_q15 << 15): with x_q15 = x*2^15, sqrt(x)*2^15 =
 * sqrt(x_q15 * 2^15). The argument is at most 2^30, so the classic restoring bit-by-bit
 * integer sqrt converges in 16 iterations and the result fits Q15 exactly. Exact
 * (floor), branch-light, no table -- see the rationale in stsmo.h. */
q15_t stsmo_sqrt_pu(q15_t x) {
    if(x <= 0) return 0;
    return isqrt_q30((uint32_t)x << 15);   /* the loop lives in qmath.c; it clamps to Q15_MAX */
}

/* sgn(s) in Q15: the exact sign when inv_eps == 0, else the saturated ramp sat(s/eps)
 * that softens the switch inside a boundary layer of +-eps. Unlike smo.c's sigmoid this
 * introduces NO steady-state bias -- the integral term still drives s to zero; it only
 * limits how hard the discontinuity is hit between samples. */
static int32_t sgn_q15(const stsmo_t *o, int32_t s) {
    if(o->inv_eps == 0) return s > 0 ? Q15_MAX : (s < 0 ? -Q15_MAX : 0);
    int32_t g = (s * o->inv_eps) >> 8;             /* s(Q15) * (1/eps)(Q8) >> 8 = Q15 */
    if(g >  Q15_MAX) g =  Q15_MAX;
    if(g < -Q15_MAX) g = -Q15_MAX;
    return g;
}

/* One axis of Stage 1: surface -> super-twisting switch. Updates the axis' Q30 integral
 * state in place and returns z (Q15 pu volts), with w (Q15) out through w_q15. */
static q15_t st_switch(const stsmo_t *o, int32_t s, q31_t *w_acc, q15_t *w_q15) {
    if(s >  Q15_ONE) s =  Q15_ONE;                 /* keep |s| <= 1.0 pu: sqrt and the */
    if(s < -Q15_ONE) s = -Q15_ONE;                 /* Q8 sgn multiply both assume it   */
    int32_t sgn = sgn_q15(o, s);
    int32_t as  = s < 0 ? -s : s;
    if(as > Q15_MAX) as = Q15_MAX;                 /* sqrt takes a q15_t */
    /* z = k1*sqrt(|s|)*sgn(s) + w. k1 is Q12 so the first product is Q27 (<= 2^30);
     * >>12 lands back in Q15, then the sgn multiply is Q30 -> >>15. */
    int32_t root = stsmo_sqrt_pu((q15_t)as);
    int32_t term = ((o->k1 * root) >> 12);         /* k1*sqrt(|s|), Q15, may exceed 1.0 */
    int32_t z    = ((term * sgn) >> 15) + (int32_t)(*w_acc >> 15);
    /* w integrates the sign: increment k2_ts(Q15) * sgn(Q15) is exactly Q30. Clamped
     * like pll_t.integ so w stays inside Q15 when read out. */
    *w_acc += (q31_t)o->k2_ts * sgn;
    q31_t w_max = Q31_MAX, w_min = Q31_MIN;
    if(*w_acc > w_max) *w_acc = w_max;
    if(*w_acc < w_min) *w_acc = w_min;
    *w_q15 = (q15_t)(*w_acc >> 15);
    /* z is a pu voltage handed to a Q15 consumer (the Park in pll.c), so clamp it. */
    if(z >  Q15_MAX) z =  Q15_MAX;
    if(z < -Q15_MAX) z = -Q15_MAX;
    return (q15_t)z;
}

angle_t stsmo_step(stsmo_t *o, ab_pu_t iab_meas, ab_pu_t uab_ref_lim) {
    if(!o) return 0;
    /* Stage 1: sliding surface (estimated - measured) -> super-twisting switch. Both
     * z and w converge to the active-flux EMF; keep both, they are the comparison. */
    q15_t z_afa = st_switch(o, (int32_t)o->iab_est.afa - iab_meas.afa, &o->w_afa, &o->wab_est.afa);
    q15_t z_bet = st_switch(o, (int32_t)o->iab_est.bet - iab_meas.bet, &o->w_bet, &o->wab_est.bet);
    o->eab_est.afa = z_afa;
    o->eab_est.bet = z_bet;
    /* current model: i_est = F*i_est + G*(v - z) -- identical to smo.c, which is the
     * point: the two observers differ ONLY in how z is formed. */
    o->iab_est.afa = (q15_t)((((int32_t)o->f_decay * o->iab_est.afa) >> 15)
                        + (((int32_t)o->g_volt * (uab_ref_lim.afa - z_afa)) >> 15));
    o->iab_est.bet = (q15_t)((((int32_t)o->f_decay * o->iab_est.bet) >> 15)
                        + (((int32_t)o->g_volt * (uab_ref_lim.bet - z_bet)) >> 15));

    /* --- Stage 2: the SHARED tracking PLL (pll.c) -- byte-identical to smo.c's ----
     * z is already continuous (the discontinuity lives in dw/dt), so it feeds the loop
     * with no equivalent-control LPF, exactly as smo.c's sigmoid output does. w is the
     * smoother tap of the two; pll_on_w picks which one Stage 2 tracks. */
    return pll_step(&o->pll, o->pll_on_w ? o->wab_est : o->eab_est);
}

/* --- generic observer interface (ob.h) adapters + bind (mirrors smo.c) ----------- */
/* Mirrors smo.c: the step publishes all four, so no slot can be left unbound.
 *
 * THAT USED TO BE POSSIBLE, and it bit here. When the vtable had separate getters, emf_sq and
 * theta_err were missing from this bind -- silently, because ob.c's dispatchers were NULL-safe
 * and returned 0. A stsmo bound as PRIMARY then fed conv_step(0, 0) every tick: "no EMF"
 * (mag_tick decays, RESYNC refuses forever) and, worse, "zero phase error" (lock_tick earns
 * full credit for an angle nobody reported). Fail-closed on one condition and fail-OPEN on the
 * other. One struct filled by one producer is what makes that unwritable, not just fixed. */
static void step_vt(void *state, ab_pu_t iab, ab_pu_t v, ob_out_t *out) {
    stsmo_t *o = (stsmo_t *)state;
    out->theta_est = stsmo_step(o, iab, v);
    if(!o) { out->spd_est = 0; out->emf_est_sq = 0; out->theta_err = 0; return; }
    out->spd_est       = o->pll.spd_est;
    out->emf_est_sq    = o->pll.e_mag_sq;   /* before the PLL_E_MIN floor (pll.h), as smo's is */
    out->theta_err = o->pll.theta_err;
}
static void init_vt(void *state) { stsmo_init(state); }

void stsmo_bind(ob_t *ob, stsmo_t *o) {
    if(!ob) return;
    ob->step  = step_vt;
    ob->init  = init_vt;
    ob->state = o;
}
