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


typedef enum FILTER_TYPE_ENUM {
    TYPE_LOWPASS,
    TYPE_HIGHPASS
} filterType;


typedef enum FILTER_ERROR_ENUM {
    FILT_NO_ERR, // No error.
    FILT_MEM_ERR, // Error allocating or accessing memory.
    FILT_ARG_NULL, // Fuction has been passed NULL pointer.
    FILT_OOB_ARG, // Function has been passed an argument out of parameter bounds.
    FILT_FILE_ERR // Error relating to tempory file for memory handling.
} firErr;


/* GLOBALS */

extern firErr g_FILT_ERR;


/* FUNCTION PROTOTYPES */

/*      createFilter()
 * Creates FIR filter struct to contain a filter of order <order>.
 * <circularBuffer> = pointer to an array of doubles of size order + 1.
 * Returns NULL and sets g_FILT_ERR to relevent error code if unsuccessful. */
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


/*      initiFiltErrHandling()
 * To be run at program start. Initialises g_FILT_ERR to FILT_NO_ERR, and sets up dynamic memory
 * tracking. */
firErr initFiltErrHandling( void );


/*      filtFreeMem()
 * To be used when an error occurs. Will free all dynamically allocated memory related to fir
 * filter variables. */
void filtFreeMem( void );


#endif // firfilter_h
