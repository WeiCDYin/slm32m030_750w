#ifndef MC_HW_IF_H
#define MC_HW_IF_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Cold-path power-stage vtable: the supervisor's hardware seam (gate on/off, cal,
 * health, fault). Lives with the supervisor (super/), NOT the core -- the control
 * core never touches hardware; only the HSM entry/exit handlers do. */
typedef struct { int dummy; /* TODO: bus V, temp */ } mc_health_t;
typedef enum { MC_FAULT_NONE, MC_FAULT_OVERCURRENT, MC_FAULT_OVERVOLTAGE, MC_FAULT_SENSOR } mc_fault_t;
typedef struct {                  /* cold-path lifecycle vtable */
    void (*init)(void);
    void (*pwm_enable)(void);
    void (*pwm_disable)(void);
    void (*calibrate)(void);      /* encoder alignment, offset/gain cal */
    bool (*monitor)(mc_health_t *out);
    void (*fault_handle)(mc_fault_t f);
} mc_hw_if_t;
#ifdef __cplusplus
}
#endif
#endif
