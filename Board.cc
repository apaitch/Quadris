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
    //examine each cell, if not Null, delete the cell
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
    return gameOver;
} // setActiveBlock()

// Checks if the specified cell is occupied by a block different from the
// specified block.
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

void Board::deleteBlock( Block * block ) {
    vector< pair<int , int> > blockPoints;
    block->getPoints( blockPoints );

    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        int x = blockPoints[ i ].first;
        int y = blockPoints[ i ].second;
        theBoard[ x ][ y ] = 0;
        filledCellsInRow[ y ] -= 1;
    } // for
}

void Board::draw( Xwindow * window ) {
    //find out how much the block could drop
    int maxDrop = activeBlock->calculateDrop();

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

void Board::print() {
    //find out how much the block could drop
    int maxDrop = activeBlock->calculateDrop();

    //find out the current position of the current block
    vector< pair< int , int > > shadowPos;
    activeBlock->getPoints( shadowPos );
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        shadowPos[ i ].second += maxDrop;
    }

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

void Board::examine() {
    int numRemovedRow = 0; //record how many rows have be removed
    for ( int y = 0 ; y < numRows ; ++y ) {
        if ( filledCellsInRow[ y ] == numColumns ) {
            numRemovedRow += 1;
            this->removeRow( y );
        } // if
    } // for
    if ( numRemovedRow > 0 ) {
        game->lineCleared( numRemovedRow );
    } // if
} // examine()

void Board::removeRow( int rowToRm ) {
    assert( rowToRm < numRows );
    int cellLevel;  //record the level of the removed block

    //call the blocks of row i to delete a cell
    for ( int x = 0 ; x < numColumns ; ++x ) {
        bool removed = theBoard[ x ][ rowToRm ]->deleteCell();
        if ( removed ) {
            cellLevel = theBoard[ x ][ rowToRm ]->getLevel();
            game->blockCleared( cellLevel );
            delete theBoard[ x ][ rowToRm ];
        } // if
    } // for

    //shift the rows above row i downwards
    for ( int y = rowToRm ; y > 0 ; --y ) {
        for ( int x = 0 ; x < numColumns ; ++x ) {
            theBoard[ x ][ y ] = theBoard[ x ][ y - 1 ];
        } // for
        //the record for the # of cells filled for each row
        //should also be shifted
        filledCellsInRow[ y ] = filledCellsInRow[ y - 1 ];
    } // for

    //the first row is filled by 0 cells
    filledCellsInRow[ 0 ] = 0;

    //for the up most row, NULL will be refilled
    for ( int x = 0 ; x < numColumns ; ++x ) {
        theBoard[ x ][ 0 ] = 0;
    } // for
} // removeRow()

