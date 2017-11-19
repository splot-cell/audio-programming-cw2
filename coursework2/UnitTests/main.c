#include <stdio.h>
#include "qtest/testsuite.h"
#include "filterTests.h"

int main() {
    qtestsuite_t * testsuite = create_qtestsuite( "Test Suite" );
    
    addFilterTests( testsuite );
    
    fprint_qtestsuite( stdout, testsuite );
    getc( stdin );
    return 0;
}
