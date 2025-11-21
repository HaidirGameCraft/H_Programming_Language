#include "go.h"
#include <stdio.h>

void instruction_go_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    dreg = getreg( reg, __register.dreg );
    value = *dreg;
    reg->pc = value;
}
void instruction_go_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc ); 
    reg->pc = value;
}

SIGN_INSTRUCTION_SET(go) {
    if( opcode == OPCODE_GO_1 ) // using reg or regmem
        instruction_go_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_GO_2 )
        instruction_go_value(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_GOC_1 )
        instruction_goc_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_GOC_2 )
        instruction_goc_value(prefix, ext_prefix, opcode, memory, reg, pc);
}


void instruction_goc_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc );
    //printf("GOC Instruction: 0x%x\n", value);
    if( !(reg->flags & flags_condition) )
        return;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc = value;
}
void instruction_goc_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    if( !(reg->flags & flags_condition) )
        return;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc = value;
}