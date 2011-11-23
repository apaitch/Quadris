#include <iostream>
#include "level.h"
#include "PRNG.h"
using namespace std;

void level::restart()
{
  //reset the score record and current level
  score=0;
  currentLevel=0;
}

level::level(Board *b):b(b)
{
  hiscore=0;
  this->restart();
  fs.open("sequence.txt");
}

char level::numCharTransfer(int i)
{
  if(i==0)
    return 'I';
  else if(i==1)
    return 'J';
  else if(i==2)
    return 'L';
  else if(i==3)
    return 'O';
  else if(i==4)
    return 'S';
  else if(i==5)
    return 'Z';
  else if(i==6)
    return 'T';
}

int level::numGenerator()
{
  PRNG prng;
  int num;
  if(currentLevel==1)
    {
      num=prng(11);
      switch(num){
      case 0:
      case 1:
	num=0;
	break;
      case 2:
      case 3:
	num=1;
	break;
      case 4:
      case 5:
	num=2;
	break;
      case 6:
      case 7:
	num=3;
	break;
      case 8:
	num=4;
	break;
      case 9:
	num=5;
	break;
      case 10:
      case 11:
	num=6;
	break;
      }
    }
  else if(currentLevel==2)
    {
      num=prng(6);
    }
  else if(currentLevel==3)
    {
      num=prng(8);
      switch(num){
      case 0:
        num=0;
        break;
      case 1:
        num=1;
        break;
      case 2:
        num=2;
        break;
      case 3:
        num=3;
        break;
      case 4:
      case 5:
        num=4;
        break;
      case 6:
      case 7:
        num=5;
        break;
      case 8:
        num=6;
        break;
      }
      return num;
    }
}

void level::findNext()
{
 
  if(currentLevel==0)
    {
      //in level 0, read the next input from                                        
      //file sequence.txt directly                                                  
      fs>>nextBlock;
    }
  else
    {
      //level 1 2 and 3                                                             
      //will use random number generator                                            
      //and then transfer it to char  
      nextBlock=this->numCharTransfer(this->numGenerator());
    }

}

Block* level::createNew()
{
  Block *newBlock;

  //when create new bolck, assume findNext() function has
  //already been called, so nextBlock has already been renewed

  newBlock=new block(nextBlock,0,0,currentLevel,);
  return newBlock;
}

void level::levelup()
{
  currentLevel+=1;
  if(currentLevel>3)
    currentLevel=3;
}

void level::leveldown()
{
  currentLevel-=1;
  if(currentLevel<0)
    currentLevel=0;
}
