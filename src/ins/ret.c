#include "ret.h"

uint8_t instruction_ret(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    value = gread32(memory, reg->rs);
    reg->rs += 4;
    reg->pc = value;
    return 1;
}
SIGN_INSTRUCTION_SET(ret) {
    if( opcode == OPCODE_RETURN )
        return instruction_ret(prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

