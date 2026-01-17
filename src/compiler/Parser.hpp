#pragma once

#include "Tokenizer.hpp"
#include "Token.hpp"

class Parser {
public:
    static bool make_easy_parser;
    static Token* ParserExp( vector<Token*> tokens );

    static int PrecedenceOperators( const std::string& symbols );
};
