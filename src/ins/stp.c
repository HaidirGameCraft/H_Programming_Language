#include "stp.h"

SIGN_INSTRUCTION_SET(stp) {
    if( opcode == OPCODE_STP )
        instruction_stp(prefix, opcode, memory, reg, pc );
}

uint32_t create_instruction_stp(CREATE_INSTRUCTION_ARGS) {
    if( strcmp(opcode, "stp") != 0 )
        return pc;

    uint8_t prefix = 0x0F;
    uint8_t op = OPCODE_STP;

    write8(memory, pc, prefix); pc++;
    write8(memory, pc, op); pc++;
    return pc;
}

void instruction_stp(INSTRUCTION_SET_ARGS) {
    extern uint8_t is_program_running;
    is_program_running = 0;
}