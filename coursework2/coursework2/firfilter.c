//
//  firfilter.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "firfilter.h"


/* GLOBALS */

const double g_pi = 3.14159265359;
const double g_tau = 2 * g_pi;

firErr g_FILT_ERR;

static FILE *g_filtTemp;


/* TYPE DEFINITIONS */

typedef struct firfilter_struct {
    double *coeffs;
    int numCoeffs;
    double *delayLine;
    int delayLineIndex;
} firFilter;


/* PRIVATE FUNCTION PROTOTYPES */

firErr initDelayLine( firFilter *filter );

void applyBartlettWindow( firFilter *filter );

void applyHanningWindow( firFilter *filter );

void applyHammingWindow( firFilter *filter );

void applyBlackmanWindow( firFilter *filter );

void filterfatalError( firErr code, char *info );

firErr filtMemAllocated( void *ptr );


/* FUNCTION DEFINITIONS */

firFilter* createFilter( int order, double *circularBuffer ) {
    firFilter *filter = malloc( sizeof( firFilter ) );
    if ( filter == NULL ) {
        g_FILT_ERR = FILT_MEM_ERR;
        return NULL;
    }
    if ( filtMemAllocated( filter ) != FILT_NO_ERR ) {
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
        return FILT_MEM_ERR;
    }
    
    free( filter->coeffs );
    free( filter );
    
    return FILT_NO_ERR;
}


firErr setCoefficients( firFilter *filter, int samplerate, double cutoff, firWindow window ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    
    double ft = cutoff / samplerate;
    float M = filter->numCoeffs - 1;
    
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        if ( i == M / 2 ) {
            filter->coeffs[ i ] = 2 * ft;
        }
        else {
            filter->coeffs[ i ] = sin( g_tau * ft * ( i - ( M / 2 ) ) ) / ( g_pi * ( i - ( M / 2 ) ) );
        }
    }
    
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
    if ( numSamples < 0 ) {
        return FILT_OOB_ARG;
    }
    
    for ( int i = 0; i < numSamples; ++i ) {
        filter->delayLine[ filter->delayLineIndex ] = buffer[ i ];
        for ( int j = 0; j < filter->numCoeffs; ++j ){
            buffer[ i ] = filter->coeffs[ j ] *
                filter->delayLine[ ( filter->delayLineIndex + j ) % filter->numCoeffs ];
        }
        filter->delayLineIndex = ( filter->numCoeffs + filter->delayLineIndex - 1 ) % filter->numCoeffs;
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
        filter->coeffs[ i ] *= 0.42 - ( 0.5 * cos( 2.0 * g_pi * i / M ) ) + ( 0.08 * cos( 4.0 * g_pi * i / M ) );
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

#endif // FILTER_TESTS
