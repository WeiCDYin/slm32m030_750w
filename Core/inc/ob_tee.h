#ifndef MC_OB_TEE_H
#define MC_OB_TEE_H
#include "ob.h"
#ifdef __cplusplus
extern "C" {
#endif
/* OBSERVER TEE -- a T-fitting on the observer seam: one set of inputs in, two observers
 * driven from it, one angle back out. Named for the pipe fitting (and Unix tee): it
 * estimates nothing itself, it only splits the flow.
 *
 *   iab_meas, uab_ref_lim ---+--> shadow   (stepped; its angle is DISCARDED)
 *                            `--> primary  (stepped; its angle reaches mc->theta_est)
 *
 * It IS an ob_t, so mc.c is untouched: the core still calls ob_step on one handle and
 * never learns that two observers ran (ob.h's vtable used to COMPOSE, not to substitute).
 *
 * Why a tee rather than a second call from the port: both observers must see the SAME
 * inputs at the SAME point in the tick or the comparison is invalid. A port-side shadow
 * call after mc_fast_step would read mc->uab_ref_lim already overwritten with u[k],
 * while the primary saw u[k-1] (mc.c latches at the tail) -- one sample of skew, silently.
 *
 * Composition (port owns the storage, as with every ob impl):
 *   smo_bind(&ob_a, &smo);  stsmo_bind(&ob_b, &stsmo);
 *   ob_tee_bind(&ob, &tee, &ob_a, &ob_b);   mc->ob = &ob;
 * Swapping which observer drives control is swapping the last two arguments. Reading the
 * shadow is reading its own state struct -- which is how the probes and the J-Scope
 * globals already work (sim_api.c points them at smo.eab_est, not at the vtable).
 *
 * A single-observer build simply does not use this: bind the impl straight into mc->ob
 * and the linker drops ob_tee.o entirely. There is no cost, and no #ifdef in the core. */
typedef struct {
    const ob_t *primary;   /* drives control: its angle and speed are what the core sees */
    const ob_t *shadow;    /* runs on identical inputs; outputs observed, never used     */
} ob_tee_t;

/* Fill `ob` with the tee vtable over `tee`, which records the two handles. Both are
 * stepped and init'd; get_spd reports the PRIMARY's. Either handle may be NULL (the
 * ob.h dispatchers are NULL-safe), so a tee with no shadow degrades to the primary
 * alone. NULL-safe in `ob` and `tee`. */
void ob_tee_bind(ob_t *ob, ob_tee_t *tee, const ob_t *primary, const ob_t *shadow);
#ifdef __cplusplus
}
#endif
#endif
