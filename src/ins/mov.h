#pragma once

#include <inst.h>
#include <ins_make.h>

uint32_t create_instruction_mov_regmem_reg(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_mov_reg_regmem(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_mov_value_regmem(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_mov_addr_reg(CREATE_INSTRUCTION_ARGS);

void instruction_mov_regmem_reg(INSTRUCTION_SET_ARGS);
void instruction_mov_reg_regmem(INSTRUCTION_SET_ARGS);
void instruction_mov_value_regmem(INSTRUCTION_SET_ARGS);