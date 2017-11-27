//
//  prog_header.h
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#ifndef prog_header_h
#define prog_header_h

#include <stdlib.h> // For calloc() and free().
#include <stdio.h> // For printf(), fprintf(), sprintf().
#include <string.h> // For strcomp().
#include <unistd.h> // For getopt().
#include <ctype.h> // For isprint().

#include "iofunctions.h" // Deals with audio files and non-progam-specific i/o functions.
#include "firfilter.h" // Deals with filtering.
#include "errors.h" // Error codes are here.


/* DATA TYPES */

typedef struct userInput_struct userInput; // For storing all user input.


/* FUNCTION PROTOTYPES */

/*      printHelp()
 * Prints help text to stdout and exits program. */
void printHelp( void );


/*      createUserDataStruct
 * Allocates memory required for struct to hold user data. Returns poiter to memory. */
userInput* createUserDataStruct( void );


/*      destroyUserDataStruct
 * Frees memory stored at address <data>.
 * Returns NO_ERR if successful.
 * Returns NULL_FUNC_ARG if <data> is NULL. */
int destroyUserDataStruct( userInput *data );


/*      commandLineArgumentHandler()
 * Handles user arguements. Sanitises and stores.
 * <argc> = argc command line argument count.
 * <argv> = argv command line argument array.
 * <userOptions> = pointer to userInput variable where the options will be stored.
 *  */
int commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions );


int initialiseVar( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter );


/*      cleanupMemory()
 * Helper function for freeing dynamicly allocated memory at end of program. */
void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter );


#endif // prog_header_h
