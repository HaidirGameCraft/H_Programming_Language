#include "Function.hpp"
#include <tools/memory.h>
#include "Token.hpp"
#include "Variable.hpp"
#include "compiler/Class.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/Parser.hpp"
#include "compiler/Register.hpp"
#include "compiler/Stack.hpp"
#include <assert.h>

Function* Function::current_token = nullptr;
vector<Function*> Function::function_stack;
void FunctionToken::Init() {
    this->type = TokenType_FunctionType;
}


void Function::setName( const string& name ) { this->name = name; }
string Function::getName() { return this->name; }
void Function::initParams( vector<Token*> tokens )
{

    // this not including '(' and ')'
    if( tokens.size() == 0 )
        return;

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
    Stack* stack = (Stack*) Stack::currentStack;
    for( Variable* var : stack->variables )
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

    std::string label_name_function = this->name;

    asm_str.push_back(label_name_function + ":");
    asm_str.push_back("push rp");
    asm_str.push_back("mov rs -> rp");
    return asm_str;
}

vector<string> Function::asmEnd() {
    vector<string> asm_str;
    int size = Stack::currentStack->getTotalStackSizeVariable();

    if( size > 0 )
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
        for( Variable* var : function->parameters )
            __free( var );
    }
}

void Function::cleanLocalFunction() {
    for( Function* function : Function::function_stack )
    {
        if( !function->isGlobal )
        {
            for( Variable* var : function->parameters )
                __free( var );
        }
    }
}

vector<string> Function::FunctionCompile( vector<Token*> tokens ) {
    Token* externToken = nullptr;
    Token* funcSymbolsToken = nullptr;
    vector<string> asm_str;
    int index_start_token = 0;

    if( tokens.front()->getName() == "extern" )
    {
        externToken = tokens[0];
        funcSymbolsToken = tokens[1];
        index_start_token++;
    }
    else {
        funcSymbolsToken = tokens[0];
    }

    if( externToken != nullptr )
    {
        assert( externToken->getName() == "extern" );
        FunctionToken* functionToken = (FunctionToken*) tokens[index_start_token + 1];  // get the FunctionType after func
        BracketToken* funcBracket = (BracketToken*) tokens[index_start_token + 2];      // get the Bracket of function
        DataTypeToken* dataTypeToken = (DataTypeToken*) tokens[ tokens.size() - 1 ];
        assert( funcBracket != nullptr && "Error: excepted no bracket after function name");    // give an error is bracket not found

        vector<Token*> parameters_args_token;   // save all the parameters datatype into this vector
        for(int i = index_start_token + 3; i < funcBracket->closeBracket; i++)
            parameters_args_token.push_back( tokens[i] );

        assert( Function::getFunction( functionToken->getName() ) == nullptr && "Function Name has already in declare");
        Function* function = (Function*) __malloc( sizeof( Function ) );
        function->setName( functionToken->getName() );
        function->initParams( parameters_args_token );
        function->ret_data = dataTypeToken->getDataType();
        Function::pushFunction( function->getName() , function );
        Function::current_token = nullptr;
        return {};
    }

    if ( funcSymbolsToken->getName() == "func") {
        FunctionToken* function_name_tk = (FunctionToken*) tokens[index_start_token + 1];

        assert( tokens[index_start_token + 2]->getName() == "(" && tokens[tokens.size() - 3]->getName() == ")");
        vector<Token*> inside_arg;
        for( int i = index_start_token + 3; i < tokens.size() - 3; i++)
            inside_arg.push_back( tokens[i] );

        assert( tokens[tokens.size() - 2]->getName() == "->");
        Token* symbols_tk = tokens[tokens.size() - 2];
        DataTypeToken* ret_data = (DataTypeToken*) tokens[tokens.size() - 1];

        // Check if the name is not in Function Stack
        Function* check_function = (Function*) Function::getFunction( function_name_tk->getName() );
        assert( check_function == nullptr && "Function Name has Already in Stack");

        Function* function = (Function*) __malloc(sizeof( Function ));
        function->isGlobal = tokens[0]->getType() == TokenType_GlobalType;      // set the sGlobal inside of function where the first tokens is Global Type

        function->setName( function_name_tk->getName() );
        // printf("[DEBUG Function] Initialize Parameters: size of inside: %i\n", inside_arg.size());
        function->initParams(inside_arg);
        function->ret_data = ret_data->getDataType();

        // Display the list of argument that sign 
        // for( Variable* arg : function->parameters )
        //    printf("%s ( %i )\n", arg->getName().c_str(), arg->size);

        Function::pushFunction(function->name, function);
        Function::current_token = function;
        asm_str = function->asmStart();
        Stack::CreateStack();
    }
    else if ( funcSymbolsToken->getName() == "return") {
        
        vector<Token*> tokens_after_return;
        // copy the array from not returntype into END
        for(int i = 1; i < tokens.size(); i++)
            tokens_after_return.push_back( tokens[i] );

        Token* parser = Parser::ParserExp(tokens_after_return);
        vector<string> __code = Compiler::CompileParser(parser);
        Register* reg = Register::popRegister();
        if( reg->name != "ra" )
            __code.push_back("mov " + reg->name + " -> ra");
        Register::UnusedRegister(reg);

        for(string& str : __code )
            asm_str.push_back( str );
    }
    else if ( funcSymbolsToken->getName() == "endfunc")
    {
        Function* func = Function::current_token;
        assert( func != nullptr );
        for( std::string& code : func->codeSegment )
            asm_str.push_back( code );

        func->codeSegment.clear();
        int stackSize = Stack::currentStack->getTotalStackSizeVariable();
        asm_str.push_back("add " + std::to_string( stackSize ) + " -> rs");
        asm_str.push_back("mov rp -> rs");
        asm_str.push_back("pop rp");
        asm_str.push_back("ret");
        Stack::RemoveStack();
        //assert( false );
        Function::current_token = nullptr;
    }

    for(Token* token : tokens )
        __free( token );

    return asm_str;
}
































