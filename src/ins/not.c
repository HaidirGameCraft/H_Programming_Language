#include "not.h"

uint32_t create_instruction_not_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("not", opcode, 3) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_NOT;
    uint8_t dreg = regbytext(lReg);
    
    return instruction_reg(memory, pc, prefix, op, dreg);
}

void instruction_not_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8(memory, pc ), 1 );
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
        instruction_not_reg(prefix, opcode, memory, reg, pc );
}