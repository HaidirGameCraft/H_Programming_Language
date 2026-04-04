#pragma once

#include <cstdint>

typedef struct {
    uint16_t rows;
    uint16_t cols;
    uint8_t  text_color: 4;
    uint8_t  background_color: 4;
    uint8_t  show_cursor;
} bios_text_cursor_t;

void bios_text_init( );
void bios_text_cleartext();
void bios_text_setcolor( uint8_t text_color, uint8_t background_color );
void bios_text_putc( char _c );
void bios_text_print( const char* text );