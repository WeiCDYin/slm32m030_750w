#include "svm.h"
#include "trans.h" /* ab2abc: per-unit phase refs (inverse Clarke) */
/* Min/max (centered) common-mode injection SVPWM. Validated by test_svm.cpp.
 * Per-unit alpha-beta voltage in -> normalized duties [0,1) out (Q15: 1.0 == 32768).
 *   phase refs -> u0 = (max+min)/2 -> duty = 0.5 + k_fwd*(ux - u0),  k_fwd = u_base/Udc.
 *
 * VOLTAGE BASE: 1.0 pu == six-step fundamental amplitude (2*udc_meas/pi) AT NOMINAL BUS, the
 * inverter ceiling. k_fwd makes that mapping exact, so at nominal it IS (2/pi) and the linear
 * SVPWM limit lands at m = pi/(2*sqrt3) ~= 0.907 pu; past that the duties go over the rails and
 * are CLAMPED (basic clamping overmodulation). Off nominal the whole ceiling scales with the bus
 * (0.907*Udc/Udc_nom) and nothing needs telling: the duties clamp sooner and svm_realized reports
 * the smaller vector. Set u_base = 2*Udc_nom/pi for exact current-loop tuning (see cc_tune). */

/* Q14 forward / Q13 inverse rather than Q15 both: over [SVM_UDC_PU_MIN, SVM_UDC_PU_MAX] the two
 * gains then stay inside 16 bits, making every product below a true 16x16 whose int32 result
 * cannot overflow -- ARMv6-M (Cortex-M0) has no long multiply to fall back on. At Q15 the inverse
 * gain would overflow in the six-step corners as soon as the bus passed ~1.3x nominal. */
#define K_FWD_SHIFT 14
#define K_INV_SHIFT 13

/* k_fwd at the nominal bus: (1<<K_FWD_NUM_SHIFT)/SVM_UDC_NOM == (2/pi)*2^14. */
#define K_FWD_NOM       10430
#define K_FWD_NUM_SHIFT 27 /* K_FWD_SHIFT + K_INV_SHIFT: udc_meas is Q13, k_fwd is Q14 */

/* pu volt -> duty, u_base/Udc == 1/udc_meas. The drive's only divide -- and no longer in the hot
 * path: this is the SLOW half, taken once per outer tick (svm.h). */
svm_gain_t svm_calc_gain(udc_pu_t udc_meas)
{
    if (udc_meas < SVM_UDC_PU_MIN || udc_meas > SVM_UDC_PU_MAX)
        return K_FWD_NOM;
    return ((int32_t)1 << K_FWD_NUM_SHIFT) / udc_meas;
}

/* duty -> pu volt, Udc/u_base -- which IS udc_meas, in the Q13 it already arrives in. Expressing
 * the bus in the drive's own voltage base is what makes this the identity: no reciprocal, no
 * constant, nothing to keep in step with the forward gain. */
static int32_t inv_gain(udc_pu_t udc_meas)
{
    if (udc_meas < SVM_UDC_PU_MIN || udc_meas > SVM_UDC_PU_MAX)
        return SVM_UDC_NOM;
    return udc_meas;
}

/* ONE test for both rails: a negative d wraps to a huge unsigned, so the single unsigned
 * compare catches d < 0 and d > Q15_MAX together, and the branch that sorts out WHICH rail is
 * taken only on a tick that actually clamps. */
static q15_t clamp_duty(int32_t d) {
    if((uint32_t)d > (uint32_t)31129) return d < 655 ? 655 : (q15_t)31129;
    return (q15_t)d;
}

static q15_t sat_q15(int32_t x)
{
    if (x > Q15_MAX)
        return Q15_MAX;
    if (x < Q15_MIN)
        return Q15_MIN;
    return (q15_t)x;
}

/* Inverse of the forward map; see svm.h for why this is the right way to get a limit.
 * Forward:  duty_x = 0.5 + k_fwd*(u_x - u0),  clamped to the rails.
 * Averaged over a switching period a leg applies Udc*duty_x, so against the floating star point
 * (which sits at the mean d0) the applied phase voltage is Udc*(duty_x - d0) -- the same
 * expression the pmsm model uses. In per-unit that is  u_x = k_inv*(duty_x - d0),  and the
 * common mode cancels exactly: u0 passes through the SAME k_fwd as u_x, so substituting the
 * forward map gives duty_x - d0 = k_fwd*u_x for any u0. Min/max injection drops out and the
 * recovered voltage is the DIFFERENTIAL one the motor actually sees. Reading the CLAMPED duties
 * is what makes overmodulation show up here rather than being assumed away.
 *
 * NO STAR POINT IS FORMED. The mean duty d0 is what the phase voltages are measured against,
 * and it used to be subtracted from all three -- but d0 is COMMON MODE, and the Clarke on the
 * next line annihilates the zero sequence by construction (test_transforms.cpp:
 * abc2ab_annihilates_the_zero_sequence). Every consumer of this map reads alpha/beta, so the
 * subtraction was work whose only reader threw it away: the duties go straight into the Clarke
 * and the bus gain scales the RESULT instead of the three phases. A divide, three subtracts,
 * two multiplies and two saturations leave the tick with it.
 *
 * Saturates to Q15: the instantaneous phase voltage reaches 1.047 pu in the six-step corners
 * (2/3*Udc), which q15_t cannot hold. Only deep overmodulation reaches it, where the tiny
 * under-report is conservative for anti-windup (it never claims MORE was applied than was).
 *
 * Clipping ALPHA/BETA is also the honest place to do it. Clipping the three PHASES first, as
 * the d0 form had to, clips three numbers that are then mixed -- so the shortfall it reported
 * past the linear limit was partly its own arithmetic. Against a float reference over the whole
 * domain (0.35x to 2.5x bus, m up to 1.3): the old form drifted to 1774 LSB in overmodulation,
 * this one holds <= 6 LSB everywhere. Inside the linear range both are within a handful of LSB,
 * the difference there being that the 1/3 now truncates before the bus gain amplifies it. */
ab_pu_t svm_realized(duties_t duty, udc_pu_t udc_meas) {
    const int32_t k       = inv_gain(udc_meas);
    const ab_pu_t duty_ab = abc2ab((abc_pu_t){ duty.a, duty.b, duty.c });
    ab_pu_t uab_ref_lim;
    uab_ref_lim.afa       = sat_q15(((int32_t)duty_ab.afa * k) >> K_INV_SHIFT);
    uab_ref_lim.bet       = sat_q15(((int32_t)duty_ab.bet * k) >> K_INV_SHIFT);
    return uab_ref_lim;
}
// svm by means of zero sequence injection
int svm(ab_pu_t uab_ref, svm_gain_t k_fwd, duties_t *out) {
    /* No gain has ever been taken -> no volts. Loud by design; see svm.h. Written member-wise
     * rather than `*out = SVM_DUTY_ZERO`, which would copy from .rodata (CYCLES.md 8). */
    if(k_fwd <= 0) { out->a = SVM_DUTY_MID; out->b = SVM_DUTY_MID; out->c = SVM_DUTY_MID; return 1; }
    const int32_t k   = k_fwd;
    abc_pu_t uabc_ref = ab2abc(uab_ref);

    /* INT32 LOCALS, not q15_t. Assigning to a q15_t makes the compiler re-narrow after every
     * store -- one sxth each, six of them -- to keep a value that arrived as a q15_t inside a
     * type it never left. ARMv6-M has no free-shifted operand to hide that in, so it is real
     * there and invisible on M4. Narrow once, at the end, where it means something. */
    const int32_t a = uabc_ref.a, b = uabc_ref.b, c = uabc_ref.c;

    /* Min and max WITHOUT branches. (d >> 31) is all-ones for a negative d and zero otherwise,
     * so d & (d >> 31) is "the difference, but only when it is negative" -- which turns each
     * compare-and-maybe-assign into arithmetic. Five operations give min AND max of a pair.
     *
     * The point is not the instruction count; it is that svm becomes near CONSTANT-TIME (26
     * branches to 9). A WCET budget spends worst cases, and a data-dependent branch chain makes
     * the worst case a function of the vector angle. */
    const int32_t d1 = a - b,  t1 = d1 & (d1 >> 31);
    const int32_t lo = b + t1, hi = a - t1;          /* min(a,b), max(a,b) */
    const int32_t d2 = lo - c, t2 = d2 & (d2 >> 31);
    const int32_t d3 = hi - c, t3 = d3 & (d3 >> 31);
    const int32_t mn = c + t2, mx = hi - t3;

    /* >>1, not /2: the round-toward-zero fixup C mandates for a signed divide costs two extra
     * instructions to shift a COMMON MODE by one LSB, and svm_realized's Clarke annihilates the
     * common mode exactly -- u0 passes through the same k_fwd as u_x, so it cancels out of the
     * differential voltage the motor sees whatever value it takes. */

    const int32_t u0 = (mx + mn) >> 1;
    const int32_t da = SVM_DUTY_MID + (((a - u0) * k) >> K_FWD_SHIFT);
    const int32_t db = SVM_DUTY_MID + (((b - u0) * k) >> K_FWD_SHIFT);
    const int32_t dc = SVM_DUTY_MID + (((c - u0) * k) >> K_FWD_SHIFT);
    out->a = clamp_duty(da);
    out->b = clamp_duty(db);
    out->c = clamp_duty(dc);
    /* DID ANY LEG CLAMP -- in two ORs and one compare, and exact.
     *
     * As unsigned, a duty inside [0, Q15_MAX] has no bit above bit 14, so OR-ing three of them
     * stays inside the range; a duty outside it is either negative (all high bits set) or over
     * Q15_MAX, and OR can only set bits, so either one carries the result out. The test is
     * therefore the same one clamp_duty just made on each leg, asked once. */
    return ((uint32_t)da | (uint32_t)db | (uint32_t)dc) > (uint32_t)Q15_MAX;
}
