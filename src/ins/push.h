#pragma once

#include <inst.h>
#include <ins_make.h>

uint32_t create_instruction_push_regmem(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_push_value(CREATE_INSTRUCTION_ARGS);

void instruction_push_regmem(INSTRUCTION_SET_ARGS);
void instruction_push_value(INSTRUCTION_SET_ARGS);