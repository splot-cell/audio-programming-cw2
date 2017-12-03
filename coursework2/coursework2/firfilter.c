//
//  firfilter.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "firfilter.h"

#include <stdlib.h> // For malloc() and free().
#include <stdio.h> // For printf().
#include <math.h> // For sin().


/* GLOBALS */

const double g_pi = 3.14159265359;
const double g_tau = 2 * g_pi;

firErr g_FILT_ERR; // For returning an error from certain functions.

static FILE *g_filtTemp; // For tracking memory usage


/* TYPE DEFINITIONS */

typedef struct firfilter_struct {
    double *coeffs; // Array dynamically allocated when creating filter.
    int numCoeffs;
    double *delayLine; // Array must be (statically) created by program and passed to filter.
    int delayLineIndex;
    filterType type;
} firFilter;


/* PRIVATE FUNCTION PROTOTYPES */

/*      initDelayLine()
 * Sets all coefficient datra of <filter> to zero. */
firErr initDelayLine( firFilter *filter );


/*      Window fuctions:
 * Apply the respective window weighting to <filter>. */
void applyBartlettWindow( firFilter *filter );

void applyHanningWindow( firFilter *filter );

void applyHammingWindow( firFilter *filter );

void applyBlackmanWindow( firFilter *filter );


/*      filtMemAllocated()
 * For dynamic memory tracking. Adds <ptr> to g_filtTemp file. */
firErr filtMemAllocated( void *ptr );


/* FUNCTION DEFINITIONS */

firFilter* createFilter( int order, double *circularBuffer, filterType type ) {
    if ( type == TYPE_HIGHPASS && ( order % 2 ) != 0 ) { // Highpass filters must have an even order
        g_FILT_ERR = FILT_TYPE_ERR;                      // i.e. odd number of coefficients.
        return NULL;
    }
    
    firFilter *filter = malloc( sizeof( firFilter ) );
    if ( filter == NULL ) {
        g_FILT_ERR = FILT_MEM_ERR;
        return NULL;
    }
    if ( filtMemAllocated( filter ) != FILT_NO_ERR ) { // Error check file write
        g_FILT_ERR = FILT_FILE_ERR;
        return NULL;
    }
    
    filter->coeffs = calloc ( order + 1, sizeof( double ) );
    if ( filter->coeffs == NULL ) {
        g_FILT_ERR = FILT_MEM_ERR;
        return NULL;
    }
    if ( filtMemAllocated( filter->coeffs ) != FILT_NO_ERR ) {
        g_FILT_ERR = FILT_FILE_ERR;
        return NULL;
    }
    
    filter->numCoeffs = order + 1;
    filter->delayLine = circularBuffer;
    filter->delayLineIndex = 0;
    filter->type = type;
    
    if ( initDelayLine( filter ) != FILT_NO_ERR ) {
        g_FILT_ERR = FILT_MEM_ERR;
        return NULL;
    }
    
    return filter;
}


firErr destroyFilter( firFilter *filter ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    if ( filter->coeffs == NULL ) {
        return FILT_MEM_ERR; // Shouldn't happen
    }
    
    free( filter->coeffs );
    free( filter );
    
    return FILT_NO_ERR;
}


firErr setCoefficients( firFilter *filter, int samplerate, double cutoff, firWindow window ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    
    /* High pass filters must have an odd num of coefficients. */
    if ( ( filter->numCoeffs % 2 ) == 0 && filter->type == TYPE_HIGHPASS ) {
        return FILT_TYPE_ERR;
    }
    
    /* Prepare some variables for the maths. */
    double ft = cutoff / (double) samplerate;
    if ( ft >= 0.5 ) { // Cut-off should be less than half the sample frequency.
        return FILT_OOB_ARG;
    }
    
    float M_2 = ( filter->numCoeffs - 1 ) / 2.0; // Filter order divided by 2.
    
    /* Set coefficients. */
    for ( int i = 0; i < filter->numCoeffs / 2.0; ++i ) {
        if ( i == M_2 ) {
            filter->coeffs[ i ] = 2 * ft;
            if ( filter->type == TYPE_HIGHPASS ) {
                filter->coeffs[ i ] = 1 - filter->coeffs[ i ];
            }
        }
        else {
            filter->coeffs[ i ] = sin( g_tau * ft * ( i - M_2 ) ) / ( g_pi * ( i - M_2 ) );
            
            if ( filter->type == TYPE_HIGHPASS ) {
                filter->coeffs[ i ] = -filter->coeffs[ i ];
            }
            
            /* Coefficients are symetrical so only calculate once then copy to second half. */
            filter->coeffs[ filter->numCoeffs - i - 1 ] = filter->coeffs[ i ];
        }
    }
    
    /* Set windowing. */
    switch ( window ) {
        case WINDOW_RECTANGULAR:
            break;
        case WINDOW_BARTLETT:
            applyBartlettWindow( filter );
            break;
        case WINDOW_HANNING:
            applyHanningWindow( filter );
            break;
        case WINDOW_HAMMING:
            applyHammingWindow( filter );
            break;
        case WINDOW_BLACKMAN:
            applyBlackmanWindow( filter );
            break;
        default:
            break;
    }
    
    return FILT_NO_ERR;
}


firErr processBuffer( firFilter *filter, double *buffer, int numSamples ) {
    if ( filter == NULL || buffer == NULL ) {
        return FILT_ARG_NULL;
    }
    if ( numSamples < 0 ) { // Avoid infinite for loop.
        return FILT_OOB_ARG;
    }
    
    /* Iterate through the samples. */
    for ( int i = 0; i < numSamples; ++i ) {
        filter->delayLine[ filter->delayLineIndex ] = buffer[ i ];
        buffer[ i ] = 0;
        
        /* For each sample, iterate through the coefficients. */
        for ( int j = 0; j < filter->numCoeffs; ++j ){
            buffer[ i ] += filter->coeffs[ j ] *
                filter->delayLine[ ( filter->delayLineIndex + filter->numCoeffs - j ) % filter->numCoeffs ];
        }
        filter->delayLineIndex = ( filter->delayLineIndex + 1 ) % filter->numCoeffs;
    }
    return FILT_NO_ERR;
}


firErr initDelayLine( firFilter *filter ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    
    for ( int i = 0; i < filter->numCoeffs; ++i ){
        filter->delayLine[ i ] = 0;
    }
    return FILT_NO_ERR;
}


void applyBartlettWindow( firFilter *filter ) {
    float M = filter->numCoeffs - 1;
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        filter->coeffs[ i ] *= 1 - ( 2 * fabs( i - ( M / 2 ) ) / M );
    }
}


void applyHanningWindow( firFilter *filter ) {
    float M = filter->numCoeffs - 1;
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        filter->coeffs[ i ] *= 0.5 - ( 0.5 * cos( 2 * g_pi * i / M ) );
    }
}


void applyHammingWindow( firFilter *filter ) {
    float M = filter->numCoeffs - 1;
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        filter->coeffs[ i ] *= 0.54 - ( 0.46 * cos( 2.0 * g_pi * i / M ) );
    }
}


void applyBlackmanWindow( firFilter *filter ) {
    float M = filter->numCoeffs - 1;
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        filter->coeffs[ i ] *= 0.42 - ( 0.5 * cos( 2.0 * g_pi * i / M ) ) +
            ( 0.08 * cos( 4.0 * g_pi * i / M ) );
    }
}


/* For dynammic memory allocation tracking. */

firErr initFiltErrHandling( void ) {
    g_filtTemp = tmpfile();
    if ( g_filtTemp == NULL ) {
        return FILT_FILE_ERR;
    }
    return FILT_NO_ERR;
}


firErr filtMemAllocated( void *ptr ) {
    if ( fprintf( g_filtTemp, "%p ", ptr ) < 0 ) {
        return FILT_FILE_ERR;
    }
//    printf( "Wrote pointer %p to temp\n", ptr ); // For debugging purposes
    return FILT_NO_ERR;
}


void filtFreeMem( void ) {
    rewind( g_filtTemp );
    void *ptr;
    
    while ( fscanf( g_filtTemp, "%p", &ptr ) != EOF ) {
//        printf( "Read pointer %p from temp\n", ptr ); // For debugging purposes
        free( ptr );
    }
    
    fclose( g_filtTemp );
}


/* Additional methods used for testing. */

#ifdef FILTER_TESTS


double **getData( firFilter *filter ) {
    return &filter->coeffs;
}


int *getOrder( firFilter *filter ) {
    return &filter->numCoeffs;
}


double *getCoefficients( firFilter *filter ) {
    return filter->coeffs;
}


firErr setFilterType( firFilter *filter, filterType type ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    
    if ( type != TYPE_LOWPASS && type != TYPE_HIGHPASS ) {
        return FILT_TYPE_ERR;
    }
    
    filter->type = type;
    
    return FILT_NO_ERR;
}


#endif // FILTER_TESTS
