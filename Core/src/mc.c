#include "mc.h"
#include "trans.h"
#include "svm.h"
#include "qmath.h"

/* sqrt(3) * 100, in Q8 -- the duty vector -> modulation-index-in-percent scale (mc.h). */
#define MOD_IDX_PCT_Q8     44341            /* round(sqrt(3) * 100 * 2^8) */
#define MOD_IDX_PCT_SHIFT  (Q15_SHIFT + 8)  /* the magnitude is Q15, the constant Q8 */

/* The zero voltage vector -- 50% duty on all three phases */
static void prod_safe(mc_t *mc, const mc_in_t *in, duties_t *duty) {
    (void)mc; (void)in;
    *duty = SVM_DUTY_ZERO;
}

/* raw duties straight through */
static void prod_duty(mc_t *mc, const mc_in_t *in, duties_t *duty) {
    (void)mc;
    *duty = in->duty_abc;
}

/* VF & voltage vector (vv) mode */
static void prod_vf(mc_t *mc, const mc_in_t *in, duties_t *duty) {
    (void)in;                                    
    polar_pu_t vv_ref = vf_step(mc->vf, mc->spd_cmd);
    mc->act_theta     = vv_ref.angle;
    mc->act_spd_ref   = vf_get_spd(mc->vf);       
    // note that voltage vector is on the controller q-axis
    dq_pu_t udq_ref = {.d = 0, .q = vv_ref.mag};
    ab_pu_t uab_ref = dq2ab(udq_ref, sincos_q15(mc->act_theta));
    svm(uab_ref, mc->k_fwd, duty);
}

/* BAM angle difference -> electrical degrees, for telemetry only: 360/2^16 == 45/2^13, and the
 * +2^12 rounds rather than truncating. One multiply and a shift -- no divide (svm.h). */
static int16_t bam2deg(int16_t bam) { return (int16_t)((((int32_t)bam * 45) + 4096) >> 13); }

/* IF & current vector (cv)
   In IF mode, current vector controlled to be on controller q-axis, and initial angle is -90 deg., q-axis aligned with phase-a flux axis.
   In CV mode, current vector controlled to be on controller d-axis, and initial angle is   0 deg., d-axis aligned with phase-a flux axis. 
*/
static void prod_if(mc_t *mc, const mc_in_t *in, duties_t *duty) {
    ab_pu_t iab_meas = abc2ab(in->iabc_meas);
    ob_out_t ob_out;
    ob_step(mc->ob, iab_meas, mc->uab_ref_lim, &ob_out);    /* NULL-safe: zeroes ob_out (ob.h) */
    if(mc->ob) mc->theta_est = ob_out.theta_est;   /* no observer -> keep the last angle */
    mc->spd_est = ob_out.spd_est;

    // current vector reference from IF control
    polar_pu_t cv_ref = if_step(mc->ifg, mc->spd_cmd);   
    mc->act_theta   = cv_ref.angle;
    mc->act_spd_ref = if_get_spd(mc->ifg);
    // current vector on the controller q-axis      
    mc->act_idq_ref = (dq_pu_t){.d = 0, .q = cv_ref.mag};

    /* current vector location w.r.t estimated d-axis */
    mc->cv_deg = bam2deg((int16_t)(mc->act_theta + DEG_BAM(90) - mc->theta_est));

    // current loop -> duties
    /* Straight into the caller's duty slot and the core's realized-reference field: no
     * intermediate struct to copy out of (cc.h). */
    cc_step(mc->cc, &(cc_in_t){
        .idq_ref  = mc->act_idq_ref,
        .iab_meas = iab_meas,
        .theta    = mc->act_theta,
        .we       = mc->act_spd_fb,                
        .k_fwd    = mc->k_fwd,
        .udc_meas = in->udc_meas,
    }, duty, &mc->uab_ref_lim);
}

// calculate the target speed for I-f startup 
static spd_pu_t calc_startup_tgt_spd(const mc_t *mc) {
    int32_t tgt_spd = mc->spd_cmd;
    if(!mc->transit_en || mc->handover_spd <= 0) return (spd_pu_t)tgt_spd;
    const int32_t cap = mc->handover_spd;
    if(tgt_spd >  cap) tgt_spd =  cap;
    if(tgt_spd < -cap) tgt_spd = -cap;
    return (spd_pu_t)tgt_spd;
}

/* Transition gate (docs/HSM.md §7), evaluated each STARTUP tick: hand over to sensorless FOC once
 * the observer has locked. Enabled by mc->transit_en (off => stay forced I-f). Fires when the I-f
 * ramps are done AND the observer speed has agreed with the forced speed for TRANSIT_DWELL_TICKS
 * ticks (a debounced convergence detector; forced and actual speeds are equal at steady state).
 * NB the threshold is PLURAL and the mc_t counter it is compared against is singular: a macro
 * spelled exactly like the field would be substituted into `mc->transit_dwell_tick` itself. */
// [todo] need to make it configurable by user
#define TRANSIT_W_TOL       655   /* observer vs forced speed agreement, Q15 pu (~0.02)            */
#define TRANSIT_DWELL_TICKS 1000  /* ticks of CONTINUOUS agreement before the transition:
                             * the PLL locks frequency before phase, so a long dwell waits out the
                             * angle transient (θ̂ settling) -- see docs/HSM.md §7 / the SMO |e|-norm TODO */

// [todo] the gate need to check if the observer result is valid rather
static bool transition_gate(mc_t *mc) {
    if(!mc->transit_en || !mc->ob || !mc->ifg) { 
        return false;
    }
    if(!if_ss_reached(mc->ifg)) {
        return false;
    }
    int32_t w_if = if_get_spd(mc->ifg);
    if(w_if < 0) { 
        w_if = -w_if;
    }
    // [todo] need to change for clarity. what is this floor for?
    if(w_if < MC_SPD_FLOOR_PU) { 
        mc->transit_dwell_tick = 0; 
        return false; 
    }
    // spd diff between observer and I-f stays within TOL  
    int32_t spd_diff = (int32_t)mc->spd_est - if_get_spd(mc->ifg);
    if(spd_diff < 0) {
        spd_diff = -spd_diff;
    }
    if(spd_diff > TRANSIT_W_TOL) { 
        mc->transit_dwell_tick = 0; 
        return false; 
    }
    if(mc->transit_dwell_tick < TRANSIT_DWELL_TICKS) {
        mc->transit_dwell_tick++; 
        return false; 
    }
    return true;
}

static void transition_to_foc(mc_t *mc, ab_pu_t iab_meas) {
    const sincos_t cs_est = sincos_q15(mc->theta_est);   /* both rotations are about theta_est */
    dq_pu_t idq_est = ab2dq(iab_meas, cs_est);
    mc->act_theta   = mc->theta_est;                     
    mc->act_idq_ref = (dq_pu_t){ .d = 0, .q = idq_est.q };
    dq_pu_t udq_applied = ab2dq(mc->uab_ref_lim, cs_est);
    
    // init integrators for bumpless (ideally) transition
    /* The CURRENT loop's integrators are set here because cc_step runs at the end of this very
     * tick, and the whole point is that the next duty is bumpless. */
    cc_trans_set_I(mc->cc, udq_applied, mc->act_idq_ref, idq_est);
    /* The SPEED loop's are not. Its consumer is mc_slow_step, up to 20 carrier ticks away, so the
     * arguments are latched and the call happens there (mc.h). */
    mc->sc_trans.iq_meas       = idq_est.q;
    mc->sc_trans.spd_ref       = mc->act_spd_ref;
    mc->sc_trans.spd_est       = mc->spd_est;
    mc->sc_trans.set_I_pending = true;
    mc->spd_loop_en   = true;
    mc->sensorless    = true;
    mc->cv_deg        = 0;    /* undefined from here */
    mc->if_foc_stage  = SENSORLESS_FOC;
}

/* How long the ABSENCE of back-EMF has to persist before RESYNC calls the rotor stopped. Only the
 * absence needs a timer: any positive answer arrives on its own evidence (conv.h counts both the
 * amplitude and the angle), while "there is nothing there" cannot be distinguished from "it has
 * not arrived yet" except by waiting. The estimate is algebraic in the current error and settles
 * in ~13 ms; this is that, with room.
 *
 * It was a DWELL before -- a fixed hold in front of both decisions, standing in for the lock the
 * drive could not measure. Swept on BL4260 into a fan the air drives, 200 ticks CATCHES at every
 * windmill speed but the marginal one then collapses to 58 rpm, because it handed over on an angle
 * the PLL had not found; 400 catches and recovers at 400/600/900 rpm. That number is kept here so
 * a REST start decides exactly when it used to -- but it no longer TIMES a catch, which now ends
 * when the angle arrives: RESYNC+339..583 on BL4260 and +451 on REF in sim. Sometimes later than
 * the old 400, sometimes sooner, and that is the point -- each run waits for its own evidence
 * instead of every run waiting out the worst one's. */
#define RESYNC_REST_TICKS 400

/* Ceiling on how long RESYNC will wait for the angle to arrive (conv.h). Reached means it never
 * did, and the drive hands over anyway -- which is what it did before the check existed, so the
 * ceiling can only make RESYNC take LONGER than the old fixed dwell, never decide differently. */
#define RESYNC_CONV_CAP_TICKS 2000
static void prod_if_foc(mc_t *mc, const mc_in_t *in, duties_t *duty) {

    ab_pu_t iab_meas = abc2ab(in->iabc_meas);
    /* ONE call, and everything this tick's observer produced comes back in it (ob.h). The four
     * used to be an angle plus three dispatched getters; every consumer below read all four. */
    ob_out_t ob_out;
    ob_step(mc->ob, iab_meas, mc->uab_ref_lim, &ob_out);   /* NULL-safe: zeroes ob_out */
    if(mc->ob) mc->theta_est = ob_out.theta_est;   /* no observer -> keep the last angle */
    mc->spd_est = ob_out.spd_est;
    /* Grade the estimate every tick, not only where it is asked about: the verdict needs a run of
     * evidence (conv.h), so a gate that only starts looking when it wants an answer would spend
     * the whole run earning credit it could already have had. */
    conv_step(&mc->conv, ob_out.theta_err, ob_out.emf_est_sq);

    spd_pu_t we = 0;                                   
    switch(mc->if_foc_stage) {
    case RESYNC: {
        // ctrl the current to zero and check estimated emf from observer
        mc->act_theta   = 0;
        mc->act_idq_ref = (dq_pu_t){ .d = 0, .q = 0 };
        we              = 0;  
        /* Both decisions are read off the same verdict, which is why they can no longer disagree
         * about how much back-EMF is enough (conv.h):
         *
         *   AMPLITUDE decides WHETHER to catch. Too little back-EMF is a rotor at rest, and a
         *   rest start is the ordinary forced one -- but only its ABSENCE needs a deadline, since
         *   nothing distinguishes "stopped" from "not arrived yet" except waiting.
         *
         *   THE ANGLE decides WHEN. The rotor is turning, so the catch is settled -- but the
         *   handover JUMPS the control frame onto theta_est, and an estimate still acquiring is
         *   not an angle to jump to. Holding costs nothing: no current is no torque, so the rotor
         *   keeps turning untouched while the loop finds it. */
        if(!conv_is_converged(&mc->conv)) {
            if(mc->conv.mag_tick < mc->conv.ticks_req && mc->resync_tick >= RESYNC_REST_TICKS) {
                mc->if_foc_stage = STARTUP;
                break;
            }
            if(mc->resync_tick < RESYNC_CONV_CAP_TICKS) {
                mc->resync_tick++;
                break;
            }
        }
        mc->act_spd_ref = mc->spd_est;
        transition_to_foc(mc, iab_meas);
        we = mc->spd_est;
        break;
    }

    case STARTUP: {                                    
        polar_pu_t cv_ref = if_step(mc->ifg, calc_startup_tgt_spd(mc));
        spd_pu_t w_if     = if_get_spd(mc->ifg);           
        mc->act_theta     = cv_ref.angle;
        mc->act_idq_ref   = (dq_pu_t){ .d = 0, .q = cv_ref.mag };
        mc->act_spd_ref   = w_if;                         
        we                = w_if;
        // current vector angle w.r.t estimated d-axis                          
        mc->cv_deg = bam2deg((int16_t)(mc->act_theta + DEG_BAM(90) - mc->theta_est));

        // [todo] 
        if(transition_gate(mc))                         /* observer locked */
            transition_to_foc(mc, iab_meas);
        break;
    }
    case SENSORLESS_FOC:
        // sensorless mode 
        if(mc->sensorless) {                            
            mc->act_theta = mc->theta_est;
            we            = mc->spd_est;                
            break;
        }
        /* sensored FOC: the SENSOR owns the angle, and it arrives as an input like every other
         * measurement -- the port does not reach in and write mc->act_theta. With the speed loop OFF
         * this is TORQUE mode -- the reference is the caller's fast input (mc_in_t.idq_ref); with
         * it ON, the speed PI owns mc->act_idq_ref (mc_slow_step wrote it). The observer still
         * runs above, but only as a parallel estimate: nothing sensored consumes it. */
        mc->act_theta = in->theta_meas;
        we            = in->spd_meas;
        if(!mc->spd_loop_en) mc->act_idq_ref = in->idq_ref;
        break;

    case IDLE: break;    /* output disabled: the supervisor publishes SAFE, so this is not dispatched */
    }

    /* Whichever branch ran picked the working speed; publish it once, here, so the slow bucket
     * reads the SAME number the rotation below uses instead of re-deciding the source. */
    mc->act_spd_fb = we;

    // current control loop -> duties, and the realized ref the observer needs
    cc_step(mc->cc, &(cc_in_t){
        .idq_ref  = mc->act_idq_ref,
        .iab_meas = iab_meas,
        .theta    = mc->act_theta,
        .we       = mc->act_spd_fb,
        .k_fwd    = mc->k_fwd,
        .udc_meas = in->udc_meas,
    }, duty, &mc->uab_ref_lim);
}

/* Producers WRITE their duties through the pointer rather than returning them: mc_fast_step
 * hands over &mc->act_duty_abc, so the result lands in its final home with no copy (cc.h). */
typedef void (*calc_duty_fn)(mc_t *, const mc_in_t *, duties_t *);
static const calc_duty_fn calc_duty[CTRL_MODE_COUNT] = {
    [CTRL_MODE_DUTY] = prod_duty, [CTRL_MODE_VF]     = prod_vf,
    [CTRL_MODE_IF]   = prod_if,   [CTRL_MODE_IFFOC]  = prod_if_foc,
    /* CTRL_MODE_SAFE -> NULL -> prod_safe */
};

void mc_fast_step(mc_t *mc, const mc_in_t *in) {
    if(!mc || !in) return;   /* nothing to publish into, and no result to hand back */
    mc->udc_meas  = in->udc_meas;      /* latched; mc_slow_step owns the reciprocal it feeds */
    /* CTRL_MODE_SAFE and out-of-range fall to prod_safe (zero voltage vector). Every path writes
     * act_duty_abc and none of them returns early, so the publish below cannot be bypassed. */
    if(mc->ctrl_mode < CTRL_MODE_COUNT && calc_duty[mc->ctrl_mode])
        calc_duty[mc->ctrl_mode](mc, in, &mc->act_duty_abc);
    else
        prod_safe(mc, in, &mc->act_duty_abc);
    /* Peak of the three (mc.h). Two compares, no transform and no sqrt. Rounded rather than
     * truncated so a railed duty reads 100 and not 99 -- q15_t tops out at Q15_MAX. */
    q15_t pk = mc->act_duty_abc.a;
    if(mc->act_duty_abc.b > pk) pk = mc->act_duty_abc.b;
    if(mc->act_duty_abc.c > pk) pk = mc->act_duty_abc.c;
    mc->peak_duty_pct = (int16_t)((((int32_t)pk * 100) + (Q15_ONE / 2)) >> Q15_SHIFT);
}

// currently mainly speed loop
void mc_slow_step(mc_t *mc) {
    if(!mc) return;
    /* take into account of actual Udc. But put divide operation in cold path */
    mc->k_fwd = svm_calc_gain(mc->udc_meas);
    
    // modulation index
    const ab_pu_t d = abc2ab((abc_pu_t){ mc->act_duty_abc.a,
                                         mc->act_duty_abc.b,
                                         mc->act_duty_abc.c });
    const q15_t mag = isqrt_q30((uint32_t)((int32_t)d.afa * d.afa)
                              + (uint32_t)((int32_t)d.bet * d.bet));
    mc->mod_idx_pct = (int16_t)(((int32_t)mag * MOD_IDX_PCT_Q8
                                 + (1 << (MOD_IDX_PCT_SHIFT - 1))) >> MOD_IDX_PCT_SHIFT);
    
    // only do speed loop calc. when enabled
    if(!mc->spd_loop_en || !mc->sc) return;               
    /* The handover's deferred call (mc.h). It runs BEFORE the ramp steps and before sc_step, so
     * the speed loop's first output is the one these arguments were latched for. */
    if(mc->sc_trans.set_I_pending) {
        sc_trans_set_I(mc->sc, mc->sc_trans.iq_meas, mc->sc_trans.spd_ref, mc->sc_trans.spd_est);
        rate_limiter_set(&mc->spd_ramp, mc->sc_trans.spd_ref);
        mc->sc_trans.set_I_pending = false;
    }
    mc->act_spd_ref = rate_limiter_step(&mc->spd_ramp, mc->spd_cmd);
    mc->act_idq_ref = (dq_pu_t){ .d = 0, .q = sc_step(mc->sc, mc->act_spd_ref, mc->act_spd_fb) };
}
