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
    
//    /* Sanitise input and populate pointers */
//    if ( initialiseVar( userData, inputFile, outputFile, filter ) != NO_ERR ) {
//        return BAD_COMMAND_LINE;
//    }
//    
//    /* Proess audio files */
//    else if ( processAudio( inputFile, outputFile, filter ) != 0 ) {
//        
//    }
//    
    /* Free memory */
    cleanupMemory( userData, inputFile, outputFile, filter );
    
    return NO_ERR;
}
