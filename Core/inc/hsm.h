#ifndef MC_HSM_H
#define MC_HSM_H
#include "mc.h"       /* mc_t + mode_t: the core the supervisor drives */
#include "hw_if.h"    /* mc_hw_if_t: the power-stage vtable the handlers toggle */
#ifdef __cplusplus
extern "C" {
#endif
/* Supervisor HSM -- the COLD, event-driven layer ABOVE the control core (see
 * docs/HSM.md). It owns the transition + resource lifecycle (gate on/off, current-loop
 * seeding, fault latch) and PUBLISHES the active control mode into mc->ctrl_mode. The core
 * knows nothing of this layer: it dispatches on mc->ctrl_mode (mc/mc.h) and never calls in
 * here. That is the inversion -- the supervisor depends on the core, not the reverse, so
 * a bare port can run a producer by writing mc->ctrl_mode directly, no supervisor linked.
 *
 * The hierarchy is flat IDs + a parent table (nesting lives in HSM[].parent). ROOT is
 * the sentinel top (parent chains terminate here; never a resting/target state). The
 * producing leaves (DUTY/VV/VF/CV/IF/IF_FOC) all hang directly off RUNNING, which owns the
 * power stage. The dq-PI/observer lifecycle that IF needs is seeded by IF's OWN entry
 * (shared with IF_FOC via reset_current_loop in hsm.c). Each producing leaf's entry
 * PUBLISHES its mode as its last action; every non-producing state publishes CTRL_MODE_SAFE. */
typedef enum {
    ST_ROOT,             /* sentinel parent */
    ST_IDLE,             /* not running; PWM output off   */
    ST_RUNNING,          /* composite: owns the power stage (gate/isense)             */
      ST_DUTY,           /* raw phase duties -> bridge (bypasses SVM) */
      ST_VV,             /* static VOLTAGE vector: V/f held at standstill (speed pinned to 0) */
      ST_VF,             /* scalar V/f: the voltage ramp spins the machine open-loop */
      ST_CV,             /* static CURRENT vector: I-f held at standstill (speed pinned to 0) */
      ST_IF,             /* forced-angle CLOSED current loop (diagnostic; terminal) */
      ST_IF_FOC,         /* sensorless run: I-f startup -> FOC, STARTUP->FOC phase internal */
    ST_FAULT,            /* latch + report (gate already off) */
    ST_COUNT             /* array sizing */
} state_t;

/* The two vector leaves come FIRST in each pair because that is the bring-up order: park a static
 * VECTOR, confirm the machine pulls into it, then let the same generator RAMP. VV (voltage vector)
 * pairs with VF, CV (current vector) with IF -- so the pair says which quantity is commanded
 * (volts open-loop / amps closed-loop) and the name says whether it stands still or ramps.
 *
 * VV and CV are leaves for the reason every leaf here is one: crossing the boundary runs
 * something. Their entry resets the same generator as their ramping sibling and then PINS the DEMAND
 * to zero -- which has to happen after that reset, so the pin cannot be undone by it. Sequencing it
 * in an entry handler is what an entry handler is for; the alternative was every caller
 * transitioning and then pinning, racing its own carrier ISR for the window in between.
 *
 * They publish the SAME ctrl_mode as their sibling (VV -> CTRL_MODE_VF, CV -> CTRL_MODE_IF): the
 * producer math is identical, only the setpoints differ. That is the mode projection doing its job
 * -- ctrl_mode names the hot-path producer, not the state -- exactly as IDLE and FAULT both project
 * onto CTRL_MODE_SAFE. Magnitude and angle stay the caller's to set, live, without re-entering
 * (vf_set_vv_mag/vf_set_theta_offset, if_set_cv_mag/if_set_theta_offset).
 *
 * LIVE, but not PERMANENT: the generator reset on the NEXT entry undoes both, precisely so a
 * diagnostic session here cannot follow the SHARED generator into its ramping sibling -- the
 * magnitude back to its configured value (vf_init from v_boost_cfg, if_init from i_tgt_cfg), the
 * angle back to zero (nothing to shadow: zero is what a non-static leaf wants). So re-entering VV or
 * CV comes up on the configured vector with the knobs still reading the last session's numbers, and
 * the caller re-commands -- the same rule as the pinned demand above, for the same reason: entering
 * a mode is not a command. */

#define HSM_MAX_DEPTH 3   /* deepest leaf sits 2 below ROOT (ROOT->RUNNING->leaf); +1 slack */

/* --- EVENTS (docs/HSM.md 5.2) -------------------------------------------------------------
 * The layer clients code against: a serial monitor, an application task, or the bench adapter
 * of 5.1. Requests are POSTED, never acted on directly, so separate requests serialize through
 * one cold slot instead of resting on the convention that only one context transitions.
 *
 * TWO VOCABULARIES, and only one of them is a command alphabet. state_t remains the whole
 * TRANSITION vocabulary (5.1) -- EV_TRAN carries a state_t rather than re-spelling it. The
 * EV_SET_* ids name PARAMETERS, which no state_t can say, because a setpoint is not a place the
 * drive rests. That is the line the deleted mc_cmd_t was on the wrong side of. */
typedef enum {
    EV_NONE,        /* 0: a zero-initialized event is inert */
    EV_TRAN,        /* a = target state_t                          (handled at ROOT) */
    EV_FAULT,       /* no payload; == EV_TRAN{ST_FAULT}            (handled at ROOT) */
    EV_SET_SPD,     /* a = forced speed, q15 pu                    (VF / IF) */
    EV_SET_VEC,     /* a = magnitude q15 pu, b = angle BAM         (VV / CV) */
    EV_SET_DUTY,    /* a,b,c = per-phase duty, q15                 (DUTY) */
} ev_id_t;

/* Which payload fields are PRESENT. A "leave this alone" sentinel is impossible here -- 0 is a
 * legal magnitude, angle and duty -- so presence has to be encoded apart from value. That is what
 * lets ONE opcode serve both "rotate the vector in place" (EV_FIELD_B) and "set magnitude and angle
 * together, in one handler call" (EV_FIELD_A|EV_FIELD_B), instead of a separate id per field. */
#define EV_FIELD_A  0x01u
#define EV_FIELD_B  0x02u
#define EV_FIELD_C  0x04u

/* Exactly 8 bytes, no padding -- and deliberately shaped like a wire frame, so the planned serial
 * transport is a parser plus one hsm_post, with no second vocabulary to translate through.
 *
 * Three 16-bit fields because EVERY payload this layer carries is 16-bit (types.h: q15_t, angle_t,
 * state_t, and the 3x q15 aggregates duties_t / polar_pu_t). The width is free: {uint8_t; int32_t;}
 * pads to 8 bytes on ARM EABI anyway, and msk occupies the byte that padding would have wasted.
 * Payloads are the CORE's currency (per-unit Q15, BAM angle); each client converts on its own side,
 * which is where 5.1 already puts the unit boundary. */
typedef struct { uint8_t id, msk; int16_t a, b, c; } hsm_event_t;

#define HSM_EVQ_LEN 8   /* ring capacity; MUST be a power of two (index masking) */

/* Post-time critical section. The ring has two real producers once a serial link exists (its RX
 * path and the cold adapter), so the index bump cannot be left unguarded -- and hsm_post is
 * explicitly ISR-safe, which is only true if something makes it so.
 *
 * Masking interrupts is an MCU concept and core stays MCU-agnostic, so these are WEAK no-ops in
 * hsm.c that the PORT overrides -- the same hook pattern slow_loop_tick already uses
 * (port/stm32g431/stm32_port.c). The no-op is correct as-is for the host test build, which has no
 * concurrency. A port that overrides them must SAVE AND RESTORE the mask, not blindly re-enable,
 * or a post from inside an ISR would drop the mask on the way out. */
void hsm_crit_enter(void);
void hsm_crit_exit(void);
#ifndef HSM_CRIT_ENTER
#define HSM_CRIT_ENTER()  hsm_crit_enter()
#define HSM_CRIT_EXIT()   hsm_crit_exit()
#endif

/* The supervisor handle: its own current state + the core it commands (where it writes
 * mc->ctrl_mode and seeds producer state) + the power-stage vtable it toggles on
 * energize/de-energize. Compose in the port: bind an mc_t and (optionally) an mc_hw_if_t.
 * A zero-init handle is valid: state ST_ROOT, no core (bind before use).
 *
 * No record of the REQUEST lives here, because nothing needs one: every request lands in a
 * DISTINCT leaf, so h->state already answers "what is the drive doing?" and a caller wanting to
 * show that (a bench flag, a host link) reads it directly rather than shadowing it. That is also
 * why there is no command enum beside state_t: it would be a second spelling of the same set. */
typedef struct hsm_s {
    state_t           state;   /* current HSM state; written ONLY by mc_hsm_tran */
    mc_t             *mc;      /* the control core this supervisor drives */
    const mc_hw_if_t *hw;      /* cold-path power-stage vtable, or NULL */

    /* Is the PWM OUTPUT ENABLED? Not a second state machine -- a resource, like mc->ctrl_mode, which
     * it is always written beside (hsm.c). It exists because the PWM output is no longer a function
     * of h->state: leaving RUNNING still disables it structurally, but INSIDE running a leaf may be
     * entered with the output disabled. IF_FOC needs exactly that -- a demand below the observability
     * floor stops the drive without leaving the mode, and stopping a TURNING machine means DISABLING
     * THE PWM OUTPUT, because the zero voltage vector is a three-phase short and not a release.
     * False on a zero-init handle, matching a port that inits its PWM with the outputs off. */
    bool              pwm_en;

    /* --- the event ring (hsm_post / hsm_run). A zero-init handle is an EMPTY queue. --- */
    hsm_event_t       q[HSM_EVQ_LEN];
    uint8_t           qhead, qtail;   /* qhead == qtail: empty. One slot is never filled. */

    /* The fault request BYPASSES the ring: the carrier ISR sets this after making the hardware
     * safe (docs/HSM.md 8), and hsm_run acts on it BEFORE draining. So a trip can never queue
     * behind a chatty link, and the ring keeps no ISR producer at all. */
    volatile bool     fault_req;

    /* Health counters -- the only answer a fire-and-forget post can give. ev_unhandled is the
     * 5.1 behaviour (a knob set in the wrong mode does nothing) made COUNTABLE rather than
     * silent; ev_dropped means the ring was full and a request was lost outright. */
    uint16_t          ev_dropped, ev_unhandled;
} hsm_t;

/* One state node: its parent (the containment tree) + the resource lifecycle that runs
 * when a transition crosses its boundary. Handlers take the hsm handle so they can reach
 * both the core (mc->ctrl_mode, seeding) and the hardware (hw). NULL handler == nothing to do. */
typedef void (*hsm_action_t)(hsm_t *);

/* Per-state event handler: returns true if it CONSUMED the event. An unconsumed event bubbles to
 * the parent, so behaviour common to a whole subtree is written once on the superstate that owns
 * it -- the same argument as the entry/exit handlers, applied to events.
 *
 * This is what earns bubbling (docs/HSM.md 4). A PARAMETER event means different things in
 * different leaves (EV_SET_VEC is volts in VV, amps in CV, meaningless in DUTY) while EV_TRAN
 * means the same thing everywhere -- so EV_TRAN/EV_FAULT are handled ONCE, at ROOT, and every
 * state accepts them without repeating a line. */
typedef bool (*hsm_handler_t)(hsm_t *, const hsm_event_t *);

typedef struct { state_t parent; hsm_action_t on_entry, on_exit;
                 hsm_handler_t event_handler; } hsm_node_t;
extern const hsm_node_t HSM[ST_COUNT];

/* Least Common Ancestor of two states in the containment tree -- the level a
 * transition does NOT cross (stays entered). Exposed for tests. */
state_t hsm_lca(state_t a, state_t b);

/* Transition to a RESTABLE target: exit src..LCA (exclusive), run the optional edge action,
 * enter LCA..target. The LCA decides which entry/exit handlers fire, so shared
 * cleanup/setup is written ONCE on the superstate that owns it. Sole writer of h->state
 * and (via the handlers) of mc->ctrl_mode. COLD CONTEXT ONLY -- must never run inside the
 * carrier ISR (docs/HSM.md §5); the hot path only requests, the supervisor performs.
 * NULL-safe.
 *
 * THE TARGET IS THE VOCABULARY. Asking the drive to do something IS naming the state it should
 * rest in -- one state per verb (§2), so there is nothing a separate command alphabet could say
 * that a state_t does not. What such an alphabet did buy was a closed set of legal targets, and
 * this function enforces that itself instead: a target that HAS CHILDREN (ST_ROOT, the ST_RUNNING
 * composite) is REJECTED, no-op, because the drive may not rest there -- entering RUNNING
 * directly would enable the PWM output with no producer behind it. Out-of-range targets go too. The
 * check is structural, not a hardcoded pair, so a composite added later is covered on arrival,
 * and it covers EVERY caller -- including the fault path, which no command alphabet named. */
void mc_hsm_tran(hsm_t *h, state_t target, hsm_action_t action);

/* The PWM output. Sole writers of hsm_t.pwm_en, and IDEMPOTENT, so a handler may ask for an edge it
 * may already hold. That matters at the hardware: the G431's pwm_enable rewrites BDTR and CLEARS THE
 * BREAK LATCH (stm32_port.c), so a redundant enable would wipe a latched trip.
 * COLD CONTEXT ONLY. NULL-safe.
 *
 * The CALLER owns the ctrl_mode pairing, because the order differs by direction and only the caller
 * knows which producer mode to publish (hsm.c, top of file):
 *   enabling  -- publish CTRL_MODE_SAFE, enable, then publish the producer mode LAST
 *   disabling -- disable FIRST, then publish CTRL_MODE_SAFE, so the zero voltage vector (equal
 *                duties, which on a turning machine is a three-phase SHORT -- see SVM_DUTY_ZERO in
 *                svm.h) is never commanded into an enabled output
 *
 * Most callers never touch these: a leaf's entry handler enables if that leaf drives on arrival,
 * IF_FOC's demand handler enables and disables as the demand crosses the observability floor, and
 * leaving ST_RUNNING always disables. They are public for the case none of those covers -- a port
 * starting SENSORED FOC, which enters IF_FOC already handed over (docs/HSM.md 12) and so has no speed
 * demand to enable on. */
void hsm_pwm_enable(hsm_t *h);
void hsm_pwm_disable(hsm_t *h);

/* Append an event to the ring. Returns false if the ring was FULL (the event is lost and
 * ev_dropped bumps) -- so a client with somewhere to report, a serial link most of all, can answer
 * honestly instead of silently doing nothing. Takes the critical section above; safe from any
 * context, INCLUDING an ISR. NULL-safe.
 *
 * By value: the event is 8 bytes -- two registers -- and it lets a call site read as one thing,
 *   hsm_post(&h, (hsm_event_t){ .id = EV_TRAN, .a = ST_VF });
 * which is where the line falls. mc_fast_step's input is 28 bytes and goes by address (mc.h). */
bool hsm_post(hsm_t *h, hsm_event_t e);

/* Perform pending work: the fault request FIRST, then drain the ring, each event dispatched to
 * completion before the next is popped. Returns how many were handled; returns 0 immediately when
 * idle, so it belongs in the port's while(1) with no fixed cadence -- which is what decouples
 * event latency from poll cadence (docs/HSM.md 5.2). NULL-safe.
 *
 * THE serialization point. Because this is the only caller of mc_hsm_tran in production and it
 * runs in exactly one context, run-to-completion ACROSS separate events stops being a convention
 * (docs/HSM.md 5) and becomes structural: two transitions cannot interleave, because one thread
 * performs them. COLD CONTEXT ONLY -- never call this from the carrier ISR. */
int hsm_run(hsm_t *h);

#ifdef __cplusplus
}
#endif
#endif
