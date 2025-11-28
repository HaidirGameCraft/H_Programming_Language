#include "ins_compiler.list.hpp"
#include <iostream>
#include <string.h>
#include <tools/tools.h>

extern char* instruction_text;
extern int line_of_text;

#define ASSERT_WRONG_OPCODE     printf("\"%s\":%i -> Unknown Operand Name (%s)\n", instruction_text, line_of_text, opcode); \
                                ASSERT("unknown operand name");

PATTERN_FINDER(opcode_reg_reg) {
    REGEX_START( PATTERN_OPCODE_REG_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            lReg = regbytext( _c );
        }
        else if ( i == 3 ) // symbols char
        {
            if( strcmp("->", _c) != 0 )
                symbols = read_symbols_cnd( _c );
        }
        else if ( i == 4 )
        {
            rReg = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, rReg, symbols);
            return 1;
        }
    }

    ASSERT_WRONG_OPCODE

    return 0;
}

PATTERN_FINDER(opcode_value_reg) {
    REGEX_START( PATTERN_OPCODE_VALUE_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE


    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            value = read_number( _c );
        }
        else if ( i == 3 ) // symbols char
        {
            if( strcmp("->", _c) != 0 )
                symbols = read_symbols_cnd( _c );
        }
        else if ( i ==  4 )
            rReg = regbytext(_c);
    }
    

    int length = (sizeof( instruction_maker_opcode_value_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_value_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_value_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, rReg, symbols, value);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}
PATTERN_FINDER(opcode_label_reg) {
    REGEX_START( PATTERN_OPCODE_LABEL_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            memcpy( label, &_c[1], length - 1);
        }
        else if ( i == 3 ) // symbols char
        {
            if( strcmp("->", _c) != 0 )
                symbols = read_symbols_cnd( _c );   
        }
        else if ( i ==  4 )
            rReg = regbytext( _c );
    }

    int length = (sizeof( instruction_maker_opcode_label_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_label_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_label_reg(memory, *pc, htext->prefix, htext->ext_prefix | EXT_PREFIX_ADDRSIZE, htext->opcode, rReg, symbols, value);
            push_next_label_name( label, *pc - 4);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    // *pc = create_instruction_mov_addr_reg(opcode, NULL, rReg, NULL, 0, memory, *pc );
    

    return 0;
}

PATTERN_FINDER(opcode_reg_value) {
    REGEX_START( PATTERN_OPCODE_REG_VALUE );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE


    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            lReg = regbytext(_c);
        }
        else if ( i == 3 ) // symbols char
        {
            if( strcmp("->", _c) != 0 )
            symbols = read_symbols_cnd( _c );
        }
        else if ( i ==  4 )
            value = read_number( _c );
    }
    

    int length = (sizeof( instruction_maker_opcode_reg_value ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_value[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_value(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, symbols, value);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}

PATTERN_FINDER(opcode_reg_label) {
    REGEX_START( PATTERN_OPCODE_REG_LABEL );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            rReg = regbytext( _c );
        }
        else if ( i == 3 ) // symbols char
        {
            if( strcmp("->", _c) != 0 )
            symbols = read_symbols_cnd( _c );   
        }
        else if ( i ==  4 )
            memcpy( label, &_c[1], length - 1);
    }

    int length = (sizeof( instruction_maker_opcode_label_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_label_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_label(memory, *pc, htext->prefix, htext->ext_prefix | EXT_PREFIX_ADDRSIZE, htext->opcode, lReg, symbols, value);
            push_next_label_name( label, *pc - 4);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}


PATTERN_FINDER(opcode_regoff_reg) {
    REGEX_START( PATTERN_OPCODE_REGOFF_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            lReg = regbytext( _c );
        }
        else if ( i == 3 ) { // offset
            offset = (short) read_number( _c );
        }
        else if ( i ==  4 )
        {
            rReg = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_regoff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_regoff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_regoff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, rReg, offset);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE


    return 0;
}
PATTERN_FINDER(opcode_valueoff_reg) {
    REGEX_START( PATTERN_OPCODE_VALUEOFF_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            value = read_number( _c );
        }
        else if ( i == 3 ) { // offset
            offset = (short) read_number( _c );
        }
        else if ( i ==  4 )
        {
            rReg = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_valueoff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_valueoff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_valueoff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, rReg, offset, value );
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE


    return 0;
}
PATTERN_FINDER(opcode_labeloff_reg) {
    REGEX_START( PATTERN_OPCODE_LABELOFF_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            memcpy( label, &_c[1], length - 1);
        }
        else if ( i == 3 ) { // offset
            offset = read_number( _c );
        }
        else if ( i == 4 )
        {
            rReg = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_labeloff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_labeloff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_labeloff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, rReg, offset, 0 );
            push_next_label_name( label, *pc - 4 );
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE



    return 0;
}

PATTERN_FINDER(opcode_reg_regoff) {
    REGEX_START( PATTERN_OPCODE_REG_REGOFF );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            lReg = regbytext( _c );
        }
        else if ( i == 3 )
        {
            rReg = regbytext( _c );
        }
        else if ( i == 4 ) { // offset
            offset = read_number( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_regoff ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_regoff[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_regoff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, rReg, offset);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE


    return 0;
}
PATTERN_FINDER(opcode_reg_valueoff) {
    REGEX_START( PATTERN_OPCODE_REG_VALUEOFF );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            lReg = regbytext( _c );
        }
        else if ( i == 3 )
        {
            value = read_number( _c );
        }
        else if ( i == 4 ) { // offset
            offset = read_number( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_valueoff ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_valueoff[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_valueoff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, offset, value );
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE


    return 0;
}
PATTERN_FINDER(opcode_reg_labeloff) {
    REGEX_START( PATTERN_OPCODE_REG_LABELOFF );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
        {
            lReg = regbytext( _c );
        }
        else if ( i == 3 )
        {
            memcpy( label, &_c[1], length - 1);
        }
        else if ( i == 4 ) { // offset
            offset = read_number( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_labeloff ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_labeloff[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg_labeloff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg, offset, 0 );
            push_next_label_name( label, *pc - 4 );
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE


    return 0;
}

PATTERN_FINDER(opcode_label) {
    REGEX_START( PATTERN_OPCODE_LABEL );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            memcpy( label, &_c[1], length - 1);
        }
    }

    int length = (sizeof( instruction_maker_opcode_label ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_label[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {    
            *pc = instruction_label(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode);
            
            push_next_label_name(label, *pc - 4 );
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}
PATTERN_FINDER(opcode_reg) {
    REGEX_START( PATTERN_OPCODE_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
            lReg = regbytext( _c );
    }

    int length = (sizeof( instruction_maker_opcode_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, lReg);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}

PATTERN_FINDER(opcode_only) {
    REGEX_START( PATTERN_OPCODE_ONLY );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
    }

    int length = (sizeof( instruction_maker_opcode ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_opcode(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode);
            return 1;
        }
    }
    ASSERT_WRONG_OPCODE

    return 0;
}
PATTERN_FINDER(opcode_value) {
    REGEX_START( PATTERN_OPCODE_VALUE );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
        {
            if( strncmp(_c, "0x", 2) == 0 )
                sscanf(_c, "0x%x", &value);
            else
                sscanf(_c, "%i", &value);
        }
    }

    int length = (sizeof( instruction_maker_opcode_value ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_value[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_value(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, value);
            return 1;
        }
    }

    ASSERT_WRONG_OPCODE

    return 0;
}
PATTERN_FINDER(defined_label) {
    REGEX_START( PATTERN_DEFINED_LABEL );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    
    std::string str = match[1];
    update_address_label_name( str.c_str(), *pc );
    return 1;
}
