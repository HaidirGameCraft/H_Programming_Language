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

SIGN_INSTRUCTION_SET(go);
SIGN_INSTRUCTION_SET(gol);
SIGN_INSTRUCTION_SET(gole);
SIGN_INSTRUCTION_SET(gog);
SIGN_INSTRUCTION_SET(goge);
SIGN_INSTRUCTION_SET(goe);
SIGN_INSTRUCTION_SET(gone);
