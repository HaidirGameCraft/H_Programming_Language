#pragma once

#include <string>
#include <vector>

using namespace std;
class IfCondition {
public:
    string name;
    string else_label_name;
    string endif_label_name;
    vector<string> cond_code;
    vector<string> code;
    static vector<IfCondition*> if_conds;
    static IfCondition* current_ifconds;

    void setElseName(const string& name);
    void setEndIfName( const string& name );
    const string& getEndifName();
    void pushCode(string code);
    void pushCondCode(string cond);

    static void pushIfCondition( IfCondition* ifcond );
    static IfCondition* popIfCondition();
    static void cleanup();
};
