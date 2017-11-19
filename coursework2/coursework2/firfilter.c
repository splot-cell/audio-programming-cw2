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


typedef struct firfilter_struct {
    double *coeffs;
    int order;
} firFilter;


firFilter* createFilter( int order ) {
    firFilter *filter = malloc( sizeof( firFilter ) );
    if ( filter == NULL ) {
        exit( BAD_MEMORY );
    }
    
    filter->coeffs = calloc ( order, sizeof( double ) );
    if ( filter->coeffs == NULL ) {
        exit( BAD_MEMORY );
    }
    
    filter->order = order;
    
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

int setCoefficients( firFilter *filter, int samplerate, double cutoff, double q );

double *getCoefficients( firFilter *filter );

#ifdef FILTER_TESTS

double **getData( firFilter *filter ) {
    return &filter->coeffs;
}

int *getOrder( firFilter *filter ) {
    return &filter->order;
}

#endif // FILTER_TESTS
