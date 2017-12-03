//
//  iofunctions.c
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//
// Contains re-usable i/o functions.

#include "iofunctions.h"

#include <stdlib.h> // For calloc() and free().
#include <string.h> // For strlen().

#include "sndfile.h" // For audio file manipulation.


/* TYPE DEFINITIONS */

typedef struct audioFile_struct {
    SNDFILE *audioFile;
    SF_INFO infoFile;
} audioFile;


/* PRIVATE FUNCTION PROTOTYPES */

/*      flushStdIn()
 * Used to remove any unwanted characters from stdin buffer after getting a char. */
void flushStdIn( void );


/* FUNCTION DEFINITIONS */

bool isOnlyPositiveInt( const char *string ) {
    if ( !string ) {
        return false;
    }
    bool retValue = true;
    
    for ( int index = 0; index < ( strlen( string ) / sizeof( char ) ); ++index ) {
        /* If character is outside numerical digit range AND
         * the expression has not already been evaulated to false */
        if ( ( string[ index ] < 48 || 57 < string[ index ] ) && retValue ) {
            retValue = false; // Then there are some 'shady characters' in <string>.
        }
    }
    return retValue;
}


bool isWavFilename( const char *string ) {
    unsigned long length = strlen( string );
    if ( length < 5 ) {
        return false; // Do not accept just ".wav"
    }
    if ( string[ length - 4 ] == '.' && string[ length - 3 ] == 'w' &&
        string[ length - 2 ] == 'a' && string[ length - 1 ] == 'v' ) {
        return true;
    }
    return false;
}


int strToInt( char *str, int *output, int lowerLimit, int upperLimit ) {
    long int x = strtol( str, NULL, 10 ); // More robust conversion than atoi.
    if ( x > upperLimit || x < lowerLimit ) {
        return 1;
    }
    *output = (int) x;
    return 0;
}


bool getYesNo( void ) {
    char c;
    scanf( " %c", &c );
    flushStdIn();
    if ( c == 'y' || c == 'Y' ) {
        return true;
    }
    return false;
}


void flushStdIn( void ) {
    int c;
    while ( ( c = getchar() ) != '\n' && c != EOF ); // Get characters until newline is hit.
}


void printWithBorder( char *message[], int rows, int borderWidth ) {
    int fmtChr = 1; // Number of characters at beginning of row used for formatting.
    
    /* Set up border parameters. */
    int pad = 1, numColumns = 80,
    numRows = rows  + ( 2 * ( pad + borderWidth ) );
    
    for ( int r = 0; r < numRows; ++r ) { // Cycle through each row.
        for ( int c = 0; c < numColumns; ++c ) { // Cycle through each character in the row.
            
            /* If we're in the border rows or characters. */
            if ( r < borderWidth || r >= numRows - borderWidth ||
                c < borderWidth || c >= numColumns - borderWidth ) {
                printf( "%c", '*' );
            }
            
            /* If we're wthin the padding rows or characters. */
            else if ( r < pad + borderWidth || r >= numRows - borderWidth - pad ||
                     c < pad + borderWidth || c >= numColumns - borderWidth - pad ) {
                printf( "%c", ' ' );
            }
            
            
            else { // We must now be in the rows and character 'columns' with potential text.
                
                /* Set up whitespace for justification. */
                int centreOffset = 0;
                
                /* If the first character of the row is 'c' set offset to half the available space */
                if ( message[ r - borderWidth - pad ][ 0 ] == 'c' ) {
                    centreOffset = (int) ( ( numColumns -
                                            strlen( message[ r - borderWidth - pad ] ) + fmtChr ) / 2 ) - pad - borderWidth;

                }
                
                /* Fill in whitespace before printing message */
                if ( c - borderWidth - pad < centreOffset ) {
                    printf( "%c", ' ' );
                }
                
                /* Printing message */
                else if ( strlen( message[ r - borderWidth - pad ] ) - fmtChr >
                         c - centreOffset - borderWidth - pad ) {
                    printf( "%c",
                           message[ r-borderWidth-pad ][ c - centreOffset - borderWidth - pad + fmtChr ] );
                }
                
                /* Finishing whitespace */
                else {
                    printf( "%c", ' ' );
                }
            }
        }
        printf( "%c", '\n' );
    }
    return;
}


/* For audio IO */


audioFile* allocateAudioFileMem( void ) {
    audioFile *file = calloc( 1, sizeof( audioFile ) );
    if ( file == NULL ) {
        return NULL;
    }

    return file;
}


int freeAudioFileMem( audioFile *file ) {
    if ( file == NULL ) {
        return -1;
    }
    free( file );
    
    return 0;
}


int openInputFile( audioFile *file, char *filename ) {
    
    file->infoFile.format = 0; // Requested by libsnd documentation.
    
    file->audioFile = sf_open( filename, SFM_READ, &file->infoFile );
    if ( file->audioFile == NULL ) {
        return 1;
    }
    
    return 0;
}


int openOutputFile( audioFile *file, char *filename, audioFile *settings ) {
    
    file->infoFile = settings->infoFile; // Copy settings from input file
    
    file->audioFile = sf_open( filename, SFM_WRITE, &file->infoFile );
    if ( file->audioFile == NULL ) {
        return 1;
    }
    
    /* Set hard clipping for output file. */
    sf_command ( file->audioFile, SFC_SET_CLIPPING, NULL, SF_TRUE );
   
    return 0;
}


int closeAudioFile( audioFile *file ) {
    if ( file == NULL ) {
        return -1;
    }
    sf_close( file->audioFile );
    return 0;
}


int getSampleRate( audioFile *file ) {
    if ( file == NULL ) {
        return -1;
    }
    return file->infoFile.samplerate;
}


int getChannelCount( audioFile *file ) {
    if ( file == NULL ) {
        return -1;
    }
    return file->infoFile.channels;
}


int readAudioDouble( audioFile *file, double *buffer, int sizeOfBuffer ) {
    if ( file == NULL || buffer == NULL ) {
        return -1;
    }
    return (int) sf_read_double( file->audioFile, buffer, sizeOfBuffer );
}


int writeAudioDouble( audioFile *file, double *buffer, int numSamples ) {
    if ( file == NULL || buffer == NULL ) {
        return -1;
    }
    int written = (int) sf_write_double( file->audioFile, buffer, numSamples );
    if ( written != numSamples ) {
        return -2;
    }
    return written;
}

