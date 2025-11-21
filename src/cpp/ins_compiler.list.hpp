#pragma once

#include <regex>
#include <stdint.h>
#include <string>
#include <string.h>
#include <cpp/ins_make.hpp>
#include <ins/inst.h>

#define REGEX_START(pattern)        std::regex regex( pattern );
#define REGEX_SEARCH(text)          std::smatch match; \
                                    if( std::regex_match(text, regex) == false ) return 0; \
                                    bool status = std::regex_search(text, match, regex); \
                                    if( status == false ) { \
                                        return 0; \
                                    }
#define INITIALIZE_VARIABLE         char opcode[8] = {0}; \
                                    char label[64] = {0}; \
\
                                    uint8_t prefix = 0; \
                                    uint8_t lReg = 0; \
                                    uint8_t rReg = 0; \
                                    uint8_t symbols = 0; \
\
                                    short offset = 0; \
                                    uint32_t value = 0;

#define ARGUMENT_PATTERN    uint8_t* memory, uint32_t *pc, std::string text
#define PATTERN_FINDER(x) int x##_finder_instruction(ARGUMENT_PATTERN)

PATTERN_FINDER(opcode_reg_reg);
PATTERN_FINDER(opcode_value_reg);
PATTERN_FINDER(opcode_label_reg);
PATTERN_FINDER(opcode_reg_value);
PATTERN_FINDER(opcode_reg_label);
PATTERN_FINDER(opcode_regoff_reg);
PATTERN_FINDER(opcode_valueoff_reg);
PATTERN_FINDER(opcode_labeloff_reg);
PATTERN_FINDER(opcode_reg_regoff);
PATTERN_FINDER(opcode_reg_valueoff);
PATTERN_FINDER(opcode_reg_labeloff);
PATTERN_FINDER(opcode_reg);
PATTERN_FINDER(opcode_label);
PATTERN_FINDER(opcode_value);
PATTERN_FINDER(opcode_only);
PATTERN_FINDER(defined_label);