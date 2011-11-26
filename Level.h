#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <fstream>
#include "PRNG.h"

class Block;
class Board;
class QuadrisGame;

const int max_level = 3;
const int min_level = 0;

class Level{
 private:
  int currentLevel; //stores the level currently in
  std::ifstream fs; //store the content in "sequence.txt"
  Board *b; //a ptr to the board
  char nextBlock; //record the type of the next block
  PRNG prng; //random number generator
  //int seed;  //record the seed for PRNG
  QuadrisGame *game; //store the pointer to the game

  char numCharTransfer(int i);
  void restart();
  int numGenerator();
  void findNext(); //find the next block's type

 public:
  Level( Board * , QuadrisGame * , int seed = -1 );
  Block* createNew();
  int getLevel();
  void levelup();     // increase the level by 1
  void leveldown();    //decrease the level by 1
  void printNext();  //print the type of next block

};

#endif

