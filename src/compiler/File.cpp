#include "compiler/Array.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/Function.hpp"
#include "compiler/Token.hpp"
#include "compiler/Tokenizer.hpp"
#include "compiler/Variable.hpp"
#include <cassert>
#include <compiler/File.hpp>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <tools/memory.h>

vector<string> File::global_code;
vector<string> File::code;
vector<string> File::data;
vector<string> File::filesImport;
void File::readFile( string filename ) {

    // Check the files has be read or not
    // if already in list, return because We do not duplicate the code
    for( string& file_import : File::filesImport )
        if( file_import == filename )
            return;

    FILE* file = fopen( filename.c_str(), "rb+" );

    if( !file )
        assert(false && "FILE_NOT_FOUND");

    // add the filename into list of file import
    File::filesImport.push_back( filename );

    char* buffer = nullptr;
    size_t size = 0;
    while( getline( &buffer, &size, file ) != -1 ) {
        string instruction = buffer;
        vector<Token*> tokens = Tokenizer::extract(instruction);
        if( tokens.size() == 0 )
            continue;

        printf("[Code]: %s\n", instruction.c_str());
        if( tokens[0]->getType() == TokenType_SymbolsType && tokens[0]->getName() == "@" )
        {
            Token* config_token = tokens[1];
            if( config_token->getName() == "import" )
            {
                assert(tokens[2] != nullptr && tokens[2]->getType() == TokenType_StringType);
                string import_filename = tokens[2]->getName().substr(1, tokens[2]->getName().size() - 2);
                printf("Import File: %s\n", import_filename.c_str());
                File importFile;
                importFile.readFile( import_filename ); 
                printf("Return to File: %s\n", filename.c_str());
            }

            for(Token* token : tokens )
                __free( token );
        } else {
            vector<string> code = Compiler::CompileTokens( tokens );
            for( string& __code : code )
                File::code.push_back( __code );
        }
    }

    Function::cleanLocalFunction();

    free( buffer );
    fclose( file );
}
