#include "lod.h"
#include <stdio.h>

SIGN_INSTRUCTION_SET(lod) {
    if( opcode == OPCODE_LOD_1 || opcode == OPCODE_LODS_1 || opcode == OPCODE_LODD_1 )
        instruction_lod_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_LOD_2 || opcode == OPCODE_LODS_2 || opcode == OPCODE_LODD_2 )
        instruction_lod_addr_reg(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_lod_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = (short)( offset & 0xFFFF );
    }

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    addr = (uint32_t)(((int) *sreg) + off16);
    if( opcode == OPCODE_LODS_1 )
        *dreg = (*dreg & 0xFFFFFF00) + gread8( memory, addr );
    else if ( opcode == OPCODE_LODD_1 )
        *dreg = (*dreg & 0xFFFF0000) + gread16( memory, addr );
    else
        *dreg = gread32( memory, addr );
}
void instruction_lod_addr_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = (short)( offset && 0xFFFF);
    }

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );
    addr = (uint32_t)(((int) value) + off16);

    if( opcode == OPCODE_LODS_2 )
        *dreg = (*dreg & 0xFFFFFF00) + gread8( memory, addr );
    else if ( opcode == OPCODE_LODD_2 )
        *dreg = (*dreg & 0xFFFF0000) + gread16( memory, addr );
    else
        *dreg = gread32( memory, addr );
}