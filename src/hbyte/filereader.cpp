#include "filereader.h"
#include "hbyte/HByte.h"
#include "hbyte/Label.hpp"
#include "hbyte/Token.hpp"
#include "hbyte/Tokenizer.hpp"
#include "tools/memory.h"
#include <cstddef>
#include <fstream>
#include <string>

FILE* file = nullptr;
FILE* output_file = nullptr;
static struct hbyte_memory __memory;

int hbyte_open_file( const char* filename ) {
    file = fopen( filename, "rb+" );
    assert( file != nullptr );
    return 1;
}

void hbyte_create_file( const char* filename ) {
    output_file = fopen( filename, "wb+" );
    assert( output_file != nullptr );
}

void hbyte_close_files() {
    fclose( output_file );
    fclose( file );
}

int hbyte_write8( uint32_t pc, uint8_t data ) {
    if( pc >= __memory.max_size )
    {
        __memory.max_size += MAX_MEMORY;
        __memory.data = (uint8_t*) __realloc(__memory.data, __memory.max_size );
    }

    __memory.data[pc] = data & 0xFF;
    pc++;
    return pc;
}

int hbyte_write16( uint32_t pc, uint16_t data ) {
    hbyte_write8(pc, data & 0xFF);
    hbyte_write8(pc + 1, ( data >> 8 ) & 0xFF );
    pc = pc + 2;
    return pc;
}
int hbyte_write32( uint32_t pc, uint32_t data ) {
    hbyte_write16(pc, data & 0xFFFF);
    hbyte_write16(pc + 2, ( data >> 16 ) & 0xFFFF);
    pc = pc + 4;
    return pc;
}

void hbyte_compile() {

    __memory.data = (uint8_t*) __malloc( MAX_MEMORY );
    __memory.size = 0;
    __memory.max_size = MAX_MEMORY;

    char* lines = NULL;
    size_t size = 0;
    vector<string> text_lines;
    while( getline(&lines, &size,file) != -1 ) {
        string instruction = lines;
        if( instruction == "" || instruction == "\t" || instruction == "\n" )
            continue;
        vector<Token*> tokens = Tokenizer::Extract(instruction);
        if( tokens.size() == 0 )
            continue;

        if( tokens[0]->getType() == TokenType_LabelType )
            Label_add( tokens[0]->getName() );
        text_lines.push_back( lines );
        for(Token* token : tokens)
            __free( token );
    }

    for( string& instruction : text_lines )
    {
        printf("HBYTE: %s", instruction.c_str());
        vector<Token*> tokens = Tokenizer::Extract(instruction);
        if( tokens.size() == 0 )
            continue;

        for(Token* token : tokens )
            token->print(0);
        HByte::Compile(tokens);
        for(Token* token : tokens)
            __free( token );
    }

    Label_update();
    fwrite(__memory.data, __memory.max_size, 1, output_file);

    free( lines );
    __free( __memory.data );
}
