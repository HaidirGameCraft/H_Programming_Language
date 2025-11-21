#include "call.h"

SIGN_INSTRUCTION_SET(call) {
    if( opcode == OPCODE_CALL_1 )
        instruction_call_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_CALL_2 )
        instruction_call_value( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_call_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg(reg, __register.sreg);

    reg->rs -= 4;
    write32( memory, reg->rs, reg->pc );
    reg->pc = *sreg;
}

void instruction_call_regmem(INSTRUCTION_SET_ARGS) {
    instruction_call_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_call_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION

    value = read32(memory, pc);
    reg->rs -= 4;
    write32( memory, reg->rs, reg->pc );
    reg->pc = value;
}