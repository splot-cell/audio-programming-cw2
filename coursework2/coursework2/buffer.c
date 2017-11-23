//
//  buffer.c
//  coursework2
//
//  Created by Olly Seber on 23/11/2017.
//

#include "buffer.h"

typedef struct buffer_struct {
    double *data; // Pointer to array of data
    int size; // Size of buffer
    int readIndex;
    int writeIndex;
} buffer;
