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


/* TYPE DEFINITIONS */

typedef struct firfilter_struct {
    double *coeffs;
    int numCoeffs;
} firFilter;


/* PRIVATE FUNCTION PROTOTYPES */

void fatalError( firErr code, char *info );


/* FUNCTION DEFINITIONS */

firFilter* createFilter( int order ) {
    firFilter *filter = malloc( sizeof( firFilter ) );
    if ( filter == NULL ) {
        fatalError( FILT_MEM_ERR, "Could not allocate filter memory." );
    }
    
    filter->coeffs = calloc ( order + 1, sizeof( double ) );
    if ( filter->coeffs == NULL ) {
        free( filter ); // Tidy up.
        fatalError( FILT_MEM_ERR, "Could not allocate coefficient memory." );
    }
    
    filter->numCoeffs = order + 1;
    
    return filter;
}


firErr destroyFilter( firFilter *filter ) {
    if ( filter == NULL ) {
        return FILT_ARG_NULL;
    }
    if ( filter->coeffs == NULL ) {
        fatalError( FILT_MEM_ERR, "Filter coefficients NULL." );
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
    int M = filter->numCoeffs - 1;
    
    for ( int i = 0; i < filter->numCoeffs; ++i ) {
        if ( i == M / 2 ) {
            filter->coeffs[ i ] = 2 * ft;
        }
        else {
            filter->coeffs[ i ] = sin( g_tau * ft * ( i - ( M / 2 ) ) ) / ( g_pi * ( i - ( M / 2 ) ) );
        }
    }
    
    // NEED TO ADD WINDOWING
    
    return FILT_NO_ERR;
}


void fatalError( firErr code, char *info ) {
    printf( "FIR FILTER ERROR: %s\n", info );
    exit( code );
}


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
