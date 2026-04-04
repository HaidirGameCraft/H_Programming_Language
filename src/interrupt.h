#pragma once

/**
 * Interrupt
 * - The interrupt service that called in H from C
 */

#include <cpu.h>
#include <stdint.h>

#define MAX_INTERRUPT_ARRAY         256

typedef struct __interrupt_t {
    uint64_t handle;
} interrupt_t;

#define DivideZeroExceptionInt      0x00
#define UnknownOpcodeExceptionInt   0x01
#define OutOfMemoryExceptionInt     0x02

EXTR void InitInterrupt();
EXTR void AddInterrupt( int index, uint64_t address );
EXTR void CallInterrupt( int index );
