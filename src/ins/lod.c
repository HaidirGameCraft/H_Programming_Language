#include "lod.h"
#include "ins/inst.h"
#include <stdio.h>

__instruction_opcode_reg_reg(lod) {
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

    return 1;
}
__instruction_opcode_value_reg(lod) {
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

    return 1;
}
SIGN_INSTRUCTION_SET(lod) {
    uint8_t status = 0;
    if( opcode == OPCODE_LOD_1 || opcode == OPCODE_LODS_1 || opcode == OPCODE_LODD_1 )
        status = instruction_lod_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_LOD_2 || opcode == OPCODE_LODS_2 || opcode == OPCODE_LODD_2 )
        status = instruction_lod_value_reg(prefix, ext_prefix, opcode, memory, reg, pc );

    return status;
}

