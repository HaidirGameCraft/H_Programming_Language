#include <inst.h>
#include <stdint.h>
#include <stdlib.h>
#include "mov.h"
#include "ins/inst.h"
__instruction_opcode_reg_reg(mov) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );

    if( opcode == OPCODE_MOVS_1 )
        *dreg = (*dreg & 0xFFFFFF00) + (*sreg & 0xFF);
    else if ( opcode == OPCODE_MOVD_1 )
        *dreg = (*dreg & 0xFFFF0000) + (*sreg & 0xFFFF);
    else
        *dreg = *sreg;

    return 1;
}

__instruction_opcode_value_reg(mov) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );

    if( opcode == OPCODE_MOVS_2 )
        *dreg = (*dreg & 0xFFFFFF00) + (value & 0xFF);
    else if ( opcode == OPCODE_MOVD_2 )
        *dreg = (*dreg & 0xFFFF0000) + (value & 0xFFFF);
    else
        *dreg = value;

    return 1;
}

SIGN_INSTRUCTION_SET(mov) {
    uint8_t status = 0;
    if( opcode == OPCODE_MOV_1 || opcode == OPCODE_MOVS_1 || opcode == OPCODE_MOVD_1 )
        status |= instruction_mov_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_MOV_2 || opcode == OPCODE_MOVS_2 || opcode == OPCODE_MOVD_2 )
        status |= instruction_mov_value_reg(prefix, ext_prefix, opcode, memory, reg, pc);
    return status;
}


