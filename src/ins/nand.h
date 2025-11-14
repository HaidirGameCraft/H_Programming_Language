#pragma once

#include <inst.h>
#include <ins_make.h>

uint32_t create_instruction_nand_regmem_reg(CREATE_INSTRUCTION_ARGS);
uint32_t create_instruction_nand_value_reg(CREATE_INSTRUCTION_ARGS);

void instruction_nand_regmem_reg(INSTRUCTION_SET_ARGS);
void instruction_nand_value_reg(INSTRUCTION_SET_ARGS);