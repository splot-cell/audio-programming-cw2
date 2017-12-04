//
//  main.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

/* Program files structure:
 *      main.c              = main function
 *      prog_header.h       = program header, contains program-specific prototypes and globals
 *      prog_file.c         = program source file, contains program-specific functions
 *      errors.h/.c         = error handling for the program
 *      firfilter.h/.c      = fir filter 'library', reusable code
 *      iofunctions.h/.c    = audio file in/out handling and user input sanitising, reusable
 *
 * Dependancy:
 *      As errors.h is specific to this program, only prog_header.h (and therefore main.c/prog_file.c)
 *      depend on it. All error calls are therefore made from main.c or prog_file.c
 *
 *      firfilter.h and iofunctions.h are both used solely by the main and prog_ files. Therefore
 *      the dependancy tree does not contain any cyclic-dependancies or cross-dependancies.
 *
 * Program structure notes:
 *      ERRORS:
 *      When an error is called, the program must exit. To do so gracefully, dynamically allocated
 *      memory must be freed and any open files must be closed. To do this, three linked lists are
 *      implemented:
 *      program dynamic memory  = list stored in errors.c
 *      filter dynamic memory   = list stored in firfilter.c
 *      open files              = list stored in prog_file.c
 *      These lists track the memory allocation and file use, and when an error is called using
 *      errorHandler(), the memory will be freed and files closed before exit.
 *
 *      UNIT TESTS:
 *      An open source framework "qtest" = https://github.com/inkychris/qtest is used to provide
 *      some testing. This has been developed by Chris Wright, with some aid from Olly Seber.
 *      The tests currently cover some of the key filter functions and io functions. Documentation
 *      of the tests is limited, but they should be fairly self-documenting.
 *
 *      All other areas should be documented within the code. Any questions or suggestions should
 *      be directed to Olly Seber: os00075@surrey.ac.uk.
 */

#include "prog_header.h" // program header file

#include <stdlib.h>
#include <stdio.h>


int main( int argc, char * argv[] ) {

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
    
    /* Check selected filter frequency is within range. */
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
