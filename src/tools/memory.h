#pragma once

#include <stdlib.h>

#ifdef __DEBUG__
#define checkAlloc(x) \
    printf("Test -> %s:%i\n", __FILE__, __LINE__); \
    x;\
    printf("Test Done\n");
#else
#define checkAlloc(x) x
#endif

void* __malloc(size_t size);
void* __realloc(void* ptr, size_t size);
void* __calloc(size_t nmemb, size_t size);
void  __free(void *ptr);
void  __memcheck();
void  __memclear();
