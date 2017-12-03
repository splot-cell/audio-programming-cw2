//
//  firfilterAdditionalTest.h
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#ifndef firfilterAdditionalTest_h
#define firfilterAdditionalTest_h


/* DELCARE DATA TYPES */

typedef enum FILTER_TYPE_ENUM filterType;

typedef enum FILTER_ERROR_ENUM firErr;


/* ADDITIONAL FUNCTION PROTOTYPES FOR TESTING */

typedef struct firfilter_struct firFilter;

double **getData( firFilter *filter );

int *getOrder( firFilter *filter );

double *getCoefficients( firFilter *filter );

void applyBartlettWindow( firFilter *filter );

void applyHanningWindow( firFilter *filter );

void applyHammingWindow( firFilter *filter );

void applyBlackmanWindow( firFilter *filter );

firErr setFilterType( firFilter *filter, filterType type );


#endif // firfilterAdditionalTest_h
