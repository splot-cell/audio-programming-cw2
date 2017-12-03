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
    FILT_TYPE_ERR // Error related to selected filter type.
} firErr;


/* GLOBALS */

extern firErr g_FILT_ERR; // For returning error code from createFilter().


/* FUNCTION PROTOTYPES */

/*      createFilter()
 * Creates FIR filter struct to contain a filter of order <order>.
 * <circularBuffer> = pointer to an array of doubles of size order + 1.
 * Returns NULL and sets g_FILT_ERR to relevent error code if unsuccessful. */
firFilter* createFilter( int order, double *circularBuffer, filterType type );


/*      destroyFilter()
 * Frees memory allocated to <filter>.
 * Returns FILT_NO_ERR if successful.
 * Returns FILT_ARG_NULL if <filter> is NULL. */
firErr destroyFilter( firFilter *filter );


/*      setCoefficients()
 * Sets the coefficients of <filter> based on the chosen <samplerate> (Hz) and <cutoff> frequency (Hz).
 * Low-pass or high-pass response selected during filter creation using enum above.
 * Coefficients generated using <window> specified.
 * Returns FILT_NO_ERR if successful.
 * Returns FILT_ARG_NULL if <filter> is NULL. */
firErr setCoefficients( firFilter *filter, int samplerate, double cutoff, firWindow window );


/*      processBuffer()
 * Iterates through <numSamples> samples in <buffer>, applying effects of <filter>.
 * Returns FILT_NO_ERR if successful.
 * Returns FILT_ARG_NULL if <filter> or <buffer> is NULL.
 * Returns FILT_OOB_ARG if numSamples < 0. */
firErr processBuffer( firFilter *filter, double *buffer, int numSamples );


/*      initiFiltErrHandling()
 * To be run at program start. Initialises g_FILT_ERR to FILT_NO_ERR, and sets up dynamic memory
 * tracking. */
firErr initFiltErrHandling( void );


/*      filtFreeMem()
 * To be used when an error occurs. Will free all dynamically allocated memory related to fir
 * filter variables. */
void freeFiltMemory( void );


#endif // firfilter_h
