#pragma once

#include "compiler/Token.hpp"
#include <vector>
#include <string>

using namespace std;

class Compiler {
public:
    static vector<string> Compile( string instruction );
    static vector<string> CompileTokens( vector<Token*> tokens );
    static vector<string> CompileParser( Token* parser );
};
