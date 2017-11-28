//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "prog_header.h"


int main( int argc, char * argv[] ) {
    
    if ( argc == 1 ) {
        printHelp();
    }
    
    userInput *userData = createUserDataStruct();
    
    commandLineArgumentHandler( argc, argv, userData );
    
    /* Create pointers for input and output files, and the FIR filter */
    audioFile *inputFile, *outputFile;
    firFilter *filter;
    double buffer[ g_maxBufferSize ];
    double filterDelayLine[ g_filterOrder + 1 ];
    
    /* Open files and create filter */
    openFiles( userData, &inputFile, &outputFile );
    createFilter( g_filterOrder, filterDelayLine );
    
    /* Free memory */
    cleanupMemory( userData, inputFile, outputFile, filter );
    
    return NO_ERR;
}
