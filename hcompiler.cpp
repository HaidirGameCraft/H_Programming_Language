#include <stdio.h>
#include <file_reader.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
#include <tools/memory.h>

#define MAX_MEMORY_SIZE 0x100000
FILE* output_file = NULL;
int main(int argc, const char** argv) {
    uint8_t* memory = NULL;
    uint8_t is_compile = 0;
    uint8_t is_run = 0;
    std::vector<std::string> filenames;
    std::string file = "";
    std::string __output_file = "";
    for(int i = 1; i < argc; i++) {
        if( strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0 )
        {
            printf("usage: %s -c|--compile <file.hm>; -> Compiling the raw text into binary code\n", argv[0]);
            printf("help: %s -h|--help\n", argv[0] );
            return EXIT_SUCCESS;
        } else if ( strcmp("-c", argv[i]) == 0 || strcmp("--compile", argv[i]) == 0 )
        {
            is_compile = 1;
        } else if ( strcmp("-o", argv[i]) == 0 )
        {
            if( i + 1 >= argc )
            {
                printf("<output file> is not providing\n");
                return EXIT_FAILURE;
            }

            __output_file = argv[i + 1];
        } else {
            std::string tmp( argv[i] );
            filenames.push_back( tmp );
        }
    }

    if( is_compile == 1 )
    {
        std::string __output_filename = __output_file;
        if( __output_file.empty() ) {
            __output_filename = file;
            __output_filename += ".ho";
        }

        
        //free( output_filename );
        make_file_reader(__output_filename.c_str() );
        uint32_t status = open_files_reader( filenames );
        if( status == 1 )
        {
            printf("Error: Cannot open file or no such file\n");
            return EXIT_FAILURE;
        }
        read_file_reader(MAX_MEMORY_SIZE);
        write_file_reader();
        close_file_reader();
        
        // FILE* binary = fopen(__output_file.c_str(), "wb+");
        // if( !binary )
        // {
        //     perror("Error, Cannot open the file\n");
        //     free( memory );
        //     return EXIT_FAILURE;
        // }
        // fwrite((char*) memory, 1, MAX_MEMORY_SIZE, binary);
        // fclose( binary );
    }

    __memcheck();
    __memclear();
}
