#include <iostream>
#include "Board.h"
#include "Block.h"
#include "Colours.h"
#include "QuadrisGame.h"
#include "XWindow.h"

using namespace std;

Board::Board( QuadrisGame *game ):game( game ) {
    for ( int x = 0 ; x < numColumns ; ++x ) {
        for ( int y = 0 ; y < numRows ; ++y ) {
            theBoard[ x ][ y ] = 0; 
        } // for
    } // for

    //initiate the number of filled cells to be zero
    //for all rows
    for ( int y = 0 ; y < numRows ; ++y ) {
        filledCellsInRow[ y ] = 0;
    } // for
} // Board()

Board::~Board() {
    //examines each cell, if not Null, delete the cell
    //if a block is totally deleted, 
    //release the memory for the block
    for ( int x = 0 ; x < numColumns ; ++x ) {
        for ( int y = 0 ; y < numRows ; ++y ) {
            if ( theBoard[ x ][ y ] != 0 ) {
                bool removed = theBoard[ x ][ y ]->deleteCell();
                if ( removed ) {
                    delete theBoard[ x ][ y ];
                } // if
            } // if
        } // for
    } // for
} // ~Board()

Block * Board::getActiveBlock() {
    return activeBlock;
} // getActiveBlock()

/*
 * Sets the given block to be the active block. The active block is the one the
 * user/AI controls.
 *
 * Returns true if setting the block as the active block results in a collusion.
 */
bool Board::setActiveBlock( Block * block ) {
    vector< pair<int , int> > blockPoints;
    block->getPoints( blockPoints );
    bool gameOver = false;
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        if ( cellOccupied( block , blockPoints[ i ] ) ) {
            gameOver = true;
            break;
        } // if
    } // for

    if ( ! gameOver ) {
        activeBlock = block;
        addBlock ( activeBlock );
    } // if
    else {
        // If the game is over, need to delete the given block explicitly since
        // it's not on the board.
        delete block;
    }
    return gameOver;
} // setActiveBlock()

/* 
 * Returns true if the specified cell is occupied by a block different from the
 * specified block.
 */
bool Board::cellOccupied( Block * block , pair<int,int> point ) {
    bool occupied = true;

    if ( point.first >= 0 && point.first < numColumns &&
         point.second >= 0 && point.second < numRows ) {  
        if ( theBoard[ point.first ][ point.second ] == 0 ||
             theBoard[ point.first ][ point.second ] == block ) {
            occupied = false;
        } // if
    } // if

    return occupied;
} // cellOccupied()

/*
 * Adds a pointer to the specified block at the cells that match the coordinates
 * of the points in the block. This is to add a block to the board for
 * drawing/calculations.
 */
void Board::addBlock( Block * block ) {
    vector< pair<int , int> > blockPoints;
    block->getPoints( blockPoints );

    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        int x = blockPoints[ i ].first;
        int y = blockPoints[ i ].second;
        theBoard[ x ][ y ] = block;
        filledCellsInRow[ y ] += 1;
    } // for
} // addBlock()

/*
 * Sets the cells that match the coordinates of the points in the block to 0.
 * This is to remove a block from the board for drawing/calculation purposes.
 * NOTE: this does NOT delete the block from memory.
 */
void Board::deleteBlock( Block * block ) {
    vector< pair<int , int> > blockPoints;
    block->getPoints( blockPoints );

    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        int x = blockPoints[ i ].first;
        int y = blockPoints[ i ].second;
        theBoard[ x ][ y ] = 0;
        filledCellsInRow[ y ] -= 1;
    } // for
} // deleteBlock()

void Board::draw( Xwindow * window ) {
    //find out how much the block could drop
    int maxDrop = activeBlock->getDropAmount();

    //find out the current position of the current block
    vector< pair< int , int > > shadowPos;
    activeBlock->getPoints( shadowPos );
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        shadowPos[ i ].second += maxDrop;
    }

    for ( int x = 0 ; x < numColumns ; ++x ) {
        for ( int y = 0 ; y < numRows ; ++y ) {
            if ( theBoard[ x ][ y ] != 0 ) {
                window->fillBorderedRectangle( 
                            boardPosn.first + x * blockWidth ,
                            boardPosn.second + y * blockHeight ,
                            blockWidth , blockHeight ,
                            theBoard[ x ][ y ]->getColour() );
            } // if
            else if ( ( x == shadowPos[0].first && y == shadowPos[0].second ) ||
                      ( x == shadowPos[1].first && y == shadowPos[1].second ) ||
                      ( x == shadowPos[2].first && y == shadowPos[2].second ) ||
                      ( x == shadowPos[3].first && y == shadowPos[3].second ) ) {
                window->fillBorderedRectangle( 
                            boardPosn.first + x * blockWidth ,
                            boardPosn.second + y * blockHeight ,
                            blockWidth , blockHeight ,
                            Grey );
            } // else if
        } // for
    } // for
} // draw()

/*
 * Outputs the state of the board to stdout.
 */
void Board::print() {
    // Find out how much the block could drop
    int maxDrop = activeBlock->getDropAmount();

    // Find out the position of the current block
    // if it was to be dropped.
    vector< pair< int , int > > shadowPos;
    activeBlock->getPoints( shadowPos );
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        shadowPos[ i ].second += maxDrop;
    } // for

    for ( int y = 0 ; y < numRows ; ++y ) {
        for ( int x = 0 ; x < numColumns ; ++x ) {
            if ( theBoard[ x ][ y ] == 0 ) {
                if ( ( x == shadowPos[0].first && y == shadowPos[0].second ) ||
                     ( x == shadowPos[1].first && y == shadowPos[1].second ) ||
                     ( x == shadowPos[2].first && y == shadowPos[2].second ) ||
                     ( x == shadowPos[3].first && y == shadowPos[3].second ) ) {
                    cout << "*";
                } // if
                else {
                    cout << " ";
                } // else
            } // if
            else {
                cout << theBoard[x][y]->getType();
            } // else
        } // for
        cout << endl;
    } // for
} // print()

void Board::clearFilledRows() {
    int rowsRemoved = 0;
    for ( int y = 0 ; y < numRows ; ++y ) {
        if ( filledCellsInRow[ y ] == numColumns ) {
            rowsRemoved += 1;
            this->removeRow( y );
        } // if
    } // for
    if ( rowsRemoved > 0 ) {
        game->lineCleared( rowsRemoved );
    } // if
} // clearFilledRows()

/*
 * Removes the specified row from the board and shifts all the rows above
 * downwards.
 */
void Board::removeRow( int rowToRm ) {
    if ( rowToRm < numRows ) {

        // If a block is completely removed, need to delete the block from
        // memory and report the score.
        for ( int x = 0 ; x < numColumns ; ++x ) {
            bool blockGone = theBoard[ x ][ rowToRm ]->deleteCell();
            if ( blockGone ) {
                game->blockCleared( theBoard[ x ][ rowToRm ]->getLevel() );
                delete theBoard[ x ][ rowToRm ];
            } // if
        } // for

        // Shift the rows above downwards
        for ( int y = rowToRm ; y > 0 ; --y ) {
            for ( int x = 0 ; x < numColumns ; ++x ) {
                theBoard[ x ][ y ] = theBoard[ x ][ y - 1 ];
            } // for
            filledCellsInRow[ y ] = filledCellsInRow[ y - 1 ];
        } // for

        // Make sure the first row is filled with 0's
        for ( int x = 0 ; x < numColumns ; ++x ) {
            theBoard[ x ][ 0 ] = 0;
        } // for
        filledCellsInRow[ 0 ] = 0;
    } // if
} // removeRow()

