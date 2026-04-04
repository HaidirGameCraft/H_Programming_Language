#pragma once

#include <stdint.h>

typedef uint8_t* memory_t;
#ifdef __cplusplus
extern "C"
#else
extern
#endif
memory_t create_memory( uint32_t size_of_memory );
memory_t get_memory();

// read from memory
uint8_t     read8_memory( uint32_t address );
uint16_t    read16_memory( uint32_t address );
uint32_t    read32_memory( uint32_t address );

// write into memory
void        write8_memory( uint32_t address, uint8_t data );
void        write16_memory( uint32_t address, uint16_t data );
void        write32_memory( uint32_t address, uint32_t data );

#ifdef __cplusplus
extern "C"
#else
extern
#endif
void     clear_memory();