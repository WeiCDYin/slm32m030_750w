#ifndef __FOC_TASK_H__
#define __FOC_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"
#include "mc.h"
#include "hsm.h"
#include "smo.h"
#include "trans.h"

extern hsm_t g_hsm;

/* FOC application task (User layer). Implementation in User/Src/foc_task.c.
 *
 *   foc_init()      one-shot: tune controllers, bind observers, post ST_IDLE
 *   foc_1ms_proc()  slow loop: mc_slow_step (speed loop, 1 ms tick)
 *   foc_isr_proc()  carrier ISR: mc_fast_step, returns the new duties
 *   foc_poll()      main loop: hsm_run (supervisor event processing)
 *   foc_hsm_set()   post one event to the supervisor (any context)
 */
static inline void foc_hsm_set(uint8_t event_id, uint8_t event_msk, int16_t value_a, int16_t value_b, int16_t value_c)
{
    hsm_post(&g_hsm, (hsm_event_t){.id = event_id, .msk = event_msk, .a = value_a, .b = value_b, .c = value_c});
}

void foc_init(void);
void foc_1ms_proc(void);
void foc_isr_proc(mc_in_t *in, volatile duties_t *dabc_pu);
void foc_poll_proc(uint8_t state);

#ifdef __cplusplus
}
#endif
#endif
