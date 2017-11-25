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

#include "iofunctions.h"
#include "firfilter.h"
#include "errors.h"


/* FUNCTION PROTOTYPES */

/*      cleanupMemory()
 * Helper function for freeing dynamicly allocated memory at end of program. */
void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter );


#endif // prog_header_h
