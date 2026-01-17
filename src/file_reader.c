#include "file_reader.h"
#include <assert.h>
#include <stdio.h>

static h_file_header fileHeader;
static FILE* file;

void h_openFile( const char* name ) {
    file = fopen( name, "rb+" );
    if( !file )
    {
        assert(0 && "Error: could not open the file");
    }

    fread(&fileHeader, sizeof( fileHeader ), 1, file );
}

void h_loadFile( uint8_t* memory ) {
    uint32_t count = fileHeader.region_count;
    uint32_t addr = 0;
    for( int i = 0; i < count; i++ )
    {
        region_header regionHeader;
        fread( &regionHeader, sizeof( region_header ), 1, file );
        uint32_t dataSize = regionHeader.size_region;
        fread( &memory[ addr ], dataSize, 1, file );
        addr += dataSize;
    }
}

void h_closeFile() {
    fclose( file );
}
