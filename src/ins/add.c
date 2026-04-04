#include <inst.h>
#include "cpu.h"
#include "inst.h"
#include <stdio.h>
#include <stdlib.h>
#include "add.h"

__instruction_opcode_reg_reg(add);
__instruction_opcode_value_reg(add);

SIGN_INSTRUCTION_SET(add) {
    uint8_t status = 0; 
    if( opcode == OPCODE_ADD_1 )
        status |= call_instruction_opcode_reg_reg(add);
    else if ( opcode == OPCODE_ADD_2 )
        status |= call_instruction_opcode_value_reg(add);
    return status;
}

__instruction_opcode_reg_reg(add) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    uint64_t result = *dreg + *sreg;
    *dreg = result & 0xFFFFFFFF;
    
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_CARRY( ( result >> 32 ) > 0 );
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_SIGN( ( result >> 31 ) & 1 );
    reg->flags |= FLAGS_EVEN( result & 1 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1 ) == ( *sreg >> 31 & 1 ) ) && ( ( *dreg >> 31 & 1 ) != ( result >> 31 & 1 ) ) );      // Add; d[31] == s[31] && d[31] != r[31]

    return 1;

}

__instruction_opcode_value_reg(add) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
   
    uint64_t result = *dreg + value;
    *dreg = result & 0xFFFFFFFF;

    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_CARRY( ( result >> 32 ) > 0 );
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_SIGN( ( result >> 31 ) & 1 );
    reg->flags |= FLAGS_EVEN( result & 1 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1 ) == ( *sreg >> 31 & 1 ) ) && ( ( *dreg >> 31 & 1 ) != ( result >> 31 & 1 ) ) );      // Add; d[31] == s[31] && d[31] != r[31]

    return 1;
}
