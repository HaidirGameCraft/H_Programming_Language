#include "go.h"
#include "cpu.h"
#include "ins/inst.h"
#include "inst.h"
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
__instruction_opcode_reg(gonc);
__instruction_opcode_value(gonc);

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
    else if ( opcode == OPCODE_GONC && (prefix & PREFIX_VAL_INC) )
        status = call_instruction_opcode_value(gonc);
    else if ( opcode == OPCODE_GONC && ( prefix & PREFIX_REG_INC) )
        status = call_instruction_opcode_reg(gonc);
    return status;
}


__instruction_opcode_value(goc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc );
    //printf("GOC Instruction: 0x%x\n", value);
    //printf("Condition: %i\n", FLAGS_CONDITION );
    if( !(reg->flags & FLAGS_CONDITION) )
        return 1;

    reg->flags = reg->flags ^ FLAGS_CONDITION;
    reg->pc = value;

    return 1;
}
__instruction_opcode_reg(goc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    if( !(reg->flags & FLAGS_CONDITION) )
        return 1;

    reg->flags = reg->flags ^ FLAGS_CONDITION;
    reg->pc = *sreg;

    return 1;
}


__instruction_opcode_value(gonc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc );
    if( reg->flags & FLAGS_CONDITION ) {
        reg->flags = reg->flags ^ FLAGS_CONDITION;
        return 1;
    }

//    reg->flags = reg->flags ^ FLAGS_CONDITION;
    reg->pc = value;

    return 1;
}
__instruction_opcode_reg(gonc) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    if( reg->flags & FLAGS_CONDITION ) {
        reg->flags = reg->flags ^ FLAGS_CONDITION;
        return 1;
    }

//    reg->flags = reg->flags ^ FLAGS_CONDITION;
    reg->pc = *sreg;

    return 1;
}
