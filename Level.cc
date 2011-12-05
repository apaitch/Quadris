#include <iostream>
#include "Level.h"
#include "Block.h"
#include "Board.h"
#include "GameSettings.h"
#include "QuadrisGame.h"
#include "XWindow.h"

using namespace std;

Level::Level( Board * board , QuadrisGame * game , int seed )
                        : board( board ) , game( game ) {
    if ( seed >= 0 ) {
        randomNumGenerator.seed( seed );
    } // if

    currentLevel = 1;
    randomNumGenerator();

    blockSequenceFile.open( "sequence.txt" );

    //find the type of the fist Block
    findNextBlockType();
} // Level

char Level::numCharTransfer( int i ) {
    char returnChar = 0;
    switch ( i ) {
        case 0 : returnChar = 'I';
                 break;
        case 1 : returnChar = 'J';
                 break;
        case 2 : returnChar = 'L';
                 break;
        case 3 : returnChar = 'O';
                 break;
        case 4 : returnChar = 'S';
                 break;
        case 5 : returnChar = 'Z';
                 break;
        case 6 : returnChar = 'T';
    } // switch

    return returnChar;
} // numCharTransfer()

int Level::generateNumber() {
    int num;
    if ( currentLevel == 1 ) {
        num = randomNumGenerator( 11 );
        switch( num ) {
            case 0: case 1:
                num = 0;
                break;
            case 2: case 3:
                num = 1;
                break;
            case 4: case 5:
                num = 2;
                break;
            case 6: case 7:
                num = 3;
                break;
            case 8:
                num = 4;
                break;
            case 9:
                num = 5;
                break;
            case 10: case 11:
                num = 6;
        } // switch
    } // if
    else if ( currentLevel == 2 ) {
        num = randomNumGenerator( 6 );
    } // else if
    else if ( currentLevel == 3 ) {
        num = randomNumGenerator( 8 );
        switch ( num ) {
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
} // generateNumber()

void Level::findNextBlockType() {
    if ( currentLevel == 0 ) {
        //in level 0, read the next input from
        //file sequence.txt directly
        if ( ! ( blockSequenceFile >> nextBlockType ) ) {
            //sequence.txt has been read to the end
            //re-open the file
            blockSequenceFile.close();
            blockSequenceFile.open( "sequence.txt" );
            blockSequenceFile >> nextBlockType;
        }
    } // if
    else {
        //level 1 2 and 3
        //will use random number generator
        //and then transfer it to char
        nextBlockType = this->numCharTransfer( this->generateNumber() );
    } // else
} // findNextBlockType()

Block * Level::getNextBlock() {
    Block * newBlock;

    //when create new bolck, findNextBlockType() function has
    //already been called, so nextBlockType has already been renewed
    newBlock = new Block( nextBlockType , startingOrigin ,
                          currentLevel , board );
    findNextBlockType();

    return newBlock;
} // getNextBlock()

int Level::getLevel() {
    return currentLevel;
} // getLevel()

void Level::levelUp() {
    currentLevel += 1;
    if ( currentLevel > maxLevel ) {
        currentLevel = maxLevel;
    } // if
} // levelUp()

void Level::levelDown() {
    currentLevel -= 1;
    if ( currentLevel < minLevel ) {
        currentLevel = minLevel;
    } // if
} // levelDown()

void Level::printNext() {
    if ( nextBlockType == 'I' ) {
        cout << "IIII" << endl;
    } // if
    else if ( nextBlockType == 'J' ) {
        cout << "J  " << endl;
        cout << "JJJ" << endl;
    } // else if
    else if ( nextBlockType == 'L' ) {
        cout << "  L" << endl; 
        cout << "LLL" << endl;
    } // else if
    else if ( nextBlockType == 'O' ) {
        cout << "OO" << endl;
        cout << "OO" << endl;
    } // else if
    else if ( nextBlockType == 'S' ) {
        cout << " SS" << endl;
        cout << "SS " << endl;
    } // else if
    else if ( nextBlockType == 'Z' ) {
        cout << "ZZ " << endl;
        cout << " ZZ" << endl;
    } // else if
    else if ( nextBlockType == 'T' ) {
        cout << "TTT" << endl;
        cout << " T " << endl;
    } // else if
} // printNext()

void Level::drawNext( Xwindow * window ) {
    if ( nextBlockType == 'I' ) {
        window->fillBorderedRectangle( 0 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Red );
        window->fillBorderedRectangle( 1 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Red );
        window->fillBorderedRectangle( 2 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Red );
        window->fillBorderedRectangle( 3 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Red );
    } // if
    else if ( nextBlockType == 'J' ) {
        window->fillBorderedRectangle( 0.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Yellow );
        window->fillBorderedRectangle( 0.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Yellow );
        window->fillBorderedRectangle( 1.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Yellow );
        window->fillBorderedRectangle( 2.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Yellow );
    } // else if
    else if ( nextBlockType == 'L' ) {
        window->fillBorderedRectangle( 0.5 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Green );
        window->fillBorderedRectangle( 1.5 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Green );
        window->fillBorderedRectangle( 2.5 * blockWidth , 1 * blockHeight ,
                                       blockWidth , blockHeight , Green );
        window->fillBorderedRectangle( 2.5 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Green );
    } // else if
    else if ( nextBlockType == 'O' ) {
        window->fillBorderedRectangle( 1 * blockWidth , 1 * blockHeight ,
                                       blockWidth , blockHeight , Brown );
        window->fillBorderedRectangle( 1 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Brown );
        window->fillBorderedRectangle( 2 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Brown );
        window->fillBorderedRectangle( 2 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Brown );
    } // else if
    else if ( nextBlockType == 'S' ) {
        window->fillBorderedRectangle( 0.5 * blockWidth , 2 * blockHeight ,
                                       blockWidth , blockHeight , Cyan );
        window->fillBorderedRectangle( 1.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Cyan );
        window->fillBorderedRectangle( 1.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Cyan );
        window->fillBorderedRectangle( 2.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Cyan );
    } // else if
    else if ( nextBlockType == 'Z' ) {
        window->fillBorderedRectangle( 0.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Blue );
        window->fillBorderedRectangle( 1.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Blue );
        window->fillBorderedRectangle( 1.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Blue );
        window->fillBorderedRectangle( 2.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Blue );
    } // else if
    else if ( nextBlockType == 'T' ) {
        window->fillBorderedRectangle( 0.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Orange );
        window->fillBorderedRectangle( 1.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Orange );
        window->fillBorderedRectangle( 1.5 * blockWidth , 2 * blockHeight , 
                                       blockWidth , blockHeight , Orange );
        window->fillBorderedRectangle( 2.5 * blockWidth , 1 * blockHeight , 
                                       blockWidth , blockHeight , Orange );
    } // else if
} // drawNext()
