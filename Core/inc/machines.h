#ifndef MACHINES_H
#define MACHINES_H
#include "motor_cfg.h"
#include "base.h"
#ifdef __cplusplus
extern "C" {
#endif
/* WHAT A MACHINE IS -- the physics (motor_cfg_t) bundled with the board's per-unit base (base_t),
 * so the two cannot be paired wrong. Every *_tune takes the pair, and this is the shape to hold
 * them in.
 *
 * THE TYPE SHIPS; NO ENTRY DOES. A deployed motor's parameters belong to the board they were
 * measured on -- a released archive has no business carrying them -- so a consumer defines their
 * own (see port/stm32g431/machine.c for the shape). The core's own fixture and the lookup that
 * finds it are in machine_catalog.h, which stays behind.
 *
 * DATA ONLY. Core control code never includes this; parameters reach the core solely through the
 * *_tune functions. */

typedef struct {
    const char *name;
    motor_cfg_t motor;
    base_t      base;
} machine_t;

#ifdef __cplusplus
}
#endif
#endif /* MACHINES_H */
