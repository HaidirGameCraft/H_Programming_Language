#pragma once

#include <string>
#include "DataType.hpp"
#include "compiler/objects/object.h"

enum TokenType {
    TokenType_NullType,
    TokenType_AssignType,
    TokenType_DataType,

    TokenType_BracketType,
    TokenType_NumberType,
    TokenType_StringType,
    TokenType_CharType,

    TokenType_PointerType,

    TokenType_ArrayType,
    TokenType_GlobalType,
    TokenType_VariableType,

    TokenType_ImportType,
    TokenType_KeyWordType,
    TokenType_ConditionType,

    TokenType_FunctionType,
    TokenType_OperatorType,
    TokenType_SymbolsType,
    TokenType_ReturnType
};

class Token {
public:
    std::string name;
    TokenType type;

    bool hasSet = false;
    Token* left = nullptr;
    Token* right = nullptr;

    Token();

    void setName(const std::string& name);
    void setType(TokenType type);

    TokenType getType();
    std::string getName();

    void print(int indent);
};

class VariableToken : public Token {
public:
    bool isPointer;
    bool isArray;
    Token* indexToken = nullptr;
};

class DataTypeToken : public Token {
public:
    int size;
    bool isPointer = false;
    Object* object;
    DataTypeToken();

    void setDataType( Object* object );
    Object* getDataType();
};

class BracketToken : public Token {
public:
    int closeBracket;
    BracketToken();
};

class ArrayToken : public Token {
public:
    vector<Token*> tokens;
};

class ControlStructureToken : public Token {
public:
    ControlStructureToken();
    void Init();
};


