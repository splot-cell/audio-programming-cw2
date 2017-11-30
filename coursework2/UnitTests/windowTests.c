//
//  windowTests.c
//  coursework2
//
//  Created by Olly Seber on 30/11/2017.
//  Copyright © 2017 Olly Seber. All rights reserved.
//

#include "filterTests.h"

void bartTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyBartlettWindow( filter );
    
    double result[] = { 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
        1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Bart window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void hannTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyHanningWindow( filter );
    
    double result[] = { 0, 0.0244717418524232, 0.0954915028125263, 0.206107373853763,
        0.345491502812526, 0.5, 0.654508497187474, 0.793892626146236,
        0.904508497187474, 0.975528258147577, 1, 0.975528258147577,
        0.904508497187474, 0.793892626146237, 0.654508497187474, 0.5,
        0.345491502812526, 0.206107373853764, 0.0954915028125263, 0.0244717418524232, 0 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Hann window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void hammTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyHammingWindow( filter );
    
    double result[] = { 0.08, 0.102514002504229, 0.167852182587524, 0.269618783945462,
        0.397852182587524, 0.54, 0.682147817412476, 0.810381216054538,
        0.912147817412476, 0.977485997495771, 1, 0.977485997495771,
        0.912147817412476, 0.810381216054538, 0.682147817412476, 0.54,
        0.397852182587524, 0.269618783945462, 0.167852182587524, 0.102514002504229, 0.08 };
    
    for ( int i = 0; i < order + 1; ++i ) {
        char str[ 100 ];
        sprintf( str, "Hamm window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.00002, str, test );
    }
    
}


void blkTests( qunittest_t *test ) {
    const int order = 20;
    firFilter *filter = createFilter( 20, g_buf );
    
    setAllCoefficientsToOne( filter, order );
    
    applyBlackmanWindow( filter );
    
    double result[] = { -0.0, 0.00919310140241901,
        0.0402128623625221, 0.101386014303768, 0.20077014326253, 0.34,
        0.509787137637478, 0.689171266596241, 0.849229856737469, 0.960249617697573,
        1, 0.960249617697573, 0.849229856737469, 0.689171266596241,
        0.509787137637478, 0.34, 0.200770143262531, 0.101386014303768,
        0.0402128623625221, 0.009193101402419, -0.0 };
    
    
    /* First and last coefficients are so so tiny that up to about 1% error occurs. They are therefore separate
     * from the following test loop. */
    char str[ 100 ];
    sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", 0, result[ 0 ], getCoefficients( filter )[ 0 ] );
    qtest_doubles_equal( result[ 0 ], getCoefficients( filter )[ 0 ], 1, str, test );
    
    for ( int i = 1; i < order; ++i ) {
        sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", i, result[ i ], getCoefficients( filter )[ i ] );
        qtest_doubles_equal( result[ i ], getCoefficients( filter )[ i ], 0.000001, str, test );
    }
    
    sprintf( str, "Blackman window %d expected:\t%f\tactual:\t%f", order, result[ order ], getCoefficients( filter )[ order ] );
    qtest_doubles_equal( result[ order ], getCoefficients( filter )[ order ], 1, str, test );
    
}
