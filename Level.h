#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <fstream>
#include "PRNG.h"

class Block;
class Board;
class QuadrisGame;

class Level{
 private:
  int currentLevel; //stores the level currently in
  std::ifstream fs; //store the content in "sequence.txt"
  Board *b; //a ptr to the board
  char nextBlock; //record the type of the next block
  PRNG prng; //random number generator
  int seed;  //record the seed for PRNG
  QuadrisGame *game; //store the pointer to the game

  char numCharTransfer(int i);
  void restart();
  int numGenerator();
  void findNext(); //find the next block's type

 public:
  Level(Board *b,QuadrisGame *game);
  Block* createNew();
  int getLevel();
  void levelup();     // increase the level by 1
  void leveldown();    //decrease the level by 1
  void setSeed(int); //set new seed for PRNG

};

#endif

