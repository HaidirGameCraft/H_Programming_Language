#include "compiler/Class.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ControlStructure.hpp"
#include "compiler/Function.hpp"
#include "compiler/Token.hpp"
#include "compiler/Tokenizer.hpp"
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

  //      printf("Text: %s", buffer);

        if( tokens.front()->getType() == TokenType_KeyWordType &&
            tokens.front()->getName() == "class" )
        {
          ClassObject* classObject = ( ClassObject* ) __malloc( sizeof( ClassObject ) ); 
          ClassObject::mainClass = classObject;
          
          classObject->setClassName( tokens[1]->getName() );
        } else if (
            tokens.front()->getType() == TokenType_KeyWordType && tokens.front()->getName() == "endclass"
        ) {
            
        }
        else {
            std::vector<std::string> codeCompile = Compiler::CompileTokens( tokens );
            if( Function::current_token != nullptr )
                for( std::string& __code : codeCompile )
                {
                    if( ControlStructure::current_controlStruct != nullptr )
                        ControlStructure::current_controlStruct->pushCode( __code );
                    else
                        Function::current_token->codeSegment.push_back( __code );
                }
            else
                for( std::string& code : codeCompile )
                    File::code.push_back( code );
        }
    }
    Function::cleanLocalFunction();

    free( buffer );
    fclose( file );
}
