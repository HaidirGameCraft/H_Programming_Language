#pragma once

#include <inst.h>
#ifdef __cpluplus
#include <cpp/ins_make.hpp>
#else
#include <ins_make.h>
#endif

void instruction_call_reg(INSTRUCTION_SET_ARGS);
void instruction_call_regmem(INSTRUCTION_SET_ARGS);
void instruction_call_value(INSTRUCTION_SET_ARGS);