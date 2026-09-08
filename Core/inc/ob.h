#ifndef MC_OB_H
#define MC_OB_H
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Generic OBSERVER interface -- the seam the control core talks to,
 * agnostic to which observer is behind it (sliding-mode, flux, ...). A function-
 * pointer vtable + the implementation's state pointer, exactly like the cold-path
 * mc_hw_if_t power-stage vtable. The core (prod_if_foc) and hsm only ever call the
 * NULL-safe dispatchers below on mc->ob; they never know the implementation.
 *
 * Compose it in the port: allocate the impl's state (e.g. smo_t) + an ob_t, tune the
 * impl, then *_bind() the impl into the ob_t and point mc->ob at it (see smo.h /
 * sim_api.c). A second observer is a new *_bind filling the same vtable -- core and
 * ports unchanged. (The hot path pays one indirect call per tick for this; the
 * observer math dwarfs it.)
 *
 * Fixed signal contract: stationary-frame (alpha/beta) measured current + commanded
 * voltage in, electrical angle out -- the caller already did the Clarke, so the
 * observer shares it. State is the impl's explicit object the port owns (never a
 * file-static, or the current loop loses purity). */
/* EVERYTHING ONE STEP PRODUCES, written through a pointer (cf. cc_step, f7f0f1c).
 *
 * These four used to be an angle returned plus three getters in the vtable, and every consumer
 * read all four in the same breath -- so the core paid three indirect call CHAINS per tick to
 * fetch fields the observer had already latched. Counted: ~59 cycles on M4, ~109 on M0+ (no
 * tail-call there, so each hop is a real stack frame), against ~10 for four struct reads.
 *
 * The bigger reason is that optional slots were a bug the interface INVITED, and it bit once:
 * see stsmo_bind, where two unbound getters read as 0 through the NULL-safe dispatchers and the
 * transition gate silently failed open. One producer filling one struct has nothing to forget. */
typedef struct {
    angle_t  theta_est;  /* the estimate: electrical angle                                    */
    spd_pu_t spd_est;    /* the tracking loop's speed byproduct                               */
    uint32_t emf_est_sq; /* |EMF estimate|^2, pu^2 of u_base^2 -- the estimate itself, with no
                          * tracking loop in it. SQUARED because nothing downstream wants |e|
                          * except to compare it, and that survives squaring, so no observer
                          * takes a root (pll.h). 0 where an implementation has none, which
                          * reads as "no back-EMF" -- mc.c RESYNC then starts from rest.       */
    q15_t    theta_err;  /* the tracking loop's PHASE ERROR this tick, Q15 radians (pll.h). A
                          * reading, not a verdict: whether the angle has ARRIVED is decided
                          * from this and emf_est_sq together, outside any observer (conv.h). 0
                          * where an implementation has none -- "no objection", since a check
                          * that cannot be made must not become a veto.                        */
} ob_out_t;

typedef struct {
    void  (*step)(void *state, ab_pu_t iab_meas, ab_pu_t uab_ref_lim, ob_out_t *out); /* hot */
    void  (*init)(void *state);                       /* reset runtime state (re-entry) */
    void   *state;                                    /* the implementation's state     */
} ob_t;

/* NULL-safe dispatchers: the only observer calls the core makes. A NULL ob (or NULL slot)
 * ZEROES `out` rather than leaving it, so a build with no observer just works and reads as
 * "at rest, nothing to report". `out` itself is the caller's own storage and is not checked
 * -- it is never optional, and this runs every carrier tick (cf. cc.h). */
void ob_step(const ob_t *ob, ab_pu_t iab_meas, ab_pu_t uab_ref_lim, ob_out_t *out);
void ob_init(const ob_t *ob);
#ifdef __cplusplus
}
#endif
#endif
