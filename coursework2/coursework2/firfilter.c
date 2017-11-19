//
//  firfilter.c
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "firfilter.h"
#include <stdlib.h> // For malloc() and free().
#include "enums.h" // For error codes.
#include "constants.h" // For g_pi and g_tau
#include <math.h> // For sin().


typedef struct firfilter_struct {
    double *coeffs;
    int numCoeffs;
} firFilter;


firFilter* createFilter( int order ) {
    firFilter *filter = malloc( sizeof( firFilter ) );
    if ( filter == NULL ) {
        exit( BAD_MEMORY );
    }
    
    filter->coeffs = calloc ( order + 1, sizeof( double ) );
    if ( filter->coeffs == NULL ) {
        exit( BAD_MEMORY );
    }
    
    filter->numCoeffs = order + 1;
    
    return filter;
}


int destroyFilter( firFilter *filter ) {
    if ( filter == NULL ) {
        return -1;
    }
    if ( filter->coeffs == NULL ) {
        return -2;
    }
    
    free( filter->coeffs );
    free( filter );
    
    return 0;
}

int setCoefficients( firFilter *filter, int samplerate, double cutoff ) {
    if ( filter == NULL ) {
        return -1;
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
    
    return 0;
}

double *getCoefficients( firFilter *filter ) {
    return filter->coeffs;
}

#ifdef FILTER_TESTS

double **getData( firFilter *filter ) {
    return &filter->coeffs;
}

int *getOrder( firFilter *filter ) {
    return &filter->numCoeffs;
}

#endif // FILTER_TESTS
