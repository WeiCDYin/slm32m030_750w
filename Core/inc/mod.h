#ifndef MC_MOD_H
#define MC_MOD_H
#include "types.h"
#include "trans.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MOD_UDC_NOM      12868  /* pi/2 pu == the nominal bus (2*Udc_nom/pi == u_base) */
#define MOD_UDC_PU_MIN    4503  /* 0.35x nominal */
#define MOD_UDC_PU_MAX   32170  /* 2.50x nominal */
#define MOD_DUTY_MID    (Q15_ONE / 2)   /* 0.5 in [0,1) Q15 (1.0 == 32768) */
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
 * THEY STAY INSIDE mod() WHEREVER THEY COME FROM. A port that clipped the duties afterwards
 * would apply less voltage than the core believed it had, mod_realized_ref would report the vector
 * that was asked for rather than the one applied, and the current loop would wind up against a
 * ceiling nothing told it about. Setting them here keeps the readback and the clamp flag honest.
 *
 * WHAT IS IN FORCE BEFORE ANYONE CALLS mod_set_cfg: SVPWM, a floor of 0 and a ceiling of 0.95 of
 * the switching period. The 95% is a paper number pending the bench sweep (TODO.md, "low-side
 * sampling window"); the 0 is the floor a board with no minimum-pulse requirement wants. A port
 * that configures nothing modulates at that pair -- mod.c holds it as the initializer of the
 * statics the tick reads, so the default costs no call and cannot be forgotten.
 *
 * THOSE TWO NUMBERS ARE NOT MACROS HERE, deliberately, and this paragraph is the whole of what
 * this header can honestly say about them. A `MOD_DUTY_MAX_DEFAULT` in inc/ would read as the
 * archive's default and would not be one: lib/libmc_core.a was compiled with its value already
 * folded into .data, so editing a macro in a shipped header changes only what YOUR code passes to
 * mod_set_cfg -- never what the core falls back to. The two would then disagree with nothing to
 * report it, because MC_CORE_HASH covers the archive's objects and not these headers. What you
 * pass to the setter is what takes effect; that is the only mechanism there is, so it is the only
 * one named.
 *
 * AND A BOARD SHOULD BE TYPING ITS OWN NUMBERS ANYWAY. The same fraction is a different TIME on
 * every board -- 2% is 3.3 us at a 6 kHz carrier and 1.0 us at 20 kHz -- so a port naming a core
 * constant is a port that has not yet asked the question the limit exists to answer. */
/* Duty fraction -> the Q15 the clamp works in, ROUNDED, matching the to_q15_sat every other cfg
 * in the core converts through (if.c, sc.c, smo.c, pll.c). One conversion for both paths is what
 * makes "configured 0.95" and "never configured" the same LSB rather than 31130 and 31129.
 *
 * SATURATED AT THE TOP because 1.0 is a duty a person may reasonably write and 1.0 is NOT a
 * q15_t: full scale is Q15_ONE == 32768 and the type stops one LSB below it. Saturating gives a
 * 100% ask the longest pulse the type can express (0.99997) instead of wrapping it to -1.0, which
 * is the same trap the old q15_t cfg sprang. No bottom arm: a negative duty never reaches here,
 * because mod_set_cfg rejects one.
 *
 * A CONSTANT EXPRESSION, so the statics in mod.c fold at compile time and a port that configures
 * nothing pays no float at runtime. (pu) is read three times -- write a constant, not a call. */
#define MOD_DUTY_Q15(pu)        ((q15_t)(((pu) * Q15_ONE + 0.5f) > (float)Q15_MAX ?                                          (float)Q15_MAX : ((pu) * Q15_ONE + 0.5f)))
#define MOD_DUTY_ZERO   ((duties_t){ MOD_DUTY_MID, MOD_DUTY_MID, MOD_DUTY_MID })
typedef int32_t mod_gain_t;

/* THE MODULATION SCHEME -- which common mode the modulator injects, and nothing else. All three
 * apply the SAME differential voltage for the same command: they move the three duties TOGETHER,
 * which is the one thing the motor cannot see and the power stage can. The map is
 *
 *     duty_x = duty_base + k_fwd*(u_x - u0)
 *
 * and a scheme is only the pair (duty_base, u0); see mod.c. Nothing downstream changes -- the
 * clamp, the clamp flag and mod_realized_ref all stand, the last because its Clarke annihilates both
 * terms by construction.
 *
 *   MOD_SVPWM    u0 = (max+min)/2. Centered min/max injection: what every release has shipped and
 *                what a build that sets nothing gets. Best ripple; all three legs switch every
 *                carrier period.
 *   MOD_SPWM     u0 = 0. No injection at all, so the duty waveform IS the command. Least linear
 *                range of the three -- it clamps at pi/4 of the six-step base where centered
 *                reaches pi/(2*sqrt3), i.e. 87% as far. It earns its place as the REFERENCE the
 *                other two are measured against, not as a way to run a motor.
 *   MOD_DPWMMIN  u0 = min. The LOWEST leg is parked on the duty floor and stops switching for 120
 *                electrical degrees of every cycle, each leg taking its turn: a third fewer
 *                switching transitions, and a parked leg contributes no dead-time error while it
 *                sits there. Its low side is held ON throughout, which is the widest the low-side
 *                shunt's window ever gets. It costs ripple -- the same carrier gives a larger
 *                current excursion than centered -- and that is the trade being made.
 *
 * THE CEILING-PARKING MIRROR OF DPWMMIN IS NOT OFFERED. Parking a leg at the duty MAX saves the
 * same switching on paper, and on a stage that senses current through low-side shunts it is
 * unusable for the reason the duty ceiling above already gives: a leg held at the top never turns
 * its low side on, so the shunt carries nothing for 120 degrees of every cycle and a bootstrapped
 * high-side supply has no window to refresh in.
 *
 * DPWMMIN also reaches FURTHER before it clamps whenever the duty window is asymmetric, which is
 * the case on any board with a ceiling below 100%: centered can only swing +-(max - mid) about
 * mid, spending 2*(max - mid) of the window, while floor-parked spends all of (max - min). At the
 * default [0, 0.95] that is 0.95 against 0.90 -- ~5% more linear range for nothing.
 *
 * SETTABLE AT RUNTIME, and for exactly the reason the duty limits above are: whoever holds a
 * BUILT ARCHIVE has to be able to choose it. A -D can only be set by whoever compiled the core,
 * and the audience a release goes to receives dist/ -- headers and a library -- so a compile-time
 * scheme would be a choice they could read about here and never make. It costs the carrier tick
 * one compare against a value that has not moved since composition, which is the price of the
 * choice being theirs rather than the compiler's. */
typedef enum {
    MOD_SVPWM   = 0, 
    MOD_SPWM    = 1,
    MOD_DPWMMIN = 2,
} mod_scheme_t;

/* EVERYTHING THE MODULATOR IS CONFIGURED WITH, in one object. The scheme and the duty window are
 * one decision, not three: which legs park and how far they may swing are both answers to the same
 * question about a power stage, and DPWMMIN parks ON mod_min_duty, so a scheme set without its
 * window -- or a window set without its scheme -- is half a configuration. One object also means
 * one validation and one all-or-nothing write, so there is no ordering in which a caller can leave
 * the modulator holding one half of an old setting and one half of a new one.
 *
 * Matches cc_cfg_t / sc_cfg_t / if_cfg_t in being what a PERSON authors, and in being authored
 * in the units the quantity is stated in rather than the ones the tick works in: a duty is a
 * fraction of the switching period, so it is a float here and a Q15 inside mod.c, converted once
 * on the cold path. Authoring the Q15 directly is what used to make a plain 0.95 read as
 * `(q15_t)(Q15_ONE * 0.95f)` at the one call site that sets this (a port's board file), and what
 * made a fumbled full scale unrejectable -- see mod_set_cfg. */
typedef struct {
    float        mod_max_duty;   /* duty ceiling, as a fraction of the period -- the stage's, above */
    float        mod_min_duty;   /* duty floor; 0 unless the gate driver needs a minimum pulse */
    mod_scheme_t mod_scheme;     /* which common mode to inject */
} mod_cfg_t;
/* Take a configuration. COLD PATH -- call once at composition, before the bridge is energized.
 * Not because a later call is dangerous: the common mode is precisely what the motor cannot see,
 * so a change mid-run applies no voltage step. It moves every leg's sampling window and dead-time
 * error at once, though, which belongs with the rest of composition rather than mid-run.
 *
 * Returns 1 if the whole cfg was taken, 0 if ANY field was rejected -- in which case NOTHING is
 * written and the previous configuration stands entire. REJECTS rather than repairs: a caller that
 * fumbles the units must not be able to silently narrow the modulator to nothing, nor a caller
 * that invents a scheme number be able to silently re-modulate the drive. Rejected are a NULL cfg,
 * a duty outside [0, 1], mod_min_duty >= mod_max_duty, a window that excludes half duty, any
 * scheme that is not one of the three -- and a NaN in either duty, which is why mod.c asks each
 * bound in the negated form.
 *
 * WHY THE WINDOW MUST STILL CONTAIN MID DUTY, now that not every scheme parks the zero vector
 * there: the centered schemes do, so limits that cannot express mid duty would make standstill
 * unreachable for two of the three. DPWMMIN parks its zero vector on the floor instead and does
 * not need mid, but a window that excludes it is a misconfiguration under any scheme -- it is less
 * than half the duty range in one direction.
 *
 * THE CEILING IS 1.0 AND IS TESTED, which it could not be while these were q15_t: full scale is
 * 32768 and does not fit the type, so the value a fumbling caller passed had already wrapped to
 * -32768 before the function was entered. A float says 1.0 plainly. */
int mod_set_cfg(const mod_cfg_t *c);
mod_gain_t mod_calc_gain(udc_pu_t udc_meas);
/* BY OUT-POINTER, unlike the transforms in trans.h, which are pure value-returning maps.
 * The reason is ARMv6-M: `*duty = mod(...)` makes the callee fill its hidden return buffer and
 * the caller then copy 6 bytes out of it, and a core with no unaligned access cannot expand that
 * copy inline -- it is a `bl memcpy`, ~100 cycles, on the current loop's every-tick path. Writing
 * `out` member by member costs nothing on either core. `out` must not be NULL and is not checked.
 * See docs/CYCLES.md 8; section 7's `mod` rework belongs in the same pass as this. */
/* Returns NONZERO if any leg clamped. That one bit is what lets the caller skip mod_realized_ref:
 * the forward map and the inverse are exact inverses of each other, so with no clamp the round
 * trip returns the vector it was handed (see mod.c). Nearly free -- the same two limit compares
 * clamp_duty already makes, asked once per leg. The answer has to be EXACT in BOTH directions:
 * a missed clamp hands the caller a voltage the motor never got, and a clamp reported where
 * none happened costs it ~238 cycles recovering a vector it already had. That rules out the
 * cheaper `(da|db|dc) > max`, which was exact only while the limit was the all-ones
 * mask 0x7FFF: below it, two legs straddling MOD_DUTY_MID OR to 0x7FFF at ZERO modulation and
 * the flag fires on ~68% of ticks. */
int  mod(ab_pu_t uab_ref, mod_gain_t k_fwd, duties_t *out);
ab_pu_t  mod_realized_ref(duties_t dabc, udc_pu_t udc_meas);
#ifdef __cplusplus
}
#endif
#endif
