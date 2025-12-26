#include "IfCondition.hpp"
#include  <tools/memory.h>

vector<IfCondition*> IfCondition::if_conds;
IfCondition* IfCondition::current_ifconds = nullptr;
void IfCondition::pushIfCondition(IfCondition *ifcond) {
    IfCondition::if_conds.push_back(ifcond);
    IfCondition::current_ifconds = ifcond;
}

IfCondition* IfCondition::popIfCondition() {
    IfCondition* ifcond = IfCondition::if_conds.back();
    IfCondition::if_conds.pop_back();
    IfCondition::current_ifconds = nullptr;
    if( IfCondition::if_conds.size() > 0 )
        IfCondition::current_ifconds = IfCondition::if_conds.back();
    return ifcond;
}

void IfCondition::pushCode(string code) {
    this->code.push_back( code );
}

void IfCondition::pushCondCode( string cond_code ) {
    this->cond_code.push_back( cond_code );
}
void IfCondition::setElseName(const string& name) {
    this->else_label_name =  name;
}
void IfCondition::setEndIfName( const string& name ) {
    this->endif_label_name = name;
}
const string& IfCondition::getEndifName() { return this->endif_label_name; }

void IfCondition::cleanup() {
    for( IfCondition* if_conds : IfCondition::if_conds )
        __free( if_conds );
}
