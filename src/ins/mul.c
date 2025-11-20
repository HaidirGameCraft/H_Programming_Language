#include <inst.h>
#include "mul.h"

SIGN_INSTRUCTION_SET(mul) {
    if( opcode == OPCODE_MUL_1 )
        instruction_mul_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_MUL_2 )
        instruction_mul_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_mul_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32(reg, __regmem.sreg );
        dr32 = reg32( reg, __regmem.dreg );
        *dr32 *= *r32;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.sreg );
        dr16 = reg16( reg, __regmem.dreg );
        *dr16 *= *r16;
    } else {
        r8 = reg8(reg, __regmem.sreg );
        dr8 = reg8( reg, __regmem.dreg );
        *dr8 *= *r8;
    }
}

void instruction_mul_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    if( prefix & PREFIX_32BITS )
    {
        value = read32( memory, pc );
        dr32 = reg32( reg, __regmem.dreg );
        *dr32 *= value;
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
        dr16 = reg16( reg, __regmem.dreg );
        *dr16 *= value;
    } else {
        value = read8( memory, pc );
        dr8 = reg8( reg, __regmem.dreg );
        *dr8 *= value;
    }
}