#include "go.h"
#include "ins/inst.h"
#include <stdio.h>

__instruction_opcode_reg(go) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    dreg = getreg( reg, __register.dreg );
    value = *dreg;
    reg->pc = value;

    return 1;
}

__instruction_opcode_value(go) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc ); 
    reg->pc = value;

    return 1;
}

__instruction_opcode_reg(goc);
__instruction_opcode_value(goc);

SIGN_INSTRUCTION_SET(go) {
    uint8_t status = 0;
    if( opcode == OPCODE_GO_1 ) // using reg or regmem
        status = call_instruction_opcode_reg(go);
    else if ( opcode == OPCODE_GO_2 )
        status = call_instruction_opcode_value(go);
    else if ( opcode == OPCODE_GOC_1 )
        status = call_instruction_opcode_reg(goc);
    else if ( opcode == OPCODE_GOC_2 )
        status = call_instruction_opcode_value(goc);

    return status;
}


__instruction_opcode_value(goc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc );
    //printf("GOC Instruction: 0x%x\n", value);
    if( !(reg->flags & flags_condition) )
        return 1;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc = value;

    return 1;
}
__instruction_opcode_reg(goc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    if( !(reg->flags & flags_condition) )
        return 1;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc = *sreg;

    return 1;
}
