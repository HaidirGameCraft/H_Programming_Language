#pragma once

#include <stdint.h>
#include <inst.h>

#ifdef HVM
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
#include <ins/cmp.h>
#endif


#define PREFIX_REG_REG                 ( PREFIX_REG_INC )
#define PREFIX_REGOFF_REG              ( PREFIX_REG_INC | PREFIX_OFF_INC )
#define PREFIX_REG_REGOFF              ( PREFIX_REGOFF_REG | PREFIX_SWP_INC )
#define PREFIX_VAL_REG                 ( PREFIX_REG_INC | PREFIX_VAL_INC )
#define PREFIX_REG_VAL                 ( PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SWP_INC )
#define PREFIX_VALOFF_REG              ( PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_OFF_INC )
#define PREFIX_REG_VALOFF              ( PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_OFF_INC | PREFIX_SWP_INC )
#define PREFIX_LBL_REG                   PREFIX_VAL_REG
#define PREFIX_LBLOFF_REG                PREFIX_VALOFF_REG
#define PREFIX_REG_LBLOFF                PREFIX_REG_VALOFF
#define PREFIX_OPCODE_ONLY               0

enum {
    ISR_Opcode,
    ISR_Opcode_Reg_Reg,
    ISR_Opcode_RegOff_Reg,
    ISR_Opcode_Reg_RegOff,
    ISR_Opcode_Value_Reg,
    ISR_Opcode_Value_Value,
    ISR_Opcode_Value_RegOff,
    ISR_Opcode_ValueOff_Reg,
    ISR_Opcode_Value_ValueOff,
    ISR_Opcode_Reg_ValueOff,
    ISR_Opcode_RegOff_Value,
    ISR_Opcode_Reg_Value,
    ISR_Opcode_RegOff_RegOff,
    ISR_Opcode_ValueOff_ValueOff,
    ISR_Opcode_Reg,
    ISR_Opcode_Value,
    ISR_Label,
    ISR_Config
};

typedef struct h_text_compile {
    union {
        const char* opcode_name;
        const char* name;
    };
    uint8_t opcode;
    uint8_t prefix;
    uint8_t ext_prefix;
    int issr;
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
    {"mov",     OPCODE_MOV_1 ,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"movs",    OPCODE_MOVS_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"movd",    OPCODE_MOVD_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"add",     OPCODE_ADD_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"cmp",     OPCODE_CMP,     PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"mul",     OPCODE_MUL_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"div",     OPCODE_DIV_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"sub",     OPCODE_SUB_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"and",     OPCODE_AND_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"or",      OPCODE_OR_1,    PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"nand",    OPCODE_NAND_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"nor",     OPCODE_NOR_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"xor",     OPCODE_XOR_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"lod",     OPCODE_LOD_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"lods",    OPCODE_LODS_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"lodd",    OPCODE_LODD_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"str",     OPCODE_STR_1,   PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"strs",    OPCODE_STRS_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"strd",    OPCODE_STRD_1,  PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg},
    {"cnd",     OPCODE_CND_1,   PREFIX_REG_REG | PREFIX_SYM_INC, 0, ISR_Opcode_Reg_Reg},
    {"sh",      OPCODE_SH_1,    PREFIX_REG_REG | PREFIX_SYM_INC, 0, ISR_Opcode_Reg_Reg},
    {"in",      OPCODE_IN,      PREFIX_REG_REG, 0, ISR_Opcode_Reg_Reg }
};

static h_text_compile instruction_maker_opcode_value_reg[] = {
    {"mov",     OPCODE_MOV_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"movs",    OPCODE_MOVS_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"movd",    OPCODE_MOVD_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"add",     OPCODE_ADD_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"cmp",     OPCODE_CMP,     PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"mul",     OPCODE_MUL_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"div",     OPCODE_DIV_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"sub",     OPCODE_SUB_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"and",     OPCODE_AND_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"or",      OPCODE_OR_2,    PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"nand",    OPCODE_NAND_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"nor",     OPCODE_NOR_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"xor",     OPCODE_XOR_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"lod",     OPCODE_LOD_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"lods",    OPCODE_LODS_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"lodd",    OPCODE_LODD_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"str",     OPCODE_STR_2,   PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"strs",    OPCODE_STRS_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"strd",    OPCODE_STRD_2,  PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg },
    {"cnd",     OPCODE_CND_2,   PREFIX_VAL_REG | PREFIX_SYM_INC, 0, ISR_Opcode_Value_Reg},
    {"in",      OPCODE_IN,      PREFIX_VAL_REG, 0, ISR_Opcode_Value_Reg}
};

static h_text_compile instruction_maker_opcode_label_reg[] = {
    {"mov",     OPCODE_MOV_2,  PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"movs",    OPCODE_MOVS_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"movd",    OPCODE_MOVD_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"lod",     OPCODE_LOD_2,  PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"lods",    OPCODE_LODS_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"lodd",    OPCODE_LODD_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"str",     OPCODE_STR_2,  PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"strs",    OPCODE_STRS_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg},
    {"strd",    OPCODE_STRD_2, PREFIX_LBL_REG, 0, ISR_Opcode_Value_Reg}
};

static h_text_compile instruction_maker_opcode_reg_value[] = {
    {"cmp",     OPCODE_CMP,     PREFIX_REG_VAL, 0, ISR_Opcode_Reg_Value},
    {"cnd",     OPCODE_CND_3,   PREFIX_REG_VAL | PREFIX_SYM_INC, 0, ISR_Opcode_Reg_Value},
    {"sh",      OPCODE_SH_2,    PREFIX_REG_VAL | PREFIX_SYM_INC, 0, ISR_Opcode_Reg_Value},
};

static h_text_compile instruction_maker_opcode_reg_label[] = {
    {"mov",     OPCODE_CND_3,   PREFIX_REG_VAL, 0, ISR_Opcode_Reg_Value},
};

static h_text_compile instruction_maker_opcode_regoff_reg[] = {
    {"lod",     OPCODE_LOD_1,    PREFIX_REGOFF_REG, 0, ISR_Opcode_RegOff_Reg},
    {"lods",    OPCODE_LODS_1,   PREFIX_REGOFF_REG, 0, ISR_Opcode_RegOff_Reg},
    {"lodd",    OPCODE_LODD_1,   PREFIX_REGOFF_REG, 0, ISR_Opcode_RegOff_Reg},
};

static h_text_compile instruction_maker_opcode_valueoff_reg[] = {
    {"lod",     OPCODE_LOD_2,    PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
    {"lods",    OPCODE_LODS_2,   PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
    {"lodd",    OPCODE_LODD_2,   PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
};

static h_text_compile instruction_maker_opcode_labeloff_reg[] = {
    {"lod",     OPCODE_LOD_2,   PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
    {"lods",    OPCODE_LODS_2,  PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
    {"lodd",    OPCODE_LODD_2,  PREFIX_VALOFF_REG, 0, ISR_Opcode_ValueOff_Reg},
};

static h_text_compile instruction_maker_opcode_reg_regoff[] = {
    {"str",     OPCODE_STR_1,    PREFIX_REG_REGOFF, 0, ISR_Opcode_Reg_RegOff},
    {"strs",    OPCODE_STRS_1,   PREFIX_REG_REGOFF, 0, ISR_Opcode_Reg_RegOff},
    {"strd",    OPCODE_STRD_1,   PREFIX_REG_REGOFF, 0, ISR_Opcode_Reg_RegOff}
};

static h_text_compile instruction_maker_opcode_reg_valueoff[] = {
    {"str",     OPCODE_STR_2,    PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
    {"strs",    OPCODE_STRS_2,   PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
    {"strd",    OPCODE_STRD_2,   PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
};

static h_text_compile instruction_maker_opcode_reg_labeloff[] = {
    {"str",     OPCODE_STR_2,  PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
    {"strs",    OPCODE_STRS_2, PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
    {"strd",    OPCODE_STRD_2, PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
};

// static h_text_compile instruction_maker_opcode_reg_valueoff[] = {
//     {"str",     OPCODE_STR_2,    PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
//     {"strs",    OPCODE_STRS_2,   PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
//     {"strd",    OPCODE_STRD_2,   PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
// };

// static h_text_compile instruction_maker_opcode_reg_labeloff[] = {
//     {"str",     OPCODE_STR_2,  PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
//     {"strs",    OPCODE_STRS_2, PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
//     {"strd",    OPCODE_STRD_2, PREFIX_REG_VALOFF, 0, ISR_Opcode_Reg_ValueOff},
// };


static h_text_compile instruction_maker_opcode_value[] = {
    {"push",    OPCODE_PUSH_2,  PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"call",    OPCODE_CALL_2,  PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"go",      OPCODE_GO_2,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gol",     OPCODE_GOL,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gole",    OPCODE_GOLE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gog",     OPCODE_GOG,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"goge",    OPCODE_GOGE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"goe",     OPCODE_GOE,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gone",    OPCODE_GONE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"pnt",     OPCODE_PNT_2,   PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"pct",     OPCODE_PCT_2,   PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"int",     OPCODE_INT,     PREFIX_VAL_INC, 0, ISR_Opcode_Value}
};
static h_text_compile instruction_maker_opcode_label[] = {
    {"push",    OPCODE_PUSH_2,  PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"call",    OPCODE_CALL_2,  PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"go",      OPCODE_GO_2,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gol",     OPCODE_GOL,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gole",    OPCODE_GOLE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gog",     OPCODE_GOG,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"goge",    OPCODE_GOGE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"goe",     OPCODE_GOE,     PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"gone",    OPCODE_GONE,    PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"pnt",     OPCODE_PNT_2,   PREFIX_VAL_INC, 0, ISR_Opcode_Value},
    {"pct",     OPCODE_PCT_2,   PREFIX_VAL_INC, 0, ISR_Opcode_Value},
};
static h_text_compile instruction_maker_opcode_reg[] = {
    {"push",    OPCODE_PUSH_1,  PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"pop",     OPCODE_POP_1,   PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"call",    OPCODE_CALL_1,  PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"go",      OPCODE_GO_1,    PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"gol",     OPCODE_GOL,     PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"gole",    OPCODE_GOLE,    PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"gog",     OPCODE_GOG,     PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"goge",    OPCODE_GOGE,    PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"goe",     OPCODE_GOE,     PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"gone",    OPCODE_GONE,    PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"not",     OPCODE_NOT,     PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"pnt",     OPCODE_PNT_1,   PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"pct",     OPCODE_PCT_1,   PREFIX_REG_INC, 0, ISR_Opcode_Reg},
    {"int",     OPCODE_INT,     PREFIX_REG_INC, 0, ISR_Opcode_Reg}
};
static h_text_compile instruction_maker_opcode[] = {
    {"ret",     OPCODE_RETURN,  0, 0, ISR_Opcode},
    {"stp",     OPCODE_STP,     0, 0, ISR_Opcode},
};
