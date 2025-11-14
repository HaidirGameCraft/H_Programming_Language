#include <inst.h>
#include "pop.h"

SIGN_INSTRUCTION_SET(pop) {
    if( opcode == OPCODE_POP_1 )
        instruction_pop_reg(prefix, opcode, memory, reg, pc );
}


void instruction_pop_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    memset(&__regmem, read8( memory, pc ), sizeof( __regmem ) );

    if( !(prefix & PREFIX_32BITS ) )
        ASSERT("POP: cannot be use 16 or 8 bits register");

    dr32 = reg32( reg, __regmem.dreg );
    uint32_t stack_value = read32( memory, &reg->r32s );
    *dr32 = stack_value;
}

uint32_t create_instruction_pop_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("pop", opcode, 3 ) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_POP_1;
    uint8_t lreg = regbytext( lReg );
    return instruction_reg(memory, pc, prefix, op, lreg);
}