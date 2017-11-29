//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "prog_header.h"


int main( int argc, char * argv[] ) {
    
    /* Initialise memory and file tracking. */
    initMemTracking();
    if ( initFiltErrHandling() != FILT_NO_ERR ) {
        programExit( BAD_FILE_OPEN, "Error initialising filter memory temporary file." );
    }
    initFileTracking();

    if ( argc == 1 ) {
        printHelp();
    }
    
    /* Create user data struct. */
    userInput *userData = createUserDataStruct();
    const int defaultBufferSize = 128;
    userData->bufferSize = defaultBufferSize;
    
    /* Populate userData from command line arguments and options. */
    commandLineArgumentHandler( argc, argv, userData );
    
    /* Create pointers for input and output files, and the FIR filter. */
    audioFile *inputFile, *outputFile;
    firFilter *filter;
    
    /* Create buffers required for audio processing. */
    double buffer[ g_maxBufferSize ];
    double filterDelayLine[ g_filterOrder + 1 ];
    
    /* Open files and create filter. */
    openFiles( userData, &inputFile, &outputFile );
    if ( ( filter = createFilter( g_filterOrder, filterDelayLine ) ) == NULL ) {
        printf( "Filter error code: %d\n", g_FILT_ERR );
        errorHandler( BAD_MEMORY, "Filter creation failed." );
    }
    setCoefficients( filter, getSampleRate( inputFile ), userData->filterFrequncy, userData->windowing );
    
    /* Audio processing loop. */
    int count = 0;
    
    do {
        count = readAudioDouble( inputFile, buffer, userData->bufferSize );
        
        
        /* Flush buffer logic goes here. */
        
        
        processBuffer( filter, buffer, count );
        
        if ( writeAudioDouble( outputFile, buffer, count ) < 0 ) {
            errorHandler( BAD_FILE_WRITE, "Could not write buffer to output file." );
        }
        
    } while ( count != 0 );
    
    /* Free memory. */
    cleanupMemory( userData, inputFile, outputFile, filter );
    
    return NO_ERR;
}
