#include "stp.h"
uint8_t instruction_stp(INSTRUCTION_SET_ARGS) {
    extern uint8_t is_program_running;
    is_program_running = 0;
    return 1;
}

SIGN_INSTRUCTION_SET(stp) {
    if( opcode == OPCODE_STP )
        return instruction_stp(prefix, ext_prefix, opcode, memory, reg, pc );
    return 0;
}

