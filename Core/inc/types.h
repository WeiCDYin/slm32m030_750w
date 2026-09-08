#ifndef MC_TYPES_H
#define MC_TYPES_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef int16_t q15_t;
typedef int32_t q31_t;
/* Q15 fixed-point: 1.0 == 2^15. */
#define Q15_SHIFT (15)
#define Q15_ONE   (1 << Q15_SHIFT)                                   /* 32768  (== 1.0, not representable in q15_t) */
/* WIDENING A Q15 INTO A WIDER ACCUMULATOR IS A MULTIPLY, NOT A LEFT SHIFT -- `v * Q15_ONE`, or
 * `v * Q15_16_SCALE` for the Q15.16 ramp accumulators (if.c, vf.c, rate_limiter.c). Every such value here
 * is signed and goes NEGATIVE on reverse running, and C99 6.5.7p4 defines E1 << E2 only for a
 * nonnegative signed E1: a negative left operand is undefined, whatever the magnitude. The multiply
 * says the same thing in defined language and costs nothing -- q15_t bounds the product (-32768 *
 * 65536 is exactly INT32_MIN, representable), and gcc/armclang/armcc all expand a constant
 * power-of-two multiply to the same single shift instruction, at -O0 as well as -O2.
 *
 * NARROWING BACK STAYS A RIGHT SHIFT. `acc >> Q15_16_SHIFT` is NOT interchangeable with a divide: the
 * shift floors, the divide truncates toward zero, and for negative acc they differ by one LSB --
 * which for a ramp accumulator is a real behavioural change. A right shift of a negative value is
 * implementation-defined (6.5.7p5), not undefined, and every compiler this tree builds with defines
 * it as the arithmetic shift the fixed-point maths wants. Leave the reads alone. */
#define Q15_MAX   (32767)                                            /* +0.99997 */
#define Q15_MIN   (-32768)                                           /* -1.0     */
#define Q31_MAX   (2147483647)
#define Q31_MIN   (-2147483648)
/* Q15.16: a Q15 value carried with 16 EXTRA fractional bits, so a per-tick increment far below one
 * Q15 LSB still accumulates instead of stalling -- the ramp accumulators of if.c, vf.c and
 * rate_limiter.c. Widen with a multiply by Q15_16_SCALE, narrow with a shift by Q15_16_SHIFT (see
 * the note above on why the two directions are not mirror images). */
#define Q15_16_SHIFT  (16)
#define Q15_16_SCALE  (1 << Q15_16_SHIFT)              /* 65536: one Q15 LSB, in Q15.16 counts */
#define PU_ONE_Q15_16 ((float)Q15_ONE * Q15_16_SCALE)  /* 2^31: 1.0 pu, in Q15.16 counts. Float
                                                        * because 2^31 is one past Q31_MAX; used
                                                        * only to derive increments (cold path). */
/* Narrow an intermediate to q15_t by SATURATING, never by wrapping. Every transform below works in
 * int32 and lands somewhere that only holds int16, and the results genuinely can leave [-1,1): the
 * Clarke of three clamped phase currents reaches 4/3, and a dq vector of magnitude 1 rotates into an
 * alpha of up to sqrt(2). A plain cast turns those into a SIGN FLIP -- and a sign-flipped current
 * measurement is a current loop with positive feedback, which is how an overcurrent becomes a
 * runaway instead of a clamp. Saturation keeps the wrong answer on the right side of zero. */
static inline q15_t q15_sat(int32_t v) {
    if(v > Q15_MAX) return (q15_t)Q15_MAX;
    if(v < Q15_MIN) return (q15_t)Q15_MIN;
    return (q15_t)v;
}
typedef uint16_t angle_t;                                            /* electrical angle as turns: whole circle == 2^16 (BAM); rad = a/2^16 * 2pi. Wraps mod 2pi for free. */
/* Q32 phase accumulators (if.c, vf.c, pll.c): the whole circle is 2^32 counts -- one too many for
 * 32 bits, hence the long long -- and angle_t is simply their top 16. Integrating there rather than
 * in angle_t keeps per-tick increments far below one angle_t LSB from being lost to rounding. */
#define FULL_360DEG_Q32 (1LL << 32)
#define ANGLE_SHIFT     (16)                                         /* Q32 accumulator -> angle_t */
#define DEG_Q32(deg)    ((uint32_t)((int64_t)(deg) * FULL_360DEG_Q32 / 360))   /* degrees -> Q32 counts */
#define DEG_BAM(deg)    ((angle_t)(DEG_Q32(deg) >> ANGLE_SHIFT))               /* degrees -> angle_t counts; DEG_BAM(90) is the quarter turn */
#define PU_ONE_Q32      ((float)FULL_360DEG_Q32 / Q15_ONE)           /* 2^17: Q32 counts per Q15 speed LSB, i.e. what turns w_base*Ts/2pi (turns per tick at 1.0 pu) into the per-tick angle increment k_theta */
typedef q15_t   spd_pu_t;                                            /* electrical speed, per-unit of base_t.w_base (Q15, see mc/base.h) */
typedef int32_t udc_pu_t;                                            /* DC-link voltage, per-unit of base_t.u_base (Q13 -- NOT Q15: the bus exceeds 1.0 pu by construction, and further under regen pump-up). The NOMINAL bus is pi/2 pu == SVM_UDC_NOM, since u_base == 2*Udc_nom/pi (see mc/svm.h). */
typedef struct { q15_t a, b, c; } abc_pu_t;                          /* per-unit phase quantities [-1,1) */
typedef struct { q15_t afa, bet; } ab_pu_t;                          /* per-unit alpha/beta (stationary frame) [-1,1) */
typedef struct { q15_t d, q; }    dq_pu_t;                           /* per-unit dq quantities         */
typedef struct { q15_t mag; angle_t angle; } polar_pu_t;             /* per-unit polar vector: magnitude [0,1) + electrical angle (angle_t); a forced voltage or current reference (cf. dq_pu_t, shared across quantities) */
typedef struct { q15_t a, b, c; } duties_t;                          /* normalized phase duties [0,1)  */
#ifdef __cplusplus
}
#endif
#endif
