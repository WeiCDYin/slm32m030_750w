#ifndef MC_STSMO_H
#define MC_STSMO_H
#include "types.h"
#include "base.h"
#include "motor_cfg.h"
#include "ob.h"          /* ob_t: the generic observer interface stsmo_bind fills */
#include "pll.h"         /* pll_t: the SAME Stage-2 tracking loop smo.h embeds */
#ifdef __cplusplus
extern "C" {
#endif
/* SUPER-TWISTING sliding-mode observer (active flux) -- a second implementation of the
 * generic observer interface (ob.h), sharing Stage 1's active-flux model and Stage 2's
 * tracking PLL with smo.h. Only the SWITCH differs, which is the whole point: bind both
 * through ob_tee.h and the difference you measure is the switch, nothing else.
 *
 * WHY, vs the first-order sigmoid switch in smo.h:
 *   - smo.h needs k_slide > max|e_a|, so at low speed it injects many times the actual
 *     EMF. Here the discontinuity sits in the INTEGRAL, so the gains bound |de_a/dt|,
 *     not |e_a| -- one gain set covers standstill to rated.
 *     MEASURED on MACHINE_REF at 20 kHz, one gain set (k1 = 1.2, k2 = 1860) holds the
 *     EMF estimate to 0.07-0.17% over a 30x span of EMF amplitude, while the sigmoid at
 *     k_slide = 0.2 goes from -11.8% to -92.3% as its switch saturates.
 *   - The sigmoid's boundary layer parks s where k_slide*sigma(a*s) == e_a, an
 *     amplitude-dependent bias in the ESTIMATE. Super-twisting's error is a zero-mean
 *     limit cycle instead (see the discretization note below), so z is unbiased.
 *   - z stays continuous without a filter, for a structural reason rather than by
 *     smoothing -- so the "no equivalent-control LPF" property of smo.h survives.
 *
 * DISCRETIZATION (read before tuning). In continuous time super-twisting reaches s == 0
 * in finite time. This is explicit Euler at a finite Ts, and it does NOT: s settles into
 * a bounded limit cycle, ~300 LSB (~0.009 pu) at the design point above. The mechanism is
 * that the sqrt term's per-step correction G*k1*sqrt(|s|) exceeds 2*|s| once
 * |s| < (G*k1/2)^2, so the surface cannot settle below that and dithers instead. It is a
 * ripple, not a bias -- the mean estimate stays accurate and Stage 2's bandwidth filters
 * what is left -- but it means k1 has an UPPER bound as well as Levant's lower one.
 *
 * Stage 1, per axis, on the sliding surface s = i_est - i_meas:
 *   z      = k1*sqrt(|s|)*sgn(s) + w        (the EMF estimate; continuous)
 *   dw/dt  = k2*sgn(s)                      (the discontinuity, integrated away)
 *   i_est[k+1] = F*i_est[k] + G*(v_pu - z)  (identical current model to smo.c)
 * In sliding BOTH z and w converge to the active-flux EMF e_a; w is the smoother of the
 * two (it carries no sqrt term), which is what pll_on_w below selects between.
 *
 * Fields are PRIVATE to stsmo.c (kept in the header only so a port can statically
 * allocate, like smo_t/if_t/pi2dof_t). Cold COEFFICIENTS (stsmo_tune, preserved across
 * re-entry) then runtime STATE (zeroed by stsmo_init). */
typedef struct {
    /* --- coefficients: cold, set by stsmo_tune; PRESERVED across stsmo_init ------ */
    q15_t    f_decay;   /* F = 1 - Ts*Rs/Lq : estimated-current decay/step      [Q15] */
    q15_t    g_volt;    /* G = Ts*u_base/(Lq*i_base): pu-volt -> pu-i increment [Q15] */
    int32_t  k1;        /* sqrt-term gain          [pu-V per sqrt(pu-A), Q12, < 8.0] */
    q15_t    k2_ts;     /* integral-term gain, Ts folded in: k2*Ts        [pu-V, Q15] */
    int32_t  inv_eps;   /* 1/eps for the smoothed sgn      [1/pu-A, Q8]; 0 -> exact sgn */
    bool     pll_on_w;  /* feed Stage 2 the integral state w instead of the full z    */
    /* --- state: runtime, zeroed by stsmo_init ----------------------------------- */
    ab_pu_t  iab_est;   /* current estimate                                  [pu, Q15] */
    ab_pu_t  eab_est;   /* EMF estimate z = k1*sqrt(|s|)*sgn(s) + w          [pu, Q15] */
    ab_pu_t  wab_est;   /* the integral state w alone, saturated to Q15 (probe/compare) */
    q31_t    w_afa;     /* integral state, alpha axis, Q30 (mirrors pll_t.integ)       */
    q31_t    w_bet;     /* integral state, beta  axis, Q30                             */
    pll_t    pll;       /* Stage 2, by value -- the same loop smo_t embeds (pll.h)     */
} stsmo_t;

/* Tuning knobs the app chooses (cf. smo_cfg_t). SIZING, from the surface dynamics
 * s' = -alpha*(z - e_a) with alpha = u_base/(Lq*i_base) [(pu-A/s) per pu-V], and C an
 * upper bound on |de_a/dt| in pu-V/s (~ w_elec_max * |e_a|_max):
 *   k2 >  C                    (Levant; typically 1.1*C)
 *   k1 >  1.5*sqrt(C/alpha)    (Levant lower bound)
 * Note k1/k2 bound the EMF's RATE, not its amplitude -- the structural advantage over
 * smo_cfg_t.k_slide. Both are per-unit voltages; k1 additionally carries 1/sqrt(pu-A).
 *
 * k1 ALSO HAS A DISCRETE UPPER BOUND (see the discretization note above), the analogue of
 * smo_cfg_t.sig_a's stability bound: the surface cannot settle below (G*k1/2)^2 with
 * G = Ts*alpha, so an over-large k1 turns the limit cycle into gross chatter and then
 * instability. MEASURED on MACHINE_REF at 20 kHz (G = 0.154), k2 = 1860:
 *   k1 = 0.9 .. 1.6  surface ripple ~300 LSB, estimate within 0.2%   <- usable window
 *   k1 = 2.0         ripple 660 LSB, estimate error 4x worse
 *   k1 = 3.0         ripple 2200 LSB, visibly degraded
 *   k1 = 6.0         unstable (w rails)
 * The (G*k1/2)^2 expression is conservative -- it over-predicts the ripple by ~2x -- so
 * treat it as the mechanism and the window above as the calibration. Both bounds move
 * with Ts: G is proportional to Ts, so a slower loop tightens the k1 window.
 *
 * WORKED EXAMPLE, MACHINE_REF at rated (alpha = 3087, |e_a| = 0.898 pu, w_e = 1885 rad/s):
 *   C = w_e*|e_a| = 1693 pu-V/s -> k2 = 1.1*C ~ 1860
 *   Levant floor 1.5*sqrt(C/alpha) = 1.11, discrete window tops out ~1.6 -> k1 = 1.2
 * Note how narrow that window is. It is a real design constraint of this observer, not
 * slack: if the two bounds cross for a given machine, raise the sample rate. */
typedef struct {
    float k1;        /* sqrt-term gain    [pu-V/sqrt(pu-A)]; Q12 internally, keep < 8 */
    float k2;        /* integral-term gain            [pu-V per second]               */
    float eps;       /* sgn boundary layer [pu-A]: sgn(s) -> sat(s/eps). <= 0 gives the
                      * exact sign. RECOMMENDED ~0.01: measured, it removes the DC offset
                      * the exact sign leaves on the surface (s_mean -32 -> 0 LSB) and
                      * trims the ripple, at no cost in estimate accuracy. It does NOT
                      * reintroduce smo.h's bias -- the integral term still owns the
                      * steady state -- and it does not cause the limit cycle either, so
                      * do not expect it to remove one.                               */
    bool  pll_on_w;  /* true: Stage 2 tracks w (smoother); false: the full z (faster) */
    float bw_pll;    /* PLL tracking bandwidth                             [rad/s]    */
    float zeta_pll;  /* PLL damping (~1)                                      [-]     */
} stsmo_cfg_t;

/* Derive the Q12/Q15/Q30 coefficients from the SI machine + bases + tuning cfg (float,
 * COLD PATH -- call once at composition, like smo_tune). Uses Rs and Lq (the active-flux
 * series inductance); Ld/lam never enter, exactly as in smo.c. Zeroes the whole object
 * first, so it also clears state. NULL-safe / inert on a bad cfg (Ts <= 0, Lq <= 0,
 * NULL m/b/c). */
void       stsmo_tune(stsmo_t *o, const motor_cfg_t *m, const base_t *b,
                      const stsmo_cfg_t *c, float Ts);
/* Reset runtime STATE only (estimates, integral states, PLL), PRESERVING the
 * coefficients stsmo_tune set -- runs on every IF/IF_FOC entry (via ob_init, hsm.c). */
void       stsmo_init(stsmo_t *o);
/* Hot path: one observer step. NULL-safe: returns 0. */
angle_t    stsmo_step(stsmo_t *o, ab_pu_t iab_meas, ab_pu_t uab_ref_lim);
/* No per-field getters -- see smo.h. Read ob_step's ob_out_t, or this struct directly
 * (o.pll.spd_est, o.pll.e_mag_sq, o.pll.theta_err), which is what a SHADOW observer's
 * consumers already do (ob_tee.h). Note e_mag_sq is |z| as Stage 2 was handed it, BEFORE the
 * PLL_E_MIN floor (pll.h) -- the same quantity smo publishes, so a consumer grading against a
 * threshold in physical per-unit (conv.c derives emf_mag_min_sq from lambda_pm and w_base)
 * means the same thing whichever observer is primary. */
/* Bind this observer into a generic handle (see smo_bind / ob.h). NULL-safe. */
void       stsmo_bind(ob_t *ob, stsmo_t *o);

/* sqrt of a per-unit magnitude: Q15 in [0, 1] -> Q15 in [0, 1], exact to the LSB
 * (floor). Integer-only, no LUT: sqrt(x_pu) in Q15 is exactly isqrt(x_q15 << 15), and
 * the classic restoring bit-by-bit isqrt is ~16 iterations. A LUT would need ~193
 * entries to hold 1 LSB near s = 0 -- where accuracy decides the estimate -- so the
 * bitwise form is both smaller and exact. Internal to stsmo.c; declared here so the
 * host tests can sweep it against std::sqrt. */
q15_t      stsmo_sqrt_pu(q15_t x);
#ifdef __cplusplus
}
#endif
#endif
