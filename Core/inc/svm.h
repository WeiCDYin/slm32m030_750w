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
/* THE MODULATOR'S DUTY LIMITS -- what a leg is allowed to ask the POWER STAGE for, which is a
 * smaller thing than what a q15_t can hold. Both are PROPERTIES OF THE BOARD:
 *   ceiling -- a leg held near 100% never turns its low side on long enough for the low-side
 *              shunt to carry phase current, nor long enough for a bootstrapped high-side gate
 *              supply to refresh. Reach 100% and the high-side drive collapses; that is the
 *              hardware overcurrent a low bus provokes, because the modulator asks for more duty
 *              to make the same volts.
 *   floor   -- a leg asked for a pulse narrower than the gate driver can reproduce turns on
 *              partially rather than not at all, which is dissipation instead of switching.
 *              Whether a board needs one is a driver question, and the usual alternative is to
 *              drop the pulse rather than widen it -- so 0 stays the default.
 *
 * SETTABLE AT RUNTIME, not by a -D, because the limits have to be changeable by whoever holds a
 * built archive rather than only by whoever compiled it. They are the same fraction on paper and
 * a different TIME on every board: 2% is 3.3 us at a 6 kHz carrier and 1.0 us at 20 kHz, against
 * dead times of 1.0 us and 0.5 us. A number that means two different things cannot be a core
 * constant.
 *
 * THEY STAY INSIDE svm() WHEREVER THEY COME FROM. A port that clipped the duties afterwards
 * would apply less voltage than the core believed it had, svm_realized would report the vector
 * that was asked for rather than the one applied, and the current loop would wind up against a
 * ceiling nothing told it about. Setting them here keeps the readback and the clamp flag honest.
 *
 * The two below are named _DEFAULT because that is all they are: what is in force until
 * svm_set_duty_limit says otherwise, and what a port that never calls it keeps. 95% is the paper
 * number pending the bench sweep (TODO.md, "low-side sampling window"); 0 is the floor a board
 * with no minimum-pulse requirement wants. svm() stays correct for any accepted pair -- see
 * svm.c on why its clamp flag can only over-report, never miss. */
#define SVM_DUTY_MAX_DEFAULT    ((q15_t)((Q15_ONE * 0.95f)))   /* 0.95 duty: 31129 */
#define SVM_DUTY_MIN_DEFAULT    ((q15_t)0)
#define SVM_DUTY_ZERO   ((duties_t){ SVM_DUTY_MID, SVM_DUTY_MID, SVM_DUTY_MID })
typedef int32_t svm_gain_t;
/* Set the duty limits (above). COLD PATH -- call once at composition, before the bridge is
 * energized. Returns 1 if the pair was taken, 0 if it was REJECTED and the previous limits still
 * stand: a caller that fumbles the units must not be able to silently narrow the modulator to
 * nothing. Rejected pairs are min < 0, min >= max, and any pair that excludes SVM_DUTY_MID --
 * the zero-voltage vector parks all three legs there, so limits that cannot express it would
 * make standstill unreachable. max carries no ceiling of its own: q15_t is one (svm.c). */
int svm_set_duty_limit(q15_t min, q15_t max);
svm_gain_t svm_calc_gain(udc_pu_t udc_meas);
/* BY OUT-POINTER, unlike the transforms in trans.h, which are pure value-returning maps.
 * The reason is ARMv6-M: `*duty = svm(...)` makes the callee fill its hidden return buffer and
 * the caller then copy 6 bytes out of it, and a core with no unaligned access cannot expand that
 * copy inline -- it is a `bl memcpy`, ~100 cycles, on the current loop's every-tick path. Writing
 * `out` member by member costs nothing on either core. `out` must not be NULL and is not checked.
 * See docs/CYCLES.md 8; section 7's `svm` rework belongs in the same pass as this. */
/* Returns NONZERO if any leg clamped. That one bit is what lets the caller skip svm_realized:
 * the forward map and the inverse are exact inverses of each other, so with no clamp the round
 * trip returns the vector it was handed (see svm.c). Nearly free -- the same two limit compares
 * clamp_duty already makes, asked once per leg. The answer has to be EXACT in BOTH directions:
 * a missed clamp hands the caller a voltage the motor never got, and a clamp reported where
 * none happened costs it ~238 cycles recovering a vector it already had. That rules out the
 * cheaper `(da|db|dc) > SVM_DUTY_MAX_DEFAULT`, which was exact only while the limit was the all-ones
 * mask 0x7FFF: below it, two legs straddling SVM_DUTY_MID OR to 0x7FFF at ZERO modulation and
 * the flag fires on ~68% of ticks. */
int  svm(ab_pu_t uab_ref, svm_gain_t k_fwd, duties_t *out);
ab_pu_t  svm_realized(duties_t dabc, udc_pu_t udc_meas);
#ifdef __cplusplus
}
#endif
#endif
