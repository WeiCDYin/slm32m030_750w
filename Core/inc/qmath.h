#ifndef MC_QMATH_H
#define MC_QMATH_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

/* isqrt of a Q30 value -> Q15. Shifts/compares/subtracts only: no divide, no CLZ, no FPU,
 * so it is ARMv6-M legal. Binary longhand square root -- 2 radicand bits consumed per
 * iteration, 1 root bit produced, on the identity (2r+b)^2 = (2r)^2 + b*(4r+b); root
 * carries 2r during the loop, hence the |= 2 and the final >>= 1. Exact (floor).
 *
 * CLAMPED to Q15_MAX on the way out. The argument is typically a sum of TWO Q15 squares,
 * which reaches ~2^31 and whose root reaches 46341 (1.414 pu) -- past what q15_t holds. The
 * clamp is what makes the narrowing safe, so callers may treat the result as a true Q15. */
q15_t isqrt_q30(uint32_t v);

/* --- A WIDE gain times a Q15 signal, without a long multiply -------------------------
 * pi2dof's gains are Q15 stored in int32 because the per-unit loop gain reaches HUNDREDS
 * (a speed loop on REF tunes k_p to 216 pu). Multiplied by a Q15 signal that is a product
 * needing up to 2^38 -- so the C is written in int64, and on ARMv7-M that is one `smull`.
 *
 * ARMv6-M has no long multiply and GCC calls `__aeabi_lmul` instead: 40 instructions, two
 * push/pop pairs, ~50 cycles, NINE TIMES per carrier tick (CYCLES.md 13). The gains cannot
 * be narrowed -- the range is the reason they are wide -- so the MULTIPLY is split instead.
 *
 * Split the wide operand at bit 15. For any int32 `g`, arithmetic shift and mask give
 * exactly  g == (g >> 15) * 2^15 + (g & 0x7FFF)  with the remainder non-negative, so
 *
 *     g*x  ==  ((g >> 15) * x) * 2^15  +  (g & 0x7FFF) * x
 *
 * and floor() distributes over the first term: both forms below are BIT-EXACT against the
 * int64 they replace, not approximations. Two MULS and an add, no call.
 *
 * CONTRACT. `x` is a Q15 signal, |x| <= 2^15. `g` may be any int32 for which (g >> 15) * x
 * fits int32 -- which for q15_mul_shr15 is precisely "the result fits int32", i.e. no new
 * restriction on a caller already storing it in one. The low partial product is bounded by
 * 32767 * 32768 < 2^31 for every input and needs no check.
 *
 * ARMv7-M and hosts take the plain expression: one `smull`/`smlal` beats the split, so this
 * must never cost the M4 anything. */
#if defined(__ARM_ARCH) && (__ARM_ARCH < 7)
static inline int32_t q15_mul_shr15(int32_t g, int32_t x) {
    return (g >> 15) * x + (((g & 0x7FFF) * x) >> 15);
}
static inline int64_t q15_mul_wide(int32_t g, int32_t x) {
    /* MULTIPLY, not << 15: the high partial product is signed and routinely negative, and C99
     * 6.5.7p4 leaves a left shift of a negative undefined whatever the magnitude (types.h). The
     * multiply says the same thing in defined language and costs nothing -- gcc expands a
     * constant power-of-two multiply to the same single shift, and the product is bounded by
     * 2147450880 * 32768, four orders inside int64. */
    return (int64_t)((g >> 15) * x) * Q15_ONE + (int32_t)((g & 0x7FFF) * x);
}
#else
static inline int32_t q15_mul_shr15(int32_t g, int32_t x) {
    return (int32_t)(((int64_t)g * x) >> 15);
}
static inline int64_t q15_mul_wide(int32_t g, int32_t x) {
    return (int64_t)g * x;
}
#endif

#ifdef __cplusplus
}
#endif
#endif
