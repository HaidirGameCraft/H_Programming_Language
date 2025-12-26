#pragma once

#include <string>
#include <vector>
#include <stdint.h>

using namespace std;
struct label_t {
    string name;
    uint32_t address;
    vector<label_t> child;
    vector<uint32_t> target;
};

void Label_add( string label );
void Label_updateAddress( string label, uint32_t address );
void Label_push( string label, uint32_t dest );
void Label_update();


