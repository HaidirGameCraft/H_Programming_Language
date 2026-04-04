#include "Variable.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include <cstdio>
#include <stack>
#include <asrt.h>
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

    if( Function::currentFunction != nullptr )
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
    if( Function::currentFunction != nullptr )
    {
        int stack = 4;
        Function* crt = (Function*) Function::currentFunction;
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
    if( Function::currentFunction != nullptr ) {
        Variable* var = Function::currentFunction->getVariable(name);
        if( var != nullptr )
            return var;
    }

    for(Variable* var : Variable::globalVariable)
        if( var->name == name )
            return var;
    return nullptr;
}

// Convert global List<Variable> into ASMData
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
                        int __idx_items = 1;
                        // Declare and push first element on array into data
                        std::string tasm = "=quad ";
                        if( var->array.tokens.size() > 0 )
                        {
                            tasm += var->array.tokens[0]->getName();
                            __size -= 4;
                        }

                        while( __idx_items < var->array.tokens.size() )
                        {
                            tasm += ", " + var->array.tokens[__idx_items]->getName();
                            __idx_items++;
                            __size -= 4;

                        }

                        asm_str.push_back( tasm );
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
                        int __idx_items = 1;
                        // Declare and push first element on array into data
                        std::string tasm = "=double ";
                        if( var->array.tokens.size() > 0 )
                        {
                            tasm += var->array.tokens[0]->getName();
                            __size -= 2;
                        }

                        while( __idx_items < var->array.tokens.size() )
                        {
                            tasm += ", " + var->array.tokens[__idx_items]->getName();
                            __idx_items++;
                            __size -= 2;
                        }

                        asm_str.push_back( tasm );
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
                    if( var->array.tokens.size() != 0 )
                    {
                        for( Token* strToken : var->array.tokens )
                        {
                            string labelGen = GenerateRandomName();
                            String::pushString( labelGen, strToken->getName().substr(1, strToken->getName().size() - 2) );
                            asm_str.push_back("=rq $" + labelGen );
                        }
                    } else {
                        if( str_length > 0 )
                            asm_str.push_back("=ascii " + var->getValue());
                        if( __size - str_length > 0 )
                            asm_str.push_back("=rs " + to_string( __size - str_length ) );
                    }
                }
                else {
                    if( var->provideReserved )
                        asm_str.push_back("=rs " + to_string( __size ));
                    else {
                        if ( var->array.tokens.size() > 0 )
                        {
                            int __idx_items = 1;
                            std::string tasm = "=single ";
                            if( var->array.tokens.size() > 0 )
                            {
                                tasm += var->array.tokens[0]->getName();
                                __size--;
                            }

                            while( __idx_items < var->array.tokens.size() )
                            {
                                tasm += ", " + var->array.tokens[__idx_items]->getName();
                                __idx_items++;
                                __size--;
                            }
                            
                            asm_str.push_back( tasm );
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
            else if( var->getDataType()->getName() == "char" ) {
                if( var->getSize() > 0 && var->isPointer ) {
                    asm_str.push_back("=ascii " + var->getValue());
                    asm_str.push_back("=single 0");
                }
                else
                    asm_str.push_back("=single " + var->getValue());
            }
            else if ( var->getDataType()->getName() == "string" )
            {
                asm_str.push_back("=ascii " + var->getValue());
                asm_str.push_back("=single 0");
            }
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

std::vector<std::string> Variable::VariableCompiler( std::vector<Token*> tokens ) {
    std::vector<std::string> resultCode;
    bool isConstant = false;

    DataTypeToken* dataType_token = ( DataTypeToken* ) tokens.front();
    VariableToken* varToken = ( VariableToken* ) tokens[1];
    if( tokens.front()->getType() == TokenType_KeyWordType && tokens.front()->getName() == "const" )
    {
        isConstant = true;
        dataType_token = ( DataTypeToken* ) tokens[1];
        varToken = (VariableToken*) tokens[2];
    }
    Object* dataObj = dataType_token->getDataType();
    EXITFAIL(dataObj != nullptr, printf("[Syntax Error]: Invalid DataType %s\n", dataType_token->getName() ) )

    // Configure the New Variable
    Variable* newVar = ( Variable* ) __malloc( sizeof( Variable ) );
    newVar->isConstant = isConstant;
    newVar->isPointer = varToken->isPointer;
    newVar->isArray = varToken->isArray;
    newVar->setName( varToken->getName() );
    newVar->setDataType( dataObj );
    newVar->setSize( dataObj->getSize() );
    Variable::pushVariable( newVar );

    std::vector<Token*> newTokens;
    newTokens.push_back( (Token*) varToken );
    int index = 0;

    while( tokens[index]->getName() != "=" ) {
        if( tokens[index]->getType() == TokenType_BracketType )
            index = ((BracketToken*) tokens[index])->closeBracket;
        index++;
    }

    for(; index < tokens.size(); index++)
        newTokens.push_back( tokens[index] );

    // [Debug] displaying the newTokens
    // for( int i = 0; i < newTokens.size(); i++ )
    //     newTokens[i]->print(0);

    //printf("[DEBUG] Hai\n");
    Token* parser = Parser::ParserExp( newTokens );
    //parser->print(0);

    // If new Variable is an Global Variable
    if( newVar->isGlobal == true )
    {
        // Whether it is an Array?
        if( newVar->isArray )
        {
            EXITFAIL( parser->right != nullptr, printf("[Error]: Right Parser is NULL\n") )
            EXITFAIL( parser->right->getType() == TokenType_ArrayType, printf("[Error]: Right Parser os not an Array\n"))
            ArrayListToken* rightArray = (ArrayListToken*) parser->right;
            Array& array = newVar->array;
            
            // checking the items
            for( int i = 0; i < rightArray->tokens.size(); i++ ) {
                if( rightArray->tokens[i]->getType() != TokenType_NumberType && rightArray->tokens[i]->getType() != TokenType_StringType )
                    EXITFAILMSG(printf("Error: the items must be numbers or string because of global variable\n"));
                
                if( rightArray->tokens[i]->getType() == TokenType_StringType )
                {
                    std::string lblstring = "__String" + GenerateRandomName();
                    String::pushString( lblstring, rightArray->tokens[i]->getName() );

                    Token* new_token = (Token*) __malloc( sizeof( Token ) );
                    new_token->setType( TokenType_NumberType );
                    
                } else {
                    array.tokens.push_back( (Token*) __copy(rightArray->tokens[i]) );
                }
            }
            
        } else {
            EXITFAIL( parser->right != nullptr, printf("[Error]: No Right Token\n"))
            EXITFAIL( parser->right->getType() == TokenType_CharType || parser->right->getType() == TokenType_StringType || parser->right->getType() == TokenType_NumberType, printf("[Error]: could not make arithmetic statement outside of Function\n"))

            if( parser->right->getType() == TokenType_NumberType )
                newVar->setValue( parser->right->getName() );
            else if ( parser->right->getType() == TokenType_StringType ) {
                newVar->setSize( parser->right->getName().size() );
                newVar->setValue( parser->right->getName() );
                //String::pushString( newVar->getName(), parser->right->getName() );
            } else if ( parser->right->getType() == TokenType_CharType )
            {
                newVar->setValue( parser->right->getName() );
            }
        }
    } else {    // Now, run the stement for Local Variable
        // if( parser->right->getType() == TokenType_StringType )
        //     newVar->setSize( 4 );

        int size = ((int)( newVar->getSize() / 4 ) + ( newVar->getSize() % 4 > 0) ) * 4;

        int stackIdx = Stack::getStackIndex( newVar->getName() );
        resultCode.push_back("sub " + std::to_string( size ) + " -> rs");

        std::vector<std::string> code = Compiler::CompileParser( parser->right );
        for( std::string& c : code ) {
            resultCode.push_back( c );
        }

        Register* reg = Register::popRegister();
        if( newVar->getDataType()->getName() == "int" || newVar->getDataType()->getName() == "string" || newVar->isPointer )
            resultCode.push_back("str " + reg->name + " -> rp(" + std::to_string( stackIdx ) + ")");
        else if( newVar->getDataType()->getName() == "short" )
            resultCode.push_back("strd " + reg->name + " -> rp(" + std::to_string( stackIdx ) + ")");
        else if( newVar->getDataType()->getName() == "char" || newVar->getDataType()->getName() == "bool" )
            resultCode.push_back("strs " + reg->name + " -> rp(" + std::to_string( stackIdx ) + ")");
        Register::UnusedRegister( reg );
    }

    for( Token* token : tokens )
        __free( token );
    return resultCode;
}
