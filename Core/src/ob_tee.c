#include "ob_tee.h"
/* The observer tee (ob_tee.h): an ob_t that fans one input set out to two observers and
 * returns the primary's answer. Pure plumbing -- no state, no math of its own. */

/* Shadow FIRST, primary second, so the primary's ob_step is the last thing to touch the
 * inputs; order is otherwise irrelevant (neither observer can see the other, both take
 * their arguments by value). The shadow's angle is deliberately dropped. */
static void tee_step(void *state, ab_pu_t iab_meas, ab_pu_t uab_ref_lim, ob_out_t *out) {
    ob_tee_t *t = (ob_tee_t *)state;
    if(!t) { out->theta_est = 0; out->spd_est = 0; out->emf_est_sq = 0; out->theta_err = 0; return; }
    ob_out_t discard;
    ob_step(t->shadow, iab_meas, uab_ref_lim, &discard);   /* observed elsewhere, never used here */
    ob_step(t->primary, iab_meas, uab_ref_lim, out);       /* the PRIMARY's is what the core sees */
}

/* Both observers reset together, so a re-entry (IF/IF_FOC) leaves them comparable. */
static void tee_init(void *state) {
    ob_tee_t *t = (ob_tee_t *)state;
    if(!t) return;
    ob_init(t->primary);
    ob_init(t->shadow);
}

/* Everything the core sees is the PRIMARY's -- the shadow steers nothing, so its angle, speed,
 * amplitude and error are not the ones being handed to. That used to be three getter shims;
 * tee_step above now simply writes the primary's struct straight into the caller's. */
void ob_tee_bind(ob_t *ob, ob_tee_t *tee, const ob_t *primary, const ob_t *shadow) {
    if(!ob || !tee) return;
    tee->primary = primary;
    tee->shadow  = shadow;
    ob->step     = tee_step;
    ob->init     = tee_init;
    ob->state    = tee;
}
