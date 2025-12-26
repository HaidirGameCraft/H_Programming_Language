#include "Variable.hpp"
#include "DataType.hpp"
#include "Function.hpp"
#include <tools/memory.h>
#include "Token.hpp"
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

void Variable::setDataType( DataType data_type )
{
    this->data_type = data_type;
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
const DataType& Variable::getDataType() { return  this->data_type; }


void Variable::pushVariable( string name, int size, DataTypeToken* datatype )
{
    Variable* var = (Variable*) __malloc( sizeof( Variable ) );

    int __s = ((int)( size / 4 ) + 1 ) * 4;
    int stack = 4;

    if( Function::current_token != nullptr )
    {
        Function* crt = (Function*) Function::current_token;
        for(Variable* __var : crt->local_variables )
            stack += ((int)(__var->size / 4) + 1) * 4;

        var->stack_index = -stack;
    }

    var->setName( name );
    var->setSize( __s );
    var->setDataType( datatype->getDataType() );

    if( Function::current_token != nullptr )
        Function::current_token->pushVariable(var);
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
        
        for(Variable* __var : crt->local_variables )
            stack += ((int)(__var->size / 4) + 1) * 4;

        var->stack_index = -stack;
    }

    if( Function::current_token != nullptr ) {
        Function::current_token->pushVariable(var);
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
            if( var->getDataType() == DataType_Int ) {

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
            else if ( var->getDataType() == DataType_Short )
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
            } else if ( var->getDataType() == DataType_Char || var->getDataType() == DataType_Bool )
            {
                if( var->getDataType() == DataType_Char ) {
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
            if( var->getDataType() == DataType_Int )
                asm_str.push_back("=quad " + var->getValue());
            else if( var->getDataType() == DataType_Short )
                asm_str.push_back("=double " + var->getValue());
            else if( var->getDataType() == DataType_Char )
                asm_str.push_back("=single " + var->getValue());
            else if( var->getDataType() == DataType_Bool )
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



