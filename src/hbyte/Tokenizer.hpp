#pragma once

#include "Token.hpp"

class Tokenizer {
public:
    static vector<Token*> Extract(string instruction);
};
