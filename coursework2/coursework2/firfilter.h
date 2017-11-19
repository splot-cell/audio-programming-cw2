//
//  firfilter.h
//  coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#ifndef firfilter_h
#define firfilter_h

#ifdef FILTER_TESTS

#include "firfilterAdditionalTest.h"

#else

/* Struct for storing filter data */
typedef struct firfilter_struct firFilter;

#endif // FILTER_TESTS

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
 * Sets the coefficients of <filter> based on the chosen <samplerate>, <cutoff> frequency (Hz),
 * and quality factor <q>. Coefficients generate is low-pass response.
 * Returns 0 if successful. */
int setCoefficients( firFilter *filter, int samplerate, double cutoff, double q );


/* getCoefficients()
 * Returns point to an array of doubles. Each element in array is a coefficient. Number of
 * coefficients specified by <order> parameter of createFilter(). */
double *getCoefficients( firFilter *filter );

#endif // firfilter_h
