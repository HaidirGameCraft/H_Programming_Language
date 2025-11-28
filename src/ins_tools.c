/**
 * ins_tools.c
 * Created By Haidir
 */
#include <inst.h>
#include <ins_make.h>
#include <stdio.h>

#define MAX_REGMATCH_LENGTH     10

uint8_t read8(uint8_t* memory, uint32_t* pc) {
    uint8_t r = memory[*pc];
    (*pc)++;
    return r;
}
uint16_t read16(uint8_t* memory, uint32_t* pc) {
    uint16_t r = memory[*pc + 1] << 8 | memory[*pc];
    (*pc) += 2;
    return r;
}
uint32_t read32(uint8_t* memory, uint32_t* pc) {
    uint32_t r = memory[*pc + 3] << 24 | memory[*pc + 2] << 16 | memory[*pc + 1] << 8 | memory[*pc];
    (*pc) += 4;
    return r;
}
uint8_t gread8(uint8_t* memory, uint32_t pc) {
    uint8_t r = memory[pc];
    return r;
}
uint16_t gread16(uint8_t* memory, uint32_t pc) {
    uint16_t r = memory[pc + 1] << 8 | memory[pc];
    return r;
}
uint32_t gread32(uint8_t* memory, uint32_t pc) {
    uint32_t r = memory[pc + 3] << 24 | memory[pc + 2] << 16 | memory[pc + 1] << 8 | memory[pc];
    return r;
}

uint32_t write8(uint8_t* memory, uint32_t pc, uint8_t data) {
    memory[pc] = data & 0xFF;
    return pc + 1;
}
uint32_t write16(uint8_t* memory, uint32_t pc, uint16_t data) {
    write8(memory, pc, data & 0xFF);
    write8(memory, pc + 1, (data >> 8) & 0xFF);
    return pc + 2;
}
uint32_t write32( uint8_t* memory, uint32_t pc, uint32_t data) {
    write16( memory, pc, data & 0xFFFF );
    write16( memory, pc + 2, ( data >> 16 ) & 0xFFFF );
    return pc + 4;
}

uint32_t* getreg(cpu_register_t* reg, uint8_t r) {
    uint32_t* b = (uint32_t*) reg;
    if( r > 0xF )
    {
        ASSERT("Error: No such register");
        return NULL;
    }

    return &b[r];
}

/*
regex_matches_t* regex_matches(const char* pattern, const char* text) {
    regex_t regex;
    int val = 0;

    // build the regex component with pattern
    val = regcomp( &regex, pattern, REG_EXTENDED );
    if( val != 0 )
    {
        
        printf("Doent match\n");
        return NULL;
    }


    

    regex_matches_t* rmatch = (regex_matches_t*) malloc( sizeof( regex_matches_t ) );
    memset(rmatch, 0, sizeof( regex_matches_t ) );
    rmatch->match = (regmatch_t*) malloc( MAX_REGMATCH_LENGTH * sizeof( regmatch_t ) );
    memset( rmatch->match, 0, sizeof( regmatch_t ) * MAX_REGMATCH_LENGTH );

    // executing the regex matches with text
    val = regexec( &regex, text, MAX_REGMATCH_LENGTH, rmatch->match, 0 );
    if (val != 0 && val != REG_NOMATCH ){
        free_regex_matches( rmatch );
        rmatch = NULL;
        perror("Error to regex\n");
        regfree( &regex );
        return rmatch;
    }

    if( val == REG_NOMATCH )
    {
        free_regex_matches( rmatch );
        rmatch = NULL;
        regfree( &regex );
        return rmatch;
    }

    // finding the length on matches
    rmatch->length = MAX_REGMATCH_LENGTH;

    regfree( &regex );
    return rmatch;
}


void             free_regex_matches(regex_matches_t* ptr) {
    if( ptr == NULL )
        return;

    free( ptr->match );
    free( ptr );
}

*/




