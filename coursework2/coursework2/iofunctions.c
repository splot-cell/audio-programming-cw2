//
//  iofunctions.c
//  coursework2
//
//  Created by Olly Seber on 19/11/2017.
//

#include "iofunctions.h"


/* TYPE DEFINITIONS */

typedef struct audioFile_struct {
    SNDFILE *audioFile;
    SF_INFO infoFile;
} audioFile;


/* FUNCTION DEFINITIONS */

bool isOnlyPositiveInt( const char *string ) {
    if ( !string ) {
        return false;
    }
    bool retValue = true;
    
    for ( int index = 0; index < ( strlen( string ) / sizeof( char ) ); ++index ) {
        /* If character is outside numerical digit range AND
         * the expression has not already been evaulated to false */
        if ( ( string[ index ] < 48 || 57 < string[ index ] ) && retValue ) {
            retValue = false; // Then there are some 'shady characters' in <string>.
        }
    }
    return retValue;
}


audioFile openWavRead( char *filename ) {
    audioFile *file = calloc( 1, sizeof( audioFile ) );
    if ( file == NULL ) {
        fatalError( BAD_MEMORY, "Could not allocate memory for audio file." );
    }
    file->audioFile = sf_open(<#const char *path#>, <#int mode#>, <#SF_INFO *sfinfo#>)
}
