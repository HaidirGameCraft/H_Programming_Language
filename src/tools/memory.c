#include "memory.h"
#include <stdio.h>
#include <stdint.h>

typedef struct memory_store {
    void* ptr;
    struct memory_store* next;
} memory_store;

memory_store* memstart = NULL;
void* __malloc(size_t size) {

    // if memstart is NULL
    void* ptr = NULL;
    if( memstart == NULL )
    {
        memstart = (memory_store*) malloc( sizeof( memory_store ) );
        memstart->ptr = malloc( size );
        memstart->next = NULL;
        return memstart->ptr;
    }

    ptr = malloc( size );
    memory_store* new_mem = (memory_store*) malloc( sizeof( memory_store ) );
    new_mem->ptr = ptr;
    new_mem->next = NULL;

    memory_store* crt = memstart;
    while( crt->next != NULL )
        crt = crt->next;
    crt->next = new_mem;

    return ptr;
}

void* __realloc(void* ptr, size_t size) {
    void* crt_ptr = ptr;
    if( ptr == NULL )
        return ptr;

    if( memstart == NULL )
    {
        printf("Must be malloc first");
    }
    else {

        memory_store* crt = memstart;
        while( crt != NULL )
        {
            if( crt->ptr == ptr )
                break;
            crt = crt->next;
        }

        printf("[MEMORY] realloc: resize the (0x%.08x) to %i\n", (uint64_t) ptr, size);
        crt->ptr = realloc( ptr, size );
    }

    return crt_ptr;

}

void* __calloc(size_t nmemb, size_t size) {

}

void  __free(void *ptr) {
    memory_store* crt = memstart;
    memory_store* prev = NULL;
    while( crt != NULL )
    {

        if( crt->ptr == ptr )
        {
            free( crt->ptr );

            // remove the memory_store
            if( prev != NULL )
                prev->next = crt->next;
            if( crt != memstart )
                free( crt );
            return;
        }
        prev = crt;
        crt = crt->next;
    }

    return;
}

void  __memcheck() {
    memory_store* crt = memstart;

    if( crt->ptr != NULL )
    {
        printf("[Memory]: Freeing Address 0x%x\n", (uint64_t) crt->ptr);
        free( crt->ptr );
        crt->ptr = NULL;
    }
    crt = crt->next;

    while( crt != NULL )
    {
        if( crt->ptr != NULL )
        {
            printf("[Memory]: Freeing Address 0x%x\n", (uint64_t) crt->ptr);
            free( crt->ptr );
        }

        memory_store* next = crt->next;
        free( crt );
        crt = next;
    }

    free( memstart );
}


void  __memclear() {
    memory_store* crt = memstart;

    if( crt->ptr != NULL )
    {
        printf("[Memory]: Freeing Address 0x%x\n", (uint64_t) crt->ptr);
        free( crt->ptr );
    }
    crt = crt->next;

    while( crt != NULL )
    {
        if( crt->ptr != NULL )
        {
            printf("[Memory]: Freeing Address 0x%x\n", (uint64_t) crt->ptr);
            free( crt->ptr );
        }

        memory_store* next = crt->next;
        free( crt );
        crt = next;
    }

    free( memstart );
}
