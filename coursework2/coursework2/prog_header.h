//
//  prog_header.h
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#ifndef prog_header_h
#define prog_header_h

#include <stdio.h>

#include "uifunctions.h"
#include "errors.h"
#include "firfilter.h"
#include "filehandler.h"


/* FUNCTION PROTOTYPES */

/*      cleanupMemory()
 * Helper function for freeing dynamicly allocated memory at end of program */
void cleanupMemory( wav *inputFile, wav *outputFile, firFilter *filter );

#endif // prog_header_h
