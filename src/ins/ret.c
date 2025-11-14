#include "ret.h"

SIGN_INSTRUCTION_SET(ret) {
    if( opcode == OPCODE_RETURN )
        instruction_ret(prefix, opcode, memory, reg, pc );
}

uint32_t create_instruction_ret(CREATE_INSTRUCTION_ARGS) {
    if( strcmp(opcode, "ret") != 0 )
        return pc;

    uint8_t prefix = 0;
    uint8_t op = OPCODE_RETURN;
    write8(memory, pc, prefix); pc++;
    write8(memory, pc, op); pc++;
    return pc;
}

void instruction_ret(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    value = gread32(memory, reg->r32s);
    reg->r32s += 4;
    reg->pc32 = value;
}
