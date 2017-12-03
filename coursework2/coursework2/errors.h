//
//  errors.h
//  coursework2
//
//  Created by Olly Seber on 25/11/2017.
//

#ifndef errors_h
#define errors_h


/* DATA TYPES */

enum ERR {
    NO_ERR,
    BAD_MEMORY,
    BAD_COMMAND_LINE,
    OUT_OF_BOUNDS_VALUE,
    BAD_FILE_OPEN,
    BAD_BUFFER_PROCESS,
    BAD_FILE_WRITE,
    NULL_FUNC_ARG
};


/* FUNCTION PROTOTYPES */

/*      memAllocated()
 * To be called each time program memory is allocated dynamically. Adds <ptr> to linked list. */
void memAllocated( void *ptr );


/*      programExit()
 * Helper function for deallocating memory, printing error code, and exiting program. */
void programExit( int code, char *info );


#endif // errors_h
