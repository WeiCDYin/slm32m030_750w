#ifndef MC_IF_H
#define MC_IF_H
#include "types.h"
#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* I-f startup generator: a THETA + CURRENT-REFERENCE source, not a controller.
 *
 * Sequence (implicit, advanced by if_step):
 *   1. ramp |i_ref| 0 -> i_amp at zero frequency. The reference sits on the
 *      q-axis (i-f frame), as rotor alignment from standstill.
 *   2. hold current vector for i_hold_ms
 *   3. ramp electrical frequency 0 -> the target the CALLER passes to if_step.
 *
 * The generator holds no speed target of its own. It is handed one per tick and slews toward it
 * at the configured rate -- so the question "where is this ramp going?" has exactly one answer,
 * the caller's, and there is no stored target for a reset to have to restore. */

typedef struct {
    float i_mag_pu;     /* current vector magnitude                   [pu] */
    float i_ramp_ms;    /* time to ramp current 0 -> i_mag_pu         [ms] */
    float i_hold_ms;    /* time to hold current vector before w ramp  [ms] */
    float w_accel_pu_s; /* forced-speed ramp RATE [pu of w_base per second]. A rate, not a
                         * destination: where the ramp is going is the caller's per-tick argument
                         * to if_step, and the handover speed belongs to the drive (mc_t) rather
                         * than to this generator. */
} if_cfg_t;

typedef struct {
    /* coefficients, derived by if_tune (cold path) */
    int32_t  k_theta;     /* Q32-angle increment per step per speed LSB */
    int32_t  d_spd;       /* speed ramp increment per step (Q15.16)     */
    int32_t  d_i;         /* current ramp increment per step (Q15.16)   */
    uint32_t hold_steps;
    q15_t    spd_tgt;     /* THIS TICK's target, as handed to if_step -- cached, not owned, so
                           * if_ss_reached can answer against the target actually being chased */
    q15_t    i_tgt;       /* the current-magnitude target the ramp chases. The LIVE value -- also the
                           * static current vector's magnitude, since at standstill the ramp settles
                           * ON it, so if_set_cv_mag aims at this field. */
    q15_t    i_tgt_cfg;   /* the CONFIGURED magnitude (if_cfg.i_mag_pu), kept apart from the live one
                           * so it survives an if_set_cv_mag override: if_init restores i_tgt from
                           * it on every entry, so a static-vector session cannot leave the I-f
                           * STARTUP ramp aiming at a diagnostic magnitude. One source of truth;
                           * written only by if_tune. Mirrors vf_t's v_boost / v_boost_cfg. */
    angle_t  theta_offset; /* constant angle added to the output (Q16); 0 for plain I-f, and 0 is what
                            * if_init restores -- only the static-vector leaf ever wants it non-zero. */
    /* state */
    uint32_t theta;       /* Q32 angle accumulator; output angle_t = top 16 bits */
    int32_t  spd_acc;     /* Q15.16 electrical speed, per-unit of w_base  */
    int32_t  i_acc;       /* Q15.16 current magnitude, per-unit of i_base */
    uint32_t hold_cnt;
} if_t;

/* Derive Q15/Q31 coefficients from SI config (float, cold path -- like
 * cc_tune). Needs only the per-unit bases, no machine parameters.
 * Requires |w_tgt|*Ts < pi (well under half an electrical rev per step).
 * Nonzero ramp rates are guaranteed to progress (increments round up to
 * >= 1 LSB). */
void    if_tune(if_t *g, const base_t *b, const if_cfg_t *c, float Ts);
/* Reset only the RUNTIME ramp state to the startup point (current/frequency 0, alignment
 * hold pending, angle at the -90 deg d-axis start), keeping the derived coefficients.
 * The runtime half of if_tune; call on every I-f entry (hsm on_entry_if/on_entry_if_foc) so re-entry
 * restarts the trajectory from standstill instead of resuming a stale ramp. NULL-safe.
 * Also undoes both LIVE overrides, so a static-vector session (ST_CV shares this if_t) cannot leak
 * into the next entry's STARTUP: i_tgt is restored from i_tgt_cfg, and theta_offset is CLEARED --
 * zero is its configured value, so there is nothing to shadow. The SPEED target needs no mention: it
 * is an argument to if_step, so this reset has nothing to hold over. */
void    if_init(if_t *g);
/* Add a constant angle offset (Q16) to the forced angle, LIVE -- and live only: if_init clears it,
 * so the aim has to be re-commanded after any entry. With the speed
 * target at zero (so the angle holds) this parks a STATIC current vector at the
 * offset -- the "current vector" bring-up mode: I-f forces current on the CONTROLLER's
 * d-axis (the forced frame, not the rotor magnet axis), so the stationary current
 * vector points AT the offset (no q-axis 90 deg shift, unlike V/f). */
/* ST_CV's entry point: if_init, then the magnitude commanded to zero -- entering puts nothing on the
 * machine. ST_IF / ST_IF_FOC keep if_init, whose restored i_tgt is the startup current they need. */
void    cv_init(if_t *g);
void    if_set_theta_offset(if_t *g, angle_t offset);
/* Set the current vector magnitude target the ramp chases (per-unit Q15), live. Lets the
 * static "current vector" mode slew magnitude without resetting the generator. LIVE only:
 * i_tgt_cfg is untouched, so the next if_init puts the configured magnitude back. */
void    if_set_cv_mag(if_t *g, q15_t i_pu);
/* Hot path, pure fixed-point: advance the ramps one tick, integrate theta.
 *
 * spd_tgt is the forced-speed target for THIS tick (electrical, per-unit Q15 of w_base). The ramp
 * slews toward it at the configured rate; it does NOT jump. Open loop -- too large or too fast a
 * target against the held current pulls the rotor out of sync, which is the caller's to respect.
 *
 * Returns the forced reference as a polar vector -- { mag = ramped current magnitude, angle =
 * forced electrical angle (== mc->act_theta) }. The magnitude sits on the forced d-axis
 * (alignment/field); mc_fast_step feeds it to the current loop. NULL-safe: {0,0}. */
polar_pu_t if_step(if_t *g, spd_pu_t spd_tgt);
/* Current setpoint in the forced frame: { 0, i_mag_pu } (forced q-axis). */
dq_pu_t    if_get_idq_ref(const if_t *g);
/* Current forced angle (same value the last if_step returned as .angle). */
angle_t    if_get_theta(const if_t *g);
spd_pu_t   if_get_spd(const if_t *g);
/* i-f control (current and speed reference) reached steady state */
bool       if_ss_reached(const if_t *g);

#ifdef __cplusplus
}
#endif
#endif
