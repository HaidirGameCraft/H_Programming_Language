#include <inst.h>
#include "mul.h"

SIGN_INSTRUCTION_SET(mul) {
    if( opcode == OPCODE_MUL_1 )
        instruction_mul_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_MUL_2 )
        instruction_mul_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_mul_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = *dreg * *sreg;
}

void instruction_mul_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );

    *dreg = *dreg * value;
}