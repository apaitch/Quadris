#ifndef _AIH__
#define _AIH__

#include <vector>
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

        std::vector<float> weights;
         
        // Evaluates the state of the board and returns a score as a function of
        // the weight.
        float evaluateBoard() const;

        // Calculates the best move to make - as a sequence of commands
        void getBestMove( std::vector<moves> & );

    public:
        Ai( QuadrisGame * , Board * );
        // Calculates the best move according to the weights, and executes the
        // appropriate commands.
        void makeNextMove( std::vector< std::string > & );
        
};

#endif
