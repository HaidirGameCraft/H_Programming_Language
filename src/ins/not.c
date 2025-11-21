#include "not.h"

void instruction_not_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);
    
    sreg = getreg( reg, __register.sreg );
    *sreg = !(*sreg);
}

SIGN_INSTRUCTION_SET(not) {
    if( opcode == OPCODE_NOT )
        instruction_not_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}