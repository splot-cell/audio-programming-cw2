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


void isOnlyPInt_isCorrect( qunittest_t *test );


void isWav_isCorrect( qunittest_t *test );


void addUITests( qtestsuite_t *testsuite );


#endif // uiTests_h
