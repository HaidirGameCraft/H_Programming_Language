#include <tools/tools.h>
#include <tools/memory.h>

char*  strcpy(const char* text, size_t length ) {
    char* ptr = ( char* ) __malloc( length + 1 );
    memset( ptr, 0, length + 1 );
    memcpy( ptr, text, length );
    return ptr;
}

char*  strtrim( const char* text ) {
    int length = strlen( text );
    int s = 0;
    int e = length;

    while( text[s] == ' ' && s < length )
        s++;

    while( text[e] == ' ' && e >= 0 )
        e--;

    if( e <= s )
        return NULL;

    char* __tmp = (char*) strcpy( &text[s], e - s );
    return __tmp;
}
