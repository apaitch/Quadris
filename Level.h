#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <fstream>
#include "PRNG.h"
#include "GameSettings.h"

class Block;
class Board;
class QuadrisGame;
class Xwindow;

class Level {
    private:
        int currentLevel; //stores the level currently in
        blockType nextBlockType; //record the type of the next block
        std::ifstream blockSequenceFile; //store the content in "sequence.txt"
        PRNG randomNumGenerator;
        Board * board;
        QuadrisGame * game;

        blockType numToType( int );
        void restart();
        int generateNumber();
        void findNextBlockType();

    public:
        Level( Board * , QuadrisGame * , int seed = -1 );
        Block * getNextBlock();
        int getLevel();
        void levelUp();     // increase the level by 1
        void levelDown();    //decrease the level by 1
        void printNext();  //print the type of next block
        void drawNext(Xwindow *); //draw the next block in window
};

#endif

