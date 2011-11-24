#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <fstream>
#include "PRNG.h"

class Block;
class Board;

class Level{
 private:
  int score;
  int hiscore;
  int currentLevel;
  std::ifstream fs; //store the content in "sequence.txt"
  Board *b;
  char nextBlock; //record the type of the next block
  PRNG prng;
  int seed;  //record the seed for PRNG

  char numCharTransfer(int i);
  void restart();
  int numGenerator();
  void findNext(); //find the next block's type

 public:
  Level(Board *b);
  Block* createNew();
  int getLevel();
  void levelup();     // increase the level by 1
  void leveldown();    //decrease the level by 1
  void setSeed(int); //set new seed for PRNG

};

#endif

