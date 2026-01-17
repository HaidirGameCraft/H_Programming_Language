/**
 * cpu.h
 * Created By Haidir
 */
#pragma once

#include <stdint.h>

#define flags_condition 1 << 2

#define FLAGS_CONDITION 1 << 2

typedef struct {
    uint32_t ra;
    uint32_t rb;
    uint32_t rc;
    uint32_t rd;
    uint32_t re;
    uint32_t rf;
    uint32_t rg;
    uint32_t rh;
    uint32_t ri;
    uint32_t rj;
    uint32_t rk;
    uint32_t rl;
    uint32_t rm;
    uint32_t rn;
    uint32_t rs;
    uint32_t rp;

    uint32_t pc;
    uint16_t flags;
} cpu_register_t;

#define REG_A   0b0000
#define REG_B   0b0001
#define REG_C   0b0010
#define REG_D   0b0011
#define REG_E   0b0100
#define REG_F   0b0101
#define REG_G   0b0110
#define REG_H   0b0111
#define REG_I   0b1000
#define REG_J   0b1001
#define REG_K   0b1010
#define REG_L   0b1011
#define REG_M   0b1100
#define REG_N   0b1101
#define REG_S   0b1110
#define REG_P   0b1111

#ifdef __cplusplus
#define EXTR extern "C"
#else
#define EXTR extern
#endif
EXTR void cpu_initialize();
EXTR uint8_t* get_memory();
EXTR int get_memory_size();
EXTR void cpu_set_memory(uint8_t* memory, uint64_t size);
EXTR void cpu_execute(uint8_t exec_one );
EXTR void cpu_output();
