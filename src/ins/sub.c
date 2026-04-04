#include <inst.h>
#include <stdio.h>
#include "sub.h"
uint8_t instruction_sub_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED(reg, memory, pc)

    uint64_t result = *dreg - *sreg;
    *dreg = result;
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_CARRY( ( result >> 32 ) > 0 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1) != (*sreg >> 31 & 1) ) && ( (*dreg >> 31 & 1) != (result >> 31 & 1 )) );
    reg->flags |= FLAGS_SIGN( ( result >> 32 & 1 ) );
    reg->flags |= FLAGS_EVEN( result & 1);
    return 1;
}

uint8_t instruction_sub_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED(reg, memory, pc)

    uint64_t result = *dreg - value;
    *dreg = result;
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_CARRY( ( result >> 32 ) > 0 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1) != (*sreg >> 31 & 1) ) && ( (*dreg >> 31 & 1) != (result >> 31 & 1 )) );
    reg->flags |= FLAGS_SIGN( ( result >> 32 & 1 ) );
    reg->flags |= FLAGS_EVEN( result & 1);
    
    return 1;
}

SIGN_INSTRUCTION_SET(sub) {
    if( opcode == OPCODE_SUB_1 )
        return instruction_sub_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_SUB_2 )
        return instruction_sub_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

