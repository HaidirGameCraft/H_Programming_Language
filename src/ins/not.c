#include "not.h"

void instruction_not_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);
    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32(reg, __regmem.dreg );
        *r32 = ~*r32;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.dreg );
        *r16 = ~*r16;
    } else {
        r8 = reg8(reg, __regmem.dreg );
        *r8 = ~*r8;
    }
}

SIGN_INSTRUCTION_SET(not) {
    if( opcode == OPCODE_NOT )
        instruction_not_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}