/**
 * ins_make.h
 * Created By Haidir
 */

#pragma once
#include <stdint.h>

/**
 * Tools
 */
uint8_t read_memtype(const char* memtype);
uint8_t read_symbols_cnd(const char* _sym);
uint32_t     read_number(const char* _sym);
