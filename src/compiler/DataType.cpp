#include "DataType.hpp"
#include "Token.hpp"


DataTypeToken::DataTypeToken() {
    this->type = TokenType_DataType;
}

void DataTypeToken::setDataType( Object* type ) { this->object = type; }
Object* DataTypeToken::getDataType() { return this->object; }


int DataTypeSize(DataType type){
    if( type == DataType_Char ) return 1;
    else if ( type == DataType_Short ) return 2;
    else if ( type == DataType_Int ) return 4;
    else if ( type == DataType_Bool) return 1;
    return 0;
}
