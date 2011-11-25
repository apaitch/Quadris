#ifndef __BOARD_H__
#define __BOARD_H__

#include <utility>
#include <vector>

class Block;
class QuadrisGame;

const int num_columns = 10;
const int num_rows = 18;

class Board {
 private:
  Block *activeBlock; //store the pointer to the active block
  int rowFilled[ num_rows ]; //record the number of filled cells in each row
  void removeARow(int); //remove a whole row
  Block* blockPtr[ num_columns  ][ num_rows ];
  QuadrisGame *game; //store the pointer to the game

public:
  Board(QuadrisGame *game);
  ~Board();
  Block * getActiveBlock();
  void setActiveBlock( Block * );
  Block* getBlockPtr(std::pair<int,int>);
  void addBlock( Block * );     //add a new block
  void deleteBlock( Block * );     //remove a whole block
  void print();     //print out the board
  void examine();    //check if any row is completely filled
  
};

#endif
