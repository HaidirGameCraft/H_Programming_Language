#include "Label.hpp"
#include "hbyte/filereader.h"
#include "tools/memory.h"
#include <assert.h>
#include <string.h>

static vector<label_t*> labels;
static label_t* current_labels = nullptr;

void Label_add( string label ) {
    bool isChild = label[0] == '.';

    if( !isChild )
    {
        label_t* __lbl = (label_t*) __malloc(sizeof( label_t ) );
        __lbl->name = label;
        __lbl->address = 0;
        labels.push_back( __lbl );
        current_labels = __lbl;
    }
    else {
        assert( isChild && current_labels != nullptr );
        label_t child_lbl;
        child_lbl.name = &label.c_str()[1];
        child_lbl.address = 0;
        current_labels->child.push_back(child_lbl);
    }
}

void Label_updateAddress( string label, uint32_t address ) {
   if( label[0] == '.' )
   {
        // when is has token
        assert( current_labels != nullptr );
        for(int i = 0; i < current_labels->child.size(); i++ )
        {
            label_t* lbl = &current_labels->child[i];
            if( strcmp(lbl->name.c_str(), &label.c_str()[1] ) == 0 )
            {
                lbl->address = address;
                break;
            }
        }
   } else {
        for(int i = 0; i < labels.size(); i++ )
        {
            label_t* lbl = labels[i];
            if( lbl->name == label )
            {
                lbl->address = address;
                current_labels = lbl;
                break;
            }
        }
    
   }
}

void Label_push( string label, uint32_t dest ) {
    vector<string> __labels;
    string __tmp;
    bool hasChild = false;
    for(int i = 1; i < label.size(); i++)
    {
        if( label[i] == '.' )
        {
            if( __tmp.size() > 0 )
            {
                __labels.push_back( __tmp );
                __tmp.clear();
            }

            hasChild = true;
        } else {
            __tmp += label[i];
        }
    }

    if( __tmp.size() > 0 )
    {
        __labels.push_back( __tmp );
    }


    if( __labels.size() == 2 ) {
        label_t* lbl = nullptr;
        for(int i = 0; i < labels.size(); i++)
        {
            lbl = labels[i];
            if( lbl->name == __labels[0] )
                break;
            lbl = nullptr;
        }
        assert( lbl != nullptr );
        
        label_t* child_lbl = nullptr;
        for(int i = 0; i < lbl->child.size(); i++)
        {
            child_lbl = &lbl->child[i];
            if( child_lbl->name == __labels[1] )
                break;
            child_lbl = nullptr;
        }
        assert( child_lbl != nullptr );
        child_lbl->target.push_back(dest);
    }
    else if ( __labels.size() == 1 )
    {
        
        if( hasChild )
        {
            label_t* child_lbl = nullptr;
            for(int i = 0; i < current_labels->child.size(); i++)
            {
                child_lbl = &current_labels->child[i];
                if( child_lbl->name == __labels[0] )
                    break;
                child_lbl = nullptr;
            }
            assert( child_lbl != nullptr );
            child_lbl->target.push_back( dest );
        } else {
            label_t* lbl = nullptr;
            for(int i = 0; i < labels.size(); i++)
            {
                lbl = labels[i];
                if( lbl->name == __labels[0] )
                    break;
                lbl = nullptr;
            }
            assert(lbl != nullptr);
            lbl->target.push_back(dest);
        }
    }
    
}

void Label_update() {
    for(label_t* lbl : labels)
    {
        for(label_t& child_lbl : lbl->child )
            for(uint32_t& dest : child_lbl.target )
                hbyte_write32(dest, child_lbl.address); 
        for(uint32_t& dest : lbl->target )
            hbyte_write32(dest, lbl->address); 
        __free( lbl );
    }
}









