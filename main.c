#include <cpu.h>
#include <inst.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define MAX_MEMORY_SIZE 0x00100000

int main(int argc, const char** argv) {
    
    uint8_t* memory = NULL;
    uint32_t memory_size = MAX_MEMORY_SIZE;
    uint8_t save_ram = 0;
    const char* file = NULL;
    const char* output_file = NULL;
    bool show_register = false;
    for(int i = 0; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s [OPTIONS] <binary_file.ho>; -> running the binary file\n", argv[0] );
            printf("help: %s -h|--help\n", argv[0] );
            printf("===============================\n");
            printf("[OPTIONS]:\n");
            printf("-sr     --show-register : Showing the Output Register\n");
            printf("-h      --help          : help | details \n");
            printf("-save_ram               : save the memory to file\n");
            return EXIT_SUCCESS;
        } else if( strcmp("-rsize", argv[i]) == 0 ) {
            if( i + 1 >= argc )
            {
                printf("failed: the value is not insert or non");
                return EXIT_FAILURE;
            }

            sscanf(argv[i + 1], "%i", &memory_size);
        } else if ( strcmp(argv[i], "-save_ram") == 0 )
        {
            save_ram = 1;
        } else if ( strcmp(argv[i], "-sr") == 0 || strcmp(argv[i], "--show-register") == 0 )
        {
            show_register = true;
        }
        else {
            file = argv[i];
        }
    }

    memory = (uint8_t*) malloc(memory_size);
    memset( memory, 0, memory_size );

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
    cpu_set_memory(memory, memory_size);
    cpu_execute();
    if( show_register )
        cpu_output();

    if( save_ram )
    {
        FILE* ram = fopen("ram", "wb+");
        if( !ram )
        {
            perror("Error, Cannot open the file\n");
            free( memory );
            return EXIT_FAILURE;
        }

        fwrite((char*) memory, 1, MAX_MEMORY_SIZE, ram);
    }

    free( memory );

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
