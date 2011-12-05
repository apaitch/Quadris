#include <string>
#include <cstdlib>
#include <iostream>

#include "QuadrisGame.h"

using namespace std;

int main( int argc , char * argv[] ) {
    bool textOnly = false;
    int aiLevel = -1;
    int seed = -1;

    bool seedNumNext = false;
    bool aiLevelNext = false;
    for ( int i = 1 ; i < argc ; ++i ) {
        if ( seedNumNext ) {
            seed = atoi( argv[i] );
            seedNumNext = false;
            continue;
        } // if
        else if ( aiLevelNext ) {
            aiLevel = atoi( argv[i] );
            aiLevelNext = false;        
            continue;
        }

        string argument = argv[i];
        // Only taking into account the first appearance of any flag. Subsequent
        // appearances have no effect.
        if ( ( argument.compare("-text") == 0 ) && ( ! textOnly ) ) {
            textOnly = true;
        } // if
        else if ( ( argument.compare( "-seed" ) == 0 ) && ( seed == -1 ) ) {
            seedNumNext = true;
        } // else if
        else if ( ( argument.compare( "-ai" ) == 0 ) && ( aiLevel == -1 ) ) {
            aiLevelNext = true;
        } // else if
        else {
            cerr << "Invalid flag: " << argument << endl;
        } // else if
    } // for

    if ( seedNumNext || aiLevelNext ) {
        cerr << "ERROR: no numeric argument for -seed or -ai flags" << endl;
        exit(1);
    }

    QuadrisGame quadris ( textOnly , aiLevel , seed );
    quadris.runGameLoop();
    return 0;
} // main()
