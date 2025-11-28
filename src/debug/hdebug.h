#pragma once

#include <stdint.h>
#include <stdio.h>
#include <inst.h>

/**
 * DEBUG
 * - The purpose of this is to debug the code from H Programming Language
 */

void debug_open_file(const char* filename);
void debug_read_file();
void debug_close_file();