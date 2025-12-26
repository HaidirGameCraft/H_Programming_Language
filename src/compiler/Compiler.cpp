#include "Compiler.hpp"
#include "Array.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include "IfCondition.hpp"
#include "Register.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Tools.hpp"
#include "Variable.hpp"
#include "compiler/String.h"
#include <tools/memory.h>
#include <cassert>
#include <cstring>

vector<string> CompileToken( Token* token );

vector<string> ReturnCompile( vector<Token*> tokens );
vector<string> VariableCompile( vector<Token*> tokens );
vector<string> FunctionCompile( vector<Token*> tokens );
vector<string> IfConditionCompile( vector<Token*> tokens );


vector<string> Compiler::Compile(string instruction) {
    vector<Token*> tokens = Tokenizer::extract(instruction);
    if( tokens.size() == 0 )
        return {};

    vector<string> codes = Compiler::CompileTokens(tokens);
    if( IfCondition::current_ifconds != nullptr )
    {
        IfCondition* ifCondition = IfCondition::current_ifconds;
        if( codes.size() > 0 )
            for(string& code : codes)
                ifCondition->pushCode(code);
        return {};
    }
    
    return  codes;
}

vector<string> Compiler::CompileTokens( vector<Token*> tokens ) {
    if( tokens.size() == 0 )
        return {};

    if( tokens.front()->getType() == TokenType_DataType )
        return VariableCompile(tokens);
    else if ( tokens.front()->getType() == TokenType_ReturnType )
        return ReturnCompile(tokens);
    else if ( ( tokens.front()->getType() == TokenType_GlobalType && 
            tokens[1]->getType() == TokenType_FuncSymbolType ) || tokens.front()->getType() == TokenType_FuncSymbolType )
        return FunctionCompile(tokens);
    else if ( tokens.front()->getType() == TokenType_IFType )
        return IfConditionCompile(tokens);

    Token* parser = Parser::ParserExp(tokens);
    vector<string> asm_str = CompileToken( parser );

    Register::UnusedAllRegister();
    for(Token* token : tokens )
        __free( token );

    return asm_str;
}

vector<string> CompileToken( Token* token ) {
    vector<string> asm_str;
    if( token->getType() == TokenType_OperatorType )
    {
        Token* left_token = token->left;
        Token* right_token = token->right;
        bool isRightOperator = false;
        bool isLeftOperator = false;

        assert( left_token != nullptr && right_token != nullptr );

        if( right_token->getType() == TokenType_OperatorType ||
            right_token->getType() == TokenType_VariableType ||
            right_token->getType() == TokenType_FunctionType )
        {
            vector<string> __asm_str = CompileToken( right_token );
            for(auto& str : __asm_str )
                asm_str.push_back( str );

            isRightOperator = true;
        }

        if( left_token->getType() == TokenType_OperatorType ||
            left_token->getType() == TokenType_VariableType ||
            left_token->getType() == TokenType_FunctionType )
        {
            vector<string> __asm_str = CompileToken( left_token );
            for(auto& str : __asm_str )
                asm_str.push_back( str );

            isLeftOperator = true;
        }

        if( right_token->getType() == TokenType_NumberType )
            Register::pushRegister( Register::getAllocRegister() );

        if( left_token->getType() == TokenType_NumberType )
            Register::pushRegister( Register::getAllocRegister() );

        Register* left_reg = nullptr;
        Register* right_reg = nullptr;

        if( isRightOperator && isLeftOperator )
        {
            left_reg = Register::popRegister();
            right_reg = Register::popRegister();
        }
        else if ( isRightOperator )
        {
            left_reg = Register::popRegister();
            right_reg = Register::popRegister();

            asm_str.push_back("mov " + left_token->getName() + " -> " + left_reg->name);
        } else if ( isLeftOperator )
        {
            right_reg = Register::popRegister();
            left_reg = Register::popRegister();

            asm_str.push_back("mov " + right_token->getName() + " -> " + right_reg->name);
        } else {
            left_reg = Register::popRegister();
            right_reg = Register::popRegister();

            
            asm_str.push_back("mov " + right_token->getName() + " -> " + right_reg->name);
            asm_str.push_back("mov " + left_token->getName() + " -> " + left_reg->name);
        }

        assert( left_reg != nullptr && right_reg != nullptr );

        if( token->getName() == "+" ) asm_str.push_back("add " + right_reg->name + " -> " + left_reg->name );
        else if( token->getName() == "-" ) asm_str.push_back("sub " + right_reg->name + " -> " + left_reg->name );
        else if( token->getName() == "*" ) asm_str.push_back("mul " + right_reg->name + " -> " + left_reg->name );
        else if( token->getName() == "/" ) asm_str.push_back("div " + right_reg->name + " -> " + left_reg->name );
        else if( token->getName() == "==" ) asm_str.push_back("cnd " + right_reg->name + " == " + left_reg->name);
        else if( token->getName() == "<" ) asm_str.push_back("cnd " + right_reg->name + " > " + left_reg->name);
        else if( token->getName() == ">" ) asm_str.push_back("cnd " + right_reg->name + " < " + left_reg->name);
        else if( token->getName() == "<=" ) asm_str.push_back("cnd " + right_reg->name + " >= " + left_reg->name);
        else if( token->getName() == ">=" ) asm_str.push_back("cnd " + right_reg->name + " <= " + left_reg->name);
        Register::UnusedRegister(right_reg);
        Register::pushRegister(left_reg);
    }
    else if ( token->getType() == TokenType_AssignType )
    {
        Variable* left_var = nullptr;
        Variable* right_var = nullptr;
        bool isRightOperator = false;

        if( token->right != nullptr )
        {
            vector<string> __asm_str = CompileToken( token->right );
            for(string& str : __asm_str)
                asm_str.push_back( str );
        }
        if( token->left != nullptr )
        {
            left_var = Variable::findVariable( token->left->getName() );
            assert( left_var != nullptr );
        }

        Register* right_reg = Register::popRegister();

        DataType data_type = left_var->getDataType();
        if( left_var->isGlobal ) {
            if( left_var->isArray ) {
                Token* parser_idx = Parser::ParserExp( ((ArrayToken*) token->left)->tokens );
                Register* reg = Register::getAllocRegister();
                asm_str.push_back("mov $" + left_var->getName() + " -> " + reg->name);
                if( parser_idx->getType() == TokenType_NumberType )
                {
                    int size = DataTypeSize(data_type);
                    asm_str.push_back("add " + to_string( stoi(parser_idx->getName()) * size ) + " -> " + reg->name );
                } else {
                    vector<string> __code = CompileToken(parser_idx);
                    for(string& str : __code)
                        asm_str.push_back( str );
                    Register* prev_reg = Register::popRegister();

                    int size = DataTypeSize(data_type);
                    if( data_type == DataType_Int ) asm_str.push_back("mul 4 -> " + prev_reg->name );
                    else if( data_type == DataType_Short ) asm_str.push_back("mul 2 -> " + prev_reg->name );

                    asm_str.push_back("add " + prev_reg->name + " -> " + reg->name );
                    Register::UnusedRegister( prev_reg );
                }

                if( data_type == DataType_Int ) asm_str.push_back("str " + right_reg->name + " -> " + reg->name );
                else if( data_type == DataType_Short ) asm_str.push_back("strd " + right_reg->name + " -> " + reg->name );
                else if( data_type == DataType_Char ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                else if( data_type == DataType_Bool ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                Register::UnusedRegister(reg);
            } else {

                if( data_type == DataType_Int ) asm_str.push_back("str " + right_reg->name + " -> $" + left_var->name );
                else if( data_type == DataType_Short ) asm_str.push_back("strd " + right_reg->name + " -> $" + left_var->name );
                else if( data_type == DataType_Char ) asm_str.push_back("strs " + right_reg->name + " -> $" + left_var->name );
                else if( data_type == DataType_Bool ) asm_str.push_back("strs " + right_reg->name + " -> $" + left_var->name );
            }
        }
        else {
            // When the Left Variable is Local Variable
            if( left_var->isArray ) {   // When it is an Array
                Token* parser_idx = Parser::ParserExp( ((ArrayToken*) token->left)->tokens );
                Register* reg = Register::getAllocRegister();


                asm_str.push_back("mov rp  -> " + reg->name);
                if( left_var->stack_index < 0 )
                    asm_str.push_back("sub " + to_string( (left_var->stack_index * -1) ) + " -> " + reg->name );
                else
                    asm_str.push_back("add " + to_string( left_var->stack_index ) + " -> " + reg->name );


                // When inside of bracket 'var[____]' is Number
                if( parser_idx->getType() == TokenType_NumberType )
                {
                    int size = DataTypeSize(data_type);
                    asm_str.push_back("add " + to_string( stoi(parser_idx->getName()) * size ) + " -> " + reg->name );
                } else if( parser_idx->getType() == TokenType_VariableType || parser_idx->getType() == TokenType_OperatorType ) { // Otherwise, is not Number but Variable or Operation
                    vector<string> __code = CompileToken(parser_idx);
                    for(string& str : __code)
                        asm_str.push_back( str );
                    Register* prev_reg = Register::popRegister();

                    int size = DataTypeSize(data_type);
                    if( data_type == DataType_Int ) asm_str.push_back("mul 4 -> " + prev_reg->name);
                    if( data_type == DataType_Short ) asm_str.push_back("mul 2 -> " + prev_reg->name);

                    asm_str.push_back("add " + prev_reg->name + " -> " + reg->name );
                    Register::UnusedRegister( prev_reg );
                } else {
                    assert( false && "String is not support to be index");
                }

                if( data_type == DataType_Int ) asm_str.push_back("str " + right_reg->name + " -> " + reg->name );
                else if( data_type == DataType_Short ) asm_str.push_back("strd " + right_reg->name + " -> " + reg->name );
                else if( data_type == DataType_Char ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                else if( data_type == DataType_Bool ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                Register::UnusedRegister(reg);
            } else {    // When it is not An Array
                // Store all the register into rp with specific index/offset
                if( data_type == DataType_Int ) asm_str.push_back("str " + right_reg->name + " -> rp(" + to_string( left_var->stack_index ) + ")" );
                else if( data_type == DataType_Short ) asm_str.push_back("strd " + right_reg->name + " -> rp(" + to_string( left_var->stack_index ) + ")" );
                else if( data_type == DataType_Char ) asm_str.push_back("strs " + right_reg->name + " -> rp(" + to_string( left_var->stack_index ) + ")" );
                else if( data_type == DataType_Bool ) asm_str.push_back("strs " + right_reg->name + " -> rp(" + to_string( left_var->stack_index ) + ")" );
            }
        }
        
        // Clear right Register
        Register::UnusedRegister(right_reg);
    }
    else if ( token->getType() == TokenType_FunctionType )
    {
        Function* func = (Function*) Function::getFunction(token->getName());
        assert( func != nullptr );
        FunctionToken* func_tk = (FunctionToken*) token;
        for(int __j = func_tk->args.size() - 1; __j >= 0 ; __j-- )
        {
            vector<string> parser = CompileToken(func_tk->args[__j]);
            for(string& str : parser)
                asm_str.push_back( str );
            Register* reg = Register::popRegister();
            asm_str.push_back("push " + reg->name );
            Register::UnusedRegister(reg);
        }

        asm_str.push_back("call $" + func->getName() );

        int org_stack = 0;
        for(Variable* param_var : func->parameters)
            org_stack += param_var->size;
        if( org_stack > 0 )
            asm_str.push_back("add " + to_string(org_stack) + " -> rs");


        Register* reg = Register::getAllocRegister();
        if( reg->name != "ra" )
            asm_str.push_back("mov ra -> " + reg->name);
        Register::pushRegister(reg);
    }
    else if ( token->getType() == TokenType_NumberType )
    {
        Register* reg = Register::getAllocRegister();
        string value = token->getName();
        asm_str.push_back("mov " + value + " -> " + reg->name );
        Register::pushRegister( reg );
    }
    else if ( token->getType() == TokenType_VariableType )
    {
        Variable* var = Variable::findVariable( token->getName() );
        assert( var != nullptr );

        Register* reg = Register::getAllocRegister();
        if( var->isGlobal ) {
            DataType data_type = var->getDataType();
            if(data_type == DataType_Int) asm_str.push_back("lod $" + var->getName() + " -> " + reg->name);
            else if(data_type == DataType_Short) asm_str.push_back("lodd $" + var->getName() + " -> " + reg->name);
            else if(data_type == DataType_Char) asm_str.push_back("lods $" + var->getName() + " -> " + reg->name);
            else if(data_type == DataType_Bool) asm_str.push_back("lods $" + var->getName() + " -> " + reg->name);
        }
        else
            asm_str.push_back("lod rp(" + to_string(var->stack_index) + ") -> " + reg->name);

        Register::pushRegister( reg );
    }
    else if ( token->getType() == TokenType_ArrayType ) {
        ArrayToken* arrayToken = (ArrayToken*) token;
        Variable* array_var = Variable::findVariable( token->getName() );
        assert( array_var != nullptr );

        // parser_idx is The index in 'var[____]'
        Token* parser_idx = Parser::ParserExp(arrayToken->tokens);
        assert( parser_idx != nullptr );
        if( array_var->isGlobal )
        {
            DataType data_type = array_var->getDataType();
            int size_data = DataTypeSize( data_type );
            if( parser_idx->getType() == TokenType_NumberType ) {
                string __offset = to_string( stoi( parser_idx->getName() ) * size_data );
                Register* reg = Register::getAllocRegister();
                if(data_type == DataType_Int) asm_str.push_back("lod $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Short) asm_str.push_back("lodd $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Char) asm_str.push_back("lods $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Bool) asm_str.push_back("lods $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                Register::pushRegister(reg);
            } else if( parser_idx->getType() == TokenType_OperatorType && parser_idx->getType() == TokenType_VariableType ) {
                vector<string> code_idx = CompileToken(parser_idx);
                for(string& str : code_idx)
                    asm_str.push_back( str );
                
                Register* old_reg = Register::popRegister();
                if( data_type == DataType_Int ) asm_str.push_back("mul 4 -> " + old_reg->name );
                else if( data_type == DataType_Short ) asm_str.push_back("mul 2 -> " + old_reg->name );
                else if( data_type == DataType_Char || data_type == DataType_Bool ) asm_str.push_back("mul 1 -> " + old_reg->name );

                Register* label_reg = Register::getAllocRegister();
                asm_str.push_back( "mov $" + array_var->getName() + " -> " + label_reg->name );
                asm_str.push_back("add " + old_reg->name + " -> " + label_reg->name );
                Register::UnusedRegister( old_reg );

                Register* reg = Register::getAllocRegister();
                if(data_type == DataType_Int) asm_str.push_back("lod " + label_reg->name + " -> " + reg->name);
                else if(data_type == DataType_Short) asm_str.push_back("lodd " + label_reg->name + " -> " + reg->name);
                else if(data_type == DataType_Char || data_type == DataType_Bool ) asm_str.push_back("lods " + label_reg->name + " -> " + reg->name);
                Register::UnusedRegister( label_reg );
                Register::pushRegister(reg);
            } else {
                assert( false && " String, Function, and more are not support to be index");
            }
        } else {
            // when the variable is as Local Variable

            DataType data_type = array_var->getDataType();
            int size_data = DataTypeSize( data_type );
            Register* rp_reg = Register::getAllocRegister();
            asm_str.push_back( "mov rp -> " + rp_reg->name );
            if( array_var->stack_index < 0 )
                asm_str.push_back("sub " + to_string( array_var->stack_index * -1 ) + " -> " + rp_reg->name );
            else
                asm_str.push_back( "add " + to_string( array_var->stack_index ) + " -> " + rp_reg->name );

            // When it is Pointer, load the address of pointer
            if( array_var->isPointer )
                asm_str.push_back("lod " + rp_reg->name + " -> " + rp_reg->name );

            if( parser_idx->getType() == TokenType_NumberType ) {
                string __offset = to_string( stoi( parser_idx->getName() ) * size_data );
                Register* reg = Register::getAllocRegister();
                if(data_type == DataType_Int) asm_str.push_back("lod " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Short) asm_str.push_back("lodd " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Char) asm_str.push_back("lods " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type == DataType_Bool) asm_str.push_back("lods " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                Register::pushRegister(reg);
            } else if( parser_idx->getType() == TokenType_OperatorType && parser_idx->getType() == TokenType_VariableType ) {
                vector<string> code_idx = CompileToken(parser_idx);
                Register* old_reg = Register::popRegister();
                if( data_type == DataType_Int ) asm_str.push_back("mul 4 -> " + old_reg->name );
                else if( data_type == DataType_Short ) asm_str.push_back("mul 2 -> " + old_reg->name );
                else if( data_type == DataType_Char || data_type == DataType_Bool ) asm_str.push_back("mul 1 -> " + old_reg->name );

                asm_str.push_back("add " + old_reg->name + " -> " + rp_reg->name );
                Register::UnusedRegister( old_reg );

                Register* reg = Register::getAllocRegister();
                if(data_type == DataType_Int) asm_str.push_back("lod " + rp_reg->name + " -> " + reg->name);
                else if(data_type == DataType_Short) asm_str.push_back("lodd " + rp_reg->name + " -> " + reg->name);
                else if(data_type == DataType_Char || data_type == DataType_Bool ) asm_str.push_back("lods " + rp_reg->name + " -> " + reg->name);
                Register::pushRegister(reg);
            } else {
                assert( false && "Other than Variable, Operation and Number is not support to be index");
            }
            Register::UnusedRegister( rp_reg );
        }
    }

    return asm_str;
}


vector<string> VariableCompile( vector<Token*> tokens ) {
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

        // Scaanning until finding the closed bracket ']';
        for(int __i = array_start; __i < tokens.size(); __i++)
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
    var->setSize( DataTypeSize(datatype_token->data_type) );
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
        }
        // Global Variable which is Constant Value
        if ( var->isGlobal ) {

            if( isArray )
            {
                // Parser the index tokens that we push it earlier
                Token* parser_index_array = Parser::ParserExp(array_tokens);

                // when it has number, set the size and set providingReserved is True ( when it doesn't have initialize items)
                if( parser_index_array != nullptr && parser_index_array->getType() == TokenType_NumberType ) {
                    var->setSize( stoi( parser_index_array->getName() ) * DataTypeSize(datatype_token->getDataType()));
                    if ( array.tokens.size() == 0 )
                        var->provideReserved = true;
                }
                else if ( array.tokens.size() > 0 ) {
                    var->setSize( array.tokens.size() * DataTypeSize(datatype_token->getDataType()) );
                }
                // When it is StringType, set the size of variable as the length of string
                else if ( parser->getType() == TokenType_StringType )
                {
                    int strlength = parser->getName().size() - 2; // Ignoring the quote of String
                    var->setSize( strlength );
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
                    var->setSize( stoi( parser_index_array->getName() ) * DataTypeSize(datatype_token->getDataType()));
                    if( array.tokens.size() == 0 )
                        var->provideReserved = true;
                }
                else if ( array.tokens.size() > 0 )
                {
                    var->setSize( array.tokens.size() * DataTypeSize( datatype_token->getDataType() ) );
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
                    var->setDataType(DataType_Char);
                    var->isPointer = true;

                    String::pushString(string_label_name, parser->getName().substr(1, strlength ));
                }

                Function* crt_func = Function::current_token;
                int stack = 0;
                for(int i = 0; i < crt_func->local_variables.size(); i++) {
                    int size = crt_func->local_variables[i]->size;
                    printf("Size of Variable: %i\n", size);
                    stack += size;
                }
                var->stack_index = -stack;
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
                        if( parser_var->isGlobal) {
                            Register* __reg = Register::getAllocRegister();
                            asm_str.push_back("mov $" + parser_var->getName() + " -> " + __reg->name );
                            asm_str.push_back("str " + __reg->name + " -> rp(" + to_string( var->stack_index ) + ")" );
                            Register::UnusedRegister( __reg );
                        }
                        else {
                            vector<string> __tmp_code = CompileToken(parser);
                            Register* __reg = Register::popRegister();
                            for( string& str : __tmp_code )
                                asm_str.push_back( str );
                            asm_str.push_back("str " + __reg->name + " -> rp(" + to_string( var->stack_index ) + ")" );
                            Register::UnusedRegister( __reg );
                        }
                            
                    }
                }
                else {
                    int item_index = 0;
                    for( Token* items_parser : array.tokens ) {
                        vector<string> items_code_ = CompileToken(items_parser);
                        Register* items_reg = Register::popRegister();

                        int data_size = DataTypeSize( datatype_token->getDataType() );
                        if( data_size == 4 ) items_code_.push_back("str " + items_reg->name + " -> rp(" + to_string( var->stack_index + data_size * item_index ) + ")" );
                        else if( data_size == 2 ) items_code_.push_back("strd " + items_reg->name + " -> rp(" + to_string( var->stack_index + data_size * item_index ) + ")" );
                        else if( data_size == 1 ) items_code_.push_back("strs " + items_reg->name + " -> rp(" + to_string( var->stack_index + data_size * item_index ) + ")" );

                        for(string& str : items_code_)
                            asm_str.push_back( str );

                        Register::UnusedRegister( items_reg );
                        item_index++;
                    }
                    array.CleanUp();
                }
            } else {

                Function* crt_func = Function::current_token;
                int stack = 0;
                for(int i = 0; i < crt_func->local_variables.size(); i++) {
                    int size = crt_func->local_variables[i]->size;
                    // printf("Size of Variable: %i\n", size);
                    stack += size;
                }

                var->stack_index = -stack;
                // printf("Final stack for %s is %i\n", var->getName().c_str(), var->stack_index);
                // Compile the operation that have inside of parser ( logic_maths_tokens ) into string CODE
                vector<string> __asm_str = CompileToken(parser);
                Register* reg = Register::popRegister();
                __asm_str.push_back("sub " + to_string(var->size) + " -> rs");
                
                if( var->getDataType() == DataType_Int ) __asm_str.push_back("str " + reg->name + " -> rp(" + to_string( var->stack_index ) + ")");
                if( var->getDataType() == DataType_Short ) __asm_str.push_back("strd " + reg->name + " -> rp(" + to_string( var->stack_index ) + ")");
                if( var->getDataType() == DataType_Char || var->getDataType() == DataType_Bool ) __asm_str.push_back("strs " + reg->name + " -> rp(" + to_string( var->stack_index ) + ")");
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
            asm_str.push_back("sub " + to_string( var->size ) + " -> rs");
        }
    }

    for(Token* token : tokens)
        __free( token );
    return asm_str;
}

vector<string> FunctionCompile( vector<Token*> tokens ) {
    Token* type_function_tk = (Token*) tokens[0];
    vector<string> asm_str;
    int index_start_token = 0;
    
    if( tokens[0]->getType() == TokenType_GlobalType ) {
        type_function_tk = tokens[1];
        index_start_token = 1;
    }

    if ( type_function_tk->getName() == "func") {
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
        function->ret_data = *ret_data;
        Function::current_token = function;

        // Display the list of argument that sign 
        // for( Variable* arg : function->parameters )
        //    printf("%s ( %i )\n", arg->getName().c_str(), arg->size);

        Function::pushFunction(function->name, function);
        asm_str = function->asmStart();
        for(Token* tk : tokens )
            __free( tk );

        return asm_str;
    } else if ( type_function_tk->getName() == "endfunc")
    {
        Function* func = Function::current_token;
        assert( func != nullptr );
        asm_str = func->asmEnd();
        Function::current_token = nullptr;
    }

    for(Token* token : tokens )
        __free( token );

    return asm_str;
}

vector<string> ReturnCompile( vector<Token*> tokens ) {
    vector<string> code_str;

    Token* return_token = tokens[0];
    assert( return_token->getType() == TokenType::TokenType_ReturnType && return_token->getName() == "return");

    vector<Token*> tokens_after_return;
    // copy the array from not returntype into END
    for(int i = 1; i < tokens.size(); i++)
        tokens_after_return.push_back( tokens[i] );

    Token* parser = Parser::ParserExp(tokens_after_return);
    vector<string> __code = CompileToken(parser);
    Register* reg = Register::popRegister();
    if( reg->name != "ra" )
        __code.push_back("mov " + reg->name + " -> ra");
    Register::UnusedRegister(reg);

    for(string& str : __code )
        code_str.push_back( str );

    // Cleaning up tokens
    for(Token* token : tokens )
        __free( token );

    return code_str;
}

vector<string> IfConditionCompile( vector<Token*> tokens ) {
    Token* if_token = tokens[0];
    if( if_token->getType() == TokenType_IFType )
    {
        if( if_token->getName() == "else" )
        {
            assert( IfCondition::current_ifconds != nullptr );  // checking either the current if statement is not null as pointer
            IfCondition* ifCondition = IfCondition::current_ifconds;
            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();

            ifCondition->pushCondCode("go $" + label_name);

            ifCondition->pushCode("go $" + ifCondition->getEndifName());
            ifCondition->pushCode(label_name + ":");
            return {};
        }
        else if ( if_token->getName() == "endif" )
        {
            IfCondition* ifCondition = IfCondition::popIfCondition();
            ifCondition->pushCondCode("go $" + ifCondition->getEndifName());
            ifCondition->pushCode(ifCondition->getEndifName() + ":");
            
            vector<string> code;
            for(string& __code : ifCondition->cond_code)
                code.push_back( __code );

            for(string& __code : ifCondition->code )
                code.push_back( __code );

            __free( ifCondition );
            return code;
        }
    } 

    vector<int> brackets_idx;

    vector<Token*> condition_tokens;
    for(int i = 1; i < tokens.size(); i++) {
        Token* token = tokens[i];
        if( token->getType() == TokenType_BracketType )
        {
            if( token->getName() == "(" )
                brackets_idx.push_back(i);
            else if ( token->getName() == ")" )
            {
                if( brackets_idx.size() == 1 )
                {
                    int start = brackets_idx[0] + 1;    // Skipping '(' index
                    int end = i;

                    // Copy from tokens to condition_tokens
                    for(int __j = start; __j < end; __j++ )
                        condition_tokens.push_back( tokens[__j] );

                    brackets_idx.pop_back();
                    break;
                }
                
                brackets_idx.pop_back();
            }
        }
    }

    assert( brackets_idx.size() == 0 );     // checking the brackets_idx size is ZERO
    assert( condition_tokens.size() > 0 );  // checking where condition_tokens size is greater than ZERO or not Empty
                                            //
    Token* parser = Parser::ParserExp(condition_tokens);
    vector<string> condition_code_asm = CompileToken(parser);
   
    if( if_token->getType() == TokenType_IFType )
    {
        if( if_token->getName() == "if" )
        {
            IfCondition* ifCondition = (IfCondition*) __malloc( sizeof( IfCondition ) );
            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();

            ifCondition->setEndIfName("." +GenerateRandomName() + "_" + Function::current_token->getName());

            for(string& str : condition_code_asm)
                ifCondition->pushCondCode(str);
            ifCondition->pushCondCode("goc $" + label_name);

            ifCondition->pushCode(label_name + ":");
            IfCondition::pushIfCondition( ifCondition );
        }
        else if ( if_token->getName() == "elif" ) {
            assert( IfCondition::current_ifconds != nullptr );  // checking either the current if statement is not null as pointer
            IfCondition* ifCondition = IfCondition::current_ifconds;
            
            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();

            ifCondition->pushCode("go $" + ifCondition->getEndifName());
            for(string& str : condition_code_asm)
                ifCondition->pushCondCode(str);

            ifCondition->pushCode(label_name + ":");
            ifCondition->pushCondCode("goc $" + label_name);
        }
    }

    for(Token* token : tokens )
        __free( token );
    
    return {};
}









