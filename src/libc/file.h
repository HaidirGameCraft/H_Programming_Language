#pragma once

#include <stdint.h>
int FileOpen( const char* filename, const char* modes );
void FileRead( uint32_t buffer, int size, int fd );
void FileWrite( uint32_t buffer, int size, int fd );
void FileClose( int fd );


