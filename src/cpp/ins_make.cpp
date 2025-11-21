/**
 * ins_make.c
 * Created By Haidir
 */
#include <stdint.h>
#include <inst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asrt.h>
#include "ins_make.hpp"
#include <stdbool.h>
#include <iostream>

#include <data/ascii.h>
#include <data/datatype.h>

#include <ins/inst.h>
#include <cpp/ins_compiler.list.hpp>

uint8_t regbytext(const char* reg_text) {
    // 32 register
    if( strcmp(reg_text, "ra") == 0 || strcmp(reg_text, "RA") == 0)
        return  0b0000;
    else if( strcmp(reg_text, "rb") == 0 || strcmp(reg_text, "RB") == 0)
        return  0b0001;
    else if( strcmp(reg_text, "rc") == 0 || strcmp(reg_text, "RC") == 0)
        return  0b0010;
    else if( strcmp(reg_text, "rd") == 0 || strcmp(reg_text, "RD") == 0)
        return  0b0011;
    else if( strcmp(reg_text, "re") == 0 || strcmp(reg_text, "RE") == 0)
        return  0b0100;
    else if( strcmp(reg_text, "rf") == 0 || strcmp(reg_text, "RF") == 0)
        return  0b0101;
    else if( strcmp(reg_text, "rg") == 0 || strcmp(reg_text, "RG") == 0)
        return  0b0110;
    else if( strcmp(reg_text, "rh") == 0 || strcmp(reg_text, "RH") == 0)
        return  0b0111;
    else if( strcmp(reg_text, "ri") == 0 || strcmp(reg_text, "RI") == 0)
        return  0b1000;
    else if( strcmp(reg_text, "rj") == 0 || strcmp(reg_text, "RJ") == 0)
        return  0b1001;
    else if( strcmp(reg_text, "rk") == 0 || strcmp(reg_text, "RK") == 0)
        return  0b1010;
    else if( strcmp(reg_text, "rl") == 0 || strcmp(reg_text, "RL") == 0)
        return  0b1011;
    else if( strcmp(reg_text, "rm") == 0 || strcmp(reg_text, "RM") == 0)
        return  0b1100;
    else if( strcmp(reg_text, "rn") == 0 || strcmp(reg_text, "RN") == 0)
        return  0b1101;
    else if( strcmp(reg_text, "rs") == 0 || strcmp(reg_text, "RS") == 0)
        return  0b1110;
    else if( strcmp(reg_text, "rp") == 0 || strcmp(reg_text, "RP") == 0)
        return  0b1111;
}

uint8_t rm_set(uint8_t bit, uint8_t sreg, uint8_t dreg) {
    return ((sreg & 7) << 5) | ((dreg & 7) << 2) | (bit & 0b11);
}

uint32_t create_instruction(uint8_t* memory, uint32_t pc, const char* instruction ) {
    int index = 0;
    char opcode[8] = {0};
    char label[64] = {0};
    char lReg[8] = {0};
    char rReg[8] = {0};

    uint8_t PREFIX = 0;
    uint8_t LREG = 0;
    uint8_t RREG = 0;
    uint8_t MEMTYPE = 0;

    uint32_t offset = 0;
    uint32_t value = 0;

    


    //pc = opcode_label_as_reg_finder_instruction(memory, pc, instruction);
    //ASSERT("");
    int status = 0;
    status |= opcode_reg_reg_finder_instruction          (memory, &pc, instruction);
    status |= opcode_value_reg_finder_instruction        (memory, &pc, instruction);
    status |= opcode_label_reg_finder_instruction        (memory, &pc, instruction);
    status |= opcode_reg_value_finder_instruction        (memory, &pc, instruction);
    status |= opcode_reg_label_finder_instruction        (memory, &pc, instruction);
    status |= opcode_regoff_reg_finder_instruction       (memory, &pc, instruction);
    status |= opcode_valueoff_reg_finder_instruction     (memory, &pc, instruction);
    status |= opcode_labeloff_reg_finder_instruction     (memory, &pc, instruction);
    status |= opcode_reg_regoff_finder_instruction       (memory, &pc, instruction);
    status |= opcode_reg_valueoff_finder_instruction     (memory, &pc, instruction);
    status |= opcode_reg_labeloff_finder_instruction     (memory, &pc, instruction);
    status |= opcode_label_finder_instruction            (memory, &pc, instruction);
    status |= opcode_reg_finder_instruction              (memory, &pc, instruction);
    status |= opcode_only_finder_instruction             (memory, &pc, instruction);
    status |= opcode_value_finder_instruction            (memory, &pc, instruction);
    status |= defined_label_finder_instruction           (memory, &pc, instruction);

    // ASSERT("");


    // ascii data
    status |= ascii_variable_finder(instruction, memory, &pc );
    status |= datatype_define_finder(instruction, memory, &pc );

    if( status == 0 )
        __assert_fail("Unknown Instruction", instruction, pc,  __ASSERT_FUNCTION );

    // if( regmatch->size() == 0 )
    //     __assert_fail("Cannot Compile with Unknown Matches", instruction, pc, __ASSERT_FUNCTION);

    return pc;
}


static label_name_addr** list_label = 0;
int length_list_label = 0;
void new_label_name(const char* name, int size, uint32_t dest, uint32_t addr, uint8_t make_child) {
    if( list_label == 0 )
    {
        list_label = (label_name_addr**) malloc( sizeof( label_name_addr* ) );
    } else {
        list_label = (label_name_addr**) realloc( list_label, (length_list_label + 1) * sizeof( label_name_addr* ) );    
    }


    label_name_addr* label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
    list_label[length_list_label] = label;

    char* _name = (char*) malloc( size + 1 );
    memset(_name, 0, size + 1 );
    memcpy(_name, name, size );

    label->label_name = _name;
    label->destination = 0;
    label->address = addr;
    label->next = NULL;
    length_list_label++;

    
    if( make_child == 1 )
    {
        label_name_addr* new_label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
        new_label->address = 0;
        new_label->destination = dest;
        new_label->next = NULL;
        
        label->next = new_label;
    }
}

void push_label(const char* name, int size, uint32_t dest) {
    printf("New Label: %s\n", name);
    for(int i = 0; i < length_list_label; i++) {
        label_name_addr* label = list_label[i];
        if( strcmp(label->label_name, name) == 0 )
        {
            while ( label != NULL )
            {
                if( label->next == NULL )
                {
                    label_name_addr* new_label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
                    new_label->address = label->address;
                    new_label->destination = dest;
                    new_label->next = NULL;
        
                    label->next = new_label;
                    break;
                }

                label = label->next;
            }
            
            return;
        }
    }

    new_label_name( name, size, dest, 0, 1);
}

void update_label(const char* name, int size, uint32_t address ) {
    for(int i = 0; i < length_list_label; i++) {
        label_name_addr* label = list_label[i];
        if( strcmp(label->label_name, name) == 0 )
        {
            label->address = address;
            return;
        }
    }

    new_label_name( name, strlen( name ), 0, address, 0 );
}

void update_chlid_instruction(label_name_addr* label, uint32_t addr, uint8_t* memory) {
    label_name_addr* crt = label;
    while( crt != NULL ) // if crt is not NULL
    {
        write32(memory, crt->destination, addr );
        label_name_addr* next = crt->next;
        // go to next
        crt = next;
    }
}

void update_all_instruction_label(uint8_t* memory) {
    for(int i = 0; i < length_list_label; i++) {
        label_name_addr* label = list_label[i];
        update_chlid_instruction(label->next, label->address, memory);
    }

}

void list_label_instruction() {
    for(int i = 0; i < length_list_label; i++) {
        label_name_addr* label = list_label[i];
        printf("Label: %s at 0x%8x\n", label->label_name, label->address );
    }
}

void clear_child_label(label_name_addr* label) {
    label_name_addr* crt = label;
    while( crt != NULL ) // if crt is not NULL
    {
        label_name_addr* next = crt->next;
        free( crt );

        // go to next
        crt = next;
    }
}

void clear_label() {
    for(int i = 0; i < length_list_label; i++)
    {
        label_name_addr* label = list_label[i];

        // clearing the child label
        clear_child_label(label->next);

        free( label->label_name );
        free( label );
    }

    free( list_label );
}

