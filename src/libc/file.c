#include <libc/file.h>
#include <tools/memory.h>
#include "array.h"
#include "cpu.h"
#include <string.h>
#include <asrt.h>
#include <inst.h>

#define FILE_ARRAY_SIZE 16

array_t file_array;

int FileOpen( const char* filename, const char* modes ) {

    if( file_array.data == NULL ) {
        file_array.data = (void*) __malloc( sizeof( uint64_t ) * FILE_ARRAY_SIZE );
        memset( file_array.data, 0, sizeof( uint64_t ) * FILE_ARRAY_SIZE );
        file_array.data_size = sizeof( uint64_t );
        file_array.max_size = file_array.data_size * FILE_ARRAY_SIZE;
        file_array.size = 4;
    }
    
    if( file_array.size >= file_array.max_size )
    {
        int tmp = file_array.max_size;
        file_array.max_size = file_array.max_size + file_array.data_size * FILE_ARRAY_SIZE;
        file_array.data = (void*) __realloc(file_array.data, file_array.max_size);
        memset( &file_array.data[tmp], 0, file_array.data_size * FILE_ARRAY_SIZE);
    }

    FILE* file = fopen( filename, modes );
    if( !file )
    {
        ASSERT("FileOpen Error: no such file or directory");
    }

    for(int i = 3; i < file_array.size; i++ )
    {
        if( ((uint64_t*)file_array.data)[i] == 0 )
        {
            ((uint64_t*) file_array.data)[i] = (uint64_t) file;
            file_array.size++;
            return i;
        }
    }
}

void FileRead( uint32_t buffer, int size, int fd ) {
    uint8_t* memory = get_memory();
    FILE* file = (FILE*) ((uint64_t*) file_array.data)[fd];
    for(int i = 0; i < size; i++) {
       if( (buffer + i) < get_memory_size() )
       {
            int ret = fread( &memory[buffer + i], 1, 1, file);
            if( ret == 0 )
                break;
       }
    } 
}

void FileWrite( uint32_t buffer, int size, int fd ) {

}

void FileClose( int fd ) {
    FILE* file = (FILE*) ((uint64_t*) file_array.data)[fd];
    fclose( file );
    ((uint64_t*) file_array.data)[fd] = 0;
    file_array.size--;
}
