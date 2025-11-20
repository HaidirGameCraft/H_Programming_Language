// /**
//  * ins_make.c
//  * Created By Haidir
//  */
// #include <stdint.h>
// #include <inst.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <asrt.h>
// #include <regex.h>
// #include <ins_make.h>

// #include <ins/add.h>
// #include <ins/and.h>
// #include <ins/div.h>
// #include <ins/mov.h>
// #include <ins/mul.h>
// #include <ins/push.h>
// #include <ins/pop.h>
// #include <ins/sub.h>
// #include <ins/or.h>
// #include <ins/not.h>
// #include <ins/nand.h>
// #include <ins/nor.h>
// #include <ins/xor.h>
// #include <ins/stp.h>
// #include <ins/call.h>
// #include <ins/ret.h>
// #include <ins/go.h>

// uint8_t regbytext(const char* reg_text) {
//     if( strcmp(reg_text, "r8a") == 0 || strcmp(reg_text, "R8A") == 0)
//         return  0b00000;
//     else if( strcmp(reg_text, "r8b") == 0 || strcmp(reg_text, "R8B") == 0)
//         return  0b00001;
//     else if( strcmp(reg_text, "r8c") == 0 || strcmp(reg_text, "R8C") == 0)
//         return  0b00010;
//     else if( strcmp(reg_text, "r8d") == 0 || strcmp(reg_text, "R8D") == 0)
//         return  0b00011;
//     else if( strcmp(reg_text, "r8e") == 0 || strcmp(reg_text, "R8E") == 0)
//         return  0b00100;
//     else if( strcmp(reg_text, "r8f") == 0 || strcmp(reg_text, "R8F") == 0)
//         return  0b00101;
//     else if( strcmp(reg_text, "r8s") == 0 || strcmp(reg_text, "R8S") == 0)
//         return  0b00110;
//     else if( strcmp(reg_text, "r8bp") == 0 || strcmp(reg_text, "R8BP") == 0)
//         return  0b00111;
//     // 16 register
//     else if( strcmp(reg_text, "r16a") == 0 || strcmp(reg_text, "R16A") == 0)
//         return  0b01000;
//     else if( strcmp(reg_text, "r16b") == 0 || strcmp(reg_text, "R16B") == 0)
//         return  0b01001;
//     else if( strcmp(reg_text, "r16c") == 0 || strcmp(reg_text, "R16C") == 0)
//         return  0b01010;
//     else if( strcmp(reg_text, "r16d") == 0 || strcmp(reg_text, "R16D") == 0)
//         return  0b01011;
//     else if( strcmp(reg_text, "r16e") == 0 || strcmp(reg_text, "R16E") == 0)
//         return  0b01100;
//     else if( strcmp(reg_text, "r16f") == 0 || strcmp(reg_text, "R16F") == 0)
//         return  0b01101;
//     else if( strcmp(reg_text, "r16s") == 0 || strcmp(reg_text, "R16S") == 0)
//         return  0b01110;
//     else if( strcmp(reg_text, "r16bp") == 0 || strcmp(reg_text, "R16BP") == 0)
//         return  0b01111;
//     // 32 register
//     else if( strcmp(reg_text, "r32a") == 0 || strcmp(reg_text, "R32A") == 0)
//         return  0b10000;
//     else if( strcmp(reg_text, "r32b") == 0 || strcmp(reg_text, "R32B") == 0)
//         return  0b10001;
//     else if( strcmp(reg_text, "r32c") == 0 || strcmp(reg_text, "R32C") == 0)
//         return  0b10010;
//     else if( strcmp(reg_text, "r32d") == 0 || strcmp(reg_text, "R32D") == 0)
//         return  0b10011;
//     else if( strcmp(reg_text, "r32e") == 0 || strcmp(reg_text, "R32E") == 0)
//         return  0b10100;
//     else if( strcmp(reg_text, "r32f") == 0 || strcmp(reg_text, "R32F") == 0)
//         return  0b10101;
//     else if( strcmp(reg_text, "r32s") == 0 || strcmp(reg_text, "R32S") == 0)
//         return  0b10110;
//     else if( strcmp(reg_text, "r32bp") == 0 || strcmp(reg_text, "R32BP") == 0)
//         return  0b10111;
// }

// uint8_t rm_set(uint8_t bit, uint8_t sreg, uint8_t dreg) {
//     return ((sreg & 7) << 5) | ((dreg & 7) << 2) | (bit & 0b11);
// }

// int match_regex(const char* pattern, const char* text, int max_matches, regmatch_t** matches) {
//     regex_t regex;
//     int val = 0;
//     val = regcomp( &regex, pattern, REG_EXTENDED );
//     if( val != 0 )
//     {
//         printf("Doent match\n");
//         return -1;
//     }

//     *matches = (regmatch_t*) malloc( max_matches * sizeof( regmatch_t ) );

//     val = regexec( &regex, text, max_matches, *matches, 0 );
//     if (val != 0 && val != REG_NOMATCH ){
//         free( *matches );
//         *matches = NULL;
//         perror("Error to regex\n");
//     }

//     if( val == REG_NOMATCH )
//     {
//         free( *matches );
//         *matches = NULL;
//     }

//     regfree( &regex );
//     return val;
// }

// int create_instruction(uint8_t* memory, uint32_t pc, const char* instruction ) {
//     int index = 0;
//     char opcode[8] = {0};
//     char label[64] = {0};
//     char lReg[8] = {0};
//     char rReg[8] = {0};
//     uint32_t disp = 0;
//     uint32_t value = 0;

//     regmatch_t* regmatch = NULL;
//     int length_regmatch = 0;

//     if( match_regex(PATTERN_OPCODE_REG_REG, instruction, 4, &regmatch) == 0 )
//     {
//         // Opcode reg8/16/32 -> reg8/16/32
//         for(int i = 1; i < 4; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//                 memcpy( lReg, &instruction[start], end - start );
//             else if ( i ==  3 )
//                 memcpy( rReg, &instruction[start], end - start );
//         }

//         pc = create_instruction_add_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_mov_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_div_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_sub_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );

//         // Logic Gate
//         pc = create_instruction_and_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_or_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_nand_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_nor_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );
//         pc = create_instruction_xor_regmem_reg(opcode, lReg, rReg, NULL, NULL, memory, pc );

//     } else if ( match_regex(PATTERN_OPCODE_REG_MEMORY_REG, instruction, 5, &regmatch) == 0 ) {
//         // Opcode mem8/16/32 -> reg8/16/32
//         char disp_char[16] = {0};
//         for(int i = 1; i < 5; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//                 memcpy( lReg, &instruction[start], end - start );
//             else if ( i == 3 )
//             {
//                 memcpy( disp_char, &instruction[start], end - start );
//                 if( memcmp( disp_char, "0x", 2 ) == 0 ) // When it is Hexadecimal
//                 {
//                     sscanf(disp_char, "0x%x", &disp);
//                 } else {
//                     sscanf(disp_char, "%i", &disp);
//                 }
//             }
//             else if ( i ==  4 )
//                 memcpy( rReg, &instruction[start], end - start );
//         }

//         pc = create_instruction_add_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_and_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_div_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_mov_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_sub_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );

//         pc = create_instruction_or_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_nand_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_nor_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//         pc = create_instruction_xor_regmem_reg(opcode, lReg, rReg, &disp, NULL, memory, pc );
//     } else if ( match_regex(PATTERN_OPCODE_REG_REG_MEMORY, instruction, 5, &regmatch) == 0 )
//     {
//         // Opcode reg8/16/32 -> mem8/16/32
//         char disp_char[16] = {0};
//         for(int i = 1; i < 5; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//                 memcpy( lReg, &instruction[start], end - start );
//             else if ( i == 3 )
//                 memcpy( rReg, &instruction[start], end - start );
//             else if ( i == 4 )
//             {
//                 memcpy( disp_char, &instruction[start], end - start );
//                 if( memcmp( disp_char, "0x", 2 ) == 0 ) // When it is Hexadecimal
//                 {
//                     sscanf(disp_char, "0x%x", &disp);
//                 } else {
//                     sscanf(disp_char, "%i", &disp);
//                 }
//             }
//         }

//         pc = create_instruction_mov_reg_regmem(opcode, lReg, rReg, &disp, NULL, memory, pc );
//     } else if ( match_regex(PATTERN_OPCODE_VALUE_REG_MEMORY, instruction, 5, &regmatch) == 0 )
//     {
//         // Opcode value8/16/32 -> mem8/16/32
//         char disp_char[16] = {0};
//         for(int i = 1; i < 5; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//             {
//                 memcpy( disp_char, &instruction[start], end - start );
//                 if( memcmp( disp_char, "0x", 2 ) == 0 ) // When it is Hexadecimal
//                 {
//                     sscanf(disp_char, "0x%x", &value);
//                 } else {
//                     sscanf(disp_char, "%i", &value);
//                 }
//             }
//             else if ( i == 3 )
//                 memcpy( rReg, &instruction[start], end - start );
//             else if ( i == 4 )
//             {
//                 memcpy( disp_char, &instruction[start], end - start );
//                 if( memcmp( disp_char, "0x", 2 ) == 0 ) // When it is Hexadecimal
//                 {
//                     sscanf(disp_char, "0x%x", &disp);
//                 } else {
//                     sscanf(disp_char, "%i", &disp);
//                 }
//             }
//         }

//         pc = create_instruction_add_value_regmem(opcode, NULL, rReg, &disp, &value, memory, pc );
//         pc = create_instruction_mov_value_regmem(opcode, NULL, rReg, &disp, &value, memory, pc );

//     } else if ( match_regex(PATTERN_OPCODE_VALUE_REG, instruction, 4, &regmatch) == 0 )
//     {
//         // Opcode value8/16/32 -> reg8/16/32
//         char val[16] = {0};
//         for(int i = 1; i < 4; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//             {
//                 memcpy( val, &instruction[start], end - start );
//                 if( memcmp( val, "0x", 2 ) == 0 ) // When it is Hexadecimal
//                 {
//                     sscanf(val, "0x%x", &value);
//                 } else {
//                     sscanf(val, "%i", &value);
//                 }
//             }
//             else if ( i ==  3 )
//                 memcpy( rReg, &instruction[start], end - start );
//         }

        
//         pc = create_instruction_add_value_regmem(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_and_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_div_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_mov_value_regmem(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_mul_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_sub_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );

//         pc = create_instruction_or_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_nand_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_nor_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//         pc = create_instruction_xor_value_reg(opcode, NULL, rReg, NULL, &value, memory, pc );
//     } else if ( match_regex(PATTERN_OPCODE_LABEL_REG, instruction, 4, &regmatch) == 0 ) {
//         // Opcode label32 -> reg32
//         for(int i = 1; i < 4; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//             {
//                 memcpy( label, &instruction[start + 1], end - start - 1 );
//             }
//             else if ( i ==  3 )
//                 memcpy( rReg, &instruction[start], end - start );
//         }

//         pc = create_instruction_mov_addr_reg(opcode, NULL, rReg, NULL, 0, memory, pc );
//         push_label( label, strlen( label ), pc - 4);
//     }
//     else if ( match_regex(PATTERN_OPCODE_LABEL, instruction, 3, &regmatch ) == 0 ) {
//         // Opcode label32
//         for(int i = 1; i < 4; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//             {
//                 memcpy( label, &instruction[start + 1], end - start - 1 );
//             }
//         }

//         pc = create_instruction_call_label(opcode, NULL, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_go_label(opcode, NULL, NULL, NULL, NULL, memory, pc);
//         push_label(label, strlen( label ), pc - 4 );
//     }
//     else if ( match_regex(PATTERN_OPCODE_REG, instruction, 3, &regmatch) == 0 )
//     {
//         // Opcode reg8/16/32
//         char val[16] = {0};
//         for(int i = 1; i < 3; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//                 memcpy( lReg, &instruction[start], end - start );
//         }

//         pc = create_instruction_push_regmem(opcode, lReg, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_pop_reg(opcode, lReg, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_call_reg(opcode, lReg, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_go_regmem(opcode, lReg, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_not_reg(opcode, lReg, NULL, NULL, NULL, memory, pc );
//     }
//     else if ( match_regex(PATTERN_OPCODE_REG_MEMORY, instruction, 4, &regmatch) == 0 ) {
//         // Opcode mem8/16/32
//         char val[16] = {0};
//         char offset_char[16] = {0};
//         for(int i = 1; i < 4; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//                 memcpy( lReg, &instruction[start], end - start );
//             else if ( i == 3 )
//             {
//                 memcpy( offset_char, &instruction[start], end - start );
//                 if( strncmp(offset_char, "0x", 2) == 0 )
//                     sscanf(offset_char, "0x%x", &disp);
//                 else
//                     sscanf(offset_char, "%i", &disp);
//             }
//         }

//         pc = create_instruction_push_regmem(opcode, lReg, NULL, &disp, NULL, memory, pc );
//         pc = create_instruction_call_regmem(opcode, lReg, NULL, &disp, NULL, memory, pc );
//         pc = create_instruction_go_regmem(opcode, lReg, NULL, &disp, NULL, memory, pc );
//     }
//     else if ( match_regex(PATTERN_OPCODE_VALUE, instruction, 3, &regmatch) == 0 )
//     {
//         // Opcode value8/16/32
//         char val[16] = {0};
//         for(int i = 1; i < 3; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//             else if ( i == 2)
//             {
//                 memcpy( val, &instruction[start], end - start );
//                 if( strncmp(val, "0x", 2) == 0 )
//                     sscanf(val, "0x%x", &value);
//                 else
//                     sscanf(val, "%i", &value);
//             }
//         }

//         pc = create_instruction_push_value(opcode, NULL, NULL, NULL, &value, memory, pc );
//         pc = create_instruction_call_value(opcode, NULL, NULL, NULL, &value, memory, pc );
//         pc = create_instruction_go_value(opcode, NULL, NULL, NULL, &value, memory, pc );
//     } else if ( match_regex(PATTERN_DEFINED_LABEL, instruction, 2, &regmatch) == 0 ) {
//         // This pattern matching for only label name location
//         int size_label = regmatch[1].rm_eo - regmatch[1].rm_so + 1;
//         char* label_name = (char*) malloc( size_label );
//         memset( label_name, 0, size_label );
//         memcpy( label_name, &instruction[regmatch[1].rm_so], size_label - 1 );
//         update_label( label_name, size_label - 1, pc);

//         free( label_name );
//     } else if ( match_regex(PATTERN_OPCODE_ONLY, instruction, 2, &regmatch) == 0 ) {
//         for(int i = 1; i < 2; i++) {
//             regoff_t start = regmatch[i].rm_so;
//             regoff_t end = regmatch[i].rm_eo;

//             if( i == 1)
//                 memcpy(opcode, &instruction[start], end - start);
//         }

//         pc = create_instruction_stp(opcode, NULL, NULL, NULL, NULL, memory, pc );
//         pc = create_instruction_ret(opcode, NULL, NULL, NULL, NULL, memory, pc );
//     }

//     if( regmatch != NULL )
//         free( regmatch );
//     return pc;
// }


// static label_name_addr** list_label = 0;
// int length_list_label = 0;
// void new_label_name(const char* name, int size, uint32_t dest, uint32_t addr, uint8_t make_child) {
//     if( list_label == 0 )
//     {
//         list_label = (label_name_addr**) malloc( sizeof( label_name_addr* ) );
//     } else {
//         list_label = (label_name_addr**) realloc( list_label, (length_list_label + 1) * sizeof( label_name_addr* ) );    
//     }


//     label_name_addr* label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
//     list_label[length_list_label] = label;

//     char* _name = (char*) malloc( size + 1 );
//     memset(_name, 0, size + 1 );
//     memcpy(_name, name, size );

//     label->label_name = _name;
//     label->destination = 0;
//     label->address = addr;
//     label->next = NULL;
//     length_list_label++;

//     if( make_child == 1 )
//     {
//         label_name_addr* new_label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
//         new_label->address = 0;
//         new_label->destination = dest;
//         new_label->next = NULL;

//         label->next = new_label;
//     }
// }

// void push_label(const char* name, int size, uint32_t dest) {
//     for(int i = 0; i < length_list_label; i++) {
//         label_name_addr* label = list_label[i];
//         if( strcmp(label->label_name, name) == 0 )
//         {
//             while ( label != NULL )
//             {
//                 if( label->next == NULL )
//                 {
//                     label_name_addr* new_label = (label_name_addr*) malloc( sizeof( label_name_addr ) );
//                     new_label->address = label->address;
//                     new_label->destination = dest;
//                     new_label->next = NULL;
        
//                     label->next = new_label;
//                     break;
//                 }

//                 label = label->next;
//             }
            
//             return;
//         }
//     }

//     new_label_name( name, size, dest, 0, 1);
// }

// void update_label(const char* name, int size, uint32_t address ) {
//     for(int i = 0; i < length_list_label; i++) {
//         label_name_addr* label = list_label[i];
//         if( strcmp(label->label_name, name) == 0 )
//         {
//             label->address = address;
//             return;
//         }
//     }

//     new_label_name( name, size, 0, address, 0 );
// }

// void update_chlid_instruction(label_name_addr* label, uint32_t addr, uint8_t* memory) {
//     label_name_addr* crt = label;
//     while( crt != NULL ) // if crt is not NULL
//     {
//         write32(memory, crt->destination, addr );
//         label_name_addr* next = crt->next;
//         // go to next
//         crt = next;
//     }
// }

// void update_all_instruction_label(uint8_t* memory) {
//     for(int i = 0; i < length_list_label; i++) {
//         label_name_addr* label = list_label[i];
//         update_chlid_instruction(label->next, label->address, memory);
//     }

// }

// void list_label_instruction() {
//     for(int i = 0; i < length_list_label; i++) {
//         label_name_addr* label = list_label[i];
//         printf("Label: %s at 0x%8x\n", label->label_name, label->address );
//     }
// }

// void clear_child_label(label_name_addr* label) {
//     label_name_addr* crt = label;
//     while( crt != NULL ) // if crt is not NULL
//     {
//         label_name_addr* next = crt->next;
//         free( crt );

//         // go to next
//         crt = next;
//     }
// }

// void clear_label() {
//     for(int i = 0; i < length_list_label; i++)
//     {
//         label_name_addr* label = list_label[i];

//         // clearing the child label
//         clear_child_label(label->next);

//         free( label->label_name );
//         free( label );
//     }

//     free( list_label );
// }

// /**
//  * Instruction
//  */
// uint32_t instruction_reg_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.sreg = lReg & 0b111;
//     regmem.dreg = rReg & 0b111;

//     if( lReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( lReg & 0b1000 )
//         prefix |= PREFIX_16BITS;

//     if( (lReg & 0b11000) != (rReg & 0b11000) )
//         ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;

//     return pc;
// }
// uint32_t instruction_regmem_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint32_t offset) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.sreg = lReg & 0b111;
//     regmem.dreg = rReg & 0b111;

//     prefix |= PREFIX_MEMORY;
//     if( lReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( lReg & 0b1000 )
//         prefix |= PREFIX_16BITS;

//     if( (lReg & 0b11000) != (rReg & 0b11000) )
//         ASSERT("ERROR COMPILER: The bits of lreg is not same of rreg");

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;
//     if( prefix & PREFIX_32BITS )
//     {
//         write32(memory, pc, offset);
//         pc += 4;
//     } else if ( prefix & PREFIX_16BITS )
//     {
//         write16( memory, pc, offset );
//         pc += 2;
//     } else {
//         write8( memory, pc, offset ); 
//         pc++;
//     }

//     return pc;
// }

// uint32_t instruction_reg_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t lReg, uint8_t rReg, uint32_t offset) {
//     return instruction_regmem_reg(memory, pc, prefix, opcode, lReg, rReg, offset);
// }
// uint32_t instruction_label_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t value) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.dreg = rReg & 0b111;
//     prefix |= PREFIX_32BITS;

//     if( !(rReg & 0b10000) )
//         ASSERT("Compiler Error: LABEL -> REG, REG is not 32bits");

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;
//     // Value Origin
//     write32(memory, pc, 0); pc += 4;
//     return pc;
// }

// uint32_t instruction_value_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t value) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.dreg = rReg & 0b111;

//     if( rReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( rReg & 0b1000 )
//         prefix |= PREFIX_16BITS;


//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;
//     // Value Origin
//     if( prefix & PREFIX_32BITS )
//     {
//         write32(memory, pc, value);
//         pc += 4;
//     } else if ( prefix & PREFIX_16BITS )
//     {
//         write16( memory, pc, value );
//         pc += 2;
//     } else {
//         write8( memory, pc, value ); 
//         pc++;
//     }

//     return pc;
// }

// uint32_t instruction_value_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t offset, uint32_t value) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.dreg = rReg & 0b111;
    
//     prefix |= PREFIX_MEMORY;
//     if( rReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( rReg & 0b1000 )
//         prefix |= PREFIX_16BITS;

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;
//     if( prefix & PREFIX_32BITS )
//     {
//         write32(memory, pc, offset);
//         pc += 4;
//     } else if ( prefix & PREFIX_16BITS )
//     {
//         write16( memory, pc, offset );
//         pc += 2;
//     } else {
//         write8( memory, pc, offset ); 
//         pc++;
//     }
//     // Value Origin
//     if( prefix & PREFIX_32BITS )
//     {
//         write32(memory, pc, value);
//         pc += 4;
//     } else if ( prefix & PREFIX_16BITS )
//     {
//         write16( memory, pc, value );
//         pc += 2;
//     } else {
//         write8( memory, pc, value ); 
//         pc++;
//     }

//     return pc;
// }
// uint32_t instruction_reg(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.dreg = rReg & 0b111;

//     if( rReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( rReg & 0b1000 )
//         prefix |= PREFIX_16BITS;

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;

//     return pc;
// }
// uint32_t instruction_regmem(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint8_t rReg, uint32_t offset) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;
//     regmem.dreg = rReg & 0b111;

//     prefix |= PREFIX_MEMORY;
//     if( rReg & 0b10000 )
//         prefix |= PREFIX_32BITS;
//     else if ( rReg & 0b1000 )
//         prefix |= PREFIX_16BITS;

//     write8(memory, pc, prefix); pc++;
//     write8(memory, pc, opcode); pc++;
//     write8(memory, pc, *((uint8_t*) &regmem)); pc++;

//     if( prefix & PREFIX_32BITS )
//     {
//         write32(memory, pc, offset);
//         pc += 4;
//     } else if ( prefix & PREFIX_16BITS )
//     {
//         write16( memory, pc, offset );
//         pc += 2;
//     } else {
//         write8( memory, pc, offset ); 
//         pc++;
//     }

//     return pc;
// }

// uint32_t instruction_value(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode, uint32_t value ) {
//     ins_regmem_t regmem = {0};
//     regmem.bits = 0;

//     if( value > 0xFFFF )
//         prefix |= PREFIX_32BITS;
//     else if ( value > 0xFF )
//         prefix | PREFIX_16BITS;
        

//     write8( memory, pc, prefix ); pc++;
//     write8( memory, pc, opcode ); pc++;
//     if( prefix |= PREFIX_32BITS )
//     {
//         write32( memory, pc, value); 
//         pc += 4;
//     }
//     else if( prefix |= PREFIX_16BITS )
//     {
//         write16( memory, pc, value); 
//         pc += 2;
//     }
//     else {
//         write8( memory, pc, value); 
//         pc ++;
//     }
//     return pc;
// }

// uint32_t instruction_label(uint8_t* memory, uint32_t pc, uint8_t prefix, uint8_t opcode) {
//     prefix |= PREFIX_32BITS;
//     write8( memory, pc, prefix ); pc++;
//     write8( memory, pc, opcode ); pc++;
//     write32( memory, pc, 0); pc += 4;
//     return pc;
// }