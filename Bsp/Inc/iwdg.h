#ifndef __IWDG_H__
#define __IWDG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void iwdg_init(void);
void iwdg_refresh(void);

#ifdef __cplusplus
}
#endif

#endif
