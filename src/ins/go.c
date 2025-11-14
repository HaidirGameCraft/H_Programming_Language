#include "go.h"
#include <stdio.h>

uint32_t create_instruction_go_regmem(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("go", opcode) != 0 ) return pc;
    uint8_t prefix = 0;
    uint8_t op = OPCODE_GO_1;
    uint8_t dreg = regbytext( lReg );
    if( offset != NULL )
        return instruction_regmem(memory, pc, prefix, op, dreg, *offset );
    else
        return instruction_reg(memory, pc, prefix, op, dreg);
}

uint32_t create_instruction_go_value(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("go", opcode) != 0 ) return pc;
    uint8_t prefix = 0;
    uint8_t op = OPCODE_GO_2;
    return instruction_value(memory, pc, prefix, op, *value);
}

uint32_t create_instruction_go_label(CREATE_INSTRUCTION_ARGS) {
    if( strcmp("go", opcode) != 0 ) return pc;
    uint8_t prefix = 0;
    uint8_t op = OPCODE_GO_2;
    return instruction_label(memory, pc, prefix, op);
}

void instruction_go_regmem(INSTRUCTION_SET_ARGS) {
    // GO REG
    // GO REGMEM

    VARIABLE_INSTRUCTION
    memset(&__regmem, read8(memory, pc), 1);

    if( prefix & PREFIX_32BITS )
    {
        r32 = reg32(reg, __regmem.dreg);
        if( prefix & PREFIX_MEMORY )
        {
            offset = read32(memory, pc );
            value = gread32(memory, *r32 + offset );
        } else {
            value = *r32;
        }

    } else if ( prefix & PREFIX_16BITS )
    {
        r16 = reg16( reg, __regmem.dreg );
        if( prefix & PREFIX_MEMORY )
        {
            offset = read16(memory, pc );
            value = gread16(memory, *r16 + offset );
        } else {
            value = *r16;
        }

    } else {
        r8 = reg8(reg, __regmem.dreg);
        if( prefix & PREFIX_MEMORY )
        {
            offset = read8(memory, pc );
            value = gread8(memory, *r8 + offset );
        } else {
            value = *r8;
        }
    }

    reg->pc32 = value;
}

void instruction_go_value(INSTRUCTION_SET_ARGS) {
    // GO VALUE
    VARIABLE_INSTRUCTION
    if( prefix & PREFIX_32BITS )
    {
        value = read32(memory, pc);

    } else if ( prefix & PREFIX_16BITS )
    {
        value = read16(memory, pc);

    } else {
        value = read8(memory, pc);
    }

    reg->pc32 = value;
}

SIGN_INSTRUCTION_SET(go) {
    if( opcode == OPCODE_GO_1 ) // using reg or regmem
        instruction_go_regmem(prefix, opcode, memory, reg, pc);
    else if ( opcode == OPCODE_GO_2 )
        instruction_go_value(prefix, opcode, memory, reg, pc );
}