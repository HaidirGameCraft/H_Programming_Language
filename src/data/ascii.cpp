#include "ascii.h"
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
        const char* mtext = match[i].str().c_str();
        int length = match[i].str().size();

        for(int j = 0; j < length; j++)
        {
            write8(memory, *pc, (uint8_t) match[i].str()[j]);
            *pc = *pc + 1;
        }
        
        // if( mtext[0] == '"' && mtext[length - 1] == '"' )    // Matching to the string
        // {
        //     int size_str = length - 2;
        //     int idx_str = 0;
        //     char* t = (char*) &mtext[1];
        //     while( idx_str < size_str )
        //     {
        //         write8(memory, *pc, t[idx_str]);
        //         *pc = *pc + 1;
        //         idx_str++;
        //     }
        // }

    }

    return 1;
}