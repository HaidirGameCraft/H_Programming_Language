/**
 * cpu.c
 * Created By Haidir
 */
#include <string.h>
#include <memory.h>
#include <cpu.h>
#include <inst.h>
#include <stdio.h>

cpu_register_t reg;
uint8_t* memory;
uint64_t size_memory = 0;
uint8_t is_program_running = 1;
void cpu_initialize() {
    memset( &reg, 0, sizeof( cpu_register_t ) );
}

void cpu_set_memory(uint8_t* __memory, uint64_t size) {
    memory = __memory;
    size_memory = size;
}
void cpu_execute() {
    while( reg.pc32 < size_memory && is_program_running )
    {
        uint8_t prefix = read8(memory, &reg.pc32);
        uint8_t opcode = read8(memory, &reg.pc32);
#define ARGS_INSTRUCTION_SET prefix, opcode, memory, &reg, &reg.pc32
        mov_instruction_set(ARGS_INSTRUCTION_SET);
        add_instruction_set(ARGS_INSTRUCTION_SET);
        and_instruction_set(ARGS_INSTRUCTION_SET);
        sub_instruction_set(ARGS_INSTRUCTION_SET);
        mul_instruction_set(ARGS_INSTRUCTION_SET);
        div_instruction_set(ARGS_INSTRUCTION_SET);
        push_instruction_set(ARGS_INSTRUCTION_SET);
        pop_instruction_set(ARGS_INSTRUCTION_SET);
        or_instruction_set(ARGS_INSTRUCTION_SET);
        nand_instruction_set(ARGS_INSTRUCTION_SET);
        nor_instruction_set(ARGS_INSTRUCTION_SET);
        xor_instruction_set(ARGS_INSTRUCTION_SET);
        stp_instruction_set(ARGS_INSTRUCTION_SET);
        call_instruction_set(ARGS_INSTRUCTION_SET);
        ret_instruction_set(ARGS_INSTRUCTION_SET);
        go_instruction_set(ARGS_INSTRUCTION_SET);
    }
}
void cpu_output() {
    printf("=============== OUTPUT ================\n");
    printf("R8A: %2x, R16A: %4x, R32A: %8x\n", reg.r8a, reg.r16a, reg.r32a);
    printf("R8B: %2x, R16B: %4x, R32B: %8x\n", reg.r8b, reg.r16b, reg.r32b);
    printf("R8C: %2x, R16C: %4x, R32C: %8x\n", reg.r8c, reg.r16c, reg.r32c);
    printf("R8D: %2x, R16D: %4x, R32D: %8x\n", reg.r8d, reg.r16d, reg.r32d);
    printf("R8E: %2x, R16E: %4x, R32E: %8x\n", reg.r8e, reg.r16e, reg.r32e);
    printf("R8F: %2x, R16F: %4x, R32F: %8x\n", reg.r8f, reg.r16f, reg.r32f);
    printf("R8S: %2x, R16S: %4x, R32S: %8x\n", reg.r8s, reg.r16s, reg.r32s);
    printf("R8BP: %2x, R16BP: %4x, R32BP: %8x\n", reg.r8bp, reg.r16bp, reg.r32bp);
    printf("\n");
    printf("FLAGS: %4x, PC32: %8x", reg.flags, reg.pc32);
}