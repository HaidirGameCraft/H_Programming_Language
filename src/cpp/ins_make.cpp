/**
 * ins_make.c
 * Created By Haidir
 */
#include <stdint.h>
#include <inst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asrt.h>
#include "ins_make.hpp"
#include <stdbool.h>
#include <iostream>
#include <tools/tools.h>

#include <data/ascii.h>
#include <data/datatype.h>

#include <ins/inst.h>
#include <cpp/ins_compiler.list.hpp>

char* instruction_text = NULL;
int line_of_text = 0;

#define ASSERT_WRONG_REGISTER(x) printf("%s:%i -> Unknown/Wrong Register ( %s )", instruction_text, line_of_text, x); \
                                 ASSERT("unknown of wrong register");

uint8_t regbytext(const char* reg_text) {
    // 32 register
    if( strcmp(reg_text, "ra") == 0 || strcmp(reg_text, "RA") == 0)
        return  0b0000;
    else if( strcmp(reg_text, "rb") == 0 || strcmp(reg_text, "RB") == 0)
        return  0b0001;
    else if( strcmp(reg_text, "rc") == 0 || strcmp(reg_text, "RC") == 0)
        return  0b0010;
    else if( strcmp(reg_text, "rd") == 0 || strcmp(reg_text, "RD") == 0)
        return  0b0011;
    else if( strcmp(reg_text, "re") == 0 || strcmp(reg_text, "RE") == 0)
        return  0b0100;
    else if( strcmp(reg_text, "rf") == 0 || strcmp(reg_text, "RF") == 0)
        return  0b0101;
    else if( strcmp(reg_text, "rg") == 0 || strcmp(reg_text, "RG") == 0)
        return  0b0110;
    else if( strcmp(reg_text, "rh") == 0 || strcmp(reg_text, "RH") == 0)
        return  0b0111;
    else if( strcmp(reg_text, "ri") == 0 || strcmp(reg_text, "RI") == 0)
        return  0b1000;
    else if( strcmp(reg_text, "rj") == 0 || strcmp(reg_text, "RJ") == 0)
        return  0b1001;
    else if( strcmp(reg_text, "rk") == 0 || strcmp(reg_text, "RK") == 0)
        return  0b1010;
    else if( strcmp(reg_text, "rl") == 0 || strcmp(reg_text, "RL") == 0)
        return  0b1011;
    else if( strcmp(reg_text, "rm") == 0 || strcmp(reg_text, "RM") == 0)
        return  0b1100;
    else if( strcmp(reg_text, "rn") == 0 || strcmp(reg_text, "RN") == 0)
        return  0b1101;
    else if( strcmp(reg_text, "rs") == 0 || strcmp(reg_text, "RS") == 0)
        return  0b1110;
    else if( strcmp(reg_text, "rp") == 0 || strcmp(reg_text, "RP") == 0)
        return  0b1111;

    ASSERT_WRONG_REGISTER(reg_text);
}

uint8_t rm_set(uint8_t bit, uint8_t sreg, uint8_t dreg) {
    return ((sreg & 7) << 5) | ((dreg & 7) << 2) | (bit & 0b11);
}

uint32_t create_instruction(uint8_t* memory, uint32_t pc, const char* instruction, int line) {
    int index = 0;
    char opcode[8] = {0};
    char label[64] = {0};
    char lReg[8] = {0};
    char rReg[8] = {0};

    uint8_t PREFIX = 0;
    uint8_t LREG = 0;
    uint8_t RREG = 0;
    uint8_t MEMTYPE = 0;

    uint32_t offset = 0;
    uint32_t value = 0;

    instruction_text = (char*) instruction;
    line_of_text = line;

    //pc = opcode_label_as_reg_finder_instruction(memory, pc, instruction);
    //ASSERT("");
    int status = 0;
    status |= opcode_reg_reg_finder_instruction          (memory, &pc, instruction, status);
    status |= opcode_value_reg_finder_instruction        (memory, &pc, instruction, status);
    status |= opcode_label_reg_finder_instruction        (memory, &pc, instruction, status);
    status |= opcode_reg_value_finder_instruction        (memory, &pc, instruction, status);
    status |= opcode_reg_label_finder_instruction        (memory, &pc, instruction, status);
    status |= opcode_regoff_reg_finder_instruction       (memory, &pc, instruction, status);
    status |= opcode_valueoff_reg_finder_instruction     (memory, &pc, instruction, status);
    status |= opcode_labeloff_reg_finder_instruction     (memory, &pc, instruction, status);
    status |= opcode_reg_regoff_finder_instruction       (memory, &pc, instruction, status);
    status |= opcode_reg_valueoff_finder_instruction     (memory, &pc, instruction, status);
    status |= opcode_reg_labeloff_finder_instruction     (memory, &pc, instruction, status);
    status |= opcode_label_finder_instruction            (memory, &pc, instruction, status);
    //printf("Status: %i\n", status);
    status |= opcode_reg_finder_instruction              (memory, &pc, instruction, status);
    status |= opcode_only_finder_instruction             (memory, &pc, instruction, status);
    status |= opcode_value_finder_instruction            (memory, &pc, instruction, status);
    status |= defined_label_finder_instruction           (memory, &pc, instruction, status);
    // printf("Test 2....\n");

    // ASSERT("");


    // ascii data
    status |= ascii_variable_finder(instruction, memory, &pc, status );
    status |= datatype_define_finder(instruction, memory, &pc, status );

    if( status == 0 )
        ASSERT_INSTRUCTION("Unknown Instruction", instruction, line );
    // if( regmatch->size() == 0 )
    //     __assert_fail("Cannot Compile with Unknown Matches", instruction, pc, __ASSERT_FUNCTION
    return pc;
}


