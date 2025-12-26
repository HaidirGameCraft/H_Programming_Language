#include "sh.h"

uint8_t instruction_sh_reg_reg(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    uint8_t symbols = read8(memory, pc);
    sreg = getreg( reg, __register.dreg );
    dreg = getreg( reg, __register.sreg );
    if( symbols == SYM_SHIFT_LEFT )
        *dreg = *dreg << *sreg;
    else if ( symbols == SYM_SHIFT_RIGHT )
        *dreg = *dreg >> *sreg;
    else
        ASSERT("Interrupted. The Symbols of SH opcode is unknows");
    return 1;
}

uint8_t instruction_sh_reg_value(INSTRUCTION_SET_ARGS) {
    VARIABLE_INSTRUCTION
    REGMEM_DEFINED(memory, pc);

    uint8_t symbols = read8(memory, pc);
    dreg = getreg( reg, __register.sreg );
    value = read32( memory, pc );
    if( symbols == SYM_SHIFT_LEFT )
        *dreg = *dreg << value;
    else if ( symbols == SYM_SHIFT_RIGHT )
        *dreg = *dreg >> value;
    else
        ASSERT("Interrupted. The Symbols of SH opcode is unknows");
    return 1;
}
SIGN_INSTRUCTION_SET(sh) {
    if( opcode == OPCODE_SH_1 )
        return instruction_sh_reg_reg(prefix, ext_prefix, opcode, memory, reg, pc );
    else if ( opcode == OPCODE_SH_2 )
        return instruction_sh_reg_value( prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

