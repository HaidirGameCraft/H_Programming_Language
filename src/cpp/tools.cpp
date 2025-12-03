#include <tools/tools.h>
#include <tools/memory.h>

char*  strcpy(const char* text, size_t length ) {
    char* ptr = ( char* ) __malloc( length + 1 );
    memcpy( ptr, text, length );
    ptr[ length ] = 0;
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

char*  strjoin( char* text, const char* text_join ) {
    int _t1 = strlen( text );
    int _t2 = strlen( text_join );

    int length = _t1 + _t2;
    char* __result = (char*) __malloc( length + 1 );
    memcpy( __result, text, _t1 );
    memcpy( &__result[ _t1 ], text_join, _t2 );
    
    return __result;
}
