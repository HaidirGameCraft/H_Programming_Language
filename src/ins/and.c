#include <inst.h>
#include "and.h"
#include "ins/inst.h"
#include <stdio.h>

__instruction_opcode_reg_reg(and);
__instruction_opcode_value_reg(and);

SIGN_INSTRUCTION_SET(and) {
    uint8_t status = 0;
    if( opcode == OPCODE_AND_1 )
        status = call_instruction_opcode_reg_reg(and);
    else if ( opcode == OPCODE_AND_2 )
        status = call_instruction_opcode_value_reg(and);
    return status;
}

__instruction_opcode_reg_reg(and) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    *dreg = *dreg & *sreg;

    return 1;
}

__instruction_opcode_value_reg(and) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg(reg, __register.dreg);
    value = read32(memory, pc);
    *dreg = *dreg & value;

    return 1;
}
