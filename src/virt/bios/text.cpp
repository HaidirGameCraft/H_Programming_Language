#include "BIOS.h"
#include "text.h"

static bios_text_cursor_t text_cursor;
void bios_text_init() {
    text_cursor.rows = 0;
    text_cursor.cols = 0;
    text_cursor.background_color = 0;
    text_cursor.text_color = 0xF;
    text_cursor.show_cursor = 1;
}
void bios_text_cleartext() {

}
void bios_text_setcolor( uint8_t text_color, uint8_t background_color ) {

}
void bios_text_putc( char _c ) {
    if( _c == '\n' )
    {
        text_cursor.rows++;
        text_cursor.cols = 0;
        return;
    }

    int index = text_cursor.cols + text_cursor.rows * BIOS::max_char_width;
    BIOS::memory[TEXT_MODE_MEMORY + index * 2 + 0] = _c;
    BIOS::memory[TEXT_MODE_MEMORY + index * 2 + 1] = (text_cursor.text_color & 0xF) | (text_cursor.background_color << 4 & 0xF);
    index++;
    text_cursor.cols = (uint16_t)( index % BIOS::max_char_width );
    text_cursor.rows = (uint16_t)( index / BIOS::max_char_width );
}

void bios_text_print( const char* text ) {
    int index = 0;
    while( text[index] != 0 ) bios_text_putc( text[index++] );
}