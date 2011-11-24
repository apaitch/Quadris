#include <iostream>
#include "Board.h"
#include "Block.h"
using namespace std;

Board::Board()
{
  //initial all block pointers to NULL
  for(int i=0;i<num_rows;++i)
    {
      for(int j=0;j<num_columns;++j)
	blockPtr[i][j]=NULL;
    }
  
  //initiate the number of filled cells to be zero
  //for all rows
  for(int i=0;i<num_rows;++i)
    rowFilled[i]=0;
}

Board::~Board() {
    delete activeBlock;
    for ( int x = 0 ; x < num_rows ; ++x ) {
        for ( int y = 0 ; y < num_columns ; ++y ) {
            delete blockPtr[x][y];
        }
    }
}

Block * Board::getActiveBlock() {
    return activeBlock;
}

void Board::setActiveBlock( Block * b ) {
    activeBlock = b;
    addBlock ( activeBlock );
}

Block* Board::getBlockPtr(pair<int,int> a)
{
  if( a.second >= 0 && a.second < num_rows && a.first >= 0 && a.first < 9 ) {
    return blockPtr[a.second][a.first];
  }
  else {
    return activeBlock + 1;
  }
}

void Board::addBlock( Block * b )
{
    vector< pair<int , int> > block_points;
    b->getPoints( block_points );

  for(int i=0;i<4;++i)
    {
      int m=block_points[i].second;
      int n=block_points[i].first;
      cout << m << " " << n  << endl;
      cout << blockPtr[0][0] << endl;
      blockPtr[m][n]=b;
      rowFilled[m]+=1;
    }
}

void Board::deleteBlock( Block * b ) //used when delete the whole block
{
    vector< pair<int , int> > block_points;
    b->getPoints( block_points );

  for(int i=0;i<4;++i)
    {
      int m=block_points[i].second;
      int n=block_points[i].first;
      cout << m << " " << n << "X" << endl;
      blockPtr[m][n]=0;
      rowFilled[m]-=1;
    }
}

void Board::print()
{
  for(int i=0;i<num_rows;++i)
    {
      for(int j=0;j<num_columns;++j)
	{
          if ( blockPtr[i][j] == 0 ) {
            cout << " ";
          }
          else {
	    cout << blockPtr[i][j]->getType();
          }
	}
      cout<<endl;
    }
}

void Board::examine()
{
  int numRemovedRow=0; //record how many rows have be removed
  for(int i=0;i<num_rows;++i)
    {
      if(rowFilled[i]==num_columns)
	{
	  numRemovedRow+=0;
	  this->removeARow(i);
	}
    }
}
  
void Board::removeARow(int i)
{
  int cellLevel;  //record the level of the removed block

  //call the blocks of row i to delete a cell
  for(int k=0;k<num_columns;++k)
    {
      bool removed=blockPtr[i][k]->deleteCell();
      if(removed)
	{
	  cellLevel=blockPtr[i][k]->getLevel();
	  //numRemovedCell+=1;
	  delete blockPtr[i][k];
	}
    }
  
  //shift the rows above row i downwards
  for(int k=i;k<14;++k)
    {
      for(int m=0;m<num_columns;++m)
	{
	  blockPtr[k][m]=blockPtr[k+1][m];
	}
    }

  //for the up most row, NULL will be refilled
  for(int m=0;m<num_columns;++m)
    {
      blockPtr[14][m]=NULL;
    }
}
