#pragma once

#include <virt/driver/Disk.h>
#include <stdint.h>

struct Color;

/*
    BIOS - Basic Input Output Standard (x86)
*/

#define TEXT_MODE_MEMORY    0xA000

#define READ_MEMORY_8(memory, index) (uint8_t)(memory[index] & 0xFF)
#define READ_MEMORY_16(memory, index) (uint16_t)( READ_MEMORY_8(memory, index + 1) << 8 | READ_MEMORY_8(memory, index) )
#define READ_MEMORY_32(memory, index) (uint32_t)( READ_MEMORY_16(memory, index + 2) << 16 | READ_MEMORY_16(memory, index) )

class BIOS {
public:
    static Disk primary_disk;
    static Disk secondary_disk;
    static uint8_t* memory;

    static uint16_t char_width;
    static uint16_t char_height;
    static uint16_t max_char_width;
    static uint16_t max_char_height;

    static bool is_draw_text;

    static void Init(int width, int height);

    static uint16_t GetCharFromMemory( uint32_t index );
    static void SetCharToMemory( uint16_t c, uint32_t index );

    static void BIOSBasicInterface();
    static Color BIOSGetColor( uint8_t color );
};