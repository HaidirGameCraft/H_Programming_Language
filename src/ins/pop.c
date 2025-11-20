#include <inst.h>
#include "pop.h"

SIGN_INSTRUCTION_SET(pop) {
    if( opcode == OPCODE_POP_1 )
        instruction_pop_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}


void instruction_pop_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    if( !(prefix & PREFIX_32BITS ) )
        ASSERT("POP: cannot be use 16 or 8 bits register");

    dr32 = reg32( reg, __regmem.dreg );
    uint32_t stack_value = read32( memory, &reg->r32s );
    *dr32 = stack_value;
}
