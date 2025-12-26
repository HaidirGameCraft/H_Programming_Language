#include "datatype.h"
#include "cpp/ins_make.hpp"
#include "tools/memory.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <tools/tools.h>
#include <cpp/ins_compiler.list.hpp>

static int datatype_define(const char* text, uint8_t* memory, uint32_t* pc, int __status) {
    REGEX_START(PATTERN_DATATYPE);
    REGEX_SEARCH(text);

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
        }

        __free( mtext );
    }

    return 1;
}

static int reserved_datatype_define(const char* text, uint8_t* memory, uint32_t* pc, int __status) {
    REGEX_START(PATTERN_DATATYPE_RESERVED);
    REGEX_SEARCH(text);

    uint8_t size_data = 0;
    for(int i = 1; i < match.size(); i++) {
        std::string  __match = match[i].str();
        char* mtext = (char*) strcpy(__match.c_str(), __match.size() );
        int length = __match.size();
        
        if( i == 1 )
        {
            if( strcmp("s", mtext) == 0 )
                size_data = 1;
            else if ( strcmp("d", mtext) == 0 )
                size_data = 2;
            else if ( strcmp("q", mtext) == 0 )
                size_data = 4;
            else
            {
                ASSERT("Error Compiling: is not an basic datatype");
            }
        }
        else if( i == 2 )    // Matching to the string
        {
            int value = read_number(mtext);
            while( value > 0 )
            {
                if( size_data == 1 )
                {
                    *pc = write8(memory, *pc, 0);
                } else if ( size_data == 2 )
                {
                    *pc = write16(memory, *pc, 0);
                } else if ( size_data == 4 )
                {
                    *pc = write32(memory, *pc, 0);
                }
                value--;
            }
        }

       checkAlloc(__free( mtext ));
    }

    return 1;
}

int datatype_define_finder(const char* text, uint8_t* memory, uint32_t* pc, int __status) {
   __status |= datatype_define(text, memory, pc, __status);
   __status |= reserved_datatype_define(text, memory, pc, __status);

   return __status;
}
