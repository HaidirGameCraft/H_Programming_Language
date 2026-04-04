#include "Stack.hpp"
#include "compiler/Variable.hpp"
#include "tools/memory.h"
#include "Function.hpp"
#include <cassert>

std::vector<Stack*> Stack::stacks;
Stack* Stack::currentStack = nullptr;

void Stack::CreateStack() {
    Stack* stack = (Stack*) __malloc( sizeof( Stack ) );
    Stack::stacks.push_back( stack );
    Stack::currentStack = stack;
}

void Stack::PushVariable(Variable *variable) {
    if( currentStack == nullptr )
        assert(false && "Error: CurrentStack is NULL");

    Stack::currentStack->variables.push_back( variable );
}

void Stack::clearVariables() {
    for( Variable* var : this->variables )
        __free( var );
    this->variables.clear();
}

int Stack::getTotalStackSizeVariable() {
    int stacks = 0;
    for( Variable* var : this->variables )
    {
        int size = var->getSize();
        stacks += ((int)( size / 4 ) + ( size % 4 > 0 ) ) * 4;
    }
    return stacks;
}

Variable* Stack::getVariable(const std::string &name) {
    // Find the variable in Local Variable
    for( Stack* stack : Stack::stacks )
        for( Variable* var : stack->variables )
            if( var->getName() == name )
                return var;
            
    // Find the variable in Formal Parameters
    if( Function::currentFunction != nullptr )
        for( Variable* var : Function::currentFunction->parameters )
            if( var->getName() == name )
                return var;

    // Find the variable in Global Variable
    for( Variable* var : Variable::globalVariable )
        if( var->getName() == name )
            return var;
    return nullptr;
}

int Stack::getStackIndex(const std::string &name) {
    int stackIdx = 0;
    if( Function::currentFunction != nullptr ) {
        stackIdx = 8;
        for( Variable* var : Function::currentFunction->parameters )
        {
            if( var->getName() == name )
            {
                return stackIdx;
            }
            else {
                int size = var->getSize();
                stackIdx += ((int)(size / 4) + (size % 4 > 0 ? 1 : 0)) * 4;
            }
        }
    }

    stackIdx = 4;
    for( Stack* stack : Stack::stacks ) {
        for( Variable* var : stack->variables )
        {
            if( var->getName() == name )
            {
                return -stackIdx;
            } else {
                int size = var->getSize();
                stackIdx += ((int)(size / 4) + (size % 4 > 0 ? 1 : 0)) * 4;
            }
        }
    }

    return -9999;
}

void Stack::RemoveStack() {
    Stack* stack = Stack::currentStack;
    if( stack == nullptr )
        assert( false && "Error: There not has Stack left");

    Stack::stacks.pop_back();
    stack->clearVariables();
    __free( stack );
    if( Stack::stacks.size() > 0 )
        Stack::currentStack = Stack::stacks.back();
    else
        Stack::currentStack = nullptr;
}
