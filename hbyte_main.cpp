#include "tools/memory.h"
#include <hbyte/filereader.h>
#include <cstdlib>
#include <string>
#include <string.h>


int main(int argc, const char* argv[]) {
    std::string filename;
    std::string outfilename;

    for(int i = 1; i < argc; i++) {

        if( strcmp( argv[i], "-o") == 0 )
        {
            assert( i + 1 < argc );
            outfilename = argv[i + 1];
            i++;
        } else {
            filename = argv[i];
        }
    }

    printf("Filename: %s\n", filename.c_str());
    printf("Output Filename: %s\n", outfilename.c_str());

    hbyte_open_file(filename.c_str());
    hbyte_create_file(outfilename.c_str());
    hbyte_compile();
    hbyte_close_files();
    __memclear();
    return EXIT_SUCCESS;
}
