//
//  main.c
//  Unit Tests - coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "test_header.h"

#include "errors.h"

int main() {
        
    qtestsuite_t * testsuite = create_qtestsuite( "Test Suite" );
    
    addFilterTests( testsuite );
    addUITests( testsuite );
    
    fprint_qtestsuite( stdout, testsuite );
    
    return 0;
}
