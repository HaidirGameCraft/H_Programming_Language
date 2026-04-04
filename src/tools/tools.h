#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif


EXTERN char*  __strcpy(const char* text, size_t length );
char*  __strtrim( const char* text );
char*  __strjoin( char* text, const char* text_join);

