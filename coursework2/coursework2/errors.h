//
//  errors.h
//  coursework2
//
//  Created by Olly Seber on 25/11/2017.
//

#ifndef errors_h
#define errors_h

#include <stdio.h>


/* DATA TYPES */

enum ERR {
    NO_ERR,
    BAD_MEMORY,
    BAD_COMMAND_LINE,
    BAD_RUNTIME_ARG,
    OUT_OF_BOUNDS_VALUE
};


/* FUNCTION PROTOTYPES */

/*      fatalError()
 * Helper function for printing error code and exiting program. */
void fatalError( int code, char *info );


#endif // errors_h
