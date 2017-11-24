//
//  firfilterAdditionalTest.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef firfilterAdditionalTest_h
#define firfilterAdditionalTest_h

typedef struct firfilter_struct firFilter;

double **getData( firFilter *filter );

int *getOrder( firFilter *filter );

double *getCoefficients( firFilter *filter );

#endif // firfilterAdditionalTest_h
