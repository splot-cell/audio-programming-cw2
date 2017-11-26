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


/* FUNCTION DEFINITIONS */

void printHelp( void ) {
    
    exit( NO_ERR );
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


void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    closeFile( inputFile );
    closeFile( outputFile );
    destroyFilter( filter );
}
