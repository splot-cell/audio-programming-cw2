//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "prog_header.h"


int main( int argc, const char * argv[] ) {
    
    /* Check correct number of arguments are supplied */
    if ( commandLineArgumentHandler( argc, argv ) != 0 ) {
        fatalError( BAD_COMMAND_LINE,
                   "Run with zero additional command line arguments to print help." );
    }
    
    /* Create pointers for input and output files, and the FIR filter */
    audioFile *inputFile, *outputFile;
    firFilter *filter;
    
    /* Sanitise input and populate pointers */
    else if ( interpretUserInput( argc, argv, inputFile, outputFile, filter ) != 0 ) {
        return BAD_COMMAND_LINE;
    }
    
    /* Proess audio files */
    else if ( processAudio( inputFile, outputFile, filter ) != 0 ) {
        
    }
    
    /* Free memory */
    cleanupMemory( inputFile, outputFile, filter );
    
    return NO_ERR;
}
