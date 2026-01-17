#include <assert.h>
#include "ControlStructure.hpp"
#include "compiler/Compiler.hpp"
#include "compiler/Function.hpp"
#include "compiler/Parser.hpp"
#include "compiler/Register.hpp"
#include "compiler/Stack.hpp"
#include "compiler/Token.hpp"
#include "compiler/Tools.hpp"
#include  <tools/memory.h>

vector<ControlStructure*> ControlStructure::controlStruct;
ControlStructure* ControlStructure::current_controlStruct = nullptr;
void ControlStructure::pushControlStructure(ControlStructure *ifcond) {
    ControlStructure::controlStruct.push_back(ifcond);
    ControlStructure::current_controlStruct = ifcond;
}

ControlStructure* ControlStructure::popControlStructure() {
    ControlStructure* __controlStruct = ControlStructure::controlStruct.back();
    ControlStructure::controlStruct.pop_back();
    ControlStructure::current_controlStruct = nullptr;
    if( ControlStructure::controlStruct.size() > 0 )
        ControlStructure::current_controlStruct = ControlStructure::controlStruct.back();
    return __controlStruct;
}

void ControlStructure::pushCode(string code) {
    this->code.push_back( code );
}

void ControlStructure::pushCondCode( string cond_code ) {
    this->cond_code.push_back( cond_code );
}
void ControlStructure::setElseName(const string& name) {
    this->else_label_name =  name;
}
void ControlStructure::setEndIfName( const string& name ) {
    this->endif_label_name = name;
}
const string& ControlStructure::getEndifName() { return this->endif_label_name; }

void ControlStructure::cleanup() {
    for( ControlStructure* selectcontrol : ControlStructure::controlStruct )
        __free( selectcontrol );
}






vector<string> ControlStructure::RepetitionControlCompiler( vector<Token*> tokens ) {
   Token* first_token = (Token*) tokens.front();
   if( first_token->getType() != TokenType_KeyWordType )
       return {};

   if( first_token->getName() == "while" )
   {
        assert( tokens[1] != nullptr || tokens[1]->getType() != TokenType_BracketType );
        vector<Token*> conditionTokens;
        for(int idx = 2; idx < ((BracketToken*) tokens[1])->closeBracket; idx++)
            conditionTokens.push_back( tokens[idx] );
        
        string endLabel = "." + GenerateRandomName();
        ControlStructure* whileCondition = (ControlStructure*) __malloc( sizeof( ControlStructure ) );
        whileCondition->setName( "." + GenerateRandomName() );    // Generate label name for WhileLoop 
        whileCondition->setEndIfName( endLabel );

        Token* parser = Parser::ParserExp( conditionTokens );
        vector<string> condition_code = Compiler::CompileParser( parser ); // Compiler
        whileCondition->code.push_back( whileCondition->getName() + ":" );
        for( string& cnd : condition_code ) whileCondition->code.push_back( cnd );

        // Register Condition
        if( parser->getType() == TokenType_OperatorType && 
            ( parser->getName() == "&&" || parser->getName() == "||" )) {
            Register* condition_reg = Register::popRegister();
            whileCondition->code.push_back("cnd " + condition_reg->name + " != 0" );
            Register::UnusedRegister( condition_reg );
        }
        whileCondition->code.push_back("gonc $" + endLabel );

        // Make new Stack 
        Stack::CreateStack();
        ControlStructure::pushControlStructure( whileCondition );

        return {};
   }
   else if ( first_token->getName() == "endwhile" )
   {
        ControlStructure* current_while = ControlStructure::popControlStructure();
        assert( current_while != nullptr && "Current While loop is not define");


        vector<string> codes;
        for( string& code : current_while->code) codes.push_back( code );

        int stackSize = Stack::currentStack->getTotalStackSizeVariable();
        Stack::currentStack->clearVariables();

        if( stackSize > 0 )
            codes.push_back("add " + std::to_string( stackSize ) + " -> rs");
        codes.push_back("go $" + current_while->getName() );
        codes.push_back( current_while->getEndifName() + ":" );
        
        Stack::RemoveStack();   // Removing the latest Stack
        Register::UnusedAllRegister();
        return codes;
   }

   return {};
}


vector<string> ControlStructure::SelectionControlCompiler( vector<Token*> tokens ) {
    Token* if_token = tokens[0];
    if( if_token->getType() == TokenType_KeyWordType )
    {

        // When it is While with End
        if( if_token->getName() == "while" || if_token->getName() == "endwhile" )
            return ControlStructure::RepetitionControlCompiler( tokens );

        if( if_token->getName() == "else" )
        {
            assert( ControlStructure::current_controlStruct != nullptr );  // checking either the current if statement is not null as pointer
            ControlStructure* ifCondition = ControlStructure::current_controlStruct;

            int stackSizeVariable = Stack::currentStack->getTotalStackSizeVariable();
            Stack::currentStack->clearVariables();
            if( stackSizeVariable > 0 )
                ifCondition->pushCode( "add " + std::to_string( stackSizeVariable ) + " -> rs" );

            ifCondition->pushCode("go $" + ifCondition->getEndifName() );
            ifCondition->pushCode(ifCondition->getNextSelectLabel() + ":");

            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();
            return {};
        }
        else if ( if_token->getName() == "endif" )
        {
            ControlStructure* ifCondition = ControlStructure::popControlStructure();
            int stackSizeVariable = Stack::currentStack->getTotalStackSizeVariable();
            Stack::currentStack->clearVariables();
            if( stackSizeVariable > 0 )
                ifCondition->pushCode( "add " + std::to_string( stackSizeVariable ) + " -> rs" );
            Stack::RemoveStack();   // Removing the latest Stack

            ifCondition->pushCode(ifCondition->getEndifName() + ":");
            vector<string> ret_code = ifCondition->code;
            ifCondition->code.clear();
            __free( ifCondition );
            return ret_code;
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
    vector<string> condition_code_asm = Compiler::CompileParser(parser);
   
    if( if_token->getType() == TokenType_KeyWordType )
    {
        if( if_token->getName() == "if" )
        {
            ControlStructure* ifCondition = (ControlStructure*) __malloc( sizeof( ControlStructure ) );
            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();

            ifCondition->setNextSelectLabel( label_name );
            ifCondition->setEndIfName("." + GenerateRandomName() + "_" + Function::current_token->getName());

            for(string& str : condition_code_asm)
                ifCondition->pushCode(str);
            if( parser->getType() == TokenType_OperatorType && ( parser->getName() == "&&" || parser->getName() == "||" ) )
            {
                Register* reg = Register::popRegister();
                ifCondition->pushCode("cnd " + reg->name + " != 0 ");
                Register::UnusedRegister( reg );
            }
            ifCondition->pushCode("gonc $" + label_name);    // When the condition is FALSE

            // Make new Stack for Selection Control
            Stack::CreateStack();
            ControlStructure::pushControlStructure( ifCondition );
        }
        else if ( if_token->getName() == "elif" ) {
            assert( ControlStructure::current_controlStruct != nullptr );  // checking either the current if statement is not null as pointer
            ControlStructure* ifCondition = ControlStructure::current_controlStruct;

            int stackSizeVariable = Stack::currentStack->getTotalStackSizeVariable();
            Stack::currentStack->clearVariables();
            if( stackSizeVariable > 0 )
                ifCondition->pushCode( "add " + std::to_string( stackSizeVariable ) + " -> rs" );
            ifCondition->pushCode("go $" + ifCondition->getEndifName() );
            ifCondition->pushCode(ifCondition->getNextSelectLabel() + ":");

            string label_name = "." + GenerateRandomName();
            if( Function::current_token != nullptr )
                label_name += "_" + Function::current_token->getName();

            ifCondition->setNextSelectLabel( label_name );
            for(string& str : condition_code_asm)
                ifCondition->pushCode(str);
            if( parser->getType() == TokenType_OperatorType && ( parser->getName() == "&&" || parser->getName() == "||" ) )
            {
                Register* reg = Register::popRegister();
                ifCondition->pushCode("cnd " + reg->name + " != 0 ");
                Register::UnusedRegister( reg );
            }
            ifCondition->pushCode("gonc $" + label_name);

            return {};
        }
    }

    for(Token* token : tokens )
        __free( token );
    
    return {};


}
