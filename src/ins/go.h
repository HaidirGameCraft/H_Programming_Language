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

SIGN_INSTRUCTION_SET(goc);
SIGN_INSTRUCTION_SET(go);
