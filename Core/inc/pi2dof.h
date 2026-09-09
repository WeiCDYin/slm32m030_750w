#ifndef MC_PI2DOF_H
#define MC_PI2DOF_H
#include "types.h"
#include "qmath.h"   /* q15_mul_shr15 / q15_mul_wide, used by the inline step below */
#ifdef __cplusplus
extern "C" {
#endif


/* THE LIMIT IS THE CALLER'S TOO -- this regulator never clamps its own output. It cannot: a
 * limit is a property of the ACTUATOR, and only the caller knows it.
 *   sc  -- the current limit i_max, which sc itself owns and applies.
 *   cc  -- whatever svm could actually synthesize, which is not a per-axis number at all: it is
 *          the hexagon, read back from the duties (svm_realized -> mc->uab_ref_lim). d and q
 *          spend ONE shared voltage budget, so no per-axis box could express it honestly.
 * So the step is SPLIT: pi2dof_output RETURNS the unlimited ask, the caller limits it, and
 * pi2dof_update_I TAKES BACK what that ask became once the limiter had its say -- the SAME tick,
 * no lag. An integrator winding against a limit the regulator invented is windup with extra steps.
 *
 * WIDE gains: the per-unit loop gain routinely exceeds 1.0 (sc: w_base >> i_base; cc: L*alpha
 * over the voltage base), so gains are Q15 stored in int32 and the products accumulate in
 * int64. A q15_t gain field would silently saturate at 1.0 and mistune the loop. */
typedef struct {
    int32_t kp, ki, kt; /* Q15 gains in int32 (effective gain may exceed 1.0); ki folds Ts */
    int32_t k_aw;       /* Q15 back-calc anti-windup gain (dimensionless per-step, Ts folded) */
    int64_t I;          /* Q30 integrator; holds the fb-proportional direct-path term, may exceed any clamp */
    int32_t out;        /* the ask from pi2dof_output, awaiting pi2dof_update_I -- WITHIN-tick
                         * scratch, not persisted state. int32, not q15_t: against an error it
                         * cannot reject the ask runs far past 1.0 pu (the whole point of the
                         * limiter downstream), and saturating it would corrupt (out_lim - out). */
} pi2dof_t;

/* The COLD half of the struct above -- exactly the fields a *_tune writes, and nothing else.
 * Split out as its own type so DERIVING a gain set and LOADING one are separate operations: the
 * numbers can come from cc_tune / sc_tune, or from anywhere else that produces this struct.
 * Same widths and the same conventions -- Q15 in int32, Ts already folded into k_i and k_aw. */
typedef struct {
    int32_t kp, ki, kt, k_aw;
} pi2dof_gains_t;

/* Load a gain set, then clear the runtime state -- which is what tuning has always done here
 * (every *_tune resets the integrator it retunes). NULL gains -> INERT: every gain zero, so the
 * regulator asks for nothing at all. That is the state a regulator is in before it has been
 * given a gain set, and it must be a safe one rather than a crash. NULL-safe. */
void pi2dof_set_gains(pi2dof_t *p, const pi2dof_gains_t *g);
/* Reset the runtime state -- integrator + last output to zero, gains kept. The per-entry reset
 * (cf. cc_init / sc_init, which call through to this). NULL-safe. */
void pi2dof_init(pi2dof_t *p);
/* Part 1 of the step: compute and RETURN the UNLIMITED output (Q15 in int32); the caller applies
 * its own limit to it. Also stashes the ask in p->out for pi2dof_update_I to read back. Does NOT
 * touch the integrator. Call this, limit the result, then call pi2dof_update_I -- in that order,
 * same tick. NULL-safe: 0. */
/* INLINE, with pi2dof_update_I below: both are ~20 instructions and BOTH run twice on every
 * carrier tick (cc.c steps a d and a q axis), so out of line the drive spent four ABI crossings
 * a tick on 40 instructions of arithmetic. Three call sites each, so the duplication is bounded. */
static inline int32_t pi2dof_output(pi2dof_t *p, q15_t ref, q15_t fb) {
    if(!p) return 0;
    int32_t ff  = q15_mul_shr15(p->kt, ref);                         /* k_t*ref (feedforward),   Q15 */
    int32_t bk  = q15_mul_shr15(p->kp, fb);                          /* k_p*fb  (state feedback), Q15 */
    int32_t out = ff - bk + (int32_t)(p->I >> Q15_SHIFT);            /* the ask, unlimited */
    p->out = out;                                                    /* hand off to pi2dof_update_I */
    return out;
}
/* Part 2 of the step: advance the integrator, same tick as the pi2dof_output that preceded it.
 *
 *   out_lim = what THIS tick's ask actually became after the caller limited it (sc: clamped to
 *             +-i_max; cc: ab2dq(svm_realized(...)) -- what svm really applied). The back-calc
 *             drains I by (out_lim - out), out being the ask pi2dof_output stashed in p->out.
 *             Pass the unlimited ask back unchanged when nothing limited it; the term is then 0.
 *
 * SAME-tick, not one step late: the split lets the limit of tick k's ask land in tick k's
 * integrator, because the caller limits BETWEEN output and update (for cc, svm runs there). ref
 * and fb must match the pi2dof_output call so err is this tick's. NULL-safe. */
static inline void pi2dof_update_I(pi2dof_t *p, q15_t ref, q15_t fb, q15_t out_lim) {
    if(!p) return;
    int32_t err = (int32_t)ref - fb;                                 /* Q15 */
    if(err > Q15_MAX) err = Q15_MAX;                                 /* saturate error to Q15 */
    if(err < Q15_MIN) err = Q15_MIN;

    /* Integrate, and drain by whatever the limiter rejected on THIS tick's ask -- p->out is the
     * value pi2dof_output just stored, so (out_lim - out) is a matched, same-tick pair. Both Q30. */
    /* The anti-windup term takes its operands the other way round: k_aw is the NARROW one
     * (bandwidth*Ts, capped below 1.0 where it is tuned) and the rejected amount is the wide
     * one, since p->out is the unlimited ask and runs past 1.0 pu by design (pi2dof.h). */
    p->I += q15_mul_wide(p->ki, err)
          + q15_mul_wide((int32_t)out_lim - p->out, p->k_aw);
}
/* Bumpless seed: set I so the NEXT pi2dof_output asks for `out` at THIS operating point
 * (ref, fb). Solves out == k_t*ref - k_p*fb + I for the integrator:
 *   I = (out<<15) - k_t*ref + k_p*fb
 * 2-DOF needs the operating point because I != out (a 1-DOF PI at zero error could just take
 * I = out). No steady-state assumption -- exact even when ref != fb, so it is valid mid-ramp
 * and mid-transient. Used by the I-f -> FOC handover (mc.c). NULL-safe. */
void pi2dof_set_I(pi2dof_t *p, q15_t out, q15_t ref, q15_t fb);
#ifdef __cplusplus
}
#endif
#endif
