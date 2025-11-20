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

typedef struct h_text_compile {
    const char* opcode_name;
    uint8_t opcode;
    uint8_t prefix;
    uint8_t ext_prefix;
} h_text_compile;

static h_text_compile instruction_maker_opcode_reg_reg[] = {
    {"mov",     OPCODE_MOV_1,   PREFIX_RM_INC, 0},
    {"add",     OPCODE_ADD_1,   PREFIX_RM_INC, 0},
    {"mul",     OPCODE_MUL_1,   PREFIX_RM_INC, 0},
    {"div",     OPCODE_DIV_1,   PREFIX_RM_INC, 0},
    {"sub",     OPCODE_SUB_1,   PREFIX_RM_INC, 0},
    {"and",     OPCODE_AND_1,   PREFIX_RM_INC, 0},
    {"or",      OPCODE_OR_1,    PREFIX_RM_INC, 0},
    {"nand",    OPCODE_NAND_1,  PREFIX_RM_INC, 0},
    {"nor",     OPCODE_NOR_1,   PREFIX_RM_INC, 0},
    {"xor",     OPCODE_XOR_1,   PREFIX_RM_INC, 0},
    {"lod",     OPCODE_LOD_1,   PREFIX_RM_INC | PREFIX_EXT_INC, 0},
    {"str",     OPCODE_STR_1,   PREFIX_RM_INC | PREFIX_EXT_INC, 0}
};

static h_text_compile instruction_maker_opcode_value_reg[] = {
    {"mov", OPCODE_MOV_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"add", OPCODE_ADD_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"mul", OPCODE_MUL_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"div", OPCODE_DIV_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"sub", OPCODE_SUB_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"and", OPCODE_AND_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"or",  OPCODE_OR_2,    PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"nand",OPCODE_NAND_2,  PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"nor", OPCODE_NOR_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"xor", OPCODE_XOR_2,   PREFIX_RM_INC | PREFIX_VAL_INC, 0},
    {"lod", OPCODE_LOD_2,   PREFIX_RM_INC | PREFIX_VAL_INC | PREFIX_EXT_INC, 0},
    {"str", OPCODE_STR_2,   PREFIX_RM_INC | PREFIX_VAL_INC | PREFIX_EXT_INC, 0}
};

static h_text_compile instruction_maker_opcode_label_reg[] = {
    {"mov", OPCODE_MOV_2, PREFIX_RM_INC | 0, 0},
    {"lod", OPCODE_LOD_2, PREFIX_RM_INC | PREFIX_EXT_INC, 0},
    {"str", OPCODE_STR_2, PREFIX_RM_INC | PREFIX_EXT_INC, 0}
};

static h_text_compile instruction_maker_opcode_regoff_reg[] = {
    {"lod", OPCODE_LOD_1,   PREFIX_OFF_INC | PREFIX_RM_INC | PREFIX_EXT_INC, 0},
};

static h_text_compile instruction_maker_opcode_valueoff_reg[] = {
    {"lod", OPCODE_LOD_2,   PREFIX_RM_INC | PREFIX_OFF_INC | PREFIX_VAL_INC | PREFIX_EXT_INC, 0},
};

static h_text_compile instruction_maker_opcode_labeloff_reg[] = {
    {"lod", OPCODE_LOD_2, PREFIX_RM_INC | PREFIX_OFF_INC | PREFIX_EXT_INC, 0},
};

static h_text_compile instruction_maker_opcode_reg_regoff[] = {
    {"str", OPCODE_STR_1,   PREFIX_OFF_INC | PREFIX_RM_INC | PREFIX_EXT_INC, 0}
};

static h_text_compile instruction_maker_opcode_reg_valueoff[] = {
    {"str", OPCODE_STR_2,   PREFIX_RM_INC | PREFIX_OFF_INC | PREFIX_VAL_INC | PREFIX_EXT_INC, 0}
};

static h_text_compile instruction_maker_opcode_reg_labeloff[] = {
    {"str", OPCODE_STR_2, PREFIX_RM_INC | PREFIX_OFF_INC | PREFIX_EXT_INC, 0}
};


static h_text_compile instruction_maker_opcode_value[] = {
    {"push", OPCODE_PUSH_2, PREFIX_VAL_INC, 0},
    {"call", OPCODE_CALL_2, PREFIX_VAL_INC, 0},
    {"go", OPCODE_GO_2,     PREFIX_VAL_INC, 0},
    {"goc", OPCODE_GOC_2,   PREFIX_VAL_INC, 0},
};
static h_text_compile instruction_maker_opcode_label[] = {
    {"push", OPCODE_PUSH_2, PREFIX_VAL_INC, 0},
    {"call", OPCODE_CALL_2,     PREFIX_VAL_INC, 0},
    {"go", OPCODE_GO_2,         PREFIX_VAL_INC, 0},
    {"goc", OPCODE_GOC_2,       PREFIX_VAL_INC, 0},
};
static h_text_compile instruction_maker_opcode_reg[] = {
    {"push", OPCODE_PUSH_1,     PREFIX_RM_INC, 0},
    {"pop", OPCODE_POP_1,       PREFIX_RM_INC, 0},
    {"call", OPCODE_CALL_1,     PREFIX_RM_INC, 0},
    {"go", OPCODE_GO_1,         PREFIX_RM_INC, 0},
    {"goc", OPCODE_GOC_1,       PREFIX_RM_INC, 0},
    {"not", OPCODE_NOT,         PREFIX_RM_INC, 0},
};
static h_text_compile instruction_maker_opcode[] = {
    {"ret", OPCODE_RETURN, 0, 0},
    {"stp", OPCODE_STP, 0, 0},
};

static h_text_compile instruction_maker_opcode_reg_sym_reg[] = {
    {"cnd", OPCODE_CND_1, PREFIX_RM_INC, 0 }
};

static h_text_compile instruction_maker_opcode_value_sym_reg[] = {
    {"cnd", OPCODE_CND_2, PREFIX_RM_INC | PREFIX_VAL_INC, 0}
};

static h_text_compile instruction_maker_opcode_reg_sym_value[] = {
    {"cnd", OPCODE_CND_2, PREFIX_RM_INC | PREFIX_VAL_INC, 0}
};