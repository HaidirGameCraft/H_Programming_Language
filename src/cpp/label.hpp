#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <tools/memory.h>
#include <file_reader.h>

typedef struct target_label_t {
    uint32_t destination;
    struct target_label_t* next;
} target_label_t;

typedef struct label_name_t {
    char* label_name;
    uint32_t address;
    int length;
    struct label_name_t* children;
    target_label_t* next;
} label_name_t;

void    push_label_name( const char* label_name );
void    push_next_label_name( const char* label_name, uint32_t destination );
void    update_address_label_name( const char* label_name, uint32_t address );
void    update_all_label_destination( uint8_t* memory );
void    list_label();
void    clear_all_label_name();
