#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include "Token.hpp"
#include <memory>

using namespace std;

class Tokenizer {
public:
    static vector<Token*> extract( string instruction );
private:
    static bool isOperator( char c );
    static bool isOperator( string c );
    static bool isConditionOperator( string c );
    static bool isIfOperand( string c );
    static bool isSymbols( char c );
    static bool isSymbols( string c );
    static bool isNumber( string value );
    static bool isAlpha( string value );

    static bool isDataType( string text );
};
