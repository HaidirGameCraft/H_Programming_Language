#pragma once

#include <stdint.h>
#include <inst.h>


/**
 * File Compiler Component
 */
int file_compiler_open(const char* filename);
int file_compiler_read();
void file_compiler_close();


/**
 * Compiler Component
 */
uint32_t instruction_compile(char* instruction, int size_instruction, uint32_t pc );


