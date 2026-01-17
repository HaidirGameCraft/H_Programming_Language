#include "Token.hpp"
#include <cstdio>

Token::Token() {
    this->name = "";
    this->type = TokenType_NullType;
}

void Token::setName( const std::string& name ) {
    this->name = name;
}

void Token::setType( TokenType type ) {
    this->type = type;
}

std::string Token::getName() { return this->name; }
TokenType   Token::getType() { return this->type; }

void Token::print(int indent = 0) {

    for(int i = 0; i < indent;i++)
        printf("\t");

    std::string str = this->name;
    switch ( this->type ) { 
        case  TokenType_NullType:
            str += " ( Null Type ) ";
            break;
        case  TokenType_AssignType:
            str += " ( Assign Type ) ";
            break;
        case  TokenType_BracketType:
            str += " ( Bracket Type ) ";
            break;
        case  TokenType_NumberType:
            str += " ( Number Type ) ";
            break;
        case  TokenType_KeyWordType:
            str += " ( KeyWord Type ) ";
            break;
        case  TokenType_StringType:
            str += " ( String Type ) ";
            break;
        case  TokenType_VariableType:
            str += " ( Variable Type ) ";
            break;
        case  TokenType_FunctionType:
            str += " ( Function Type ) ";
            break;
        case  TokenType_ArrayType:
            str += " ( Array Type ) ";
            break;
        case  TokenType_OperatorType:
            str += " ( Operator Type ) ";
            break;
        case  TokenType_DataType:
            str += " ( DataType Type ) ";
            break;
        case  TokenType_PointerType:
            str += " ( Pointer Type ) ";
            break;
        case  TokenType_SymbolsType:
            str += " ( Symbols Type ) ";
            break;
    }

    printf("%s\n",  str.c_str() );
    if( this->left != nullptr )
    {
        for(int i = 0; i < indent;i++)
            printf("\t");
        printf("Left: ");
        this->left->print(indent + 1);
    }

    
    if( this->right != nullptr )
    {
        for(int i = 0; i < indent;i++)
            printf("\t");
        printf("Right: ");
        this->right->print(indent + 1);
    }
}
