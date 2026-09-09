#ifndef __POKE_TASK_H__
#define __POKE_TASK_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "mc.h" /* mc_in_t */

void poke_task_init(void);
void poke_task_poll(void);
void poke_task_1ms(void);


#ifdef __cplusplus
}
#endif
#endif
