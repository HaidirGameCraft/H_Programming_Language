#include "ascii.h"
#include "asrt.h"
#include <string.h>
#include <stdio.h>
#include <inst.h>
#include <cpp/ins_compiler.list.hpp>

int ascii_variable_finder(std::string text, uint8_t* memory, uint32_t* pc, int __status) {
    REGEX_START( PATTERN_ASCII );
    std::smatch match;
    if( std::regex_search(text, match, regex) == false )
        return 0;

    uint8_t size_data = 0;
    for(int i = 1; i < match.size(); i++) {
        std::string text = match[i].str();
        const char* mtext = text.c_str();
        int length = text.size();

        for(int j = 0; j < length; j++)
        {
            if( mtext[j] == '\\' )
            {
                if( j + 1 >= length )
                    ASSERT("ASCII Error: \\ is not has child");

                if( mtext[j+1] == 'n' )
                    *pc = write8(memory, *pc, '\n');
                else if ( mtext[j+1] == '0' )
                    *pc = write8( memory, *pc, '\0');
                j++;
            } else 
                *pc = write8(memory, *pc, (uint8_t) mtext[j]);
        }

        // add Null Terminator
        *pc = write8(memory, *pc, 0);
    }

    return 1;
}
