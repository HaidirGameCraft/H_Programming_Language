#include "cnd.h"
#include <inst.h>
#include <stdio.h>

SIGN_INSTRUCTION_SET(cnd) {
    if( opcode == OPCODE_CND_1 )
        instruction_cnd_reg_sym_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_CND_2 )
        instruction_cnd_value_sym_reg( prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_CND_3 )
        instruction_cnd_reg_sym_value( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_cnd_reg_sym_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION;
    REGMEM_DEFINED(memory, pc);

    uint8_t sym_cnd = read8( memory, pc );
    uint32_t cmp1 = 0;
    uint32_t cmp2 = 0;
    
    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    cmp1 = *sreg;
    cmp2 = *dreg;

    if ( sym_cnd == SYM_EQUAL )
        reg->flags |= ( cmp1 == cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS )
        reg->flags |= ( cmp1 < cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER )
        reg->flags |= ( cmp1 > cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS_EQUAL )
        reg->flags |= ( cmp1 <= cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER_EQUAL )
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
    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );

    cmp1 = *dreg;
    cmp2 = value;

    if ( sym_cnd == SYM_EQUAL )
        reg->flags = ( cmp1 == cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS )
        reg->flags = ( cmp1 < cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER )
        reg->flags = ( cmp1 > cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS_EQUAL )
        reg->flags = ( cmp1 <= cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER_EQUAL )
        reg->flags = ( cmp1 >= cmp2 ) << 2;
}

void instruction_cnd_reg_sym_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION;
    REGMEM_DEFINED(memory, pc);

    uint8_t sym_cnd = read8( memory, pc );
    uint32_t cmp1 = 0;
    uint32_t cmp2 = 0;
    sreg = getreg( reg, __register.sreg );
    value = read32( memory, pc );

    cmp1 = *sreg;
    cmp2 = value;

    if ( sym_cnd == SYM_EQUAL )
        reg->flags = ( cmp1 == cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS )
        reg->flags = ( cmp1 < cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER )
        reg->flags = ( cmp1 > cmp2 ) << 2;
    else if ( sym_cnd == SYM_LESS_EQUAL )
        reg->flags = ( cmp1 <= cmp2 ) << 2;
    else if ( sym_cnd == SYM_GREATER_EQUAL )
        reg->flags = ( cmp1 >= cmp2 ) << 2;
}