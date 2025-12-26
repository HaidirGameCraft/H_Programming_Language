#pragma once

#include "Tokenizer.hpp"
#include "Token.hpp"

class Parser {
public:
    static Token* ParserExp( vector<Token*> tokens );
};
