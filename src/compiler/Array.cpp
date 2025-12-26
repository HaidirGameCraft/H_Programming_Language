#include "Array.hpp"
#include <tools/memory.h>
#include "Parser.hpp"
#include "Token.hpp"
#include <cstring>

void Array::CleanUp() {
    for( Token* token : this->tokens )
        __free( token );

    for( Array* array : this->arrays ) {
        array->CleanUp();
        __free( array );
    }
}

void Array::GetArrayTokens( Array* array, vector<Token*>* tokens, int start_index ) {
    bool hasFirstBracket = false;
    vector<Token*> __tokens;
    while( start_index < tokens->size() )
    {
        Token* token = (Token*) tokens->operator[]( start_index );
        if( token->getType() == TokenType_BracketType )
        {
            if( token->getName() == "[" )
            {
                if( hasFirstBracket ) {
                    // Creating new array
                    Array* __array = (Array*) __malloc( sizeof( Array ) );
                    Array::GetArrayTokens(__array, tokens, start_index);
                    array->arrays.push_back( __array );
                }
                hasFirstBracket = true;
            }
            else if ( token->getName() == "]" )
            {
                hasFirstBracket = false;
                if( __tokens.size() > 0 )
                {
                    Token* parser = Parser::ParserExp(__tokens);
                    array->tokens.push_back( parser );
                    __tokens.clear();
                }
                break;
            }
        }
        else if ( token->getType() == TokenType_SymbolsType && token->getName() == "," ) {
            Token* parser = Parser::ParserExp(__tokens);
            array->tokens.push_back( parser );
            __tokens.clear();
        }
        else {
            Token* tk = (Token*) __malloc(sizeof(Token));
            memcpy(tk, token, sizeof( Token ));
            __tokens.push_back( tk );
        }
        start_index++;
    }
}
