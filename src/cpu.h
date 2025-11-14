/**
 * cpu.h
 * Created By Haidir
 */
#pragma once

#include <stdint.h>

#define eflags_carry_flags 1 << 0
#define eflags_parity_flags 1 << 2
#define eflags_auxiliary_flags 1 << 4
#define eflags_zero_flags 1 << 6
#define eflags_sign_flags 1 << 7
#define eflags_trap_flags 1 << 8
#define eflags_interrupt_enable 1 << 9
#define eflags_direction_flags 1 << 10
#define eflags_overflow 1 << 11
#define eflags_io_privilege_level 1 << 12
#define eflags_nested_task_flags 1 << 13
#define eflags_resume_flags 1 << 15
#define eflags_v8086_flags 1 << 16

typedef struct {
    union {
        uint8_t r8a;
        uint16_t r16a;
        uint32_t r32a;
    };
    union {
        uint8_t r8b;
        uint16_t r16b;
        uint32_t r32b;
    };
    union {
        uint8_t r8c;
        uint16_t r16c;
        uint32_t r32c;
    };
    union {
        uint8_t r8d;
        uint16_t r16d;
        uint32_t r32d;
    };
    union {
        uint8_t r8e;
        uint16_t r16e;
        uint32_t r32e;
    };
    union {
        uint8_t r8f;
        uint16_t r16f;
        uint32_t r32f;
    };
    union {
        uint8_t r8s;
        uint16_t r16s;
        uint32_t r32s;
    };
    union {
        uint8_t r8bp;
        uint16_t r16bp;
        uint32_t r32bp;
    };

    union {
        uint16_t pc16;
        uint32_t pc32;
    };
    uint16_t flags;
} cpu_register_t;

#define REG_A   0b000
#define REG_B   0b001
#define REG_C   0b010
#define REG_D   0b011
#define REG_E   0b100
#define REG_F   0b101
#define REG_S   0b110
#define REG_BP  0b111

#define RM8     0b00
#define RM16    0b01
#define RM32    0b10

void cpu_initialize();
void cpu_set_memory(uint8_t* memory, uint64_t size);
void cpu_execute();
void cpu_output();