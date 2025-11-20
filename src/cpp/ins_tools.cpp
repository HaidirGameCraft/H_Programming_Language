/**
 * ins_tools.c
 * Created By Haidir
 */
#include <regex>
#include <inst.h>
#include "ins_make.hpp"
#include <stdio.h>
#include <iostream>
#include <ins/inst.h>

#define MAX_REGMATCH_LENGTH     10

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

uint32_t write8(uint8_t* memory, uint32_t pc, uint8_t data) {
    memory[pc] = data & 0xFF;
    return pc + 1;
}
uint32_t write16(uint8_t* memory, uint32_t pc, uint16_t data) {
    write8(memory, pc, data & 0xFF);
    write8(memory, pc + 1, (data >> 8) & 0xFF);
    return pc + 2;
}
uint32_t write32( uint8_t* memory, uint32_t pc, uint32_t data) {
    write16( memory, pc, data & 0xFFFF );
    write16( memory, pc + 2, ( data >> 16 ) & 0xFFFF );
    return pc + 4;
}

uint8_t* reg8(cpu_register_t* reg, uint8_t r) {
    if( r == 0 )
        return &reg->r8a;
    else if ( r == 1 )
        return &reg->r8b;
    else if ( r == 2 )
        return &reg->r8c;
    else if ( r == 3 )
        return &reg->r8d;
    else if ( r == 4 )
        return &reg->r8e;
    else if ( r == 5 )
        return &reg->r8f;
    else if ( r == 6 )
        return &reg->r8s;
    else if ( r == 7 )
        return &reg->r8bp;
}
uint16_t* reg16(cpu_register_t* reg, uint8_t r) {
    if( r == 0 )
        return &reg->r16a;
    else if ( r == 1 )
        return &reg->r16b;
    else if ( r == 2 )
        return &reg->r16c;
    else if ( r == 3 )
        return &reg->r16d;
    else if ( r == 4 )
        return &reg->r16e;
    else if ( r == 5 )
        return &reg->r16f;
    else if ( r == 6 )
        return &reg->r16s;
    else if ( r == 7 )
        return &reg->r16bp;
}
uint32_t* reg32(cpu_register_t* reg, uint8_t r) {
    if( r == 0 )
        return &reg->r32a;
    else if ( r == 1 )
        return &reg->r32b;
    else if ( r == 2 )
        return &reg->r32c;
    else if ( r == 3 )
        return &reg->r32d;
    else if ( r == 4 )
        return &reg->r32e;
    else if ( r == 5 )
        return &reg->r32f;
    else if ( r == 6 )
        return &reg->r32s;
    else if ( r == 7 )
        return &reg->r32bp;
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

uint8_t read_symbols_cnd(const char* _sym) {
    if( strcmp(_sym, "==") == 0 )
        return SYM_CND_EQUAL;
    else if ( strcmp(_sym, "<") == 0 )
        return SYM_CND_LESS;
    else if ( strcmp(_sym, ">") == 0 )
        return SYM_CND_GREATER;
    else if ( strcmp(_sym, "<=") == 0 )
        return SYM_CND_LESS_EQUAL;
    else if ( strcmp(_sym, ">=") == 0 )
        return SYM_CND_GREATER_EQUAL;

    return 0;
}

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