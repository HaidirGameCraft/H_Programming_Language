/**
 * ins_make.h
 * Created By Haidir
 */

#pragma once

#define PATTERN_OPCODE_REG_REG                  "^(\\w+)\\s+([rR][a-z])\\s*(->|==|<=?|>=?|<<|>>)\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REGOFF_REG               "^(\\w+)\\s+([rR][a-z])(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*->\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REG_REGOFF               "^(\\w+)\\s+([rR][a-z])\\s*->\\s*([rR][a-z])(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*$"

#define PATTERN_OPCODE_VALUE_REG                "^(\\w+)\\s+(0x[[:xdigit:]]+|[[:digit:]]+)\\s*(->|==|<=?|>=?|<<|>>)\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REG_VALUE                "^(\\w+)\\s+([rR][a-z])\\s*(->|==|<=?|>=?|<<|>>)\\s*(0x[[:xdigit:]]+|[[:digit:]]+)\\s*$"
#define PATTERN_OPCODE_VALUEOFF_REG             "^(\\w+)\\s+(0x[[:xdigit:]]+|[[:digit:]]+)(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*->\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REG_VALUEOFF             "^(\\w+)\\s+([rR][a-z])\\s*->\\s*(0x[[:xdigit:]]+|[[:digit:]]+)(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*$"

#define PATTERN_OPCODE_LABEL_REG                "^(\\w+)\\s+(\\$\\w*.?\\w[^ ]+)\\s*(->|==|<=?|>=?|<<|>>)\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REG_LABEL                "^(\\w+)\\s+([rR][a-z])\\s*(->|==|<=?|>=?|<<|>>)\\s*(\\$\\w*.?\\w[^ ]+)\\s*$"
#define PATTERN_OPCODE_LABELOFF_REG             "^(\\w+)\\s+(\\$\\w*.?\\w*[^ ]+)(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*->\\s*([rR][a-z])\\s*$"
#define PATTERN_OPCODE_REG_LABELOFF             "^(\\w+)\\s+([rR][a-z])\\s*->\\s*(\\$\\w*.?\\w*[^ ]+)(?:\\(([-+]?0x[[:xdigit:]]+|[-+]?[[:digit:]]+)\\))\\s*$"

#define PATTERN_OPCODE_LABEL                    "^(\\w+)\\s+(\\$\\w*.?\\w*)$"
#define PATTERN_OPCODE_REG                      "^(\\w+)\\s+([rR][a-z]+)$"
#define PATTERN_OPCODE_VALUE                    "^(\\w+)\\s+(0x[0-9a-fA-F]+|[0-9]+)$"
#define PATTERN_OPCODE_ONLY                     "^(\\w+)$"
#define PATTERN_DEFINED_LABEL                   "^(.?\\w+):$"

#define PATTERN_REGION_NAME                     "^region\\s+(\\w+)$"

#include <regex>
#include <stdint.h>


std::smatch* cpp_regex_matches(std::string pattern, std::string text);

/**
 * Tools
 */
uint8_t read_memtype(const char* memtype);
uint8_t read_symbols_cnd(const char* _sym);
uint32_t     read_number(const char* _sym);

uint32_t instruction_reg_reg    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint8_t sym);
uint32_t instruction_value_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint8_t sym, uint32_t value);
uint32_t instruction_label_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, uint8_t sym, uint32_t value);
uint32_t instruction_reg_value  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t sym, uint32_t value);
uint32_t instruction_reg_label  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t sym, uint32_t value);

uint32_t instruction_regoff_reg    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg   , short offset);
uint32_t instruction_valueoff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, short offset, uint32_t value );
uint32_t instruction_labeloff_reg  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg, short offset, uint32_t value );

uint32_t instruction_reg_regoff    (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg   , short offset);
uint32_t instruction_reg_valueoff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, short offset, uint32_t value );
uint32_t instruction_reg_labeloff  (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t lReg, short offset, uint32_t value );

uint32_t instruction_reg        (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t rReg);
uint32_t instruction_value      (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint32_t value );
uint32_t instruction_label      (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode);
uint32_t instruction_opcode     (uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t ext_prefix, uint8_t opcode);

/**
 * Instruction Set Created 
*/
#define ARGUMENST_CREATED_INSTRUCTION   uint8_t memory, uint32_t pc, \
                                        uint8_t prefix, \
                                        uint8_t ext_prefix, \
                                        uint8_t opcode, \
                                        uint8_t registers, \
                                        uint8_t symbols, \
                                        uint32_t offset, \
                                        uint32_t value
#define CREATE_INSTRUCTION(x) uint32_t create_instruction_##x( ARGUMENST_CREATED_INSTRUCTION )

CREATE_INSTRUCTION(add_reg_reg);
CREATE_INSTRUCTION(add_value_reg);

CREATE_INSTRUCTION(and_reg_reg);
CREATE_INSTRUCTION(and_value_reg);

CREATE_INSTRUCTION(call_reg);
CREATE_INSTRUCTION(call_value);
CREATE_INSTRUCTION(call_label);

CREATE_INSTRUCTION(cnd_reg_reg);
CREATE_INSTRUCTION(cnd_value_reg);

CREATE_INSTRUCTION(div_reg_reg);
CREATE_INSTRUCTION(div_value_reg);

CREATE_INSTRUCTION(go_reg);
CREATE_INSTRUCTION(go_value);
CREATE_INSTRUCTION(go_label);

CREATE_INSTRUCTION(goc_reg);
CREATE_INSTRUCTION(goc_value);
CREATE_INSTRUCTION(goc_label);

CREATE_INSTRUCTION(lod_reg_reg);
CREATE_INSTRUCTION(lod_value_reg);
CREATE_INSTRUCTION(lod_label_reg);

CREATE_INSTRUCTION(mov_reg_reg);
CREATE_INSTRUCTION(mov_value_reg);
CREATE_INSTRUCTION(mov_label_reg);

CREATE_INSTRUCTION(mul_reg_reg);
CREATE_INSTRUCTION(mul_value_reg);

CREATE_INSTRUCTION(nand_reg_reg);
CREATE_INSTRUCTION(nand_value_reg);

CREATE_INSTRUCTION(nor_reg_reg);
CREATE_INSTRUCTION(nor_value_reg);

CREATE_INSTRUCTION(not_reg);

CREATE_INSTRUCTION(or_reg_reg);
CREATE_INSTRUCTION(or_value_reg);

CREATE_INSTRUCTION(pop_reg);

CREATE_INSTRUCTION(push_reg);
CREATE_INSTRUCTION(push_value);
CREATE_INSTRUCTION(push_label);

CREATE_INSTRUCTION(ret);

CREATE_INSTRUCTION(stp);

CREATE_INSTRUCTION(str_reg_reg);
CREATE_INSTRUCTION(str_reg_value);
CREATE_INSTRUCTION(str_reg_label);

CREATE_INSTRUCTION(sub_reg_reg);
CREATE_INSTRUCTION(sub_value_reg);

CREATE_INSTRUCTION(xor_reg_reg);
CREATE_INSTRUCTION(xor_value_reg);
