//
//  uifunctions.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef uifunctions_h
#define uifunctions_h

#include <stdbool.h> // For booleans.
#include <string.h> // For strlen()

#include "firfilter.h"
#include "filehandler.h"

typedef struct userInputStrings_struct userInputStrings;

/* isOnlyPositiveInt()
 * Input of a <string>. Each character is evaluated in turn.
 * Returns true if each character is a numerial digit '0' to '9'. */
bool isOnlyPositiveInt( const char *string );


/* commandLineArgumentHandler()
 * Checks the number of arguments supplied.
 *  - One argument = print help and exit program.
 *  - Four arguments = return 0.
 *  - Else = invalid, return -1. */
int commandLineArgumentHandler( int argc, char *argv[] );


int interpretUserInput( int argc, char *argv[], audioFile *inputFile, audioFile *outputFile, firFilter *filter );

#endif // uifunctions_h
