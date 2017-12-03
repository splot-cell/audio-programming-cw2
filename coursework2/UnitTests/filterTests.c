//
//  filterTests.c
//  coursework2
//
//  Created by Olly Seber on 30/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "filterTests.h"


/* GLOBALS */

double g_buf[ 130 ];


/* FUNCTION DEFINITIONS */

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


void processingTests( qunittest_t *test ) {
    const int order = 9;
    firFilter *filter = createFilter( order, g_buf );
    const int bufSize = 20;
    double inBuffer[ bufSize ] = { 1, 0 };
    double coeffs[] = { 12, 13, 10, 20, 5.4, 32, 0.04, 10, 1, 0.4, 9 };
    double **data = getData( filter );
    
    for ( int i = 0; i < 6; ++i ) {
        *( *data + i ) = coeffs[ i ];
    }
    
    for ( int i = 0; i < bufSize; ++i ) {
        printf( "%f\n", inBuffer[ i ] );
    }
    
    int x = processBuffer( filter, inBuffer, bufSize );
    
    qtest_assert_true( x == bufSize, "Process buffer correct count", test );
    
    for ( int i = 0; i < order + 1; ++i ) {
        qtest_assert_true( inBuffer[ i ] == coeffs[ i ], "Diract samples correct", test );
    }
    
    destroyFilter( filter );
}


void addFilterTests( qtestsuite_t *testsuite ) {
    srand( ( unsigned int ) time( NULL ) );
    resetg_buf();
    
    qunittest_t *createFilterTest = add_qunittest( "Create/destroy filter", testsuite );
    createFilter_returnsPointerToFilter( createFilterTest );
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
    
    qunittest_t *processing = add_qunittest( "Processing buffer", testsuite );
    processingTests( processing );
}
