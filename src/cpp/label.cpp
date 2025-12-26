#include "label.hpp"
#include "ins_make.hpp"
#include <inst.h>
#include <stdlib.h>
#include <tools/tools.h>
#include <asrt.h>

label_name_t* label_lists = NULL;
label_name_t* parent = NULL;
int length_label_lists = 0;
int parent_index_label_track = 0;
void    push_label_name( const char* label_name ) {
#ifdef __DEBUG__
    printf("Pushing New Label: %s\n", label_name );
#endif
    // printf("Test 1...\n");
    if( label_name[0] == '.' )
    {
        if( parent == NULL ) 
        {
            __free( label_lists );
            ASSERT("Labeling Error: Parent is not define");
        }

        if( parent->children == NULL )
            parent->children = (label_name_t*) __malloc( sizeof( label_name_t ) * ( parent->length + 1 ) );
        else
            parent->children = (label_name_t*) __realloc( parent->children, sizeof( label_name_t ) * ( parent->length + 1) );

        char* __label_name = (char*) strcpy( &label_name[1], strlen( label_name ) - 2 );
        label_name_t* label = &parent->children[ parent->length ];
        memset( label, 0, sizeof( label_name_t ) );

        label->label_name = __label_name;
        parent->length++;
        return;
    }

    if( label_lists == NULL )
        label_lists = ( label_name_t* ) __malloc( sizeof( label_name_t ) * ( length_label_lists + 1 ) );
    else
        label_lists = ( label_name_t* ) __realloc( label_lists, sizeof( label_name_t ) * ( length_label_lists + 1) );

    if( parent != NULL )
        parent = &label_lists[ parent_index_label_track ];

    char* __label_name = (char*) strcpy( label_name, strlen( label_name ) - 1 );
    label_name_t* label = &label_lists[ length_label_lists ];
    memset( label, 0, sizeof( label_name_t ) );
    
    label->label_name = __label_name;
    length_label_lists++;
    if( __label_name[0] != '.' )
    {
        parent = label;
        parent_index_label_track = length_label_lists;
    }
}

void    push_next_label_name( const char* label_name, uint32_t destination ) {
    // Three Type Case:
    // 1. $main
    // 2. $main.test1
    // 3. $.test1

    int length = strlen( label_name );
    int index = 0;
    int start_char = 0;
    int end_char = 0;
    label_name_t* __parent = NULL;

    // length including EOF items
    while( index <= length )
    {
        if( label_name[ end_char ] == '.' || label_name[ end_char ] == 0 )
        {
            int __length = end_char - start_char; // the length from [start, end] char including '.' or 0
            char* __label_name = (char*) strcpy( &label_name[ start_char ], __length);
#ifdef __DEBUG__ 
            printf("Target Label: %s %i\n", __label_name, __length ); 
#endif
            // First Case
            if( __length != 0 && ( label_name[ end_char ] == '.' || label_name[ end_char ] == 0 ) && __parent == NULL )
            {
                label_name_t* label = NULL;
#ifdef __DEBUG__ 
                printf("Label Finder: |%s|\n", __label_name );
#endif 
                for(int i = 0; i < length_label_lists; i++)
                {
                    label = &label_lists[ i ];
                    if( strcmp( label->label_name, __label_name ) != 0 )    // Not Matching
                    {
                        label = NULL;
                        continue;
                    }

                    break;
                }

                if( label == NULL )
                {
                    __free( __label_name );
                    ASSERT("Label Not Found");
                }

                target_label_t* target_label = (target_label_t*) __malloc( sizeof( target_label_t ) );
                memset( target_label, 0, sizeof( target_label_t ) );
                target_label->destination = destination;

                target_label_t* current = label->next;
                target_label_t* prev = NULL;
                while( current != NULL )
                {
                    prev = current;
                    current = current->next;
                }


                if( prev == NULL )
                    label->next = target_label;
                else
                    prev->next = target_label;

                __parent = label;
            } else if ( __parent == NULL && __length == 0 )
            {
                if( parent == NULL )
                {
                    ASSERT("Global Parent Not Define");
                }
#ifdef __DEBUG__ 
                printf("Global Parent: using %s\n", parent->label_name );
#endif
                __parent = parent;
            } else {    // Second Case and Third Case
                label_name_t* child_label = NULL;
                for(int i = 0; i < __parent->length; i++) 
                {
                    child_label = &__parent->children[ i ];
                    if( strcmp( child_label->label_name, __label_name ) != 0 )    // Not Matching
                    {
                        child_label = NULL;
                        continue;
                    }

                    break;
                }

                if ( child_label == NULL )
                {
                    ASSERT("Childen Label Not Found");
                }

                target_label_t* target_label = (target_label_t*) __malloc( sizeof( target_label_t ) );
                memset( target_label, 0, sizeof( target_label_t ) );
                target_label->destination = destination;

                target_label_t* current = child_label->next;
                target_label_t* prev = NULL;
                while( current != NULL )
                {
                    prev = current;
                    current = current->next;
                }


                if( prev == NULL )
                    child_label->next = target_label;
                else
                    prev->next = target_label;
            }



            start_char = end_char + 1;
            __free( __label_name);
        }

        end_char++;
        index++;
    }

}

void    update_address_label_name( const char* label_name, uint32_t address ) {
    if( label_name[0] != '.' )
    {
        for( int i = 0; i < length_label_lists; i++ )
        {
            label_name_t* __l = &label_lists[ i ];
            if( strcmp(__l->label_name, label_name ) == 0 )
            {
                __l->address = address;
                parent = __l;
                break;
            }
        }
    } else {
        if( parent == NULL )
            ASSERT("[Error]: Parent is not define");

        for( int i = 0; i < parent->length; i++ )
        {
            label_name_t* __l = &parent->children[ i ];
            if ( strcmp( __l->label_name, &label_name[1] ) == 0 )
                __l->address = address;
        }
    }
}

void    update_all_label_destination( uint8_t* memory ) {
    for( int i = 0; i < length_label_lists; i++ )
    {
        label_name_t* label = &label_lists[i];

        // children
        for( int j = 0; j < label->length; j++ ){
            label_name_t* child = &label->children[j];
            target_label_t* target = child->next;
            while( target != NULL ) {
                write32( memory, target->destination, child->address );
#ifdef __DEBUG__ 
                printf("%.08x: Update the Value 0x%.08x\n", target->destination, child->address);
#endif
                target = target->next;
            }
        }

        // targets
        target_label_t* target = label->next;
        while( target != NULL ) {
            write32( memory, target->destination, label->address );
#ifdef __DEBUG__ 
            printf("%.08x: Update the Value 0x%.08x\n", target->destination, label->address);
#endif 
            target = target->next;
        }
    }
}

void    clear_all_label_name() {
    for(int i = 0; i < length_label_lists; i++ ) { 
        label_name_t* label = &label_lists[i];

        // children
        for( int j = 0; j < label->length; j++ ){
            label_name_t* child = &label->children[j];
            target_label_t* target = child->next;
#ifdef __DEBUG__ 
            printf("[Delete Target in Child]\n");
#endif 
            while( target != NULL ) {
                target_label_t* tmp = target->next;
                __free( target );
                target = tmp;
            }

#ifdef __DEBUG__
            printf("Deleting %s.%s\n", label->label_name, child->label_name);
#endif 
            __free( child->label_name );
        }
        __free( label->children );

        // targets
        target_label_t* target = label->next;
        while( target != NULL ) { 
            target_label_t* tmp = target->next;
            __free( target );
            target = tmp;
        }

#ifdef __DEBUG__ 
        printf("Deleting %s\n", label->label_name);
#endif 
        __free( label->label_name );
    } 
    __free( label_lists );
    label_lists = NULL;
}

void    list_label() {
#ifdef __DEBUG__ 
    printf("List Unsigned Label: \n");
#endif
    for(int i = 0; i < length_label_lists; i++) {
        label_name_t* label = &label_lists[i];
#ifdef __DEBUG__ 
        printf("%s: 0x%.08x\n", label->label_name, label->address );
#endif 
        for(int j = 0; j < label->length; j++) {
            label_name_t* child = &label->children[j];
#ifdef __DEBUG__ 
            printf("%s.%s: 0x%.08x\n", label->label_name, child->label_name, child->address );
#endif 
        }
    }
}
















