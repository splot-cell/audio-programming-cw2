//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"

#include <stdbool.h> // For booleans.
#include <limits.h> // For converting integers.

/* GLOBAL INITIALISATIONS */

const int g_minFilterFreq = 20;
const int g_maxFilterFreq = 20000;
const int g_filterOrder = 126;
const int g_maxBufferSize = 2048;
const int g_minBufferSize = 64;


/* TYPE DEFINITIONS */




/* PRIVATE FUNCTION PROTOTYPES */

/*      optionalArgumentHandler()
 * Handles optional arguments from the comand line.
 * <argc> = argc from command line.
 * <argv> = argv from command line.
 * <userOptions> = pointer to userInput struct for storing user input. */
void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions );


void allocFilenameMem( char **filename, unsigned long length );


bool wavFilenameHandler( char **filename, char mode );


int strToInt( char *str, int lowerLimit, int upperLimit, char *label );


/* FUNCTION DEFINITIONS */

userInput* createUserDataStruct( void ) {
    userInput *data = calloc( 1, sizeof( userInput ) );
    if ( data == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for user input." );
    }
    return data;
}


int destroyUserDataStruct( userInput *data ) {
    if ( data == NULL ) {
        return NULL_FUNC_ARG;
    }
    free( data->inputFilename );
    free( data->outputFilename );
    free( data );
    return NO_ERR;
}


void printHelp( void ) {
    
    exit( NO_ERR );
}


void commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    optionalArgumentHandler( argc, argv, userOptions );
    
    int providedArg = argc - optind;
    if ( providedArg != 3 ) {
        fatalError( BAD_COMMAND_LINE, "Could not continue, incorrect number of arguments detected." );
    }
    
    /* Allocate memory for filnames, adding 6 additional characters for '.wav\0'. */
    allocFilenameMem( &userOptions->inputFilename, strlen( argv[ argc - 3 ] ) + 6 );
    allocFilenameMem( &userOptions->outputFilename, strlen( argv[ argc - 3 ] ) + 6 );
    
    strcpy( userOptions->inputFilename, argv[ argc - 3] );
    strcpy( userOptions->outputFilename, argv[ argc - 2] );
    
    if ( wavFilenameHandler( &userOptions->inputFilename, 'i' ) == false ||
            wavFilenameHandler( &userOptions->outputFilename, 'o' ) == false ) {
        cleanupMemory( userOptions, NULL, NULL, NULL );
        exit( NO_ERR );
    }
    
    /* Check characters and range of requested frequency */
    if ( isOnlyPositiveInt( argv[ argc - 1 ] ) == false ) {
        fatalError( BAD_COMMAND_LINE, "Non-integer character detected in cut-off frequency" );
    }
    userOptions->filterFrequncy = strToInt( argv[ argc - 1],
                                           g_minFilterFreq, g_maxFilterFreq, "filter frequency" );
}


void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    struct option optionalArgs[] = {
        { "window", required_argument, 0, 'w' },
        { "highpass", no_argument, 0, 'h' },
        { "buffersize", required_argument, 0, 'b' },
        { 0, 0, 0, 0 }
    };
    int optionIndex = 0;
    char option;
    
    while ( ( option = getopt_long( argc, argv, "w:hb:", optionalArgs, &optionIndex ) ) != -1 ) {
        switch ( option ) {
            case 'w':
                if ( strcmp( optarg, "rect" ) == 0 ) {
                    userOptions->windowing = WINDOW_RECTANGULAR;
                }
                else if ( strcmp( optarg, "bart" ) == 0 ) {
                    userOptions->windowing = WINDOW_BARTLETT;
                }
                else if ( strcmp( optarg, "hann" ) == 0 ) {
                    userOptions->windowing = WINDOW_HANNING;
                }
                else if ( strcmp( optarg, "hamm" ) == 0 ) {
                    userOptions->windowing = WINDOW_HAMMING;
                }
                else if ( strcmp( optarg, "black" ) == 0 ) {
                    userOptions->windowing = WINDOW_BLACKMAN;
                }
                else {
                    fprintf( stderr,
                            "Invalid option '%s' for windowing.\nWill try to continue using default: bartlett.\n",
                            optarg );
                }
                break;
            case 'h':
                userOptions->filterType = TYPE_HIGHPASS;
                break;
            case 'b':
                if ( isOnlyPositiveInt( optarg ) == false ) {
                    fatalError( BAD_COMMAND_LINE, "Buffer size must be a positive integer." );
                }
                userOptions->bufferSize = strToInt( optarg, g_minBufferSize, g_maxBufferSize, "buffer size" );
                if ( ( userOptions->bufferSize % g_minBufferSize ) != 0 ) {
                    fatalError( BAD_COMMAND_LINE, "Buffer size must be a power of 2." );
                }
                break;
            case '?':
                if ( optopt == 'w' ) {
                    fprintf( stderr, "Option -w requires an argument. Using default: bartlett.\n" );
                }
                else if ( isprint( optopt ) ) {
                    char error[100];
                    sprintf( error, "Unknown option '-%c'.", optopt );
                    fatalError( BAD_COMMAND_LINE, error );
                }
                else {
                    char error[100];
                    sprintf( error, "Unknown option character '\\x%x'.\n", optopt );
                    fatalError( BAD_COMMAND_LINE, error );
                }
        }
    }
}

void openFiles( userInput *userData, audioFile **inputFile, audioFile **outputFile ) {
    *inputFile = openInputFile( userData->inputFilename );
    *outputFile = openInputFile( userData->outputFilename );
}


bool wavFilenameHandler( char **filename, char mode ) {
    char str[ 8 ];
    strcpy( str, ( mode == 'i' ? "Input" : "Output" ) );
    
    if ( isWavFilename( *filename ) == false ) {
        printf( "%s filename '%s' not recognised as '*.wav'. Would you like to append '.wav'? (y/n)\n",
               str, *filename );
        if ( getYesNo() ) {
            strcat( *filename, ".wav" );
        }
        else {
            return false;
        }
    }
    return true;
}

        
void allocFilenameMem( char **filename, unsigned long length ) {
    *filename = calloc( length, sizeof( char ) );
    if ( filename == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for filename." );
    }
}


int strToInt( char *str, int lowerLimit, int upperLimit, char *label ) {
    long int x = strtol( str, NULL, 10 ); // More robust conversion than atoi.
    if ( x > upperLimit || x < lowerLimit ) {
        char error[ 100 ];
        sprintf( error, "The number %s is out of range for %s.", str, label );
        fatalError( OUT_OF_BOUNDS_VALUE, error );
    }
    return (int) x;
}
        

void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    destroyUserDataStruct( userOptions ); // No need to check if NULL as cleaning nothing is fine
    closeAudioFile( inputFile );
    closeAudioFile( outputFile );
    destroyFilter( filter );
}
