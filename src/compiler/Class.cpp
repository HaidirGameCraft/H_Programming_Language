#include "Class.hpp"
#include "tools/memory.h"

ClassObject* ClassObject::mainClass = nullptr;


void ClassObject::pushFunction( Function* function ) {
    this->functions.push_back( function );
}

void ClassObject::pushVariable( Variable* variable ) {
    this->variables.push_back( variable );
}

void ClassObject::CreateObject( const std::string& name ) {
    ClassObject* cobject = (ClassObject*) __malloc( sizeof( ClassObject ) );
    cobject->setClassName( name );
    ClassObject::mainClass = cobject;
}
