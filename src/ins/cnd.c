#include "cnd.h"
#include "ins/inst.h"
#include <inst.h>
#include <stdio.h>

__instruction_opcode_reg_reg(cnd);
__instruction_opcode_value_reg(cnd);
__instruction_opcode_reg_value(cnd);

SIGN_INSTRUCTION_SET(cnd) {
    uint8_t status = 0;
    if( opcode == OPCODE_CND_1 )
        status = call_instruction_opcode_reg_reg(cnd);
    else if ( opcode == OPCODE_CND_2 )
        status = call_instruction_opcode_value_reg(cnd);
    else if ( opcode == OPCODE_CND_3 )
        status = call_instruction_opcode_reg_value(cnd);

    return status;
}

__instruction_opcode_reg_reg(cnd) {
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
    
    return 1;
}

__instruction_opcode_value_reg(cnd) {
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

    return 1;
}

__instruction_opcode_reg_value(cnd) {
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

    return 1;
}
