#include "HByte.h"
#include "hbyte/Label.hpp"
#include "hbyte/Token.hpp"
#include "hbyte/filereader.h"
#include "hbyte/inst/instruction.h"
#include "inst.h"

#include <cstdio>
#include <string.h>

#define LENGTH(x, y) sizeof( x ) / sizeof( y )

using namespace HByte;

uint32_t HByteCompiler::pc = 0;
int ISRIdentifier(vector<Token*> tokens);
vector<Token*> BracketChecker(vector<Token*> tokens);

int convertNumber( string nm ) {
    int num = 0;
    if( strncmp(nm.c_str(), "0x", 2) == 0 )
        sscanf(nm.c_str(), "0x%x", &num );
    else
        sscanf(nm.c_str(), "%i", &num);
    return num;
}

int ISRIdentifier(vector<Token*> tokens) {
    Token* firstToken = tokens[0];
    // Command 'MOV', 'SUB', 'ADD', ...
    if( firstToken->getType() == TokenType_CommandType )
    {
        // Operand <Val/Reg> <Symbols> <Val/Reg>
        if( tokens.size() == 4 )
        {
            Token* left_token = tokens[1];
            Token* symbols_token = tokens[2];
            Token* right_token = tokens[3];

            bool isLefthasOffset = left_token->hasOffset;
            bool isRighthasOffset = right_token->hasOffset;
            bool isLeftReg = left_token->getType() == TokenType_RegisterType;
            bool isRightReg = right_token->getType() == TokenType_RegisterType;
            bool isLeftNumber = left_token->getType() == TokenType_NumberType;
            bool isRightNumber = right_token->getType() ==  TokenType_NumberType;
            bool isLeftLabel = left_token->getType() == TokenType_LabelType;
            bool isRightLabel = right_token->getType() == TokenType_LabelType;

            if( isLeftReg )
            {
                if( isLefthasOffset )
                {
                    if( isRightReg ) return ISR_Opcode_RegOff_Reg;
                }
                else {
                    if( isRightReg && isRighthasOffset ) return ISR_Opcode_Reg_RegOff;
                    else if ( (isRightNumber || isRightLabel) && isRighthasOffset ) return ISR_Opcode_Reg_ValueOff;
                    else if ( (isRightNumber || isRightLabel) ) return ISR_Opcode_Reg_Value;
                    else if ( isRightReg ) return ISR_Opcode_Reg_Reg;
                }
            }
            else if ( isLeftNumber || isLeftLabel )
            {
                if( isLefthasOffset )
                {
                    if( isRightReg ) return ISR_Opcode_ValueOff_Reg;
                }
                else {
                    if( isRightReg && isRighthasOffset ) return ISR_Opcode_Value_RegOff;
                    else if ( (isRightNumber || isRightLabel) && isRighthasOffset ) return ISR_Opcode_Value_ValueOff;
                    else if ( isRightNumber || isRightLabel ) return ISR_Opcode_Value_Value;
                    else if ( isRightReg ) return ISR_Opcode_Value_Reg;
                }
            }

        }
        // Operand <Val/Reg>
        else if ( tokens.size() == 2 )
        {
            Token* __tk = tokens[1];
            bool isNumber = __tk->getType() == TokenType_NumberType;
            bool isReg = __tk->getType() == TokenType_RegisterType;
            bool isLabel = __tk->getType() == TokenType_LabelType;

            if( isReg ) return ISR_Opcode_Reg;
            else if ( isNumber || isLabel ) return ISR_Opcode_Value;
        }
        // Operand
        else {
            return ISR_Opcode;
        }
    }
    // Label of Address '<label>:'
    else if ( firstToken->getType() == TokenType_LabelType )
    {
        return ISR_Label;
    }
    // Config '=region', '=rs', '=rb'
    else if ( firstToken->getType() == TokenType_OperationType &&
              firstToken->getName() == "=" )
    {
        return ISR_Config;
    }

    return -1;
}

vector<Token*> BracketChecker(vector<Token*> tokens) {
    // Scanning the brackets
    vector<int> bracket_idx;
    for( int i = 0; i < tokens.size(); i++ )
    {
        Token* tk = tokens[i];
        if( tk->getType() == TokenType_BracketType )
        {
            if( tk->getName() == "(" )
                bracket_idx.push_back(i);
            else if ( tk->getName() == ")" )
            {
                if( bracket_idx.size() == 1 )
                {
                    int start = bracket_idx[0];
                    int end = i;

                    Token* bracket = tokens[start];
                    bracket->closeBracket = end;
                }
            }
        }
    }

    // Adjust the bracket
    for( int i = 0; i < tokens.size(); i++) {
        Token* token = tokens[i];
        if( token->hasOffset )
        {
            Token* openbracket_token = tokens[i + 1];
            tokens[i + 1] = nullptr;
            i = i + 2;
            while( i < openbracket_token->closeBracket )
            {
                token->tokens.push_back( tokens[i] );
                tokens[i] = nullptr;
                i++;
            }
            tokens[i] = nullptr;
        }
    }

    // push it to new tokens
    vector<Token*> newTokens;
    for(int i = 0; i < tokens.size(); i++) {
        if( tokens[i] == nullptr )
            continue;

        newTokens.push_back( tokens[i] );
    }

    return newTokens;
}

void HByteCompiler::Compile(vector<Token *> tokens) {
    tokens = BracketChecker(tokens);
    int isr = ISRIdentifier(tokens);
    uint8_t prefix = 0;
    uint8_t ext_prefix = 0;
    uint8_t opcode = 0;
    ins_register_t _register;
    ins_symbols_t symbols;
    short offset;
    uint32_t value;

    if( tokens[0]->getType() == TokenType_CommandType )
    {
        InstructionSet* is = nullptr;

#define find_instruction_set( x ) \
        for( int i = 0; i < sizeof( x ) / sizeof( InstructionSet ); i++ ) { \
            is = &x[i]; \
            if( strcmp(tokens[0]->getName().c_str(), is->name) == 0 ) \
                break; \
            is = nullptr; \
        }

        if( isr == ISR_Opcode_Reg_Reg ) {
            find_instruction_set(instruction_set_rr_list);
        }
        else if ( isr == ISR_Opcode_RegOff_Reg ) {
            find_instruction_set(instruction_set_rfr_list);
        }
        else if ( isr == ISR_Opcode_Reg_RegOff ) {
            find_instruction_set(instruction_set_rrf_list);
        }
        else if ( isr == ISR_Opcode_Value_Reg ) {
            find_instruction_set(instruction_set_vr_list);
        }
        else if ( isr == ISR_Opcode_Reg_Value ) {
            find_instruction_set(instruction_set_rv_list);
        }
        else if ( isr == ISR_Opcode_ValueOff_Reg ) {
            find_instruction_set(instruction_set_vfr_list);
        }
        else if ( isr == ISR_Opcode_Reg_ValueOff ) {
            find_instruction_set(instruction_set_rvf_list);
        }
        else if ( isr == ISR_Opcode_Value_RegOff ) {
            find_instruction_set(instruction_set_vrf_list);
        }
        else if ( isr == ISR_Opcode_Reg ) {
            find_instruction_set(instruction_set_r_list);
        }
        else if ( isr == ISR_Opcode_Value ) {
            find_instruction_set(instruction_set_v_list);
        }
        else if ( isr == ISR_Opcode ) {
            find_instruction_set(instruction_set_op_list);
        }
        assert(is != 0);

        prefix = is->prefix;
        ext_prefix = is->ext_prefix;
        opcode = is->opcode;
        Token* left_tk = (1 < tokens.size()) ? tokens[1] : nullptr;
        Token* symbols_tk = (2 < tokens.size()) ? tokens[2] : nullptr;
        Token* right_tk = (3 < tokens.size()) ? tokens[3] : nullptr;

        // Left Token
        if( left_tk != nullptr && left_tk->getType() == TokenType_RegisterType )
            _register.sreg = getRegister( left_tk->getName() );
        else if ( left_tk != nullptr && left_tk->getType() == TokenType_NumberType )
            value = (uint32_t) convertNumber( left_tk->getName() );
        else if ( left_tk != nullptr && left_tk->getType() == TokenType_LabelType ) {
            value = 0;
        }

        if( left_tk != nullptr && left_tk->hasOffset )
        {
            Token* offToken = left_tk->tokens.front();
            if( offToken->getType() == TokenType_NumberType )
                offset = (short)( convertNumber( offToken->getName() ) & 0xFFFF );
        }

        // Right Token
        if( right_tk != nullptr && right_tk->getType() == TokenType_RegisterType )
            _register.dreg = getRegister(right_tk->getName());
        else if ( right_tk != nullptr && right_tk->getType() == TokenType_NumberType )
            value = (uint32_t) convertNumber( right_tk->getName() );
        else if ( right_tk != nullptr && right_tk->getType() == TokenType_LabelType )
            value = 0;

        if( right_tk != nullptr && right_tk->hasOffset )
        {
            Token* offToken = right_tk->tokens.front();
            if( offToken->getType() == TokenType_NumberType )
                offset = (short)( convertNumber( offToken->getName() ) & 0xFFFF );
        }

        // Symbols Token
        if( symbols_tk != nullptr )
            symbols.sym = getSymbols( symbols_tk->getName() );

        pc = hbyte_write8(pc, prefix);
        if( prefix & PREFIX_EXT_INC )
            pc = hbyte_write8(pc, prefix);
        pc = hbyte_write8(pc, opcode);

        if( prefix & PREFIX_REG_INC )
            pc = hbyte_write8( pc, *((uint8_t*) &_register));
        
        if( prefix & PREFIX_OFF_INC )
            pc = hbyte_write16(pc, (uint16_t) offset);
        else if ( prefix & PREFIX_SYM_INC )
            pc = hbyte_write8(pc, *((uint8_t*) &symbols));

        if( prefix & PREFIX_VAL_INC) {
            if( left_tk != nullptr && left_tk->getType() == TokenType_LabelType )
                Label_push(left_tk->getName(), pc);
            else if( right_tk != nullptr && right_tk->getType() == TokenType_LabelType )
                Label_push(right_tk->getName(), pc);
            pc = hbyte_write32(pc, value);
        }
    }
    else if ( tokens[0]->getType() == TokenType_LabelType )
    {
       Label_updateAddress(tokens[0]->getName(), pc); 
    }
    else if ( tokens[0]->getType() == TokenType_SymbolsType && tokens[0]->getName() == "=" )
    {
        Token* data_type = tokens[1];
        Token* data = tokens[2];
        if( data_type->getName() == "ascii" ) {
            assert( data->getType() == TokenType_StringType );
            const string& text = data->getName().substr(1, data->getName().size() - 2);
            for(int i = 0; i < text.size(); i++)
                pc = hbyte_write8(pc, text[i]);
        }
        else if ( data_type->getName() == "single" )
            pc = hbyte_write8(pc, convertNumber( data->getName() ) & 0xFF );
        else if ( data_type->getName() == "double" )
            pc = hbyte_write16(pc, convertNumber( data->getName() ) & 0xFFFF );
        else if ( data_type->getName() == "quad" )
            pc = hbyte_write32(pc, convertNumber( data->getName() ) & 0xFFFFFFFF );
        else if ( data_type->getName() == "rs" )
            for( int i = 0; i < convertNumber(data->getName()); i++)
                pc = hbyte_write8(pc, 0);
        else if ( data_type->getName() == "rd" )
            for( int i = 0; i < convertNumber(data->getName()); i++)
                pc = hbyte_write16(pc, 0);
        else if ( data_type->getName() == "rq" )
            for( int i = 0; i < convertNumber(data->getName()); i++)
                pc = hbyte_write32(pc, 0);
    }


}

int HByteCompiler::getRegister( string c ) {
    if      ( c == "ra" ) return 0;
    else if ( c == "rb" ) return 1;
    else if ( c == "rc" ) return 2;
    else if ( c == "rd" ) return 3;
    else if ( c == "re" ) return 4;
    else if ( c == "rf" ) return 5;
    else if ( c == "rg" ) return 6;
    else if ( c == "rh" ) return 7;
    else if ( c == "ri" ) return 8;
    else if ( c == "rj" ) return 9;
    else if ( c == "rk" ) return 10;
    else if ( c == "rl" ) return 11;
    else if ( c == "rm" ) return 12;
    else if ( c == "rn" ) return 13;
    else if ( c == "rs" ) return 14;
    else if ( c == "rp" ) return 15;

    return -1;
}

int HByteCompiler::getSymbols( string c ) {
    if ( c == "==" ) return 1;
    else if ( c == "<"  ) return 2;
    else if ( c == ">"  ) return 3;
    else if ( c == "<=" ) return 4;
    else if ( c == ">=" ) return 5;
    else if ( c == "<<" ) return 6;
    else if ( c == ">>" ) return 7;
    else if ( c == "!=" ) return 8;
    return 0;
}







