#pragma once

#include <string>
#include <vector>

using namespace std;
class String {
public:
    static vector<String*> constant_string;

    string name;
    string data;
    void setName( const string& name );
    void setString( const string& str );
    const string& getName();
    const string& getString();

    static void pushString(string name, string str);
    static vector<string> codeString();
    static void cleanup();
};
