#pragma once

#include <stdint.h>
#include <stdio.h>
#include "Token.hpp"

class HByte {
public:
    static uint32_t pc;
    static void Compile(vector<Token*> tokens);
    static int getRegister( string c );
    static int getSymbols( string c );
};
