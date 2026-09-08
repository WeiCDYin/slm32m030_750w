#ifndef MC_PI2DOF_H
#define MC_PI2DOF_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Two-degree-of-freedom PI regulator, WIDE-gain fixed point. The reusable regulator behind
 * both control loops: sc (speed, cold) and cc (the dq current pair, hot). The 1-DOF form
 * survives only inside pll.c, whose tracking loop carries its own; this is the 2-DOF one.
 *
 *   out = k_t*ref - k_p*fb + I          <- 2 DOF: the reference and the feedback enter with
 *   I  += k_i*err + k_aw*(out_lim-out)     DIFFERENT gains, so tracking and rejection are
 *                                          tuned INDEPENDENTLY. (1-DOF ties them: k_t == k_p,
 *                                          which is just k_p*err.)
 *
 * WHY 2-DOF, on a plant already pole-zero cancelled: cancellation HIDES the plant pole, it does
 * not MOVE it -- the pole survives in the DISTURBANCE response, so a disturbance decays at the
 * open-loop rate no matter how fast the loop is tuned. Feeding fb back through its own k_p
 * (the "active damping" term) places BOTH closed-loop poles at alpha, so rejection runs at the
 * loop bandwidth. Reference tracking stays first-order alpha/(s+alpha) either way.
 *
 * The plant mapping is the CALLER's (it is what makes this generic): pick k_p/k_i/k_t from the
 * plant and the desired bandwidth alpha, and the closed loop is (s+alpha)^2 --
 *   mechanical (sc.c): k_p = (2*alpha*J - B)/k, k_i = alpha^2*J/k, k_t = alpha*J/k
 *   electrical (cc.c): k_p =  2*alpha*L - R,    k_i = alpha^2*L,    k_t = alpha*L
 * i.e. the SAME design under J->L, B->R, torque-gain k->1. Anti-windup is the realizable form,
 * k_aw = k_i/k_t = alpha. Convert to per-unit and fold Ts into k_i/k_aw before storing (the
 * step SUMS the error, it does not multiply by dt).
 *
 * THE LIMIT IS THE CALLER'S TOO -- this regulator never clamps its own output. It cannot: a
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

/* Reset the runtime state -- integrator + last output to zero, gains kept. The per-entry reset
 * (cf. cc_init / sc_init, which call through to this). NULL-safe. */
void pi2dof_init(pi2dof_t *p);
/* Part 1 of the step: compute and RETURN the UNLIMITED output (Q15 in int32); the caller applies
 * its own limit to it. Also stashes the ask in p->out for pi2dof_update_I to read back. Does NOT
 * touch the integrator. Call this, limit the result, then call pi2dof_update_I -- in that order,
 * same tick. NULL-safe: 0. */
int32_t pi2dof_output(pi2dof_t *p, q15_t ref, q15_t fb);
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
void pi2dof_update_I(pi2dof_t *p, q15_t ref, q15_t fb, q15_t out_lim);
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
