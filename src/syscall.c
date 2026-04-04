#include <inst.h>
#include <libc/file.h>
#include "memory.h"

void SyscallInterrupt(int index, cpu_register_t *reg ) {
    memory_t memory = get_memory();
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
    // printf [STD]
    else if ( reg->ra == 3 )
    {
        uint32_t stack_frame = reg->rp; // Stack Base/Stack Frame
        stack_frame = stack_frame + 8; // skipping rp, return address

        int index = 0;
        uint32_t format_ptr = read32_memory( stack_frame );
        stack_frame += 4;
        while( read8_memory( format_ptr + index ) != 0 )
        {
            if( read8_memory( format_ptr + index ) == '%' )
            {
                index++;
                if( read8_memory( format_ptr + index ) == 'i' )
                {
                    printf("%i", read32_memory( stack_frame ) );
                    stack_frame += 4;
                } else if ( read8_memory( format_ptr + index ) == 'x' )
                {
                    printf("%x", read32_memory( stack_frame ) );
                    stack_frame += 4;
                } else if ( read8_memory( format_ptr + index ) == 's' )
                {
                    printf("%s", &memory[read32_memory( stack_frame )] );
                    stack_frame += 4;
                } else if ( read8_memory( format_ptr + index ) == 'c' ) {
                    printf("%c", (char) read8_memory( stack_frame ) );
                    stack_frame += 4;
                } else {
                    printf("%c", (char) read8_memory( format_ptr + (index - 1) ) );
                    printf("%c", (char) read8_memory( format_ptr + index ) );
                }
            } else if( read8_memory( format_ptr + index ) == '\\' )
            {
                index++;
                char __c = read8_memory( format_ptr + index );
                // New Line
                if( __c == 'n' )
                    printf("\n" );
                else if ( __c == 't' )
                    printf("\t");
            } 
            else {
                printf("%c", (char) read8_memory( format_ptr + index ) );
            }

            index++;
        }
    }
}

