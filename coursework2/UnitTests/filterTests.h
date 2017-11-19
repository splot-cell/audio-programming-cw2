//
//  filterTests.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef filterTests_h
#define filterTests_h

#include <stdbool.h>
#include "qtest/qtest/testsuite.h"
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


void destroyFilter_returnsZero( qunittest_t *test ) {
    firFilter *filter = createFilter( 1 );
    bool retVal = false;
    
    if ( destroyFilter( filter ) == 0 )
        retVal = true;
    
    qtest_assert_true( retVal, "Destroy filter returns 0", test );
}


void destroyFilter_returnsMinusOne( qunittest_t *test ) {
    bool retVal = false;
    
    if ( destroyFilter( NULL ) == -1 )
        retVal = true;
    
    qtest_assert_true( retVal, "Destroy filter returns -1", test );
}


void destroyFilter_returnsMinusTwo( qunittest_t *test ) {
    firFilter *filter = createFilter( 1 );
    double **data = getData( filter );
    double *copy = *data;
    
    *data = NULL;
    
    bool retVal = false;
    
    if ( destroyFilter( filter ) == -2 )
        retVal = true;
    
    free( copy ); // Tidy up memory leak
    free( filter ); // Tidy up memory leak
    
    qtest_assert_true( retVal, "Destroy filter returns -2", test );
}

void filterDataCorrectSize( qunittest_t *test ) {
    firFilter *filter = createFilter( 1 );
    double **data = getData( filter );
    int *order = getOrder( filter );
    
    qtest_assert_true( **data == 0 && *( *data + 5 ) == 0, "Data initialised to zero", test );
    qtest_assert_true( *order == 1, "Order initialised correctly", test );
    qtest_assert_true( *( *data + 6 ) != 0, "OOB data is not zero", test ); // Should succeed as next data is order
    
    destroyFilter( filter );
}


void addFilterTests( qtestsuite_t *testsuite ) {
    qunittest_t *createFilterTest = add_qunittest( "Create/destroy filter", testsuite );
    createFilter_returnsPointerToFilter( createFilterTest );
    destroyFilter_returnsZero( createFilterTest );
    destroyFilter_returnsMinusOne( createFilterTest );
    destroyFilter_returnsMinusTwo( createFilterTest );
    filterDataCorrectSize( createFilterTest );
    
}


#endif // filterTests_h
