#include "not.h"

uint8_t instruction_not_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);
    
    sreg = getreg( reg, __register.sreg );
    *sreg = !(*sreg);

    return 1;
}

SIGN_INSTRUCTION_SET(not) {
    uint8_t status = 0;
    if( opcode == OPCODE_NOT )
        status = instruction_not_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    return status;
}
