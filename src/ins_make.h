/**
 * ins_make.h
 * Created By Haidir
 */

#pragma once

// #define PATTERN_OPCODE_LABEL_AS_REG             "^([a-zA-Z]+)\\s+(\\(\\$[\\w]*\\))\\s+as\\s+(single|double|quad)\\s*->\\s*([r|R][0-9a-zA-Z]+)$"
// #define PATTERN_OPCODE_REG_REG                  "^([a-zA-Z]+)\\s+([r|R][0-9a-zA-Z]+)\\s*->\\s*([r|R][0-9a-zA-Z]+)$"

// #define PATTERN_OPCODE_REG_MEMORY_REG           "^([a-zA-Z]+)\\s+\\(([rR][0-9a-zA-Z]+)\\s*(\\+)?\\s*(0x[0-9a-fA-F]+|[0-9]+)?\\)\\s*(?:as\\s+(single|double|quad))?\\s*->\\s*([rR][0-9a-zA-Z]+)$"
// #define PATTERN_OPCODE_REG_REG_MEMORY           "^([a-zA-Z]+)\\s+([r|R][0-9a-zA-Z]+)\\s*->\\s*\\(([rR][0-9a-zA-Z]+)\\s*(\\+)?\\s*(0x[0-9a-fA-F]+|[0-9]+)?\\)$"

// #define PATTERN_OPCODE_VALUE_REG_MEMORY         "^([a-zA-Z]+)\\s+(0x[0-9a-fA-F]+|[0-9]+)\\s*->\\s*\\(([r|R][0-9a-zA-Z]+)\\s*\\+\\s*(0x[0-9a-fA-F]+|[0-9]+)\\)$"

// #define PATTERN_OPCODE_VALUE_REG                "^([a-zA-Z]+)\\s+(0x[0-9a-fA-F]+|[0-9]+)\\s*->\\s*([r|R][0-9a-zA-Z]+)$"

// #define PATTERN_OPCODE_LABEL_REG                "^([a-zA-Z]+)\\s+(\\$\\w*)\\s*->\\s*([r|R][0-9a-zA-Z]+)$"
// #define PATTERN_OPCODE_LABEL                    "^([a-zA-Z]+)\\s+(\\$\\w*)$"

// #define PATTERN_OPCODE_REG                      "^(\\w+)\\s+([r|R][0-9]*[a,b,c,d,e,f,g,h,s,bp]+)$"
// #define PATTERN_OPCODE_REG_MEMORY               "^(\\w+)\\s+\\(([r|R][0-9]*[a,b,c,d,e,f,g,h,s,bp]+)\\s+\\+\\s+(0x[0-9a-fA-F]+|[0-9]+)\\)$"

// #define PATTERN_OPCODE_ONLY                     "^([a-zA-Z]+)$"

// #define PATTERN_OPCODE_VALUE                    "^(\\w+)\\s+(0x[0-9a-fA-F]+|[0-9]+)$"
// #define PATTERN_DEFINED_LABEL                   "^(\\w*):$"

// #define PATTERN_OPCODE_ADDR_AS_REG              "^([a-zA-Z]+)\\s+\\(\\s*(0x[0-9a-fA-F]+|[0-9]+)\\)\\s+as\\+(single|double|quad)\\s*->\\s*([r|R][0-9a-zA-Z]+)$"
// #define PATTERN_OPCODE_REG_ADDR_AS              "^([a-zA-Z]+)\\s+\\(\\s*(0x[0-9a-fA-F]+|[0-9]+)\\)\\s+as\\+(single|double|quad)\\s*->\\s*([r|R][0-9a-zA-Z]+)\\s+as\\+(single|double|quad)$"
// #define PATTERN_OPCODE_VALUE_ADDR_AS            "^([a-zA-Z]+)\\s+(0x[0-9a-fA-F]+|[0-9]+)\\s*->\\s*\\((0x[0-9a-fA-F]+|[0-9]+)\\)\\s+as\\+(single|double|quad)$"

// #define PATTERN_OPCODE_REG_SYM_REG              "^(\\w+)\\s+([rR][0-9a-zA-Z]+)\\s*(==|<=?|>=?)\\s*([rR][0-9a-zA-Z]+)$"
// #define PATTERN_OPCODE_REG_MEMORY_SYM_REG       "^(\\w+)\\s+\\(\\s*([rR]\\d+\\w+)\\s*(\\+)?\\s*(0x[0-9a-fA-F]+|(\\d+))?\\s*\\)\\s*(==|<=?|>=?)\\s*([rR]\\d+\\w+)\\s*$"
// #define PATTERN_OPCODE_REG_SYM_REG_MEMORY       "^(\\w+)\\s+([rR]\\d+\\w+)\\s*(==|<=?|>=?)\\s*\\(\\s*([rR]\\d+\\w+)\\s*(\\+)?\\s*(0x[0-9a-fA-F]+|(\\d+))?\\s*\\)\\s*$"
// #define PATTERN_OPCODE_VALUE_SYM_REG            "^(\\w+)\\s+(0x[0-9a-fA-F]+|\\d+)\\s*(==|<=?|>=?)\\s*([rR]\\d+[a-z][p]?)\\s*$"

//#include <regex.h>
#include <stdint.h>

// typedef struct label_name_addr {
//     char* label_name;
//     uint32_t address;
//     uint32_t destination;
//     struct label_name_addr* next;
// } label_name_addr ;

//typedef struct regex_matches_t {
//   regmatch_t* match;
//    int length;
//} regex_matches_t;

/**
 * regex matches
 * @return when (NULL) -> No Match, otherwise address of regex_matches_t
 */

//regex_matches_t* regex_matches(const char* pattern, const char* text);
// void             free_regex_matches(regex_matches_t* ptr);

void new_label_name(const char* name, int size, uint32_t dest, uint32_t address, uint8_t make_child);
void push_label(const char* name, int size, uint32_t dest);
void update_label(const char* name, int size, uint32_t address );
void list_label_instruction();
void update_all_instruction_label( uint8_t* memory );
void clear_label();

uint8_t regbytext(const char* reg_text);

uint32_t instruction_reg_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg);
uint32_t instruction_regmem_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint32_t offset);
uint32_t instruction_regmem_as_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t memtype, uint8_t rReg, uint32_t offset);
uint32_t instruction_reg_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint32_t offset);
uint32_t instruction_reg_regmem_as(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint8_t memtype, uint32_t offset);
uint32_t instruction_label_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t value);
uint32_t instruction_value_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t value);
uint32_t instruction_value_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t offset, uint32_t value);
uint32_t instruction_value_regmem_as(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint8_t memtype, uint32_t offset, uint32_t value);
uint32_t instruction_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg);
uint32_t instruction_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t offset);
uint32_t instruction_regmem_as(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint8_t memtype, uint32_t offset);
uint32_t instruction_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint32_t value );
uint32_t instruction_label(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode);

// using symbols in middle
uint32_t instruction_reg_sym_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t sym, uint8_t lreg, uint8_t rreg);
uint32_t instruction_regmem_sym_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t sym, uint8_t lreg, uint8_t rreg, int offset);
uint32_t instruction_reg_sym_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t sym, uint8_t lreg, uint8_t rreg, int offset);
uint32_t instruction_value_sym_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t sym, uint8_t rreg, uint32_t value);
uint32_t instruction_reg_sym_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t sym, uint8_t lreg, uint32_t value );
