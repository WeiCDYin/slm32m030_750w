#ifndef MC_PLL_H
#define MC_PLL_H
#include "types.h"
#include "base.h"
#ifdef __cplusplus
extern "C" {
#endif
/* TRACKING PLL -- the shared Stage 2 of every EMF-based observer. Takes a stationary-
 * frame vector that points at theta + 90*sign(w) (a back-EMF / active-flux EMF estimate)
 * and locks an angle onto it, with speed as a byproduct. Deliberately knows NOTHING about
 * how that vector was produced: smo.c makes it with a sigmoid switch, stsmo.c with a
 * super-twisting differentiator, and both drive the identical loop below. That is what
 * makes an A/B between them meaningful -- only Stage 1 differs (cf. pi2dof.h, the same
 * "one regulator, many users" split).
 *
 * Phase detector = the Park-d of the input vector in the PRESENT estimated frame:
 *   e_d = e_a*cos(theta_est) + e_b*sin(theta_est) = |e|*sin(theta_est - theta), zero at lock.
 * The error fed to the loop is -dir*e_d, so at dir = +1 theta_est lagging (theta_est <
 * theta) gives e_d < 0, error > 0, and the loop speeds theta_est up.
 *
 * THE dir TERM IS NOT OPTIONAL -- it is what makes the loop work in REVERSE. The input
 * vector is the flux differentiated, e = w*lam_a*u(theta+90), and w is SIGNED: a negative
 * w flips the vector by 180, so it points at theta+90 running forward and theta-90 running
 * backward. Meanwhile e_d = 0 has TWO roots, 180 apart, and which one the loop holds is
 * fixed by the detector's sign alone. Linearizing x = phi - theta_est (phi = the input
 * vector's angle) about a root x*, with err = -dir*cos(x):
 *   err(x* + d) ~ dir*sin(x*)*d,  so  lambda^2 + Kp*dir*sin(x*)*lambda + Ki*dir*sin(x*) = 0
 * which is Hurwitz only for dir*sin(x*) = +1, i.e. the loop's single attractor is
 * x* = 90*dir. With phi = theta + 90*sign(w) that gives theta_est = theta + 90*sign(w) -
 * 90*dir -- zero frame error in BOTH directions iff dir = sign(w), and 180 deg out
 * whenever it is not. A hard-coded dir = +1 therefore locks the estimated d-axis onto the
 * rotor's SOUTH pole in reverse, and since the rotation by 180 is -I (it commutes with the
 * current loop, which is regulating in the same inverted frame) the drive happily lands
 * i_q_ref on -i_q, inverting torque and closing the speed loop with POSITIVE feedback.
 * That was a real bug: reverse startups stalled on the reference machine and ran away to
 * the voltage ceiling on BL4260, gated only by rotor inertia. See core.py's
 * test_if_foc_transition_negative_speed.
 *
 * dir comes from spd_est, and that is sound rather than circular: the analysis above puts
 * BOTH roots at dtheta_est/dt = w, so the integrator tracks the true speed WITH THE RIGHT
 * SIGN even while sitting at the wrong root (measured -487 vs -491 rad/s truth). So the
 * loop always knows which way the vector is going; it just was not using it. A wrong
 * initial dir is self-correcting -- it locks 180 out, spd_est goes negative, dir flips, the
 * former attractor becomes the saddle and the loop rotates onto the right root -- which is
 * why no external seed is needed. PLL_DIR_HYST (pll.c) keeps that flip from chattering at
 * standstill, where the sign is genuinely undefined.
 *
 * NORMALIZED by |e| = w*|lam_a|:  err = -e_d/|e| = sin(theta_est - theta), the phase error
 * in RADIANS. This is what the gains below are actually derived for -- a type-II loop wants
 * w = Kp*dtheta + Ki*int(dtheta) with dtheta in radians, and sin(dtheta) ~ dtheta. The
 * un-normalized -e_d silently multiplied that design by |e|, so the loop gain rode on SPEED:
 * |e| = 0.855*s pu on BL4260, a 4x gain swing between the I-f handover (s = 0.15) and
 * 5000 rpm (s = 0.625), critically damped at only one of them. Normalized, the small-signal
 * bandwidth IS w_pll everywhere above the low-speed floor.
 *
 * RETUNE AFTER SWITCHING THIS ON. Any w_pll bench-tuned against the old detector had the
 * |e| attenuation baked in, so it now runs ~1/|e| hotter at the point it was swept (~1.9x at
 * s = 0.625). Expect the correct values to be LOWER than the pre-normalization ones.
 *
 * The implementation (pll.c) is DIVISION-FREE and ROOT-FREE -- two small LUTs and a handful
 * of multiplies, no SDIV and no CLZ -- because the core has to compile for an ARMv6-M (M0)
 * port that has neither. It carries a low-speed FLOOR on |e|; below it the gain falls back
 * to being speed-proportional, which is safe here because the I-f->FOC transition is
 * agreement-gated (docs/HSM.md 7) and the loop only has to be right above the handover.
 *
 * THE LOOP NEVER TAKES A SQUARE ROOT. It used to take two per tick -- ~250 cycles each on an
 * M0, the largest single algorithmic item in the carrier ISR -- to turn |e|^2 into |e|. But
 * arg() is scale-invariant, so the angle never wanted a magnitude at all; and every consumer
 * of the one |e| that IS published either compares it against a threshold (conv.c) or divides
 * it by the floor (below), and both of those survive squaring, since x >= t iff x^2 >= t^2 for
 * non-negative x. So the loop publishes |e|^2 and the threshold is squared once at tune time.
 * isqrt_q30 still exists in qmath.c for stsmo_sqrt_pu and mc.c's modulation index; it is no
 * longer on the observer path.
 *
 * HARNEFORS TAP: the INTEGRATOR output is the speed estimate w_est = Ki*int(err); the
 * Kp*err correction goes straight onto the POSITION integrator (theta_est =
 * int(w_est + Kp*err)). Identical type-II transfer to a PI-then-integrate loop, but
 * w_est carries no direct P-path phase-detector noise -- it is the smoothed
 * mean-frequency state (Harnefors & Nee 2000). It mattered more before normalization (the
 * raw error grew with speed, so P-path noise rode up with it); it still matters, because
 * dividing by |e| AMPLIFIES Stage-1 ripple as |e| falls -- most visibly for stsmo.c, whose
 * z carries the discrete limit cycle's sqrt term (stsmo.h).
 *
 * THE atan DETECTOR -- now the only one. It replaced an |e|-NORMALIZED detector that formed
 * the same phase error a different way:
 *
 *   normalized   err = -dir*e_d/|e|                    = sin(theta - theta_est)
 *   atan         err = theta_meas - theta_est,           theta_meas = arg(e) - 90*dir
 *
 * arg() is scale-invariant, so the atan form IS normalized -- there is no |e| in the error
 * path to divide out. That is what retired the other one: its isqrt + reciprocal + mantissa
 * apparatus existed only to synthesize, at ~250 cycles a tick and 4 LSB of error, what an
 * argument gives exactly and for free. The A/B that made the case is in the git history;
 * carrying a second detector to keep re-running it was not worth its place in the ISR.
 * What the atan form BUYS, beyond the cycles, is linearity. sin carries
 * a cos(dtheta) gain that droops to zero at 90 deg and inverts past it, so pll_tune's
 * bandwidth and damping describe the loop only NEAR LOCK; a wrapped angle difference is the
 * error itself over the whole circle, so the designed response is also the ACQUISITION
 * response. That is what RESYNC wants: it hands over by JUMPING the control frame onto
 * theta_est (mc.c), so its dwell is a PLL-lock allowance, and a loop that acquires in its
 * designed time is what shortens it.
 *
 * CLAMPED AT +-1.0 rad, deliberately -- precisely the range sin can produce. Every bound
 * downstream (the integ clamp, the w_ang clamp, the k_theta overflow argument) was derived
 * for an error in [-1, 1] and is untouched, and small-signal this detector and the sin form
 * it replaced are the same number -- so the gains carried across WITHOUT retuning.
 * Past a radian the linear form holds full drive where sin decays and then inverts, which
 * is strictly better pull-in, not a compromise.
 *
 * dir IS STILL REQUIRED, for the reason it always was: arg(e) is theta + 90*sign(w), so the
 * quarter turn to take back off carries the sign of the speed. It stops being a choice of
 * attractor and becomes an explicit offset, but every conclusion above survives -- lock is
 * correct iff dir = sign(w), and a wrong dir still self-corrects through spd_est.
 *
 * THE FLOOR IS EXPLICIT. Scale invariance affords no graceful degradation of its own: arg()
 * of a near-zero vector is full-scale noise delivered at full gain. So the detector scales
 * the error down below PLL_E_MIN -- a speed-proportional fallback stated on purpose rather
 * than inherited from a numerical guard. Stated on the SQUARE, the taper is (|e|/PLL_E_MIN)^2:
 * full trust and zero trust at the same two places, steeper in between, which is the safe
 * direction for a mute that exists to stop the loop chasing what it cannot resolve.
 *
 * Fields are PRIVATE to pll.c (in the header so an observer can embed one by value).
 * Split into cold COEFFICIENTS (pll_tune, preserved) and runtime STATE (pll_init). */
typedef struct {
    /* --- coefficients: cold, set by pll_tune; PRESERVED across pll_init ---------- */
    q15_t    kp;        /* proportional gain                                   [Q15] */
    q15_t    ki;        /* integral gain (Ts folded in)                        [Q15] */
    int32_t  k_theta;   /* angle increment per speed LSB: w_base*Ts/2pi * 2^17       */
    /* --- state: runtime, zeroed by pll_init ------------------------------------- */
    uint32_t theta_acc; /* phase accumulator, Q32 (full circle == 2^32); angle = >>16 */
    q31_t    integ;     /* PI integrator, Q30 (cf. pi2dof.c's I)                     */
    spd_pu_t spd_est;   /* estimated electrical speed (pu of w_base)           [Q15] */
    uint32_t e_mag_sq;  /* |e|^2 of the EMF the detector was handed, BEFORE the PLL_E_MIN
                         * floor below it. Kept because the detector computes it anyway and
                         * because it is the one speed-proportional quantity here that owes
                         * nothing to the loop having locked: |e| = lambda*|we| is algebraic
                         * in the estimate, while spd_est is the integrator of a PI that has
                         * to acquire first. Un-floored on purpose -- a caller asking "is the
                         * rotor turning at all" needs to see below 0.05 pu.
                         *
                         * SQUARED, and published that way, because taking the root would cost
                         * ~250 cycles a tick to produce a number every consumer immediately
                         * compares against a threshold (conv.c) -- and a comparison survives
                         * squaring. Q30, not Q15: the sum of two Q15 squares reaches 2^31, so
                         * this is uint32 and a railed 1.414 pu pair needs no clamp.    [Q30] */
    q15_t    theta_err; /* the phase error the detector formed THIS tick, in Q15 RADIANS, signed
                         * and clamped to +-1.0 rad. Published because judging it is not the
                         * loop's job -- whether the angle has ARRIVED is decided outside, on
                         * this and |e| alone (conv.h). Costs nothing: the detector forms it
                         * every tick regardless.
                         *
                         * ALWAYS MEANS "HOW WRONG IS THE ANGLE", and a wrapped angle difference
                         * says so on its own: 180 deg out reads as 180 deg out. The sin form
                         * this replaced could not -- it vanishes at BOTH roots, so it measured
                         * ~0 while the frame was inverted, and pll_step had to substitute full
                         * scale at the wrong root. Nothing to correct here.            [Q15] */
    int32_t  dir;       /* detector sign, +1/-1: which of the two 180-apart roots the
                         * loop holds. Tracks sign(spd_est) with hysteresis; STATE, not
                         * a setting -- pll_init puts it at +1. See the block above.   */
} pll_t;

/* Derive the Q15/Q31 coefficients from the bases + bandwidth/damping (float, COLD PATH
 * -- call once at composition, from the owning observer's *_tune):
 *   Kp = 2*zeta*w_pll/w_base,  Ki = w_pll^2/w_base * Ts   (Ts folded in, cf. pi2dof.c)
 * Zeroes the whole object first, so it also clears state. NULL-safe / inert on a bad
 * cfg (Ts <= 0, NULL b, w_base <= 0). */
void       pll_tune(pll_t *pll, const base_t *b, float w_pll, float zeta, float Ts);
/* Reset runtime STATE only (accumulator, integrator, speed), PRESERVING coefficients --
 * runs on every observer re-entry (via ob_init, hsm.c). */
void       pll_init(pll_t *pll);
/* Hot path: one PLL step against an EMF estimate in the stationary frame. Returns the
 * updated angle estimate. NULL-safe: returns 0. */
angle_t    pll_step(pll_t *pll, ab_pu_t eab_est);
/* The tracking-loop byproducts. NULL-safe: return 0. */
spd_pu_t   pll_get_spd(const pll_t *pll);
angle_t    pll_get_angle(const pll_t *pll);
#ifdef __cplusplus
}
#endif
#endif
