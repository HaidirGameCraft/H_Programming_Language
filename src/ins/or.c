#include "or.h"

uint8_t instruction_or_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = *sreg | *dreg;
    return 1;
}

uint8_t instruction_or_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );
    *dreg = value | *dreg;
    return 1;
}

SIGN_INSTRUCTION_SET(or) {
    if( opcode == OPCODE_OR_1 )
        return instruction_or_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_OR_2 )
        return instruction_or_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    return 0;
}

