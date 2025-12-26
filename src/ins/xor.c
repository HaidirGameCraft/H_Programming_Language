#include "xor.h"

uint8_t instruction_xor_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    *dreg = *dreg ^ *sreg;
    return 1;
}

uint8_t instruction_xor_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg(reg, __register.dreg);
    value = read32(memory, pc);
    *dreg = *dreg ^ value;
    return 1;
}
SIGN_INSTRUCTION_SET(xor) {
    if( opcode == OPCODE_XOR_1 )
        return instruction_xor_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_XOR_2 )
        return  instruction_xor_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    return 0;
}

