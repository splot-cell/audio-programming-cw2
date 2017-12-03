//
//  uifunctions.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef iofunctions_h
#define iofunctions_h

#include <stdbool.h> // For booleans.


/* DATA TYPES */

typedef struct audioFile_struct audioFile;


/* FUNCTION PROTOTYPES */

/*      isOnlyPositiveInt()
 * Input of a <string>. Each character is evaluated in turn.
 * Returns true if each character is a numerial digit '0' to '9', false otherwise. */
bool isOnlyPositiveInt( const char *string );


/*      isWavFilename()
 * Checks last four characters of <string> are '.wav'.
 * Returns true if so, or false otherwise. */
bool isWavFilename( const char *string );


/*      strToInt()
 * A more robust way of casting a string to an integer.
 * <str> = string to be converted, must contain valid integer in string format.
 * <output> = pointer to int destination.
 * <lowerLimit> = lowest acceptable value (should be INT_MIN or higher).
 * <upperLimit> = highest acceptable value (should be INT_MAX or higher).
 * Returns zero if int is within the limits specified.
 * Returns 1 if int is outside of limits specified. */
int strToInt( char *str, int *output, int lowerLimit, int upperLimit );


/*      getYesNo()
 * Gets a character from stdin.
 *  - If 'y' or 'Y' returns true.
 *  - If anything else returns false. */
bool getYesNo( void );


/*      printWithBorder()
 * Prints array of strings <message> as lines of text with a border of asterisks.
 * <rows> = total number of strings in array, i.e. number of lines of text to print.
 * <borderWidth> defines the size of the border. */
void printWithBorder( char *message[], int rows, int borderWidth );


/* For audio file manipulation */

/*      allocateAudioFileMem()
 * Returns pointer to dynamically allocated memory for storing audio file.
 * Returns NULL if unsuccessful. */
audioFile* allocateAudioFileMem( void );

/*      freeAudioFileMem()
 * Frees memory allocated for <file>.
 * Returns 0 if successful.
 * Returns -1 if file is NULL. */
int freeAudioFileMem( audioFile *file );


/*      openInputFile()
 * Opens audio file called <filename> in read mode.
 * Returns 0 if successful. 
 * Returns 1 if file cannot be opened. */
int openInputFile( audioFile *file, char *filename );


/*      openOutputFile()
 * Opens audio file called <filename> in write mode.
 * <settings> = input file, so that the settigs may be reused.
 * Returns 0 if successful.
 * Returns 1 if file cannot be opened. */
int openOutputFile( audioFile *file, char *filename, audioFile *settings );


/*      closeAudioFile()
 * Closes audio file.
 * Returns 0 if successful.
 * Returns -1 if <file> is NULL. */
int closeAudioFile( audioFile *file );


/*      getSampleRate()
 * Returns sample rate in Hz of <file>.
 * Returns -1 if file is NULL. */
int getSampleRate( audioFile *file );


/*      getChannelCount()
 * Returns channel count of <file>.
 * Returns -1 if file is NULL. */
int getChannelCount( audioFile *file );


/*      readAudioDouble()
 * Reads up to <sizeOfBuffer> samples into <buffer> from <file>.
 * Returns -1 if file or buffer is NULL.
 * Returns the number of samples read. */
int readAudioDouble( audioFile *file, double *buffer, int sizeOfBuffer );


/*      writeAudioDouble()
 * Writes <numSamples> samples from <buffer> into <file>.
 * Returns -1 if file or buffer is NULL.
 * Checks number of samples written is equal to <numSamples>.
 * Returns -2 if not equal.
 * Returns number of samples written if successful. */
int writeAudioDouble( audioFile *file, double *buffer, int numSamples );


#endif // iofunctions_h
