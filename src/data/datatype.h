#pragma once

/**
 * DataType
 * Created By Haidir
 * 
 * SINGLE, DOUBLE and QUAD 
 * SINGLE = 1 byte
 * DOUBLE = 2 bytes
 * QUAD   = 4 bytes
*/

#include <cpp/ins_make.hpp>
#include <inst.h>
#include <stdint.h>

#define PATTERN_DATATYPE            "^=(single|double|quad)\\s+(0x[0-9a-fA-F]+|[0-9]+)$"
#define PATTERN_DATATYPE_RESERVED   "^=r(s|d|q)\\s+(0x[0-9a-fA-F]+|[0-9]+)$"

int datatype_define_finder(const char* text, uint8_t* memory, uint32_t* pc, int __status);
