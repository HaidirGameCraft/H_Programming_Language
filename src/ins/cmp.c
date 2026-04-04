#include "cmp.h"
#include "cpu.h"
#include "inst.h"

__instruction_opcode_reg_reg(cmp);
__instruction_opcode_reg_value(cmp);
__instruction_opcode_value_reg(cmp);

SIGN_INSTRUCTION_SET(cmp) {
    uint8_t status = 0;
    if( opcode == OPCODE_CMP ) // using reg or regmem
    {
        if( prefix == PREFIX_REG_REG )
            status = call_instruction_opcode_reg_reg(cmp);
        else if ( prefix == PREFIX_REG_VAL )
            status = call_instruction_opcode_reg_value(cmp);
        else if ( prefix == PREFIX_VAL_REG )
            status = call_instruction_opcode_value_reg(cmp);
    }
    return status;
}

__instruction_opcode_reg_reg(cmp) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    uint64_t diff = *dreg - *sreg;
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_ZERO( diff == 0 );
    reg->flags |= FLAGS_EVEN( diff & 1);
    reg->flags |= FLAGS_SIGN( ( diff >> 31 & 1 ) );
    reg->flags |= FLAGS_CARRY( ( diff >> 32 ) > 0 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1) != (*sreg >> 31 & 1) ) && ( (*dreg >> 31 & 1) != (diff >> 31 & 1 )) );
    return 1;
}
__instruction_opcode_reg_value(cmp) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    uint64_t diff = *dreg - *sreg;
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_ZERO( diff == 0 );
    reg->flags |= FLAGS_EVEN( diff & 1 );
    reg->flags |= FLAGS_SIGN( ( diff >> 31 & 1 ) );
    reg->flags |= FLAGS_CARRY( ( diff >> 32 ) > 0 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1) != (*sreg >> 31 & 1) ) && ( (*dreg >> 31 & 1) != (diff >> 31 & 1 )) );
    return 1;
}
__instruction_opcode_value_reg(cmp) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    uint64_t diff = *dreg - value;
    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_ZERO( diff == 0 );
    reg->flags |= FLAGS_EVEN( diff & 1);
    reg->flags |= FLAGS_SIGN( ( diff >> 31 & 1 ) );
    reg->flags |= FLAGS_CARRY( ( diff >> 32 ) > 0 );
    reg->flags |= FLAGS_OVERFLOW( ( ( *dreg >> 31 & 1) != (*sreg >> 31 & 1) ) && ( (*dreg >> 31 & 1) != (diff >> 31 & 1 )) );
    return 1;
}
