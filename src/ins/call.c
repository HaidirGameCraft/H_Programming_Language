#include "call.h"
#include "ins/inst.h"

__instruction_opcode_reg(call);
__instruction_opcode_value(call);

SIGN_INSTRUCTION_SET(call) {
    uint8_t status = 0;
    if( opcode == OPCODE_CALL_1 )
        status = call_instruction_opcode_reg(call);
    else if ( opcode == OPCODE_CALL_2 )
        status = call_instruction_opcode_value(call);

    return status;
}

__instruction_opcode_reg(call) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg(reg, __register.sreg);

    reg->rs -= 4;
    write32( memory, reg->rs, reg->pc );
    reg->pc = *sreg;
    return 1;
}

__instruction_opcode_value(call) {
    VARIABLE_INSTRUCTION

    value = read32(memory, pc);
    reg->rs -= 4;
    write32( memory, reg->rs, reg->pc );
    reg->pc = value;

    return 1;
}
