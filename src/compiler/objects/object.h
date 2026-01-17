#pragma once

#include <string>
#include <vector>

using namespace std;
class Object {
public:
    string name;
    int size;
    static vector<Object*> objects;

    void setName( const string& name ) {
        this->name = name;
    }
    void setSize( int size ) {
        this->size = size;
    }

    static void initObjects();
    static bool isDataType( string name );
    static Object* createObject( const string& name, int size );
    static Object* getObject( const string& name );
    static void cleanObject();

    const string&   getName() { return this->name; }
    int             getSize() { return this->size; }
};
