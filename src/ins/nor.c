#include "nor.h"

SIGN_INSTRUCTION_SET(nor) {
    if( opcode == OPCODE_NOR_1 )
        instruction_nor_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_NOR_2 )
        instruction_nor_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
}

void instruction_nor_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = !(*sreg | *dreg);
}

void instruction_nor_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );
    *dreg = !(value & *dreg);
}