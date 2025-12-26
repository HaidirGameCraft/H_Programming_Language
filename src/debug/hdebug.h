#pragma once

#include <stdint.h>
#include <stdio.h>
#include <inst.h>

/**
 * DEBUG
 * - The purpose of this is to debug the code from H Programming Language
 */

void draw_terminal();

void debug_main(const char* filename);
void debug_open_file(const char* filename);
void debug_read_file(uint32_t size_memory);
void debug_close_file();
