#include "hdebug.h"

FILE* debug_file = NULL;
void debug_open_file(const char* filename) {
    debug_file = fopen(filename, "+rb");
    if( debug_file )
        ASSERT("The File is not defined");
}

void debug_read_file(int size_memory) {
    uint8_t* memory = (uint8_t*) malloc( size_memory );

    fseek(debug_file, 0, SEEK_END );
    long size = ftell( debug_file );
    fseek(debug_file, 0, SEEK_SET );
    fread( memory, size, 1, debug_file );

    

    free( memory );
}

void debug_close_file() {
    fclose( debug_file );
}
