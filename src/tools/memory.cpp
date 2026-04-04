#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include <asrt.h>
#include <string.h>

#define MAX_MEMORY_CACHE    64

typedef struct {
    void* ptr;
    size_t size;
} allocItems;

allocItems* __memory = NULL;
int length_memory = 0;
int max_length_memory = 0;
int idx_zero_index = 0;

// Debug
int total_malloc = 0;
int total_free = 0;

void* __malloc(size_t size) {
    total_malloc = total_malloc + 1;
    // if memstart is NULL

    void* ptr = (void*) malloc( size );
    memset(ptr, 0, size);

    if( __memory == NULL )
    {
        __memory = (allocItems*) malloc( MAX_MEMORY_CACHE * sizeof( allocItems ) );
        memset(__memory, 0, MAX_MEMORY_CACHE * sizeof( allocItems ) );
        max_length_memory = MAX_MEMORY_CACHE;
    }
    
    for( int i = idx_zero_index; i < max_length_memory; i++ )
    {
        if( __memory[i].ptr == 0 )
        {
            __memory[i].ptr = ptr;
            __memory[i].size = size;
            idx_zero_index = i;
            return ptr;
        }
    }

    int __l = max_length_memory;
    max_length_memory += MAX_MEMORY_CACHE;
    __memory = (allocItems*) realloc( __memory, max_length_memory * sizeof( allocItems ) );
    memset( &__memory[__l], 0, MAX_MEMORY_CACHE * sizeof( allocItems ) );

    __memory[__l].ptr = ptr;
    __memory[__l].size = size;
    idx_zero_index = __l;
#ifdef __DEBUG__
    printf("[Memory]: New Memory Address at 0x%.08x with size %i\n", (uint64_t) ptr, size );
#endif
    return ptr;
}

void* __realloc(void* ptr, size_t size) {
    void* crt_ptr = ptr;
    if( ptr == NULL )
        return ptr;

    if( __memory == NULL )
    {
        printf("Must be malloc first");
    }
    else {
        
        for(int i = 0; i < max_length_memory; i++) {
            if( __memory[i].ptr == ptr )
            {
                __memory[i].ptr = realloc(ptr, size);
                __memory[i].size = size;
                crt_ptr = (void*) __memory[i].ptr;
                break;
            }
        }


    }

    return crt_ptr;
}

size_t __getSize( void* ptr ) {
    for( int i = 0; i < max_length_memory; i++)
    {
        if( __memory[i].ptr == ptr )
            return  __memory[i].size;
    }

    return -1;
}

void* __calloc(size_t nmemb, size_t size) {

}

void* __copy(void* ptr ) {
    if( ptr == NULL )
        return NULL;
    size_t size = __getSize( ptr );
    if( size == -1 )
        return NULL;
    void* newPtr = __malloc( size );
    memcpy( newPtr, ptr, size );
    return newPtr;
}

void  __free(void *ptr) {
    if( ptr == NULL )
        return;

    for( int i = 0; i < max_length_memory; i++) {
        if( __memory[i].ptr == ptr ) {
#ifdef __DEBUG__
            printf("[Memory]: Freeing address 0x%.08x\n", __memory[i]);
#endif
            free( (void*) __memory[i].ptr );
            __memory[i].ptr = 0;
            __memory[i].size = 0;
            total_free = total_free + 1;
            if( idx_zero_index > i )
                idx_zero_index = i;
            return;
        }
    }
    return;
}

void  __memcheck() {
    for(int i = 0; i < max_length_memory; i++) {
        if( __memory[i].ptr == 0 )
            continue;

#ifdef __DEBUG__
        printf("[Memory]: This Memory is not free 0x%.08x\n", (uint64_t) __memory[i] );
#endif
    }

    __memclear();
}


void  __memclear() {
    if( __memory == NULL )
        return;

#ifdef __DEBUG__
    printf("Clear\n");
    printf("[Memory]: Total use Malloc: %i\n", total_malloc);
    printf("[Memory]: Total use Free: %i\n",   total_free);
    printf("[Memory]: Total not Free: %i\n", total_malloc - total_free);
#endif

    for(int i = 0; i < max_length_memory; i++) {
        if( __memory[i].ptr == 0 )
            continue;
#ifdef __DEBUG__
        printf("[Memory]: Freeing the memory 0x%.08x\n", (uint64_t) __memory[i] );
#endif
        free( (void*) __memory[i].ptr );
        __memory[i].ptr = 0;
    }
    free( __memory );
    __memory = 0;
    max_length_memory = 0;
}


