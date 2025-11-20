#include "call.h"

SIGN_INSTRUCTION_SET(call) {
    if( opcode == OPCODE_CALL_1 )
        instruction_call_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_CALL_2 )
        instruction_call_value( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_call_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8(memory, pc), 1);
    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read32( memory, pc );
            value = gread32( memory, *r32 + offset );
        } else {
            value = *r32;
        }

        reg->r32s -= 4;
        write32( memory, reg->r32s, reg->pc32 );
        reg->pc32 = value;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read16( memory, pc );
            value = gread16( memory, *r16 + offset );
        } else {
            value = *r16;
        }

        reg->r32s -= 4;
        write32( memory,  reg->r32s, reg->pc32 );
        reg->pc32 = value;
    } else {
        r8 = reg8( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read8( memory, pc );
            value = gread8( memory, *r32 + offset );
        } else {
            value = *r8;
        }

        reg->r32s -= 4;
        write32( memory,  reg->r32s, reg->pc32 );
        reg->pc32 = value;
    }
}

void instruction_call_regmem(INSTRUCTION_SET_ARGS) {
    instruction_call_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_call_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION

    if( prefix & PREFIX_32BITS )
    {
        value = read32(memory, pc);
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
    } else {
        value = read8(memory, pc);
    }

    reg->r32s -= 4;
    write32( memory,  reg->r32s, reg->pc32 );
    reg->pc32 = value;
}