#include "smo.h"
/* Active-flux sliding-mode observer + tracking PLL. Unifies SPM and IPM: the active
 * flux  λ_a = λ_pm + (Ld-Lq)*i_d  is ALWAYS aligned to the rotor d-axis, so its
 * angle is θ regardless of saliency (Boldea). From λ_a = λ_s - Lq*i_s,
 *   dλ_a/dt = v - Rs*i - Lq*di/dt,
 * so the observer's SERIES INDUCTANCE is Lq (not Ld) and all saliency folds into λ_a.
 * smo_tune (cold, float) derives the Q15/Q31 coefficients from the SI machine + bases;
 * the hot path (smo_step) is integer-only. See smo.h and test_smo.cpp. The core never
 * calls these directly -- it goes through the generic ob_t interface (mc/ob.h) that
 * smo_bind fills.
 *
 * Per-unit current model (stationary frame, per axis), forward-Euler, L = Lq:
 *   Lq di/dt = v - Rs i - e_a   ->   i_est[k+1] = F*i_est[k] + G*(v_pu - z)
 *     F = 1 - Ts*Rs/Lq                  (estimated-current decay, dimensionless)
 *     G = Ts*u_base/(Lq*i_base)         (pu-voltage -> pu-current increment)
 *   z = k_slide * sigma(a*(i_est - i))  (smooth sigmoid switch; sliding => z -> e_a)
 * In sliding mode z equals the active-flux EMF e_a = ω*λ_a*[-sinθ, cosθ] (points at
 * θ+90). The sigmoid output is chatter-free, so it IS e_a directly -- no equivalent-
 * control LPF (the PLL bandwidth filters, without an LPF's speed-dependent phase lag).
 * Only the DIRECTION of z is used (the PLL locks to it), so ESTIMATING needs NO flux-magnitude
 * params -- lam and saliency never enter the angle or the speed, and the |e|-normalized phase
 * detector divides by the MEASURED |z|, still not by lam. The one place lam appears is the
 * expected_emf_mag_sq diagnostic (|e| = lam_a*|we|), which cannot be posed without the machine.
 * Keep the distinction -- k_emf is read by nothing on the estimation path, and by nothing that
 * decides anything.
 * Stage 2 is NOT here: the tracking PLL lives in pll.c and is embedded by value, because
 * stsmo.c (super-twisting) runs the IDENTICAL loop -- so an A/B between the two observers
 * differs only in the Stage-1 switch. See pll.h for the loop and the Harnefors tap.
 *
 * SPM is just Ld == Lq: λ_a = λ_pm const, series L = Lq = Ls. */

/* Cold-path float->Q15 with saturation (mirrors if.c). */
// actually, saturation should raise error
static q15_t to_q15_sat(float x) {
    float v = x * (float)Q15_ONE;
    if(v > (float)Q15_MAX) v = (float)Q15_MAX;
    if(v < (float)Q15_MIN) v = (float)Q15_MIN;
    return (q15_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

void smo_tune(smo_t *smo, const motor_cfg_t *m, const base_t *b,
              const smo_cfg_t *c, float Ts) {
    if(!smo) return;
    *smo = (smo_t){0};                             /* clear coefficients + state */
    if(!m || !b || !c || Ts <= 0.0f) return;
    const float L = m->Lq;                         /* active-flux series L = Lq (Ls SPM) */
    if(L <= 0.0f) return;
    smo->f_decay = to_q15_sat(1.0f - Ts * m->Rs / L);
    smo->g_volt  = to_q15_sat(Ts * b->u_base / (L * b->i_base));
    smo->k_slide = to_q15_sat(c->k_slide);
    /* The diagnostic pair's one machine constant (smo.h). lam_pm, not lam_a: the saliency
     * term is (Ld-Lq)*i_d and the only place this is read, RESYNC, holds i_d at zero. */
    if(b->u_base > 0.0f) smo->k_emf = to_q15_sat(m->lambda_pm * b->w_base / b->u_base);
    smo->sig_a   = (int32_t)(c->sig_a * 256.0f + 0.5f);          /* steepness, Q8 */
    pll_tune(&smo->pll, b, c->bw_pll, c->zeta_pll, Ts);   /* Stage 2, shared (pll.c) */
}

 /* STATE only -- keep control parameters */
void smo_init(smo_t *smo) {
    if(!smo) return;
    smo->iab_est = (ab_pu_t){0, 0};
    smo->eab_est = (ab_pu_t){0, 0};
    smo->calculated_emf_mag_sq = 0;
    smo->expected_emf_mag_sq   = 0;
    pll_init(&smo->pll);          /* Stage-2 state; its coefficients survive */
}

/* Sigmoid sigma(x) = tanh(x/2) = 2/(1+e^-x) - 1, odd, via a Q15 LUT over x in [0, 8]
 * in 64 steps of 0.125 (|x| >= 8 -> +-1). x is passed in Q15 (1.0 == 2^15), so the
 * step is 4096 == 2^12: idx = |x| >> 12, frac = |x| & 0xFFF, linear interp. Avoids a
 * hot-path divide/exp; mirrors the sine LUT in trans.c. */
static const q15_t SIG[65] = {
         0,   2045,   4075,   6073,   8025,   9919,  11742,  13486,
     15142,  16706,  18173,  19541,  20812,  21986,  23065,  24053,
     24955,  25775,  26518,  27190,  27796,  28340,  28829,  29267,
     29659,  30009,  30321,  30599,  30846,  31066,  31261,  31435,
     31588,  31725,  31845,  31952,  32047,  32131,  32205,  32270,
     32328,  32380,  32425,  32465,  32500,  32531,  32559,  32583,
     32605,  32624,  32641,  32656,  32669,  32680,  32690,  32699,
     32707,  32714,  32720,  32726,  32731,  32735,  32739,  32742,
     32745,
};
static q15_t sigmoid_q15(int32_t x) {
    int32_t s = x < 0 ? -x : x;
    if(s >= (8 << 15)) return x < 0 ? (q15_t)-Q15_MAX : Q15_MAX;   /* saturated */
    int32_t i = s >> 12, frac = s & 0xFFF;                        /* idx 0..63, frac 0..4095 */
    q15_t v = (q15_t)(SIG[i] + (((int32_t)(SIG[i + 1] - SIG[i]) * frac) >> 12));
    return x < 0 ? (q15_t)-v : v;
}
/* Switching term z = k_slide * sigma(a * s) for sliding surface s = i_est - i. s is
 * clamped to +-1.0 pu before the multiply (the sigmoid is fully saturated there) so
 * sig_a(Q8) * s stays inside int32 for any steepness a < 256. */
static q15_t sig_switch(const smo_t *smo, int32_t s) {
    if(s >  Q15_ONE) s =  Q15_ONE;
    if(s < -Q15_ONE) s = -Q15_ONE;
    int32_t arg = ((int32_t)smo->sig_a * s) >> 8;                 /* a(Q8)*s(Q15) >> 8 = Q15 */
    return (q15_t)(((int32_t)smo->k_slide * sigmoid_q15(arg)) >> 15);
}

/* The two EMF amplitudes side by side (smo.h): what Stage 1 MEASURED, and what Stage 2's
 * integrated speed IMPLIES it must be through the machine's lambda. TELEMETRY -- nothing decides
 * on them. They WERE the convergence test, and why they stopped being it is why the pair is
 * still worth publishing: their agreement is |e| = lam_a*|we|, which is FREQUENCY evidence and
 * therefore blind to the 180 deg root, while the handover needs the ANGLE (conv.h). Kept because
 * they are how a suspect estimate is diagnosed on the bench -- which of the two moved says
 * whether it is Stage 1, Stage 2, or the machine constant. One multiply.
 *
 * Reading them: z settles a little UNDER the real EMF (the sigmoid switch is not an exact
 * equivalent-control estimator) and that bias is SYSTEMATIC -- 3% on REF, 5-6% on BL4260, and no
 * averaging removes it. Above ~0.84*k_slide stop comparing them at all: |z| is bounded by the
 * sliding gain by construction (sig_switch), so as the machine's EMF approaches it the amplitude
 * rails and the gap against truth runs 7.4% at 0.99*k_slide, 12% at 1.10, 43% at 1.20 -- the
 * MEASUREMENT failing, not the estimate. The PLL uses only the DIRECTION of z, so the angle is
 * still good up there. */
static void report_emf(smo_t *smo) {
    int32_t w = pll_get_spd(&smo->pll);
    if(w < 0) w = -w;
    smo->calculated_emf_mag_sq = smo->pll.e_mag_sq;                   /* Stage 1 measured */
    /* Squared to match, so the pair stays comparable (smo.h). k_emf*|w| is a Q15 pu magnitude
     * bounded by Q15_MAX, so its square is at most 2^30 and stays well inside uint32. */
    uint32_t ex = (uint32_t)(((int32_t)smo->k_emf * w) >> 15);         /* Stage 2 implies  */
    smo->expected_emf_mag_sq   = ex * ex;
}

angle_t smo_step(smo_t *smo, ab_pu_t iab_meas, ab_pu_t uab_ref_lim) {
    if(!smo) return 0;
    /* sliding surface (estimated - measured) -> sigmoid switch == active-flux EMF z */
    q15_t z_afa = sig_switch(smo, (int32_t)smo->iab_est.afa - iab_meas.afa);
    q15_t z_bet = sig_switch(smo, (int32_t)smo->iab_est.bet - iab_meas.bet);
    smo->eab_est.afa = z_afa;
    smo->eab_est.bet = z_bet;
    /* current model: i_est = F*i_est + G*(v - z). pu bounds (|i_est|,|v|,|z| < 1) keep each
     * product inside int32 (cf. pi2dof.c relying on Q15-clamped operands). */
    smo->iab_est.afa = (q15_t)((((int32_t)smo->f_decay * smo->iab_est.afa) >> 15)
                          + (((int32_t)smo->g_volt * (uab_ref_lim.afa - z_afa)) >> 15));
    smo->iab_est.bet = (q15_t)((((int32_t)smo->f_decay * smo->iab_est.bet) >> 15)
                          + (((int32_t)smo->g_volt * (uab_ref_lim.bet - z_bet)) >> 15));

    /* --- Stage 2: the SHARED tracking PLL (pll.c) ----------------------------
     * z IS the EMF estimate (chatter-free, no equivalent-control LPF), so it feeds the
     * loop directly. The phase detector, the Harnefors speed tap and the angle
     * accumulator all live in pll.c now -- identical code to stsmo.c's Stage 2. */
    angle_t theta = pll_step(&smo->pll, smo->eab_est);
    report_emf(smo);               /* AFTER the loop moves: both stages are this tick's */
    return theta;
}

/* --- generic observer interface (mc/ob.h) adapters + bind --------------------
 * Thin void*-typed shims so the smo_* impl plugs into the ob_t vtable; the implicit
 * void* -> smo_t* conversion is all that differs from the public entry points. */
/* One producer, one struct: the step publishes everything this tick produced, so nothing
 * downstream dispatches to fetch a field (ob.h). The four values are all in hand here --
 * pll_step has just run and report_emf just latched the amplitude. */
static void step_vt(void *state, ab_pu_t iab, ab_pu_t v, ob_out_t *out) {
    smo_t *s = (smo_t *)state;
    out->theta_est = smo_step(s, iab, v);
    if(!s) { out->spd_est = 0; out->emf_est_sq = 0; out->theta_err = 0; return; }
    out->spd_est       = s->pll.spd_est;
    out->emf_est_sq    = s->calculated_emf_mag_sq;
    out->theta_err = s->pll.theta_err;
}
static void init_vt(void *state) { smo_init(state); }

void smo_bind(ob_t *ob, smo_t *smo) {
    if(!ob) return;
    ob->step  = step_vt;
    ob->init  = init_vt;
    ob->state = smo;
}
