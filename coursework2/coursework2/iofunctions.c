//
//  iofunctions.c
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#include "iofunctions.h"


/* TYPE DEFINITIONS */

typedef struct audioFile_struct {
    SNDFILE *audioFile;
    SF_INFO infoFile;
} audioFile;


/* PRIVATE FUNCTION PROTOTYPES */

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
        return false;
    }
    if ( string[ length - 4 ] == '.' && string[ length - 3 ] == 'w' &&
        string[ length - 2 ] == 'a' && string[ length - 1 ] == 'v' ) {
        return true;
    }
    return false;
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
    while ( ( c = getchar() ) != '\n' && c != EOF );
}


/* For audio IO */


audioFile* allocateAudioFileMem( void ) {
    audioFile *file = calloc( 1, sizeof( audioFile ) );
    if ( file == NULL ) {
        return NULL;
    }
    memAllocated( file );
    
    return file;
}


int openInputFile( audioFile *file, char *filename ) {
    
    file->infoFile.format = 0;
    
    file->audioFile = sf_open( filename, SFM_READ, &file->infoFile );
    if ( file->audioFile == NULL ) {
        return BAD_FILE_OPEN;
    }
    
    //sf_command ( file->audioFile, SFC_SET_CLIPPING, NULL, SF_TRUE );
    
    return NO_ERR;
}


int openOutputFile( audioFile *file, char *filename, audioFile *settings ) {
    
    file->infoFile = settings->infoFile; // Copy settings from input file
    
    file->audioFile = sf_open( filename, SFM_WRITE, &file->infoFile );
    if ( file->audioFile == NULL ) {
        return BAD_FILE_OPEN;
    }
    
    sf_command ( file->audioFile, SFC_SET_CLIPPING, NULL, SF_TRUE );
   
    return NO_ERR;
}


int closeAudioFile( audioFile *file ) {
    if ( file == NULL ) {
        return NULL_FUNC_ARG;
    }
    sf_close( file->audioFile );
    return NO_ERR;
}


int getSampleRate( audioFile *file ) {
    if ( file == NULL ) {
        return NULL_FUNC_ARG;
    }
    return file->infoFile.samplerate;
}


int getChannelCount( audioFile *file ) {
    if ( file == NULL ) {
        return NULL_FUNC_ARG;
    }
    return file->infoFile.channels;
}


int readAudioDouble( audioFile *file, double *buffer, int sizeOfBuffer ) {
    if ( file == NULL || buffer == NULL ) {
        return NULL_FUNC_ARG;
    }
    return (int) sf_read_double( file->audioFile, buffer, sizeOfBuffer );
}


int writeAudioDouble( audioFile *file, double *buffer, int numSamples ) {
    if ( file == NULL || buffer == NULL ) {
        return NULL_FUNC_ARG;
    }
    int written = (int) sf_write_double( file->audioFile, buffer, numSamples );
    if ( written != numSamples ) {
        return BAD_FILE_WRITE;
    }
    return written;
}

