/**
 * ins_make.h
 * Created By Haidir
 */

#pragma once
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

// This define give string only
#define INS_OPCODE_REG_REG(op, rs, rd)              op + " " + rs + " -> " + rd
#define INS_OPCODE_REGOFF_REG(op, rs, off, rd)      op + " " + rs + "(" + off + ") -> " + rd
#define INS_OPCODE_REG_REGOFF(op, rs, rd, off)      op + " " + rs + " -> " + rd + "(" + off + ")"
#define INS_OPCODE_VALUE_REG(op, val, rd)           op + " " + std::to_string( val ) + " -> " + rd
#define INS_OPCODE_REG_VALUE(op, rs, val)           op + " " + rs + " -> " + val
#define INS_OPCODE_VALUEOFF_REG(op, val, off, rd)   op + " " + val + "(" + off + ") -> " + rd 
#define INS_OPCODE_REG_VALUEOFF(op, rs, val, off)   op + " " + rs + " -> " + val + "(" + off + ")"
#define INS_OPCODE_REG(op, r)                       op + " " + r
#define INS_OPCODE_VALUE(op, val)                   op + " " + val
#define INS_OPCODE(op)                              op

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
