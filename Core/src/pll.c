#include "pll.h"

static const float TWO_PI = 6.2831853071795865f;

/* Cold-path float->Q15 with saturation (mirrors if.c / smo.c). */
static q15_t to_q15_sat(float x) {
    float v = x * (float)Q15_ONE;
    if(v > (float)Q15_MAX) v = (float)Q15_MAX;
    if(v < (float)Q15_MIN) v = (float)Q15_MIN;
    return (q15_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

/* Round a per-step angle increment to int32, clamped to [1, hi] (cf. if.c): a nonzero
 * rate must progress, and speed*k_theta must stay inside int32. */
static int32_t to_inc_sat(float v, int32_t hi) {
    if(v < 1.0f) return 1;
    if(v > (float)hi) return hi;
    return (int32_t)(v + 0.5f);
}

/* --- THE LOW-SPEED FLOOR (see pll.h) -------------------------------------------------
 * arg(e) is scale-invariant, so the detector needs no |e| to measure an angle and takes no
 * square root to find one. What it does need is a floor: the argument of a near-zero vector
 * is full-scale noise delivered at full gain, so below PLL_E_MIN the error is scaled down
 * and the loop stops trusting what it cannot measure. Costless here -- the drive is in
 * forced I-f below the handover (s = 0.15 pu, |e| = 0.128 pu on BL4260) and the transition
 * is agreement-gated, so the loop only has to be right ABOVE this floor.
 *
 * STATED AS A SQUARE. |e| only ever gets compared against a threshold or divided by the
 * floor, and both survive squaring -- x >= t iff x^2 >= t^2 for non-negative x. So the
 * detector carries |e|^2 (Q30, the sum of two Q15 squares) and never takes the root that
 * would turn it back into |e|: ~250 cycles per tick that bought a number nothing needed in
 * that form. conv.c squares its threshold once at tune time (conv.h) and the observers
 * publish the square (smo.h), so the comparison is the same comparison throughout. */
#define PLL_E_MIN     1638                                  /* 0.05 * 32768: 0.05 pu floor on |e| */
#define PLL_E_MIN_SQ  ((uint32_t)PLL_E_MIN * PLL_E_MIN)      /* 2683044: that floor, squared [Q30] */

/* Speed either side of which the detector commits to a rotation DIRECTION (Q15 pu of
 * w_base, 0.02). Below it the sign of spd_est is meaningless -- at standstill the input
 * vector has no direction to have -- so dir HOLDS its last value through the band rather
 * than tracking a sign that would dither, which would swap the loop's attractor for its
 * saddle every few ticks. Sized between the two speeds that bracket it: well above the
 * speed-estimate noise, and ~6x below the I-f handover (0.125 pu on BL4260) so the
 * direction is long since committed by the time the transition gate can fire. Nothing
 * below the |e| floor (0.05 pu of |e| ~ 0.058 pu of speed) is trusted anyway -- the drive
 * is in forced I-f down there and the gate is what keeps it there. */
#define PLL_DIR_HYST  655        /* 0.02 * 32768 */

/* REC[i] = round(2^29 / (16384 + i*256)), i = 0..64: the reciprocal of a mantissa
 * normalised to [0.5, 1.0) in Q15, sampled on 64 intervals for linear interpolation
 * (the SIG[65]/sine-LUT shape used elsewhere in the core). 1/m is smooth there
 * (|f''| = 2/m^3 <= 16), so 64 intervals hold the result inside 1 LSB. Its one caller is
 * atan2_bam below, which needs it for the num/den ratio, not for any magnitude. Descending,
 * 32768 -> 16384; uint16_t because the first entry does not fit int16. 130 B of flash. */
static const uint16_t REC[65] = {
    32768, 32264, 31775, 31301, 30840, 30394, 29959, 29537,
    29127, 28728, 28340, 27962, 27594, 27236, 26887, 26546,
    26214, 25891, 25575, 25267, 24966, 24672, 24385, 24105,
    23831, 23564, 23302, 23046, 22795, 22550, 22310, 22075,
    21845, 21620, 21400, 21183, 20972, 20764, 20560, 20361,
    20165, 19973, 19784, 19600, 19418, 19240, 19065, 18893,
    18725, 18559, 18396, 18236, 18079, 17924, 17772, 17623,
    17476, 17332, 17190, 17050, 16913, 16777, 16644, 16513,
    16384,
};

/* ATAN[i] = round(atan(i/64) * 2^16/2pi), i = 0..64: the first OCTANT in BAM, 0 -> 8192
 * (45 deg), sampled on 64 intervals for linear interpolation -- the SIG[]/REC[]/sine-LUT
 * shape used everywhere else in the core. atan is smooth on [0,1] (|f''| <= 0.65), so 64
 * intervals hold it well inside an LSB. 130 B of flash. */
static const uint16_t ATAN[65] = {
        0,   163,   326,   489,   651,   813,   975,  1136,
     1297,  1457,  1617,  1775,  1933,  2090,  2246,  2401,
     2555,  2708,  2860,  3010,  3159,  3307,  3453,  3599,
     3742,  3884,  4025,  4164,  4302,  4438,  4572,  4705,
     4836,  4966,  5094,  5220,  5344,  5467,  5589,  5708,
     5826,  5943,  6058,  6171,  6282,  6392,  6500,  6607,
     6712,  6815,  6917,  7018,  7117,  7214,  7310,  7405,
     7498,  7589,  7679,  7768,  7856,  7942,  8026,  8110,
     8192,
};

/* atan2 in BAM (full circle == 2^16), DIVISION-FREE like everything else here: octant fold,
 * then one ratio through the REC[] reciprocal above, then ATAN[]. Fold first so the table
 * only has to cover [0,1] -- outside the first octant the ratio would run to infinity.
 *
 * The normalisation is FREE here: shifting num and den together leaves their RATIO alone,
 * so there is no exponent to carry and undo afterwards. Both stay under 2^15 because
 * num <= den by construction. This is the whole reason the detector needs no |e| -- the
 * only normalisation an argument wants is one that cancels.
 *
 * Worst error 2 BAM (0.011 deg), swept over 3600 angles at ten magnitudes down to |e| = 1
 * LSB plus the axis and diagonal corners -- see test_pll.cpp, which re-runs that sweep
 * against the host's atan2. */
static int32_t atan2_bam(int32_t y, int32_t x) {
    int32_t ax = x < 0 ? -x : x, ay = y < 0 ? -y : y;
    /* q15_t reaches -32768, whose magnitude is one past Q15_MAX. Clamping costs 1 LSB of a
     * ratio and is what keeps both table indices <= 63 (idx 64 would read one entry past
     * the interpolation endpoint). */
    if(ax > Q15_MAX) ax = Q15_MAX;
    if(ay > Q15_MAX) ay = Q15_MAX;
    if((ax | ay) == 0) return 0;      /* no vector, no angle; the |e| floor is the caller's */
    int32_t num = ay < ax ? ay : ax;
    int32_t den = ay < ax ? ax : ay;
    while(den < Q15_ONE / 2) { den <<= 1; num <<= 1; }
    int32_t r = den - Q15_ONE / 2, idx = r >> 8, frac = r & 0xFF;
    int32_t rec = (int32_t)REC[idx] - ((((int32_t)REC[idx] - (int32_t)REC[idx + 1]) * frac) >> 8);
    /* rec ~ 2^29/den, so num*rec = (num/den)*2^29 and >>14 lands the ratio in Q15. Both
     * factors are under 2^15, so the product is under 2^30: inside int32. */
    int32_t ratio = (num * rec) >> 14;
    if(ratio > Q15_MAX) ratio = Q15_MAX;               /* a floored rec can nudge past 1.0 */
    int32_t i = ratio >> 9, f = ratio & 0x1FF;
    int32_t a = (int32_t)ATAN[i] + ((((int32_t)ATAN[i + 1] - (int32_t)ATAN[i]) * f) >> 9);
    if(ay > ax) a = DEG_BAM(90) - a;  /* mirror about 45 deg: atan(x/y) was the ratio taken */
    if(x < 0)   a = DEG_BAM(180) - a;
    if(y < 0)   a = -a;
    return a & 0xFFFF;
}

/* BAM -> Q15 radians: 1 BAM = 2pi/2^16 rad, so Q15 radians = bam * pi. pi in Q13; a full
 * half-circle input (32768) gives 8.4e8, inside int32. */
#define PLL_BAM2RAD_Q13  25736
/* 2^31/PLL_E_MIN_SQ, for the below-floor trust scale: (sq * this) >> 16 is sq/PLL_E_MIN_SQ
 * in Q15 (0.05% low at the floor itself, where it reads 32754 against 32768). 800 is the
 * largest multiplier that keeps the product inside int32 across sq < PLL_E_MIN_SQ. */
#define PLL_E_MIN_SQ_RECIP  800

/* The atan phase detector (pll.h): the wrapped difference between the angle the EMF vector
 * ACTUALLY has and the one the loop is holding. No Park -- the argument is taken in the
 * stationary frame, so the two trig lookups and four multiplies of ab2dq are not spent. */
static int32_t atan_phase_err(ab_pu_t e, angle_t theta_est, int32_t dir, uint32_t *sq_out) {
    /* |e|^2 is not needed to MEASURE the angle -- an argument carries no magnitude -- but the
     * floor below needs it, and so does mc.c's RESYNC gate through e_mag_sq. Two Q15 squares
     * reach 2^31, one past INT32_MAX, hence uint32. No clamp: |e| had to be narrowed into a
     * q15_t and so had to be capped at Q15_MAX, but the square is published in the width it
     * is computed in, and a railed pair (1.414 pu, 2^31) still fits. */
    uint32_t sq = (uint32_t)(e.afa * e.afa) + (uint32_t)(e.bet * e.bet);
    if(sq_out) *sq_out = sq;
    /* The vector points at theta + 90*sign(w), so the rotor angle is its argument with that
     * quarter turn taken back off -- dir is the loop's standing answer to sign(w) (pll.h). */
    int32_t theta_meas = atan2_bam(e.bet, e.afa) - dir * DEG_BAM(90);
    /* BAM subtraction IS the shortest-path wrap: the int16 cast picks the representative in
     * [-180, 180), so a 359 deg error reads as -1 deg (the idiom mc.c's cv_deg uses). */
    int32_t err = (int16_t)(uint16_t)(theta_meas - (int32_t)theta_est);
    err = (err * PLL_BAM2RAD_Q13) >> 13;
    /* Saturate at +-1.0 rad -- the range sin spans, so every downstream bound still holds
     * (pll.h). This is where a linear detector stops being linear, and it is the ONLY place
     * it differs from sin inside 57 deg. */
    if(err > Q15_MAX) err = Q15_MAX;
    if(err < Q15_MIN) err = Q15_MIN;
    /* Below the floor, fall back to a speed-proportional gain -- an argument has no magnitude
     * to freeze, so the taper is applied here explicitly (pll.h). Stated on the SQUARE it is
     * (|e|/PLL_E_MIN)^2 rather than |e|/PLL_E_MIN: full trust at the same place, zero at the
     * same place, steeper in between -- and steeper is the safe direction for a mute whose
     * job is to stop the loop chasing an angle it cannot resolve. */
    if(sq < PLL_E_MIN_SQ) err = (err * (int32_t)((sq * PLL_E_MIN_SQ_RECIP) >> 16)) >> 15;
    return err;
}

void pll_tune(pll_t *pll, const base_t *b, float w_pll, float zeta, float Ts) {
    if(!pll) return;
    *pll = (pll_t){0};                              /* clear coefficients + state */
    pll->dir = 1;                     /* dir = 0 is not a valid sign; assume forward */
    if(!b || Ts <= 0.0f || b->w_base <= 0.0f) return;
    pll->kp      = to_q15_sat(2.0f * zeta * w_pll / b->w_base);
    pll->ki      = to_q15_sat(w_pll * w_pll / b->w_base * Ts);
    pll->k_theta = to_inc_sat(b->w_base * Ts / TWO_PI * PU_ONE_Q32, 65535);   /* 2^17 */
}

/* STATE only -- keep control parameters */
void pll_init(pll_t *pll) {
    if(!pll) return;
    pll->theta_acc = 0;
    pll->integ     = 0;
    pll->spd_est = 0;
    pll->e_mag_sq  = 0;
    pll->theta_err = 0;
    /* Forward is the ASSUMPTION, not a requirement: with spd_est cleared to 0 there is no
     * direction to restore, and a reverse run simply flips it a few ticks in (pll.h). */
    pll->dir       = 1;
}

/* Anti-windup bound on the speed integrator: +-1.0 pu of speed in the integrator's Q30, i.e.
 * exactly the Q15 range the narrowing below can hold. Widened with a MULTIPLY by Q15_ONE, not
 * a shift -- Q15_MIN is negative, and a left shift of a negative signed value is undefined
 * (types.h).
 *
 * It stood at Q31_MAX/Q31_MIN, which is the ACCUMULATOR's range rather than the SPEED's: a
 * factor of two wide, and unreachable besides -- integ is int32_t, so `integ > Q31_MAX` never
 * fired and both lines compiled to nothing. Past 1.0 pu the cast below then wrapped, reading an
 * estimate a hair over +1.0 pu as -1.0 -- which flips dir, hands the loop the root 180 out, and
 * lands i_q_ref on -i_q (the inverted-frame failure pll.h describes). Saturating keeps the
 * estimate at the ceiling: wrong by however far past 1.0 pu the machine is, and no further.
 *
 * A CEILING, NOT HEADROOM. The loop still cannot REPORT a speed at or above 1.0 pu; that is
 * w_base's job (machines.c), not the clamp's. */
#define PLL_INTEG_MAX  ((int32_t)Q15_MAX * Q15_ONE)   /* +0.99997 pu, Q30 */
#define PLL_INTEG_MIN  ((int32_t)Q15_MIN * Q15_ONE)   /* -1.0 pu,     Q30 */

angle_t pll_step(pll_t *pll, ab_pu_t eab_est) {
    if(!pll) return 0;
    /* Phase detector: the EMF estimate resolved in the present estimated frame, then
     * NORMALIZED by |e|. e_d = |e|*sin(theta_est-theta), so -dir*e_d/|e| is the phase error
     * itself, in RADIANS -- which is the quantity pll_tune's gains are derived for
     * (pll.h). Both Park components are used: .q is half of |e|, so the magnitude costs
     * no extra trig. */
    /* Commit to a rotation DIRECTION before detecting phase: which root of e_d = 0 is the
     * attractor is the detector's sign to choose, and only sign(w) makes it the rotor's
     * north pole rather than its south (pll.h). Hysteresis band -> hold, so the sign never
     * dithers where it is undefined. Uses LAST tick's spd_est, which is what makes this a
     * plain feedback path and not an algebraic loop. */
    if(pll->spd_est >  PLL_DIR_HYST) pll->dir =  1;
    if(pll->spd_est < -PLL_DIR_HYST) pll->dir = -1;
    angle_t theta = (angle_t)(pll->theta_acc >> ANGLE_SHIFT);
    int32_t theta_err = atan_phase_err(eab_est, theta, pll->dir, &pll->e_mag_sq);
    /* A wrapped angle difference IS the error, so its SIZE already says which root we are
     * near: 180 deg out reads as 180 deg out, not as zero. Publish it as measured. */
    pll->theta_err = (q15_t)theta_err;
    /* Harnefors tap: the INTEGRATOR is the speed estimate w_est = Ki*int(err) (integ Q30,
     * ki folds Ts, cf. pi2dof.c; integrator-clamp anti-windup keeps w_est inside Q15). */
    pll->integ += (q31_t)pll->ki * theta_err;
    if(pll->integ > PLL_INTEG_MAX) pll->integ = PLL_INTEG_MAX;
    if(pll->integ < PLL_INTEG_MIN) pll->integ = PLL_INTEG_MIN;
    pll->spd_est = (spd_pu_t)(pll->integ >> 15);  /* integrator == w_est, already Q15 */
    /* Angle rate = w_est + Kp*err, with the Kp correction going onto the POSITION
     * integrator, not through w_est. Clamp to Q15 before the k_theta multiply so the
     * overflow bound below still holds when P sits atop a saturated integrator. */
    int32_t w_ang = pll->spd_est + (((int32_t)pll->kp * theta_err) >> 15);
    if(w_ang > Q15_MAX) w_ang = Q15_MAX;
    if(w_ang < Q15_MIN) w_ang = Q15_MIN;
    /* theta_est integrates that rate with if.c's accumulator (full circle == 2^32,
     * k_theta = w_base*Ts/2pi*2^17). Rate is Q15-bounded and k_theta < 2^16, so the
     * signed product stays inside int32 (the |w|*Ts < pi limit) and wraps mod 2^32. */
    pll->theta_acc += (uint32_t)(w_ang * pll->k_theta);
    return (angle_t)(pll->theta_acc >> ANGLE_SHIFT);   /* export the ANGLE (top 16), not the accumulator */
}


spd_pu_t pll_get_spd(const pll_t *pll) { return pll ? pll->spd_est : 0; }
angle_t    pll_get_angle(const pll_t *pll) { return pll ? (angle_t)(pll->theta_acc >> ANGLE_SHIFT) : 0; }
