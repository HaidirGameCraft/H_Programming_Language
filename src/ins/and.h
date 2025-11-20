#pragma once

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

void instruction_and_reg_reg(INSTRUCTION_SET_ARGS);
void instruction_and_value_reg(INSTRUCTION_SET_ARGS);