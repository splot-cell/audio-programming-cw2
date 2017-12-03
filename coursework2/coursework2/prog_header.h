//
//  prog_header.h
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#ifndef prog_header_h
#define prog_header_h

#include "iofunctions.h" // Deals with audio files and non-progam-specific i/o functions.
#include "firfilter.h" // Deals with filtering.
#include "errors.h" // Error codes are here.

/* GLOBALS */
extern const int g_minFilterFreq;
extern const int g_maxFilterFreq;
extern const int g_filterOrder;
extern const int g_maxBufferSize;
extern const int g_minBufferSize;
extern const int g_defaultBufferSize;


/* DATA TYPES */

/* For storing all user input. No need to be opaque. */
typedef struct userInput_struct {
    char *inputFilename;
    char *outputFilename;
    int filterFrequncy;
    filterType filterType;
    firWindow windowing;
    int bufferSize;
} userInput;


/* FUNCTION PROTOTYPES */

/*      printHelp()
 * Prints help text to stdout and exits program. */
void printHelp( void );


/*      createUserDataStruct
 * Allocates memory required for struct to hold user data. Returns poiter to memory.
 * Allocating dynamically makes passing the data around easier despite the non-opaque
 * nature of the struct. */
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
 * <userOptions> = pointer to userInput variable where the options will be stored. */
void commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions );


void openFiles( userInput *userData, audioFile **inputFile, audioFile **outputFile );


void initFileTracking( void );


void errorHandler( int code, char *info );


/*      cleanupMemory()
 * Helper function for freeing dynamicly allocated memory at end of program. */
void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter );


#endif // prog_header_h
