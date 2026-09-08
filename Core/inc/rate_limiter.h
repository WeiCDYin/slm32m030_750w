#ifndef MC_RATE_LIMITER_H
#define MC_RATE_LIMITER_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t inc;   /* per-step increment, Q15.16 (a Q15 value << 16); 0 == unlimited, pass through */
    int32_t acc;   /* the limited value, Q15.16 */
} rate_limiter_t;

void  rate_limiter_tune(rate_limiter_t *r, float rate_pu_s, float Ts);
void  rate_limiter_set(rate_limiter_t *r, q15_t v);
q15_t rate_limiter_step(rate_limiter_t *r, q15_t tgt);
q15_t rate_limiter_get(const rate_limiter_t *r);
#ifdef __cplusplus
}
#endif
#endif
