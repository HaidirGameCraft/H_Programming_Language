#pragma once

#include <cstdint>
#include <stdio.h>
#include <assert.h>

#define MAX_MEMORY 1024

struct hbyte_memory {
    uint8_t* data;
    uint32_t size;
    uint32_t max_size;
};


int hbyte_open_file( const char* filename );
void hbyte_create_file( const char* filename );

int hbyte_write8( uint32_t pc, uint8_t data );
int hbyte_write16( uint32_t pc, uint16_t data );
int hbyte_write32( uint32_t pc, uint32_t data );

void hbyte_close_files();
void hbyte_compile();








