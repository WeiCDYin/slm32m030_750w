#include "ob.h"
/* The generic observer dispatchers (mc/ob.h): NULL-safe indirection through the
 * vtable an implementation bound in (e.g. smo_bind). Keeps the core's call sites
 * (prod_if_foc, hsm seed) free of any single observer's identity. */

void ob_step(const ob_t *ob, ab_pu_t iab_meas, ab_pu_t uab_ref_lim, ob_out_t *out) {
    if(ob && ob->step) { ob->step(ob->state, iab_meas, uab_ref_lim, out); return; }
    out->theta_est = 0; out->spd_est = 0; out->emf_est_sq = 0; out->theta_err = 0;
}

void ob_init(const ob_t *ob) {
    if(ob && ob->init) ob->init(ob->state);
}


