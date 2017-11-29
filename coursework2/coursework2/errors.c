//
//  errors.c
//  coursework2
//
//  Created by Olly Seber on 25/11/2017.
//

#include <stdio.h>

#include "errors.h"

/* GLOBALS */

static FILE *g_temp;


/* FUNCTION DEFINITIONS */

void initMemTracking( void ) {
    g_temp = tmpfile();
    if ( g_temp == NULL ) {
        fprintf( stderr, "Error initialising program memory temporary file.\n" );
        exit( BAD_FILE_OPEN );
    }
}


void memAllocated( void *ptr ) {
    fprintf( g_temp, "%p ", ptr );
//    printf( "Wrote pointer %p to temp\n", ptr ); // For debugging purposes
}


void deallocateMem( void ) {
    rewind( g_temp );
    void *ptr;
    
    while ( fscanf( g_temp, "%p", &ptr ) != EOF ) {
//        printf( "Read pointer %p from temp\n", ptr ); // For debugging purposes
        free( ptr );
    }
    
    fclose( g_temp );
}


void programExit( int code, char *info ) {
    deallocateMem();
    if ( code != NO_ERR ) {
        fprintf( stderr, "ERROR: %s\n", info );
    }
    exit( code );
}
