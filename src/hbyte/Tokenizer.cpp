#include "Token.hpp"
#include <cassert>
#include "Tokenizer.hpp"
#include "../tools/memory.h"
#include <string.h>

bool isOperator(char c);
bool isSymbols(char c);
bool isWords(char c);
bool isSpace(char c);
bool isRegister(string c);
bool isNumber(string c);

vector<Token*> Tokenizer::Extract(string instruction) {
    const char* byte = instruction.c_str();
    int length_instruction = instruction.size();
    string __tmp = "";
    vector<string> strs;
    // RegExp the Instruction into different part
    for(int i = 0; i < length_instruction; i++) {
        char c = byte[i];

        if( isSpace(c) ) {
            if( __tmp.size() > 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            continue;
        }
       
        if( isSymbols(c) )
        {
            if( __tmp.size() > 0 ) {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            while( isSymbols(byte[i]) )
            {
                __tmp += byte[i];
                i++;
            }

            strs.push_back(__tmp);
            __tmp.clear();
            if( !isSpace(byte[i]) )
                __tmp += byte[i];
        }
        else if ( c == '$' )
        {
            if( __tmp.size() > 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            __tmp += c;
            i++;
            while( isWords(byte[i]) )
            {
                __tmp += byte[i];
                i++;
            }
            strs.push_back(__tmp);
            __tmp.clear();
        }
        else if ( isOperator(c) ) // Operation Token
        {
            if( __tmp.size() > 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            while( isOperator(byte[i]) )
            {
                __tmp += byte[i];
                i++;
            }

            strs.push_back(__tmp);
            __tmp.clear();
            if( !isSpace(byte[i]) )
                __tmp += byte[i];
        }
        else if ( c == '\"' )
        {
            if( __tmp.size() > 0 )
            {
                strs.push_back(__tmp);
                __tmp.clear();
            }

            __tmp += '\"';
            i++;
            while( byte[i] != '\"' )
            {
                assert(i < length_instruction);
                __tmp += byte[i];
                i++;
            }
            __tmp += '\"';

            strs.push_back(__tmp);
            __tmp.clear();
        }
        else {
            __tmp += c;
        }
    }

    if( __tmp.size() > 0 )
    {
        strs.push_back(__tmp);
        __tmp.clear();
    }

    assert(strs.size() > 0);
    vector<Token*> tokens;
    for(int i = 0; i < strs.size(); i++)
    {
        string& str = strs[i];
        Token* token = (Token*) __malloc( sizeof( Token ) );
        if(     str == "->" || str == "==" || str == "<" || 
                str == ">" || str == "<=" || str == ">=" ||
                str == "!=" || str == "<<" || str == ">>" )
            token->setType( TokenType_SymbolsType );
        else if ( str[0] == '\"' && str[str.size() - 1 ] == '\"' )
            token->setType( TokenType_StringType );
        else if ( str == ":" || str == "$" || str == "=")
            token->setType( TokenType_SymbolsType );
        else if ( str == "+" || str == "-" || str == "*" || str == "/" )
            token->setType(TokenType_OperationType);
        else if ( str == "(" || str == ")" || str == "[" || str == "]") {
            token->setType( TokenType_BracketType );
            if( tokens[tokens.size() - 1]->getType() == TokenType_RegisterType || 
                tokens[tokens.size() - 1]->getType() == TokenType_LabelType ||
                tokens[tokens.size() - 1]->getType() == TokenType_NumberType )
                tokens[tokens.size() - 1]->hasOffset = true;
        }
        else if ( isRegister( str ) )
            token->setType( TokenType_RegisterType );
        else if ( isNumber( str ) )
            token->setType(TokenType_NumberType);
        else if ( str[0] == '$' )
            token->setType(TokenType_LabelType);
        else {
            if( i + 1 < strs.size() && strs[i + 1] == ":" )
                token->setType(TokenType_LabelType);
            else
                token->setType(TokenType_CommandType);
        }
        token->setName(str);
        tokens.push_back(token);
    }

    return tokens;
}

bool isOperator(char c) {
    const char* op = "+-*/<>=";
    for(int i = 0; i < strlen( op); i++ )
        if( op[i] == c )
            return true;

    return false;
}
bool isSymbols( char c ) {
    const char* symbols = ";:()[]#";
    for(int i = 0; i < strlen( symbols ); i++)
        if( symbols[i] == c )
            return true;
    return false;
}

bool isSpace( char c ){
    return c == ' ' || c == '\n' || c == '\t';
}

bool isRegister(string c) {
    return  c == "ra" ||
            c == "rb" ||
            c == "rc" ||
            c == "rd" ||
            c == "re" ||
            c == "rf" ||
            c == "rg" ||
            c == "rh" ||
            c == "ri" ||
            c == "rj" ||
            c == "rk" ||
            c == "rl" ||
            c == "rm" ||
            c == "rn" ||
            c == "rs" ||
            c == "rp" ;
}

bool isNumber(string c) {
    const char* number = "+-0123456789";
    if( strncmp( c.c_str(), "0x", 2 ) == 0 )
    {
        number = "+-0123456789abcdef";
        for(int i = 2; i < c.size(); i++) {
            bool contain = false;
            for(int j = 0; j < strlen(number); j++)
                if( c[i] == number[j] )
                    contain = true;
            if( !contain )
                return false;
        }
    } else {
        for(int i = 0; i < c.size(); i++) {
            bool contain = false;
            for(int j = 0; j < strlen(number); j++)
                if( c[i] == number[j] )
                    contain = true;
            if( !contain )
                return false;
        }
    }

    return true;
}

bool isWords(char c) {
    const char* words = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxxyz0123456789_";
    for( int i = 0; i < strlen( words ); i++ )
        if( words[i] == c )
            return true;

    return false;
}










