//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "prog_header.h"


int main( int argc, char * argv[] ) {
    
    /* Initialise dynamic memory tracking. */
    initMemTracking();
    
    
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
    filter = createFilter( g_filterOrder, filterDelayLine );
    memAllocated( filter );
    setCoefficients( filter, getSampleRate( inputFile ), userData->filterFrequncy, userData->windowing );
    
    /* Audio processing loop. */
    int count = 0;
    
    do {
        count = readAudioDouble( inputFile, buffer, userData->bufferSize );
        
        processBuffer( filter, buffer, count );
        
        writeAudioDouble( outputFile, buffer, count );
    } while ( count != 0 );
    
    /* Flush buffer. */
    
    /* Free memory. */
    cleanupMemory( userData, inputFile, outputFile, filter );
    
    return NO_ERR;
}
