#pragma once

#include <vector>
#include <string>
#include <map>
#include "Token.hpp"

#include "Tokenizer.hpp"
#include "Variable.hpp"

using namespace std;

class FunctionToken : public Token {
public:
    vector<Token*> args;
    void Init();
};

class Function {
public:
    vector<Variable*> parameters;
    vector<Variable*> local_variables;
    
    string name;
    DataTypeToken ret_data;
    bool isGlobal = false;

    void setName( const string& name );
    string getName();

    static vector<Function*> function_stack;
    static Function* current_token;

    void initParams( vector<Token*> tokens );
    void pushVariable( Variable* var );
    static void cleanLocalFunction();
    static void cleanup();
    Variable* getVariable( const string& name );
    vector<string> asmStart();
    vector<string> asmEnd();

    static void pushFunction(const string& name, Function* fun );
    static Function* getFunction( const string& name );
    static vector<string> codeGlobal();
};
