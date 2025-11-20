#include "ins_compiler.list.hpp"
#include <iostream>
#include <string.h>

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
            LREG = regbytext( _c );
        }
        else if ( i ==  3 )
        {
            RREG = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_reg_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG, RREG);
    }


    return 1;
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
            if( memcmp( _c, "0x", 2 ) == 0 ) // When it is Hexadecimal
            {
                sscanf(_c, "0x%x", &value);
            } else {
                sscanf(_c, "%i", &value);
            }

        }
        else if ( i ==  3 )
            RREG = regbytext(_c);
    }
    

    int length = (sizeof( instruction_maker_opcode_value_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_value_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_value_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, RREG, value);
    }

    return 1;
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
        else if ( i ==  3 )
            RREG = regbytext( _c );
    }

    int length = (sizeof( instruction_maker_opcode_label_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_label_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
        {
            *pc = instruction_label_reg(memory, *pc, htext->prefix, htext->ext_prefix | EXT_PREFIX_ADDRSIZE, htext->opcode, RREG, value);
        }
    }

    // *pc = create_instruction_mov_addr_reg(opcode, NULL, rReg, NULL, 0, memory, *pc );
    push_label( label, strlen( label ), *pc - 4);
    

    return 1;
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
            LREG = regbytext( _c );
        }
        else if ( i == 3 ) { // offset
            offset = read_number( _c );
        }
        else if ( i ==  4 )
        {
            RREG = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_regoff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_regoff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_regoff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG, RREG, offset);
    }


    return 1;
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
            offset = read_number( _c );
        }
        else if ( i ==  4 )
        {
            RREG = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_valueoff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_valueoff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_valueoff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, RREG, offset, value );
    }


    return 1;
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
        else if ( i ==  4 )
        {
            RREG = regbytext( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_labeloff_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_labeloff_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_labeloff_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, RREG, offset, 0 );
    }

    push_label(label, strlen( label ), *pc - 4 );


    return 1;
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
            LREG = regbytext( _c );
        }
        else if ( i ==  3 )
        {
            RREG = regbytext( _c );
        }
        else if ( i == 4 ) { // offset
            offset = read_number( _c );
        }
    }

    int length = (sizeof( instruction_maker_opcode_reg_regoff ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_regoff[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_reg_regoff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG, RREG, offset);
    }


    return 1;
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
            LREG = regbytext( _c );
        }
        else if ( i ==  3 )
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
            *pc = instruction_reg_valueoff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG, offset, value );
    }


    return 1;
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
            LREG = regbytext( _c );
        }
        else if ( i ==  3 )
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
            *pc = instruction_reg_labeloff(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG, offset, 0 );
    }

    push_label( label, strlen( label ), *pc - 4 );

    return 1;
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
            push_label(label, strlen( label ), *pc - 4 );
        }
    }

    return 1;
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
            LREG = regbytext( _c );
    }

    int length = (sizeof( instruction_maker_opcode_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, LREG);
    }

    return 1;
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
            *pc = instruction_opcode(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode);
    }

    return 1;
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
            *pc = instruction_value(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, value);
    }

    return 1;
}
PATTERN_FINDER(defined_label) {
    REGEX_START( PATTERN_DEFINED_LABEL );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    
    std::string str = match[1];
    char* label_name = (char*) malloc( str.size() + 1 );
    memset( label_name, 0, str.size() +  1 );
    memcpy( label_name, str.c_str(), str.size() );

    update_label( label_name, str.size(), *pc);
    
    free( label_name );

    return 1;
}





PATTERN_FINDER(opcode_reg_sym_reg) {
    REGEX_START( PATTERN_OPCODE_REG_SYM_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    

    uint8_t sym_op = 0;
    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2)
            LREG = regbytext( _c );
        else if ( i == 3 )
        {
            sym_op = read_symbols_cnd( _c );
            if( sym_op == 0 )
                ASSERT("Error Compiling: SYMBOLS CND is unknowns");

            }
        else if ( i == 4 )
            RREG = regbytext( _c );
    }
        
    int length = (sizeof( instruction_maker_opcode_reg_sym_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_reg_sym_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_reg_sym_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, sym_op, LREG, RREG);
    }

    return 1;
}

PATTERN_FINDER(opcode_value_sym_reg) {
    REGEX_START( PATTERN_OPCODE_VALUE_SYM_REG );
    REGEX_SEARCH( text );
    INITIALIZE_VARIABLE

    uint8_t sym_op = 0;
    for(int i = 1; i < match.size(); i++)
    {
        auto m = match[i];
        std::string t = m;
        int length = t.size();
        const char* _c = t.c_str();

        if( i == 1)
            memcpy(opcode, _c, length);
        else if ( i == 2 )
            value = (uint32_t) read_number(_c);
        else if ( i == 3 )
        {
            sym_op = read_symbols_cnd( _c );
            if( sym_op == 0 )
                ASSERT("Error Compiling: SYMBOLS CND is unknowns");
        }
        else if ( i == 4)
            RREG = regbytext( _c );
    }

    int length = (sizeof( instruction_maker_opcode_value_sym_reg ) / sizeof( h_text_compile ));
    for(int i = 0; i < length; i++) {
        h_text_compile* htext = &instruction_maker_opcode_value_sym_reg[i];
        if( strcmp( htext->opcode_name, opcode) == 0 )
            *pc = instruction_value_sym_reg(memory, *pc, htext->prefix, htext->ext_prefix, htext->opcode, sym_op, RREG, value);
    }

    return 1;
}