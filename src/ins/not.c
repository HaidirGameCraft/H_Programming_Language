#include "not.h"

uint32_t create_instruction_not_reg(CREATE_INSTRUCTION_ARGS) {
    if( strncmp("or", opcode, 3) != 0 )
        return pc;

    uint8_t prefix = PREFIX_RM_INC;
    uint8_t op = OPCODE_NOT;
    uint8_t dreg = regbytext(rReg);
    
    return instruction_reg(memory, pc, prefix, op, dreg);
}