#include <cpu.h>
#include <inst.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <file_reader.h>

#define MAX_MEMORY_SIZE 0x00100000

int main(int argc, const char** argv) {
    
    uint8_t* memory = NULL;
    uint8_t is_compile = 0;
    uint8_t is_run = 0;
    const char* file = NULL;
    const char* output_file = NULL;
    for(int i = 0; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s -c|--compile <file.hm>; -> Compiling the raw text into binary code\n", argv[0]);
            printf("usage: %s -r <binary_file.ho>; -> running the binary file\n", argv[0] );
            printf("help: %s -h|--help\n", argv[0] );
            return EXIT_SUCCESS;
        } else if ( strcmp("-c", argv[i]) == 0 || strcmp("--compile", argv[i]) == 0 )
        {
            // compiling the raw text
            if( i + 1 >= argc )
            {
                printf("<file.hm> is not providing\n");
                return EXIT_FAILURE;
            }

            file = argv[i + 1];
            is_compile = 1;
        } else if( strcmp("-r", argv[i]) == 0 ) {
            if( i + 1 >= argc )
            {
                printf("<binary file> is not providing\n");
                return EXIT_FAILURE;
            }
            file = argv[i + 1];
            is_run = 1;
        } else if ( strcmp("-o", argv[i]) == 0 )
        {
            if( i + 1 >= argc )
            {
                printf("<output file> is not providing\n");
                return EXIT_FAILURE;
            }

            output_file = argv[i + 1];
        }
    }

    if( is_compile == 1 )
    {
        char* output_filename = output_file;
        if( output_file == NULL ) {
            output_filename = (char*) malloc( strlen(file) + 4 );
            memset(output_filename, 0, strlen(file) + 4);
            memcpy(output_filename, (char*) file, strlen(file));
            memcpy(&output_filename[strlen(file)], ".ho", 3);
        }

        
        //free( output_filename );
        uint32_t status = open_file_reader( file );
        if( status == 1 )
        {
            printf("Error: Cannot open file or no such file\n");
            if( output_file == NULL )
                free( output_filename );
            return EXIT_FAILURE;
        }
        uint8_t* memory = read_file_reader(MAX_MEMORY_SIZE);
        close_file_reader();
        
        FILE* binary = fopen(output_filename, "wb+");
        if( !binary )
        {
            perror("Error, Cannot open the file\n");
            free( memory );
            if( output_file == NULL )
                free( output_filename );
            return EXIT_FAILURE;
        }
        fwrite((char*) memory, 1, MAX_MEMORY_SIZE, binary);
        fclose( binary );

        if( output_file == NULL )
            free( output_filename );

        free( memory );
    } else if ( is_run == 1 ) {
        memory = (uint8_t*) malloc(MAX_MEMORY_SIZE);
        memset( memory, 0, MAX_MEMORY_SIZE );

        FILE* binary = fopen(file, "rb+");
        fseek(binary, 0, SEEK_END);
        int size = ftell(binary);
        fseek(binary, 0, SEEK_SET);

        if( !binary )
        {
            perror("Error, Cannot open the file\n");
            free( memory );
            return EXIT_FAILURE;
        }

        fread((char*) memory, 1, size, binary);

        cpu_initialize();
        cpu_set_memory(memory, MAX_MEMORY_SIZE);
        cpu_execute();
        cpu_output();

        free( memory );
    }

    //return EXIT_SUCCESS;
    // read file

    // uint8_t* memory = (uint8_t*) malloc(MAX_MEMORY_SIZE);
    // memset( memory, 0, MAX_MEMORY_SIZE );

    
    // int pc = 0;
    // pc = create_instruction(memory, pc, "mov 0x1000 -> r32s");
    // pc = create_instruction(memory, pc, "mov r32s -> r32bp");
    // pc = create_instruction(memory, pc, "mov 0x1234 -> r32c");
    // pc = create_instruction(memory, pc, "push r32s");
    // pc = create_instruction(memory, pc, "push 0xDEAD1234");
    // pc = create_instruction(memory, pc, "pop r32a");
    // pc = create_instruction(memory, pc, "sub r32b -> r32a");
    // pc = create_instruction(memory, pc, "mul 2 -> r32b");
    // pc = create_instruction(memory, pc, "mov (r32d + 0xFF8) -> r32c");
    // pc = create_instruction(memory, pc, "and 0xFF00 -> r32a");
    // // pc = mov_push_instruction(memory, pc, 0b1000, 0x40, rm_set(RM32, 0, REG_A), 0, 0);
    // // pc = mov_push_instruction(memory, pc, 0b1000, 0x42, rm_set(RM32, 0, REG_C), 0, 0xAABBCCDD);
    // // pc = mov_push_instruction(memory, pc, 0b1100, 0x40, rm_set(RM32, REG_A, REG_C), 0x1000, 0);
    // // pc = mov_push_instruction(memory, pc, 0b1100, 0x41, rm_set(RM32, REG_A, REG_B), 0x1000, 0);

    return EXIT_SUCCESS;
}