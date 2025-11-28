#pragma once

#include <stdlib.h>

void* __malloc(size_t size);
void* __realloc(void* ptr, size_t size);
void* __calloc(size_t nmemb, size_t size);
void  __free(void *ptr);
void  __memcheck();
void  __memclear();
