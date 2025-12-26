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
#ifdef __cplusplus
#include <string>
#include <vector>
#endif

typedef struct {
    uint8_t identifier[12];
    uint32_t program_start;
    uint32_t label_region_offset;
    uint32_t region_offset_start;
} h_file_header;

typedef struct {
    uint32_t data_offset;
    uint32_t number_label_entry;
    uint32_t size_label_entry;
} h_label_region_header;

typedef struct {
    uint32_t address;
    uint32_t label_offset;
    uint32_t label_size;
} h_label_entry;

typedef struct {
    uint32_t data_offset;
    uint32_t size_region;
    uint32_t memory_required;
    uint32_t next_region_offset;
} region_header;
/**
 * Region
 * The default order:
 * text
 * data
 * raw
 * 
*/

typedef struct {
    std::string name;
    FILE* file;
} region_t;

uint32_t open_file_reader( const char* filename );
uint32_t open_files_reader( std::vector<std::string> filenames );
uint32_t make_file_reader( const char* fileoutput );
uint8_t* read_file_reader( uint32_t max_memory );
void     write_file_reader();
void     close_file_reader();

int      mwrite8(uint32_t pc, uint8_t data);
int      mwrite16(uint32_t pc, uint16_t data);
int      mwrite32(uint32_t pc,  uint32_t data);
uint8_t* file_get_memory();
void     expand_memory(int length);

/**
 * File Format (H)
 */
void create_file_format();
