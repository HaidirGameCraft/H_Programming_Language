#include "DataType.hpp"
#include "Token.hpp"


DataTypeToken::DataTypeToken() {
    this->type = TokenType_DataType;
}

void DataTypeToken::setDataType( DataType type ) { this->data_type = type; }
DataType DataTypeToken::getDataType() { return this->data_type; }


int DataTypeSize(DataType type){
    if( type == DataType_Char ) return 1;
    else if ( type == DataType_Short ) return 2;
    else if ( type == DataType_Int ) return 4;
    else if ( type == DataType_Bool) return 1;
    return 0;
}
