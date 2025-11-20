#include "lod.h"
#include <stdio.h>

SIGN_INSTRUCTION_SET(lod) {
    if( opcode == OPCODE_LOD_1 )
        instruction_lod_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_LOD_2 )
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

    addr = (uint32_t)(((int) *reg32(reg, __regmem.sreg)) + off16);


    if( __regmem.flags & (1 << 0) )
    {
        // using ext register
        if ( __ext_regmem.dbit & 0b10 )
        {
            dr32 = reg32( reg, __regmem.dreg );
            value = gread32( memory, addr );
            *dr32 = value;
        } else if ( __ext_regmem.dbit & 0b01 )
        {
            dr16 = reg16( reg, __regmem.dreg );
            value = gread16( memory, addr );
            *dr16 = value;
        } else {
            dr8 = reg8( reg, __regmem.dreg );
            value = gread8( memory, addr );
            *dr8 = value;
        }
    }
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

    addr = (uint32_t)(((int) read32(memory, pc )) + off16);


    if( __regmem.flags & (1 << 0) )
    {
        // using ext register
        if ( __ext_regmem.dbit & 0b10 )
        {
            dr32 = reg32( reg, __regmem.dreg );
            value = gread32( memory, addr);
            *dr32 = value;
        } else if ( __ext_regmem.dbit & 0b01 )
        {
            dr16 = reg16( reg, __regmem.dreg );
            value = gread16( memory, addr);
            *dr16 = value;
        } else {
            dr8 = reg8( reg, __regmem.dreg );
            value = gread8( memory, addr);
            *dr8 = value;
        }
    }
    
}