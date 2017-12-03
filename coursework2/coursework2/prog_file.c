//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"

#include <stdlib.h> // For calloc() and free().
#include <stdio.h> // For printf(), fprintf(), sprintf(), getchar().
#include <string.h> // For strcomp().
#include <getopt.h> // For getopt().
#include <ctype.h> // For isprint().
#include <stdbool.h> // For booleans.
#include <limits.h> // For converting integers.


/* OPEN FILE TRACKING STRUCT */

typedef struct node_struct {
    audioFile *data;
    struct node_struct *next;
} node;


/* GLOBAL INITIALISATIONS */

const int g_minFilterFreq = 1;
const int g_maxFilterFreq = INT_MAX; // Frequency is also limited to less than Nyquist/2, this
                                     //     provides additional limit if desired.
const int g_filterOrder = 126;
const int g_maxBufferSize = 2048;
const int g_minBufferSize = 64;
const int g_defaultBufferSize = 128;

static node *g_head = NULL; // For open audioFile tracking.


/* PRIVATE FUNCTION PROTOTYPES */

/*      optionalArgumentHandler()
 * Handles optional arguments from the comand line, writes the result to <userOptions>.
 * <argc> = argc from command line.
 * <argv> = argv from command line.
 * <userOptions> = pointer to userInput struct for storing user input. */
void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions );


void allocFilenameMem( char **filename, unsigned long length );


/*      wavFilenameHandler()
 * Checks whether <filename> ends '.wav' and gives the user the option to append '.wav' if not. 
 * If <mode> is 'i' then the user prompt speficies "Input filename".
 * Otherwise the promt specified "Output filename". */
bool wavFilenameHandler( char **filename, char mode );


/*      checkAudioFileMono()
 * Checks whether <file> is one channel. If so returns. If not then throws error. */
void checkAudioFileMono( audioFile *file );


/*      fileOpened()
 * For tracking files opened by the program.
 * Adds <ptr> to g_tempFiles. */
void fileOpened( audioFile *ptr );


/*      closeOpenFiles()
 * Iterates through files pointed to in g_tempFiles and closes them. */
void closeOpenFiles( void );


/* FUNCTION DEFINITIONS */

userInput* createUserDataStruct( void ) {
    userInput *data = calloc( 1, sizeof( userInput ) );
    if ( data == NULL ) {
        errorHandler( BAD_MEMORY, "Could not allocate memory for user input." );
    }
    memAllocated( data );
    data->inputFilename = NULL;
    data->outputFilename = NULL;
    data->filterType = TYPE_LOWPASS; // Set default functionality
    return data;
}


int destroyUserDataStruct( userInput *data ) {
    if ( data == NULL ) {
        return NULL_FUNC_ARG;
    }
    if ( data->inputFilename != NULL ) {
        free( data->inputFilename );
    }
    if ( data->outputFilename != NULL ) {
        free( data->outputFilename );
    }
    free( data );
    return NO_ERR;
}



void commandLineArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    optionalArgumentHandler( argc, argv, userOptions );
    
    /* Check number of required arguments. */
    int providedArg = argc - optind;
    if ( providedArg != 3 ) {
        errorHandler( BAD_COMMAND_LINE, "Could not continue, incorrect number of arguments detected." );
    }
    
    /* Required arguments are placed at end of argv by getopt, provided user input was valid. */
    
    /* Allocate memory for filnames, adding additional characters for '.wav\0'. */
    allocFilenameMem( &userOptions->inputFilename, strlen( argv[ argc - 3 ] ) + 6 );
    allocFilenameMem( &userOptions->outputFilename, strlen( argv[ argc - 2 ] ) + 6 );
    
    strcpy( userOptions->inputFilename, argv[ argc - 3] );
    strcpy( userOptions->outputFilename, argv[ argc - 2] );
    
    if ( wavFilenameHandler( &userOptions->inputFilename, 'i' ) == false ||
            wavFilenameHandler( &userOptions->outputFilename, 'o' ) == false ) {
        errorHandler( NO_ERR, "" );
    }
    
    /* Check characters and range of requested frequency */
    if ( isOnlyPositiveInt( argv[ argc - 1 ] ) == false ) {
        errorHandler( BAD_COMMAND_LINE, "Non-integer character detected in cut-off frequency" );
    }
    
    if ( strToInt( argv[ argc - 1], &( userOptions->filterFrequncy ),
                  g_minFilterFreq, g_maxFilterFreq ) != 0 ) {
        errorHandler( OUT_OF_BOUNDS_VALUE, "Filter frequency outside of acceptable range." );
    }
}


void optionalArgumentHandler( int argc, char *argv[], userInput *userOptions ) {
    /* Set up options. */
    struct option optionalArgs[] = {
        { "window", required_argument, 0, 'w' },
        { "highpass", no_argument, 0, 'h' },
        { "buffersize", required_argument, 0, 'b' },
        { 0, 0, 0, 0 }
    };
    int optionIndex = 0;
    char option;
    
    /* Iterate through argv. */
    while ( ( option = getopt_long( argc, argv, "w:hb:", optionalArgs, &optionIndex ) ) != -1 ) {
        switch ( option ) {
            case 'w': // WINDOWING
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
                            "Invalid option '%s' for windowing.\n"
                            "Will try to continue using default: Bartlett.\n", optarg );
                }
                break;
                
            case 'h': // HIGHPASS FILTER
                userOptions->filterType = TYPE_HIGHPASS;
                break;
                
            case 'b': // BUFFER SIZE
                if ( isOnlyPositiveInt( optarg ) == false ) {
                    errorHandler( BAD_COMMAND_LINE, "Buffer size must be a positive integer." );
                }
                
                if ( strToInt( optarg, &( userOptions->bufferSize ), g_minBufferSize, g_maxBufferSize ) != 0 ) {
                    errorHandler( OUT_OF_BOUNDS_VALUE, "Buffer size is outside required range." );
                }
                
                if ( ( userOptions->bufferSize % g_minBufferSize ) != 0 ) {
                    errorHandler( BAD_COMMAND_LINE, "Buffer size must be a power of 2." );
                }
                break;
                
            case '?': // Equivalent to default case.
                if ( optopt == 'w' ) {
                    fprintf( stderr, "Option -w requires an argument. Using default: bartlett.\n" );
                }
                else if ( isprint( optopt ) ) {
                    char error[100];
                    sprintf( error, "Unknown option '-%c'.", optopt );
                    errorHandler( BAD_COMMAND_LINE, error );
                }
                else { // Should not occur.
                    char error[100];
                    sprintf( error, "Unknown option character '\\x%x'.\n", optopt );
                    errorHandler( BAD_COMMAND_LINE, error );
                }
        }
    }
}


void openFiles( userInput *userData, audioFile **inputFile, audioFile **outputFile ) {
    
    if ( ( *inputFile = allocateAudioFileMem() ) == NULL ) {
        errorHandler( BAD_MEMORY, "Could not allocate memory for input file." );
    }
    memAllocated( *inputFile );
    
    if ( openInputFile( *inputFile, userData->inputFilename ) != NO_ERR ) {
        errorHandler( BAD_FILE_OPEN, "Could not open input file selected!" );
    }
    fileOpened( *inputFile );
    
    checkAudioFileMono( *inputFile );
    
    if ( ( *outputFile = allocateAudioFileMem() ) == NULL ) {
        errorHandler( BAD_MEMORY, "Could not allocate memory for output file." );
    }
    memAllocated( *outputFile );
    
    if ( openOutputFile( *outputFile, userData->outputFilename, *inputFile ) != NO_ERR ) {
        errorHandler( BAD_FILE_OPEN, "Could not open output file selected!" );
    }
    fileOpened( *outputFile );
}


void checkAudioFileMono( audioFile *file ) {
    if ( getChannelCount( file ) != 1 ) {
        errorHandler( BAD_FILE_OPEN, "Input file must have one channel." );
    }
}


bool wavFilenameHandler( char **filename, char mode ) {
    char str[ 8 ];
    strcpy( str, ( mode == 'i' ? "Input" : "Output" ) );
    
    if ( isWavFilename( *filename ) == false ) {
        printf( "%s filename '%s' not recognised as '*.wav'. Would you like to append '.wav'? (y/n)\n",
               str, *filename );
        if ( getYesNo() ) {
            strcat( *filename, ".wav" );
        }
        else {
            return false;
        }
    }
    return true;
}

        
void allocFilenameMem( char **filename, unsigned long length ) {
    *filename = calloc( length, sizeof( char ) );
    if ( *filename == NULL ) {
        errorHandler( BAD_MEMORY, "Could not allocate memory for filename." );
    }
    memAllocated( *filename );
}


/* OPEN FILE TRACKING - LINKED LIST FUNCTIONS */

int filePush( audioFile *ptr ) {
    node *new;
    new = malloc( sizeof( node ) );
    if ( new == NULL ) {
        return 1;
    }
    new->data = ptr;
    new->next = g_head;
    
    g_head = new;
    
    return 0;
}


audioFile* filePop( void ) {
    audioFile *retVal = NULL;
    node *next = NULL;
    
    if ( g_head == NULL ) {
        return NULL;
    }
    
    next = g_head->next;
    retVal = g_head->data;
    free( g_head );
    g_head = next;
    
    return retVal;
}


void fileOpened( audioFile *ptr ) {
    if ( filePush( ptr ) != 0 ) {
        programExit( BAD_MEMORY, "Could not allocate memory for open file tracking!" );
    }
    
//    printf( "Wrote pointer %p to file list\n", ptr ); // For debugging purposes
}


void closeOpenFiles( void ) {
    void *temp;
    while ( ( temp = filePop() ) != NULL ) {
        closeAudioFile( temp );
//        printf( "Freed pointer %p from list\n", temp ); // Debugging
    }
}


void errorHandler( int code, char *info ) {
    freeFiltMemory(); // Prevent memory leaks from filter.
    closeOpenFiles(); // Prevent hanging files.
    programExit( code, info ); // Handles memory leaks from program and exits.
}
        

void cleanupMemory( userInput *userOptions, audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    destroyUserDataStruct( userOptions );
    closeAudioFile( inputFile );
    freeAudioFileMem( inputFile );
    closeAudioFile( outputFile );
    freeAudioFileMem( outputFile );
    destroyFilter( filter );
}


void printHelp( void ) {
    
    char *helpTitle[] = {
        "cOLLY'S WONDEROUS COURSEWORK SUBMISSION 2:",
        "THE FILTERING",
        "\"This time it's personal.\"",
        "---",
        "-->> Help Documentation <<--"
    };
    printWithBorder( helpTitle, ( sizeof ( helpTitle ) / sizeof( helpTitle[ 0 ] ) ), 3 );
    printf( "\n" );
    
    char *helpText[] = {//----------------------------single line character limit---|
        "This program will apply a low-pass filter to audio from an input wav file",
        "and write the result to a specified output wav file.",
        "",
        "The required argument format is:",
        "",
        "<input file path> <output file path> <cut-off frequency>",
        "",
        "The input and output paths should NOT contain whitespace. The frequency",
        "should be an integer in Hz. The frequency range supported is from 1Hz up ",
        "to just less than half the sample frequency of the selected input file.",
        "",
        "---",
        "",
        "-->> Optional arguments are now available! <<--",
        "",
        "For optimum results these options should be included before the required",
        "arguements. Please consult your doctor before use.",
        "",
        "In order to use the once-in-a-lifetime HIGHPASS filter mode please include:",
        "",
        "-h",
        "or",
        "--highpass",
        "",
        "To get your hands on some never-seen-before WINDOWING options please use:",
        "",
        "-w <window specifier>",
        "or",
        "--window <window specifier>",
        "",
        "The window specifier may be:",
        "",
        "rect = Rectangular window function",
        "bart = Bartlett window function (default)",
        "hann = Hanning window function",
        "hamm = Hamming window function (This one's a corker! [Henry, 2017])",
        "black = Blackman window function",
        "",
        "The window specifier should IMMEDIATELY follow the -w or --window flag.",
        "",
        "And finally, if you're flying with our luxuary package, you may also like",
        "to modify the BUFFER SIZE! Don't worry, no pesky dynamic allocation here!",
        "Simply enter:",
        "",
        "-b <desired size>",
        "or",
        "--buffersize <desired size>",
        "",
        "With a <desired size> between 64 and 2048 samples! Note, you must use a",
        "power of two. 128 samples is the default, but 256 will blow you away!",
        "",
        "The desired size should IMMEDIATELY follow the -b or --buffersize flag.",
        "",
        "References:",
        "",
        "Henry, Craig, 2017: Personal correspondance with the author"
    };
    printWithBorder( helpText, ( sizeof( helpText ) / sizeof( helpText[ 0 ] ) ), 1 );
    
    errorHandler( NO_ERR, "" );
}
