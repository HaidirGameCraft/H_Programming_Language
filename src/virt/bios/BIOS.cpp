#include "BIOS.h"
#include <raylib.h>
#include <stdio.h>

Disk BIOS::primary_disk;
Disk BIOS::secondary_disk;
uint8_t* BIOS::memory = nullptr;
uint16_t BIOS::char_width = 10;
uint16_t BIOS::char_height = 16;
uint16_t BIOS::max_char_width = 0;
uint16_t BIOS::max_char_height = 0;
bool     BIOS::is_draw_text = false;

int width_screen = 0;
int height_screen = 0;

static  int addrlastest_insert_char = -1;

void BIOS::Init( int width, int height ) {
    width_screen = width;
    height_screen = height;

    max_char_width = (uint16_t)( width_screen / char_width );
    max_char_height = ( uint16_t)( height_screen / char_height );
}

uint16_t BIOS::GetCharFromMemory( uint32_t addr ) {
    return READ_MEMORY_16( BIOS::memory, addr );
}

void BIOS::SetCharToMemory( uint16_t c, uint32_t addr ) {
    BIOS::is_draw_text = true;

    memory[addr + 0] = c & 0xFF;
    memory[addr + 1] = ( c >> 8 ) & 0xFF; 
    addrlastest_insert_char = addr;
}

Color BIOS::BIOSGetColor( uint8_t color ) {
    switch (color)
    {
        case 0b0000: return BLACK;
        case 0b0001: return BLUE;
        case 0b0010: return GREEN;
        case 0b0011: return Color{0, 255, 255, 255};
        case 0b0100: return Color{255, 0, 0, 255};
        case 0b0101: return Color{255, 0, 255, 255};
        case 0b0110: return Color{255, 255,  0, 255};
        case 0b0111: return LIGHTGRAY;
        case 0b1000: return DARKGRAY;
        case 0b1001: return Color{0xAD, 0xD8, 0xE6, 0xFF};
        case 0b1010: return Color{136, 231, 136, 255};
        case 0b1111: return WHITE;
        default: return BLACK;
    }
}

void BIOS::BIOSBasicInterface() {
    // DrawText("Hello", 0, 0, 16, RED);
    for( int i = 0; i < max_char_width * max_char_height; i++ )
    {
        int index = TEXT_MODE_MEMORY + i * 2;
        uint16_t __char = BIOS::GetCharFromMemory( index );
        if( __char == 0 ) continue;

        int cx = (int)( i % max_char_width );
        int cy = (int)( i / max_char_width );

        Color textColor = BIOSGetColor( __char >> 8 & 0xF );

        char txt[] = { (char)(__char & 0xFF), 0 };
        DrawText(txt, cx * char_width, cy * char_height, 8, textColor );
    }
}

