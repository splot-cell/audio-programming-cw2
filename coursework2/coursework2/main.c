//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "prog_header.h"

#include <stdlib.h>
#include <stdio.h>


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
    userData->bufferSize = g_defaultBufferSize;
    
    /* Populate userData from command line arguments and options. */
    commandLineArgumentHandler( argc, argv, userData );
    
    /* Create pointers for input and output files, and the FIR filter. */
    audioFile *inputFile, *outputFile;
    firFilter *filter;
    
    /* Create buffers required for audio processing. */
    double buffer[ g_maxBufferSize ];
    double filterDelayLine[ g_filterOrder + 1 ];
    
    /* Open files. */
    openFiles( userData, &inputFile, &outputFile );
    
    /* Check selected filter frequency. */
    if ( userData->filterFrequncy >= 0.5 * getSampleRate( inputFile ) ) {
        errorHandler( OUT_OF_BOUNDS_VALUE, "Please select a cut-off frequency less than half the sample rate of the input file." );
    }
    
    /* Create filter. */
    if ( ( filter = createFilter( g_filterOrder, filterDelayLine, userData->filterType ) ) == NULL ) {
        printf( "Filter error code: %d\n", g_FILT_ERR );
        errorHandler( BAD_MEMORY, "Filter creation failed." );
    }
    setCoefficients( filter, getSampleRate( inputFile ), userData->filterFrequncy, userData->windowing );
    
    /* Audio processing loop. */
    int count = 0;
    int tailCount = 0; // Number of samples written from the "decay tail" of the filter
    
    do {
        count = readAudioDouble( inputFile, buffer, userData->bufferSize );
        
        /* Flush buffer logic. */
        if ( count != userData->bufferSize ) {
            do {
                buffer [ ( count + tailCount ) % userData->bufferSize ] = 0;
                ++tailCount;
                ++count;
            } while ( count < userData->bufferSize && // While it's not the end of the buffer
                     tailCount < g_filterOrder );     // and there's still tail left.
        }
        
        if ( processBuffer( filter, buffer, count ) != FILT_NO_ERR ) {
            errorHandler( BAD_BUFFER_PROCESS, "Error processing audio buffer." );
        }
        
        if ( writeAudioDouble( outputFile, buffer, count ) < 0 ) {
            errorHandler( BAD_FILE_WRITE, "Could not write buffer to output file." );
        }
        
    } while ( g_filterOrder != tailCount );
    
    /* Free memory. */
    cleanupMemory( userData, inputFile, outputFile, filter );
    
    return NO_ERR;
}
