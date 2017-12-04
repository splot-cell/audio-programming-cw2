//
//  errors.c
//  coursework2
//
//  Created by Olly Seber on 25/11/2017.
//
// Deals with all the errors.

#include "errors.h"

#include <stdlib.h> // For exit() and free().
#include <stdio.h> // For fprintf() and printf().
#include <stdbool.h>

/* DYNAMIC MEMORY TRACKING STRUCT */

typedef struct node_struct {
    void *data;
    struct node_struct *next;
} node;
    

/* GLOBALS */

static node *g_head = NULL;


/* DYNAMIC MEMORY TRACKING - LINKED LIST FUNCTIONS */

int push( void *ptr ) {
    node *new;
    new = malloc( sizeof( node ) );
    if ( new == NULL ) {
        return 1;
    }
    new->data = ptr;
    new->next = g_head;
    
    g_head = new;
    
    return 0;
}


void* pop( void ) {
    void *retVal = NULL;
    node *next = NULL;
    
    if ( g_head == NULL ) {
        return NULL;
    }
    
    next = g_head->next;
    retVal = g_head->data;
    free( g_head );
    g_head = next;
    
    return retVal;
}


/* FUNCTION DEFINITIONS */

void memAllocated( void *ptr ) {
    
    if ( push( ptr ) != 0 ) {
        programExit( BAD_MEMORY, "Could not allocate memory for program memory tracking!" );
    }
    
//    printf( "Wrote pointer %p to list\n", ptr ); // For debugging purposes
}


void deallocateMem( void ) {
    void *temp;
    while ( ( temp = pop() ) != NULL ) {
        free( temp );
//        printf( "Freed pointer %p from list\n", temp ); // Debugging
    }
}


void programExit( int code, char *info ) {
    deallocateMem();
    if ( code != NO_ERR ) {
        /* Print code as well as message as terminal doesn't show it by default. */
        fprintf( stderr, "ERROR: code %d\n%s\n", code, info );
    }
    exit( code );
}
