//
//  prog_file.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "prog_header.h"

void cleanupMemory( audioFile *inputFile, audioFile *outputFile, firFilter *filter ) {
    destroyWav( inputFile );
    destroyWav( outputFile );
    destroyFilter( filter );
}
