#include "ret.h"

SIGN_INSTRUCTION_SET(ret) {
    if( opcode == OPCODE_RETURN )
        instruction_ret(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_ret(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    value = gread32(memory, reg->rs);
    reg->rs += 4;
    reg->pc = value;
}
