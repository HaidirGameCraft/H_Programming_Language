#include <string.h>
#include "str.h"
uint8_t instruction_str_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = (short)( offset & 0xFFFF );
    }

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    addr = (uint32_t)(((int) *dreg) + off16);

    if( opcode == OPCODE_STRS_1 )
        write8( memory, addr, *sreg );
    else if ( opcode == OPCODE_STRD_1 )
        write16( memory, addr, *sreg );
    else
        write32(memory, addr, *sreg);
    return 1;    
}
uint8_t instruction_str_reg_addr(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    uint32_t addr = 0;
    short off16 = 0;
    if( prefix & PREFIX_OFF_INC )
    {
        offset = read16( memory, pc );
        off16 = (short)( offset & 0xFFFF );
    }

    sreg = getreg( reg, __register.sreg );
    value = read32( memory, pc );
    addr = (uint32_t)(((int) value) + off16);
    if( opcode == OPCODE_STRS_2 )
        write8( memory, addr, *sreg );
    else if ( opcode == OPCODE_STRD_2 )
        write16( memory, addr, *sreg );
    else
        write32(memory, addr, *sreg);
    return 1;
}

SIGN_INSTRUCTION_SET(str) {
    if( opcode == OPCODE_STR_1 || opcode == OPCODE_STRS_1 || opcode == OPCODE_STRD_1 )
        return instruction_str_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_STR_2 || opcode == OPCODE_STRS_2 || opcode == OPCODE_STRD_2 )
        return instruction_str_reg_addr( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

