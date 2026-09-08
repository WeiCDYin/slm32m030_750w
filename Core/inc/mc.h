#ifndef MC_H
#define MC_H
#include "types.h"
#include "cc.h"   
#include "sc.h"   
#include "if.h"
#include "vf.h"
#include "ob.h"
#include "conv.h"
#include "rate_limiter.h"
#ifdef __cplusplus
extern "C" {
#endif
/* CONTROL MODE */
typedef enum {
    CTRL_MODE_SAFE,     /* zero voltage vector (not driving / faulted) */
    CTRL_MODE_DUTY,     /* raw phase duties -> bridge (bypasses SVM) */
    CTRL_MODE_VF,       /* VF control, and voltage vector (mag. and angle) */
    CTRL_MODE_IF,       /* IF control, and current vector (mag. and angle) */
    CTRL_MODE_IFFOC,    /* IF startup -> sensorless FOC */
    CTRL_MODE_COUNT     /* producer-table sizing */
} ctrl_mode_t;    

/* FOC current-loop state (cc_t) lives in its own module (mc/cc.h): tuned by cc_tune from
 * an SI motor_cfg_t, reset by cc_init, run by cc_step. V/f state likewise in mc/vf.h. */

/* Per-mode input bundle into mc_fast_step; only the active mode's field(s) are
 * read. One struct so the signature stays stable as modes gain inputs (cf. the
 * Simulink ExtU input struct -- here measurements and setpoints both flow in). */
typedef struct {
    abc_pu_t iabc_meas;  /* measured phase currents  */
    udc_pu_t udc_meas;   /* measured DC link (unit: types.h). Arrives with the currents rather
                          * than being owned anywhere; mc_fast_step latches it and mc_slow_step
                          * turns it into the modulator's forward gain, so the drive's one divide
                          * is paid per millisecond rather than per carrier tick. Out of
                          * domain -- which a ZEROED input is -- becomes the nominal gain, so a
                          * caller that cannot measure Udc behaves exactly as before (svm.h). */
    /* SENSORED position feedback, read only when mc->sensorless is false. Supplying them is what
     * makes a run sensored: the core picks theta_est/spd_est otherwise and never looks here. Both
     * ZERO -- which is what a sensorless port leaves them -- is therefore not a special case. */
    angle_t  theta_meas; /* rotor ELECTRICAL angle from the sensor (BAM, types.h). Becomes the
                          * working angle for the tick; mc->act_theta is the core's to write. */
    spd_pu_t spd_meas;   /* rotor ELECTRICAL speed from the sensor. Consumed in the same call, not
                          * latched: the producer republishes it as mc->act_spd_fb, which is what
                          * the outer loop reads. So it must be fresh every fast tick. */
    dq_pu_t  idq_ref;    /* dq curr. ref.            */
    duties_t duty_abc;   /* raw phase duties         */
} mc_in_t;

/* The speed below which the active-flux observer has too little back-EMF to be believed. Two things
 * read it, and they are the same statement from two directions: the transition never hands over INTO
 * a speed below this (mc.c), and a demand below it is not a speed the drive can be left holding once
 * sensorless (hsm.c). Both are SPEED questions -- one about a forced ramp, one about an operator
 * demand -- and neither has an estimate to ask about yet. Q15 pu -- ~0.03, comfortably under any
 * configured handover speed. A crude constant standing in for a real observability estimate; see
 * TODO.md. */
#define MC_SPD_FLOOR_PU   1024

/* RESYNC's back-EMF floor USED TO LIVE HERE. It is conv.c's CONV_EMF_MAG_MIN now, in the module that
 * grades the estimate -- because the level at which an angle can be VALIDATED and the level at
 * which a rotor is worth CATCHING have to be the same number, and while they were two constants
 * in two files they were free to drift apart. Deliberately not folded into MC_SPD_FLOOR_PU above
 * either: that one is a SPEED against a different base (1 pu speed lands at 0.86-0.90 pu EMF on
 * this drive's two machines), so tightening the handover speed has no business moving the
 * standstill verdict. */

/* IF_FOC's internal stage, in the order it progresses. IDLE first, so a zero-initialized mc_t is
 * inert and so the number reads as "how far has this run got".
 *
 *   IDLE            in the mode and holding NOTHING -- PWM output disabled, no current, no rotation.
 *                   Where every entry begins, because entering a mode is not a command; and where a
 *                   demand too small for the observer to work with sends it back, since the drive
 *                   should stop DRIVING without leaving the mode the operator selected.
 *   RESYNC          the first stage of every run: hold the current at ZERO in the stationary frame
 *                   and find out what the machine is already doing. No current is no torque, so a
 *                   rotor that was turning keeps turning untouched -- and the voltage the loop
 *                   settles on to hold zero current IS the back-EMF, which is what lets the
 *                   observer speak before the drive has committed to anything. It ends by looking
 *                   at the estimate: turning -> straight to SENSORLESS_FOC, catching the rotor
 *                   where it is; stopped -> STARTUP for the ordinary forced start.
 *   STARTUP         forced I-f: align, hold, ramp. For a machine that was at rest.
 *   SENSORLESS_FOC  handed over; the observer owns the angle and the speed loop the reference.
 *
 * So the two ways in are RESYNC -> STARTUP -> SENSORLESS_FOC (started from rest) and
 * RESYNC -> SENSORLESS_FOC (caught in motion). STARTUP is a stage a run can skip entirely. */
typedef enum { IDLE, RESYNC, STARTUP, SENSORLESS_FOC } if_foc_stage_t;

typedef struct mc_s {
    ctrl_mode_t       ctrl_mode;    /* ctrl_mode */
    angle_t           act_theta;    /* the ACTIVE working electrical angle: the frame the producer
                                     * is commutating in this tick. Its source is the producer's to
                                     * pick -- the V/f or I-f forced angle, the observer estimate
                                     * once sensorless, an externally supplied one otherwise -- so
                                     * nothing outside the core writes it. Pairs with act_spd_fb
                                     * below: same tick, same frame, angle and its speed. */
    cc_t             *cc;           /* cur. ctrl.,   or NULL */
    vf_t             *vf;           /* VF ref. gen., or NULL */
    if_t             *ifg;          /* IF ref. gen., or NULL */
    sc_t             *sc;           /* spd. ctrl. (in mc_slow_step), or NULL */
    const ob_t       *ob;           /* generic observer handle (vtable + state) */
    angle_t           theta_est;    /* latest observer angle estimate       */
    udc_pu_t          udc_meas;     /* this tick's bus, latched by mc_fast_step for the slow bucket.
                                     * One 16-bit word, so a preempted cold pass always reads a
                                     * coherent value -- the contract ctrl_mode uses (HSM.md 5) */
    svm_gain_t        k_fwd;        /* modulator forward gain, refreshed every mc_slow_step. 0 --
                                     * never refreshed -- modulates NOTHING rather than assuming a
                                     * nominal bus, so a missing slow tick is obvious (svm.h) */
    ab_pu_t           uab_ref_lim;  /* realized ref. */
    duties_t          act_duty_abc; /* what mc_fast_step ACTUALLY modulated this tick -- THE result,
                                     * not a copy of one: the port loads its CCRs from here, and so
                                     * does anything asking which duty produced a given current.
                                     * That is exactly the question a low-side shunt asks, since its
                                     * sample is only valid while that phase's duty leaves the low
                                     * side on long enough (TODO.md). Written on the way OUT of
                                     * mc_fast_step, so SAFE and the out-of-range fallback land here
                                     * through the same point as the real producers. Read-only
                                     * outside: the core never reads it back. */
    int16_t           peak_duty_pct; /* max(act_duty_abc), in WHOLE PERCENT. The quantity the
                                     * low-side sampling window is bounded in (TODO.md): a shunt
                                     * reads valid current only while its phase's low side is on,
                                     * and that window closes as that phase's duty rises. The PEAK
                                     * is what decides it, and it is exactly what a Clarke of the
                                     * duties cannot report -- min/max injection is zero-sequence,
                                     * which the transform discards by design (svm.c), while the
                                     * peak ripples 0.5+0.433m .. 0.5+0.5m with angle. Percent, not
                                     * Q15, so it reads directly against the ceilings and against
                                     * the port's _pct duty knobs. */
    int16_t           mod_idx_pct;  /* MODULATION INDEX in percent: the duty space vector
                                     * magnitude, scaled so that 100
                                     * is the linear limit, i.e. how deep the drive is modulating
                                     * as a fraction of what it HAS. Flat in steady state, unlike
                                     * peak_duty_pct's 6x-electrical scallop, and it starts at 0
                                     * rather than 50 -- the zero vector is no modulation and the
                                     * Clarke reports it as none. Sampling-window ceilings land at
                                     * 77 (phase b) and 89 (a and c), 100 IS the linear limit --
                                     * and it reads 100 at sector centres and boundaries alike,
                                     * which is the property peak_duty_pct lacks. Past the limit
                                     * svm.c clamps the duties, and that clamping shows up HERE as
                                     * a scallop the linear range does not have: pinned at 100 on
                                     * the boundaries, rising toward 115 at the centres as the
                                     * pattern squares up (2/3 is six-step's Clarke magnitude). A
                                     * flat 100 that starts to ripple is the drive entering
                                     * overmodulation. Computed in mc_SLOW_step: it moves at load
                                     * and speed rates, so the isqrt stays out of the carrier ISR. */
    if_foc_stage_t    if_foc_stage; /* IF_FOC's IDLE / RESYNC / STARTUP / SENSORLESS_FOC stage (above) */
    /* The ACTIVE references -- what the loop is tracking THIS tick, republished by whichever
     * producer is running (I-f's forced ramp, the speed PI, the caller's fast input). Already
     * capped, rate-limited and stage-appropriate, so they are what the transition seed and the
     * telemetry read. Distinct from the operator's DEMAND, which is an input and arrives
     * separately -- the same cmd/ref split the generators keep one level down (vf_t.spd_cmd
     * chased by spd_acc, if_t.spd_tgt by its ramp). */
    dq_pu_t           act_idq_ref;
    /* The arguments sc_trans_set_I will be called with, latched at the handover and applied in
     * the SLOW bucket rather than on the carrier tick.
     *
     * sc_step and the reference ramp both run at Ts_SLOW -- 20 carrier ticks away at 20 kHz --
     * so making that call inside transition_to_foc put ~116 ARMv6-M cycles (mostly one
     * pi2dof_set_I, whose Q30 products spill on a core with no long multiply) on the drive's
     * single most expensive tick, for a consumer that would not run for another millisecond.
     *
     * DEFERRED, NOT RECOMPUTED. iq_meas is the q-axis current measured at the handover and
     * spd_est the observer's estimate then; re-reading either a millisecond later gives a
     * different number, so all three arguments travel with the flag. The call still happens
     * before the speed loop's first output, which is the only thing it exists to make bumpless. */
    struct {
        q15_t    iq_meas;        /* measured i_q at the handover                      */
        spd_pu_t spd_ref;        /* the reference it is set against                   */
        spd_pu_t spd_est;        /* and the observer's estimate at that instant       */
        bool     set_I_pending;  /* raised by transition_to_foc, lowered once applied */
    } sc_trans;
    spd_pu_t          act_spd_ref;
    spd_pu_t          act_spd_fb;   /* the drive's WORKING electrical speed this tick -- what the
                                     * producer is actually commutating at, and the same number it
                                     * hands cc_step as `we`. Its SOURCE is the producer's to pick,
                                     * exactly as act_theta's is: the forced ramp in V/f and I-f,
                                     * the observer once sensorless, the sensor when sensored. The
                                     * outer loop closes on THIS, so the speed it regulates can
                                     * never disagree with the speed the fast path is rotating by. */
    spd_pu_t          spd_cmd;      /* the DEMAND: what was asked from outside, deposited by the
                                     * EV_SET_SPD handler (hsm.c) and never modified by the core.
                                     * Arbitrary and possibly unreachable -- mc_slow_step is what
                                     * turns it into an active reference, and what it MEANS depends
                                     * on the stage: the I-f startup's aim under the configured
                                     * cap before the handover, the speed PI's setpoint after.
                                     * One 16-bit word, so the cold writer cannot tear it. */
    rate_limiter_t    spd_ramp;     /* the RATE LIMIT between that demand and act_spd_ref while the
                                     * speed loop is closed (mc_slow_step). By value and owned HERE,
                                     * like conv above and for the same kind of reason: how fast a
                                     * drive is allowed to change speed follows from its current
                                     * limit against its inertia -- a property of the DRIVE, not of
                                     * the regulator that happens to be tracking the result, which
                                     * would otherwise carry a second time constant unrelated to the
                                     * plant it is tuned to. Seeded at the handover so the reference
                                     * leaves the forced speed continuously instead of stepping to
                                     * the setpoint (mc.c). Untuned == UNLIMITED (rate_limiter.h), so
                                     * a port that configures no rate steps exactly as it always did. */
    spd_pu_t          spd_est;      /* the OBSERVER's electrical speed, republished every tick it
                                     * runs (ST_IF and IF_FOC). Telemetry first -- against the
                                     * FORCED speed in I-f it is the other half of the observer
                                     * check cv_deg starts, speed error beside angle error -- but
                                     * the core reads it too, so it is latched from the ob_out_t
                                     * ob_step fills, once a tick rather than once per consumer
                                     * (ob.h). 0 when there is no observer */
    bool              spd_loop_en;  /* spd loop enable, enable the loop only as needed */
    bool              sensorless;   /*  */
    bool              transit_en;   /*  */
    spd_pu_t          handover_spd; /* where the forced I-f startup stops climbing and FOC takes over
                                     * (electrical, per-unit Q15). A property of the OBSERVER and the
                                     * machine -- how much back-EMF it needs to lock -- so it belongs
                                     * to the drive, not to the I-f generator's config. It caps the
                                     * demand during STARTUP: a demand above it hands over here and
                                     * the speed loop climbs the rest; a demand BELOW it hands over
                                     * at the demand, so a slow setpoint is never overshot just to
                                     * start. Magnitude only -- the demand keeps its sign, so a
                                     * reverse demand starts in reverse. 0 disables the cap. */
    uint16_t          transit_dwell_tick; /* */
    conv_t            conv;         /* has the observer's ANGLE arrived (conv.h)? By value and
                                     * owned HERE, not by the observer: the tolerance it grades
                                     * against is a property of what the handover does with the
                                     * angle, not of the estimator that produced it. Stepped
                                     * every observer tick (mc.c) so the verdict is warm. */
    uint16_t          resync_tick;  /* carrier ticks spent in RESYNC, counted up to the dwell that
                                     * ends it (mc.c). Separate from transit_dwell_tick on purpose:
                                     * that one is a DEBOUNCE and resets to 0 whenever its agreement
                                     * test fails, while this one only ever counts. Reusing it would
                                     * make the two stages' timing readable as one number that means
                                     * different things depending on where the run is. */
    int16_t           cv_deg;       /* CURRENT VECTOR angle from the ESTIMATED rotor d-axis */
} mc_t;

/* Runs one carrier tick. The duties land in mc->act_duty_abc, which is where the port reads them
 * from -- deliberately NOT also returned. They have to be published there anyway (a low-side shunt
 * needs to know which duty produced the current it just sampled), and handing the same six bytes
 * back a second time bought nothing but a copy: on a core without unaligned access that copy is a
 * `bl memcpy`, and on one with it it is still a value that exists twice and could drift. */
void mc_fast_step(mc_t *mc, const mc_in_t *in);
/* Takes nothing but the drive: both halves of the speed loop are already in mc_t. The demand is
 * mc->spd_cmd, deposited by the EV_SET_SPD handler when it changes (hsm.c); the feedback the core
 * chooses itself -- mc->spd_est, republished by mc_fast_step every tick the observer runs, and
 * already the working speed both the sensorless and the sensored path use (mc.c). */
void mc_slow_step(mc_t *mc);

#ifdef __cplusplus
}
#endif
#endif
