#include "call.h"

SIGN_INSTRUCTION_SET(call) {
    if( opcode == OPCODE_CALL_1 )
        instruction_call_reg(prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_CALL_2 )
        instruction_call_value( prefix, opcode, memory, reg, pc );
}

uint32_t create_instruction_call_reg(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("call", opcode) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_CALL_1;
    uint8_t reg = regbytext(lReg);

    return instruction_reg(memory, pc, prefix, op, reg);
}

uint32_t create_instruction_call_regmem(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("call", opcode) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_CALL_1;
    uint8_t reg = regbytext( lReg );
    return instruction_regmem(memory, pc, prefix,  op, reg, *offset);
}

uint32_t create_instruction_call_value(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("call", opcode) != 0 ) return  pc;

    uint8_t prefix = PREFIX_32BITS;
    uint8_t op = OPCODE_CALL_2;
    return instruction_value(memory, pc, prefix, op, *value );
}

uint32_t create_instruction_call_label(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("call", opcode) != 0 ) return pc;

    uint8_t prefix = PREFIX_32BITS;
    uint8_t op = OPCODE_CALL_2;
    return instruction_label(memory, pc, prefix, op);
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
    instruction_call_reg(prefix, opcode, memory, reg, pc );
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