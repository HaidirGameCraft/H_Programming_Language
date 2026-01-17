#pragma once

#include <compiler/Function.hpp>
#include <compiler/Variable.hpp>
#include <string>
#include <vector>

class ClassObject {
public:
    std::string name;
    std::vector<Variable*> variables;
    std::vector<Function*> functions;
    static ClassObject* mainClass;

    void setClassName( const std::string& name ) {
        this->name = name;
    }

    const std::string& getClassName() {
        return this->name;
    }

    static void CreateObject( const std::string& name );
    void pushFunction( Function* function );
    void pushVariable( Variable* variable );
};
