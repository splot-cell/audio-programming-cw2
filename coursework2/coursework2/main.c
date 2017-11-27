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
    
    /* Check correct number of arguments are supplied */
    if ( commandLineArgumentHandler( argc, argv, userData ) != NO_ERR ) {
        fatalError( BAD_COMMAND_LINE,
                   "Run with zero additional command line arguments to print help." );
    }
    
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
