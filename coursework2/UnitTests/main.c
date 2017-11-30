//
//  main.c
//  Unit Tests - coursework2
//
//  Created by Olly Seber on 18/11/2017.
//

#include "test_header.h"


int main() {
    
    initMemTracking();
    if ( initFiltErrHandling() != FILT_NO_ERR ) {
        programExit( BAD_FILE_OPEN, "Error initialising filter memory temporary file." );
    }
//    initFileTracking();
    
    
    qtestsuite_t * testsuite = create_qtestsuite( "Test Suite" );
    
    addFilterTests( testsuite );
    addUITests( testsuite );
    
    fprint_qtestsuite( stdout, testsuite );
    
    return 0;
}
