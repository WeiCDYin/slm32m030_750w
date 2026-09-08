#include "trans.h"
/* Clarke/Park in Q15, AMPLITUDE-INVARIANT (gain 2/3). Validated by test_transforms.cpp.
 * No float in the core: Park uses a fixed-point sine LUT (CORDIC could replace it). */

/* Q15 fixed-point constants. */
#define Q15_INV_SQRT3 18919   /* 1/sqrt(3)  * 2^15 */
#define Q15_SQRT3_2   28378   /* sqrt(3)/2  * 2^15 */

static inline q15_t q15mul(q15_t x, q15_t c) { return (q15_t)(((int32_t)x * c) >> 15); }

/* Quarter-wave Q15 sine LUT: SIN_Q[k] = round(32767*sin(k/256 * pi/2)), k = 0..256.
 * angle_t spans the full circle in 2^16, so a quadrant is 14 bits: the top 8 index the
 * table, the low SIN_LUT_FRAC_BITS place the angle BETWEEN two entries.
 *
 * MC_SIN_INTERP picks what happens with those low bits; it defaults in trans.h, which is
 * also where the reason to move it lives:
 *   1 -- linear interpolation between the two neighbours. Error < 1 LSB.
 *   0 -- round to the nearest entry and drop them. Saves a load, a subtract, a multiply
 *        and a shift per sine. What it costs is angle resolution, and that is the number
 *        to judge it by: the table steps 90 deg / 256 == 0.352 deg, so rounding quantizes
 *        the Park angle to +/-0.176 ELECTRICAL DEGREES. (In the sine that is +/-101 Q15
 *        LSB, worst case near zero crossing where the step is widest.)
 * Same table either way: this trades angle accuracy for carrier-tick cycles, not flash. */
#define SIN_LUT_FRAC_BITS 6     /* quadrant-phase bits below one table entry */
#define SIN_LUT_LAST      256   /* SIN_Q[SIN_LUT_LAST] == sin(90 deg) */
static const q15_t SIN_Q[257] = {
         0,    201,    402,    603,    804,   1005,   1206,   1407,
      1608,   1809,   2009,   2210,   2410,   2611,   2811,   3012,
      3212,   3412,   3612,   3811,   4011,   4210,   4410,   4609,
      4808,   5007,   5205,   5404,   5602,   5800,   5998,   6195,
      6393,   6590,   6786,   6983,   7179,   7375,   7571,   7767,
      7962,   8157,   8351,   8545,   8739,   8933,   9126,   9319,
      9512,   9704,   9896,  10087,  10278,  10469,  10659,  10849,
     11039,  11228,  11417,  11605,  11793,  11980,  12167,  12353,
     12539,  12725,  12910,  13094,  13279,  13462,  13645,  13828,
     14010,  14191,  14372,  14553,  14732,  14912,  15090,  15269,
     15446,  15623,  15800,  15976,  16151,  16325,  16499,  16673,
     16846,  17018,  17189,  17360,  17530,  17700,  17869,  18037,
     18204,  18371,  18537,  18703,  18868,  19032,  19195,  19357,
     19519,  19680,  19841,  20000,  20159,  20317,  20475,  20631,
     20787,  20942,  21096,  21250,  21403,  21554,  21705,  21856,
     22005,  22154,  22301,  22448,  22594,  22739,  22884,  23027,
     23170,  23311,  23452,  23592,  23731,  23870,  24007,  24143,
     24279,  24413,  24547,  24680,  24811,  24942,  25072,  25201,
     25329,  25456,  25582,  25708,  25832,  25955,  26077,  26198,
     26319,  26438,  26556,  26674,  26790,  26905,  27019,  27133,
     27245,  27356,  27466,  27575,  27683,  27790,  27896,  28001,
     28105,  28208,  28310,  28411,  28510,  28609,  28706,  28803,
     28898,  28992,  29085,  29177,  29268,  29358,  29447,  29534,
     29621,  29706,  29791,  29874,  29956,  30037,  30117,  30195,
     30273,  30349,  30424,  30498,  30571,  30643,  30714,  30783,
     30852,  30919,  30985,  31050,  31113,  31176,  31237,  31297,
     31356,  31414,  31470,  31526,  31580,  31633,  31685,  31736,
     31785,  31833,  31880,  31926,  31971,  32014,  32057,  32098,
     32137,  32176,  32213,  32250,  32285,  32318,  32351,  32382,
     32412,  32441,  32469,  32495,  32521,  32545,  32567,  32589,
     32609,  32628,  32646,  32663,  32678,  32692,  32705,  32717,
     32728,  32737,  32745,  32752,  32757,  32761,  32765,  32766,
     32767,
};

/* sin of a quarter phase in [0, DEG_BAM(90)] (0..90 deg) -> Q15 [0, 32767]. */
static q15_t sin_quarter(uint16_t phase) {
#if MC_SIN_INTERP
    uint16_t idx  = phase >> SIN_LUT_FRAC_BITS;                        /* 0..256 */
    uint16_t frac = phase & ((1u << SIN_LUT_FRAC_BITS) - 1u);          /* 0..63  */
    q15_t a = SIN_Q[idx];
    q15_t b = SIN_Q[idx < SIN_LUT_LAST ? idx + 1 : SIN_LUT_LAST];
    return (q15_t)(a + (((int32_t)(b - a) * frac) >> SIN_LUT_FRAC_BITS));
#else
    /* Round, don't truncate: one add, and it halves the worst-case error. Adding the half
     * entry cannot run off the end -- phase reaches DEG_BAM(90) == 16384 (quadrants 1 and 3
     * pass 90 deg - ph), and (16384 + 32) >> 6 is still SIN_LUT_LAST. */
    uint16_t idx = (phase + (1u << (SIN_LUT_FRAC_BITS - 1))) >> SIN_LUT_FRAC_BITS;
    return SIN_Q[idx];
#endif
}

static q15_t sin_q15(angle_t a) {
    uint16_t quad = a >> 14;               /* 0..3     */
    uint16_t ph   = a & 0x3FFF;            /* 0..16383 */
    switch(quad) {
        case 0:  return  sin_quarter(ph);
        case 1:  return  sin_quarter(DEG_BAM(90) - ph);
        case 2:  return (q15_t)-sin_quarter(ph);
        default: return (q15_t)-sin_quarter(DEG_BAM(90) - ph);
    }
}
static q15_t cos_q15(angle_t a) { return sin_q15((angle_t)(a + DEG_BAM(90))); }

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

ab_pu_t abc2ab(abc_pu_t i) {
    ab_pu_t r;
    r.afa = clarke_third((int32_t)2*i.a - i.b - i.c);     /* (2/3)(a - b/2 - c/2) */
    r.bet  = q15_sat(((int32_t)(i.b - i.c) * Q15_INV_SQRT3) >> 15);
    return r;
}

/* The one decode. Everything above this line is a table; everything below it is algebra
 * (trans.h): this is what a port replaces to put the rotation on hardware. */
sincos_t sincos_q15(angle_t t) {
    sincos_t r = { cos_q15(t), sin_q15(t) };
    return r;
}

ab_pu_t dq2ab(dq_pu_t x, sincos_t cs) {
    ab_pu_t r;
    r.afa = q15_sat(q15mul(x.d, cs.c) - q15mul(x.q, cs.s));
    r.bet = q15_sat(q15mul(x.d, cs.s) + q15mul(x.q, cs.c));
    return r;
}

dq_pu_t ab2dq(ab_pu_t x, sincos_t cs) {
    dq_pu_t r;
    r.d = q15_sat( q15mul(x.afa, cs.c) + q15mul(x.bet, cs.s));
    r.q = q15_sat(-q15mul(x.afa, cs.s) + q15mul(x.bet, cs.c));
    return r;
}

abc_pu_t ab2abc(ab_pu_t x) {
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
