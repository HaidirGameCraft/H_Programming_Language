#pragma once

#include <string>
#include <vector>

using namespace std;

enum DataType {
    DataType_Void = 0,
    DataType_Char,
    DataType_Short,
    DataType_Int,
    DataType_Bool
};

static vector<string> dataTypeName = {
    "char",
    "short",
    "int",
    "void",
    "bool"
};

int DataTypeSize( DataType type ); 
