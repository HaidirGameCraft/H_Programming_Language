#include <inst.h>
#include "div.h"

SIGN_INSTRUCTION_SET(div) {
    if( opcode == OPCODE_DIV_1 )
        instruction_div_regmem_reg( prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_DIV_2 )
        instruction_div_value_reg( prefix, opcode, memory, reg, pc );
}


void instruction_div_regmem_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8(memory, pc), 1 );

    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32( reg, __regmem.sreg );
        dr32 = reg32( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read32( memory, pc );
            value = gread32( memory, *r32 + offset );
        } else {
            value = *r32;
        }

        uint32_t tmp = *dr32;
        *dr32 = (uint32_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16( reg, __regmem.sreg );
        dr16 = reg16( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read16( memory, pc );
            value = gread16( memory, *r16 + offset );
        } else {
            value = *r16;
        }

        uint32_t tmp = *dr16;
        *dr16 = (uint16_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
    } else {
        r8 = reg8( reg, __regmem.sreg );
        dr8 = reg8( reg, __regmem.dreg );
        if( pre.m )
        {
            offset = read8( memory, pc );
            value = gread8( memory, *r8 + offset );
        } else {
            value = *r8;
        }

        uint32_t tmp = *dr8;
        *dr8 = (uint8_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
    }
}

void instruction_div_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8(memory, pc), 1 );

    if( prefix & PREFIX_32BITS )
    {
        dr32 = reg32( reg, __regmem.dreg );
        value = read32( memory, pc );

        uint32_t tmp = *dr32;
        *dr32 = (uint32_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
        
    } else if ( prefix & PREFIX_16BITS )
    {
        dr16 = reg16( reg, __regmem.dreg );
        value = read16( memory, pc );

        uint32_t tmp = *dr16;
        *dr16 = (uint16_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
    } else {
        dr8 = reg8( reg, __regmem.dreg );
        value = read8( memory, pc );

        uint32_t tmp = *dr8;
        *dr8 = (uint8_t)( tmp / value );
        reg->r8e = (uint8_t)( tmp % value );
    }
}

uint32_t create_instruction_div_regmem_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("div", opcode, 3) != 0 )
        return pc;
    
    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_DIV_1;
    uint8_t lreg = regbytext( lReg );
    uint8_t rreg = regbytext( rReg );
    if( offset != NULL )
        return instruction_regmem_reg(memory, pc, prefix, op, lreg, rreg, *offset);
    else
        return instruction_reg_reg(memory, pc, prefix, op,  lreg, rreg);
}

uint32_t create_instruction_div_value_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("div", opcode, 3) != 0 )
        return pc;
    
    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_DIV_2;
    uint8_t rreg = regbytext( rReg );
    return instruction_value_reg(memory, pc, prefix, op, rreg, *value );
}