#ifndef __POKE_TASK_H__
#define __POKE_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "mc.h" /* mc_in_t */

/* Bench poke adapter (User layer). Implementation in User/Src/poke_task.c.
 * Translates host pokes (the g_poke_addr/g_poke_seq doorbell) into hsm
 * events through foc_hsm_set():
 *   poke_poll()      main loop: mirror bench flags, post mode/setpoint
 *   poke_task_isr()  carrier ISR: apply forced duty/Udc to the input
 */
void poke_task_set_duty(int16_t duty_a, int16_t duty_b, int16_t duty_c);

void poke_task_init(void);
void poke_task_poll(void);
void poke_task_1ms(void);
void poke_task_isr(mc_in_t *in);

#ifdef __cplusplus
}
#endif
#endif
