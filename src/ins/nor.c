#include "nor.h"
uint8_t instruction_nor_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = !(*sreg | *dreg);

    return 1;
}

uint8_t instruction_nor_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );
    *dreg = !(value & *dreg);

    return 1;
}

SIGN_INSTRUCTION_SET(nor) {
    uint8_t status = 0;
    if( opcode == OPCODE_NOR_1 )
        status = instruction_nor_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_NOR_2 )
        status = instruction_nor_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    return status;
}

