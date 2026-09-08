#ifndef MC_MOTOR_CFG_H
#define MC_MOTOR_CFG_H
#ifdef __cplusplus
extern "C" {
#endif
/* Single source of truth for one machine: SI datasheet parameters plus the DC bus
 * it hangs on. Pure data, owned by the app/port layer (the sim and each board
 * instantiate it); the core never stores raw SI in the hot path. The pmsm model
 * consumes the SI fields directly; the tuning helpers derive Q15/Q31 controller
 * coefficients from the same struct, so gains and pmsm can't drift. The per-unit
 * bases are deliberately NOT here -- they are a scaling convention, not physics
 * (see mc/base.h). Float here is config-only — it does not put float math on
 * the hot path. */
typedef struct {
    /* Machine parameters, SI units, from datasheet. */
    float Rs;        /* phase resistance               [ohm]     */
    float Ld, Lq;    /* dq-axes inductances            [H]       */
    float lambda_pm; /* PM flux linkage (lambda_pm)    [V s]     */
    int   npp;       /* number of pole pairs           [-]       */
    float J;         /* rotor inertia                  [kg m^2]  */
    float B;         /* viscous friction coefficient   [N m s]   */
    float Udc;       /* DC bus voltage                 [V]       */
} motor_cfg_t;
#ifdef __cplusplus
}
#endif
#endif
