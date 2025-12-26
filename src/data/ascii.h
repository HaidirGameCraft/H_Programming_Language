#pragma once

/**
 * ASCII Data
 * Created By Haidir
 */

#include <cpp/ins_make.hpp>
#include <stdint.h>

#define PATTERN_ASCII   "^=ascii\\s+\"([^\"]*)\"\\s*$"

int ascii_variable_finder(std::string text, uint8_t* memory, uint32_t* pc, int __status);