#ifndef MC_CONV_H
#define MC_CONV_H
#include "types.h"
#include "base.h"        /* per-unit bases: the floor is stated in rpm and stored in pu */
#include "motor_cfg.h"   /* lambda_pm + npp, for that conversion (COLD PATH only) */
#ifdef __cplusplus
extern "C" {
#endif

// observer convergence check
// (1) theta err input to PLL stays within a tolerance for a number of ticks
//     --> PLL converged
// (2) observer emf magnitude estimate stays above a threshold for a number of ticks
//     --> emf estimate high enough, PLL convergence meaningful
//
// The AMPLITUDE condition is carried SQUARED, end to end: the observers publish |e|^2
// because taking the root cost ~250 cycles a tick in the carrier ISR (pll.h), and this
// gate never wanted |e| for anything but a comparison. x >= t iff x^2 >= t^2 for
// non-negative x, so squaring the threshold once in conv_tune -- a cold path that is
// already doing float work -- makes it the identical decision at zero hot-path cost.

typedef struct {
    q15_t    theta_err_tol; /* |theta_err| under which PLL converges, [Q15 radians] */
    uint32_t emf_mag_min_sq;/* |e|^2 under which theta estimate is not trustworthy, [Q30]   */
    uint16_t ticks_req;     /* ticks of credit each condition must earn                     */
    /* --- state: zeroed by conv_init ------------------------------------------------- */
    uint16_t lock_tick;     /* credit the theta_err (PLL input) has earned by staying inside tolerance */
    uint16_t mag_tick;      /* credit the AMPLITUDE has earned by clearing the floor         */
} conv_t;

// notice that emf threshold is configured by floor speed
void conv_tune(conv_t *c, const motor_cfg_t *m, const base_t *b, float floor_spd_pu);
void conv_init(conv_t *c);
void conv_step(conv_t *c, q15_t theta_err, uint32_t emf_mag_sq);
bool conv_is_converged(const conv_t *c);
#ifdef __cplusplus
}
#endif
#endif
