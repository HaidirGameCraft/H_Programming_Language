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
void cpu_execute(uint8_t exec_one) {
    while( reg.pc < size_memory && is_program_running )
    {
        uint8_t prefix = read8(memory, &reg.pc);
        uint8_t ext_prefix = 0;
        if( prefix & PREFIX_EXT_INC )
            ext_prefix = read8( memory, &reg.pc );

        uint8_t opcode = read8(memory, &reg.pc);
#define ARGS_INSTRUCTION_SET prefix, ext_prefix, opcode, memory, &reg, &reg.pc
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
        not_instruction_set(ARGS_INSTRUCTION_SET);
        cnd_instruction_set(ARGS_INSTRUCTION_SET);
        lod_instruction_set(ARGS_INSTRUCTION_SET);
        str_instruction_set(ARGS_INSTRUCTION_SET);
        pnt_instruction_set(ARGS_INSTRUCTION_SET);
        pct_instruction_set(ARGS_INSTRUCTION_SET);
        sh_instruction_set(ARGS_INSTRUCTION_SET);
        
        if( exec_one )
            break;
    }
}
void cpu_output() {
    printf("=============== OUTPUT ================\n");
    printf("RA: %8x\n", reg.ra);
    printf("RB: %8x\n", reg.rb);
    printf("RC: %8x\n", reg.rc);
    printf("RD: %8x\n", reg.rd);
    printf("RE: %8x\n", reg.re);
    printf("RF: %8x\n", reg.rf);
    printf("RG: %8x\n", reg.rg);
    printf("RH: %8x\n", reg.rh);
    printf("RI: %8x\n", reg.ri);
    printf("RJ: %8x\n", reg.rj);
    printf("RK: %8x\n", reg.rk);
    printf("RL: %8x\n", reg.rl);
    printf("RM: %8x\n", reg.rm);
    printf("RN: %8x\n", reg.rn);
    printf("RS: %8x\n", reg.rs);
    printf("RP: %8x\n", reg.rp);
    printf("\n");
    printf("FLAGS: %16b (%4x), PC: %8x", reg.flags, reg.flags, reg.pc);
}
