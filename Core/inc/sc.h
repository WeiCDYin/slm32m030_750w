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
    pi2dof_t pi;       /* the regulator; it does not clamp -- the limit is sc's (pi2dof.h) */
    q15_t    i_max;    /* output clamp = the current limit (Q15 pu) */
} sc_t;

/* The COLD half of sc_t: what sc_tune derives, and nothing a step writes. i_max rides along
 * because sc_tune sets it, but it is an APPLICATION limit rather than a plant-derived gain --
 * a caller free to want a lower current limit than the one it was handed may simply overwrite
 * the field after loading. */
typedef struct {
    pi2dof_gains_t pi;      /* the regulator (pi2dof.h)                            */
    q15_t          i_max;   /* output clamp = the current limit (Q15 pu)           */
} sc_gains_t;

/* Reset runtime state (the integrator) to zero, gains/limit kept. The per-entry reset
 * (parallels cc_init). NULL-safe. */
void  sc_init(sc_t *sc);
/* Load a gain set and clear the runtime state -- what sc_tune does once it has computed one.
 * NULL gains -> INERT: zero gains AND a zero current limit, so the loop commands no current.
 * NULL-safe. */
void  sc_set_gains(sc_t *sc, const sc_gains_t *g);
/* Cold path: tune to the mechanical plant for a closed-loop DOUBLE pole at alpha_s [rad/s], and
 * load it (sc_set_gains). Reads J, B, npp and lambda_pm from the machine; the derivation is in
 * sc.c. Output clamped to +-i_max_pu. A machine with no torque gain or no inertia tunes INERT
 * (all gains 0). NULL-safe. */
void  sc_tune(sc_t *sc, const motor_cfg_t *m, const base_t *b, float alpha_s, float i_max_pu, float Ts);
/* I-f -> FOC handover: set the integrator so the NEXT sc_step outputs iq at the operating point
 * (w_ref, w_meas) -- see pi2dof_set_I. Exact even mid-ramp (w_ref != w_meas). NULL-safe. */
void  sc_trans_set_I(sc_t *sc, q15_t iq, spd_pu_t w_ref, spd_pu_t w_meas);
/* Cold path: speed reference + measured speed -> q-axis current reference (Q15 pu),
 * clamped to the current limit. NULL-safe: 0.
 * [TODO] may make it more general, i.e., speed loop outputs torque reference instead of q-axis current reference.*/
q15_t sc_step(sc_t *sc, spd_pu_t w_ref, spd_pu_t w_meas);
#ifdef __cplusplus
}
#endif
#endif
