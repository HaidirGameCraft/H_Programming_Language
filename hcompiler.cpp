#include <stdio.h>
#include <file_reader.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#define MAX_MEMORY_SIZE 0x100000

int main(int argc, const char** argv) {
    uint8_t* memory = NULL;
    uint8_t is_compile = 0;
    uint8_t is_run = 0;
    std::string file = "";
    std::string output_file = "";
    for(int i = 0; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s -c|--compile <file.hm>; -> Compiling the raw text into binary code\n", argv[0]);
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
        std::string output_filename = output_file;
        if( output_file.empty() ) {
            output_filename = file;
            output_filename += ".ho";
        }

        
        //free( output_filename );
        uint32_t status = open_file_reader( file.c_str() );
        if( status == 1 )
        {
            printf("Error: Cannot open file or no such file\n");
            return EXIT_FAILURE;
        }
        uint8_t* memory = read_file_reader(MAX_MEMORY_SIZE);
        close_file_reader();
        
        FILE* binary = fopen(output_filename.c_str(), "wb+");
        if( !binary )
        {
            perror("Error, Cannot open the file\n");
            free( memory );
            return EXIT_FAILURE;
        }
        fwrite((char*) memory, 1, MAX_MEMORY_SIZE, binary);
        fclose( binary );

        free( memory );
    }
}