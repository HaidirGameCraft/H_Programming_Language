#include <tools/memory.h>
#include <tools/tools.h>
#include <virt/bios/BIOS.h>
#include <virt/emulator.h>

#include <stdint.h>

#define MAX_MEMORY 0x100000

void exit_handle() {
    __memclear();
}

int main( int argc, const char** argv ) {
    atexit( exit_handle );
    for( int i = 1; i < argc; i++ )
    {
        if( strcmp(argv[i], "-pd") == 0 )
        {
            BIOS::primary_disk.OpenDisk( argv[i + 1] );
            i = i + 1;
        } else if ( strcmp(argv[i], "-sd") == 0 ) {
            BIOS::secondary_disk.OpenDisk( argv[i + 1] );
            i = i + 1;
        }
    }

    EmulatorWindow window("Virtual Machine", 800, 600);
    uint8_t* memory = ( uint8_t* ) __malloc( MAX_MEMORY );
    BIOS::memory = memory;

    FILE* bios_file = fopen("test/bios/bios.ho", "rb+");
    if( !bios_file )
    {
        printf("Error: bios.ho not found\n");
        __memclear();
        return 1;
    }
    fseek( bios_file, 0, SEEK_END );
    size_t bios_size = ftell( bios_file );
    fseek( bios_file, 0, SEEK_SET );
    fread(memory, bios_size, 1, bios_file );

    window.Init();
    window.Run();
    
    __memclear();
    return 0;
}