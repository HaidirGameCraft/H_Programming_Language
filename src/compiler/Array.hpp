#pragma once

#include "Token.hpp"

class Array {
public:
    vector<Array*> arrays;
    vector<Token*> tokens;

    void CleanUp();

    static void GetArrayTokens( Array* array, vector<Token*>* tokens, int start_index );
};

class ArrayListToken : public Token {
public:
    // tokens after parser run
    vector<Token*> tokens;
};