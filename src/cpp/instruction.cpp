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
uint32_t instruction_reg_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg) {
    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;
    regmem.dreg = rReg & 0b111;

    if( lReg & 0b10000 )
    {
        prefix |= PREFIX_32BITS;
        ext_prefix |= EXT_PREFIX_ADDRSIZE;
    }
    else if ( lReg & 0b1000 )
    {
        prefix |= PREFIX_16BITS;
    }

    if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
        ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    if( opcode == OPCODE_LOD_1 || opcode == OPCODE_STR_1 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.sbit = (lReg >> 3) & 0b11;
        ext_regmem.dbit = (rReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }

    return pc;
}

uint32_t instruction_label_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint32_t value) {
    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.dreg = rReg & 0b111;
    if( rReg & 0b10000 )    prefix |= PREFIX_32BITS;
    else if ( rReg & 0b1000) prefix |= PREFIX_16BITS;

    if( opcode == OPCODE_LOD_2 || opcode == OPCODE_STR_2 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.dbit = (rReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }
    // Value Origin
    write32(memory, pc, 0); pc += 4;
    return pc;
}

uint32_t instruction_value_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint32_t value) {
    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.dreg = rReg & 0b111;

    if( rReg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( rReg & 0b1000 )
        prefix |= PREFIX_16BITS;

    if( opcode == OPCODE_LOD_2 || opcode == OPCODE_STR_2 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.dbit = (rReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }
    // Value Origin
    if( (prefix & PREFIX_EXT_INC) )
    {
        if( ext_prefix & EXT_PREFIX_ADDRSIZE )
        {
            write32(memory, pc, value);
            pc += 4;
        } else
        {
            write16( memory, pc, value );
            pc += 2;
        }
    } else {
        if( prefix & PREFIX_32BITS )
        {
            write32(memory, pc, value);
            pc += 4;
        } else if ( prefix & PREFIX_16BITS )
        {
            write16( memory, pc, value );
            pc += 2;
        } else {
            write8( memory, pc, value ); 
            pc++;
        }
    }

    return pc;
}

uint32_t instruction_regoff_reg    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg   , uint32_t offset) {
    prefix |= PREFIX_OFF_INC;

    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;
    regmem.dreg = rReg & 0b111;

    if( lReg & 0b10000 )
    {
        prefix |= PREFIX_32BITS;
        ext_prefix |= EXT_PREFIX_ADDRSIZE;
    }
    else if ( lReg & 0b1000 )
    {
        prefix |= PREFIX_16BITS;
    }

    if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
        ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    if( opcode == OPCODE_LOD_1 || opcode == OPCODE_STR_1 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.sbit = (lReg >> 3) & 0b11;
        ext_regmem.dbit = (rReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }
    
    // the offset would be limited to 65536 due to access memory
    // so, the offset can be write16 bits and not 32 bits to prevent it
    // convert the uint16_t into short
    short off16 = (short) ( offset & 0xFFFF );
    pc = write16( memory, pc, off16 );

    return pc;
}
uint32_t instruction_valueoff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint32_t offset, uint32_t value ) {
    prefix |= PREFIX_OFF_INC;

    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;

    if( lReg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( lReg & 0b1000 )
        prefix |= PREFIX_16BITS;

    if( opcode == OPCODE_LOD_2 || opcode == OPCODE_STR_2 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.dbit = (lReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }

    // offset
    short off16 = (short) ( offset & 0xFFFF );
    pc = write16( memory, pc, off16 );

    // Value Origin
    if( (prefix & PREFIX_EXT_INC) )
    {
        if( ext_prefix & EXT_PREFIX_ADDRSIZE )
        {
            write32(memory, pc, value);
            pc += 4;
        } else
        {
            write16( memory, pc, value );
            pc += 2;
        }
    }

    return pc;
}
uint32_t instruction_labeloff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint32_t offset, uint32_t value ) {
    prefix |= PREFIX_OFF_INC;

    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;

    if( lReg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( lReg & 0b1000 )
        prefix |= PREFIX_16BITS;

    if( opcode == OPCODE_LOD_2 || opcode == OPCODE_STR_2 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.dbit = (lReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }

    // offset
    short off16 = (short) ( offset & 0xFFFF );
    pc = write16( memory, pc, off16 );

    // Value Origin
    pc = write32( memory, pc, 0 );

    return pc;
}

uint32_t instruction_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg) {
    ins_regmem_t regmem = {0};
    regmem.dreg = rReg & 0b111;

    if( rReg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( rReg & 0b1000 )
        prefix |= PREFIX_16BITS;

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;

    return pc;
}

uint32_t instruction_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint32_t value ) {
    ins_regmem_t regmem = {0};

    if( value > 0xFFFF )
        prefix |= PREFIX_32BITS;
    else if ( value > 0xFF )
        prefix |= PREFIX_16BITS;
        

    write8( memory, pc, prefix ); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }
    write8( memory, pc, opcode ); pc++;
    if( prefix & PREFIX_32BITS )
    {
        write32( memory, pc, value); 
        pc += 4;
    }
    else if( prefix & PREFIX_16BITS )
    {
        write16( memory, pc, value); 
        pc += 2;
    }
    else {
        write8( memory, pc, value); 
        pc ++;
    }
    return pc;
}

uint32_t instruction_label(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode) {
    prefix |= PREFIX_32BITS;
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

uint32_t instruction_reg_sym_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t sym, uint8_t lreg, uint8_t rreg) {
    prefix |= PREFIX_RM_INC;
    ins_regmem_t __regmem;
    __regmem.sreg = lreg & 0b111;
    __regmem.dreg = rreg & 0b111;


    if( lreg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( lreg & 0b01000 )
        prefix |= PREFIX_16BITS;

    if( (lreg & 0b11000 ) != (rreg & 0b11000) ) {
        ASSERT("Error Compiling: LREG is not same bits as RREG");
    } 

    write8(memory, pc, prefix); pc++;
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__regmem)); pc++;
    write8(memory, pc, sym); pc++;
    return pc;
}

uint32_t instruction_opcode(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode) {
    prefix |= 0x0F;
    write8(memory, pc, prefix); pc++;
    write8(memory, pc, opcode); pc++;
    return pc;
}

uint32_t instruction_value_sym_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t sym, uint8_t rreg, uint32_t value) {
    prefix |= PREFIX_RM_INC;
    ins_regmem_t __regmem;
    __regmem.dreg = rreg & 0b111;


    if( rreg & 0b10000 )
        prefix |= PREFIX_32BITS;
    else if ( rreg & 0b01000 )
        prefix |= PREFIX_16BITS;

    write8(memory, pc, prefix); pc++;
    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &__regmem)); pc++;
    write8(memory, pc, sym); pc++;
    if( prefix & PREFIX_32BITS )
    {
        write32(memory, pc, value); pc += 4;
    }
    else if( prefix & PREFIX_16BITS )
    {
        write16(memory, pc, value); pc += 2;
    }
    else {
        write8(memory, pc, value); pc ++;
    }

    return pc;
}

// uint32_t instruction_reg_sym_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t sym, uint8_t rreg, uint32_t value) {
//     return instruction_value_sym_reg(memory, pc, prefix, ext_prefix, sym, rreg, value );
// }

uint32_t instruction_reg_regoff    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg   , uint32_t offset) {
    prefix |= PREFIX_OFF_INC;

    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;
    regmem.dreg = rReg & 0b111;

    // if( lReg & 0b10000 )
    // {
    //     prefix |= PREFIX_32BITS;
    //     ext_prefix |= EXT_PREFIX_ADDRSIZE;
    // }
    // else if ( lReg & 0b1000 )
    // {
    //     prefix |= PREFIX_16BITS;
    // }

    if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1) && (lReg & 0b11000) != (rReg & 0b11000) )
        ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    if( opcode == OPCODE_STR_1 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.sbit = (lReg >> 3) & 0b11;
        ext_regmem.dbit = (rReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }
    
    // the offset would be limited to 65536 due to access memory
    // so, the offset can be write16 bits and not 32 bits to prevent it
    // convert the uint16_t into short
    short off16 = (short) ( offset & 0xFFFF );
    pc = write16( memory, pc, off16 );

    return pc;
}
uint32_t instruction_reg_valueoff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint32_t offset, uint32_t value ) {
    prefix |= PREFIX_OFF_INC;

    ins_regmem_t regmem = {0};
    ins_ext_regmem_t ext_regmem = {0};
    regmem.sreg = lReg & 0b111;

    // if( lReg & 0b10000 )
    // {
    //     prefix |= PREFIX_32BITS;
    //     ext_prefix |= EXT_PREFIX_ADDRSIZE;
    // }
    // else if ( lReg & 0b1000 )
    // {
    //     prefix |= PREFIX_16BITS;
    // }

    // if( (opcode != OPCODE_LOD_1 && opcode != OPCODE_STR_1))
    //     ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

    if( opcode == OPCODE_STR_2 )
    {
        regmem.flags |= 1 << 0;      // set the extended instruction register
        ext_regmem.sbit = (lReg >> 3) & 0b11;
    }

    write8(memory, pc, prefix); pc++;
    if( prefix & PREFIX_EXT_INC )
    {
        write8(memory, pc, ext_prefix); 
        pc++;
    }

    write8(memory, pc, opcode); pc++;
    write8(memory, pc, *((uint8_t*) &regmem)); pc++;
    if( regmem.flags & (1 << 0) ) {
        write8(memory, pc, *((uint8_t*) &ext_regmem)); pc++;
    }
    
    // the offset would be limited to 65536 due to access memory
    // so, the offset can be write16 bits and not 32 bits to prevent it
    // convert the uint16_t into short
    short off16 = (short) ( offset & 0xFFFF );
    pc = write16( memory, pc, off16 );
    pc = write32( memory, pc, value );

    return pc;
}
uint32_t instruction_reg_labeloff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint32_t offset, uint32_t value ) {
    return instruction_reg_valueoff( memory, pc, prefix, ext_prefix, opcode, lReg, offset, 0 );
}










