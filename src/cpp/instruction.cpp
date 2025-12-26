#include <stdint.h>
#include <inst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asrt.h>
#include "ins_make.hpp"
#include <stdbool.h>
#include <iostream>

#include <data/ascii.h>
#include <data/datatype.h>

#include <ins/inst.h>
#include <cpp/ins_compiler.list.hpp>

/**
 * Instruction
 */
uint32_t instruction_reg_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint8_t sym) {
    prefix |= PREFIX_REG_INC;

    if( sym != 0 )
        prefix |= PREFIX_SYM_INC;

    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;
    __register.dreg = rReg & 0b1111;

    if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
        ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    if( sym != 0 )
        pc = write8(memory, pc, sym );
    return pc;
}

uint32_t instruction_label_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint8_t sym, uint32_t value) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC;
    ins_register_t __register = {0};
    __register.dreg = rReg & 0b1111;

    if( sym != 0 )
        prefix |= PREFIX_SYM_INC;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    if( sym != 0 )
        pc = write8(memory, pc, sym );
    // Value Origin
    write32(memory, pc, 0); pc += 4;
    return pc;
}

uint32_t instruction_value_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint8_t sym, uint32_t value) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC;
    ins_register_t __register = {0};
    __register.dreg = rReg & 0b1111;

    if( sym != 0 )
        prefix |= PREFIX_SYM_INC;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    if( sym != 0 )
        pc = write8(memory, pc, sym );
    
    pc = write32(memory, pc, value);

    return pc;
}

uint32_t instruction_reg_value  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t sym, uint32_t value) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC;
    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;

    if( sym != 0 )
        prefix |= PREFIX_SYM_INC;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    if( sym != 0 )
        pc = write8(memory, pc, sym );
    // Value Origin
    write32(memory, pc, value); pc += 4;
    return pc;
}

uint32_t instruction_reg_label  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t sym, uint32_t value) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC;
    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;

    if( sym != 0 )
        prefix |= PREFIX_SYM_INC;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    if( sym != 0 )
        pc = write8(memory, pc, sym );
    // Value Origin
    write32(memory, pc, 0); pc += 4;
    return pc;
}

uint32_t instruction_regoff_reg    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, short offset) {
    prefix |= PREFIX_OFF_INC;

    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;
    __register.dreg = rReg & 0b1111;

    // if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
    //     ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    pc = write16(memory, pc, (uint16_t) offset );

    return pc;
}
uint32_t instruction_valueoff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, short offset, uint32_t value ) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC | PREFIX_OFF_INC;
    ins_register_t __register = {0};
    __register.dreg = rReg & 0b1111;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    pc = write16(memory, pc, (uint16_t) offset );
    pc = write32(memory, pc, value);

    return pc;
}
uint32_t instruction_labeloff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, short offset, uint32_t value ) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC | PREFIX_OFF_INC;
    ins_register_t __register = {0};
    __register.dreg = rReg & 0b1111;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    pc = write16( memory, pc, (uint16_t) offset );
    // Value Origin
    write32(memory, pc, 0); pc += 4;
    return pc;
}

uint32_t instruction_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg) {
    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;

    return pc;
}

uint32_t instruction_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint32_t value ) {
    prefix |= PREFIX_VAL_INC;
    ins_register_t __register = {0};
    write8( memory, pc, prefix ); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8( memory, pc, opcode ); pc++;
    pc = write32( memory, pc, value );
    
    return pc;
}

uint32_t instruction_label(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode) {
    prefix |= PREFIX_VAL_INC;
    write8( memory, pc, prefix ); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8( memory, pc, opcode ); pc++;
    write32( memory, pc, 0); pc += 4;
    return pc;
}

uint32_t instruction_opcode(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode) {
    prefix |= 0x0F;
    write8(memory, pc, prefix); pc++;
    write8(memory, pc, opcode); pc++;
    return pc;
}

// uint32_t instruction_reg_sym_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rreg, uint32_t value) {
//     return instruction_value_sym_reg(memory, pc, prefix, ext_prefix, sym, rreg, value );
// }

uint32_t instruction_reg_regoff    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg   , short offset) {
    prefix |= PREFIX_OFF_INC;

    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;
    __register.dreg = rReg & 0b1111;

    // if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
    //     ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    pc = write16(memory, pc, (uint16_t) offset );

    return pc;
}
uint32_t instruction_reg_valueoff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, short offset, uint32_t value ) {
    prefix |= PREFIX_VAL_INC | PREFIX_REG_INC | PREFIX_OFF_INC;
    ins_register_t __register = {0};
    __register.sreg = lReg & 0b1111;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__register)); pc++;
    pc = write16(memory, pc, (uint16_t) offset );
    pc = write32(memory, pc, value);

    return pc;
}
uint32_t instruction_reg_labeloff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, short offset, uint32_t value ) {
    return instruction_reg_valueoff( memory, pc, prefix, ext_prefix, opcode, lReg, offset, 0 );
}










