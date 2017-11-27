//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"


/* TYPE DEFINITIONS */

typedef struct userInput_struct {
    char *inputFilename;
    char *outputFilename;
    filterType filterType;
    firWindow windowing;
    int bufferSize;
} userInput;


/* PRIVATE FUNCTION PROTOTYPES */

/*      optionalArgumentHandler()
 * Handles optional arguments from the comand line.
 * <argc> = argc from command line.
 * <argv> = argv from command line.
 * <userOptions> = pointer to userInput struct for storing user input. */
void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions );


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
    free( data );
    return NO_ERR;
}


void printHelp( void ) {
    
    exit( NO_ERR );
}


int commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    optionalArgumentHandler( argc, argv, userOptions );
    
    int providedArg = argc - optind;
    if ( providedArg != 3 ) {
        fatalError( BAD_COMMAND_LINE, "Could not continue, incorrect number of arguments detected." );
    }
    
    //printf( "%d\n", providedArg );
    printf( "%d\n", providedArg );
    
//    for ( int i = argc - 3; i < argc; ++ i ) {
//        printf( "%s\n", argv[ i ] );
//    }
    
    
    return NO_ERR;
}


void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    char option;
    while ( ( option = getopt( argc, argv, "w:h" ) ) != -1 ) {
        switch ( option ) {
            case 'w':
                printf( "%s", optarg );
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
                            "Invalid option for windowing. Will try to continue using default: bartlett.\n" );
                }
                break;
            case 'h':
                userOptions->filterType = TYPE_HIGHPASS;
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


void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    destroyUserDataStruct( userOptions );
    closeAudioFile( inputFile );
    closeAudioFile( outputFile );
    destroyFilter( filter );
}
