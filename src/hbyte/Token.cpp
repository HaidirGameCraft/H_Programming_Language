#include "Token.hpp"

void Token::setName( const string& name ) {
    this->name = name;
}

void Token::setType( TokenType type ) {
    this->type = type;
}

const string& Token::getName() {
    return this->name;
}

const TokenType& Token::getType() {
    return this->type;
}

void Token::print(int indent) {
    string str = this->getName();
    switch ( this->type ) {
        case TokenType_CommandType: str += " ( Command Type ) "; break;
        case TokenType_StringType: str += " ( String Type ) "; break;
        case TokenType_NumberType: str += " ( Number Type ) "; break;
        case TokenType_BracketType: str += " ( Bracket Type ) "; break;
        case TokenType_LabelType: str += " ( Label Type ) "; break;
        case TokenType_SymbolsType: str += " ( Symbols Type ) "; break;
        case TokenType_RegisterType: str += " ( Register Type ) "; break;
    }

    printf("%s\n", str.c_str());
}
