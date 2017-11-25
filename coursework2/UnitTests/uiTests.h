//
//  uiTests.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef uiTests_h
#define uiTests_h

#include "iofunctions.h"
#include "qtest/testsuite.h"


void isOnlyPInt_isCorrect( qunittest_t *test ) {
    char *testString = "1234567890";
    qtest_assert_true( isOnlyPositiveInt( testString ), "Identifies all numerical digits", test );
    
    char *testString2 = "12345.67890";
    qtest_assert_true( !isOnlyPositiveInt( testString2 ), "Identifies decimal point", test );
    
    char *testString3 = "-12";
    qtest_assert_true( !isOnlyPositiveInt( testString3 ), "Identifies negative", test );
    
    char *testString4 = "1 2";
    qtest_assert_true( !isOnlyPositiveInt( testString4 ), "Identifies space", test );
    
    char *testString5 = "143k2";
    qtest_assert_true( !isOnlyPositiveInt( testString5 ), "Identifies letter", test );
    
    char *testString6 = "143\n";
    qtest_assert_true( !isOnlyPositiveInt( testString6 ), "Identifies newline", test );
    
    char *testString7 = "143\03";
    qtest_assert_true( !isOnlyPositiveInt( testString7 ), "Identifies null", test );
    
    char *testString8 = "13\\";
    qtest_assert_true( !isOnlyPositiveInt( testString8 ), "Identifies slash", test );
    
    char *testString9 = "13 ";
    qtest_assert_true( !isOnlyPositiveInt( testString9 ), "Identifies space at end", test );
}


void addUITests( qtestsuite_t *testsuite ) {
    qunittest_t *isOnlyInt = add_qunittest( "Only integer santiation", testsuite );
    isOnlyPInt_isCorrect( isOnlyInt );
}


#endif // uiTests_h
