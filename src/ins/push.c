#include <inst.h>
#include "push.h"

#include <stdio.h>

SIGN_INSTRUCTION_SET(push) {
    if( opcode == OPCODE_PUSH_1 )
        instruction_push_regmem(prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_PUSH_2 )
        instruction_push_value(prefix, opcode, memory, reg, pc );
}

void instruction_push_regmem(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8(memory, pc), 1);
    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32( reg, __regmem.sreg );
        if( pre.m )
        {
            offset = read32( memory, pc );
            value = gread32( memory, *r32 + offset );
        } else {
            value = *r32;
        }

        reg->r32s -= 4;
        write32( memory,  reg->r32s, value );
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16( reg, __regmem.sreg );
        if( pre.m )
        {
            offset = read16( memory, pc );
            value = gread16( memory, *r16 + offset );
        } else {
            value = *r16;
        }

        reg->r32s -= 4;
        write16( memory,  reg->r32s, value );
    } else {
        r8 = reg8( reg, __regmem.sreg );
        if( pre.m )
        {
            offset = read8( memory, pc );
            value = gread8( memory, *r32 + offset );
        } else {
            value = *r8;
        }

        reg->r32s -= 4;
        write32( memory,  reg->r32s, value );
    }
}
void instruction_push_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    if( prefix & PREFIX_32BITS )
    {
        value = read32(memory, pc);
        reg->r32s -= 4;
        write32( memory,  reg->r32s, value );
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
        reg->r32s -= 4;
        write32( memory,  reg->r32s, value );
    } else {
        value = read8(memory, pc);
        reg->r32s -= 4;
        write32( memory,  reg->r32s, value );
    }
}

uint32_t create_instruction_push_regmem(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("push", opcode, 4 ) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_PUSH_1;
    uint8_t lreg = regbytext( lReg );
    
    if( offset != NULL )
        return instruction_regmem(memory, pc, prefix, op, lreg, *offset);
    else
        return instruction_reg(memory, pc, prefix, op, lreg);
}

uint32_t create_instruction_push_value(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("push", opcode, 4 ) != 0 )
        return pc;

    uint8_t prefix = 0;
    uint8_t op = OPCODE_PUSH_2;
    return instruction_value(memory, pc,  prefix, op, *value);
}