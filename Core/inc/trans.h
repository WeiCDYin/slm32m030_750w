#ifndef MC_TRANS_H
#define MC_TRANS_H
#include "types.h"

/* Sine LUT interpolation in the Park transforms (core/src/trans.c).
 *   0 -- round the angle to the nearest table entry. Fewer cycles: it drops a load, a
 *        subtract, a multiply and a shift per sine, and the hot path takes four sines per
 *        carrier tick (two sincos_q15, two each).
 *   1 -- interpolate between the two neighbouring entries. Error < 1 LSB.
 *
 * What 0 costs is ANGLE RESOLUTION, and that is the number to judge it by: the table steps
 * 90 deg / 256, so rounding quantizes the Park angle to +/-0.176 electrical degrees. Weigh
 * that against the position error the observer already carries, not against the LSB count.
 * A carrier-tick budget decision, so it is compile-time and not a runtime flag -- no branch
 * survives into the ISR either way.
 *
 * The default lives HERE so that trans.c and test_transforms.cpp cannot disagree about it
 * (the test keys its tolerance off the same macro). For an A/B, define it on the compiler
 * command line -- -DMC_SIN_INTERP=1 -- and this block stands down. */
#ifndef MC_SIN_INTERP
#define MC_SIN_INTERP 0
#endif
#ifdef __cplusplus
extern "C" {
#endif
ab_pu_t  abc2ab(abc_pu_t i);
abc_pu_t ab2abc(ab_pu_t x);

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
dq_pu_t  ab2dq(ab_pu_t x, sincos_t cs);
ab_pu_t  dq2ab(dq_pu_t x, sincos_t cs);
#ifdef __cplusplus
}
#endif
#endif
