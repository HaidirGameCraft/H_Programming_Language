#include "stp.h"

SIGN_INSTRUCTION_SET(stp) {
    if( opcode == OPCODE_STP )
        instruction_stp(prefix, ext_prefix, opcode, memory, reg, pc );
}

void instruction_stp(INSTRUCTION_SET_ARGS) {
    extern uint8_t is_program_running;
    is_program_running = 0;
}