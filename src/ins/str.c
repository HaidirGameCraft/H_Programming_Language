#include <string.h>
#include "str.h"

SIGN_INSTRUCTION_SET(str) {
    if( opcode == OPCODE_STR_1 )
        instruction_str_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_STR_2)
        instruction_str_reg_addr( prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_str_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = ((short)( offset & 0xFFFF));
    }

    addr = (uint32_t)(((int) *reg32(reg, __regmem.dreg)) + off16);

    if( __regmem.flags & 1 )
    {
        if( __ext_regmem.sbit & 0b10 )
        {
            r32 = reg32( reg, __regmem.sreg );
            write32( memory, addr, *r32 );
        } else if ( __ext_regmem.sbit & 0b01 )
        {
            r16 = reg16( reg, __regmem.sreg );
            write16( memory, addr, *r16 );
        } else {
            r8 = reg8( reg, __regmem.sreg );
            write8( memory, addr, *r8 );
        }
    }
    
}
void instruction_str_reg_addr(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = ((short)( offset & 0xFFFF));
    }

    addr = (uint32_t)(((int) read32(memory, pc)) + off16);

    if( __regmem.flags & 1 )
    {
        if( __ext_regmem.sbit & 0b10 )
        {
            r32 = reg32( reg, __regmem.sreg );
            write32( memory, addr, *r32 );
        } else if ( __ext_regmem.sbit & 0b01 )
        {
            r16 = reg16( reg, __regmem.sreg );
            write16( memory, addr, *r16 );
        } else {
            r8 = reg8( reg, __regmem.sreg );
            write8( memory, addr, *r8 );
        }
    }
}