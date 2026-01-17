#include "Compiler.hpp"
#include "Array.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include "ControlStructure.hpp"
#include "Register.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Tools.hpp"
#include "Variable.hpp"
#include "compiler/Stack.hpp"
#include "compiler/String.h"
#include <tools/memory.h>
#include <cassert>
#include <cstring>

vector<string> CompileToken( Token* token );

vector<string> Compiler::CompileParser( Token* parser ) {
    return CompileToken( parser );
}

vector<string> Compiler::Compile(string instruction) {
    vector<Token*> tokens = Tokenizer::extract(instruction);
    if( tokens.size() == 0 )
        return {};

    vector<string> codes = Compiler::CompileTokens(tokens);
    if( ControlStructure::current_controlStruct != nullptr )
    {
        ControlStructure* ifCondition = ControlStructure::current_controlStruct;
        if( codes.size() > 0 )
            for(string& code : codes) {
                ifCondition->pushCode(code);
            }
        return {};
    }
    
    return  codes;
}

vector<string> Compiler::CompileTokens( vector<Token*> tokens ) {
    if( tokens.size() == 0 )
        return {};

    if( tokens.front()->getType() == TokenType_DataType )
        return Variable::VariableCompiler(tokens);
    else if ( 
        tokens.front()->getType() == TokenType_KeyWordType &&
        ( tokens.front()->getName() == "extern" ||
          tokens.front()->getName() == "func" || 
          tokens.front()->getName() == "endfunc" || 
          tokens.front()->getName() == "return"  )
    )
        return Function::FunctionCompile(tokens);
    else if ( 
                tokens.front()->getType() == TokenType_KeyWordType &&
                ( tokens.front()->getName() == "if" || tokens.front()->getName() == "else" || tokens.front()->getName() == "elif"
                  || tokens.front()->getName() == "while" || tokens.front()->getName() == "endwhile" || tokens.front()->getName() == "endif" )
        )
        return ControlStructure::SelectionControlCompiler(tokens);

    Token* parser = Parser::ParserExp(tokens);
    vector<string> asm_str = CompileToken( parser );

    Register::UnusedAllRegister();
    for(Token* token : tokens )
        __free( token );

    return asm_str;
}

vector<string> CompileToken( Token* token ) {
    vector<string> asm_str;
    if ( token->getType() == TokenType_OperatorType && ( token->getName() == "&&" || token->getName() == "||") )
    {
        Token* left_token = token->left;
        Token* right_token = token->right;

        string cnd_label = GenerateRandomName();
        string end_label = GenerateRandomName();

        if( token->getName() == "&&" ) {

            // CND_Label will be FALSE Label
            vector<string> left_cnd = CompileToken( left_token );
            Register* reg = Register::popRegister();
            if( left_token->getType() == TokenType_OperatorType && 
                    ( left_token->getName() == "&&" || left_token->getName() == "||" ) )
                left_cnd.push_back("cnd " + reg->name + " == 0");
            left_cnd.push_back("goc $" + cnd_label );
            Register::UnusedRegister( reg );

            vector<string> right_cnd = CompileToken( right_token );
            reg = Register::popRegister();
            if( right_token->getType() == TokenType_OperatorType && 
                    ( right_token->getName() == "&&" || right_token->getName() == "||" ) )
                left_cnd.push_back("cnd " + reg->name + " == 0");
            right_cnd.push_back("goc $" + cnd_label );

            for( string& str : left_cnd ) asm_str.push_back( str );
            for( string& str : right_cnd ) asm_str.push_back( str );
            asm_str.push_back("mov 1 -> " + reg->name);
            asm_str.push_back("go $" + end_label);
            asm_str.push_back(cnd_label + ":");  // False Label
            asm_str.push_back("mov 0 -> " + reg->name);
            asm_str.push_back(end_label + ":");
            Register::pushRegister( reg );
            
        }
        else if ( token->getName() == "||" ) {
            // CND_Label will be TRUE Label
            vector<string> left_cnd = CompileToken( left_token );
            Register* reg = Register::popRegister();
            if( left_token->getType() == TokenType_OperatorType &&
                    ( left_token->getName() == "&&" || left_token->getName() == "||" ))
                left_cnd.push_back("cnd " + reg->name + " != 0");
            left_cnd.push_back("goc $" + cnd_label );
            Register::UnusedRegister( reg );

            vector<string> right_cnd = CompileToken( right_token );
            reg = Register::popRegister();
            if( right_token->getType() == TokenType_OperatorType &&
                    ( right_token->getName() == "&&" || right_token->getName() == "||" ))
                right_cnd.push_back("cnd " + reg->name + " != 0");
            right_cnd.push_back("goc $" + cnd_label );

            for( string& str : left_cnd ) asm_str.push_back( str );
            for( string& str : right_cnd ) asm_str.push_back( str );
            asm_str.push_back("mov 0 -> " + reg->name);
            asm_str.push_back("go $" + end_label);
            asm_str.push_back(cnd_label + ":");  // TRUE Label
            asm_str.push_back("mov 1 -> " + reg->name);
            asm_str.push_back(end_label + ":");
            Register::pushRegister( reg );

        }
    }
    else if( token->getType() == TokenType_OperatorType )
    {
        Token* left_token = token->left;
        Token* right_token = token->right;
        bool isRightOperator = false;
        bool isLeftOperator = false;

        assert( left_token != nullptr && right_token != nullptr );

        if( right_token->getType() == TokenType_OperatorType ||
            right_token->getType() == TokenType_VariableType ||
            right_token->getType() == TokenType_PointerType ||
            right_token->getType() == TokenType_FunctionType )
        {
            vector<string> __asm_str = CompileToken( right_token );
            for(auto& str : __asm_str )
                asm_str.push_back( str );

            isRightOperator = true;
        }

        if( left_token->getType() == TokenType_OperatorType ||
            left_token->getType() == TokenType_VariableType ||
            left_token->getType() == TokenType_PointerType ||
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
        else if( token->getName() == "==" ) asm_str.push_back("cnd " + left_reg->name + " == " + right_reg->name);
        else if( token->getName() == "!=" ) asm_str.push_back("cnd " + left_reg->name + " != " + right_reg->name);
        else if( token->getName() == "<" )  asm_str.push_back("cnd " + left_reg->name + " < "  + right_reg->name);
        else if( token->getName() == ">" )  asm_str.push_back("cnd " + left_reg->name + " > "  + right_reg->name);
        else if( token->getName() == "<=" ) asm_str.push_back("cnd " + left_reg->name + " <= " + right_reg->name);
        else if( token->getName() == ">=" ) asm_str.push_back("cnd " + left_reg->name + " >= " + right_reg->name);

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
            left_var = Stack::getVariable( token->left->getName() );
            assert( left_var != nullptr );
        }

        Register* right_reg = Register::popRegister();

        Object* data_type = left_var->getDataType();
        if( left_var->isGlobal ) {
            if( left_var->isArray ) {
                Token* parser_idx = Parser::ParserExp( ((ArrayToken*) token->left)->tokens );
                Register* reg = Register::getAllocRegister();
                asm_str.push_back("mov $" + left_var->getName() + " -> " + reg->name);
                if( parser_idx->getType() == TokenType_NumberType )
                {
                    int size = data_type->getSize();
                    asm_str.push_back("add " + to_string( stoi(parser_idx->getName()) * size ) + " -> " + reg->name );
                } else {
                    vector<string> __code = CompileToken(parser_idx);
                    for(string& str : __code)
                        asm_str.push_back( str );
                    Register* prev_reg = Register::popRegister();

                    int size = data_type->getSize();
                    if( data_type->getName() == "int" ) asm_str.push_back("mul 4 -> " + prev_reg->name );
                    else if( data_type->getName() == "short" ) asm_str.push_back("mul 2 -> " + prev_reg->name );

                    asm_str.push_back("add " + prev_reg->name + " -> " + reg->name );
                    Register::UnusedRegister( prev_reg );
                }

                if( data_type->getName() == "int" ) asm_str.push_back("str " + right_reg->name + " -> " + reg->name );
                else if( data_type->getName() == "short" ) asm_str.push_back("strd " + right_reg->name + " -> " + reg->name );
                else if( data_type->getName() == "char" ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                else if( data_type->getName() == "bool" ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                Register::UnusedRegister(reg);
            } else {

                if( data_type->getName() == "int" ) asm_str.push_back("str " + right_reg->name + " -> $" + left_var->name );
                else if( data_type->getName() == "short" ) asm_str.push_back("strd " + right_reg->name + " -> $" + left_var->name );
                else if( data_type->getName() == "char" ) asm_str.push_back("strs " + right_reg->name + " -> $" + left_var->name );
                else if( data_type->getName() == "bool" ) asm_str.push_back("strs " + right_reg->name + " -> $" + left_var->name );
            }
        }
        else {
            // When the Left Variable is Local Variable
            if( left_var->isArray ) {   // When it is an Array
                Token* parser_idx = Parser::ParserExp( ((ArrayToken*) token->left)->tokens );
                Register* reg = Register::getAllocRegister();


                int leftStackIdx = Stack::getStackIndex( left_var->getName() );
                asm_str.push_back("mov rp  -> " + reg->name);
                if( left_var->stack_index < 0 )
                    asm_str.push_back("sub " + to_string( (leftStackIdx * -1) ) + " -> " + reg->name );
                else
                    asm_str.push_back("add " + to_string( leftStackIdx ) + " -> " + reg->name );


                // When inside of bracket 'var[____]' is Number
                if( parser_idx->getType() == TokenType_NumberType )
                {
                    int size = data_type->getSize();
                    asm_str.push_back("add " + to_string( stoi(parser_idx->getName()) * size ) + " -> " + reg->name );
                } else if( parser_idx->getType() == TokenType_VariableType || parser_idx->getType() == TokenType_OperatorType ) { // Otherwise, is not Number but Variable or Operation
                    vector<string> __code = CompileToken(parser_idx);
                    for(string& str : __code)
                        asm_str.push_back( str );
                    Register* prev_reg = Register::popRegister();

                    int size = data_type->getSize();
                    if( data_type->getName() == "int" ) asm_str.push_back("mul 4 -> " + prev_reg->name);
                    if( data_type->getName() == "short" ) asm_str.push_back("mul 2 -> " + prev_reg->name);

                    asm_str.push_back("add " + prev_reg->name + " -> " + reg->name );
                    Register::UnusedRegister( prev_reg );
                } else {
                    assert( false && "String is not support to be index");
                }

                if( data_type->getName() == "int" ) asm_str.push_back("str " + right_reg->name + " -> " + reg->name );
                else if( data_type->getName() == "short" ) asm_str.push_back("strd " + right_reg->name + " -> " + reg->name );
                else if( data_type->getName() == "char" ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                else if( data_type->getName() == "bool" ) asm_str.push_back("strs " + right_reg->name + " -> " + reg->name);
                Register::UnusedRegister(reg);
            } else {    // When it is not An Array
                // Store all the register into rp with specific index/offset
                int leftStackIdx = Stack::getStackIndex( left_var->getName() );
                if( data_type->getName() == "int")          asm_str.push_back( "str " + right_reg->name + " -> rp(" + to_string( leftStackIdx ) + ")" );
                else if( data_type->getName() == "short" )  asm_str.push_back("strd " + right_reg->name + " -> rp(" + to_string( leftStackIdx ) + ")" );
                else if( data_type->getName() == "char" )   asm_str.push_back("strs " + right_reg->name + " -> rp(" + to_string( leftStackIdx ) + ")" );
                else if( data_type->getName() == "bool" )   asm_str.push_back("strs " + right_reg->name + " -> rp(" + to_string( leftStackIdx ) + ")" );
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
        //printf("Args Size: %i\n", func_tk->args.size());
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
    else if ( token->getType() == TokenType_StringType ) {
        string genName = GenerateRandomName();
        String::pushString( genName, token->getName().substr(1, token->getName().size() - 2) );
        Register* reg = Register::getAllocRegister();
        asm_str.push_back("mov $" + genName + " -> " + reg->name );
        Register::pushRegister( reg );
    }
    else if ( token->getType() == TokenType_VariableType )
    {
        Variable* var = Stack::getVariable( token->getName() );
        assert( var != nullptr );

        Register* reg = Register::getAllocRegister();
        if( var->isGlobal ) {
            Object* data_type = var->getDataType();
            if(data_type->getName() == "int") asm_str.push_back("lod $" + var->getName() + " -> " + reg->name);
            else if(data_type->getName() == "short") asm_str.push_back("lodd $" + var->getName() + " -> " + reg->name);
            else if(data_type->getName() == "char") asm_str.push_back("lods $" + var->getName() + " -> " + reg->name);
            else if(data_type->getName() == "bool") asm_str.push_back("lods $" + var->getName() + " -> " + reg->name);
        }
        else {
            int stackIdx = Stack::getStackIndex( var->getName() );
            asm_str.push_back("lod rp(" + to_string( stackIdx ) + ") -> " + reg->name);
        }

        Register::pushRegister( reg );
    }
    else if ( token->getType() == TokenType_PointerType )
    {
        Variable* var = Variable::findVariable( token->getName() );
        assert( var != nullptr );

        Register* reg = Register::getAllocRegister();
        if( var->isGlobal ) {
            Object* data_type = var->getDataType();
            asm_str.push_back("mov $" + var->getName() + " -> " + reg->name);
        }
        else {
            asm_str.push_back("mov rp -> " + reg->name );
            int stackIdx = Stack::getStackIndex( var->getName() );
            if( var->stack_index > 0 )
                asm_str.push_back("add " + to_string( stackIdx ) + " -> " + reg->name );
            else
                asm_str.push_back("sub " + to_string( stackIdx * -1 ) + " -> " + reg->name );
        }
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
            Object* data_type = array_var->getDataType();
            int size_data = data_type->getSize();
            if( parser_idx->getType() == TokenType_NumberType ) {
                string __offset = to_string( stoi( parser_idx->getName() ) * size_data );
                Register* reg = Register::getAllocRegister();
                if(data_type->getName() == "int") asm_str.push_back("lod $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "short") asm_str.push_back("lodd $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "char") asm_str.push_back("lods $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "bool") asm_str.push_back("lods $" + array_var->getName() + "(" + __offset + ") -> " + reg->name);
                Register::pushRegister(reg);
            } else if( parser_idx->getType() == TokenType_OperatorType && parser_idx->getType() == TokenType_VariableType ) {
                vector<string> code_idx = CompileToken(parser_idx);
                for(string& str : code_idx)
                    asm_str.push_back( str );
                
                Register* old_reg = Register::popRegister();
                if( data_type->getName() == "int" ) asm_str.push_back("mul 4 -> " + old_reg->name );
                else if( data_type->getName() == "short" ) asm_str.push_back("mul 2 -> " + old_reg->name );
                else if( data_type->getName() == "char" || data_type->getName() == "bool" ) asm_str.push_back("mul 1 -> " + old_reg->name );

                Register* label_reg = Register::getAllocRegister();
                asm_str.push_back( "mov $" + array_var->getName() + " -> " + label_reg->name );
                asm_str.push_back("add " + old_reg->name + " -> " + label_reg->name );
                Register::UnusedRegister( old_reg );

                Register* reg = Register::getAllocRegister();
                if(data_type->getName() == "int") asm_str.push_back("lod " + label_reg->name + " -> " + reg->name);
                else if(data_type->getName() == "short") asm_str.push_back("lodd " + label_reg->name + " -> " + reg->name);
                else if(data_type->getName() == "char" || data_type->getName() == "bool" ) asm_str.push_back("lods " + label_reg->name + " -> " + reg->name);
                Register::UnusedRegister( label_reg );
                Register::pushRegister(reg);
            } else {
                assert( false && " String, Function, and more are not support to be index");
            }
        } else {
            // when the variable is as Local Variable

            Object* data_type = array_var->getDataType();
            int size_data = data_type->getSize();
            Register* rp_reg = Register::getAllocRegister();
            asm_str.push_back( "mov rp -> " + rp_reg->name );
            int stackIdx = Stack::getStackIndex( array_var->getName() );
            if( array_var->stack_index < 0 )
                asm_str.push_back("sub " + to_string( stackIdx * -1 ) + " -> " + rp_reg->name );
            else
                asm_str.push_back( "add " + to_string( stackIdx ) + " -> " + rp_reg->name );

            // When it is Pointer, load the address of pointer
            if( array_var->isPointer )
                asm_str.push_back("lod " + rp_reg->name + " -> " + rp_reg->name );

            if( parser_idx->getType() == TokenType_NumberType ) {
                string __offset = to_string( stoi( parser_idx->getName() ) * size_data );
                Register* reg = Register::getAllocRegister();
                if(data_type->getName() == "int") asm_str.push_back("lod " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "short") asm_str.push_back("lodd " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "char") asm_str.push_back("lods " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                else if(data_type->getName() == "bool") asm_str.push_back("lods " + rp_reg->name + "(" + __offset + ") -> " + reg->name);
                Register::pushRegister(reg);
            } else if( parser_idx->getType() == TokenType_OperatorType && parser_idx->getType() == TokenType_VariableType ) {
                vector<string> code_idx = CompileToken(parser_idx);
                Register* old_reg = Register::popRegister();
                if( data_type->getName() == "int" ) asm_str.push_back("mul 4 -> " + old_reg->name );
                else if( data_type->getName() == "short" ) asm_str.push_back("mul 2 -> " + old_reg->name );
                else if( data_type->getName() == "char" || data_type->getName() == "bool" ) asm_str.push_back("mul 1 -> " + old_reg->name );

                asm_str.push_back("add " + old_reg->name + " -> " + rp_reg->name );
                Register::UnusedRegister( old_reg );

                Register* reg = Register::getAllocRegister();
                if(data_type->getName() == "int") asm_str.push_back("lod " + rp_reg->name + " -> " + reg->name);
                else if(data_type->getName() == "short") asm_str.push_back("lodd " + rp_reg->name + " -> " + reg->name);
                else if(data_type->getName() == "char" || data_type->getName() == "bool" ) asm_str.push_back("lods " + rp_reg->name + " -> " + reg->name);
                Register::pushRegister(reg);
            } else {
                assert( false && "Other than Variable, Operation and Number is not support to be index");
            }
            Register::UnusedRegister( rp_reg );
        }
    }

    return asm_str;
}





