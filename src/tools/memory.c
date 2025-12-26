#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include <asrt.h>
#include <string.h>

#define MAX_MEMORY_CACHE    64

static uint64_t* __memory = NULL;
static int length_memory = 0;
static int max_length_memory = 0;
void* __malloc(size_t size) {

    // if memstart is NULL
#ifdef __DEBUG__
    void* ptr = NULL;
    if( __memory == NULL )
    {
        __memory = (uint64_t*) malloc( MAX_MEMORY_CACHE * sizeof( uint64_t ) );
        memset(__memory, 0, MAX_MEMORY_CACHE * sizeof( uint64_t ) );
        max_length_memory = MAX_MEMORY_CACHE;
    }
    else if ( length_memory >= max_length_memory ) {
        int __l = max_length_memory;
        max_length_memory = max_length_memory + MAX_MEMORY_CACHE;
        __memory = (uint64_t*) realloc( __memory, max_length_memory * sizeof( uint64_t ) );
        memset( &__memory[__l], 0, MAX_MEMORY_CACHE * sizeof( uint64_t ) );
    }

    ptr = (void*) malloc( size );
    __memory[length_memory] = (uint64_t) ptr;
    printf("[Memory]: New Memory Address at 0x%.08x with size %i\n", (uint64_t) ptr, size );
    length_memory++;
    return ptr;
#else
    return (void*) malloc( size );
#endif
}

void* __realloc(void* ptr, size_t size) {
#ifdef __DEBUG__
    void* crt_ptr = ptr;
    if( ptr == NULL )
        return ptr;

    if( __memory == NULL )
    {
        printf("Must be malloc first");
    }
    else {
        
        for(int i = 0; i < max_length_memory; i++) {
            if( __memory[i] == (uint64_t) ptr )
            {
                __memory[i] = (uint64_t) realloc(ptr, size);
                crt_ptr = (void*) __memory[i];
                break;
            }
        }


    }

    return crt_ptr;
#else
    return (void*) realloc( ptr, size );
#endif
}

void* __calloc(size_t nmemb, size_t size) {

}

void  __free(void *ptr) {
    if( ptr == NULL )
        return;
#ifdef __DEBUG__
    for( int i = 0; i < max_length_memory; i++) {
        if( __memory[i] == (uint64_t) ptr ) {
            printf("[Memory]: Freeing address 0x%.08x\n", __memory[i]);
            free( (void*) __memory[i] );
            __memory[i] = 0;
            length_memory--;
            return;
        }
    }
    return;
#else
    free( ptr );
#endif
}

void  __memcheck() {
#ifdef __DEBUG__
    for(int i = 0; i < max_length_memory; i++) {
        if( __memory[i] == 0 )
            continue;

        printf("[Memory]: This Memory is not free 0x%.08x\n", (uint64_t) __memory[i] );
    }

    __memclear();
#endif
}


void  __memclear() {
#ifdef __DEBUG__
    if( __memory == NULL )
        return;

    printf("Clear\n");

    for(int i = 0; i < max_length_memory; i++) {
        if( __memory[i] == 0 )
            continue;

        printf("[Memory]: Freeing the memory 0x%.08x\n", (uint64_t) __memory[i] );
        free( (void*) __memory[i] );
        __memory[i] = 0;
    }
    free( __memory );
    __memory = 0;
    max_length_memory = 0;
    length_memory = 0;
#endif
}


