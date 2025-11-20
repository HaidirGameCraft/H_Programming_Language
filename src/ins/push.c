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

    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32(reg, __regmem.dreg );
        value = *r32;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.dreg );
        value = *r16;
    } else {
        r8 = reg8(reg, __regmem.dreg );
        value = *r8;
    }

    reg->r32s -= 4;
    write32(memory, reg->r32s, value);
}

void instruction_push_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    if( prefix & PREFIX_32BITS )
    {
        value = read32(memory, pc);
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16(memory, pc);
    } else {
        value = read8(memory, pc);
    }

    reg->r32s -= 4;
    write32(memory, reg->r32s, value);
}