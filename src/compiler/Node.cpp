#include "Node.h"

#include <tools/memory.h>
#include <tools/tools.h>
#include <string.h>

node_program_t* nodeprogram_create( ) {
    node_program_t* node = ( node_program_t* ) __malloc( sizeof( node_program_t ) );
    node->text = "[Start]";
    node->child = NULL;
    node->name = NULL;
    node->next = NULL;

    return node;
}

node_program_t* nodeprogram_push( node_program_t* parent, const char* text ) {
    node_program_t* node = ( node_program_t* ) __malloc( sizeof( node_program_t  ) );
    
    // Find last parent
    while( parent->next != NULL )
        parent = parent->next;

//    node->text = __strcpy( text, sizeof( text ) );
    node->child = NULL;
    node->name = NULL;
    node->next = NULL;

    parent->next = node;
    return node;
}

node_program_t* nodeprogram_pushChild( node_program_t* parent, const char* text ) {
    // No need to find parent
    node_program_t* node = ( node_program_t* ) __malloc( sizeof( node_program_t ) );

//    node->text = __strcpy( text, sizeof( text ) );
    node->name = NULL;
    node->next = NULL;

    parent->child = node;
    return node;
}
void nodeprogram_clear( node_program_t* node ) {
    while( node != NULL )
    {
        if( node->child != NULL )
            nodeprogram_clear( node->child );
        __free( node->text );
        if( node->name != NULL ) __free( node->name );
        node_program_t* next = node->next;
        __free( node );
        node = next;
    }
}
void nodeprogram_print( node_program_t* node , int indent) {
    while( node != NULL )
    {
        if( node->name == NULL )
            printf("[Program Node]: " );
        else
            printf("[Program Node] (%s): ", node->name);

        while( indent-- )
            printf(" ");

        printf("%s\n", node->text );
        nodeprogram_print( node->child, indent + 2);
        node = node->next;
    }
}
