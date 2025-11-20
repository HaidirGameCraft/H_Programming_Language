#include "cnd.h"
#include <inst.h>
#include <stdio.h>

SIGN_INSTRUCTION_SET(cnd) {
    if( opcode == OPCODE_CND_1 )
        instruction_cnd_reg_sym_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_CND_2 )
        instruction_cnd_value_sym_reg( prefix, ext_prefix, opcode, memory, reg, pc);
}

void instruction_cnd_reg_sym_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION;
    REGMEM_DEFINED(memory, pc);

    uint8_t sym_cnd = read8( memory, pc );
    uint32_t cmp1 = 0;
    uint32_t cmp2 = 0;
    if( prefix & PREFIX_32BITS )
    {
        dr32 = reg32( reg, __regmem.dreg );
        r32 = reg32( reg, __regmem.sreg );
        cmp1 = *r32;
        cmp2 = *dr32;
    } else if ( prefix & PREFIX_16BITS )
    {
        dr16 = reg16( reg, __regmem.dreg );
        r16 = reg16( reg, __regmem.sreg );
        cmp1 = *r16;
        cmp2 = *dr16;
    } else {
        dr8 = reg8( reg, __regmem.dreg );
        r8 = reg8( reg, __regmem.sreg ); 
        cmp1 = *r8;
        cmp2 = *dr8;
    }

    if ( sym_cnd == SYM_CND_EQUAL )
        reg->flags |= ( cmp1 == cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_LESS )
        reg->flags |= ( cmp1 < cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_GREATER )
        reg->flags |= ( cmp1 > cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_LESS_EQUAL )
        reg->flags |= ( cmp1 <= cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_GREATER_EQUAL )
        reg->flags |= ( cmp1 >= cmp2 ) << 2;

    // printf("Cmp 1: %x, Cmp 2: %x\n", cmp1, cmp2);
    // printf("Flags: %i\n", reg->flags);
}

void instruction_cnd_value_sym_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION;
    REGMEM_DEFINED(memory, pc);

    uint8_t sym_cnd = read8( memory, pc );
    uint32_t cmp1 = 0;
    uint32_t cmp2 = 0;
    if( prefix & PREFIX_32BITS )
    {
        dr32 = reg32( reg, __regmem.dreg );
        cmp1 = *r32;
        cmp2 = read32(memory, pc);
    } else if ( prefix & PREFIX_16BITS )
    {
        dr16 = reg16( reg, __regmem.dreg );
        cmp1 = *r16;
        cmp2 = read16(memory, pc);
    } else {
        dr8 = reg8( reg, __regmem.dreg );
        cmp1 = *r8;
        cmp2 = read8(memory, pc);
    }

    if ( sym_cnd == SYM_CND_EQUAL )
        reg->flags = ( cmp1 == cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_LESS )
        reg->flags = ( cmp1 < cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_GREATER )
        reg->flags = ( cmp1 > cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_LESS_EQUAL )
        reg->flags = ( cmp1 <= cmp2 ) << 2;
    else if ( sym_cnd == SYM_CND_GREATER_EQUAL )
        reg->flags = ( cmp1 >= cmp2 ) << 2;
}