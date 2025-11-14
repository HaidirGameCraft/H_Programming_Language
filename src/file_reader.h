/**
 * file_reader.h
 * Created By Haidir
 */
#pragma once

/**
 * File Reader For own Assembly Language
 * Created By Haidir
 */

#include <stdint.h>

uint32_t open_file_reader( const char* filename );
uint8_t* read_file_reader( uint32_t max_memory );
void close_file_reader();