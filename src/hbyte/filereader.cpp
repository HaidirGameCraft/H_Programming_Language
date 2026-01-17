#include "filereader.h"
#include "file_reader.h"
#include "hbyte/HByte.h"
#include "hbyte/Label.hpp"
#include "hbyte/Token.hpp"
#include "hbyte/Tokenizer.hpp"
#include "tools/memory.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace HByte;
static FILE* output_file = nullptr;
static struct hbyte_memory __memory;

int pcCount = 0;
static struct hbyte_region* currentRegion = nullptr;
std::vector<hbyte_region*> __regionLists;

void hbyte_createFormatFile();

static std::vector<FILE*> files;
hbyte_region* createRegionMemory( const std::string& name ) {
    hbyte_region* region = ( hbyte_region* ) __malloc( sizeof( hbyte_region ) );
    region->nameRegion = name;
    __regionLists.push_back( region );
    return region;
}

hbyte_region* getRegionMemory( const std::string& name ) {
    for( hbyte_region* reg : __regionLists )
        if( reg->nameRegion == name )
            return reg;

    return nullptr;
}

void clearRegionMemory() {
    for( hbyte_region* reg : __regionLists )
    {
        reg->nameRegion.clear();
        reg->texts.clear();
        __free( reg );
    }
}

int hbyte_open_file( const char* filename ) {
    FILE* file = fopen( filename, "rb+" );
    assert( file != nullptr );
    files.push_back( file );
    return 1;
}

int hbyte_open_file( FILE* __file ) {
    files.push_back( __file );
    return 0;
}

void hbyte_setEntry(const char *entryName) {

}

void hbyte_create_file( const char* filename ) {
    output_file = fopen( filename, "wb+" );
    assert( output_file != nullptr );
}

void hbyte_close_files() {
    fclose( output_file );

    for( FILE* __file : files )
        fclose( __file );
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
    pc = hbyte_write8(pc, data & 0xFF);
    pc = hbyte_write8(pc, ( data >> 8 ) & 0xFF );
    return pc;
}
int hbyte_write32( uint32_t pc, uint32_t data ) {
    pc = hbyte_write16(pc, data & 0xFFFF);
    pc = hbyte_write16(pc, ( data >> 16 ) & 0xFFFF);
    return pc;
}

void hbyte_compile() {

    // The Region Order will be followd by:
    // text
    // data
    //

    currentRegion = createRegionMemory("text");
    //createRegionMemory("data");

    __memory.data = (uint8_t*) __malloc( MAX_MEMORY );
    __memory.size = 0;
    __memory.max_size = MAX_MEMORY;

    char* lines = NULL;
    size_t size = 0;
    vector<string> text_lines;
    
    //text_lines.push_back("=region text");
    currentRegion->texts.push_back("__init:\n");
    currentRegion->texts.push_back("   mov 0x1000 -> rs\n");
    currentRegion->texts.push_back("   mov rs -> rp\n");
    currentRegion->texts.push_back("   call $main\n");
    currentRegion->texts.push_back("   stp\n");
    Label_add("__init");


    for( FILE* file : files ) {
        while( getline(&lines, &size,file) != -1 ) {
            string instruction = lines;
            if( instruction == "" || instruction == "\t" || instruction == "\n" )
                continue;
            vector<Token*> tokens = Tokenizer::Extract(instruction);
            if( tokens.size() == 0 )
                continue;

            //printf("[ASM]: %s", instruction.c_str());

            if( tokens[0]->getType() == HByte::TokenType_SymbolsType && tokens[0]->getName() == "=" && tokens[1]->getName() == "region")
            {
                std::string regionName = tokens[2]->getName();
                hbyte_region* crtregion = getRegionMemory( regionName );
                if( crtregion == nullptr )
                {
                    //printf("[RegionData] Create New Region with name %s\n", regionName.c_str());
                    currentRegion = createRegionMemory( regionName );
                } else {
                    //printf("[Use Region] %s\n", crtregion->nameRegion.c_str());
                    currentRegion = crtregion;
                }
            } else {
                if( tokens[0]->getType() == TokenType_LabelType )
                    Label_add( tokens[0]->getName() );
                currentRegion->texts.push_back( lines );
            }
            for(Token* token : tokens)
                __free( token );
        }
    }

    //printf("[Convert] Convert to Binary\n");

    int start__ = 0;
    for( hbyte_region* region : __regionLists )
    {
        for( string& instruction : region->texts )
        {

            //printf("[Compiling ASM] %s", instruction.c_str());
            vector<Token*> tokens = Tokenizer::Extract(instruction);
            if( tokens.size() == 0 )
                continue;


            HByteCompiler::Compile(tokens);
            for(Token* token : tokens)
                __free( token );
        }
        uint32_t end__ = HByteCompiler::pc;
        region->start_addr = start__;
        region->end_addr = end__;
        start__ = end__;
    }

    Label_update();
    //fwrite(__memory.data, __memory.max_size, 1, output_file);
    hbyte_createFormatFile();
    clearRegionMemory();
    Label_clear();

    free( lines );
    __free( __memory.data );
}

void hbyte_createFormatFile() {
    uint32_t addr = sizeof( h_file_header );
    h_file_header fileHeader;
    fileHeader.identifier = HFILEIDENTIFIER;
    fileHeader.program_start = Label_getAddress("__init");
    fileHeader.region_offset_start = sizeof( h_file_header );
    fileHeader.region_count = __regionLists.size();
    fwrite( &fileHeader, sizeof( h_file_header ), 1, output_file );

    for( hbyte_region* region : __regionLists )
    {
        region_header regHeader;
        regHeader.size_region = region->end_addr - region->start_addr;
        regHeader.data_offset = addr + sizeof( regHeader );
        regHeader.memory_required = 0;
        regHeader.next_region_offset = regHeader.data_offset + regHeader.size_region;
        addr = addr + sizeof( regHeader ) + regHeader.size_region;

        fwrite( &regHeader, sizeof( regHeader ), 1, output_file );
        fwrite( &__memory.data[region->start_addr], regHeader.size_region, 1, output_file );
    }
}
