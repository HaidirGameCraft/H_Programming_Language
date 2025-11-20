#include "go.h"
#include <stdio.h>

void instruction_go_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    if( prefix & PREFIX_32BITS)
    {
        r32 = reg32(reg, __regmem.sreg);
        value = *r32;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.sreg);
        value = *r16;
    }
    else {
        ASSERT("PREFIX 8BITS is not support");
    }

    reg->pc32 = value;
}
void instruction_go_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    if( prefix & PREFIX_32BITS)
    {
        value = read32( memory, pc );
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
    }
    else {
        ASSERT("PREFIX 8BITS is not support");
    }

    reg->pc32 = value;
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

    if( prefix & PREFIX_32BITS)
    {
        value = read32( memory, pc );
    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16( memory, pc );
    }
    else {
        ASSERT("PREFIX 8BITS is not support");
    }


    //printf("GOC Instruction: 0x%x\n", value);
    if( !(reg->flags & flags_condition) )
        return;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc32 = value;
}
void instruction_goc_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    if( prefix & PREFIX_32BITS)
    {
        r32 = reg32(reg, __regmem.sreg);
        value = *r32;
    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16(reg, __regmem.sreg);
        value = *r16;
    }
    else {
        ASSERT("PREFIX 8BITS is not support");
    }

    if( !(reg->flags & flags_condition) )
        return;

    reg->flags = reg->flags ^ flags_condition;
    reg->pc32 = value;
}