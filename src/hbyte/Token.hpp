#pragma once

#include <string>
#include <vector>

using namespace std;

namespace HByte {
enum TokenType {
    TokenType_CommandType,
    TokenType_SymbolsType,
    TokenType_NumberType,
    TokenType_OperationType,
    TokenType_StringType,
    TokenType_LabelType,
    TokenType_BracketType,
    TokenType_RegisterType,
};

class Token {
public:
    string name;
    TokenType type;

    int closeBracket = 0;
    bool hasOffset = false;
    vector<Token*> tokens;

    void setName( const string& name );
    void setType( TokenType type );

    const string& getName();
    const TokenType& getType();

    void print(int indent);
};

}
