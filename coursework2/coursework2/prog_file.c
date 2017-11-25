//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"


/* FUNCTION DEFINITIONS */

void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    closeFile( inputFile );
    closeFile( outputFile );
    destroyFilter( filter );
}
