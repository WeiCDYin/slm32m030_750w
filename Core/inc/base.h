#ifndef MC_BASE_H
#define MC_BASE_H
#ifdef __cplusplus
extern "C" {
#endif
/* The per-unit normalization bases: the SI <-> Q15 seam (per-unit x = x_SI / base).
 * This is the firmware's CHOICE of what 1.0 means, not machine physics -- it lives
 * apart from motor_cfg_t so the physical-machine config carries no scaling fields.
 * Owned by the composition root next to its motor_cfg_t; everything that crosses
 * the numeric boundary (ADC -> pu, tuning, pu commands -> volts) shares one instance.
 * Float here is config-only -- it does not put float math on the hot path.
 * Convention: u_base = six-step fundamental 2*Udc_nom/pi, the inverter ceiling (see svm.c). */
typedef struct {
    float i_base;   /* current base      [A]           */
    float u_base;   /* voltage base      [V]           */
    float w_base;   /* speed base        [elec. rad/s] */
} base_t;
#ifdef __cplusplus
}
#endif
#endif
