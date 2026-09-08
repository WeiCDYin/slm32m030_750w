#include <stddef.h>  /* NULL: the table's "no handler" and mc_hsm_tran's "no edge action" */
#include "hsm.h"
#include "cc.h"   /* cc_init: reset the dq-axes current PI regulator on entry */
#include "sc.h"   /* sc_init: reset the outer speed PI on entry */
#include "ob.h"   /* ob_init: reset the observer on entry */
#include "if.h"   /* if_init: reset the I-f ramp from standstill on entry */
#include "vf.h"   /* vf_init: reset the V/f ramp from standstill on entry */

void hsm_pwm_enable(hsm_t *h) {
    if(!h || h->pwm_en) return;
    h->pwm_en = true;
    if(h->hw && h->hw->pwm_enable) h->hw->pwm_enable();
}
void hsm_pwm_disable(hsm_t *h) {
    if(!h || !h->pwm_en) return;
    h->pwm_en = false;
    if(h->hw && h->hw->pwm_disable) h->hw->pwm_disable();
}

static void on_entry_run(hsm_t *h) { h->mc->ctrl_mode = CTRL_MODE_SAFE; }
static void on_exit_run(hsm_t *h)  { hsm_pwm_disable(h); h->mc->ctrl_mode = CTRL_MODE_SAFE; }

static void act_clear(mc_t *mc) {
    mc->act_theta   = 0;
    mc->act_spd_ref = 0;
    mc->act_spd_fb  = 0;
    mc->act_idq_ref = (dq_pu_t){ 0, 0 };
    /* A handover latched but never applied dies with the mode that latched it: left armed, it
     * would set the speed loop's integrator on some later entry from a rotor state that is
     * gone -- current and speed measured at a handover that no longer happened. */
    mc->sc_trans.set_I_pending = false;
}

static void on_entry_duty(hsm_t *h) {
    act_clear(h->mc); hsm_pwm_enable(h);
    h->mc->ctrl_mode = CTRL_MODE_DUTY;
}
/* Split so the generator init runs BEFORE ctrl_mode is published: VV seeds theta AFTER vf_init, and
 * publishing first would let a carrier tick emit the vector 90 deg off the commanded angle. */
static void enter_vf(hsm_t *h) {
    act_clear(h->mc);
    h->mc->spd_cmd   = 0;
    hsm_pwm_enable(h);
    h->mc->ctrl_mode = CTRL_MODE_VF;
}
static void on_entry_vf(hsm_t *h) { vf_init(h->mc->vf); enter_vf(h); }

static void enter_if(hsm_t *h) {
    cc_init(h->mc->cc); ob_init(h->mc->ob); act_clear(h->mc);
    h->mc->spd_cmd   = 0;
    hsm_pwm_enable(h);
    h->mc->ctrl_mode = CTRL_MODE_IF;
}
static void on_entry_if(hsm_t *h) { if_init(h->mc->ifg); enter_if(h); }
static void on_entry_vv(hsm_t *h) { vv_init(h->mc->vf);  enter_vf(h); }
static void on_entry_cv(hsm_t *h) { cv_init(h->mc->ifg); enter_if(h); }

static void if_foc_reset(mc_t *mc) {
    cc_init(mc->cc); sc_init(mc->sc); ob_init(mc->ob); if_init(mc->ifg); act_clear(mc);
    mc->spd_loop_en        = false;                     /* the transition sets spd_loop_en/sensorless (fast loop) */
    mc->sensorless         = false;
    mc->transit_dwell_tick = 0;
    mc->resync_tick        = 0;
    conv_init(&mc->conv);                               /* the angle verdict re-enters with the observer */
    mc->if_foc_stage       = IDLE;
    mc->theta_est          = 0;
    mc->cv_deg             = 0;
    mc->spd_est            = 0;
    mc->uab_ref_lim        = (ab_pu_t){ 0, 0 };         /* no stale applied voltage into the observer's 1st tick */
}
/* The sensorless entry lands WITH THE PWM OUTPUT DISABLED, mode SAFE -- and not as a special case:
 * entry clears the demand, a cleared demand is below the observability floor, and below the floor the
 * drive does not drive. Exactly the rule event_handler_foc_mode applies from then on, so the entry is
 * just its first evaluation. Poking a speed starts the run; nothing else is needed.
 *
 * It also closes the enter-into-a-TURNING-machine case. Entering while the rotor coasts down used to
 * enable the PWM output and immediately emit the zero vector into it -- the same three-phase short,
 * by a different door. */
static void on_entry_if_foc(hsm_t *h) {
    /* Disable EXPLICITLY, not by omission: arriving from a diagnostic leaf (IF -> IF_FOC) keeps
     * RUNNING entered, so the PWM output is still enabled from THAT leaf's entry. Publishing SAFE
     * into it would be the zero vector on an enabled output -- the short, by a different door. */
    hsm_pwm_disable(h);
    if_foc_reset(h->mc);
    h->mc->spd_cmd   = 0;                /* entering a mode is not a command (unlike the demand below) */
    h->mc->ctrl_mode = CTRL_MODE_SAFE;   /* entered: the PWM output is enabled when a demand arrives */
}
/* Leaving the sensorless run -> back to a CLEAN idle via the SAME reset. Entry-time reset only cleans
 * the NEXT run; this cleans the idle window between STOP and the next run, so idle telemetry reads 0
 * rather than a frozen snapshot of the last live values (nothing runs the regulators/observer in SAFE).
 * PWM output OFF first, then SAFE: a zero vector into an enabled output is a short, not a release
 * (top of file), and this handler runs BEFORE on_exit_run -- so without it the machine would be
 * shorted for the width of the four inits below. on_exit_run disables it again, harmlessly. */
static void on_exit_if_foc(hsm_t *h) {
    hsm_pwm_disable(h);
    h->mc->ctrl_mode = CTRL_MODE_SAFE;
    if_foc_reset(h->mc);
}
static void on_entry_idle(hsm_t *h)  { h->mc->ctrl_mode = CTRL_MODE_SAFE; }
static void on_entry_fault(hsm_t *h) { hsm_pwm_disable(h); h->mc->ctrl_mode = CTRL_MODE_SAFE; }

/* --- event handlers (docs/HSM.md 5.2) ----------------------------------------------------
 * Return true == CONSUMED; false bubbles to the parent. Each handler owns exactly the setpoints
 * its state runs with, so the routing lives in the state table rather than in a switch on
 * h->state kept by every port -- which is the whole reason these moved into core.
 *
 * The msk test is per FIELD, not per event: EV_SET_VEC with EV_FIELD_B alone rotates the vector and
 * leaves the magnitude where it was, which is what makes one opcode enough (hsm.h). */

/* ROOT: the universal handler. Everything that means the SAME thing in every state is written
 * here ONCE, and bubbling delivers it from wherever the drive happens to be. mc_hsm_tran does
 * its own target validation (composite / out of range), so a bad EV_TRAN payload is a no-op
 * rather than a check repeated here. */
static bool event_handler_root(hsm_t *h, const hsm_event_t *e) {
    switch(e->id) {
        case EV_TRAN:  mc_hsm_tran(h, (state_t)e->a, NULL); return true;
        case EV_FAULT: mc_hsm_tran(h, ST_FAULT,      NULL); return true;
        default:       return false;
    }
}

/* The static-vector leaves take a VECTOR and no speed: their speed is pinned to 0 by their entry
 * handler, and that pin IS what makes the vector static (hsm.h). An EV_SET_SPD arriving here is
 * therefore NOT consumed -- it bubbles off ROOT and is counted, which is the honest answer. */
static bool event_handler_vv_mode(hsm_t *h, const hsm_event_t *e) {
    if(e->id != EV_SET_VEC) return false;
    // voltage vector mag
    if(e->msk & EV_FIELD_A) vf_set_vv_mag(h->mc->vf, (q15_t)e->a);
    // voltage vector ang
    if(e->msk & EV_FIELD_B) vf_set_theta_offset(h->mc->vf, (angle_t)e->b);
    return true;
}
static bool event_handler_cv_mode(hsm_t *h, const hsm_event_t *e) {
    if(e->id != EV_SET_VEC) return false;
    // current vector mag
    if(e->msk & EV_FIELD_A) if_set_cv_mag(h->mc->ifg, (q15_t)e->a);
    // current vector ang
    if(e->msk & EV_FIELD_B) if_set_theta_offset(h->mc->ifg, (angle_t)e->b);
    return true;
}

static bool event_handler_vf_mode(hsm_t *h, const hsm_event_t *e) {
    if(e->id != EV_SET_SPD) return false;
    if(e->msk & EV_FIELD_A) h->mc->spd_cmd = (spd_pu_t)e->a;
    return true;
}

static bool event_handler_if_mode(hsm_t *h, const hsm_event_t *e) {
    if(e->id != EV_SET_SPD) return false;
    if(e->msk & EV_FIELD_A) h->mc->spd_cmd = (spd_pu_t)e->a;
    return true;
}

static bool event_handler_foc_mode(hsm_t *h, const hsm_event_t *e) {
    if(e->id != EV_SET_SPD)    return false;
    if(!(e->msk & EV_FIELD_A)) return true;
    h->mc->spd_cmd = (spd_pu_t)e->a;

    /* The floor is an OBSERVABILITY constraint, so it binds a SENSORLESS run and nothing else. A
     * sensored drive has a position sensor, holds zero speed by design, and enters IF_FOC already
     * handed over (docs/HSM.md 12) with its reference coming from mc_in_t.idq_ref or the speed loop
     * -- not from this demand, which may legitimately sit at 0 all run. Deposit and stop.
     *
     * "Sensored" reads as SENSORLESS_FOC-without-sensorless because that is how the stage encodes
     * it today (mc.c picks the sensor branch on exactly this test): the stage names the angle SOURCE
     * for one case and the PROVENANCE for the other. Worth collapsing to an explicit angle-source
     * field -- see docs/HSM.md 12 -- at which point this reads as what it means. */
    if(h->mc->if_foc_stage == SENSORLESS_FOC && !h->mc->sensorless) return true;

    int32_t spd_cmd_mag = h->mc->spd_cmd < 0 ? -(int32_t)h->mc->spd_cmd : (int32_t)h->mc->spd_cmd;
    bool    run = spd_cmd_mag >= MC_SPD_FLOOR_PU;
    bool    on  = h->pwm_en;                /* the PWM output is the "is it running" flag (hsm.h) */
    if(run && !on) {                         /* start: clean state, ENABLE, mode LAST */
        if_foc_reset(h->mc);
        /* RESYNC, not STARTUP: a start does not get to assume the machine is at rest. The stage
         * that follows is the producer's to choose once it has looked (mc.c) -- a stopped rotor
         * still gets align-hold-ramp, one dwell later. */
        h->mc->if_foc_stage = RESYNC;
        hsm_pwm_enable(h);
        h->mc->ctrl_mode    = CTRL_MODE_IFFOC;
    } else if(!run && on) {                  /* stop: DISABLE first, then SAFE, then clean up */
        hsm_pwm_disable(h);
        h->mc->ctrl_mode = CTRL_MODE_SAFE;
        if_foc_reset(h->mc);                 /* keeps spd_cmd: the demand is what disabled the output */
    }
    return true;
}

/* The containment tree + lifecycle + event routing, as data. Hierarchy = the parent column; an
 * omitted handler is NULL (== no-op, and for event_handler == "bubble"). The producing leaves all hang
 * directly off RUNNING. Every producing leaf publishes its mode on entry; IDLE/FAULT publish
 * CTRL_MODE_SAFE.
 *
 * ST_ROOT carries event_handler_root and nothing else: it is a sentinel, never entered or exited, but
 * it IS the end of every bubble chain, which is exactly where the universal handler belongs.
 *
 * ST_DUTY has NO event_handler yet -- not an oversight. Its setpoint does not live in mc_t: duties
 * arrive per-tick as mc_in_t.duty_abc (mc.c prod_duty), so a handler here would have nowhere to
 * write. EV_SET_DUTY is defined and currently bubbles off ROOT. See TODO. */
const hsm_node_t HSM[ST_COUNT] = {
    [ST_ROOT]    = { ST_ROOT,    NULL,            NULL,           event_handler_root },
    [ST_IDLE]    = { ST_ROOT,    on_entry_idle },
    [ST_RUNNING] = { ST_ROOT,    on_entry_run,    on_exit_run },
    [ST_DUTY]    = { ST_RUNNING, on_entry_duty },
    [ST_VV]      = { ST_RUNNING, on_entry_vv,     NULL,           event_handler_vv_mode },
    [ST_VF]      = { ST_RUNNING, on_entry_vf,     NULL,           event_handler_vf_mode },
    [ST_CV]      = { ST_RUNNING, on_entry_cv,     NULL,           event_handler_cv_mode },
    [ST_IF]      = { ST_RUNNING, on_entry_if,     NULL,           event_handler_if_mode },  /* no on_exit: entry comes up from a clean state */
    [ST_IF_FOC]  = { ST_RUNNING, on_entry_if_foc, on_exit_if_foc, event_handler_foc_mode },
    [ST_FAULT]   = { ST_ROOT,    on_entry_fault },   /* no on_exit: terminal */
};

static int hsm_depth(state_t s) { int d = 0; for(; s != ST_ROOT; s = HSM[s].parent) d++; return d; }

/* Does any state name s as its parent? -- i.e. is s a COMPOSITE. Read straight off the parent
 * column, so the answer follows the tree rather than a hand-kept list: add a superstate and it is
 * non-restable from its first transition. ST_ROOT is skipped as a child because it is its own
 * parent (the zero row), which would otherwise make every composite test see itself. */
static bool hsm_has_children(state_t s) {
    for(int c = ST_ROOT + 1; c < ST_COUNT; c++)
        if(HSM[c].parent == s) return true;
    return false;
}

/* calculate the least common ancestor (LCA) of state a and b */
state_t hsm_lca(state_t a, state_t b) {
    int da = hsm_depth(a), db = hsm_depth(b);
    while(da > db) { a = HSM[a].parent; da--; }
    while(db > da) { b = HSM[b].parent; db--; }
    while(a != b)  { a = HSM[a].parent; b = HSM[b].parent; }
    return a;
}

void mc_hsm_tran(hsm_t *h, state_t target, hsm_action_t action) {
    if(!h) return;
    if((unsigned)target >= ST_COUNT) return;                     /* out of range (cast: catches negatives too) */
    if(hsm_has_children(target)) return;                         /* composite -- not a state the drive may REST in (hsm.h) */
    state_t lca = hsm_lca(h->state, target);
    for(state_t s = h->state; s != lca; s = HSM[s].parent)         /* exit src..LCA (exclusive), inner->outer */
        if(HSM[s].on_exit) HSM[s].on_exit(h);
    if(action) action(h);                                          /* the edge action */
    state_t path[HSM_MAX_DEPTH]; int n = 0;                        /* collect target..LCA, then enter outer->inner */
    for(state_t s = target; s != lca; s = HSM[s].parent) path[n++] = s;
    while(n--) if(HSM[path[n]].on_entry) HSM[path[n]].on_entry(h);
    h->state = target;
}

/* --- the event layer (docs/HSM.md 5.2) ---------------------------------------------------- */

/* Weak no-ops: correct for any single-producer build, including the host tests. A port with a
 * second producer -- a serial RX path beside the cold adapter -- overrides them (hsm.h). */
__attribute__((weak)) void hsm_crit_enter(void) { }
__attribute__((weak)) void hsm_crit_exit(void)  { }

/* Walk the containment chain from the current state up to ROOT, stopping at the first handler
 * that CONSUMES. Bounded by HSM_MAX_DEPTH and cold, so the engine stays WCET-trivial even though
 * it now walks -- the thing 1 forbids is walking a hierarchy in the ISR, not walking one at all.
 * An event nobody claims is dropped and counted: 5.1 behaviour, made visible. */
static void hsm_dispatch(hsm_t *h, const hsm_event_t *e) {
    for(state_t s = h->state; ; s = HSM[s].parent) {
        if(HSM[s].event_handler && HSM[s].event_handler(h, e)) return;
        if(s == ST_ROOT) break;                       /* ROOT is its own parent: test AFTER trying it */
    }
    h->ev_unhandled++;
}

static uint8_t evq_next(uint8_t i) { return (uint8_t)((i + 1u) & (HSM_EVQ_LEN - 1u)); }

bool hsm_post(hsm_t *h, hsm_event_t e) {
    if(!h) return false;
    bool ok;
    HSM_CRIT_ENTER();                       /* two real producers once serial exists (hsm.h) */
    uint8_t next = evq_next(h->qhead);
    ok = (next != h->qtail);                /* one slot deliberately never filled: full != empty */
    if(ok) { h->q[h->qhead] = e; h->qhead = next; }
    else   { h->ev_dropped++; }
    HSM_CRIT_EXIT();
    return ok;
}

int hsm_run(hsm_t *h) {
    if(!h) return 0;
    int n = 0;

    /* The fault request first, and OUTSIDE the ring: the hardware is already safe (8), this is
     * the orderly latch. Ahead of the drain so a trip cannot wait behind queued requests. */
    if(h->fault_req) { h->fault_req = false; mc_hsm_tran(h, ST_FAULT, NULL); n++; }

    for(;;) {
        hsm_event_t e = {0};
        HSM_CRIT_ENTER();
        bool got_event = (h->qtail != h->qhead);
        if(got_event) { e = h->q[h->qtail]; h->qtail = evq_next(h->qtail); }
        HSM_CRIT_EXIT();
        if(!got_event) break;
        hsm_dispatch(h, &e);                /* to COMPLETION before the next pop -- that is the RTC */
        n++;
    }
    return n;
}
