#include "String.h"
#include "tools/memory.h"
#include <assert.h>

vector<String*> String::constant_string;
void String::setName( const string& name ) {
    this->name = name;
}

void String::setString( const string& str ) {
    this->data = str;
}

const string& String::getName() {
    return this->name;
}

const string& String::getString() {
    return this->data;
}

void String::pushString(string name, string str) {
    String* __string = (String*) __malloc( sizeof( String ) );
    __string->setName( name );
    __string->setString( str );

    // Check whether it containing the same name
    for( String* __str  : String::constant_string )
        if( __str->getName() == name )
            assert( false && "StringError: containing the same name");

    String::constant_string.push_back( __string );
}

vector<string> String::codeString() {
    vector<string> data;
    for( String* str : String::constant_string )
    {
        data.push_back( str->getName() + ":" );
        data.push_back("=ascii \"" + str->getString() + "\"");
    }
    return data;
}

void String::cleanup() {
    for( String* str : String::constant_string )
        __free( str );
}


