#pragma once

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

void instruction_cnd_reg_sym_reg(INSTRUCTION_SET_ARGS);
void instruction_cnd_value_sym_reg(INSTRUCTION_SET_ARGS);
void instruction_cnd_reg_sym_value(INSTRUCTION_SET_ARGS);