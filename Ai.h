#ifndef _AIH__
#define _AIH__

#include <vector>
#include <map>
#include <string>

class QuadrisGame;
class Board;
class Block;

/*
 * The AI, when it has control, moves blocks to their "best" position. The best
 * position is calculated based on a series of criteria.
 */
class Ai {
    private:
        enum moves { right = 0 , clockwise , drop };

        QuadrisGame * game;
        Board * board;
        Block * activeBlock;

        // Determine how important each feature is
        std::vector<float> weights;
         
        // Evaluates the state of the board and returns a score according to the
        // weights
        float evaluateBoard() const;

        void getBestMove( std::vector< moves > & );

    public:
        Ai( QuadrisGame * , Board * );
        void getNextMove( std::vector< std::string > & );
        
};

#endif
