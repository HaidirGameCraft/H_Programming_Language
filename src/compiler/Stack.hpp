#pragma once

#include <vector>
#include <string>

#include "Variable.hpp"

class Stack {
public:
    std::vector<Variable*> variables;
    static std::vector<Stack*> stacks;
    static Stack* currentStack;

    void clearVariables();
    int  getTotalStackSizeVariable();

    static void CreateStack();
    static void PushVariable(Variable* variable);
    static Variable* getVariable( const std::string& name );
    static int  getStackIndex( const std::string& name );
    static void RemoveStack();
};
