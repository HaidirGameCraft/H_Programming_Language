#pragma once

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

void instruction_sub_reg_reg(INSTRUCTION_SET_ARGS);
void instruction_sub_value_reg(INSTRUCTION_SET_ARGS);