#pragma once

#include <cstdint>
#include <stdio.h>
#include <assert.h>
#include "../file_reader.h"
#include <string>
#include <vector>


#define MAX_MEMORY 1024

struct hbyte_memory {
    uint8_t* data;
    uint32_t size;
    uint32_t max_size;
};

struct hbyte_region {
    std::string nameRegion;
    std::vector<std::string> texts;
    uint32_t start_addr;
    uint32_t end_addr;
};


int hbyte_open_file( const char* filename );
int hbyte_open_file( FILE* file );
void hbyte_create_file( const char* filename );
void hbyte_setEntry( const char* entryName );

int hbyte_write8( uint32_t pc, uint8_t data );
int hbyte_write16( uint32_t pc, uint16_t data );
int hbyte_write32( uint32_t pc, uint32_t data );

void hbyte_close_files();
void hbyte_compile();








