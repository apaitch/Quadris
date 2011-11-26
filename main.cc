#include "QuadrisGame.h"
#include <string>
#include <cstdlib>

using namespace std;

int main( int argc , char * argv[] ) {
    bool text_only = false;
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
        else {
            cerr << "Invalid flag: " << argument << endl;
        }
    }

    QuadrisGame quadris ( text_only , seed );
    quadris.runGameLoop();
    return 0;
}
