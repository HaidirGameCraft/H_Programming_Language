#include <inst.h>
#include "div.h"

SIGN_INSTRUCTION_SET(div) {
    if( opcode == OPCODE_DIV_1 )
        instruction_div_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_DIV_2 )
        instruction_div_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
}


void instruction_div_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    *dreg = (uint32_t)( *dreg / *sreg );
    // remaining value will save on RI register
    reg->ri = (uint8_t)( *dreg % *sreg );
}

void instruction_div_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    value = read32( memory, pc );
    dreg = getreg( reg, __register.dreg );
    *dreg = (uint32_t)( *dreg / value );
    // remaining value will save on RI register
    reg->ri = (uint8_t)( *dreg % value );
}