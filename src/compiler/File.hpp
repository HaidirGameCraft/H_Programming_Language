#pragma once

#include <string>
#include <vector>

using namespace std;
class File {
public:
    static vector<string> global_code;
    static vector<string> code;
    static vector<string> data;

    static vector<string> filesImport;
    
    void readFile( string filename );
};
