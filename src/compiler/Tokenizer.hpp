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
    static void AnalysisBrackets( vector<Token*> tokens );
    static vector<Token*> AnalysisTokenizer( vector<Token*> tokens );

    static bool isOperator( char c );
    static bool isOperator( string c );
    static bool isKeyWord( string keyword );
    static bool isConditionOperator( string c );
    static bool isControlStructureOperand( string c );
    static bool isSymbols( char c );
    static bool isSymbols( string c );
    static bool isNumber( string value );
    static bool isAlpha( string value );

    static bool isDataType( string text );
};
