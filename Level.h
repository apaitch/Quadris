#ifndef __LEVEL_H__aa
#define __LEVEL_H__

#include <fstream>
#include "PRNG.h"
#include "GameSettings.h"

class Block;
class Board;
class QuadrisGame;
class Xwindow;

/*
 * Encapsulates level calculations, including determining the type of the next
 * block and creating said block to pass to the Board.
 */

class Level {
    private:
        Board * board;
        QuadrisGame * game;

        int currentLevel;
        blockType nextBlockType;

        std::ifstream blockSequenceFile; 
	// Will read from "sequence.txt"

        PRNG randomNumGenerator;
	//random number generator for level 1~3

	//private helper function
        blockType numToBlockType( int ); 
	  // Like an immutable map

        int generateBlockNumber();
	  //random number generator 

        void findNextBlockType();

    public:
        Level( Board * , QuadrisGame * , int seed = -1 );
        Block * getNextBlock(); //get the pointer to next block
        int getLevel();  //get current level
        void levelUp();  //increase level by 1
        void levelDown();  //decrease level by 1
        void printNext();  // print next block in text
        void drawNext(Xwindow *); // draw next block in Xwindow 
};

#endif

