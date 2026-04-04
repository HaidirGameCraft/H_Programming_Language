#pragma once

#include <stdio.h>

typedef struct node_program_t {
    char* text;
    char* name;
    struct node_program_t* child;
    struct node_program_t* next;
} node_program_t;

node_program_t* nodeprogram_create();
node_program_t* nodeprogram_push( node_program_t* parent, const char* text );
node_program_t* nodeprogram_pushChild( node_program_t* parent, const char* text );
void nodeprogram_clear( node_program_t* node );
void nodeprogram_print( node_program_t* node , int indent);