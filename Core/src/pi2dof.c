#include "pi2dof.h"
#include "qmath.h"   /* q15_mul_shr15 / q15_mul_wide: the gain*signal product (qmath.h) */
/* 2-DOF PI regulator; see pi2dof.h for the contract and the design. Validated by
 * test_pi2dof.cpp -- that file is the contract. Lifted from sc.c, which was the first
 * (mechanical-plant) instance of this math; cc.c is the second (electrical).
 *
 *   err   = clamp(ref - fb, Q15)                                  [Q15]
 *   out   = (kt*ref)>>15 - (kp*fb)>>15 + I>>15                     [Q15 in int32, UNLIMITED]
 *   I    += ki*err + k_aw*(out_lim - out)                          [Q30 accumulator]
 *
 * Anti-windup is BACK-CALCULATION, not integrator clamping (pll.c's choice): the realizable
 * form drains the integrator by the amount the limiter actually rejected, which is what keeps
 * the loop's recovery from saturation matched to its tuning (k_aw = k_i/k_t = alpha). Clamping
 * I to some output range instead would be wrong twice over -- I legitimately exceeds the limit
 * (it carries the k_p*fb direct-path term that out subtracts back out), and this regulator does
 * not know the limit in the first place (pi2dof.h).
 *
 * The step is SPLIT so the back-calc is SAME-tick: pi2dof_output computes the ask and stores it
 * in p->out; the caller limits it; pi2dof_update_I then drains by (out_lim - p->out) with out_lim
 * the limit of THIS tick's ask -- a matched pair, no one-tick lag. p->out lives only from output
 * to update (within-tick scratch), so output MUST precede update.
 *
 * Widths: gains are Q15-in-int32 and the products land in int64 (Q30), so the per-unit loop
 * gain is free to exceed 1.0 -- see pi2dof.h. The error is saturated to Q15 first, so a
 * ref/fb pair at opposite rails cannot make the products overshoot their range. */

void pi2dof_init(pi2dof_t *p) {
    if(!p) return;
    p->I   = 0;
    p->out = 0;
}

void pi2dof_set_I(pi2dof_t *p, q15_t out, q15_t ref, q15_t fb) {
    if(!p) return;
    int64_t ff = q15_mul_wide(p->kt, ref);                   /* k_t*ref (reference FF),    Q30 */
    int64_t bk = q15_mul_wide(p->kp, fb);                    /* k_p*fb  (state feedback),  Q30 */
    p->I   = ((int64_t)out * Q15_ONE) - ff + bk;             /* the integrator supplies the rest;
                                                              * multiply: out is signed (types.h) */
}

int32_t pi2dof_output(pi2dof_t *p, q15_t ref, q15_t fb) {
    if(!p) return 0;
    int32_t ff  = q15_mul_shr15(p->kt, ref);                         /* k_t*ref (feedforward),   Q15 */
    int32_t bk  = q15_mul_shr15(p->kp, fb);                          /* k_p*fb  (state feedback), Q15 */
    int32_t out = ff - bk + (int32_t)(p->I >> Q15_SHIFT);            /* the ask, unlimited */
    p->out = out;                                                    /* hand off to pi2dof_update_I */
    return out;
}

void pi2dof_update_I(pi2dof_t *p, q15_t ref, q15_t fb, q15_t out_lim) {
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
