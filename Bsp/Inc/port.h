#ifndef _PORT_H
#define _PORT_H

#include <stdint.h>
#include <stdbool.h>

/* FreeModbus port glue for SLM32M030 (Cortex-M0+). */

/* Interrupt-free critical sections, compiler-local and HAL free: the FreeModbus
 * glue and the application protection code (fault.c) share these. */
#if defined(__GNUC__) || defined(__clang__)
#define ENTER_CRITICAL_SECTION() __asm volatile("cpsid i" ::: "memory")
#define EXIT_CRITICAL_SECTION()  __asm volatile("cpsie i" ::: "memory")
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define ENTER_CRITICAL_SECTION() __disable_irq()
#define EXIT_CRITICAL_SECTION()  __enable_irq()
#else
#error "port.h: unsupported compiler for critical section"
#endif

#define INLINE inline
#define PR_BEGIN_EXTERN_C                                                                                                                            \
    extern "C"                                                                                                                                       \
    {
#define PR_END_EXTERN_C }


typedef uint8_t       BOOL;
typedef unsigned char UCHAR;
typedef char          CHAR;
typedef uint16_t      USHORT;
typedef int16_t       SHORT;
typedef uint32_t      ULONG;
typedef int32_t       LONG;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif
