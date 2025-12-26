#include "pct.h"
#include <stdio.h>
uint8_t instruction_pct_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    sreg = getreg( reg, __register.sreg );
    char c = (char)(*sreg & 0xFF);
    putc(c, stdout);
    return 1;
}

uint8_t instruction_pct_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32(memory, pc);
    char c = (char)(value & 0xFF);
    putc(c, stdout);
    return 1;
}

SIGN_INSTRUCTION_SET(pct) {
    if( opcode == OPCODE_PCT_1 )
        return instruction_pct_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_PCT_2 )
        return instruction_pct_value( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

