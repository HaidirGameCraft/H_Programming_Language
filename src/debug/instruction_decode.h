#pragma once

#include <inst.h>

char* instruction_decode( uint8_t* memory, uint32_t pc, uint8_t size );
const char* decode_register( uint8_t reg );
#define ARGUMENTS_DECODE_INSTRUCTION uint8_t prefix, uint8_t ext_prefix, uint8_t opcode, uint8_t* memory, uint32_t pc
char* decode_instruction_opcode_reg_reg      (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_regoff_reg   (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg_regoff   (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_value_reg    (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg_value    (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_valueoff_reg (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg_valueoff (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_label_reg    (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg_label    (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_labeloff_reg (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg_labeloff (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_reg          (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_value        (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode_label        (ARGUMENTS_DECODE_INSTRUCTION);
char* decode_instruction_opcode              (ARGUMENTS_DECODE_INSTRUCTION);
