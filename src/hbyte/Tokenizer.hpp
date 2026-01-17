#pragma once

#include "Token.hpp"

namespace HByte {
    class Tokenizer {
    public:
        static vector<Token*> Extract(string instruction);
    };
}
