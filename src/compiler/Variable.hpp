#pragma once

#include <string>
#include <vector>
#include "DataType.hpp"
#include "Token.hpp"
#include  "Array.hpp"

using namespace std;

class Variable {
public:
    int size;
    int stack_index;

    bool isGlobal = false;
    bool isConstant = false;
    bool isArray = false;
    bool isPointer = false;

    bool provideReserved = false;
    string name;
    string value;
    Array array;
    DataType data_type;

    static vector<Variable*> globalVariable;

    void setName( const string& name );
    void setArray( const Array& array );
    void setSize( int size );
    void setValue( string str );
    void setDataType( DataType data_type );

    const string& getName();
    const int& getSize();
    const string& getValue();
    const DataType& getDataType();

    static void cleanup();

    static void pushVariable( string name, int size, DataTypeToken* type );
    static void pushVariable( Variable* var );
    static Variable* findVariable( string name );
    static vector<string> asmData();
};
