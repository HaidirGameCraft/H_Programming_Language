#pragma once

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

#define SYM_CND_EQUAL           1
#define SYM_CND_LESS            2
#define SYM_CND_GREATER         3
#define SYM_CND_LESS_EQUAL      4
#define SYM_CND_GREATER_EQUAL   5

void instruction_cnd_reg_sym_reg(INSTRUCTION_SET_ARGS);
void instruction_cnd_value_sym_reg(INSTRUCTION_SET_ARGS);
void instruction_cnd_reg_sym_value(INSTRUCTION_SET_ARGS);