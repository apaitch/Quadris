#include <iostream>
#include "Board.h"
using namespace std;

Board::Board()
{
  //initial all block pointers to NULL
  for(int i=0;i<15;++i)
    {
      for(int j=0;j<10;++j)
	blockPtr[i][j]=NULL;
    }
  
  //initiate the number of filled cells to be zero
  //for all rows
  for(int i=0;i<15;++i)
    rowFilled[i]=0;
}

Block* Board::getBlockPtr(pair<int,int> a)
{
  return blockPtr[a.first][a.second];
}

void Board::addBlock(pair<int,int> *a,Block *b)
{
  for(int i=0;i<4;++i)
    {
      int m=a[i].first;
      int n=a[i].second;
      blockPtr[m][n]=b;
      rowFilled[m]+=1;
    }
}

void Board::deleteBlock(pair<int,int> *a) //used when delete the whole block
{
  for(int i=0;i<4;++i)
    {
      blockPtr[a[i].first][a[i].second]=NULL;
      rowFilled[a[i].first]-=1;
    }
}

void Board::print()
{
  for(int i=0;i<15;++i)
    {
      for(int j=0;j<10;++j)
	{
	  cout<<blockPtr[i][j].getChar();
	}
      cout<<endl;
    }
}

void Board::examine()
{
  int numRemovedRow=0; //record how many rows have be removed
  for(int i=0;i<15;++i)
    {
      if(rowFilled[i]==10)
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
  for(int k=0;k<10;++k)
    {
      bool removed=blockPtr[i][k]->deleteCell();
      if(removed)
	{
	  cellLevel=blockPtr[i][k]->getLevel();
	  numRemovedCell+=1;
	  delete blockPtr[i][k];
	}
    }
  
  //shift the rows above row i downwards
  for(int k=i;k<14;++k)
    {
      for(int m=0;m<10;++m)
	{
	  blockPtr[k][m]=blockPtr[k+1][m];
	}
    }

  //for the up most row, NULL will be refilled
  for(int m=0;m<10;++m)
    {
      blockPtr[14][m]=NULL;
    }
}
