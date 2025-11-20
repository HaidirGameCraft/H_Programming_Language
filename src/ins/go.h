#pragma once

/**
 * GO and GOC (GO when Condition) Instruction Set
 * Created By: Haidir
 * - go to destination address
 */

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

void instruction_go_reg(INSTRUCTION_SET_ARGS);
void instruction_go_value(INSTRUCTION_SET_ARGS);
void instruction_go_addr(INSTRUCTION_SET_ARGS);

void instruction_goc_reg(INSTRUCTION_SET_ARGS);
void instruction_goc_value(INSTRUCTION_SET_ARGS);
void instruction_goc_addr(INSTRUCTION_SET_ARGS);