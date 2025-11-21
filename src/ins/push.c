#include <inst.h>
#include "push.h"

#include <stdio.h>

SIGN_INSTRUCTION_SET(push) {
    if( opcode == OPCODE_PUSH_1 )
        instruction_push_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_PUSH_2 )
        instruction_push_value(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_push_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    reg->rs -= 4;
    write32(memory, reg->rs, *sreg);
}

void instruction_push_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    value = read32(memory, pc);
    reg->rs -= 4;
    write32(memory, reg->rs, value);
}