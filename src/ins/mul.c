#include <inst.h>
#include "mul.h"

uint8_t instruction_mul_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED(reg, memory, pc)

    *dreg = *dreg * *sreg;
    uint64_t _v = *dreg;
    reg->flags = (reg->flags & ~FLAGS_ZERO(1)) | FLAGS_ZERO( *dreg == 0 );
    reg->flags = (reg->flags & ~FLAGS_SIGN(1)) | FLAGS_SIGN( ( *dreg >> 31 & 1 ) );
    reg->flags = (reg->flags & ~FLAGS_OVERFLOW(1)) | FLAGS_OVERFLOW( (_v >> 32 & 1) );
    
    return 1;
}

uint8_t instruction_mul_value_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED(reg, memory, pc)

    *dreg = *dreg * value;
    uint64_t _v = *dreg;
    reg->flags = (reg->flags & ~FLAGS_ZERO(1)) | FLAGS_ZERO( *dreg == 0 );
    reg->flags = (reg->flags & ~FLAGS_SIGN(1)) | FLAGS_SIGN( ( *dreg >> 31 & 1 ) );
    reg->flags = (reg->flags & ~FLAGS_OVERFLOW(1)) | FLAGS_OVERFLOW( (_v >> 32 & 1) );

    return 1;
}

SIGN_INSTRUCTION_SET(mul) {
    uint8_t status = 0;
    if( opcode == OPCODE_MUL_1 )
        status = instruction_mul_reg_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_MUL_2 )
        status = instruction_mul_value_reg( prefix, ext_prefix, opcode, memory, reg, pc );
    return status;
}

