#include "or.h"

SIGN_INSTRUCTION_SET(or) {
    if( opcode == OPCODE_OR_1 )
        instruction_or_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_OR_2 )
        instruction_or_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
}

void instruction_or_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = *sreg | *dreg;
}

void instruction_or_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );
    *dreg = value | *dreg;
}