#ifndef MC_SC_H
#define MC_SC_H
#include "types.h"
#include "pi2dof.h"     /* the regulator; sc supplies its MECHANICAL-plant tuning */
#include "motor_cfg.h"
#include "base.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Outer speed controller: a 2-DOF PI (pi2dof.h) tuned to the MECHANICAL plant. sc owns only
 * the plant mapping -- the regulator math is shared with cc, which tunes the same structure to
 * the electrical plant. The wide-gain format matters here: w_base >> i_base, so the per-unit
 * loop gain is far above 1.0 (see the tune_gains_are_large_and_positive spec). */
typedef struct {
    pi2dof_t pi;              /* the regulator */
    int32_t  w_ref;         
    int32_t  ramp_rate;       
    q15_t    i_max;           /* output clamp = the current limit (Q15 pu) */
} sc_t;

/* What a PERSON sets: SI/per-unit floats, the authored half of the loop. Separate from the derived
 * half below because they are opposite ends of sc_tune -- this one is written by hand (or by a
 * bench), and sc_tune is what turns it into the fixed-point coefficients the target runs. The
 * same split as if_cfg_t / cc_cfg_t. Only alpha_s is a regulator quantity; the other two are
 * APPLICATION limits, which is why they are handed in rather than derived from the machine. */
typedef struct {
    float alpha_s;      /* closed-loop DOUBLE pole [rad/s] -- the loop bandwidth           */
    float i_max_pu;     /* output clamp = the current limit [pu]                           */
    float ramp_pu_s;    /* w_ref ramp rate, [pu of w_base per second].                       */
    float Ts;           
} sc_cfg_t;

/* Reset runtime state (the integrator AND w_ref) to zero; gains, limit and ramp_rate
 * kept. The per-entry reset (parallels cc_init). NULL-safe. */
void  sc_init(sc_t *sc);
/* Cold path, and the ONLY way into the loop -- cfg in, tuned object out, no separate load step.
 * Tunes to the mechanical plant for a closed-loop DOUBLE pole at c->alpha_s, reading J, B, npp and
 * lambda_pm from the machine; the derivation is in sc.c. The two application limits ride along
 * unchanged but folded: c->i_max_pu becomes the Q15 clamp, c->ramp_pu_s the per-step increment
 * against c->Ts. So one call defines the whole loop, and there is no parameter to remember to set
 * afterwards. A machine with no torque gain or no inertia, or a NULL cfg, tunes INERT: no gains
 * and a zero clamp, so the loop commands no current. NULL-safe. */
void  sc_tune(sc_t *sc, const motor_cfg_t *m, const base_t *b, const sc_cfg_t *c);
/* I-f -> FOC handover: set the integrator so the NEXT sc_step outputs iq at the operating point
 * (w_ref, w_meas) -- see pi2dof_set_I. Exact even mid-ramp (w_ref != w_meas). NULL-safe. */
void  sc_trans_set_I(sc_t *sc, q15_t iq, spd_pu_t w_ref, spd_pu_t w_meas);
/* Cold path: re-rate a RUNNING loop -- sc_tune already set ramp_rate from cfg.ramp_pu_s, this is
 * for changing it afterwards. 0 either way == unlimited. Leaves w_ref where it stands, so the
 * reference bends rather than dropping to zero. NULL-safe. */
void  sc_set_ramp_rate(sc_t *sc, float rate_pu_s, float Ts);
/* Put w_ref AT w, so the next sc_step leaves from there instead of from wherever the last run
 * ended -- what a caller enabling the loop on a turning machine wants. Only a rate-limited sc
 * needs it: with no rate the next step overwrites w_ref anyway. sc_trans_set_I does this itself
 * at the handover. NULL-safe. */
void  sc_set_w_ref(sc_t *sc, spd_pu_t w);
/* w_ref narrowed to Q15 -- what mc publishes as act_spd_ref. NULL-safe: 0. */
spd_pu_t sc_get_w_ref(const sc_t *sc);
/* Cold path: speed DEMAND + measured speed -> q-axis current reference (Q15 pu), clamped to the
 * current limit. w_cmd becomes w_ref first -- paced there if a ramp rate is configured, copied
 * straight across if not -- so the regulator never sees a step the drive cannot follow when the
 * caller asked for that. Read what it tracked with sc_get_w_ref. NULL-safe: 0.
 * [TODO] may make it more general, i.e., speed loop outputs torque reference instead of q-axis current reference.*/
q15_t sc_step(sc_t *sc, spd_pu_t w_cmd, spd_pu_t w_meas);
#ifdef __cplusplus
}
#endif
#endif
