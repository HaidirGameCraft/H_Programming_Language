// /**
//  * file_reader.c
//  * Created By Haidir
//  */

// #include <inst.h>
// #include <file_reader.h>
// #include <stdio.h>
// #include <ins_make.h>
// #include <stdlib.h>

// FILE *file = NULL;
// uint32_t read_line( const char* text, int size, uint8_t* memory, uint32_t pc );

// uint32_t open_file_reader( const char* filename ) {
//     file = fopen( filename, "r");
//     if( file == NULL )
//         return 1;


//     return 0;
// }

// uint8_t* read_file_reader( uint32_t max_memory ) {
//     int index = 0;
    
//     fseek( file, 0, SEEK_END );
//     long size = ftell( file );
//     fseek( file, 0, SEEK_SET );

//     uint8_t* memory = (uint8_t*) malloc( max_memory );

//     char* text = (char*) malloc( size + 1 );
//     fread(text, 1, size, file);
//     text[size] = 0;

//     int char_start = 0;
//     int char_end = 0;
//     int pc = 0;
//     while( index <= size )
//     {
//         if( text[index] == '\n' || text[index] == 0 )
//         {
//             pc = read_line( &text[char_start], char_end - char_start, memory, pc );
//             char_start = char_end + 1;
//         }

//         char_end++;
//         index++;
        
//     }

//     list_label_instruction();
//     update_all_instruction_label( memory );
//     clear_label();

//     free( text );
//     return memory;
// }

// void close_file_reader() {
//     fclose( file );
// }

// uint32_t read_line( const char* text, int size, uint8_t* memory, uint32_t pc ) {
//     // reading the line of codes

//     char* t_tmp = (char*) malloc( size + 1 );
//     memset(t_tmp, 0, size + 1);
//     memcpy(t_tmp, text, size );

//     // triming the text
//     // starting with start of string
//     int start_char = 0;
//     while( t_tmp[ start_char ] == ' ' )
//         start_char++;

//     // start with end of sting
//     int end_char = size;
//     while( t_tmp[end_char] == ' ')
//         end_char--;

//     t_tmp[end_char + 1] = 0;

//     if( t_tmp[start_char] == ';' ) {
//         // Ignoring the comment
//         free( t_tmp );

//         return pc;    
//     }
//     //printf("Text: %.*s\n", end_char - start_char, &t_tmp[start_char] );
//     pc = create_instruction(memory, pc, &t_tmp[start_char]);

//     free( t_tmp );

//     return pc;
// }