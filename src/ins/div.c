#include <inst.h>
#include "div.h"
#include "ins/inst.h"
#include "interrupt.h"

__instruction_opcode_reg_reg(div);
__instruction_opcode_value_reg(div);

SIGN_INSTRUCTION_SET(div) {
    uint8_t status = 0;
    if( opcode == OPCODE_DIV_1 )
        status = call_instruction_opcode_reg_reg(div);
    else if ( opcode == OPCODE_DIV_2 )
        status = call_instruction_opcode_value_reg(div);

    return status;
}


__instruction_opcode_reg_reg( div ) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    uint32_t tmp = *dreg;
    if( *sreg == 0 ) {
        CallInterrupt(DivideZeroExceptionInt);
        return 1;
    }

    *dreg = (uint32_t)( tmp / *sreg );
    // remaining value will save on RI register
    reg->ri = (char)( tmp % *sreg );

    return 1;
}

__instruction_opcode_value_reg(div) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    value = read32( memory, pc );
    dreg = getreg( reg, __register.dreg );
    uint32_t tmp = *dreg;
    if( value == 0 ) {
        CallInterrupt(DivideZeroExceptionInt);
        return 1;
    }
    *dreg = (uint32_t)( tmp / value );
    // remaining value will save on RI register
    reg->ri = (char)( tmp % value );

    return 1;
}
