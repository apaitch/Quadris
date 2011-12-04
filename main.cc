#include "QuadrisGame.h"
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

int main( int argc , char * argv[] ) {
    bool text_only = false;
    bool ai_on = false;
    int seed = -1;

    bool seed_num_next = false;
    for ( int i = 1 ; i < argc ; ++i ) {
        if ( seed_num_next ) {
            seed = atoi( argv[i] );
            seed_num_next = false;
            continue;
        }

        string argument = argv[i];
        // Only taking into account the first appearance of any flag. Subsequent
        // appearances have no effect.
        if ( ( argument.compare("-text") == 0 ) && ( ! text_only ) ) {
            text_only = true;
        }
        else if ( ( argument.compare( "-seed" ) == 0 ) && ( seed == -1 ) ) {
            seed_num_next = true;
        }
        else if ( ( argument.compare( "-ai" ) == 0 ) && ( ! ai_on ) ) {
            ai_on = true;
        }
        else {
            cerr << "Invalid flag: " << argument << endl;
        }
    }

    QuadrisGame quadris ( text_only , ai_on , seed );
    quadris.runGameLoop();
    return 0;
}
