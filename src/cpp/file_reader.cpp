/**
 * file_reader.c
 * Created By Haidir
 */

#include "asrt.h"
#include <inst.h>
#include <file_reader.h>
#include <stdio.h>
#include <ins_make.h>
#include <stdlib.h>
#include <tools/memory.h>
#include <regex>
#include <string>
#include <vector>

#include <cpp/label.hpp>
#include <tools/tools.h>

#define MAX_LENGTH_DATA 1024
#define MAX_LENGTH_MEMORY 1024

typedef struct h_code_details {
    std::string str;
    uint32_t pc_start;
    uint32_t size;
} h_code_details;

FILE *file_output = NULL;
FILE *region_file = NULL;

std::vector<h_code_details> hprograms;
std::vector<std::string> regions_name;
std::vector<region_t> regions;
std::vector<FILE*> files;
region_t* current_region = NULL;

uint8_t* __memory__ = NULL;
int memory_length = 0;
int max_memory_length = 0;

uint32_t read_line( char* text, int line, int size, uint32_t pc );
void     read_label_line( char* text, int size );
void     save_dumpfile();

uint32_t open_files_reader( std::vector<std::string> filenames ) {
    for(auto& filename : filenames ) {
        FILE* file = fopen( filename.c_str(), "r" );
        if( file == NULL )
            return 1;

        files.push_back( file );
    }

    regions_name.push_back("text");
    regions_name.push_back("data");
    regions_name.push_back("raw");

    for(int i = 0; i < regions_name.size(); i++ ) {
        region_t __region;
        __region.name = regions_name[i];
        __region.file = tmpfile();
        regions.push_back( __region );
    }

    return 0;
}

uint8_t* read_file_reader( uint32_t max_memory ) {
    int index = 0;

    __memory__ = (uint8_t*) __malloc( MAX_LENGTH_MEMORY );
    max_memory_length = MAX_LENGTH_MEMORY;

    int char_start = 0;
    int char_end = 0;
    int pc = 0;
    int textLine = 0;

    long size = 0;
    for(FILE* file : files ) {
        fseek( file, 0, SEEK_END );
        size = ftell( file );
        fseek( file, 0, SEEK_SET );

        char* text = (char*) __malloc( size + 1 );
        fread(text, 1, size, file);
        text[size] = 0;


        // Scanning the label
        while( index < size )
        {
            if( text[index] == '\n' || text[index] == 0 ) {
                char* __text = strcpy( &text[char_start], char_end - char_start );
                read_label_line( &text[char_start], char_end - char_start);
                __free( __text );
                char_start = char_end + 1;
            }

            char_end++;
            index++;
        }

        char_start = 0;
        char_end = 0;
        index = 0;

        __free( text );
    }
    list_label();
    // save_dumpfile();
    //ASSERT("");
    // Scanning the instruction

    for(int i = 0; i < regions.size(); i++) {
        region_t* region = &regions[i];
        fseek( region->file, 0, SEEK_END);
        size = ftell( region->file );
        fseek( region->file, 0, SEEK_SET);

        char* text = (char*) __malloc( size + 1 );
        fread( text, size,  1, region->file );
        text[size] = 0;

        index = 0;
        char_start = 0;
        char_end = 0;
        while( index < size )
        {
            if( text[index] == '\n' || text[index] == 0 )
            {
                memory_length = read_line( &text[char_start], textLine + 1, char_end - char_start, memory_length );
                char_start = char_end + 1;
                textLine++;
            }

            char_end++;
            index++;
        }

        checkAlloc(__free( text ));
    }

    // list_label_instruction();
    list_label();
    update_all_label_destination( __memory__ );
    clear_all_label_name();

    // for(int i = 0; i < hprograms.size(); i++) {
    //     printf("%08x: ", hprograms[i].pc_start);
    //     for(int j = 0; j < hprograms[i].size; j++)
    //         printf("%02x ", gread8(memory, hprograms[i].pc_start + j));
    //     printf("%s\n", hprograms[i].str.c_str());
    // }
    return 0;
}

void close_file_reader() {
    fclose( file_output );
    __free( __memory__ );

    for(int i = 0; i < regions.size(); i++) {
        fclose( regions[i].file );
    }
}

uint32_t make_file_reader( const char* fileoutput ) {
    file_output = fopen(fileoutput, "wb+");
    if( !file_output )
        return 1;

    return 0;
}

void    write_file_reader() {
    fwrite( __memory__, max_memory_length, 1, file_output);
}

uint32_t read_line( char* text, int textLine, int size, uint32_t pc ) {
    // reading the line of codes
    char* t_tmp = strcpy( text, size );
    char* __t = strtrim( t_tmp );
    __free( t_tmp );
    if( __t == NULL )
        return pc;

    if( __t[0] == '#' ) {
        // Ignoring the comment
        __free( __t );
        return pc;    
    }

#ifdef __DEBUG__    
    printf("Text: |%s|\n", __t);
#endif
    expand_memory(pc);
    pc = create_instruction(__memory__, pc, __t, textLine);
    checkAlloc(__free( __t ));
    return pc;
}

void     read_label_line( char* text, int size ) {
    char* t_tmp = strcpy( text, size );
    // triming the text
    // starting with start of string
    char* _t = strtrim( t_tmp );
    __free( t_tmp );

    if( _t == NULL )
        return;
    // printf("Text: %s\n", _t);

    std::string str( _t );
    std::regex region_pattern("^=region\\s+(\\w+)$");
    if( std::regex_match( str, region_pattern ) == true )
    {
        std::smatch match;
        if( std::regex_search( str, match, region_pattern) == true ) {
            for( int i = 0; i < regions.size(); i++ )
            {
                if( strcmp( regions[i].name.c_str(), match[1].str().c_str() ) == 0 )
                {
                    current_region = &regions[i];
                    break;
                }
            }
        }

        return;
    }

    str += '\n';
    fwrite(str.c_str(), str.size(), 1, current_region->file);

    int length = strlen( _t );    
    if( _t[0] == '#' ) {
        // Ignoring the comment
        __free( _t );
        return; 
    }

    // Indicating label
//    printf("Label: |%s|\n", _t);
    if( _t[length - 1] == ':' )
    {
        push_label_name( _t );
    }

    __free( _t );
}

void expand_memory(int length) {
    if( __memory__ == NULL )
        ASSERT("Memory is NULL");

//    printf("Memory: max_length %i, length: %i\n", max_memory_length, length);

    if( length >= max_memory_length )
    {
        max_memory_length += MAX_LENGTH_MEMORY;
#ifdef __DEBUG__
        printf("Old Addr Memory: 0x%x\n", (uint64_t) __memory__);
#endif
        __memory__ = (uint8_t*) __realloc( __memory__, max_memory_length );
#ifdef __DEBUG__ 
        printf("New Addr MemoryL 0x%x\n", (uint64_t) __memory__);
#endif
    }
}

uint8_t* file_get_memory() {
    return __memory__;
}

void     save_dumpfile() {
    printf("Save the dump  file\n");
    for(auto& region : regions) {
        std::string filename = "tmpdumpfile_" + region.name + ".dump";
        FILE* file = fopen(filename.c_str(), "wb+");
        if ( !file )
            ASSERT("Could not save the file");

        fseek(region.file, 0, SEEK_END);
        long size = ftell(region.file);
        fseek(region.file, 0, SEEK_SET);

        char* text = (char*) __malloc( size );
        fread( text, size, 1, region.file );
        fwrite( text, size, 1, file );
        __free( text );
        fclose( file );
    }
}


int      mwrite8(uint32_t pc, uint8_t data) {
    expand_memory(pc);
    if( __memory__ == NULL )
    {
        __memclear();
        ASSERT("writing memory is error cause the memory is null");
    }
    __memory__[pc] = data & 0xFF;
    return pc + 1;
}

int      mwrite16(uint32_t pc, uint16_t data) {
    mwrite8(pc, data & 0xFF);
    mwrite8(pc + 1, (data >> 8) & 0xFF);
    pc = pc + 2;
    return pc;
}
int     mwrite32(uint32_t pc,  uint32_t data) {
    mwrite16(pc, data & 0xFFFF);
    mwrite16(pc + 2, uint16_t (data >> 16) & 0xFFFF);
    pc = pc + 4;
    return pc;
}












