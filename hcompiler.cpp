#include "compiler/File.hpp"
#include "compiler/Function.hpp"
#include "compiler/IfCondition.hpp"
#include "compiler/String.h"
#include "compiler/Variable.hpp"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>

#include <compiler/Token.hpp>
#include <compiler/Tokenizer.hpp>
#include <compiler/Compiler.hpp>
#include <tools/memory.h>

#define MAX_MEMORY_SIZE 0x100000
FILE* output_file = NULL;
int main(int argc, const char** argv) {
    uint8_t is_compile = 0;
    uint8_t is_run = 0;

    std::vector<std::string> filenames;
    std::string file = "";
    std::string __output_file = "";
    for(int i = 1; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s -c|--compile <file.hm>; -> Compiling the raw text into binary code\n", argv[0]);
            printf("help: %s -h|--help\n", argv[0] );
            return EXIT_SUCCESS;
        } else if ( strcmp("-c", argv[i]) == 0 || strcmp("--compile", argv[i]) == 0 )
        {
            is_compile = 1;
        } else if ( strcmp("-o", argv[i]) == 0 )
        {
            if( i + 1 >= argc )
            {
                printf("<output file> is not providing\n");
                return EXIT_FAILURE;
            }

            __output_file = argv[i + 1];
        } else {
            std::string tmp( argv[i] );
            filenames.push_back( tmp );
        }
    }

    if( is_compile == 1 )
    { 
        for(string& filename : filenames) {
            vector<string> code_asm;
            File file;
            file.readFile( filename );

            for( string& global_code : Function::codeGlobal() )
                code_asm.push_back( global_code );

            code_asm.push_back("=region text");
            code_asm.push_back("__init:");
            code_asm.push_back("mov 0x500 -> rs");
            code_asm.push_back("mov rs -> rp");
            code_asm.push_back("call $main");
            code_asm.push_back("stp");
            for( string& code : File::code )
                code_asm.push_back( code );
            code_asm.push_back("=region data");
            for( string& data : Variable::asmData() )
                code_asm.push_back( data );

            for( string& cstr : String::codeString() )
                code_asm.push_back( cstr );

            Variable::cleanup();
            Function::cleanup();
            IfCondition::cleanup();
            String::cleanup();

            FILE* outFile = fopen( ( filename + ".ho" ).c_str(), "wb+" );
            if( !outFile )
                assert( false && "FILE_NOT_CREATED");

            for( string& code : code_asm ) {
                fwrite(code.c_str(), code.size(), 1, outFile);
                fwrite("\n", 1, 1, outFile );
            }


            fclose( outFile );
        }
        
    }

    __memcheck();
    __memclear();
}
