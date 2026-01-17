#include "Tokenizer.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include "Token.hpp"
#include "compiler/Config.h"
#include "compiler/Variable.hpp"
#include <array>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <ios>
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
            while( instruction[i] == ' ' || instruction[i] == '\n' || instruction[i] == '\t' )
                i++;
            i--;    // reduce by One
            //__tmp += instruction[i];
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
          else if ( Tokenizer::isSymbols( str ) )
          {
              token->setType( TokenType_SymbolsType );
          }
          else if ( Tokenizer::isNumber( str ) )
          {
              token->setType( TokenType_NumberType );
          }
          else if ( Object::isDataType(str) )
          {
              __free( token );
              DataTypeToken* datatype = (DataTypeToken*) __malloc( sizeof(DataTypeToken) );
              datatype->setType(TokenType_DataType);
              datatype->setName( str );
              datatype->setDataType( Object::getObject( str ) );
              tokens.push_back( datatype );
              continue;
          }
          else if ( Tokenizer::isKeyWord( str ) )
          {
              token->setType(TokenType_KeyWordType);
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

    tokens = Tokenizer::AnalysisTokenizer( tokens );
    Tokenizer::AnalysisBrackets( tokens );
    return tokens;
}

void Tokenizer::AnalysisBrackets( vector<Token*> tokens ) {
    vector<BracketToken*> bracket_idx;
    for( int i = 0; i < tokens.size(); i++ )
    {
        Token* token = tokens[i];
        if( token->getType() == TokenType_BracketType )
        {
            BracketToken* brk_tk = (BracketToken*) token;
            if( brk_tk->getName() == "(" || brk_tk->getName() == "[")
                bracket_idx.push_back( brk_tk );
            else if ( brk_tk->getName() == ")" || brk_tk->getName() == "]")
            {
                if( bracket_idx.size() == 0 )
                {
                    assert( false && "BracketError: could not find the start of '(' or '['");
                }

                BracketToken* start_brk_tk = bracket_idx[ bracket_idx.size() - 1 ];
                bracket_idx.pop_back();

                start_brk_tk->closeBracket = i;
            }
        }
    }

    if( bracket_idx.size() != 0 )
    {
        printf("Size of Bracket idx: %i\n", bracket_idx.size() );
        assert( false && "BracketError: could not reach the end of ')' or ']'" );
    }
}

vector<Token*> Tokenizer::AnalysisTokenizer( vector<Token*> tokens ) {

    vector<Token*> newTokens;
    for( int i = 0; i < tokens.size(); i++ ) {
        Token* token = tokens[i];
        Token* leftToken = nullptr;
        Token* rightToken = nullptr;

        if ( i - 1 > 0 )
            leftToken = tokens[i - 1];
        if ( i + 1 < tokens.size() )
            rightToken = tokens[i + 1];


        if( token->getType() == TokenType_OperatorType )
        {
            // When it is reference
            if( token->getName() == "&" )
            {
                assert( rightToken != nullptr && "AnalysisError: Could not find right token for '&'");
                if( leftToken != nullptr )
                {
                    // When the left token is operation, condition or equal, we can make it as address
                    if( Tokenizer::isSymbols( leftToken->getName()[0] ) ||  Tokenizer::isOperator( leftToken->getName() ) || Tokenizer::isConditionOperator( leftToken->getName() ) || leftToken->getName() == "=" )
                    {
                        if( rightToken->getType() == TokenType_VariableType )
                        {
                            //rightToken->setName( rightToken->getName() );
                            rightToken->setType( TokenType_PointerType );
                            newTokens.push_back( rightToken );
                            __free( token );
                            i += 1; // skipping Variable Token
                            continue;
                        } else {
                            assert( false && "AnalysisError: On the Right Token, It is not Variable Type");
                        }
                    }
                }
                else {
                    //rightToken->setName( rightToken->getName() );
                    if( rightToken->getType() == TokenType_VariableType ) {
                        rightToken->setType( TokenType_PointerType );
                        newTokens.push_back( rightToken );
                        __free( token );
                        i += 1; // skipping Variable Token
                        continue;
                    }
                }
            }
            if( token->getName() == "*" )
            {
                assert( rightToken != nullptr && "AnalysisError: Could not find right token for '&'");
                if( leftToken != nullptr )
                {
                    // When the left token is operation, condition or equal, we can make it as address
                    if( Tokenizer::isSymbols( leftToken->getName()[0] ) ||  Tokenizer::isOperator( leftToken->getName() ) || Tokenizer::isConditionOperator( leftToken->getName() ) || leftToken->getName() == "=" )
                    {
                        if( rightToken->getType() == TokenType_VariableType )
                        {
                            //rightToken->setName( rightToken->getName() );
                            rightToken->setType( TokenType_PointerType );
                            newTokens.push_back( rightToken );
                            __free( token );
                            i += 1; // skipping Variable Token
                            continue;
                        } else {
                            assert( false && "AnalysisError: On the Right Token, It is not Variable Type");
                        }
                    }
                }
                else {
                    //rightToken->setName( rightToken->getName() );
                    if( rightToken->getType() == TokenType_VariableType ) {
                        rightToken->setType( TokenType_PointerType );
                        newTokens.push_back( rightToken );
                        __free( token );
                        i += 1; // skipping Variable Token
                        continue;
                    }
                }
            }

            else if ( token->getName() == "+" || token->getName() == "-" )
            {
                if( leftToken != nullptr ) {
                    if( Tokenizer::isOperator( leftToken->getName() ) || Tokenizer::isConditionOperator( leftToken->getName() ) || leftToken->getName() == "=" ) {
                        if( rightToken->getType() == TokenType_NumberType || rightToken->getType() == TokenType_VariableType || rightToken->getType() == TokenType_PointerType )
                        {
                            if( token->getName() == "-" )
                            {
                                if( leftToken->getName() == "-" ) {
                                    leftToken->setName("+");
                                    __free( token );
                                }
                                else if ( leftToken->getName() == "+" ) {
                                    leftToken->setName("-");
                                    __free( token );
                                }
                                else {
                                    // create new number ZERO
                                    Token* ZeroToken = ( Token* ) __malloc( sizeof( Token ) );
                                    ZeroToken->setType( TokenType_NumberType );
                                    ZeroToken->setName("0");
                                    newTokens.push_back( ZeroToken );
                                    newTokens.push_back( token );
                                }
                                continue;
                            }

                            __free( token );    // free
                            continue;
                        } else {
                            assert( false && "Error: Right is not Variable/Number or Pointer");
                        }
                    }
                }
            }
        }

        newTokens.push_back( token );   // push into new tokens
   }

    return newTokens;
}


bool Tokenizer::isKeyWord(string keyword) {
    int length = sizeof( KeyWordList ) / sizeof( const char* );

    for( int i = 0; i < length; i++ )
    {
        if( strcmp( KeyWordList[i], keyword.c_str()) == 0 ) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::isOperator( char c ) {
    const char* symb = "+-*/<>=&|";
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

bool Tokenizer::isControlStructureOperand( string c ) {
    return c == "if" || c == "elif" || c == "else" || c == "while" || c == "end";
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
            c == "/" ||
            c == "&" ||
            c == "|" ;
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





