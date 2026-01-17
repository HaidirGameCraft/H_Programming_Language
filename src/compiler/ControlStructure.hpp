#pragma once

#include "compiler/Token.hpp"
#include "compiler/Variable.hpp"
#include <string>
#include <vector>

using namespace std;
class ControlStructure {
public:
    string name;
    string else_label_name;
    string endif_label_name;
    string next_selection_label_name;
    vector<string> cond_code;
    vector<string> code;
    vector<Variable*> variables;
    static vector<ControlStructure*> controlStruct;
    static ControlStructure* current_controlStruct;

    void setElseName(const string& name);
    void setEndIfName( const string& name );
    void setNextSelectLabel( const string& name ) { this->next_selection_label_name = name; }

    void setName( const string& name ){ this->name = name; }
    const string& getName(){ return this->name; }

    const string& getEndifName();
    const string& getNextSelectLabel() { return this->next_selection_label_name; }
    void pushCode(string code);
    void pushCondCode(string cond);

    // Compiler
    static vector<string> RepetitionControlCompiler( vector<Token*> tokens );
    static vector<string> SelectionControlCompiler( vector<Token*> tokens );

    static void pushControlStructure( ControlStructure* ifcond );
    static ControlStructure* popControlStructure();
    static void cleanup();
};
