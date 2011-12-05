#include "Ai.h"
#include "QuadrisGame.h"
#include "GameSettings.h"
#include "Board.h"
#include "Block.h"
#include <cassert>

using namespace std;

Ai::Ai( QuadrisGame * game , Board * board ) : game( game ) ,
                                               board( board ) ,
                                               activeBlock( 0 ) {
    // Punish height at which block lands
    weights.push_back(-4.5);
    // Punish holes
    weights.push_back(-4.5);
    // Reward filled rows
    weights.push_back(5.4);
    // Punish row transitions
    weights.push_back(-4.2);
    // Punish column transitions
    weights.push_back(-5.3);
    // Punish wells
    weights.push_back(-3.5);
    // Punish blockades
    weights.push_back(-7.5);
} // Ai()

/*
 * Called after the AI tries a particular move. Assign a score to the
 * resulting state of the board. This is the key method of the AI
 */
float Ai::evaluateBoard() const {

    // These are the measures which, in combinations with weights, determine the
    // score for a game state. The rest of the code finds these values and
    // calculates said score.
    int height = 0; 
    int filledRows = 0;
    int rowTransitions = 0;
    int columnTransitions = 0;
    int holesNum = 0;
    int wellsTotal = 0;
    int blockadesTotal = 0;

    // Find height at which block landed. Takes maximum of points in block. 
    vector< pair<int,int> > blockPoints;
    activeBlock->getPoints( blockPoints );
    height = 0;
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        if ( ( numRows - blockPoints[i].second ) > height ) {
            height = numRows - blockPoints[i].second ;
        } // if
    } // for 

    // Find filled rows
    for ( int i = 0 ; i < numRows ; ++i ) {
        if ( (*board).filledCellsInRow[i] == numColumns ) {
            filledRows += 1;
        } // if
    } // for

    // Find row transitions
    for ( int y = numRows - height ; y < numRows ; ++y ) {
        bool prevEmpty = ( (*board).theBoard[0][y] == 0 ) ? true : false;

        for ( int x = 1 ; x < numColumns ; ++x ) {
            bool currentEmpty = ( (*board).theBoard[x][y] == 0 ) ? true : false;
            if ( prevEmpty != currentEmpty ) {
                rowTransitions += 1;
            } // if
            prevEmpty = currentEmpty;
        } // for
    } // for

    int numOfEmpties = 0;
    int wellDepth = 0;
    int blockadeSize = 0;
    bool blockadeStarted = false;

    // Find column transitions, holes, wells, and blockades
    for ( int x = 0 ; x < numColumns ; ++x ) {
        bool prevEmpty = ( (*board).theBoard[x][numRows - 1] == 0 ) ? true : false;
        if ( prevEmpty ) {
            numOfEmpties += 1;

            // Ugly code to look left and right
            bool leftOccupied = ( x == 0 ) ? true : false;
            bool rightOccupied = ( x == numColumns  - 1 ) ? true : false;
            if ( ! leftOccupied ) {
                leftOccupied = ( (*board).theBoard[ x-1 ][numRows - 1] != 0 ) 
                                    ? true : false;
            } // if
            if ( ! rightOccupied ) {
                rightOccupied = ( (*board).theBoard[ x+1 ][numRows - 1] != 0 ) 
                                    ? true : false;
            } // if
            if ( leftOccupied && rightOccupied ) {
                wellDepth += 1;
            } // if
        } // if
 
        for ( int y = numRows - 2 ; y > 0 ; --y ) {
            bool currentEmpty = ( (*board).theBoard[x][y] == 0 ) ? true : false;
            if ( currentEmpty ) {
                
                numOfEmpties += 1;
                blockadeStarted = false;
                blockadesTotal += blockadeSize;
                blockadeSize = 0;

                // Look left and right
                bool leftOccupied = ( x == 0 ) ? true : false;
                bool rightOccupied = ( x == numColumns  - 1 ) ? true : false;
                if ( ! leftOccupied ) {
                    leftOccupied = ( (*board).theBoard[ x-1 ][y] != 0 ) 
                                        ? true : false;
                } // if
                if ( ! rightOccupied ) {
                    rightOccupied = ( (*board).theBoard[ x+1 ][y] != 0 ) 
                                        ? true : false;
                } // if
                if ( leftOccupied && rightOccupied ) {
                    wellDepth += 1;
                } // if

            } // if
            else if ( blockadeStarted ) {
                blockadeSize += 1;
            } // else if
            
            if ( prevEmpty != currentEmpty ) {
                columnTransitions += 1;
                if ( currentEmpty == false ) {
                    holesNum += numOfEmpties;
                    numOfEmpties = 0;
                    blockadeStarted = true;
                    blockadeSize += 1;
                    wellDepth = 0;
                } // if
            } // if
            prevEmpty = currentEmpty;
        } // for
        wellsTotal += wellDepth;
        wellDepth = 0;
        if ( blockadeStarted ) {
            blockadesTotal += blockadeSize;
        } // if
        blockadeSize = 0;
        blockadeStarted = false;
        numOfEmpties = 0;
    } // for

    float score =
        weights[0] * height +
        weights[1] * holesNum +
        weights[2] * filledRows +
        weights[3] * rowTransitions +
        weights[4] * columnTransitions +
        weights[5] * wellsTotal +
        weights[6] * blockadesTotal;

    return score;
} // evaluateBoard()

/*
 * Loops through all possible ways to drop the current active block and
 * records the sequence of moves for the best one. Called from getNextMove().
 * 
 * Note: Assuming block is always at original position - 0 , 3. This should
 * be true because there is no alternating human/AI control - it's either
 * all AI or all player.
 */
void Ai::getBestMove( vector<moves> & bestMovesSequence ) {
    activeBlock = board->getActiveBlock();

    int numRotations = 0;
    switch ( activeBlock->getType() ) {
        case 'O' :
            numRotations = 0;
            break;
        case 'Z' : case 'S' : case 'I' :
            numRotations = 1;
            break;
        case 'T' : case 'L' : case 'J' :
            numRotations = 3;
    } // switch

    float highScore = 0;
    vector<moves> movesSequence;

    for ( int rotation = 0 ; rotation <= numRotations ; ++rotation ) {
        for ( int rightMove = 0 ; rightMove <= numColumns ; ++rightMove ) {
            movesSequence.clear();
            for ( int i = 0 ; i < rotation ; ++i ) {
                activeBlock->rightRotate();
                movesSequence.push_back( clockwise );
            } // for
            for ( int i = 0 ; i < rightMove ; ++i ) {
                activeBlock->moveRight();
                movesSequence.push_back( right );
            } // for
            activeBlock->drop();
            movesSequence.push_back( drop );

            board->print();
            float score = evaluateBoard();
            if ( ( highScore == 0 ) || ( score > highScore ) ) {
                highScore = score;
                bestMovesSequence = movesSequence;
            } // if
            activeBlock->reset();
        } // for
    } // for
} // getBestMove()

/*
 * Fills the given vector with the commands required for the best move. It's up
 * to the caller to actually execute the commands.
 *
 * NOTE: USES DEFAULT COMMANDS!
 */
void Ai::getNextMove( vector<string> & commands ) {
    // Uses getBestMove() to get a sequence of moves as integers, and then
    // converts the integers to strings. This is to avoid pesky string
    // manipulation.
    vector< moves > bestMovesSequence;
    bestMovesSequence.clear();
    getBestMove( bestMovesSequence );

    commands.clear();
    for( unsigned int i = 0 ; i < bestMovesSequence.size() ; ++i ) {
        switch ( bestMovesSequence[ i ] ) {
            case right : 
                commands.push_back( "right" );
                break;
            case clockwise : 
                commands.push_back( "clockwise" );
                break;
            case drop :
                commands.push_back( "drop" );
                break;
        } // switch
    } // for
} // getNextMove()


