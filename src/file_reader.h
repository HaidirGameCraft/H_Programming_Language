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

#define HFILEIDENTIFIER 0x00000000424c5048

typedef struct {
    uint64_t identifier;
    uint32_t program_start;
    uint32_t label_region_offset;
    uint32_t region_count;
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

void h_openFile( const char* name );
void h_loadFile( uint8_t* memory );
void h_closeFile();


