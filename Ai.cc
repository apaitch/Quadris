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
    weights.push_back(-3.5);
    // Punish holes
    weights.push_back(-7.5);
    // Reward filled rows
    weights.push_back(4.4);
    // Punish row transitions
    weights.push_back(-4.2);
    // Punish column transitions
    weights.push_back(-5.3);
    // Punish wells
    weights.push_back(-3.5);
    // Punish blockades
    weights.push_back(-4.5);

}

/*
 * Called after the AI tries a particular move to assign a score to the
 * resulting state of the board. This is the key method of the AI - the other
 * methods just set up the board appropriately.
 */
float Ai::evaluateBoard() const {

    int height = 0; 
    int filledRows = 0;
    int rowTransitions = 0;
    int columnTransitions = 0;
    // A hole is any empty space that has a block above it (since the AI doesn't
    // do tricks with sliding things in).
    int holesNum = 0;
    int wellsTotal = 0;
    int blockadesTotal = 0;

    // find height at which block landed by looking at the tallest point in the
    // block
    vector< pair<int,int> > blockPoints;
    activeBlock->getPoints( blockPoints );
    height = 0;
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        if ( ( numRows - blockPoints[i].second ) > height ) {
            height = numRows - blockPoints[i].second;
        }
    } 

    // find filled rows
    for ( int i = 0 ; i < numRows ; ++i ) {
        if ( (*board).filledCellsInRow[i] == numColumns ) {
            filledRows += 1;
        }
    }

    // find row transitions
    for ( int y = numRows - height ; y < numRows ; ++y ) {
        bool prevEmpty = ( (*board).theBoard[0][y] == 0 ) ? true : false;

        for ( int x = 1 ; x < numColumns ; ++x ) {
            bool currentEmpty = ( (*board).theBoard[x][y] == 0 ) ? true : false;
            if ( prevEmpty != currentEmpty ) {
                rowTransitions += 1;
            }
            prevEmpty = currentEmpty;
        }
    }

    int numOfEmpties = 0;
    // There can only be one well per column
    int wellDepth = 0;
    int blockadeSize = 0;
    bool blockadeStarted = false;
    // find column transitions and holes
    for ( int x = 0 ; x < numColumns ; ++x ) {
        bool prevEmpty = ( (*board).theBoard[x][numRows - 1] == 0 ) ? true : false;
        if ( prevEmpty ) {
            numOfEmpties += 1;

            // Ugly code to look left and right
            bool leftOccupied = ( x == 0 ) ? true : false;
            bool rightOccupied = ( x == numColumns  - 1 ) ? true : false;
            if ( ! leftOccupied ) {
                leftOccupied = ( (*board).theBoard[ x-1 ][numRows - 1] != 0 ) ? true : false;
            }
            if ( ! rightOccupied ) {
                rightOccupied = ( (*board).theBoard[ x+1 ][numRows - 1] != 0 ) ? true : false;
            }
            if ( leftOccupied && rightOccupied ) {
                wellDepth += 1;
            }
        }
 
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
                    leftOccupied = ( (*board).theBoard[ x-1 ][y] != 0 ) ? true : false;
                }
                if ( ! rightOccupied ) {
                    rightOccupied = ( (*board).theBoard[ x+1 ][y] != 0 ) ? true : false;
                }
                if ( leftOccupied && rightOccupied ) {
                    wellDepth += 1;
                }

            }
            else if ( blockadeStarted ) {
                blockadeSize += 1;
            }
            
            if ( prevEmpty != currentEmpty ) {
                columnTransitions += 1;
                if ( currentEmpty == false ) {
                    holesNum += numOfEmpties;
                    numOfEmpties = 0;
                    blockadeStarted = true;
                    blockadeSize += 1;
                    wellDepth = 0;
                }
            }
            prevEmpty = currentEmpty;
        }
        wellsTotal += wellDepth;
        wellDepth = 0;
        if ( blockadeStarted ) {
            blockadesTotal += blockadeSize;
        }
        blockadeSize = 0;
        blockadeStarted = false;
        numOfEmpties = 0;
    }

    float score =
        weights[0] * height +
        weights[1] * holesNum +
        weights[2] * filledRows +
        weights[3] * rowTransitions +
        weights[4] * columnTransitions +
        weights[5] * wellsTotal +
        weights[6] * blockadesTotal;

    /*cout << "Height: " << height << " holes: " << holesNum << " filled rows: " <<
filledRows << " rowTransitions: " << rowTransitions << " colTransitions: "
<< columnTransitions << " wellsTotal: " << wellsTotal << " blockadesTotal: "
<< blockadesTotal << " final score: " << score << endl;*/
    return score;
}

/*
 * Get move loops through all possible ways to drop the current active block and
 * records the sequence of moves for the best one. Called from makeNextMove().
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
    }

    float highScore = 0;
    vector<moves> movesSequence;
    int numOfCombinationsConsidered = 0;
    for ( int rotation = 0 ; rotation <= numRotations ; ++rotation ) {
        // TODO: Change Block to report success to prevent redundant
        // calculations when can't move right anymore.
        for ( int rightMove = 0 ; rightMove <= numColumns ; ++rightMove ) {
            movesSequence.clear();
            for ( int i = 0 ; i < rotation ; ++i ) {
                activeBlock->rightRotate();
                movesSequence.push_back( clockwise );
            }
            for ( int i = 0 ; i < rightMove ; ++i ) {
                activeBlock->moveRight();
                movesSequence.push_back( right );
            }
            activeBlock->drop();
            movesSequence.push_back( drop );

            board->print();
            float score = evaluateBoard();
            if ( ( highScore == 0 ) || ( score > highScore ) ) {
                highScore = score;
                bestMovesSequence = movesSequence;
            }
            numOfCombinationsConsidered += 1;
            activeBlock->reset();
        }
    }

    return;
}

/*
 * Figures out the best move to make and executes the appropriate commands. This
 * is the way for the game to call the AI!
 */
void Ai::makeNextMove( vector<string> & commands ) {
    vector<moves> bestMovesSequence;
    bestMovesSequence.clear();
    // This is the key call here.
    getBestMove( bestMovesSequence );

    commands.clear();
    for( unsigned int i = 0 ; i < bestMovesSequence.size() ; ++i ) {
        switch ( bestMovesSequence[i] ) {
            case right : 
                commands.push_back( "right" );
                break;
            case clockwise : 
                commands.push_back( "clockwise" );
                break;
            case drop :
                commands.push_back( "drop" );
                break;
        }
    }
}


