#include <inst.h>
#include "and.h"
#include "cpu.h"
#include "ins/inst.h"
#include <stdio.h>

__instruction_opcode_reg_reg(and);
__instruction_opcode_value_reg(and);

SIGN_INSTRUCTION_SET(and) {
    uint8_t status = 0;
    if( opcode == OPCODE_AND_1 )
        status = call_instruction_opcode_reg_reg(and);
    else if ( opcode == OPCODE_AND_2 )
        status = call_instruction_opcode_value_reg(and);
    return status;
}

__instruction_opcode_reg_reg(and) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg(reg, __register.sreg);
    dreg = getreg(reg, __register.dreg);
    uint64_t result = *dreg & *sreg;
    *dreg = result;

    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_EVEN( result & 1 );
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_SIGN( ( result >> 31 & 1 ) );

    return 1;
}

__instruction_opcode_value_reg(and) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    dreg = getreg(reg, __register.dreg);
    value = read32(memory, pc);
    uint64_t result = *dreg & value;
    *dreg = result;

    reg->flags &= ~FLAGS_STATUS_MASK;
    reg->flags |= FLAGS_EVEN( result & 1 );
    reg->flags |= FLAGS_ZERO( result == 0 );
    reg->flags |= FLAGS_SIGN( ( result >> 31 & 1 ) );

    return 1;
}
