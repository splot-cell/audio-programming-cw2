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

typedef enum FILTER_ERROR_ENUM {
    FILT_NO_ERR,
    FILT_MEM_ERR,
    FILT_ARG_NULL,
    FILT_BAD_ARG
} firErr;


/* FUNCTION PROTOTYPES */

/*      createFilter()
 * Creates FIR filter struct to contain a filter of order <order>.
 * <circularBuffer> = pointer to an array of doubles of size order + 1. */
firFilter* createFilter( int order, double *circularBuffer );


/*      destroyFilter()
 * Frees memory allocated to <filter>.
 * Returns FILT_NO_ERR if successful.
 * Returns FILT_ARG_NULL if <filter> is NULL. */
firErr destroyFilter( firFilter *filter );


/*      setCoefficients()
 * Sets the coefficients of <filter> based on the chosen <samplerate> (Hz) and <cutoff> frequency (Hz).
 * Coefficients generate windowed low-pass response, using <window> specified.
 * Returns FILT_NO_ERR if successful.
 * Returns FILT_ARG_NULL if <filter> is NULL. */
firErr setCoefficients( firFilter *filter, int samplerate, double cutoff, firWindow window );


/*      processBuffer()
 * */
firErr processBuffer( firFilter *filter, double *buffer, int numSamples );

#endif // firfilter_h
