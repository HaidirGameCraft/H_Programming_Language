#pragma once

/**
 * GO Instruction Set
 * Created By: Haidir
 * - go to destination address
 */

#include <inst.h>
#include <ins_make.h>

uint32_t create_instruction_go_regmem(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_go_value(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_go_label(CREATE_INSTRUCTION_ARGS);

void instruction_go_reg(INSTRUCTION_SET_ARGS);
void instruction_go_regmem(INSTRUCTION_SET_ARGS);
void instruction_go_value(INSTRUCTION_SET_ARGS);
void instruction_go_label(INSTRUCTION_SET_ARGS);