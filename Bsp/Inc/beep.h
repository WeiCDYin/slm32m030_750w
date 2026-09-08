#ifndef __BEEP_H__
#define __BEEP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define BEEP_FREQ (18000 / 4.5f)

void beep_init(void);
void beep_start(void);
void beep_stop(void);

#ifdef __cplusplus
}
#endif

#endif
