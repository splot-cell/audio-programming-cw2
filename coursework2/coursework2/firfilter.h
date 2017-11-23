//
//  firfilter.h
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#ifndef firfilter_h
#define firfilter_h

#include <stdlib.h> // For malloc() and free().
#include <stdio.h> // For printf().
#include <math.h> // For sin().

#include "errors.h" // For error codes.
#include "globals.h" // For g_pi and g_tau

#ifdef FILTER_TESTS

#include "firfilterAdditionalTest.h"

#else

/* DATA TYPES */

typedef struct firfilter_struct firFilter;

#endif // FILTER_TESTS

typedef enum FIR_WINDOWING_ENUM {
    WINDOW_RECTANGULAR,
    WINDOW_BARTLETT,
    WINDOW_HANNING,
    WINDOW_HAMMING,
    WINDOW_BLACKMAN
} firWindow;

/* FUNCTION PROTOTYPES */

/* createFilter()
 * Dynamically creates FIR filter struct with <order> coefficients. */
firFilter* createFilter( int order );


/* destroyFilter()
 * Frees dynamically allocated memory of <filter>.
 * If filter is NULL returns -1.
 * If data in filter is NULL returns -2 (this should not occur!).
 * If successful returns 0. */
int destroyFilter( firFilter *filter );


/* setCoefficients()
 * Sets the coefficients of <filter> based on the chosen <samplerate> and <cutoff> frequency (Hz).
 * Coefficients generate windowed low-pass response.
 * Returns 0 if successful.
 * Returns -1 if passed NULL <filter> pointer. */
int setCoefficients( firFilter *filter, int samplerate, double cutoff );


int setWindowing( firFilter *filter, );


/* getCoefficients()
 * Returns point to an array of doubles. Each element in array is a coefficient. Number of
 * coefficients specified by <order> parameter of createFilter(). */
double *getCoefficients( firFilter *filter );

#endif // firfilter_h
