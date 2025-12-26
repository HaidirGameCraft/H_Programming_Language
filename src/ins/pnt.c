#include "pnt.h"
#include <stdio.h>
uint8_t instruction_pnt_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    uint32_t addr = *sreg;
    int index = 0;
    char c = 0;
    while( (c = gread8(memory, addr + index)) != 0 )
    {
        putc(c, stdout);
        addr++;
    }
    return 1;
}

uint8_t instruction_pnt_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    uint32_t addr = read32( memory, pc );
    int index = 0;
    char c = 0;
    while( (c = gread8(memory, addr + index)) != 0 )
    {
        putc(c, stdout);
        addr++;
    }
    return 1;
}

SIGN_INSTRUCTION_SET(pnt) {
    if( opcode == OPCODE_PNT_1 )
        return instruction_pnt_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_PNT_2 )
        return instruction_pnt_value( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

