#ifndef MC_CC_H
#define MC_CC_H
#include "types.h"
#include "pi2dof.h"
#include "motor_cfg.h"
#include "base.h"
#include "svm.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pi2dof_t pi_d, pi_q;  
    int32_t  k_delay;     /* delay comp. coeff (we*k_delay)>>16 = 1.5*Ts*we */
} cc_t;

/* input */
typedef struct {
    dq_pu_t  idq_ref;    /* the reference to track (mc->act_idq_ref) */
    ab_pu_t  iab_meas;   /* measured current, stationary frame (abc2ab of this tick's sample) */
    angle_t  theta;      /* working electrical angle -- the measurement frame */
    spd_pu_t we;         /* electrical speed (pu)    -- for delay compensation */
    svm_gain_t k_fwd;    /* the modulator's forward gain, taken at the SLOW rate (svm.h) */
    udc_pu_t udc_meas;   /* measured DC link voltage (pu), THIS tick -- deliberately a different
                          * vintage from k_fwd: svm_realized must report what the bridge actually
                          * applied, which is the duty against the bus that is there NOW, not
                          * against the one the gain was taken from up to a millisecond ago. */
} cc_in_t;


void     cc_init(cc_t *cc);
void     cc_tune(cc_t *cc, const motor_cfg_t *m, const base_t *b, float bw_cc, float Ts);
/* I-f -> FOC handover: set both integrators so the NEXT cc_step asks for udq (the applied
 * u[k-1], re-expressed in the estimated frame) at the operating point (idq_ref, idq_meas)
 * -- see pi2dof_set_I. Exact even when idq_ref != idq_meas, which is the case that matters
 * here: d lands at ref 0 against the whole I-f current margin. NULL-safe. */
void     cc_trans_set_I(cc_t *cc, dq_pu_t udq, dq_pu_t idq_ref, dq_pu_t idq_meas);
/* EVERYTHING BY POINTER, in and out, for the same reason: this runs every carrier tick.
 *
 * `in` -- cc_in_t is 20 bytes, and passing it whole made the caller materialize it twice, once in
 * its own frame and once copied into the outgoing argument area (an ldmia/stmia pair per site).
 *
 * `duty` and `uab_ref_lim` -- the two results used to arrive inside one returned struct. A struct
 * bigger than 4 bytes is ALREADY a hidden-pointer return under AAPCS, so nothing was ever coming
 * back in registers; the cost was the caller copying OUT of that buffer into its own two
 * destinations. On ARMv6-M, which has no unaligned access and so cannot expand a 6-byte block
 * move inline, each of those copies is a `bl memcpy` -- two per call site. Naming the final
 * destinations here removes both. ARMv7-M compiles either form to the same four instructions, so
 * this costs nothing on the M4 and is not worth undoing if the M0-class port never happens.
 *
 * `duty` and `uab_ref_lim` must be non-NULL and are NOT checked -- they are the caller's own
 * storage, never optional, and a test in the ISR for a pointer that cannot be null is two
 * compares per carrier tick. `cc` and `in` DO stay NULL-safe (zero duties, zero applied voltage):
 * a port can step a drive whose current loop is not configured yet. */
void cc_step(cc_t *cc, const cc_in_t *in, duties_t *duty, ab_pu_t *uab_ref_lim);

#ifdef __cplusplus
}
#endif
#endif
