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


void allocFilenameMem( char **filename, unsigned long length );


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


int commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
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
    
    if ( isWavFilename( userOptions->inputFilename ) == false ) {
        printf( "Input filename '%s' not recognised as '*.wav'. Would you like to append '.wav'? (y/n)\n",
               userOptions->inputFilename );
        if ( getYesNo() ) {
            strcat( userOptions->inputFilename, ".wav" );
        }
        else {
            cleanupMemory( userOptions, NULL, NULL, NULL );
            exit( NO_ERR );
        }
    }
    if ( isWavFilename( userOptions->outputFilename ) == false ) {
        printf( "Output filename '%s' not recognised as '*.wav'. Would you like to append '.wav'? (y/n)\n",
               userOptions->outputFilename );
        if ( getYesNo() ) {
            strcat( userOptions->outputFilename, ".wav" );
        }
        else {
            cleanupMemory( userOptions, NULL, NULL, NULL );
            exit( NO_ERR );
        }
    }
    
    printf( "%s\n", userOptions->inputFilename );
    
    return NO_ERR;
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
    
    while ( ( option = getopt_long( argc, argv, "w:hb", optionalArgs, &optionIndex ) ) != -1 ) {
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

        
void allocFilenameMem( char **filename, unsigned long length ) {
    *filename = calloc( length, sizeof( char ) );
    if ( filename == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for filename." );
    }
}
        

void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    destroyUserDataStruct( userOptions ); // No need to check if NULL as cleaning nothing is fine
    closeAudioFile( inputFile );
    closeAudioFile( outputFile );
    destroyFilter( filter );
}
