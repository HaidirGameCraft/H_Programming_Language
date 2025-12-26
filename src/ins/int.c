#include "int.h"
#include "inst.h"
#include "interrupt.h"
__instruction_opcode_reg(int) {
    if( prefix != PREFIX_REG_INC )
        return 0;
    
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    CallInterrupt(*sreg);
    return 1;
}

__instruction_opcode_value(int) {
    if( prefix != PREFIX_VAL_INC )
        return 0;

    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32(memory, pc);
    CallInterrupt(value);

    return 1;
}

SIGN_INSTRUCTION_SET(int) {
    uint8_t status = 0;
    if( opcode == OPCODE_INT )
    {
        status |= call_instruction_opcode_reg(int);
        status |= call_instruction_opcode_value(int);
    }

    return status;
}

