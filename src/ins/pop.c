#include <inst.h>
#include "pop.h"

SIGN_INSTRUCTION_SET(pop) {
    if( opcode == OPCODE_POP_1 )
        instruction_pop_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}


void instruction_pop_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);


    dreg = getreg( reg, __register.sreg );
    uint32_t stack_value = read32( memory, &reg->rs );
    *dreg = stack_value;
}
