#include "cc.h"
#include "svm.h"     
#include "trans.h"   

static const float TWO_PI = 6.2831853071795865f;   

void cc_init(cc_t *cc) {
    if(!cc) return;
    pi2dof_init(&cc->pi_d);
    pi2dof_init(&cc->pi_q);
}

/* Cold-path float -> Q15 gain in int32 (no cap at 1.0; the effective gain may exceed unity --
 * REF at 500 Hz already wants k_p = 1.97). Mirrors sc.c. */
/* The defensive rail is 65535 whole pu, not INT32_MAX: qmath.h's split multiply needs
 * (gain >> 15) to stay inside +-65535 so its high partial product cannot overflow int32.
 * No tuning approaches it -- the largest gain any machine in the catalog asks for is ~216 pu
 * (REF's speed loop at 32 Hz), 0.3% of this. */
#define GAIN_Q15_MAX ((float)((int32_t)65535 << 15))    /* 65535.0 pu in Q15 */
static int32_t gain_q15(float x) {
    float v = x * (float)Q15_ONE;
    if(v >  GAIN_Q15_MAX) v =  GAIN_Q15_MAX;
    if(v < -GAIN_Q15_MAX) v = -GAIN_Q15_MAX;
    return (int32_t)(v < 0.0f ? v - 0.5f : v + 0.5f);
}

/* k_aw is bandwidth*Ts and must be below 1.0 for the discretisation to mean anything -- at 1.0
 * the back-calc would drain the whole rejected amount in a single tick. Saturating here also
 * makes it the NARROW operand of pi2dof's anti-windup product (qmath.h). */
static int32_t aw_q15(float x) {
    int32_t v = gain_q15(x);
    if(v >  Q15_ONE) v =  Q15_ONE;
    if(v < -Q15_ONE) v = -Q15_ONE;
    return v;
}

/* Tune one axis to a closed-loop double pole at cc_bw: with the RL plant L*di/dt = u - R*i - e,
 *   [L s^2 + (R + k_p) s + k_i] i = (k_t s + k_i) i_ref - s e
 * so k_p = 2*alpha*L - R makes the denominator L(s+alpha)^2 (both poles placed, hence the
 * disturbance rejection), and k_t = alpha*L cancels a numerator pole to leave alpha/(s+alpha)
 * (reference response unchanged from the old pole-zero tuning). g maps SI -> per-unit. */
static void tune_one(pi2dof_t *pi, float R, float L, float cc_bw, float Ts, float g) {
    if(!pi) return;
    pi2dof_init(pi);
    pi->kp   = gain_q15((2.0f * cc_bw * L - R) * g);   /* state feedback                   */
    pi->ki   = gain_q15(cc_bw * cc_bw * L * g * Ts);   /* integral, Ts folded in           */
    pi->kt   = gain_q15(cc_bw * L * g);                /* reference feedforward            */
    pi->k_aw = aw_q15(cc_bw * Ts);                     /* anti windup                      */
}

void cc_tune(cc_t *cc, const motor_cfg_t *m, const base_t *b, float cc_bw, float Ts) {
    if(!cc) return;
    cc->k_delay = 0;                         /* inert until proven tunable */
    if(!m || !b) return;
    float g = b->i_base / b->u_base;         /* SI gain -> per-unit (i_pu in, v_pu out) */
    tune_one(&cc->pi_d, m->Rs, m->Ld, cc_bw, Ts, g);
    tune_one(&cc->pi_q, m->Rs, m->Lq, cc_bw, Ts, g);
    /* Output-rotation coefficient for the 1.5*Ts transport delay. One tick advances the angle by
     * (we * k_theta) >> 16 with k_theta = w_base*Ts/2pi*2^17 (the smo/vf scaling); k_delay is
     * 1.5x that, the lead applied at the output. The runtime product is int64, so this needs no
     * <2^16 cap the way k_theta does. */
    float kd = 1.5f * b->w_base * Ts / TWO_PI * PU_ONE_Q32;    /* 1.5 * k_theta, 2^17 scale */
    if(kd > 65535.0f) kd = 65535.0f;    /* the same rail to_inc_sat puts on k_theta, and for the
                                         * same reason: it is what keeps we*k_delay a 32-bit
                                         * product (32768 * 65535 < INT32_MAX), so the lead below
                                         * needs no long multiply. Both machines in the catalog
                                         * land near 2600-2950, so this never binds. */
    cc->k_delay = (int32_t)(kd + 0.5f);
}

void cc_trans_set_I(cc_t *cc, dq_pu_t udq, dq_pu_t idq_ref, dq_pu_t idq_meas) {
    if(!cc) return;
    pi2dof_set_I(&cc->pi_d, udq.d, idq_ref.d, idq_meas.d);
    pi2dof_set_I(&cc->pi_q, udq.q, idq_ref.q, idq_meas.q);
}

/* Saturate the ask into dq_pu_t for the svm tail. Lossless in practice (cc.h): 1.0 pu is the
 * six-step ceiling. The unlimited ask survives in pi->out for the back-calc. */
static q15_t sat_q15(int32_t x) {
    if(x > Q15_MAX) return Q15_MAX;
    if(x < Q15_MIN) return Q15_MIN;
    return (q15_t)x;
}

void cc_step(cc_t *cc, const cc_in_t *in, duties_t *duty, ab_pu_t *uab_ref_lim) {
    /* duty and uab_ref_lim are NOT checked -- they are the caller's own storage, never optional,
     * and this runs every carrier tick (cc.h). cc and in stay NULL-safe: a port can step a drive
     * whose current loop is not configured yet, and that must give zero volts, not a crash. */
    if(!cc || !in) { *duty = SVM_DUTY_ZERO; *uab_ref_lim = (ab_pu_t){0, 0}; return; }

    /* Measure in the working frame, then the ask (both axes); pi2dof_output keeps the unlimited
     * ask in pi->out for the back-calc below. */
    dq_pu_t idq     = ab2dq(in->iab_meas, sincos_q15(in->theta));
    dq_pu_t udq_ref = {
        .d = sat_q15(pi2dof_output(&cc->pi_d, in->idq_ref.d, idq.d)),
        .q = sat_q15(pi2dof_output(&cc->pi_q, in->idq_ref.q, idq.q)),
    };

    /* Rotate the output forward by 1.5*Ts*we so it lands aligned with the rotor when applied. */
    angle_t theta_o = (angle_t)(in->theta + (angle_t)(((int32_t)in->we * cc->k_delay) >> ANGLE_SHIFT));

    /* Modulate, and read back what the bridge will actually apply. */
    /* Both maps get the SAME bus reading, so the voltage handed to anti-windup and the observer
     * below is the one this tick's duties will really apply (svm.h). */
    const sincos_t cs_o    = sincos_q15(theta_o); /* ONE decode: both rotations below are about it */
    const ab_pu_t  uab_ref = dq2ab(udq_ref, cs_o);

    /* THE ROUND TRIP IS THE IDENTITY UNLESS SOMETHING CLIPPED, so ask first and only pay when the
     * answer is yes. Out through ab2abc, +u0, xk_fwd, clamp; back through abc2ab, xk_inv, ab2dq --
     * and every step but the clamp inverts exactly: the two Clarkes cancel, u0 and SVM_DUTY_MID
     * are common mode which the Clarke annihilates by construction, k_fwd*k_inv is 1 (svm.c), and
     * the two Parks share cs_o. Measured over the linear range the recovered vector lands 2-4 LSB
     * from the one handed in -- 0.01%, pure quantization. Taking uab_ref directly is both cheaper
     * and MORE faithful, since it skips three roundings rather than accumulating them.
     *
     * Two ways to clip, and both have to be caught. svm reports a clamped leg. dq2ab's own q15_sat
     * is the other: above ~1.1x nominal the ceiling is past 1.0 pu, so a rail there can happen with
     * no leg clamping, and missing it would hide a real clip from the anti-windup. A railed
     * component is the test for it -- a false positive merely costs the slow path.
     *
     * ~238 ARMv6-M cycles a tick, and on the WORST tick too: the handover runs at 0.125 pu, far
     * under the 0.907 clamp threshold, so nothing clamps there. */
    const int clipped = svm(uab_ref, in->k_fwd, duty)
                     || uab_ref.afa == Q15_MAX || uab_ref.afa == Q15_MIN
                     || uab_ref.bet == Q15_MAX || uab_ref.bet == Q15_MIN;

    /* Same-tick anti-windup, in the ASK's frame -- theta_o feeds both the forward and this inverse
     * transform, so its lead cancels and only the clip survives in (udq_ref_lim - udq_ref). */
    dq_pu_t udq_ref_lim;
    if(clipped) {
        *uab_ref_lim = svm_realized(*duty, in->udc_meas);
        udq_ref_lim  = ab2dq(*uab_ref_lim, cs_o);
    } else {
        *uab_ref_lim = uab_ref;      /* what was asked IS what was applied */
        udq_ref_lim  = udq_ref;      /* ...so the back-calc has nothing to reject */
    }
    pi2dof_update_I(&cc->pi_d, in->idq_ref.d, idq.d, udq_ref_lim.d);
    pi2dof_update_I(&cc->pi_q, in->idq_ref.q, idq.q, udq_ref_lim.q);
}
