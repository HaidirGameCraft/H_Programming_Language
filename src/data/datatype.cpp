#include "datatype.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <tools/tools.h>
#include <cpp/ins_compiler.list.hpp>

int datatype_define_finder(std::string text, uint8_t* memory, uint32_t* pc, int __status) {
    REGEX_START( PATTERN_DATATYPE );
    std::smatch match;
    if( std::regex_search(text, match, regex) == false )
        return 0;


    uint8_t size_data = 0;
    for(int i = 1; i < match.size(); i++) {
        std::string  __match = match[i].str();
        char* mtext = (char*) strcpy(__match.c_str(), __match.size() );
        int length = strlen( mtext );
        
        if( i == 1 )
        {
            if( strcmp("single", mtext) == 0 )
                size_data = 1;
            else if ( strcmp("double", mtext) == 0 )
                size_data = 2;
            else if ( strcmp("quad", mtext) == 0 )
                size_data = 4;
            else
            {
                ASSERT("Error Compiling: is not an basic datatype");
            }
        }
        else if( i == 2 )    // Matching to the string
        {
            uint32_t value = 0;
            if( strncmp(mtext, "0x", 2) == 0 )
            {
                sscanf(mtext, "0x%x", &value);
                if( size_data == 1 )
                {
                    write8(memory, *pc, value);
                } else if ( size_data == 2 )
                {
                    write16(memory, *pc, value);
                } else if ( size_data == 4 )
                {
                    write32(memory, *pc, value);
                }
                *pc = *pc + size_data;
            } else
            {
                sscanf(mtext, "%i", &value);
                if( size_data == 1 )
                {
                    write8(memory, *pc, value);
                } else if ( size_data == 2 )
                {
                    write16(memory, *pc, value);
                } else if ( size_data == 4 )
                {
                    write32(memory, *pc, value);
                }
                *pc = *pc + size_data;
            }
            //printf("Test 2...\n");
        }

        __free( mtext );
    }

    return 1;
}
