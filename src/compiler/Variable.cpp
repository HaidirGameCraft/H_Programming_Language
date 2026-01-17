#include "Variable.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include <cstdio>
#include <stack>
#include <tools/memory.h>
#include "Token.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/ControlStructure.hpp"
#include "compiler/Parser.hpp"
#include "compiler/Register.hpp"
#include "compiler/Stack.hpp"
#include "compiler/String.h"
#include "compiler/Tools.hpp"
#include <cassert>

vector<Variable*> Variable::globalVariable;
void Variable::setName( const string& name )
{
    this->name = name;
}

void Variable::setSize( int size )
{
    this->size = size;
}

void Variable::setDataType( Object* object )
{
    this->object = object;
}

void Variable::setValue( string str )
{
    this->value = str;
}

void Variable::setArray( const Array& array ) {
    this->array = array;
}

const string& Variable::getName() { return this->name; }
const int& Variable::getSize() { return this->size; }
const string& Variable::getValue() { return this->value; }
Object* Variable::getDataType() { return  this->object; }


void Variable::pushVariable( string name, int size, DataTypeToken* datatype )
{
    Variable* var = (Variable*) __malloc( sizeof( Variable ) );

    int __s = datatype->getDataType()->size;
    int stack = 4;

    var->setName( name );
    var->setSize( __s );
    var->setDataType( datatype->getDataType() );

    if( Function::current_token != nullptr )
        if( ControlStructure::current_controlStruct != nullptr ) {
            ControlStructure::current_controlStruct->variables.push_back( var ); 
        } else {
            Stack* stack = Stack::currentStack;
            stack->PushVariable( var );
        }
    else {
        var->isGlobal = true;
        Variable::globalVariable.push_back( var );
    }
}

void Variable::pushVariable( Variable* var )
{
    if( Function::current_token != nullptr )
    {
        int stack = 4;
        Function* crt = (Function*) Function::current_token;
        Stack* __stack = (Stack*) Stack::currentStack;
        __stack->PushVariable(var);
    }
    else {
        var->isGlobal = true;
        Variable::globalVariable.push_back( var );
    }
}

Variable* Variable::findVariable(string name)
{
    if( Function::current_token != nullptr ) {
        Variable* var = Function::current_token->getVariable(name);
        if( var != nullptr )
            return var;
    }

    for(Variable* var : Variable::globalVariable)
        if( var->name == name )
            return var;
    return nullptr;
}

vector<string> Variable::asmData() {
    vector<string> asm_str;
    for(int i = 0; i < Variable::globalVariable.size(); i++)
    {
        Variable* var = Variable::globalVariable[i];
        asm_str.push_back(var->name + ":");

        if( var->isArray )
        {
            int __size = var->getSize();
            if( var->getDataType()->getName() == "int") {

                if( var->provideReserved )
                    asm_str.push_back("=rq " + to_string(__size ));
                else {
                    if ( var->array.tokens.size() > 0 )
                    {
                        int __idx_items = 0;
                        //for(Token* token : var->array.tokens )
                        //    token->print(0);
                        //assert(false);
                        while( __idx_items < var->array.tokens.size() )
                        {
                            asm_str.push_back("=quad " + var->array.tokens[__idx_items]->getName() );
                        //assert(false);
                            __idx_items++;
                            __size -= 4;
                        }
                        var->array.CleanUp();
                    }

                    if( (int)( __size / 4 ) > 0 )
                        asm_str.push_back("=rq " + to_string((int)( __size / 4 )) );
                }
            }
            else if ( var->getDataType()->getName() == "short" )
            {
                if( var->provideReserved )
                    asm_str.push_back("=rd " + to_string( __size ));
                else {
                    if ( var->array.tokens.size() > 0 )
                    {
                        int __idx_items = 0;
                        while( __idx_items < var->array.tokens.size() )
                        {
                            asm_str.push_back("=double " + var->array.tokens[__idx_items]->getName() );
                            __idx_items++;
                            __size -= 2;
                        }
                        var->array.CleanUp();
                    }

                    if( (int)( __size / 2 ) > 0 )
                        asm_str.push_back("=rd " + to_string((int)( __size / 2 )) );
                }
            } else if ( var->getDataType()->getName() == "string" ) {
                if( var->array.tokens.size() > 0 ) {
                    for( int i = 0; i < var->array.tokens.size(); i++ ) {
                        asm_str.push_back("=ascii " + var->array.tokens[i]->getName() );
                    }
                }
            } 
            else if ( var->getDataType()->getName() == "char" || var->getDataType()->getName() == "bool" )
            {
                if( var->getDataType()->getName() == "char" ) {
                    int str_length = var->getValue().size() - 2; // ignoring quote '"'
                    if( str_length > 0 )
                        asm_str.push_back("=ascii " + var->getValue());
                    if( __size - str_length > 0 )
                        asm_str.push_back("=rs " + to_string( __size - str_length ) );
                }
                else {
                    if( var->provideReserved )
                        asm_str.push_back("=rs " + to_string( __size ));
                    else {
                        if ( var->array.tokens.size() > 0 )
                        {
                            int __idx_items = 0;
                            while( __idx_items < var->array.tokens.size() )
                            {
                                asm_str.push_back("=single " + var->array.tokens[__idx_items]->getName() );
                                __idx_items++;
                                __size -= 1;
                            }
                            var->array.CleanUp();
                        }

                        if( (int)( __size / 1 ) > 0 )
                            asm_str.push_back("=rs " + to_string((int)( __size / 1 )) );
                        }
                }
            }
        }
        else {
            if( var->getDataType()->getName() == "int" )
                asm_str.push_back("=quad " + var->getValue());
            else if( var->getDataType()->getName() == "short" )
                asm_str.push_back("=double " + var->getValue());
            else if( var->getDataType()->getName() == "char" )
                asm_str.push_back("=single " + var->getValue());
            else if( var->getDataType()->getName() == "bool" )
                asm_str.push_back("=single " + var->getValue());
        }
    }

    return asm_str;
}

void Variable::cleanup() {
    for( Variable* var : Variable::globalVariable ) {
        var->array.CleanUp();
        __free(var);
    }
}

vector<string> Variable::VariableCompiler( vector<Token*> tokens ) {
    vector<string> asm_str;
    DataTypeToken* datatype_token = ( DataTypeToken* ) tokens[0];
    Token* var_token = (Token*) tokens[1];
    vector<Token*> array_tokens;
    
    bool isArray = false;
    // Where it has '[' after var_token 
    // if it has bracket symbols mean that it is Array
    if( 2 < tokens.size() && tokens[2]->getType() == TokenType_BracketType && tokens[2]->getName() == "[") {
        isArray = true;
        int array_start = 3;    // set it as 3 for starter because we want to skip the open bracket '['
        BracketToken* bracketToken = (BracketToken*) tokens[2];
        // Scaanning until finding the closed bracket ']';
        for(int __i = array_start; __i < bracketToken->closeBracket; __i++)
        {
            if( tokens[__i]->getType() == TokenType_BracketType &&
                    tokens[__i]->getName() == "]" )
                break;

            // push the non-bracket ']' into tokens
            array_tokens.push_back( tokens[__i] );
        }
    }

    // Initialize Variable
    Variable* var = (Variable*) __malloc( sizeof( Variable ) );
    var->setName(var_token->getName());
    var->isArray = isArray;
    var->setSize( datatype_token->getDataType()->getSize() );
    var->setDataType(datatype_token->getDataType());
    Variable::pushVariable( var );

    // When it has Assign Type or containing Bracket Type
    // Meaning that the Variable has to be Initializing
    if( 2 < tokens.size() )
    {
        vector<Token*> logic_maths_tokens;
        Token* parser = nullptr;
        Array array;
        int assign_symbols_idx = 0;
        bool hasAssignToken = false;
        // Find the Equals Symbols ( Assign Symbols )
        for(assign_symbols_idx = 0; assign_symbols_idx < tokens.size(); assign_symbols_idx++)
            if( tokens[assign_symbols_idx]->getName() == "=" )
            {
                hasAssignToken = true;
                assign_symbols_idx++;
                break;
            }
        
        // After that, push it into logic_maths_tokens to parser
        if( hasAssignToken && tokens[assign_symbols_idx]->getType() == TokenType_BracketType && tokens[assign_symbols_idx]->getName() == "[")
        {
            Array::GetArrayTokens(&array, &tokens, assign_symbols_idx);
            var->setArray(array);
        }
        else {
            for(int i = assign_symbols_idx; i < tokens.size(); i++)
                logic_maths_tokens.push_back( tokens[i] );

            // this parser mean that the logic math
            // meaning that it parser after finding AssignToken
            parser = Parser::ParserExp(logic_maths_tokens); 
            //parser->print(0);
        }
        // Global Variable which is Constant Value
        if ( var->isGlobal ) {

            if( isArray )
            {
                // Parser the index tokens that we push it earlier
                Token* parser_index_array = Parser::ParserExp(array_tokens);

                // when it has number, set the size and set providingReserved is True ( when it doesn't have initialize items)
                if( parser_index_array != nullptr && parser_index_array->getType() == TokenType_NumberType ) {
                    var->setSize( stoi( parser_index_array->getName() ) * datatype_token->getDataType()->getSize());
                    if ( array.tokens.size() == 0 )
                        var->provideReserved = true;
                }
                else if ( array.tokens.size() > 0 ) {
                    var->setSize( array.tokens.size() * datatype_token->getDataType()->getSize() );
                }

                // get an Error when Both of Token Parser it null
                if( parser == nullptr && parser_index_array == nullptr && array.tokens.size() == 0 )
                {
                    assert(false &&"ArrayError: Could not know the size of array should be provided.");
                }
            }

            var->isConstant = true;
            if( parser != nullptr ) {
                assert(parser->getType() == TokenType_NumberType || parser->getType() == TokenType_StringType);
                var->setValue( parser->getName() );
            }
        }
        else {
            // Defined as Local Variable

            if( isArray ) {
                Token* parser_index_array = Parser::ParserExp(array_tokens);
                if( parser_index_array != nullptr && parser_index_array->getType() == TokenType_NumberType ) {
                    var->setSize( stoi( parser_index_array->getName() ) * datatype_token->getDataType()->getSize() );
                    if( array.tokens.size() == 0 )
                        var->provideReserved = true;
                }
                else if ( array.tokens.size() > 0 )
                {
                    var->setSize( array.tokens.size() * datatype_token->getDataType()->getSize() );
                }
                else if ( parser->getType() == TokenType_VariableType )
                {
                    Variable* parser_var = Variable::findVariable( parser->getName() );
                    assert( parser_var->getDataType() == var->getDataType() &&
                            parser_var->isArray == isArray );
                    var->setValue( parser_var->getName() );
                    var->setSize(4);
                    var->isPointer = true;
                }
                else if ( parser->getType() == TokenType_StringType )
                {
                    int strlength = parser->getName().size() - 2; // Ignoring the quote of String
                    var->setSize( 4 );

                    string string_label_name = GenerateRandomName() + "_" + var_token->getName();
                    var->setValue( string_label_name );
                    var->setDataType( Object::getObject("char"));
                    var->isPointer = true;

                    String::pushString(string_label_name, parser->getName().substr(1, strlength ));
                }

                Function* crt_func = Function::current_token;
                int stack = Stack::currentStack->getStackIndex( var->getName() );
                // for(int i = 0; i < crt_func->local_variables.size(); i++) {
                //     int size = crt_func->local_variables[i]->size;
                //     size = (int)( size / 4 ) + ( size % 4 > 0 );
                //     stack += size * 4;
                // }
                //var->stack_index = -stack;
                // printf("Final stack for %s is %i\n", var->getName().c_str(), var->stack_index);
                // get an Error when Both of Token Parser it null
                if( parser == nullptr && parser_index_array == nullptr && array.tokens.size() == 0 )
                {
                    printf("ArrayError: Could not know the size of array should be provided.");
                    assert(false);
                }

                asm_str.push_back("sub " + to_string( var->size ) + " -> rs" );
                if( parser->getType() == TokenType_StringType )
                {
                    Register* __reg = Register::getAllocRegister();
                    asm_str.push_back("mov $" + var->getValue() + " -> " + __reg->name );
                    asm_str.push_back("str " + __reg->name + " -> rp(" + to_string( var->stack_index ) + ")" );
                    Register::UnusedRegister( __reg );
                } else if ( parser->getType() == TokenType_VariableType )
                {
                    Variable* parser_var = Variable::findVariable( parser->getName() );
                    assert( parser_var->getDataType() == var->getDataType() &&
                            parser_var->isArray == isArray );
                     
                    if( var->isPointer )
                    {
                        int stackIdx = Stack::getStackIndex( var->getName() );
                        if( parser_var->isGlobal) {
                            Register* __reg = Register::getAllocRegister();
                            asm_str.push_back("mov $" + parser_var->getName() + " -> " + __reg->name );
                            asm_str.push_back("str " + __reg->name + " -> rp(" + to_string( stackIdx ) + ")" );
                            Register::UnusedRegister( __reg );
                        }
                        else {
                            vector<string> __tmp_code = Compiler::CompileParser(parser);
                            Register* __reg = Register::popRegister();
                            for( string& str : __tmp_code )
                                asm_str.push_back( str );
                            asm_str.push_back("str " + __reg->name + " -> rp(" + to_string( stackIdx ) + ")" );
                            Register::UnusedRegister( __reg );
                        }
                            
                    }
                }
                else {
                    int item_index = 0;
                    for( Token* items_parser : array.tokens ) {
                        vector<string> items_code_ = Compiler::CompileParser(items_parser);
                        Register* items_reg = Register::popRegister();

                        int data_size = datatype_token->getDataType()->getSize();
                        int stackIdx = Stack::getStackIndex( var->getName() );
                        if( data_size == 4 )        items_code_.push_back( "str " + items_reg->name + " -> rp(" + to_string( stackIdx + data_size * item_index ) + ")" );
                        else if( data_size == 2 )   items_code_.push_back("strd " + items_reg->name + " -> rp(" + to_string( stackIdx + data_size * item_index ) + ")" );
                        else if( data_size == 1 )   items_code_.push_back("strs " + items_reg->name + " -> rp(" + to_string( stackIdx + data_size * item_index ) + ")" );

                        for(string& str : items_code_)
                            asm_str.push_back( str );

                        Register::UnusedRegister( items_reg );
                        item_index++;
                    }
                    array.CleanUp();
                }
            } else {

                Function* crt_func = Function::current_token;
                int stack = Stack::getStackIndex( var->getName() );
                /*
                for(int i = 0; i < crt_func->local_variables.size(); i++) {
                    int size = crt_func->local_variables[i]->size;
                    size = (int)( size / 4 ) + ( size % 4 > 0 );
                    stack += size * 4;
                }
                */
                // printf("Final stack for %s is %i\n", var->getName().c_str(), var->stack_index);
                // Compile the operation that have inside of parser ( logic_maths_tokens ) into string CODE
                vector<string> __asm_str = Compiler::CompileParser(parser);
                Register* reg = Register::popRegister();
                __asm_str.push_back("sub " + to_string(var->size) + " -> rs");
               
                if( var->getDataType()->getName() == "int" ) __asm_str.push_back("str " + reg->name + " -> rp(" + to_string( stack ) + ")");
                if( var->getDataType()->getName() == "short" ) __asm_str.push_back("strd " + reg->name + " -> rp(" + to_string( stack ) + ")");
                if( var->getDataType()->getName() == "char" || var->getDataType()->getName() == "bool" ) __asm_str.push_back("strs " + reg->name + " -> rp(" + to_string( stack ) + ")");
                Register::UnusedRegister(reg);
                for(string& str : __asm_str )
                    asm_str.push_back( str );
            }
        }
    }
    // Declare Variable but not Initialize it
    else {
        if( var->isGlobal )
        {
            // TODO
        } else {
            int stackIdx = Stack::getStackIndex( var->getName() );
            asm_str.push_back("sub " + to_string( stackIdx ) + " -> rs");
        }
    }

    for(Token* token : tokens)
        __free( token );
    return asm_str;
}


