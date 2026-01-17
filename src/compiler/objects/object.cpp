#include "object.h"
#include <tools/memory.h>
#include <assert.h>

vector<Object*> Object::objects;

void Object::initObjects() { 
    Object::createObject( "char", 1 );   // Create 'char' object 
    Object::createObject( "short", 2 );   // Create 'short' object 
    Object::createObject( "int", 4 );   // Create 'int' object 
    Object::createObject( "void", 0 );   // Create 'void' object 
    Object::createObject( "string", 4 );   // Create 'string' object 
}

bool Object::isDataType( string data )
{
    for(Object* object : Object::objects )
        if( object->getName() == data )
            return true;

    return false;
}

Object* Object::getObject(const string &name) {
    for( Object* object : Object::objects )
        if( object->getName() == name )
            return object;

    assert( false && "Error: ObjectType is not Defined");
    return nullptr;
}

Object* Object::createObject( const string& name, int size ) {
    for( int i = 0; i < Object::objects.size(); i++ )
        if( Object::objects[i]->getName() == name )
            assert( false && "Class object already declare");

    Object* object = (Object*) __malloc( sizeof( Object ) );
    object->setName( name );
    object->setSize( size );
   
    Object::objects.push_back( object );
    return object;
}

void Object::cleanObject() {
    for( Object* object : Object::objects )
        __free( object );
}
