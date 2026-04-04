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
#include "Tools.hpp"
#include "Node.h"
#include <string.h>

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

//    node_program_t* node_program = nodeprogram_create();
//    node_program_t* current_node = node_program;

    std::vector<node_program_t*> stacks_node;

    while( getline( &buffer, &size, file ) != -1 ) {
        buffer[strlen( buffer ) - 1] = 0;
        // Trim
        int _i = 0;
        while( buffer[_i] == ' ' && _i < strlen( buffer ) )
            _i++; // Find the non ' '
        


        string instruction = &buffer[_i];
        vector<Token*> tokens = Tokenizer::extract(instruction);
        if( tokens.size() == 0 )
            continue;

            
/*
        printf("Text: %s", buffer);
        // Push into Node Program
        if( 
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "func") ||
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "while") ||
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "if") 
        ) {
            node_program_t* node = nodeprogram_push( current_node, &buffer[_i] );
            stacks_node.push_back( node );
            current_node = node;

            if( tokens[0]->getName() == "func" )
            {
                int __size = tokens[1]->getName().size();
                node->name = (char*) __malloc( __size + 1 );
                memcpy( node->name, (char*) tokens[1]->getName().c_str(), __size );
                node->name[ __size ] = 0;
            } else {
                std::string gen_name = GenerateRandomName();
                int __size = gen_name.size();
                node->name = (char*) __malloc( __size + 1 );
                memcpy( node->name, (char*) gen_name.c_str(), __size );
                node->name[ __size ] = 0;
            }

        } else if( 
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "endfunc") ||
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "endwhile") ||
            (tokens[0]->getType() == TokenType_KeyWordType && tokens[0]->getName() == "endif") 
        ) {
            
            if( stacks_node.size() > 0 )
            {
                node_program_t* node = nodeprogram_pushChild( current_node->child, &buffer[_i] );
                if ( tokens[0]->getName() != "endfunc" ) {
                    std::string gen_name = GenerateRandomName();
                    int __size = gen_name.size();
                    node->name = (char*) __malloc( __size + 1 );
                    memcpy( node->name, (char*) gen_name.c_str(), __size );
                    node->name[ __size ] = 0;
                }

                current_node = stacks_node.back();
                stacks_node.pop_back();
            }
        } else {
            if( stacks_node.size() > 0 ) {
                // target main parent
                node_program_t* node = NULL;
                if( current_node->child == NULL )
                    node = nodeprogram_pushChild( current_node, &buffer[_i] );
                else
                    node = nodeprogram_push(current_node->child, &buffer[_i] );
            } else  // Global 
                nodeprogram_push( current_node, &buffer[_i] );
        }


*/


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
            if( Function::currentFunction != nullptr )
                for( std::string& __code : codeCompile )
                {
                    if( ControlStructure::current_controlStruct != nullptr )
                        ControlStructure::current_controlStruct->pushCode( __code );
                    else
                        Function::currentFunction->codeSegment.push_back( __code );
                }
            else
                for( std::string& code : codeCompile )
                    File::code.push_back( code );
        }
    }
    Function::cleanLocalFunction();

//    nodeprogram_print( node_program, 0 );
//    nodeprogram_clear( node_program );

    free( buffer );
    fclose( file );
}
