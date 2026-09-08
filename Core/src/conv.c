#include "conv.h"
#define CONV_THETA_ERR_TOL 8579   /* 15 deg = 0.2618 rad, in Q15 radians */
#define CONV_EMF_MAG_MIN   1024   /* 0.031 * 32768 */
#define CONV_EMF_MAG_MIN_SQ ((uint32_t)CONV_EMF_MAG_MIN * CONV_EMF_MAG_MIN)   /* 1048576 [Q30] */
#define CONV_TICKS         256

void conv_tune(conv_t *c, const motor_cfg_t *m, const base_t *b, float floor_spd_pu) {
    if(!c) return;
    c->theta_err_tol = CONV_THETA_ERR_TOL;   /* the core's, on every machine (conv.h) */
    c->ticks_req     = CONV_TICKS;
    c->emf_mag_min_sq = CONV_EMF_MAG_MIN_SQ; /* the fallback, replaced below if it can be */
    if(!m || !b || b->u_base <= 0.0f || floor_spd_pu <= 0.0f) return;
    /* pu SPEED -> pu EMF, the one place lambda enters this module (conv.h). Saturating rather
     * than wrapping: an absurd floor becomes an unmeetable one, which refuses to catch -- the
     * safe direction, since not catching is a forced start and over-catching is a frame jump
     * onto an angle nobody validated. */
    float e = floor_spd_pu * m->lambda_pm * b->w_base / b->u_base * (float)Q15_ONE;
    if(e > (float)Q15_MAX) e = (float)Q15_MAX;
    if(e < 1.0f)           e = 1.0f;         /* never zero: that would grade nothing at all */
    /* SQUARED here, once, so the hot path compares against |e|^2 without taking a root
     * (conv.h). Rounded to the Q15 threshold FIRST, then squared, so the stored bound is the
     * exact square of the |e| this gate would have used -- not a separately rounded number. */
    uint32_t emin = (uint32_t)(e + 0.5f);
    c->emf_mag_min_sq = emin * emin;
}

void conv_init(conv_t *c) {
    if(!c) return;                    /* a re-entered observer has converged on nothing yet */
    c->lock_tick = 0;
    c->mag_tick  = 0;
}

/* One tick of evidence for one condition: earn a tick when it holds, LOSE ONE when it does not
 * (conv.h) -- leaky rather than latching, so an isolated tick cannot wipe the run. */
static void credit(uint16_t *tick, uint16_t cap, bool ok) {
    if(ok) {
        if(*tick < cap) (*tick)++;
    } else if(*tick) {
        (*tick)--;
    }
}

void conv_step(conv_t *c, q15_t theta_err, uint32_t emf_mag_sq) {
    if(!c) return;
    int32_t aerr = theta_err < 0 ? -theta_err : theta_err;
    credit(&c->lock_tick, c->ticks_req, aerr < c->theta_err_tol);
    credit(&c->mag_tick,  c->ticks_req, emf_mag_sq >= c->emf_mag_min_sq);
}

bool conv_is_converged(const conv_t *c) {
    /* BOTH, and each on its own run of evidence: an angle that has been right for long enough,
     * measured on a signal that has been there for long enough (conv.h). An untuned object has
     * ticks_req 0 and would read as converged from the first tick, so that is excluded here --
     * a missing conv_tune must not become an unconditional pass. */
    return c && c->ticks_req && c->lock_tick >= c->ticks_req && c->mag_tick >= c->ticks_req;
}
