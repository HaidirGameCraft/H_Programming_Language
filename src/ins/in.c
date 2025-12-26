#include "in.h"
#include "ins/inst.h"

__instruction_opcode_reg_reg(in);
__instruction_opcode_value_reg(in);

SIGN_INSTRUCTION_SET(in) {
    uint8_t status = 0;
    if( opcode == OPCODE_IN ) {
        status |= call_instruction_opcode_reg_reg(in);
        status |= call_instruction_opcode_value_reg(in);
    }
    return status;
}

__instruction_opcode_reg_reg(in) {
    if( prefix != PREFIX_REG_REG ) return 0;

    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    sreg = getreg( reg, __register.sreg );
    dreg = getreg( reg, __register.dreg );
    
    // port: 0x1103 - get the key character from user 
    if( *sreg == 0x1103 )
        *dreg = getchar();

    return 1;
}

__instruction_opcode_value_reg(in) {
    if( prefix != PREFIX_VAL_REG ) return 0;

    VARIABLE_INSTRUCTION
    REGMEM_DEFINED( memory, pc );

    dreg = getreg( reg, __register.dreg );
    value = read32( memory, pc );

    if( value == 0x1103 ) {
        *dreg = getchar();
    }

    return 1;
}
