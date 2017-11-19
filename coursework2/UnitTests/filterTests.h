//
//  filterTests.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef filterTests_h
#define filterTests_h

#include <stdbool.h>
#include "qtest/testsuite.h"
#include "firfilter.h"
#include <stdlib.h>

void createFilter_returnsPointerToFilter( qunittest_t *test ) {
    firFilter *filter = createFilter( 1 );
    bool retVal = true;
    if ( filter == NULL ) {
        retVal = false;
    }
    
    destroyFilter( filter );
    
    qtest_assert_true( retVal, "Create filter returns pointer", test );
}


void destroyFilter_returnsCorrectArg( qunittest_t *test ) {
    firFilter *filter = createFilter( 1 );
    qtest_assert_true( destroyFilter( filter ) == 0, "Destroy filter returns 0", test );
    
    qtest_assert_true( destroyFilter( NULL ) == -1, "Destroy filter returns -1", test );
    
    firFilter *filter2 = createFilter( 1 );
    double **data = getData( filter2 );
    double *copy = *data;
    
    *data = NULL;
    
    qtest_assert_true( destroyFilter( filter2 ) == -2, "Destroy filter returns -2", test );
    
    free( copy ); // Tidy up memory leak
    free( filter2 ); // Tidy up memory leak
}


void filterDataCorrectSize( qunittest_t *test ) {
    const int size = 1;
    firFilter *filter = createFilter( size - 1 );
    double **data = getData( filter );
    int *order = getOrder( filter );
    
    qtest_assert_true( ( **data == 0 ), "Data initialised to zero", test );
    qtest_assert_true( *order == size, "Order initialised correctly", test );
    
    destroyFilter( filter );
    
    const int size2 = 130;
    firFilter *filter2 = createFilter( size2 - 1 );
    double **data2 = getData( filter2 );
    int *order2 = getOrder( filter2 );
    
    qtest_assert_true( ( **data2 == 0 && *( *data2 + ( size2 - 1 ) ) == 0 ), "Large data initialised to zero", test );
    qtest_assert_true( ( *order2 == size2 ), "Large order initialised correctly", test );
    
    destroyFilter( filter2 );
}


void getCoefficients_getsCoefficients( qunittest_t *test ) {
    const int size = 6;
    firFilter *filter = createFilter( size - 1 );
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
}


void getCoefficients_hasZeros( qunittest_t *test ) {
    const int size = 19;
    firFilter *filter = createFilter( size - 1 );
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
}

//void setCoefficients_calculatesCorrectly( qunittest_t *test ) {
//    const int size =
//}


void addFilterTests( qtestsuite_t *testsuite ) {
    qunittest_t *createFilterTest = add_qunittest( "Create/destroy filter", testsuite );
    createFilter_returnsPointerToFilter( createFilterTest );
    destroyFilter_returnsCorrectArg( createFilterTest );
    filterDataCorrectSize( createFilterTest );
    
    qunittest_t *dataRetrivalTest = add_qunittest( "Set/get filter data", testsuite );
    getCoefficients_getsCoefficients( dataRetrivalTest );
    getCoefficients_hasZeros( dataRetrivalTest );
}


#endif // filterTests_h
