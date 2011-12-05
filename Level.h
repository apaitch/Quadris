#ifndef __LEVEL_H__
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
        std::ifstream blockSequenceFile; // Will read from "sequence.txt"
        PRNG randomNumGenerator;

        blockType numToBlockType( int ); // Like an immutable map
        void restart();
        int generateBlockNumber();
        void findNextBlockType();

    public:
        Level( Board * , QuadrisGame * , int seed = -1 );
        Block * getNextBlock();
        int getLevel();
        void levelUp();
        void levelDown();
        void printNext();
        void drawNext(Xwindow *);
};

#endif

