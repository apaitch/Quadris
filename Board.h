#ifndef __BOARD_H__
#define __BOARD_H__

#include <utility>
#include <vector>
#include <cassert>

#include "GameSettings.h"

class Xwindow;
class Block;
class QuadrisGame;
class Ai;

class Board {
    private:
        Block * activeBlock; 
        int filledCellsInRow[ numRows ]; 
        void removeRow( int ); //remove a whole row
        Block * theBoard[ numColumns  ][ numRows ];
        QuadrisGame *game; //store the pointer to the game

    public:
        Board( QuadrisGame * );
        ~Board();
        Block * getActiveBlock();
        bool setActiveBlock( Block * );
        bool cellOccupied( Block * , std::pair<int,int> );
        void addBlock( Block * );     //add a new block
        void deleteBlock( Block * );     //remove a whole block
        void print();     //print out the board
        void draw( Xwindow * );
        void examine();    //check if any row is completely filled
 
        friend class Ai;
};

#endif
