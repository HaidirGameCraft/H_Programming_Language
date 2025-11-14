#pragma once

#include <inst.h>
#include <ins_make.h>

uint32_t create_instruction_call_reg(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_call_regmem(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_call_value(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_call_label(CREATE_INSTRUCTION_ARGS);

void instruction_call_reg(INSTRUCTION_SET_ARGS);
void instruction_call_regmem(INSTRUCTION_SET_ARGS);
void instruction_call_value(INSTRUCTION_SET_ARGS);