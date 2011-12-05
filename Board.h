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
	  //store the pointer to the block which could move around

        QuadrisGame * game;
	  //pointer to the main game

        int filledCellsInRow[ numRows ]; 
	  //record the number of filled cells on each row

        Block * theBoard[ numColumns  ][ numRows ];
	  //record the pointer to block on each cell

       	//private helper function
        void removeRow( int );
	  //remove a filled row, and transfer rows above it 1 row down
	

    public:
        Board( QuadrisGame * );
        ~Board();

        Block * getActiveBlock();
 	  //get the pointer pointing to the block that can be moved

        bool setActiveBlock( Block * );
	  //set the pointer for the moving block
	  //if failed, game ends

        bool cellOccupied( Block * , std::pair<int,int> );
	  //check if the cell is occupied

        void addBlock( Block * );
  	  //called from block, add new block to the board

        void deleteBlock( Block * );
	  //called from block, delete the whole block

        void clearFilledRows();    
	  //check if any row is completely filled

        void print();
	  //print the board by text

        void draw( Xwindow * );
	  //draw the board on Xwindow
 
        friend class Ai;
};

#endif
