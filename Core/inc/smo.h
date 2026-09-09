#ifndef MC_SMO_H
#define MC_SMO_H
#include "types.h"
#include "base.h"
#include "motor_cfg.h"
#include "ob.h"          /* ob_t: the generic observer interface smo_bind fills */
#include "pll.h"         /* pll_t: the shared Stage-2 tracking loop (also used by stsmo) */
#ifdef __cplusplus
extern "C" {
#endif
/* ACTIVE-FLUX sliding-mode observer + tracking PLL -- one implementation of the
 * generic observer interface (mc/ob.h). The core only ever sees ob_t; a port
 * composes a smo_t, smo_tune()s it, and smo_bind()s it into an ob_t (see sim_api.c).
 * Swapping in a different observer (e.g. a flux observer) means another *_bind, with
 * the core and ports unchanged. Input contract (Clarke already done by the caller):
 * stationary-frame currents + commanded voltages in, angle out.
 *
 * Active flux lam_a = lam_pm + (Ld-Lq)*i_d is always d-axis aligned, so its angle is
 * the rotor angle for SPM AND IPM (Boldea); the observer's series inductance is Lq
 * and the saliency folds into lam_a. See smo.c for the full derivation.
 *
 * Two stages (docs/HSM.md, smo.c):
 *   1. SM current model (series L = Lq) drives an estimated current onto the measured
 *      one; in sliding mode the switching term equals the active-flux EMF e_a. The
 *      switch is a SMOOTH sigmoid sigma(a*s) = 2/(1+e^-a*s) - 1 (not a discontinuous
 *      sign/sat), so its output is already chatter-free and feeds the PLL directly --
 *      no equivalent-control LPF (and none of the speed-dependent phase lag one would
 *      add); the PLL bandwidth does the filtering. a sets sharpness.
 *   2. The SHARED tracking PLL (pll.h) locks to that e_a vector; the angle estimate is
 *      the top 16 bits of its Q32 accumulator, with spd_est as a byproduct. Stage 2 is
 *      NOT this observer's -- stsmo.h embeds the identical pll_t, so a comparison between
 *      the two observers isolates Stage 1.
 *
 * Fields are PRIVATE to smo.c (kept in the header only so the port can statically
 * allocate, like if_t/pi2dof_t). The struct splits into cold COEFFICIENTS (set once by
 * smo_tune, preserved across re-entry) and runtime STATE (zeroed by smo_init). */
typedef struct {
    /* --- coefficients: cold, set by smo_tune; PRESERVED across smo_init -------- */
    q15_t    f_decay;     /* F = 1 - Ts*Rs/Lq : estimated-current decay/step   [Q15] */
    q15_t    g_volt;      /* G = Ts*u_base/(Lq*i_base): pu-volt -> pu-i incr    [Q15] */
    q15_t    k_slide;     /* sliding gain (switching amplitude)            [pu, Q15] */
    int32_t  sig_a;       /* sigmoid steepness a in z=k_slide*sigma(a*s) [1/pu, Q8] */
    q15_t    k_emf;       /* pu EMF per pu SPEED: lam_a*w_base/u_base -- the ONLY place a
                           * flux magnitude enters this observer, and it enters for the
                           * expected_emf_mag_sq DIAGNOSTIC alone, never for the estimate and
                           * never for a decision. ~0.86 and ~0.90 on the two machines here: the
                           * per-unit bases are chosen independently, so it is not 1.0. [Q15] */
    /* --- state: runtime, zeroed by smo_init ----------------------------------- */
    ab_pu_t  iab_est;     /* current estimate [pu, q15] */
    ab_pu_t  eab_est;     /* emf estimate     [pu, q15] */
    /* A DIAGNOSTIC PAIR, kept rather than left in locals: one says what the observer sees, the
     * other what the machine should be producing at the speed it thinks it has, and which of them
     * moved tells you whether a suspect estimate is Stage 1, Stage 2, or the machine constant.
     * Nothing in the drive decides on them -- see smo.c report_emf for how to read them, and
     * conv.h for what the handover is actually gated on. Both SQUARED, pu^2 of u_base^2 [Q30] --
     * the pair is squared TOGETHER so their ratio still reads 1.0 when they agree; everywhere else
     * it is the square of the old ratio (0.9 reads 0.81). Neither is rooted, because nothing
     * gating on them needs |e| rather than |e|^2 (pll.h). Refreshed every step, so they are
     * this tick's. */
    uint32_t calculated_emf_mag_sq; /* |z|^2 -- Stage 1's own answer, no tracking loop in it     */
    uint32_t expected_emf_mag_sq;   /* (k_emf*|spd_est|)^2 -- what Stage 2's speed implies       */
    /* Stage 2, by value: holds BOTH its own coefficients (preserved by smo_init) and its
     * runtime state (cleared by smo_init, via pll_init). See pll.h. */
    pll_t    pll;
} smo_t;

/* The COLD half of smo_t: exactly what smo_tune writes, Stage 2's block included, and nothing
 * a step touches. Its own type so DERIVING the coefficients and LOADING them are separable
 * (cf. pi2dof.h) -- smo_set_gains takes one of these however it was produced. */
typedef struct {
    q15_t       f_decay;  /* F = 1 - Ts*Rs/Lq                                       [Q15] */
    q15_t       g_volt;   /* G = Ts*u_base/(Lq*i_base)                              [Q15] */
    q15_t       k_slide;  /* sliding gain (switching amplitude)                 [pu, Q15] */
    int32_t     sig_a;    /* sigmoid steepness a                              [1/pu, Q8] */
    q15_t       k_emf;    /* pu EMF per pu speed -- the diagnostic constant only    [Q15] */
    pll_gains_t pll;      /* Stage 2, the shared tracking loop (pll.h)                   */
} smo_gains_t;

/* Load a gain set and clear the runtime state, Stage 2 included -- what smo_tune does once it
 * has computed one. NULL gains -> INERT: every coefficient zero, so the current model never
 * moves and the loop reports angle 0. NULL-safe. */
void smo_set_gains(smo_t *smo, const smo_gains_t *g);

/* Tuning knobs the app chooses (cf. if_cfg_t): pu/SI design parameters that
 * smo_tune turns into the Q15/Q31 coefficients of smo_gains_t. */
typedef struct {
    float k_slide;   /* sliding gain -- must exceed the max back-EMF seen     [pu] */
    float sig_a;     /* sigmoid steepness [1/pu]: larger -> sharper, closer to sign. Bounded
                      * ABOVE by discrete stability -- past the bound the current observer
                      * chatters, which would defeat the no-LPF design. Single digits on these
                      * machines; the bound itself is derived in smo.c. */
    float bw_pll;    /* PLL tracking bandwidth                             [rad/s] */
    float zeta_pll;  /* PLL damping (~1)                                      [-]  */
} smo_cfg_t;

/* Derive Q15/Q31 coefficients from the SI machine + bases + tuning cfg (float,
 * COLD PATH -- call once at composition, like cc_tune). Uses Rs, Lq (series
 * inductance), and Ld/lam for the active flux; SPM (Ld==Lq) zeroes the saliency term.
 * Zeroes the whole object first, so it also clears state. NULL-safe / inert on a
 * bad cfg (Ts <= 0, Lq <= 0, NULL m/b/c). */
void smo_tune(smo_t *smo, const motor_cfg_t *m, const base_t *b,
              const smo_cfg_t *c, float Ts);
/* Reset runtime STATE only (estimates, integrators, accumulators), PRESERVING the
 * coefficients smo_tune set -- runs on every IF/IF_FOC entry (via ob_init, hsm.c). */
void smo_init(smo_t *smo);
/* Hot path: one observer step. NULL-safe: returns 0. */
angle_t smo_step(smo_t *smo, ab_pu_t iab_meas, ab_pu_t uab_ref_lim);
/* NO PER-FIELD GETTERS. Everything one step produces reaches a caller two ways and no third:
 * through ob_step's ob_out_t (ob.h) if it holds a generic handle, or by reading this struct --
 * smo.pll.spd_est, smo.calculated_emf_mag_sq, smo.pll.theta_err -- if it holds an smo_t, which
 * is already the documented route for a SHADOW observer (ob_tee.h) and for the bench and
 * J-Scope channels. The accessors that used to sit here existed to fill vtable slots; once the
 * step published its own outputs, nothing outside the tests called them.
 *
 * The two quantities settle on different clocks, which is why both are published: |e| =
 * lambda*|we| is algebraic in the estimate and is there as soon as the current error is, while
 * spd_est is the integrator of a tracking loop that must ACQUIRE -- after an smo_init it swings
 * through both signs on the way in. An amplitude answers "is the rotor turning"; only the PLL
 * answers "which way, and at what angle".
 *
 * Neither is a VERDICT. This observer does not certify itself: how much angle error survives
 * the handover is set by the speed-loop-to-PLL bandwidth ratio, which is the drive's property,
 * not the observer's (conv.c). The observer reports, mc.c decides. */

/* Bind this SMO into a generic observer handle: point ob's vtable at the smo_* impl
 * and ob->state at this smo. After this, mc->ob = ob makes the core run the SMO through
 * the generic ob_step/ob_init. NULL-safe. */
void smo_bind(ob_t *ob, smo_t *smo);
#ifdef __cplusplus
}
#endif
#endif
