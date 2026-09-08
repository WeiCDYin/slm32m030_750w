#ifndef MC_VF_H
#define MC_VF_H
#include "types.h"
#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* V/f control: it generates a voltage vector reference.
 * The caller hands it an electrical speed target every tick -- the generator holds none of its
 * own, exactly as if_t does not (if.h). Each step:
 *   1. slew the working speed toward the command at the configured accel (rate
 *      limit), so a step change in setpoint doesn't snap the angle.
 *   2. integrate the forced angle from the working speed.
 *   3. set the voltage magnitude from the V/f curve
 *         v(|w|) = v_boost + (v_rated - v_boost)*min(|w_pu|, 1)
 * Open loop: no current feedback, no machine parameters -- only the per-unit
 * bases, like if_tune. */

typedef struct {
    float v_boost_pu;   /* voltage at zero speed (low-speed boost)    [pu] */
    float v_rated_pu;   /* voltage at rated speed (|w_pu| == 1)       [pu] */
    float w_ramp_ms;    /* accel: time to slew speed across full pu   [ms] */
} vf_cfg_t;

typedef struct {
    /* coefficients, derived by vf_tune (cold path) */
    int32_t  k_theta;     /* Q32-angle increment per step per speed LSB */
    int32_t  d_spd;       /* speed slew increment per step (Q15.16)     */
    q15_t    v_boost;     /* curve: voltage at |w| == 0          (Q15). The LIVE value -- also the
                           * static voltage vector's magnitude, since at standstill the curve output
                           * IS the boost, so vf_set_vv_mag aims at this field. */
    q15_t    v_boost_cfg; /* the CONFIGURED boost (vf_cfg.v_boost_pu), kept apart from the live one
                           * so it survives a vf_set_vv_mag override: vf_init restores v_boost from
                           * it on every entry, so a static-vector session cannot leave the V/f CURVE
                           * re-fluxed behind it. One source of truth; written only by vf_tune.
                           * The one field of this shape left: the SPEED target used to need the
                           * same treatment, and stopped needing it once it became an argument. */
    int32_t  k_v;         /* curve slope: (v_rated - v_boost)    (Q15)  */
    angle_t  theta_offset; /* constant angle added to the output (Q16); 0 for plain V/f, and 0 is what
                            * vf_init restores -- only the static-vector leaf ever wants it non-zero. */
    /* state */
    uint32_t theta;   /* Q32 angle accumulator; output angle_t = top 16 bits */
    int32_t  spd_acc;     /* Q15.16 working speed, per-unit of w_base      */
} vf_t;

/* Cold path: derive Q15/Q31 coefficients from SI config + per-unit bases + Ts
 * (like if_tune). Needs only the bases, no machine parameters. Requires
 * w_base*Ts < pi (under half an electrical rev per step at rated speed). */
void    vf_tune(vf_t *g, const base_t *b, const vf_cfg_t *c, float Ts);
/* Reset only the RUNTIME state to standstill (working speed 0, angle accumulator 0), keeping the
 * derived coefficients. The runtime half of vf_tune; call on VF entry (hsm on_entry_vf) so entry
 * starts a clean ramp instead of resuming a stale working speed. NULL-safe.
 *
 * Also undoes both LIVE overrides, so a static-vector session (ST_VV shares this vf_t) cannot leak
 * into the next entry: v_boost is restored from v_boost_cfg, and theta_offset is CLEARED -- zero is
 * its configured value, so there is nothing to shadow. if_init does the same, for the same reason.
 * The speed target needs no mention: it is an argument to vf_step, so this reset has nothing to
 * hold over. */
void    vf_init(vf_t *g);
/* Add a constant angle offset (Q16) to the output, live. With the speed command at
 * zero (so the integrated angle holds) this parks a STATIC voltage vector at the
 * offset: the open-loop voltage-vector bring-up mode (formerly the VOLT leaf) = set
 * speed 0, vf_set_theta_offset, vf_set_vv_mag. Note VF rides the q-axis, so the
 * stator vector sits at theta_offset + 90 deg. */
void    vf_set_theta_offset(vf_t *g, angle_t off);
/* Set the boost (zero-speed) voltage live (per-unit Q15). At |w| == 0 the V/f curve
 * output IS the boost voltage, so this is the static vector's magnitude. */
void    vf_set_vv_mag(vf_t *g, q15_t v_pu);
/* Hot path, pure fixed-point: slew toward spd_tgt (THIS tick's electrical speed target, per-unit
 * Q15 of w_base), integrate the angle, return the forced reference as a polar vector --
 * { mag = V/f-curve voltage, angle = forced electrical angle }. The magnitude rides the q-axis at
 * that angle (the V/f drive is in quadrature); mc_fast_step places it. NULL-safe: {0,0}. */
polar_pu_t vf_step(vf_t *g, spd_pu_t spd_tgt);
/* Current working speed (per-unit Q15); == the target once the ramp settles. */
spd_pu_t   vf_get_spd(const vf_t *g);
/* ST_VV's entry point: vf_init, plus the frame at -90 deg (the producer drives the q-axis, so the
 * vector then lands ON the commanded angle) and the magnitude commanded to zero. VF keeps vf_init:
 * its 0 start is arbitrary and its restored boost is a real curve parameter. See if.h's cv_init. */
void vv_init(vf_t *g);
#ifdef __cplusplus
}
#endif
#endif
