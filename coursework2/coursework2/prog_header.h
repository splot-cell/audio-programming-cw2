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
#include <stdio.h> // For printf() and fprintf().
#include <string.h> // For strcomp().
#include <getopt.h> // For getopt().

#include "iofunctions.h" // Deals with audio files and non-progam-specific i/o functions.
#include "firfilter.h" // Deals with filtering.
#include "errors.h" // Error codes are here.


/* DATA TYPES */

typedef struct userInput_struct userInput; // For storing all user input.


/* FUNCTION PROTOTYPES */

/*      printHelp()
 * Prints help text to stdout and exits program. */
void printHelp( void );

/*      commandLineArgumentHandler()
 * Handles optioinal user arguments and required user arguements.
 * <argc> = argc command line argument count.
 * <argv> = argv command line argument array.
 * <userOptions> = pointer to userInput variable where the options will be stored.
 *  */
int commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions );


int interpretUserInput( int argc, char *argv[], audioFile *inputFile, audioFile *outputFile, firFilter *filter );


/*      cleanupMemory()
 * Helper function for freeing dynamicly allocated memory at end of program. */
void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter );


#endif // prog_header_h
