//
//  iofunctions.c
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#include "iofunctions.h"


/* TYPE DEFINITIONS */

typedef struct userInput_struct {
    char *inputFilename;
    char *outputFilename;
    filterType filterType;
    firWindow windowing;
} userInput;


typedef struct audioFile_struct {
    SNDFILE *audioFile;
    SF_INFO infoFile;
} audioFile;


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


int commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    char option;
    while ( ( option = getopt( argc, argv, "w:h" ) ) != -1 ) {
        switch ( option ) {
            case 'w':
                if ( strcmp( optarg, "rect" ) ) {
                    userOptions->windowing = WINDOW_RECTANGULAR;
                }
                else if ( strcmp( optarg, "bart" ) ) {
                    userOptions->windowing = WINDOW_BARTLETT;
                }
                else if ( strcmp( optarg, "hann" ) ) {
                    userOptions->windowing = WINDOW_HANNING;
                }
                else if ( strcmp( optarg, "hamm" ) ) {
                    userOptions->windowing = WINDOW_HAMMING;
                }
                else if ( strcmp( optarg, "black" ) ) {
                    userOptions->windowing = WINDOW_BLACKMAN;
                }
                else {
                    fprintf( stderr, "Invalid option for windowing. Using default: bartlett.\n" );
                }
                break;
        }
    }
}


/* Audio IO */

audioFile* openInputFile( char *filename ) {
    audioFile *file = calloc( 1, sizeof( audioFile ) );
    if ( file == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for input file." );
    }
    file->audioFile = sf_open( filename, SFM_READ, &file->infoFile );
    
    return file;
}


audioFile* openOutputFile( char *filename, audioFile settings, int filterOrder ) {
    audioFile *file = calloc( 1, sizeof( audioFile ) );
    if (file == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for output file." );
    }
    file->infoFile = settings.infoFile; // Copy settings from input file
    file->infoFile.frames += filterOrder; // Output file must have additional frames
    
    file->audioFile = sf_open( filename, SFM_WRITE, &file->infoFile );
    
    return file;
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
    return file->infoFile.samplerate;
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
        fatalError( BAD_FILE_WRITE, "Could not write buffer to output file." );
    }
    return written;
}

