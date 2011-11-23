#include <utility>
#include "block.h"

class board{
 private:
  Block *activeBlock; //store the pointer to the active block
  int rowFilled[15]; //record the number of filled cells in each row
  void removeARow(int); //remove a whole row
  Block* blockPtr[15][10];

public:
  board();
  ~board();
  Block* getBlockPtr(std::pair<int,int>);
  void addBlock(std::pair<int,int> *a,block*);     //add a new block
  void deleteBlock(std::pair<int,int> *b);     //remove a whole block (4 cells)
  void print();     //print out the board
  void examine();    //check if any row is completely filled
  
};
