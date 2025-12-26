#include <inst.h>
#include <libc/file.h>

extern uint8_t* memory;
void SyscallInterrupt(int index, cpu_register_t *reg ) {
    if( reg->ra == 0 ) // open the file
    {
        char* filename = (char*) &memory[reg->rb];
        char* modes = (char*) &memory[reg->rd];
        reg->ra = FileOpen(filename, modes); 
    } else if ( reg->ra == 1 ) // close the file
    {
        uint32_t fd = reg->rd;
        FileClose(fd);
        reg->ra = 0;
    } else if ( reg->ra == 2 ) // read the file
    {
        uint32_t buffer = reg->rb;
        int size = reg->rc;
        int fd = reg->rd;
        FileRead(buffer, size, fd);
        reg->ra = 0;
    }
}

