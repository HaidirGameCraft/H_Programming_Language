#include <tools/tools.h>

char*  strcpy(const char* text, size_t length ) {
    char* ptr = ( char* ) malloc( length + 1 );
    memset( ptr, 0, length + 1 );
    memcpy( ptr, text, length );
    return ptr;
} 
char*  strtrim( const char* text ) {
    size_t length = strlen( text );
    char* ptr = ( char* ) malloc( length + 1 );
    memset( ptr, 0, length + 1 );
}