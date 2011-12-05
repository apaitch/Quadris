#include <string>
#include <cstdlib>
#include <iostream>

#include "QuadrisGame.h"

using namespace std;

int main( int argc , char * argv[] ) {
    bool textOnly = false;
    bool aiOn = false;
    int seed = -1;

    bool seedNumNext = false;
    for ( int i = 1 ; i < argc ; ++i ) {
        if ( seedNumNext ) {
            seed = atoi( argv[i] );
            seedNumNext = false;
            continue;
        } // if

        string argument = argv[i];
        // Only taking into account the first appearance of any flag. Subsequent
        // appearances have no effect.
        if ( ( argument.compare("-text") == 0 ) && ( ! textOnly ) ) {
            textOnly = true;
        } // if
        else if ( ( argument.compare( "-seed" ) == 0 ) && ( seed == -1 ) ) {
            seedNumNext = true;
        } // else if
        else if ( ( argument.compare( "-ai" ) == 0 ) && ( ! aiOn ) ) {
            aiOn = true;
        } // else if
        else {
            cerr << "Invalid flag: " << argument << endl;
        } // else if
    } // for

    QuadrisGame quadris ( textOnly , aiOn , seed );
    quadris.runGameLoop();
    return 0;
} // main()
