//
//  uifunctions.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef iofunctions_h
#define iofunctions_h

#include <stdlib.h> // For calloc() and free().
#include <stdbool.h> // For booleans.
#include <string.h> // For strlen().

#include "sndfile.h" // For audio file manipulation.

#include "errors.h" // Program errors.


/* DATA TYPES */

typedef struct audioFile_struct audioFile;


/* FUNCTION PROTOTYPES */

/*      isOnlyPositiveInt()
 * Input of a <string>. Each character is evaluated in turn.
 * Returns true if each character is a numerial digit '0' to '9'. */
bool isOnlyPositiveInt( const char *string );


/* For audio file manipulation */

/*      openInputFile()
 * Opens audio file called <filename> in read mode.
 * Returns pointer to the file if successful. */
audioFile* openInputFile( char *filename );


/*      openOutputFile()
 * Opens audio file called <filename> in write mode.
 * <settings> = input file, so that the settigs may be reused.
 * <filterOrder> = the order of the filter to be applied, as output file must contain additional samples.
 * Returns pointer to the file if successful. */
audioFile* openOutputFile( char *filename, audioFile settings, int filterOrder ); // Could change to take in filter


/*      closeAudioFile()
 * Closes audio file.
 * Returns NO_ERR if successful. */
int closeAudioFile( audioFile *file );


/*      getSampleRate()
 * Returns sample rate in Hz of <file>.
 * Returns NULL_FUNC_ARG if file is NULL. */
int getSampleRate( audioFile *file );


/*      getChannelCount()
 * Returns channel count of <file>.
 * Returns NULL_FUNC_ARG if file is NULL. */
int getChannelCount( audioFile *file );


/*      readAudioDouble()
 * Reads up to <sizeOfBuffer> samples into <buffer> from <file>.
 * Returns the number of samples read. */
int readAudioDouble( audioFile *file, double *buffer, int sizeOfBuffer );


/*      writeAudioDouble()
 * Writes <numSamples> samples from <buffer> into <file>.
 * Checks number of samples written.
 * Returns NO_ERR if successful. */
int writeAudioDouble( audioFile *file, double *buffer, int numSamples );


#endif // iofunctions_h
