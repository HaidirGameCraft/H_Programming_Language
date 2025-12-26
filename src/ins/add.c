#include <inst.h>
#include "inst.h"
#include <stdio.h>
#include <stdlib.h>
#include "add.h"

__instruction_opcode_reg_reg(add);
__instruction_opcode_value_reg(add);

SIGN_INSTRUCTION_SET(add) {
    uint8_t status = 0; 
    if( opcode == OPCODE_ADD_1 )
        status |= call_instruction_opcode_reg_reg(add);
    else if ( opcode == OPCODE_ADD_2 )
        status |= call_instruction_opcode_value_reg(add);
    return status;
}

__instruction_opcode_reg_reg(add) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    *dreg += *sreg;

    return 1;
}

__instruction_opcode_value_reg(add) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg(reg, __register.dreg );
    value = read32(memory, pc);
    *dreg += value;

    return 1;
}
