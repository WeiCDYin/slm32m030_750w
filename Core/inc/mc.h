#ifndef MC_H
#define MC_H
#include "types.h"
#include "cc.h"   
#include "sc.h"   
#include "if.h"
#include "vf.h"
#include "ob.h"
#include "conv.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CTRL_MODE_NONE,     /* no control mode is selected */
    CTRL_MODE_DUTY,     /* raw phase duties -> bridge (bypasses mod) */
    CTRL_MODE_VF,       /* VF control, and voltage vector (mag. and angle) */
    CTRL_MODE_IF,       /* IF control, and current vector (mag. and angle) */
    CTRL_MODE_IF_FOC,   /* IF startup <-> sensorless FOC */
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
                          * caller that cannot measure Udc behaves exactly as before (mod.h). */
    /* SENSORED position feedback, read only when mc->sl_enable is false. Supplying them is what
     * makes a run sensored: the core picks theta_est/spd_est otherwise and never looks here. Both
     * ZERO -- which is what a sensorless port leaves them -- is therefore not a special case. */
    angle_t  theta_meas; /* rotor ELECTRICAL angle from the sensor (BAM, types.h). Becomes the
                          * working angle for the tick; mc->act_theta is the core's to write. */
    spd_pu_t spd_meas;   /* rotor ELECTRICAL speed from the sensor. Consumed in the same call, not
                          * latched: the producer republishes it as mc->act_spd_fb, which is what
                          * the outer loop reads. So it must be fresh every fast tick. */
    dq_pu_t  idq_ref;    /* dq curr. ref.            */
} mc_in_t;



/* IF_FOC's internal stage, in the order it progresses. IDLE first, so a zero-initialized mc_t is
 * inert and so the number reads as "how far has this run got".
 *
 *   IDLE            in the mode, but not running yet, PWM is off
 *   RESYNC          the first stage of every run: hold the current at ZERO in the stationary frame
 *                   and find out what the machine is already doing. No current is no torque, so a
 *                   rotor that was turning keeps turning untouched -- and the voltage the loop
 *                   settles on to hold zero current IS the back-EMF, which is what lets the
 *                   observer speak before the drive has committed to anything. It ends by looking
 *                   at the estimate: turning -> straight to SENSORLESS_FOC, catching the rotor
 *                   where it is; stopped -> STARTUP for the ordinary forced start.
 *   STARTUP         I-f: align, hold, ramp. For a machine that was at rest.
 *   SENSORLESS_FOC  handed over; the observer owns the angle and the speed loop the reference.
 *
 * So the two ways in are RESYNC -> STARTUP -> SENSORLESS_FOC (started from rest) and
 * RESYNC -> SENSORLESS_FOC (caught in motion). STARTUP is a stage a run can skip entirely.
 *
 * SENSORLESS_FOC -> STARTUP is the only edge that runs BACKWARDS, and it is not a failure path: a
 * run slowing down through the hand-back speed is leaving the speed the observer was declared
 * believable at, so it goes back to the producer that needs no observer (foc2if, mc.c).
 * The drive can then sit at any speed under it, indefinitely, in forced I-f -- which is the
 * region the deleted floor used to refuse outright. Nothing else moves backwards: RESYNC is the
 * start of a run, not somewhere a run returns to. */
typedef enum { IDLE, RESYNC, STARTUP, SENSORLESS_FOC } if_foc_stage_t;

typedef struct mc_s {
    ctrl_mode_t       ctrl_mode;    /* ctrl_mode */
    /* THE BYTE-WIDE FLAGS, KEPT TOGETHER AT THE FRONT ON PURPOSE. Thumb's compact load/store
     * encodes a field offset in 5 bits scaled by the access width, so a byte member reachable
     * within the first 32 costs one 16-bit instruction and one beyond it costs a 32-bit encoding
     * on ARMv7-M -- or, on ARMv6-M where no wide form exists, an extra instruction to build the
     * address. These four are read or written by prod_if_foc_mode on every carrier tick, and
     * they were scattered at 46 and 78-80, all past the limit. Grouped here they cost nothing
     * to reach.
     *
     * Only the BYTES were gathered. The halfwords that are also past their own limit (62) --
     * spd_est, trans_dwell_tick, and the counters inside conv -- would need those
     * nested structs relocated and most of the members below resorted, which would scatter the
     * role-based grouping the comments here depend on. That is a poor trade for a saving that is
     * zero cycles on M4 and only shows up on an ARMv6-M port. */
    if_foc_stage_t    if_foc_stage; /* IF_FOC's IDLE / RESYNC / STARTUP / SENSORLESS_FOC stage (above) */
    bool              sc_enable;    /* speed control loop enable */
    bool              sl_enable;    /* sensorless mode enable */
    bool              tr_enable;    /* transition enable, I-f <-> FOC (both directions) */
    /* HERE, not further down, for the same reason the flags above are: set_I_pending is a BYTE the
     * carrier tick reads and writes through the transition, and inside a struct only the struct can
     * move it -- a member cannot be relocated out of its own aggregate. At its old place the whole
     * group sat past 58 and every access to the flag took a wide encoding. */
    /* The arguments sc_trans_set_I will be called with, latched at the handover and applied in
     * the SLOW bucket rather than on the carrier tick.
     *
     * sc_step and the reference ramp both run at Ts_SLOW -- 20 carrier ticks away at 20 kHz --
     * so making that call inside handover2foc put ~116 ARMv6-M cycles (mostly one
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
        bool     set_I_pending;  /* raised by handover2foc, lowered once applied */
    } sc_trans;
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
    mod_gain_t        k_fwd;        /* modulator forward gain, refreshed every mc_slow_step. 0 --
                                     * never refreshed -- modulates NOTHING rather than assuming a
                                     * nominal bus, so a missing slow tick is obvious (mod.h) */
    ab_pu_t           uab_ref_lim;  /* realized ref. */
    duties_t          act_duty_abc; 
    dq_pu_t           act_idq_ref;
    spd_pu_t          act_spd_ref;
    spd_pu_t          act_spd_fb;   
    spd_pu_t          user_spd_cmd; /* the DEMAND: what was asked from outside, deposited by the
                                     * EV_SET_SPD handler (hsm.c) and never modified by the core.
                                     * Arbitrary and possibly unreachable -- mc_slow_step is what
                                     * turns it into an active reference, and what it MEANS depends
                                     * on the stage: the I-f startup's aim under the configured
                                     * cap before the handover, the speed PI's setpoint after.
                                     * One 16-bit word, so the cold writer cannot tear it. */
    spd_pu_t          spd_est;      /* the OBSERVER's electrical speed, republished every tick it
                                     * runs (ST_IF and IF_FOC). Telemetry first -- against the
                                     * FORCED speed in I-f it is the other half of the observer
                                     * check cv_deg starts, speed error beside angle error -- but
                                     * the core reads it too, so it is latched from the ob_out_t
                                     * ob_step fills, once a tick rather than once per consumer
                                     * (ob.h). 0 when there is no observer */
    spd_pu_t          if2foc_handover_spd; /* A MAGNITUDE, not a signed speed: one threshold, the
                                     * same in both directions of rotation, read as |.| wherever mc.c
                                     * tests it. Under it the drive stays in I-f, and a run that
                                     * falls back under the hand-back threshold RETURNS to I-f, so
                                     * this one number sets BOTH edges (mc.c). foc2if_handover_spd
                                     * is DERIVED from it -- three quarters of it -- rather than
                                     * configured beside it, so the two cannot be set the wrong way
                                     * round, and they straddle with room to spare. */
    uint16_t          trans_dwell_tick; /* ticks of CONTINUOUS agreement with whatever the CURRENT
                                     * stage's transition is gated on -- the forward gate's
                                     * observer-vs-forced speed match in STARTUP, the reverse gate's
                                     * under-speed check in SENSORLESS_FOC. One counter for both
                                     * because a run is only ever in one of those stages, and both
                                     * are debounces that reset to 0 the moment their test fails.
                                     * That is what separates it from resync_tick, which only ever
                                     * counts (below). */
    conv_t            conv;         /* has the observer's ANGLE arrived (conv.h)? By value and
                                     * owned HERE, not by the observer: the tolerance it grades
                                     * against is a property of what the handover does with the
                                     * angle, not of the estimator that produced it. Stepped
                                     * every observer tick (mc.c) so the verdict is warm. */
    uint16_t          resync_tick;  /* carrier ticks spent in RESYNC, counted up to the dwell that
                                     * ends it (mc.c). Separate from trans_dwell_tick on purpose:
                                     * that one is a DEBOUNCE and resets to 0 whenever its agreement
                                     * test fails, while this one only ever counts. Reusing it would
                                     * make the two stages' timing readable as one number that means
                                     * different things depending on where the run is. */
    int16_t           cv_deg;       /* CURRENT VECTOR angle from the ESTIMATED rotor d-axis */
    duties_t          user_duty_cmd; /* the DUTY-mode DEMAND, three raw phase duties, deposited by
                                      * the EV_SET_DUTY handler (hsm.c) exactly as user_spd_cmd
                                      * above is by EV_SET_SPD. It used to arrive per-tick in mc_in_t,
                                      * which meant the port wrote the core's input struct directly and
                                      * ST_DUTY was the one leaf with no event handler. Seeded to
                                      * the zero vector on entry, so the mode starts at rest.
                                      * Read by prod_duty_mode and nothing else. */
} mc_t;

/* The duties land in mc->act_duty_abc, which is where the port reads them
 * from -- deliberately NOT also returned. They have to be published there anyway (a low-side shunt
 * needs to know which duty produced the current it just sampled), and handing the same six bytes
 * back a second time bought nothing but a copy: on a core without unaligned access that copy is a
 * `bl memcpy`, and on one with it it is still a value that exists twice and could drift. */
void mc_fast_step(mc_t *mc, const mc_in_t *in);
/* Takes nothing but the drive: both halves of the speed loop are already in mc_t. The demand is
 * mc->user_spd_cmd, deposited by the EV_SET_SPD handler when it changes (hsm.c); the feedback the core
 * chooses itself -- mc->spd_est, republished by mc_fast_step every tick the observer runs, and
 * already the working speed both the sensorless and the sensored path use (mc.c). */
void mc_slow_step(mc_t *mc);

/* AT THE BOTTOM, deliberately: the assert below needs mc_t to exist, and by here this header has
 * pulled in cc.h/sc.h/if.h/vf.h/ob.h/conv.h, so the types a consumer allocates are
 * all in scope. Included from mc.h rather than left for the consumer to remember, because that is
 * the whole point -- the checks ride along with a header they already write. */
#include "mc_version.h"

/* DOES YOUR COMPILER PUT THE FIELDS OF mc_t WHERE OURS DID? You declare mc_t (`static mc_t g_mc;`)
 * so your compiler decides where each field sits, while the library that reads those fields was
 * compiled by us.
 * If the two disagree there is no error anywhere -- it compiles, links, runs, and reads the wrong
 * offsets. This turns that into a build failure. See mc_version.h for what to check when it fires.
 *
 * 0 means the configure-time bootstrap, where the size is not measured yet and there is nothing to
 * compare against; CMake refuses to install a header in that state. */
#if MC_T_SIZE_EXPECTED
/* The NAME is the message: C99 has no _Static_assert, so this expands to a negative-array typedef
 * and the identifier is all the compiler will print (mc_version.h). */
MC_STATIC_ASSERT(sizeof(mc_t) == MC_T_SIZE_EXPECTED,
    mc_t_is_not_the_size_this_release_was_built_for__check_enum_size_and_packing);
#endif

/* DID THESE HEADERS AND THAT LIBRARY COME FROM THE SAME RELEASE? Calling the tag is what asks the
 * question: a declaration nothing references links happily against any library at all, so
 * something has to USE it. Inline and called from here rather than a call the consumer must
 * remember to write. */
static inline void mc_abi_check(void) { MC_ABI_TAG(); }

#ifdef __cplusplus
}
#endif
#endif
