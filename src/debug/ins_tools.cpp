/**
 * ins_tools.c
 * Created By Haidir
 */
#include <regex>
#include <inst.h>
#include "cpu.h"
#include "ins_make.hpp"
#include <stdio.h>
#include <iostream>
#include <ins/inst.h>
#include <file_reader.h>

#define MAX_REGMATCH_LENGTH     10

extern char* instruction_text;
extern int line_of_text;
#define ASSERT_SYMBOLS(x)          printf("\"%s\":%i -> Unknown/Wrong Symbols (%s)\n", instruction_text, line_of_text, x); \
                                ASSERT("unknown operand name");

uint8_t read8(uint8_t* memory, uint32_t* pc) {
    uint8_t r = memory[*pc];
    (*pc)++;
    return r;
}
uint16_t read16(uint8_t* memory, uint32_t* pc) {
    uint16_t r = memory[*pc + 1] << 8 | memory[*pc];
    (*pc) += 2;
    return r;
}
uint32_t read32(uint8_t* memory, uint32_t* pc) {
    uint32_t r = memory[*pc + 3] << 24 | memory[*pc + 2] << 16 | memory[*pc + 1] << 8 | memory[*pc];
    (*pc) += 4;
    return r;
}
uint8_t gread8(uint8_t* memory, uint32_t pc) {
    uint8_t r = memory[pc];
    return r;
}
uint16_t gread16(uint8_t* memory, uint32_t pc) {
    uint16_t r = memory[pc + 1] << 8 | memory[pc];
    return r;
}
uint32_t gread32(uint8_t* memory, uint32_t pc) {
    uint32_t r = memory[pc + 3] << 24 | memory[pc + 2] << 16 | memory[pc + 1] << 8 | memory[pc];
    return r;
}

/*
uint32_t write8(uint8_t* memory, uint32_t pc, uint8_t data) {
    return mwrite8(pc, data);
}
uint32_t write16(uint8_t* memory, uint32_t pc, uint16_t data) {
    return mwrite16(pc, data);
}
uint32_t write32( uint8_t* memory, uint32_t pc, uint32_t data) {
    return mwrite32(pc, data);
}
*/
uint32_t* getreg(cpu_register_t* reg, uint8_t r) {
    uint32_t* b = (uint32_t*) reg;
    if( r > 0xF )
    {
        ASSERT("Error: No such register");
        return NULL;
    }

    return &b[r];
}

std::smatch* cpp_regex_matches(std::string pattern, std::string text) {
    std::smatch* match = new std::smatch();
    
    std::regex r(pattern);
    
    bool status = std::regex_search(text, *match, r);
    if ( status == false )
    {
        delete( match );
        return nullptr;
    }

    for(auto x : *match )
        std::cout << x << std::endl;

    return std::move(match);
}

uint8_t read_memtype(const char* memtype) {
    if( strcmp( memtype, "single") == 0 )
        return 1;
    else if ( strcmp( memtype, "double") == 0 )
        return 2;
    else if ( strcmp( memtype, "quad") == 0 )
        return 3;

    return 0;
}
/*
uint8_t read_symbols_cnd(const char* _sym) {
    if( strcmp(_sym, "==") == 0 )
        return SYM_EQUAL;
    else if ( strcmp(_sym, "<") == 0 )
        return SYM_LESS;
    else if ( strcmp(_sym, ">") == 0 )
        return SYM_GREATER;
    else if ( strcmp(_sym, "<=") == 0 )
        return SYM_LESS_EQUAL;
    else if ( strcmp(_sym, ">=") == 0 )
        return SYM_GREATER_EQUAL;
    else if ( strcmp(_sym, "<<") == 0 )
        return SYM_SHIFT_LEFT;
    else if ( strcmp(_sym, ">>") == 0 )
        return SYM_SHIFT_RIGHT;
    else
    {
        ASSERT_SYMBOLS(_sym)
    }

    return 0;
}
*/
uint32_t     read_number(const char* _text) {
    int r = 0;
    int sign = 1;
    if( _text[0] == '-' )
        sign = -1;

    if( _text[0] == '-' || _text[0] == '+' )
    {
        _text = &_text[1];    
    }

    if( strncmp("0x", _text, 2) == 0 )
        sscanf(_text, "0x%x", &r);
    else
        sscanf(_text, "%i", &r);

    r = r * sign;

    return r;
}
