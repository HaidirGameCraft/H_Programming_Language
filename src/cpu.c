/**
 * cpu.c
 * Created By Haidir
 */
#include "interrupt.h"
#include <string.h>
#include <memory.h>
#include <cpu.h>
#include <inst.h>
#include <ins/inst.h>
#include <stdio.h>

cpu_register_t reg;
uint8_t* memory;
uint64_t size_memory = 0;
uint8_t is_program_running = 1;
void cpu_initialize() {
    memset( &reg, 0, sizeof( cpu_register_t ) );
}

uint8_t* get_memory() {
    return memory;
}
int get_memory_size() {
    return size_memory;
}

void cpu_set_memory(uint8_t* __memory, uint64_t size) {
    memory = __memory;
    size_memory = size;
}
void cpu_execute(uint8_t exec_one) {
    while( reg.pc < size_memory && is_program_running )
    {
        uint8_t prefix = read8(memory, &reg.pc);
        uint8_t ext_prefix = 0;
        if( prefix & PREFIX_EXT_INC )
            ext_prefix = read8( memory, &reg.pc );

        uint8_t opcode = read8(memory, &reg.pc);
        uint8_t status = 0;
#define ARGS_INSTRUCTION_SET prefix, ext_prefix, opcode, memory, &reg, &reg.pc
        status |= mov_instruction_set(ARGS_INSTRUCTION_SET);
        status |= add_instruction_set(ARGS_INSTRUCTION_SET);
        status |= and_instruction_set(ARGS_INSTRUCTION_SET);
        status |= sub_instruction_set(ARGS_INSTRUCTION_SET);
        status |= mul_instruction_set(ARGS_INSTRUCTION_SET);
        status |= div_instruction_set(ARGS_INSTRUCTION_SET);
        status |= push_instruction_set(ARGS_INSTRUCTION_SET);
        status |= pop_instruction_set(ARGS_INSTRUCTION_SET);
        status |= or_instruction_set(ARGS_INSTRUCTION_SET);
        status |= nand_instruction_set(ARGS_INSTRUCTION_SET);
        status |= nor_instruction_set(ARGS_INSTRUCTION_SET);
        status |= xor_instruction_set(ARGS_INSTRUCTION_SET);
        status |= stp_instruction_set(ARGS_INSTRUCTION_SET);
        status |= call_instruction_set(ARGS_INSTRUCTION_SET);
        status |= ret_instruction_set(ARGS_INSTRUCTION_SET);
        status |= go_instruction_set(ARGS_INSTRUCTION_SET);
        status |= not_instruction_set(ARGS_INSTRUCTION_SET);
        status |= cnd_instruction_set(ARGS_INSTRUCTION_SET);
        status |= lod_instruction_set(ARGS_INSTRUCTION_SET);
        status |= str_instruction_set(ARGS_INSTRUCTION_SET);
        status |= pnt_instruction_set(ARGS_INSTRUCTION_SET);
        status |= pct_instruction_set(ARGS_INSTRUCTION_SET);
        status |= sh_instruction_set(ARGS_INSTRUCTION_SET);
        status |= in_instruction_set(ARGS_INSTRUCTION_SET);
        status |= int_instruction_set(ARGS_INSTRUCTION_SET);

        if( status == 0 )
            CallInterrupt(UnknownOpcodeExceptionInt);

        if( exec_one )
            break;
    }
}
void cpu_output() {
    printf("=============== OUTPUT ================\n");
    printf("RA: %.08x ( %i )\n", reg.ra, reg.ra );
    printf("RB: %.08x ( %i )\n", reg.rb, reg.rb );
    printf("RC: %.08x ( %i )\n", reg.rc, reg.rc );
    printf("RD: %.08x ( %i )\n", reg.rd, reg.rd );
    printf("RE: %.08x ( %i )\n", reg.re, reg.re );
    printf("RF: %.08x ( %i )\n", reg.rf, reg.rf );
    printf("RG: %.08x ( %i )\n", reg.rg, reg.rg );
    printf("RH: %.08x ( %i )\n", reg.rh, reg.rh );
    printf("RI: %.08x ( %i )\n", reg.ri, reg.ri );
    printf("RJ: %.08x ( %i )\n", reg.rj, reg.rj );
    printf("RK: %.08x ( %i )\n", reg.rk, reg.rk );
    printf("RL: %.08x ( %i )\n", reg.rl, reg.rl );
    printf("RM: %.08x ( %i )\n", reg.rm, reg.rm );
    printf("RN: %.08x ( %i )\n", reg.rn, reg.rn );
    printf("RS: %.08x ( %i )\n", reg.rs, reg.rs );
    printf("RP: %.08x ( %i )\n", reg.rp, reg.rp );
    printf("\n");
    printf("FLAGS: %.016b (%4x), PC: %.08x", reg.flags, reg.flags, reg.pc);
}
