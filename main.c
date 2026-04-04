#include "file_reader.h"
#include <cpu.h>
#include <inst.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <term.h>
#include <interrupt.h>
#include "memory.h"

#define MAX_MEMORY_SIZE 0x00100000

int main(int argc, const char** argv) {
    
    uint8_t* memory = NULL;
    uint32_t memory_size = MAX_MEMORY_SIZE;
    uint8_t save_ram = 0;
    uint8_t binary_file = 0;
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
            printf("-ub     --use-binary    : read Binary File format\n");
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
        }
        else if ( strcmp(argv[i], "-ub") == 0 || strcmp( argv[i], "--use-binary") == 0 )
        {
            binary_file = 1;
        } 
        else if ( strcmp(argv[i], "-sr") == 0 || strcmp(argv[i], "--show-register") == 0 )
        {
            show_register = true;
        }
        else {
            file = argv[i];
        }
    }

    // printf("\x1B[2J\x1B[H");


    //  Creating virtual memory
    memory = create_memory( memory_size );

//    FILE* binary = fopen(file, "rb+");
//    fseek(binary, 0, SEEK_END);
//    int size = ftell(binary);
//    fseek(binary, 0, SEEK_SET);

//    if( !binary )
//    {
//        perror("Error, Cannot open the file\n");
//        free( memory );
//        return EXIT_FAILURE;
//    }

//    fread((char*) memory, 1, size, binary);

    if( !binary_file ) {
        h_openFile( file );
        h_loadFile( memory );
        h_closeFile();
    } else {
        bin_openFile( file );
        bin_loadFile( memory );
        bin_closeFile();
    }

    enableRawMode();
    InitInterrupt();
    cpu_initialize();
    
    int width = 0;
    int height = 0;
    GetSizeTerminal(&width, &height);
    // printf("Terminal Size: %i:%i\n", width, height);

    cpu_set_memory( get_memory(), memory_size );
    cpu_execute( 0 );
    if( show_register )
        cpu_output();

    if( save_ram )
    {
        FILE* ram = fopen("ram", "wb+");
        if( !ram )
        {
            perror("Error, Cannot open the file\n");
            clear_memory();
            return EXIT_FAILURE;
        }

        fwrite((char*) memory, 1, MAX_MEMORY_SIZE, ram);
    }

    clear_memory();

    return EXIT_SUCCESS;
}
