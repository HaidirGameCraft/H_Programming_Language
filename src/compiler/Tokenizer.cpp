#include "Tokenizer.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include "Token.hpp"
#include <array>
#include <cctype>
#include <tools/memory.h>
#include <memory>
#include <string.h>

vector<Token*> Tokenizer::extract(string instruction) {
    
    vector<string> strs;
    int length = instruction.size();
    string __tmp = "";
    for(int i = 0; i < length; i++) {
        char c = instruction[i];

        if( c == ' ' || c == '\t' || c == '\n' )
        {
            if( __tmp.size() != 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            continue;
        }

        if( Tokenizer::isOperator( c ) )
        {
            if( __tmp.size() != 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            while( Tokenizer::isOperator(instruction[i] ) ) {
                __tmp += instruction[i];
                i++;
            }
            
            strs.push_back( __tmp);
            __tmp.clear();
        }
        else if ( c == '\"' )
        {

            if( __tmp.size() != 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            __tmp += "\"";
            i++;
            while( instruction[i] != '\"' )
            {
                __tmp += instruction[i];
                i++;
            }
            __tmp += "\"";
            strs.push_back(__tmp);
            __tmp.clear();
        }
        else if ( Tokenizer::isSymbols( c ) )
        {
            if( __tmp.size() != 0 )
            {
                strs.push_back( __tmp );
                __tmp.clear();
            }

            __tmp += c;
            strs.push_back( __tmp);
            __tmp.clear();
        } else {
            __tmp.push_back(c);
        } 
    }

    if( __tmp.size() > 0 )
        strs.push_back( __tmp );


//    for(string& str : strs)
//        printf("%s\n", str.c_str());
//    printf("\n");

    // printf("[DEBUG] Test 1.....");
    vector<Token*> tokens; 
    for( int i = 0; i < strs.size(); i++ )
    {
        Token* token = (Token*) __malloc( sizeof( Token ) );
        string str = strs[i];
        // Opening of Strifng
          if( Tokenizer::isOperator(str) ) 
          {
              token->setType( TokenType_OperatorType );
          }
          else if ( str == "=" )
          {
              token->setType( TokenType_AssignType );
          }
          else if ( Tokenizer::isConditionOperator(str) )
          {
              token->setType( TokenType_OperatorType );
          }
          else if ( str[0] == '\"' && str[str.size() - 1] == '\"' )
          {
              token->setType(TokenType_StringType);
          }
          else if ( str[0] == ')' || str[0] == '(' || str[0] == '[' || str[0] == ']')
          {
              __free( token );
              BracketToken* bracket_token = (BracketToken*) __malloc(sizeof(BracketToken));
              bracket_token->setType(TokenType_BracketType);
              bracket_token->setName(str);
              tokens.push_back( bracket_token );

              continue;
          }
          else if ( isIfOperand( str ) )
          {
              token->setType( TokenType::TokenType_IFType );
          }
          else if ( str == "global" )
              token->setType( TokenType_GlobalType );
          else if ( str == "import" )
              token->setType( TokenType_ImportType );
          else if ( str == "return" )
                token->setType( TokenType::TokenType_ReturnType );
          else if ( Tokenizer::isSymbols( str ) )
          {
              token->setType( TokenType_SymbolsType );
          }
          else if ( Tokenizer::isNumber( str ) )
          {
              token->setType( TokenType_NumberType );
          }
          else if ( Tokenizer::isDataType(str) )
          {
              __free( token );
              DataTypeToken* datatype = (DataTypeToken*) __malloc( sizeof(DataTypeToken) );
              datatype->setType(TokenType_DataType);
              datatype->setName( str );
              if( str == "void" ) datatype->setDataType( DataType_Void );
              else if ( str == "char" ) datatype->setDataType( DataType_Char );
              else if ( str == "short" ) datatype->setDataType( DataType_Short );
              else if ( str == "int" ) datatype->setDataType( DataType_Int );

              tokens.push_back( datatype );
              continue;
          }
          else if ( str == "func" || str == "endfunc" )
          {
              token->setType(TokenType_FuncSymbolType);
          }
          else {
                if( i + 1 < strs.size() && strs[i + 1] == "(" ) {
                    __free( token );
                    FunctionToken* functionToken = (FunctionToken*) __malloc( sizeof( FunctionToken ) );
                    functionToken->setType( TokenType_FunctionType );
                    functionToken->setName( str );
                    tokens.push_back( functionToken );
                    continue;
                }
                else if ( i + 1 < strs.size() && strs[i + 1] == "[" )
                {
                    __free( token );
                    ArrayToken* arrayToken = (ArrayToken*) __malloc( sizeof( ArrayToken ) );
                    arrayToken->setType( TokenType_ArrayType );
                    arrayToken->setName( str );
                    tokens.push_back( arrayToken );
                    continue;
                }
                else
                    token->setType(TokenType_VariableType);
          }

        token->setName( str );
        tokens.push_back( token );
    }
    return tokens;
}

bool Tokenizer::isOperator( char c ) {
    const char* symb = "+-*/<>=";
    for( int i = 0; i < strlen( symb ); i++ )
        if( symb[i] == c )
            return true;

    return false;
}

bool Tokenizer::isSymbols( char c ){
    const char* symb = "()[],.':;@";
    int size = strlen( symb );
    for( int i = 0; i < size; i++ )
        if( symb[i] == c )
            return true;

    return false;
}

bool Tokenizer::isIfOperand( string c ) {
    return c == "if" || c == "elif" || c == "else" || c == "endif";
}

bool Tokenizer::isConditionOperator( string c ) {
    return c == "==" || c == "&&" || c == "||" || c == "<" ||
        c == ">" || c == "<=" || c == ">=" || c == "!=";
}

bool Tokenizer::isDataType( string text ) {
    for( int i = 0; i < dataTypeName.size(); i++)
        if( dataTypeName[i] ==text )
            return true;

    return false;
}


bool Tokenizer::isOperator( string c ) {
    return  c == "+" ||
            c == "-" ||
            c == "*" ||
            c == "/";
}

bool Tokenizer::isSymbols( string c ){
    const char* symb = "()[],.@':;";
    int size = strlen( symb );
    for( int i = 0; i < size; i++ )
        if( symb[i] == c[0] )
            return true;

    return false;
}

bool Tokenizer::isNumber( string value ) {
    for( int i = 0; i < value.size(); i++ )
        if( std::isdigit( value[i] ) == 0 )
            return false;

    return true;
}





