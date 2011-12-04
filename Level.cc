#include <iostream>
#include "Level.h"
#include "Block.h"
#include "Board.h"
#include "QuadrisGame.h"
#include "XWindow.h"

using namespace std;

void Level::restart()
{
  //reset the score record and current level
  currentLevel=1;
  prng();

}

Level::Level( Board * b , QuadrisGame * game , int seed ) 
            : b(b) , game(game) {
  if ( seed >= 0 ) {
    prng.seed( seed );
  } // if

  this->restart();
  fs.open("sequence.txt");

  //find the type of the fist Block
  findNext();
} // Level

char Level::numCharTransfer(int i)
{
  char return_char = 0;
  switch ( i ) {
    case 0 : return_char = 'I';
             break;
    case 1 : return_char = 'J';
             break;
    case 2 : return_char = 'L';
             break;
    case 3 : return_char = 'O';
             break;
    case 4 : return_char = 'S';
             break;
    case 5 : return_char = 'Z';
             break;
    case 6 : return_char = 'T';
  } // switch

  return return_char;
} // numCharTransfer()

int Level::numGenerator()
{
  int num;
  if(currentLevel==1) {
      num=prng(11);
      switch(num){
          case 0: case 1:
            num=0;
            break;
          case 2: case 3:
            num=1;
            break;
          case 4: case 5:
            num=2;
            break;
          case 6: case 7:
            num=3;
            break;
          case 8:
            num=4;
            break;
          case 9:
            num=5;
            break;
          case 10: case 11:
            num=6;
      } // switch
  } // if
  else if(currentLevel==2) {
      num=prng(6);
  } // else if
  else if(currentLevel==3) {
      num=prng(8);
      switch(num) {
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
          case 4: case 5:
            num=4;
            break;
          case 6: case 7:
            num=5;
            break;
          case 8:
            num=6;
      } // switch
  } // else if
  return num;
} // numGenerator()

void Level::findNext() {
 
  if(currentLevel==0) {
      //in level 0, read the next input from
      //file sequence.txt directly
    if(fs>>nextBlock){
	;
      }
    else {
      //sequence.txt has been read to the end
      //re-open the file
      fs.close();
      fs.open("sequence.txt");
      fs>>nextBlock;
    }
  } // if
  else {
      //level 1 2 and 3
      //will use random number generator
      //and then transfer it to char
      nextBlock=this->numCharTransfer(this->numGenerator());
  } // else

} // findNext()

Block* Level::createNew() {
  Block * newBlock;

  //when create new bolck, findNext() function has
  //already been called, so nextBlock has already been renewed
  newBlock = new Block( nextBlock , make_pair( 0 , 3 ) , currentLevel , b);
  findNext();

  return newBlock;
}

int Level::getLevel() {
    return currentLevel;
}

void Level::levelup() {
  currentLevel += 1;
  if ( currentLevel > max_level ) {
    currentLevel = max_level;
  } // if
}

void Level::leveldown() {
  currentLevel -= 1;
  if ( currentLevel < min_level ) {
    currentLevel = min_level;
  } // if
}

void Level::printNext() {
  if(nextBlock=='I')
    cout<<"IIII"<<endl;
  else if(nextBlock=='J')
    cout<<"J"<<endl<<"JJJ"<<endl;
  else if(nextBlock=='L')
    cout<<"  L"<<endl<<"LLL"<<endl;
  else if(nextBlock=='O')
    cout<<"OO"<<endl<<"OO"<<endl;
  else if(nextBlock=='S')
    cout<<" SS"<<endl<<"SS"<<endl;
  else if(nextBlock=='Z')
    cout<<"ZZ"<<endl<<" ZZ"<<endl;
  else if(nextBlock=='T')
    cout<<"TTT"<<endl<<" T "<<endl;
}

void Level::drawNext(Xwindow *smallWindow)
{
  smallWindow->fillRectangle( 0,0 , 200, 200, Grey );

   if(nextBlock=='I')
     {
        smallWindow->fillBorderedRectangle( 0,0 ,50 ,50 ,Red );
	smallWindow->fillBorderedRectangle( 50,0 ,50 ,50 ,Red );
	smallWindow->fillBorderedRectangle( 100,0 ,50 ,50 ,Red );
        smallWindow->fillBorderedRectangle( 150,0 ,50 ,50 ,Red );
     }
  else if(nextBlock=='J')
    {
       smallWindow->fillBorderedRectangle( 0,0 ,50 ,50 ,Yellow );
       smallWindow->fillBorderedRectangle( 0,50 ,50 ,50 ,Yellow );
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Yellow );
       smallWindow->fillBorderedRectangle( 100,50 ,50 ,50 ,Yellow );
    }
  else if(nextBlock=='L')
    {
       smallWindow->fillBorderedRectangle( 100,0 ,50 ,50 ,Green);
       smallWindow->fillBorderedRectangle( 0,50 ,50 ,50 ,Green);
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Green);
       smallWindow->fillBorderedRectangle( 100,50 ,50 ,50 ,Green);
    }
  else if(nextBlock=='O')
    {
       smallWindow->fillBorderedRectangle( 0,0 ,50 ,50 ,Brown);
       smallWindow->fillBorderedRectangle( 50,0 ,50 ,50 ,Brown);
       smallWindow->fillBorderedRectangle( 0,50 ,50 ,50 ,Brown);
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Brown);
    }
  else if(nextBlock=='S')
    {
       smallWindow->fillBorderedRectangle( 50,0 ,50 ,50 ,Cyan );
       smallWindow->fillBorderedRectangle( 100,0 ,50 ,50 ,Cyan );
       smallWindow->fillBorderedRectangle( 0,50 ,50 ,50 ,Cyan );
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Cyan );
    }
  else if(nextBlock=='Z')
    {
       smallWindow->fillBorderedRectangle( 0,0 ,50 ,50 ,Blue );
       smallWindow->fillBorderedRectangle( 50,0 ,50 ,50 ,Blue );
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Blue );
       smallWindow->fillBorderedRectangle( 100,50 ,50 ,50 ,Blue );
    }
  else if(nextBlock=='T')
    {
       smallWindow->fillBorderedRectangle( 0,0 ,50 ,50 ,Orange );
       smallWindow->fillBorderedRectangle( 50,0 ,50 ,50 ,Orange );
       smallWindow->fillBorderedRectangle( 100,0 ,50 ,50 ,Orange );
       smallWindow->fillBorderedRectangle( 50,50 ,50 ,50 ,Orange );
    }
}
