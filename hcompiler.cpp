#include "compiler/Class.hpp"
#include "compiler/File.hpp"
#include "compiler/Function.hpp"
#include "compiler/ControlStructure.hpp"
#include "compiler/Parser.hpp"
#include "compiler/String.h"
#include "compiler/Variable.hpp"
#include "compiler/objects/object.h"
#include <cstdint>
#include <cstdlib>
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
#include <hbyte/HByte.h>
#include <hbyte/filereader.h>

#define MAX_MEMORY_SIZE 0x100000
FILE* output_file = NULL;

void test1() {
    Parser::make_easy_parser = false;
    vector<string> instructions = {
        "int v1 = 3",
        "int v2 = 4",
        "func main() -> void",
        "   int num1 = 1",
        "   int num2 = 3",
        "   int result = num1 + num2",
        "   if ( result > 0 && num1 > num2 ) ",
        "       result = 1",
        "   elif ( result < 0 || num1 < num2 ) ",
        "       result = -1",
        "   else ",
        "       result = 0",
        "   endif",
        "endfunc",
    };

    vector<string> codes_asm;
    for( string& instruction : instructions ) {
        vector<Token*> tokens = Tokenizer::extract( instruction );
        for( Token* token : tokens )
            token->print(0);
        printf("\n\n");

        std::vector<std::string> codeCompile = Compiler::CompileTokens( tokens );
        if( Function::current_token != nullptr )
        {
            if( ControlStructure::current_controlStruct != nullptr )
            {
                for( std::string& code : codeCompile )
                    ControlStructure::current_controlStruct->code.push_back( code );
            } else {
                for(std::string& __c : codeCompile )
                    Function::current_token->codeSegment.push_back( __c );
            }
        }
        else {
            for( std::string& code : codeCompile )
                codes_asm.push_back( code );
        }
        
        
        
        //std::vector<std::string> code = Compiler::CompileTokens( tokens );
        //if( ControlStructure::current_controlStruct != nullptr ) {
        //    for( std::string& str : code ) ControlStructure::current_controlStruct->code.push_back( str ); 
        //}
        //else {
        //    for( std::string& str : code ) codes_asm.push_back( str );
        //}
        
        for(Token* token : tokens )
            __free( token );
    }

    for( std::string& data : Variable::asmData() )
        codes_asm.push_back( data );

    Variable::cleanup();
    ControlStructure::cleanup();
    Function::cleanup();

    for( string& str : codes_asm ) printf("Code: %s\n", str.c_str());
}

int main(int argc, const char** argv) {
    Object::initObjects();
//    test1();
//    __memclear();
//    Object::cleanObject();
//    return EXIT_SUCCESS;
    uint8_t is_compile = 0;
    uint8_t is_lowlevelfile = 0;
    uint8_t is_run = 0;
    uint8_t show_temp = 0;

    std::vector<std::string> filenames;
    std::string file = "";
    std::string __output_file = "";
    for(int i = 1; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s -c|--compile <file.hm>; -> Compiling the raw text into binary code\n", argv[0]);
            printf("help: %s -h|--help\n", argv[0] );

            printf("usage: %s [OPTIONS]\n", argv[0] );
            printf("help: %s -h|--help\n", argv[0] );
            printf("===============================\n");
            printf("[OPTIONS]:\n");
            printf("-o              <outputFile>    : create new OutputFile after compiling\n");
            printf("-c --compile    <file...>       : read the hi (H High Level Language) files\n");
            printf("-cl             <file...>       : read the hm (H Assembly Language) files\n");
            printf("-h --help                       : help | details \n");
            return EXIT_SUCCESS;
        } else if ( strcmp("-c", argv[i]) == 0 || strcmp("--compile", argv[i]) == 0 )
        {
            is_compile = 1;
            is_lowlevelfile = 0;
        } else if ( strcmp("-cl", argv[i]) == 0 )  {
            is_lowlevelfile = 1;
            is_compile = 0;
        }
        else if ( strcmp("-o", argv[i]) == 0 )
        {
            if( i + 1 >= argc )
            {
                printf("<output file> is not providing\n");
                return EXIT_FAILURE;
            }

            __output_file = argv[i + 1];
        } else if ( strcmp("-s", argv[i]) == 0 ) {
            show_temp = true;
        } 
        else {
            std::string tmp( argv[i] );
            if( is_compile )
                filenames.push_back( tmp );
            else if ( is_lowlevelfile )
                hbyte_open_file( tmp.c_str() );
        }
    }

    for(string& filename : filenames) {
        vector<string> code_asm;
        File file;
        file.readFile( filename );

        for( string& global_code : Function::codeGlobal() )
            code_asm.push_back( global_code );

        code_asm.push_back("=region text");
        //code_asm.push_back("__init:");
        //code_asm.push_back("mov 0x500 -> rs");
        //code_asm.push_back("mov rs -> rp");
        //code_asm.push_back("call $main");
        //code_asm.push_back("stp");
        for( string& code : File::code )
            code_asm.push_back( code );
        code_asm.push_back("=region data");
        for( string& data : Variable::asmData() )
            code_asm.push_back( data );

        for( string& cstr : String::codeString() )
            code_asm.push_back( cstr );

        Variable::cleanup();
        Function::cleanup();
        ControlStructure::cleanup();
        String::cleanup();

        FILE* tempFile = tmpfile();
        for( string& code : code_asm ) {
            fwrite(code.c_str(), code.size(), 1, tempFile);
            fwrite("\n", 1, 1, tempFile );
        }

        if ( show_temp ) {
            FILE* outFile = fopen( ( filename + ".hm" ).c_str(), "wb+" );
            if( !outFile )
                assert( false && "FILE_NOT_CREATED");

            for( string& code : code_asm ) {
                fwrite(code.c_str(), code.size(), 1, outFile);
                fwrite("\n", 1, 1, outFile );
            }


            fclose( outFile );
        }
        
        fseek(tempFile, 0, SEEK_SET);
        hbyte_open_file(tempFile);
        //fclose( tempFile );
    }

    printf("Compiling the sources into Machine code\n");
    hbyte_create_file(__output_file.c_str());
    //assert( false );
    hbyte_compile();
    hbyte_close_files();
    __memcheck();
    Object::cleanObject();
    __memclear();
}
