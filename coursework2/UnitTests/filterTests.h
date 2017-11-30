//
//  filterTests.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef filterTests_h
#define filterTests_h

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "qtest/testsuite.h"
#include "firfilter.h"

/* GLOBALS */
extern double g_buf[ 130 ]; // Single buffer used to create each filter under test. Saves memory.


/* FUNCTION PROTOTYPES */

void resetg_buf( void );


void createFilter_returnsPointerToFilter( qunittest_t *test );


void filterDataCorrectSize( qunittest_t *test );


void getCoefficients_getsCoefficients( qunittest_t *test );


void getCoefficients_hasZeros( qunittest_t *test );


void setCoefficients_calculatesCorrectly( qunittest_t *test );


void setCoefficients_returnsMinusOne( qunittest_t *test );


void setAllCoefficientsToOne( firFilter *filter, int order );


void bartTests( qunittest_t *test );


void hannTests( qunittest_t *test );


void hammTests( qunittest_t *test );


void blkTests( qunittest_t *test );


void addFilterTests( qtestsuite_t *testsuite );


#endif // filterTests_h
