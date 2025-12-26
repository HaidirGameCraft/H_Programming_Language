#include "Function.hpp"
#include <tools/memory.h>
#include "Token.hpp"
#include "Variable.hpp"
#include <assert.h>
#include <type_traits>

Function* Function::current_token = nullptr;
vector<Function*> Function::function_stack;
void FunctionToken::Init() {
    this->type = TokenType_FunctionType;
}




// Function
void Function::pushVariable( Variable* var ) {
    this->local_variables.push_back( var );
}

void Function::setName( const string& name ) { this->name = name; }
string Function::getName() { return this->name; }
void Function::initParams( vector<Token*> tokens )
{

    // this not including '(' and ')'
    if( tokens.size() == 0 )
        return;
;
    vector<Token*> arg;
    int start = 0;
    int stack = 8;
    for( int i = 0; i < tokens.size(); i++ )
    {
        Token* token = tokens[i];
        if( ( token->getType() == TokenType_SymbolsType && token->getName() == ",")  )
        {
            assert( arg[0]->getType() == TokenType_DataType );
            DataTypeToken* dataType = ( DataTypeToken* ) arg[0];
            Token* name_token = arg[1];

            Variable* var = (Variable*) __malloc( sizeof( Variable ));
            var->setName( name_token->getName() );
            var->setSize(4);
            var->stack_index = stack;
            stack += 4;

            this->parameters.push_back( var );
            //this->pushVariable( var );

            arg.clear();
        } else {
            arg.push_back( token );
        }
    }

    if( arg.size() != 0 )
    {
        assert( arg[0]->getType() == TokenType_DataType );
        DataTypeToken* dataType = ( DataTypeToken* ) arg[0];
        Token* name_token = arg[1];

        Variable* var = (Variable*) __malloc( sizeof( Variable ));
        var->setName( name_token->getName() );
        var->setSize(4);
        var->stack_index = stack;
        stack += 4;

        this->parameters.push_back( var );
            //this->pushVariable( var );

        arg.clear();
    }
}

Variable* Function::getVariable( const string& name ) {
    for( Variable* var : this->local_variables )
        if( var->name == name )
            return var;

    for( Variable* var : this->parameters )
        if( var->name == name )
            return var;

    return nullptr;
}

void Function::pushFunction(const string &name, Function *fun) {
    Function::function_stack.push_back( fun );
    Function::current_token = fun;
}

Function* Function::getFunction(const string &name) {
    
    for( Function* func : Function::function_stack )
        if( func->name == name )
            return func;

    return nullptr;
}

vector<string> Function::asmStart() {
    vector<string> asm_str;
    asm_str.push_back(this->name + ":");
    asm_str.push_back("push rp");
    asm_str.push_back("mov rs -> rp");
    return asm_str;
}

vector<string> Function::asmEnd() {
    vector<string> asm_str;
    int size = 0;
    for(Variable* var : this->local_variables )
        size += var->size;

    asm_str.push_back("add " + to_string( size ) + " -> rs");
    asm_str.push_back("pop rp");
    asm_str.push_back("ret");
    return asm_str;
}

vector<string> Function::codeGlobal() {
    vector<string> code_str;
    for( Function* func : Function::function_stack )
    {
        if( func->isGlobal )
            code_str.push_back("=global " + func->getName() );
    }
    return code_str;
}

void Function::cleanup() { 
    for(Function* function : Function::function_stack ) {
        for( Variable* var : function->local_variables )
            __free( var );

        for( Variable* var : function->parameters )
            __free( var );
    }
}

void Function::cleanLocalFunction() {
    for( Function* function : Function::function_stack )
    {
        if( !function->isGlobal )
        {
            for( Variable* var : function->local_variables )
                __free( var );
            for( Variable* var : function->parameters )
                __free( var );
        }
    }
}

