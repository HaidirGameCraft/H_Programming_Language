#pragma once

#include <stdint.h>
#include <inst.h>

#include <ins/add.h>
#include <ins/and.h>
#include <ins/div.h>
#include <ins/mov.h>
#include <ins/mul.h>
#include <ins/push.h>
#include <ins/pop.h>
#include <ins/sub.h>
#include <ins/or.h>
#include <ins/not.h>
#include <ins/nand.h>
#include <ins/nor.h>
#include <ins/xor.h>
#include <ins/stp.h>
#include <ins/call.h>
#include <ins/ret.h>
#include <ins/go.h>
#include <ins/cnd.h>
#include <ins/sh.h>
#include <ins/in.h>
#include <ins/lod.h>
#include <ins/str.h>
#include <ins/pct.h>
#include <ins/pnt.h>
#include <ins/int.h>

#define PREFIX_REG_REG                 ( PREFIX_REG_INC )
#define PREFIX_VAL_REG                 ( PREFIX_REG_INC | PREFIX_VAL_INC )
#define PREFIX_VALOFF_REG              ( PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_OFF_INC )
#define PREFIX_LBL_REG                   PREFIX_VAL_REG
#define PREFIX_LBLOFF_REG                PREFIX_VALOFF_REG 
#define PREFIX_REG_VALOFF                PREFIX_VAL_REG
#define PREFIX_REG_LBLOFF                PREFIX_VALOFF_REG
#define PREFIX_OPCODE_ONLY               0


typedef struct h_text_compile {
    const char* opcode_name;
    uint8_t opcode;
    uint8_t prefix;
    uint8_t ext_prefix;
    uint8_t flip;
} h_text_compile;

typedef h_text_compile inst_opcode_t;

#define call_argument_instruction prefix, ext_prefix, opcode, memory, reg, pc
#define __instruction_opcode_reg_reg(x) uint8_t instruction_##x##_reg_reg(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_reg_reg(x) instruction_##x##_reg_reg(call_argument_instruction)

#define __instruction_opcode_regoff_reg(x) uint8_t instruction_##x##_regoff_reg(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_regoff_reg(x) instruction_##x##_regoff_reg(call_argument_instruction)

#define __instruction_opcode_reg_regoff(x) uint8_t instruction_##x##_reg_regoff(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_reg_regoff(x) instruction_##x##_reg_regoff(call_argument_instruction)

#define __instruction_opcode_value_reg(x) uint8_t instruction_##x##_value_reg(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_value_reg(x) instruction_##x##_value_reg(call_argument_instruction)

#define __instruction_opcode_reg_value(x) uint8_t instruction_##x##_reg_value(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_reg_value(x) instruction_##x##_reg_value(call_argument_instruction)

#define __instruction_opcode_valueoff_reg(x) uint8_t instruction_##x##_valueoff_reg(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_valueoff_reg(x) instruction_##x##_valueoff_reg(call_argument_instruction)

#define __instruction_opcode_reg_valueoff(x) uint8_t instruction_##x##_reg_valueoff(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_reg_valueoff(x) instruction_##x##_reg_valueoff(call_argument_instruction)

#define __instruction_opcode_reg(x) uint8_t instruction_##x##_reg(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_reg(x) instruction_##x##_reg(call_argument_instruction)

#define __instruction_opcode_value(x) uint8_t instruction_##x##_value(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode_value(x) instruction_##x##_value(call_argument_instruction)

#define __instruction_opcode(x) uint8_t instruction_##x##(INSTRUCTION_SET_ARGS)
#define call_instruction_opcode(x) instruction_##x##(call_argument_instruction)

static h_text_compile instruction_maker_opcode_reg_reg[] = {
    {"mov",     OPCODE_MOV_1 ,   PREFIX_REG_INC, 0},
    {"movs",    OPCODE_MOVS_1,   PREFIX_REG_INC, 0},
    {"movd",    OPCODE_MOVD_1,   PREFIX_REG_INC, 0},
    {"add",     OPCODE_ADD_1,   PREFIX_REG_INC, 0},
    {"mul",     OPCODE_MUL_1,   PREFIX_REG_INC, 0},
    {"div",     OPCODE_DIV_1,   PREFIX_REG_INC, 0},
    {"sub",     OPCODE_SUB_1,   PREFIX_REG_INC, 0},
    {"and",     OPCODE_AND_1,   PREFIX_REG_INC, 0},
    {"or",      OPCODE_OR_1,    PREFIX_REG_INC, 0},
    {"nand",    OPCODE_NAND_1,  PREFIX_REG_INC, 0},
    {"nor",     OPCODE_NOR_1,   PREFIX_REG_INC, 0},
    {"xor",     OPCODE_XOR_1,   PREFIX_REG_INC, 0},
    {"lod",     OPCODE_LOD_1,   PREFIX_REG_INC, 0},
    {"lods",     OPCODE_LODS_1,   PREFIX_REG_INC, 0},
    {"lodd",     OPCODE_LODD_1,   PREFIX_REG_INC, 0},
    {"str",     OPCODE_STR_1,   PREFIX_REG_INC, 0},
    {"strs",     OPCODE_STRS_1,   PREFIX_REG_INC, 0},
    {"strd",     OPCODE_STRD_1,   PREFIX_REG_INC, 0},
    {"cnd",     OPCODE_CND_1,   PREFIX_REG_INC | PREFIX_SYM_INC, 0},
    {"sh",      OPCODE_SH_1,    PREFIX_REG_INC | PREFIX_SYM_INC, 0},
    {"in",      OPCODE_IN,      PREFIX_REG_REG, 0}
};

static h_text_compile instruction_maker_opcode_value_reg[] = {
    {"mov", OPCODE_MOV_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"movs", OPCODE_MOVS_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"movd", OPCODE_MOVD_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"add", OPCODE_ADD_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"mul", OPCODE_MUL_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"div", OPCODE_DIV_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"sub", OPCODE_SUB_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"and", OPCODE_AND_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"or",  OPCODE_OR_2,    PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"nand",OPCODE_NAND_2,  PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"nor", OPCODE_NOR_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"xor", OPCODE_XOR_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lod", OPCODE_LOD_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lods", OPCODE_LODS_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lodd", OPCODE_LODD_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"str", OPCODE_STR_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"strs", OPCODE_STRS_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"strd", OPCODE_STRD_2,   PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"cnd", OPCODE_CND_2,   PREFIX_REG_INC | PREFIX_SYM_INC, 0},
    {"in", OPCODE_IN,       PREFIX_VAL_REG, 0}
};

static h_text_compile instruction_maker_opcode_label_reg[] = {
    {"mov", OPCODE_MOV_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"movs", OPCODE_MOVS_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"movd", OPCODE_MOVD_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lod", OPCODE_LOD_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lods", OPCODE_LODS_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"lodd", OPCODE_LODD_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"str", OPCODE_STR_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"strs", OPCODE_STRS_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
    {"strd", OPCODE_STRD_2, PREFIX_REG_INC | PREFIX_VAL_INC, 0}
};

static h_text_compile instruction_maker_opcode_reg_value[] = {
    {"cnd", OPCODE_CND_3,   PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SYM_INC, 0},
    {"sh",  OPCODE_SH_2,    PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SYM_INC, 0},
};

static h_text_compile instruction_maker_opcode_reg_label[] = {
    {"mov", OPCODE_CND_3, PREFIX_REG_INC | PREFIX_VAL_INC, 0},
};

static h_text_compile instruction_maker_opcode_regoff_reg[] = {
    {"lod", OPCODE_LOD_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0},
    {"lods", OPCODE_LODS_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0},
    {"lodd", OPCODE_LODD_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0},
};

static h_text_compile instruction_maker_opcode_valueoff_reg[] = {
    {"lod", OPCODE_LOD_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"lods", OPCODE_LODS_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"lodd", OPCODE_LODD_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
};

static h_text_compile instruction_maker_opcode_labeloff_reg[] = {
    {"lod", OPCODE_LOD_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"lods", OPCODE_LODS_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"lodd", OPCODE_LODD_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
};

static h_text_compile instruction_maker_opcode_reg_regoff[] = {
    {"str", OPCODE_STR_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0},
    {"strs", OPCODE_STRS_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0},
    {"strd", OPCODE_STRD_1,   PREFIX_OFF_INC | PREFIX_REG_INC, 0}
};

static h_text_compile instruction_maker_opcode_reg_valueoff[] = {
    {"str", OPCODE_STR_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"strs", OPCODE_STRS_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"strd", OPCODE_STRD_2,   PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
};

static h_text_compile instruction_maker_opcode_reg_labeloff[] = {
    {"str", OPCODE_STR_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"strs", OPCODE_STRS_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
    {"strd", OPCODE_STRD_2, PREFIX_REG_INC | PREFIX_OFF_INC | PREFIX_VAL_INC, 0},
};


static h_text_compile instruction_maker_opcode_value[] = {
    {"push", OPCODE_PUSH_2, PREFIX_VAL_INC, 0},
    {"call", OPCODE_CALL_2, PREFIX_VAL_INC, 0},
    {"go", OPCODE_GO_2,     PREFIX_VAL_INC, 0},
    {"goc", OPCODE_GOC_2,   PREFIX_VAL_INC, 0},
    {"gonc", OPCODE_GONC, PREFIX_VAL_INC, 0},
    {"pnt", OPCODE_PNT_2,   PREFIX_VAL_INC, 0},
    {"pct", OPCODE_PCT_2,   PREFIX_VAL_INC, 0},
    {"int", OPCODE_INT, PREFIX_VAL_INC, 0}
};
static h_text_compile instruction_maker_opcode_label[] = {
    {"push", OPCODE_PUSH_2, PREFIX_VAL_INC, 0},
    {"call", OPCODE_CALL_2,     PREFIX_VAL_INC, 0},
    {"go", OPCODE_GO_2,         PREFIX_VAL_INC, 0},
    {"goc", OPCODE_GOC_2,       PREFIX_VAL_INC, 0},
    {"gonc", OPCODE_GONC,       PREFIX_VAL_INC, 0},
    {"pnt", OPCODE_PNT_2,   PREFIX_VAL_INC, 0},
    {"pct", OPCODE_PCT_2,   PREFIX_VAL_INC, 0},
};
static h_text_compile instruction_maker_opcode_reg[] = {
    {"push", OPCODE_PUSH_1,     PREFIX_REG_INC, 0},
    {"pop", OPCODE_POP_1,       PREFIX_REG_INC, 0},
    {"call", OPCODE_CALL_1,     PREFIX_REG_INC, 0},
    {"go", OPCODE_GO_1,         PREFIX_REG_INC, 0},
    {"goc", OPCODE_GOC_1,       PREFIX_REG_INC, 0},
    {"gonc", OPCODE_GONC,       PREFIX_REG_INC, 0},
    {"not", OPCODE_NOT,         PREFIX_REG_INC, 0},
    {"pnt", OPCODE_PNT_1,   PREFIX_REG_INC, 0},
    {"pct", OPCODE_PCT_1,   PREFIX_REG_INC, 0},
    {"int", OPCODE_INT, PREFIX_REG_INC, 0}
};
static h_text_compile instruction_maker_opcode[] = {
    {"ret", OPCODE_RETURN, 0, 0},
    {"stp", OPCODE_STP, 0, 0},
};
