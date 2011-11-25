#include <iostream>
#include "Board.h"
#include "Block.h"
#include "QuadrisGame.h"
using namespace std;

Board::Board(QuadrisGame *game):game(game)
{
  //initial all block pointers to NULL
  for(int x=0;x<num_columns;++x)
    {
      for(int y=0;y<num_rows;++y)
	blockPtr[x][y]=NULL;
    }
  
  //initiate the number of filled cells to be zero
  //for all rows
  for(int y=0;y<num_rows;++y)
    rowFilled[y]=0;
}

Board::~Board() {
  //examine each cell, if not Null, delete the cell
  //if a block is totally deleted, 
  //release the memory for the block
  for ( int x = 0 ; x < num_columns ; ++x ) {
    for ( int y = 0 ; y < num_rows ; ++y ) {
      if(blockPtr[x][y]!=NULL){
	bool removed=blockPtr[x][y]->deleteCell();
	if(removed)
	  {
	    delete blockPtr[x][y];
	  }
      }
    }
  }
}

Block * Board::getActiveBlock() {
    return activeBlock;
}

bool Board::setActiveBlock( Block * b ) {
    vector< pair<int , int> > block_points;
    b->getPoints( block_points );
    bool game_over = false;
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        if ( cellOccupied( b , block_points[i] ) ) {
            game_over = true;
            break;
        }
    }

    if ( ! game_over ) {
        activeBlock = b;
        addBlock ( activeBlock );
    }
    return game_over;
}

// Checks if the specified cell is occupied by a block different from the
// specified block.
bool Board::cellOccupied( Block * block , pair<int,int> point )
{
    bool occupied = true;
    if ( point.first >= 0 && point.first < num_columns
         && point.second >= 0 && point.second < num_rows ) {  
        if ( blockPtr[point.first][point.second] == 0 ||
             blockPtr[point.first][point.second] == block ) {
            occupied = false;
        }
    }

    return occupied;
}

void Board::addBlock( Block * b )
{
    vector< pair<int , int> > block_points;
    b->getPoints( block_points );

  for(int i=0;i<points_per_block;++i)
    {
      int x=block_points[i].first;
      int y=block_points[i].second;
      blockPtr[x][y]=b;
      rowFilled[y]+=1;
    }
}

void Board::deleteBlock( Block * b ) //used when delete the whole block
{
    vector< pair<int , int> > block_points;
    b->getPoints( block_points );

  for(int i=0;i<points_per_block;++i)
    {
      int x=block_points[i].first;
      int y=block_points[i].second;
      blockPtr[x][y]=0;
      rowFilled[y]-=1;
    }
}

void Board::print()
{
  for(int y=0;y<num_rows;++y)
    {
      for(int x=0;x<num_columns;++x)
	{
          if ( blockPtr[x][y] == 0 ) {
            cout << " ";
          }
          else {
	    cout << blockPtr[x][y]->getType();
          }
	}
      cout<<endl;
    }
}

void Board::examine()
{
  int numRemovedRow=0; //record how many rows have be removed
  for(int y=0;y<num_rows;++y)
    {
      if(rowFilled[y]==num_columns)
	{
	  numRemovedRow+=1;
	  this->removeARow(y);
	}
    }
  game->lineCleared(numRemovedRow);
}
  
void Board::removeARow(int row_to_rm)
{
  int cellLevel;  //record the level of the removed block

  //call the blocks of row i to delete a cell
  for(int x=0;x<num_columns;++x)
    {
      bool removed=blockPtr[x][row_to_rm]->deleteCell();
      if(removed)
	{
	  cellLevel=blockPtr[x][row_to_rm]->getLevel();
	  game->blockCleared(cellLevel);
	  delete blockPtr[x][row_to_rm];
	}
    }
  
  //shift the rows above row i downwards
  for(int y=row_to_rm;y>0;--y)
    {
      for(int x=0;x<num_columns;++x)
	{
	  blockPtr[x][y]=blockPtr[x][y-1];
	}
      //the record for the # of cells filled for each row
      //should also be shifted
      rowFilled[y]=rowFilled[y-1];
    }

  //the first row is filled by 0 cells
  rowFilled[0]=0;

  //for the up most row, NULL will be refilled
  for(int x=0;x<num_columns;++x)
    {
      blockPtr[x][0]=0;
    }
}
