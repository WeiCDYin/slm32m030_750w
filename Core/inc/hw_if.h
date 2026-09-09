#ifndef MC_HW_IF_H
#define MC_HW_IF_H
#ifdef __cplusplus
extern "C" {
#endif
/* Cold-path power-stage vtable: the supervisor's ONE hardware seam. The control core never
 * touches hardware -- only the HSM's entry/exit handlers call through here (hsm.c), and every
 * call site is NULL-guarded, so a port may bind no vtable at all and still run.
 *
 * TWO SLOTS, DELIBERATELY. It also carried init/calibrate/monitor (with an mc_health_t out-param
 * for bus V and temperature) and fault_handle (with an mc_fault_t reason code). All four were
 * declared and never called by anything, in any port. In a header that SHIPS, a slot nothing
 * calls is worse than an absent one: the port defines this struct and the core reads through the
 * pointer, so its shape is ABI -- appending a member later breaks a port built against older
 * headers exactly the way growing a struct does. Better to carry only what is real and pay the
 * major bump if a fifth seam ever earns its place.
 *
 * The fault path does NOT come through here, and that is a decision rather than an omission: the
 * carrier ISR makes the hardware safe itself and raises hsm_t.fault_req, which hsm_run services
 * AHEAD of the event ring so a trip cannot queue behind requests (hsm.h). A callback would have
 * arrived after the fact and by the slower route. */
typedef struct {
    void (*pwm_enable)(void);
    void (*pwm_disable)(void);
} mc_hw_if_t;
#ifdef __cplusplus
}
#endif
#endif
