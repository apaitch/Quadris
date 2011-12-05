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
        QuadrisGame * game;

        int filledCellsInRow[ numRows ]; 
        Block * theBoard[ numColumns  ][ numRows ];

        void removeRow( int );

    public:
        Board( QuadrisGame * );
        ~Board();

        Block * getActiveBlock();
        bool setActiveBlock( Block * );

        bool cellOccupied( Block * , std::pair<int,int> );
        void addBlock( Block * );
        void deleteBlock( Block * );
        void clearFilledRows();    //check if any row is completely filled

        void print();
        void draw( Xwindow * );
 
        friend class Ai;
};

#endif
