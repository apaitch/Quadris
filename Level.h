#include "block.h"
#include "board.h"
#include <fstream>

class Level{
 private:
  int score;
  int hiscore;
  int currentLevel;
  std::ifstream fs; //store the content in "sequence.txt"
  Board *b;
  char nextBlock; //record the type of the next block

  char numCharTransfer(int i);
  void restart();
  int numGenerator();
  void findNext(); //find the next block's type

 public:
  Level(Board *b);
  Block* createNew();
  void levelup();     // increase the level by 1
  void leveldown();    //decrease the level by 1
};
