//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"


/* FUNCTION DEFINITIONS */

void fatalError( int code, char *info ) {
    printf( "ERROR: %s\n", info );
    exit( code );
}


void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    closeFile( inputFile );
    closeFile( outputFile );
    destroyFilter( filter );
}
