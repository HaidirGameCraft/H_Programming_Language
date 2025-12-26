#include <inst.h>
#include <stdio.h>
#include "sub.h"
uint8_t instruction_sub_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    *dreg -= *sreg;
    return 1;
}

uint8_t instruction_sub_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg(reg, __register.dreg );
    value = read32(memory, pc);
    *dreg -= value;
    return 1;
}

SIGN_INSTRUCTION_SET(sub) {
    if( opcode == OPCODE_SUB_1 )
        return instruction_sub_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_SUB_2 )
        return instruction_sub_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

