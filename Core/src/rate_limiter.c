#include "rate_limiter.h"
/* Rate limiter. See rate_limiter.h for the contract, and for why a bounded slope rather than a
 * filter. Nothing below knows which signal it is pacing. */

void rate_limiter_tune(rate_limiter_t *r, float rate_pu_s, float Ts) {
    if(!r) return;
    r->inc = 0; r->acc = 0;                   /* no limit */
    if(rate_pu_s <= 0.0f || Ts <= 0.0f) return;
    /* pu/s -> Q15.16 per step: 1.0 pu is 2^15, and the accumulator carries 16 fractional bits
     * below that, so one pu per second is 2^31 spread over one second of steps. */
    float v = rate_pu_s * Ts * PU_ONE_Q15_16;
    if(v < 1.0f)               r->inc = 1;             /* a configured rate must always progress */
    else if(v > 1073741824.0f) r->inc = 1 << 30;       /* far past the full Q15 span in one step */
    else                       r->inc = (int32_t)(v + 0.5f);
}

void rate_limiter_set(rate_limiter_t *r, q15_t v) {
    if(r) r->acc = (int32_t)v * Q15_16_SCALE;        /* multiply, not << : v is signed (types.h) */
}

q15_t rate_limiter_step(rate_limiter_t *r, q15_t tgt) {
    if(!r) return tgt;
    const int32_t t = (int32_t)tgt * Q15_16_SCALE;
    if(r->inc == 0) { r->acc = t; return tgt; }   /* unlimited: the target IS the output */
    /* The move in int64: acc spans the whole int32 range and inc reaches 2^30, so the sum can
     * leave int32 on its way to a target that fits. The clamp to t brings it back before the
     * store, which is why the narrow accumulator is still enough to hold the result. */
    int64_t a = r->acc;
    if(a < t)      { a += r->inc; if(a > t) a = t; }
    else if(a > t) { a -= r->inc; if(a < t) a = t; }
    r->acc = (int32_t)a;
    return (q15_t)(r->acc >> Q15_16_SHIFT);
}

q15_t rate_limiter_get(const rate_limiter_t *r) {
    return r ? (q15_t)(r->acc >> Q15_16_SHIFT) : 0;
}
