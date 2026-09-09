#ifndef MC_TRANS_H
#define MC_TRANS_H
#include "types.h"

/* THE PARK ANGLE IS QUANTIZED to the nearest of 1024 steps -- +/-0.176 electrical degrees -- by
 * the flat sine table in trans.c. That is the number to judge this by: weigh it against the
 * position error the observer already carries, not against an LSB count.
 *
 * There used to be an MC_SIN_INTERP switch here selecting a quarter-wave table with interpolation
 * for under 1 LSB. It is gone with the fold it interpolated: the flat table is the same accuracy
 * for fewer cycles and no branch (trans.c), and the finer option had no caller. A build variant
 * nothing in CI exercised is not an option, it is an untested path. */
#ifdef __cplusplus
extern "C" {
#endif
/* THE ALGEBRA LIVES IN THE HEADER, the table does not. These four are leaf arithmetic on structs
 * small enough to travel in registers, and out of line every call spent MORE on the boundary than
 * on the maths: measured on the g431 build, ab2dq ran 35 instructions of which 13 were ABI -- four
 * sxth/asrs unpacking two register-packed structs, five uxth/bfi packing the result back, a frame,
 * and a stack adjustment it never used. abc2ab was worse, opening `strd r0,r1,[sp]` to spill its
 * own arguments and reading them back out. Four of these run on every sensorless FOC tick, so that
 * is ~60 cycles a tick paid to cross a boundary the compiler can simply see through.
 *
 * WHAT IT COSTS: +472 B of flash, because a body is duplicated at each of the ten call sites and
 * only four are on the hot tick -- prod_vf, prod_if, transition_to_foc and the two clamp-only
 * paths pay the size for no cycles. Whether that trade is net positive on THIS part is not settled
 * by counting: a bigger hot path competes for the G4's single-KB instruction cache, and a miss is
 * a flash fetch at four wait states (docs/CYCLES.md, and the CCM item in TODO.md). isr_budget.py
 * is what decides it.
 *
 * sincos_q15 stays OUT of line deliberately -- it is the seam a port swaps for CORDIC (below), and
 * a static inline in a header has no symbol left to replace. */
#define Q15_INV_SQRT3 18919   /* 1/sqrt(3)  * 2^15 */
#define Q15_SQRT3_2   28378   /* sqrt(3)/2  * 2^15 */
static inline q15_t q15mul(q15_t x, q15_t c) { return (q15_t)(((int32_t)x * c) >> 15); }

/* The Clarke's 1/3, as a reciprocal multiply rather than a divide. 43691 == ceil(2^17/3), which
 * reproduces C's truncation-toward-zero EXACTLY for every |num| < 2^17 -- wider than the +-131070
 * three saturated phases can reach -- but the 32-bit product only holds |num| <= 98303.
 *
 * That is not a restriction the caller has to respect, because the clamp below is not an
 * approximation: 3*Q15_MAX == 98301 is precisely where the quotient rails, so clamping the
 * NUMERATOR to +-98301 IS the q15_sat this replaces, moved in front of the multiply rather than
 * after the divide. The one input that changes answer is a numerator at or below -98302, which
 * used to give Q15_MIN and now gives -Q15_MAX: one LSB, at a rail, on a phase set no star
 * connection can produce (all three railed with opposing signs -- a hard fault or an injected
 * sim vector), and both values read as "railed negative" to everything downstream.
 *
 * ARMv7-M has UMULL/SMMUL and GCC forms this reciprocal itself, so on M4 the divide was already
 * free and this is a wash. ARMv6-M has only MULS (32x32 -> low 32), cannot form it, and calls
 * libgcc's shift-subtract loop instead: ~50-100 cycles, twice per tick. */
static inline q15_t clarke_third(int32_t num) {
    if(num >  3*Q15_MAX) num =  3*Q15_MAX;
    if(num < -3*Q15_MAX) num = -3*Q15_MAX;
    uint32_t mag = (num < 0) ? (uint32_t)(-num) : (uint32_t)num;
    int32_t  q   = (int32_t)((mag * 43691u) >> 17);
    return (q15_t)((num < 0) ? -q : q);
}

static inline ab_pu_t abc2ab(abc_pu_t i) {
    ab_pu_t r;
    r.afa = clarke_third((int32_t)2*i.a - i.b - i.c);     /* (2/3)(a - b/2 - c/2) */
    r.bet = q15_sat(((int32_t)(i.b - i.c) * Q15_INV_SQRT3) >> 15);
    return r;
}

static inline abc_pu_t ab2abc(ab_pu_t x) {
    /* >>1, not /2. The two differ by one LSB on a negative odd alpha -- the shift floors, the
     * divide truncates toward zero -- and both sit 0.5 LSB from alpha/2, so neither is the more
     * faithful half. Nothing integrates this (types.h's warning is about the ramp ACCUMULATORS,
     * where a one-LSB bias compounds), so the shift is the same answer without the
     * round-toward-zero fixup C would otherwise mandate: three instructions down to one on a
     * core with no hardware divide. */
    q15_t half_a = (q15_t)(x.afa >> 1);
    q15_t k      = q15mul(x.bet, Q15_SQRT3_2);   /* (sqrt(3)/2) beta */
    abc_pu_t r;
    r.a = x.afa;
    r.b = q15_sat(-half_a + k);
    r.c = q15_sat(-half_a - k);
    return r;
}

/* Park takes the ROTATION, not the angle it came from.
 *
 * An angle_t parameter hides the table lookup INSIDE the transform, so a caller that rotates
 * twice about one angle pays for two decodes with nothing at the call site saying so -- and
 * two of the seven call sites in this drive did exactly that (cc_step's forward/anti-windup
 * pair, transition_to_foc's current/voltage pair). Naming the decode makes the second one
 * visible; there is deliberately NO angle-taking form to fall back to, because the convenient
 * spelling is the one the next duplicate would be written in.
 *
 * It is also the seam a port swaps. sincos_q15 is the only thing here that knows a table
 * exists, so a G4's CORDIC replaces ONE function instead of reaching into two transforms. */
typedef struct { q15_t c, s; } sincos_t;   /* cos and sin of one angle, Q15 */
sincos_t sincos_q15(angle_t theta);
/* The Parks, inline for the reason given above; they take the ROTATION, never the angle. */
static inline dq_pu_t ab2dq(ab_pu_t x, sincos_t cs) {
    dq_pu_t r;
    r.d = q15_sat( q15mul(x.afa, cs.c) + q15mul(x.bet, cs.s));
    r.q = q15_sat(-q15mul(x.afa, cs.s) + q15mul(x.bet, cs.c));
    return r;
}
static inline ab_pu_t dq2ab(dq_pu_t x, sincos_t cs) {
    ab_pu_t r;
    r.afa = q15_sat(q15mul(x.d, cs.c) - q15mul(x.q, cs.s));
    r.bet = q15_sat(q15mul(x.d, cs.s) + q15mul(x.q, cs.c));
    return r;
}
#ifdef __cplusplus
}
#endif
#endif
