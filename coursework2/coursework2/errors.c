//
//  errors.c
//  coursework2
//
//  Created by Olly Seber on 25/11/2017.
//

#include "errors.h"


/* FUNCTION DEFINITIONS */

void fatalError( int code, char *info ) {
    fprintf( stderr, "ERROR: %s\n", info );
    exit( code );
}
