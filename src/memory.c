#include "memory.h"
#include <tools/memory.h>
#include "asrt.h"
#include <stdio.h>


memory_t MEMORY = NULL;
uint32_t SIZE_MEMORY = 0;
memory_t create_memory( uint32_t size_of_memory ) {
    MEMORY = (memory_t) __malloc( size_of_memory );
    SIZE_MEMORY = size_of_memory;
    EXITFAIL( MEMORY != NULL, printf("[Virtual System Error]: could not create virtual MEMORY\n") )
}

memory_t get_memory() {
    return MEMORY;
}

// read from memory
uint8_t     read8_memory( uint32_t address ) {
    EXITFAIL( address < SIZE_MEMORY, printf("[System Panic]: the address of memory is OUT OF RANGE\n") )
    return MEMORY[ address ] & 0xFF;
}
uint16_t    read16_memory( uint32_t address ) {
    uint8_t low = read8_memory( address );
    uint8_t high = read8_memory( address + 1);
    return (high << 8) | low;
}
uint32_t    read32_memory( uint32_t address ) {
    uint16_t low = read16_memory( address );
    uint16_t high = read16_memory( address + 2 );
    return (high << 16 & 0xFFFF) | ( low & 0xFFFF ); 
}

// write into memory
void        write8_memory( uint32_t address, uint8_t data ) {
    EXITFAIL( address < SIZE_MEMORY, printf("[System Panic]: the address of memory is OUT OF RANGE\n") )
    MEMORY[ address ] = data;
}
void        write16_memory( uint32_t address, uint16_t data ) {
    write8_memory( address, data & 0xFF );
    write8_memory( address + 1, (data >> 8) & 0xFF );
}
void        write32_memory( uint32_t address, uint32_t data ) {
    write16_memory( address, data & 0xFFFF );
    write16_memory( address + 2, (data >> 16) & 0xFFFF );
}

void     clear_memory() {
    __free( MEMORY );
}