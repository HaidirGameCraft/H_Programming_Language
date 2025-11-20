#include <inst.h>
#include "div.h"

SIGN_INSTRUCTION_SET(div) {
    if( opcode == OPCODE_DIV_1 )
        instruction_div_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_DIV_2 )
        instruction_div_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
}


void instruction_div_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32(reg, __regmem.sreg );
        dr32 = reg32( reg, __regmem.dreg );
        *dr32 = (uint32_t)(*dr32 / *r32);
        reg->r8e = (uint8_t)(*dr32 % *r32);
        
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.sreg );
        dr16 = reg16( reg, __regmem.dreg );
        *dr16 = (uint16_t)(*dr16 / *r16);
        reg->r8e = (uint8_t)(*dr16 % *r16);
    } else {
        r8 = reg8(reg, __regmem.sreg );
        dr8 = reg8( reg, __regmem.dreg );
        *dr8 = (uint8_t)(*dr8 / *r8);
        reg->r8e = (uint8_t)(*dr8 % *r8);
    }
}

void instruction_div_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    if( prefix & PREFIX_32BITS )
    {
        value = read32( memory, pc );
        dr32 = reg32( reg, __regmem.dreg );
        *dr32 = (uint32_t)(*dr32 / value);
        reg->r8e = (uint8_t)(*dr32 % value);
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
        dr16 = reg16( reg, __regmem.dreg );
        *dr16 = (uint16_t)(*dr16 / value);
        reg->r8e = (uint8_t)(*dr16 % value);
    } else {
        value = read8( memory, pc );
        dr8 = reg8( reg, __regmem.dreg );
        *dr8 = (uint8_t)(*dr8 / value);
        reg->r8e = (uint8_t)(*dr8 % value);
    }
}