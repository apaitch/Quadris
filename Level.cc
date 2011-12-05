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

    // Find the type of the first Block
    findNextBlockType();
} // Level

blockType Level::numToBlockType( int i ) {
    blockType returnType;
    switch ( i ) {
        case 0 : returnType = I;
                 break;
        case 1 : returnType = J;
                 break;
        case 2 : returnType = L;
                 break;
        case 3 : returnType = O;
                 break;
        case 4 : returnType = S;
                 break;
        case 5 : returnType = Z;
                 break;
        case 6 : returnType = T;
    } // switch

    return returnType;
} // numToBlockType()

/*
 * Uses randomNumGenerator to generate a number between 0 and 6 to indicate the
 * type of the next block. The probabilities vary by level.
 */
int Level::generateBlockNumber() {
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
} // generateBlockNumber()

/*
 * Stores the type of the next block in the nextBlockType field.
 */
void Level::findNextBlockType() {
    if ( currentLevel == 0 ) {
        char nextBlock;
        if ( ! ( blockSequenceFile >> nextBlock ) ) {
            // Sequence.txt read to end. Re-open.
            blockSequenceFile.close();
            blockSequenceFile.open( "sequence.txt" );
            blockSequenceFile >> nextBlock;
        } // if

        // Need to make sure the next block corresponds to a type.
        switch ( nextBlock ) {
            case 'I' : case 'J' : case 'L' : case 'O' : case 'S' : case 'Z' :
            case 'T' :
                nextBlockType = ( blockType ) nextBlock;
                break;
            default :
                cerr << "Your sequence.txt file has an invalid block type. ";
                cerr << "Get rid of it ;) For now, here's an I." << endl; 
                nextBlockType = I;
        } // switch
    } // if
    else {
        nextBlockType = this->numToBlockType( this->generateBlockNumber() );
    } // else
} // findNextBlockType()

/*
 * Called to get the next block according to the level specifications. Serves a
 * pointer to the created block.
 */
Block * Level::getNextBlock() {
    Block * newBlock;

    newBlock = new Block( nextBlockType , startingOrigin ,
                          currentLevel , board );

    // Refresh the next block type.
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
    findNextBlockType();
} // levelUp()

void Level::levelDown() {
    currentLevel -= 1;
    if ( currentLevel < minLevel ) {
        currentLevel = minLevel;
    } // if
    findNextBlockType();
} // levelDown()

/*---------------
 * Drawing/Printing Methods
 *      This is hard-coded ;)
 *---------------*/
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
