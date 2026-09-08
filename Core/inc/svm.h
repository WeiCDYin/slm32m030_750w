#ifndef MC_SVM_H
#define MC_SVM_H
#include "types.h"
#include "trans.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SVM_UDC_NOM      12868  /* pi/2 pu == the nominal bus (2*Udc_nom/pi == u_base) */
#define SVM_UDC_PU_MIN    4503  /* 0.35x nominal */
#define SVM_UDC_PU_MAX   32170  /* 2.50x nominal */
#define SVM_DUTY_MID    (Q15_ONE / 2)   /* 0.5 in [0,1) Q15 (1.0 == 32768) */
#define SVM_DUTY_ZERO   ((duties_t){ SVM_DUTY_MID, SVM_DUTY_MID, SVM_DUTY_MID })
typedef int32_t svm_gain_t;
svm_gain_t svm_calc_gain(udc_pu_t udc_meas);
/* BY OUT-POINTER, unlike the transforms in trans.h, which are pure value-returning maps.
 * The reason is ARMv6-M: `*duty = svm(...)` makes the callee fill its hidden return buffer and
 * the caller then copy 6 bytes out of it, and a core with no unaligned access cannot expand that
 * copy inline -- it is a `bl memcpy`, ~100 cycles, on the current loop's every-tick path. Writing
 * `out` member by member costs nothing on either core. `out` must not be NULL and is not checked.
 * See docs/CYCLES.md 8; section 7's `svm` rework belongs in the same pass as this. */
/* Returns NONZERO if any leg clamped. That one bit is what lets the caller skip svm_realized:
 * the forward map and the inverse are exact inverses of each other, so with no clamp the round
 * trip returns the vector it was handed (see svm.c). Free to compute -- it is the same unsigned
 * test clamp_duty already makes, folded into an OR. */
int  svm(ab_pu_t uab_ref, svm_gain_t k_fwd, duties_t *out);
ab_pu_t  svm_realized(duties_t dabc, udc_pu_t udc_meas);
#ifdef __cplusplus
}
#endif
#endif
