#include "go.h"
#include "cpu.h"
#include "ins/inst.h"
#include "inst.h"
#include <stdio.h>

__instruction_opcode_reg(go) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    dreg = getreg( reg, __register.dreg );
    value = *dreg;
    reg->pc = value;

    return 1;
}

__instruction_opcode_value(go) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    value = read32( memory, pc ); 
    reg->pc = value;

    return 1;
}

__instruction_opcode_reg(gol);
__instruction_opcode_value(gol);
__instruction_opcode_reg(gole);
__instruction_opcode_value(gole);
__instruction_opcode_reg(gog);
__instruction_opcode_value(gog);
__instruction_opcode_reg(goge);
__instruction_opcode_value(goge);
__instruction_opcode_reg(goe);
__instruction_opcode_value(goe);
__instruction_opcode_reg(gone);
__instruction_opcode_value(gone);

SIGN_INSTRUCTION_SET(go) {
    uint8_t status = 0;
    if( opcode == OPCODE_GO_1 ) // using reg or regmem
        status = call_instruction_opcode_reg(go);
    else if ( opcode == OPCODE_GO_2 )
        status = call_instruction_opcode_value(go);
    else if ( opcode == OPCODE_GOL && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(gol);
    else if ( opcode == OPCODE_GOLE && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(gole);
    else if ( opcode == OPCODE_GOG && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(gog);
    else if ( opcode == OPCODE_GOGE && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(goge);
    else if ( opcode == OPCODE_GOE && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(goe);
    else if ( opcode == OPCODE_GONE && prefix == PREFIX_REG_INC )
        status = call_instruction_opcode_reg(gone);
    else if ( opcode == OPCODE_GOL && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(gol);
    else if ( opcode == OPCODE_GOLE && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(gole);
    else if ( opcode == OPCODE_GOG && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(gog);
    else if ( opcode == OPCODE_GOGE && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(goge);
    else if ( opcode == OPCODE_GOE && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(goe);
    else if ( opcode == OPCODE_GONE && prefix == PREFIX_VAL_INC )
        status = call_instruction_opcode_value(gone);
    return status;
}

__instruction_opcode_reg(gol) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
    
    // When both signed number give different negative
    if( (reg->flags & FLAGS_SIGN(1)) && !(reg->flags & FLAGS_ZERO(1)) )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(gol) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    // When both signed number give result negative
    if( (reg->flags & FLAGS_SIGN(1)) && !(reg->flags & FLAGS_ZERO(1)) )
        reg->pc = value;
    return 1;
}
__instruction_opcode_reg(gole) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result negative or zero
    if( ( reg->flags & FLAGS_SIGN(1)) || (reg->flags & FLAGS_ZERO(1)) )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(gole) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )

    // When both signed number give result negative or zero
    if( ( reg->flags & FLAGS_SIGN(1)) || (reg->flags & FLAGS_ZERO(1)) )
        reg->pc = value;
    return 1;
}
__instruction_opcode_reg(gog) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result positive
    if( !(reg->flags & FLAGS_SIGN(1)) && !(reg->flags & FLAGS_ZERO(1)) )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(gog) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result positive
    if( !(reg->flags & FLAGS_SIGN(1)) && !(reg->flags & FLAGS_ZERO(1)) )
        reg->pc = value;
    return 1;
}
__instruction_opcode_reg(goge) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
    
    // When both signed number give result positive or zero
    if ( (reg->flags & FLAGS_SIGN(1)) == 0 || ( reg->flags & FLAGS_ZERO(1) ) )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(goge) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result positive or zero
    if ( (reg->flags & FLAGS_SIGN(1)) == 0 || ( reg->flags & FLAGS_ZERO(1) ) )
        reg->pc = value;
    return 1;
}
__instruction_opcode_reg(goe) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result zero
    if( ( reg->flags & FLAGS_ZERO(1) ) )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(goe) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result zero
    if( ( reg->flags & FLAGS_ZERO(1) ) )
        reg->pc = value;
    return 1;
}
__instruction_opcode_reg(gone) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result not zero
    if( ( reg->flags & FLAGS_ZERO(1) ) == 0 )
        reg->pc = *sreg;
    return 1;
}
__instruction_opcode_value(gone) {
    VARIABLE_INSTRUCTION
    VARIABLE_DEFINED( reg, memory, pc )
 
    // When both signed number give result not zero
    if( ( reg->flags & FLAGS_ZERO(1) ) == 0 )
        reg->pc = value;
    return 1;
}
