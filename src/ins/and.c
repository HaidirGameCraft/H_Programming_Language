#include <inst.h>
#include "and.h"
#include <stdio.h>

SIGN_INSTRUCTION_SET(and) {
    if( opcode == OPCODE_AND_1 )
        instruction_and_regmem_reg(prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_AND_2 )
        instruction_and_value_reg(prefix, opcode, memory, reg, pc);
}

void instruction_and_regmem_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION

    memset( &__regmem, read8( memory, pc ), sizeof( __regmem ) );
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

        *dr32 = *dr32 & value;
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

        *dr16 = *dr16 & value;
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

        *dr8 = *dr8 & value;
    }
}
void instruction_and_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset( &__regmem, read8( memory, pc ), sizeof( __regmem ) );
    if( prefix & PREFIX_32BITS )
    {
        value = read32( memory, pc );
        dr32 = reg32( reg, __regmem.dreg );

        *dr32 = *dr32 & value;
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
        dr16 = reg16( reg, __regmem.dreg );

        *dr16 = *dr16 & value;
    } else {
        value = read8( memory, pc );
        dr8 = reg8( reg, __regmem.dreg  );

        *dr8 = *dr8 & value;
    }
}

uint32_t create_instruction_and_regmem_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("and", opcode, 3) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_AND_1;
    uint8_t sreg = regbytext(lReg);
    uint8_t dreg = regbytext(rReg);
    
    if( offset != NULL )
        return instruction_regmem_reg(memory, pc, prefix, op, sreg, dreg, *offset);
    else
        return instruction_reg_reg(memory, pc, prefix, op, sreg, dreg);
;}

uint32_t create_instruction_and_value_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("and", opcode, 3) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_AND_2;
    uint8_t dreg = regbytext(rReg);
    return instruction_value_reg( memory,  pc, prefix, op, dreg, *value );
}