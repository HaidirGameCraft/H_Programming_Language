#include "compiler.h"

#include <stdlib.h>
#include <stdio.h>
#include <tools/tools.h>

FILE* compiler_file = NULL;
int file_compiler_open(const char* filename) {
    compiler_file = fopen( filename, "rb+" );
    if( compiler_file == NULL )
    {
        ASSERT("[(ERROR) Compiler]: File Not Found");
    }
}

int file_compiler_read() {
    fseek( compiler_file, 0, SEEK_END );
    long filesize = ftell( compiler_file );
    fseek( compiler_file, 0, SEEK_SET );

    char* filedata = (char*) malloc( filesize + 1 );
    fread( filedata, filesize, 1, compiler_file );
    filedata[filesize] = 0;

    int start_char = 0, end_char = 0, index = 0, line = 0;
    while( index <= filesize )
    {
        if( filedata[end_char] == '\n' || filedata[end_char] == '\0' )
        {
            char* text = (char*) strcpy(&filedata[start_char], end_char - start_char );



            free( text );
            start_char = end_char + 1;
        }
        end_char++;
    }


    free( filedata );
}

void file_compiler_close() {
    fclose( compiler_file );
}