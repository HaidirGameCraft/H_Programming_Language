#pragma once


#include <stdint.h>
#include <inst.h>
// instruction set rule
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

struct InstructionSet {
    const char* name;
    uint8_t prefix;
    uint8_t ext_prefix;
    uint8_t opcode;
    int issr;
};

static InstructionSet instruction_set_rr_list[] = {
    // opcode reg -> reg
    {"mov", PREFIX_REG_INC, 0, OPCODE_MOV_1, ISR_Opcode_Reg_Reg},
    {"movs", PREFIX_REG_INC, 0, OPCODE_MOVS_1, ISR_Opcode_Reg_Reg},
    {"movd", PREFIX_REG_INC, 0, OPCODE_MOVD_1, ISR_Opcode_Reg_Reg},
    {"add", PREFIX_REG_INC, 0, OPCODE_ADD_1, ISR_Opcode_Reg_Reg},
    {"mul", PREFIX_REG_INC, 0, OPCODE_MUL_1, ISR_Opcode_Reg_Reg},
    {"div", PREFIX_REG_INC, 0, OPCODE_DIV_1, ISR_Opcode_Reg_Reg},
    {"sub", PREFIX_REG_INC, 0, OPCODE_SUB_1, ISR_Opcode_Reg_Reg},
    {"and", PREFIX_REG_INC, 0, OPCODE_AND_1, ISR_Opcode_Reg_Reg},
    {"or", PREFIX_REG_INC, 0, OPCODE_OR_1, ISR_Opcode_Reg_Reg},
    {"nand", PREFIX_REG_INC, 0, OPCODE_NAND_1, ISR_Opcode_Reg_Reg},
    {"nor", PREFIX_REG_INC, 0, OPCODE_NOR_1, ISR_Opcode_Reg_Reg},
    {"xor", PREFIX_REG_INC, 0, OPCODE_XOR_1, ISR_Opcode_Reg_Reg},
    {"lod", PREFIX_REG_INC, 0, OPCODE_LOD_1, ISR_Opcode_Reg_Reg},
    {"lods", PREFIX_REG_INC, 0, OPCODE_LODS_1, ISR_Opcode_Reg_Reg},
    {"lodd", PREFIX_REG_INC, 0, OPCODE_LODD_1, ISR_Opcode_Reg_Reg},
    {"str", PREFIX_REG_INC, 0, OPCODE_STR_1, ISR_Opcode_Reg_Reg},
    {"strs", PREFIX_REG_INC, 0, OPCODE_STRS_1, ISR_Opcode_Reg_Reg},
    {"strd", PREFIX_REG_INC, 0, OPCODE_STRD_1, ISR_Opcode_Reg_Reg},
    {"cnd", PREFIX_REG_INC | PREFIX_SYM_INC, 0, OPCODE_CND_1, ISR_Opcode_Reg_Reg},
    {"sh", PREFIX_REG_INC | PREFIX_SYM_INC, 0, OPCODE_SH_1, ISR_Opcode_Reg_Reg},
    {"in", PREFIX_REG_INC, 0, OPCODE_IN, ISR_Opcode_Reg_Reg},
};
static InstructionSet instruction_set_vr_list[] = {
    // opcode value -> reg
    {"mov", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_MOV_2, ISR_Opcode_Value_Reg},
    {"movs", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_MOVS_2, ISR_Opcode_Value_Reg},
    {"movd", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_MOVD_2, ISR_Opcode_Value_Reg},
    {"add", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_ADD_2, ISR_Opcode_Value_Reg},
    {"mul", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_MUL_2, ISR_Opcode_Value_Reg},
    {"div", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_DIV_2, ISR_Opcode_Value_Reg},
    {"sub", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_SUB_2, ISR_Opcode_Value_Reg},
    {"and", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_AND_2, ISR_Opcode_Value_Reg},
    {"or", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_OR_2, ISR_Opcode_Value_Reg},
    {"nand", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_NAND_2, ISR_Opcode_Value_Reg},
    {"nor", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_NOR_2, ISR_Opcode_Value_Reg},
    {"xor", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_XOR_2, ISR_Opcode_Value_Reg},
    {"lod", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_LOD_2, ISR_Opcode_Value_Reg},
    {"lods", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_LODS_2, ISR_Opcode_Value_Reg},
    {"lodd", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_LODD_2, ISR_Opcode_Value_Reg},
    {"str", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_STR_2, ISR_Opcode_Value_Reg},
    {"strs", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_STRS_2, ISR_Opcode_Value_Reg},
    {"strd", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_STRD_2, ISR_Opcode_Value_Reg},
    {"cnd", PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SYM_INC, 0, OPCODE_CND_2, ISR_Opcode_Value_Reg},
    {"in", PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_IN, ISR_Opcode_Value_Reg},
};

static InstructionSet instruction_set_rv_list[] = {
    {"cnd", PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SYM_INC, 0, OPCODE_CND_3, ISR_Opcode_Reg_Value},
    {"sh", PREFIX_REG_INC | PREFIX_VAL_INC | PREFIX_SYM_INC, 0, OPCODE_SH_2, ISR_Opcode_Reg_Value},
};

static InstructionSet instruction_set_rfr_list[] = {
    {"lod", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_LOD_1},
    {"lods", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_LODS_1},
    {"lodd", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_LODD_1},
};

static InstructionSet instruction_set_vfr_list[] = { 
    {"lod", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_LOD_2},
    {"lods", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC , 0, OPCODE_LODS_2},
    {"lodd", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC, 0, OPCODE_LODD_2},
};

static InstructionSet instruction_set_rrf_list[] = {
    {"str", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_STR_1 },
    {"strs", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_STRS_1 },
    {"strd", PREFIX_OFF_INC | PREFIX_REG_INC, 0, OPCODE_STRD_1 },
};

static InstructionSet instruction_set_rvf_list[] = {
    {"str", PREFIX_OFF_INC | PREFIX_REG_INC  | PREFIX_VAL_INC , 0, OPCODE_STR_2 },
    {"strs", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC , 0, OPCODE_STRS_2 },
    {"strd", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC , 0, OPCODE_STRD_2 },
};

static InstructionSet instruction_set_vrf_list[] = {
    {"str", PREFIX_OFF_INC | PREFIX_REG_INC  | PREFIX_VAL_INC , 0, OPCODE_STR_2 },
    {"strs", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC , 0, OPCODE_STRS_2 },
    {"strd", PREFIX_OFF_INC | PREFIX_REG_INC | PREFIX_VAL_INC , 0, OPCODE_STRD_2 },
};

static InstructionSet instruction_set_v_list[] = {
    {"push", PREFIX_VAL_INC, 0, OPCODE_PUSH_2},
    {"call", PREFIX_VAL_INC, 0, OPCODE_CALL_2},
    {"go", PREFIX_VAL_INC, 0, OPCODE_GO_2},
    {"goc", PREFIX_VAL_INC, 0, OPCODE_GOC_2},
    {"gonc", PREFIX_VAL_INC, 0, OPCODE_GONC},
    {"pnt", PREFIX_VAL_INC, 0, OPCODE_PNT_2},
    {"pct", PREFIX_VAL_INC, 0, OPCODE_PCT_2},
    {"int", PREFIX_VAL_INC, 0, OPCODE_INT},
};

static InstructionSet instruction_set_r_list[] = { 
    {"push", PREFIX_REG_INC, 0, OPCODE_PUSH_1},
    {"pop", PREFIX_REG_INC, 0, OPCODE_POP_1},
    {"call", PREFIX_REG_INC, 0, OPCODE_CALL_1},
    {"go", PREFIX_REG_INC, 0, OPCODE_GO_1},
    {"goc", PREFIX_REG_INC, 0, OPCODE_GOC_1},
    {"gonc", PREFIX_REG_INC, 0, OPCODE_GONC},
    {"not", PREFIX_REG_INC, 0, OPCODE_NOT},
    {"pnt", PREFIX_REG_INC, 0, OPCODE_PNT_1},
    {"pct", PREFIX_REG_INC, 0, OPCODE_PCT_1},
    {"int", PREFIX_REG_INC, 0, OPCODE_INT},
};

static InstructionSet instruction_set_op_list[] = {
    {"ret", 0, 0, OPCODE_RETURN},
    {"stp", 0, 0, OPCODE_STP}
};















