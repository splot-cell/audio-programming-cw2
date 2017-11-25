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
double g_buf[ 130 ]; // Single buffer used to create each filter under test. Saves memory.


void resetg_buf( void ) {
    for ( int i = 0; i < 130; ++i ){
        g_buf[ i ] = rand() / RAND_MAX;
    }
}


void createFilter_returnsPointerToFilter( qunittest_t *test ) {
    firFilter *filter = createFilter( 1, g_buf );
    bool retVal = true;
    if ( filter == NULL ) {
        retVal = false;
    }
    
    destroyFilter( filter );
    
    qtest_assert_true( retVal, "Create filter returns pointer", test );
}


/* Should be implemented in integration tests */
//void destroyFilter_returnsCorrectArg( qunittest_t *test ) {
//    firFilter *filter = createFilter( 1 );
//    qtest_assert_true( destroyFilter( filter ) == 0, "Destroy filter returns 0", test );
//    
//    qtest_assert_true( destroyFilter( NULL ) == FILT_ARG_NULL, "Destroy filter returns ARG_ERR", test );
//    
//    firFilter *filter2 = createFilter( 1 );
//    double **data = getData( filter2 );
//    double *copy = *data;
//    
//    *data = NULL;
//    
//    qtest_assert_true( destroyFilter( filter2 ) == FILT_MEM_ERR, "Destroy filter returns MEM_ERR", test );
//    
//    free( copy ); // Tidy up memory leak
//    free( filter2 ); // Tidy up memory leak
//}


void filterDataCorrectSize( qunittest_t *test ) {
    const int size = 1;
    firFilter *filter = createFilter( size - 1, g_buf );
    double **data = getData( filter );
    int *order = getOrder( filter );
    
    qtest_assert_true( ( **data == 0 ), "Coeffs initialised to zero", test );
    qtest_assert_true( *order == size, "Order initialised correctly", test );
    
    bool retVal = true;
    for ( int i = 0; i < size && retVal; ++i ){
        if ( g_buf[ i ] != 0 ){
            retVal = false;
        }
    }
    qtest_assert_true( retVal, "Short delayLine initialised to zero", test );
    resetg_buf();
    
    
    destroyFilter( filter );
    
    const int size2 = 130;
    firFilter *filter2 = createFilter( size2 - 1, g_buf );
    double **data2 = getData( filter2 );
    int *order2 = getOrder( filter2 );
    
    qtest_assert_true( ( **data2 == 0 && *( *data2 + ( size2 - 1 ) ) == 0 ), "Large coeffs initialised to zero", test );
    qtest_assert_true( ( *order2 == size2 ), "Large order initialised correctly", test );
    
    retVal = true;
    for ( int i = 0; i < size2 && retVal; ++i ){
        if ( g_buf[ i ] != 0 ){
            retVal = false;
        }
    }
    qtest_assert_true( retVal, "Long delayLine initialised to zero", test );
    resetg_buf();
    
    destroyFilter( filter2 );
}


void getCoefficients_getsCoefficients( qunittest_t *test ) {
    const int size = 6;
    firFilter *filter = createFilter( size - 1, g_buf );
    double **data = getData( filter );
    
    double coeffs[ size ] = { 1, 3, 4, 6, 2, 19 };
    
    for ( int i = 0; i < size; ++i ) {
        *( *data + i ) = coeffs[ i ];
    }
    
    for ( int i = 0; i < size; ++i ) {
        char str[ 100 ];
        sprintf( str, "Retrieved coefficient %d: %f", i, getCoefficients( filter )[ i ] );
        qtest_assert_true( getCoefficients( filter )[ i ] == coeffs[ i ], str, test );
    }
    
    destroyFilter( filter );
}


void getCoefficients_hasZeros( qunittest_t *test ) {
    const int size = 19;
    firFilter *filter = createFilter( size - 1, g_buf );
    double **data = getData( filter );
    
    double coeffs[ 6 ] = { 5, 4, 5, 2, 5, 5.6 };
    double result[ 19 ] = { 5, 4, 5, 2, 5, 5.6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    for ( int i = 0; i < 6; ++i ) {
        *( *data + i ) = coeffs[ i ];
    }
    
    for ( int i = 0; i < size; ++i ) {
        char str[ 100 ];
        sprintf( str, "Retrieved coeff %d:\t%f", i, getCoefficients( filter )[ i ] );
        qtest_assert_true( getCoefficients( filter )[ i ] == result[ i ], str, test );
    }
    
    destroyFilter( filter );
}


void setCoefficients_calculatesCorrectly( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( order, g_buf );
    
    int x = setCoefficients( filter, 2000, 460, WINDOW_RECTANGULAR );
    
    qtest_assert_true( x == FILT_NO_ERR, "Set coefficients returns 0", test );
    
    double result[ order + 1 ] = { 0.030273, 0.015059, -0.033595, -0.028985, 0.036316, 0.051504, -0.038337, -0.098652, 0.039580, 0.315800, 0.460000, 0.315800, 0.039580, -0.098652, -0.038337, 0.051504, 0.036316, -0.028985, -0.033595, 0.015059, 0.030273 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Coefficient %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
    destroyFilter( filter );
}


void setCoefficients_returnsMinusOne( qunittest_t *test ) {
    int i = setCoefficients( NULL, 1, 2, WINDOW_RECTANGULAR );
    
    qtest_assert_true( i == FILT_ARG_NULL, "Set coefficients returns -1", test );
}


void setAllCoefficientsToOne( firFilter *filter, int order ) {
    double **data = getData( filter );
    
    for( int i = 0; i < order + 1; ++i ) {
        (*data)[ i ] = 1;
    }
}


void bartTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyBartlettWindow( filter );
    
    double result[] = { 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
        1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Bart window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void hannTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyHanningWindow( filter );
    
    double result[] = { 0, 0.0244717418524232, 0.0954915028125263, 0.206107373853763,
        0.345491502812526, 0.5, 0.654508497187474, 0.793892626146236,
        0.904508497187474, 0.975528258147577, 1, 0.975528258147577,
        0.904508497187474, 0.793892626146237, 0.654508497187474, 0.5,
        0.345491502812526, 0.206107373853764, 0.0954915028125263, 0.0244717418524232, 0 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Hann window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void hammTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyHammingWindow( filter );
    
    double result[] = { 0.08, 0.102514002504229, 0.167852182587524, 0.269618783945462,
        0.397852182587524, 0.54, 0.682147817412476, 0.810381216054538,
        0.912147817412476, 0.977485997495771, 1, 0.977485997495771,
        0.912147817412476, 0.810381216054538, 0.682147817412476, 0.54,
        0.397852182587524, 0.269618783945462, 0.167852182587524, 0.102514002504229, 0.08 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Hamm window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void blkTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyBlackmanWindow( filter );
    
    double result[] = { -0.0, 0.00919310140241901,
        0.0402128623625221, 0.101386014303768, 0.20077014326253, 0.34,
        0.509787137637478, 0.689171266596241, 0.849229856737469, 0.960249617697573,
        1, 0.960249617697573, 0.849229856737469, 0.689171266596241,
        0.509787137637478, 0.34, 0.200770143262531, 0.101386014303768,
        0.0402128623625221, 0.009193101402419, -0.0 };
    
    
    /* First and last coefficients are so so tiny that up to about 1% error occurs. They are therefore separate
     * from the following test loop. */
    char str[ 100 ];
    sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", 0, result[ 0 ], getCoefficients( filter )[ 0 ] );
    qtest_doubles_equal( result[ 0 ], getCoefficients( filter )[ 0 ], 1, str, test );
    
    for ( int i = 1; i < order; ++i ) {
        sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.000001, str, test );
    }
    
    sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", order, result[ order ], getCoefficients( filter )[ order ] );
    qtest_doubles_equal( result[ order ], getCoefficients( filter )[ order ], 1, str, test );
    
}


void addFilterTests( qtestsuite_t *testsuite ) {
    srand( ( unsigned int ) time( NULL ) );
    resetg_buf();
    
    qunittest_t *createFilterTest = add_qunittest( "Create/destroy filter", testsuite );
    createFilter_returnsPointerToFilter( createFilterTest );
    //destroyFilter_returnsCorrectArg( createFilterTest );
    filterDataCorrectSize( createFilterTest );
    
    qunittest_t *dataRetrivalTest = add_qunittest( "Get filter data", testsuite );
    getCoefficients_getsCoefficients( dataRetrivalTest );
    getCoefficients_hasZeros( dataRetrivalTest );
    
    qunittest_t *coefficientTests = add_qunittest( "Set coefficients", testsuite );
    setCoefficients_calculatesCorrectly( coefficientTests );
    setCoefficients_returnsMinusOne( coefficientTests );
    
    qunittest_t *windowingTests = add_qunittest( "Window weighting", testsuite );
    bartTests( windowingTests );
    hannTests( windowingTests );
    hammTests( windowingTests );
    blkTests( windowingTests );
    
    
    
}


#endif // filterTests_h
