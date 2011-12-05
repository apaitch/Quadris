#include <iostream>
#include <sstream>

#include "Colours.h"
#include "GameSettings.h"
#include "ScoreBoard.h"
#include "XWindow.h"

using namespace std;

ScoreBoard::ScoreBoard( int level ) : highScore ( 0 ) ,
                                      currentScore ( 0 ) ,
                                      currentLevel ( level ) {}

/*
 * Called when a block is completely cleared from the board. Determines the
 * score given the level at which the block was created.
 */
void ScoreBoard::blockCleared( int blockLevel ) {
  currentScore += ( blockLevel + 1 ) * (blockLevel + 1 );
  if ( currentScore > highScore ) {
    highScore = currentScore;
  } // if
} // blockCleared()

/*
 * Called when a number of rows/lines are cleared from the board.
 */
void ScoreBoard::linesCleared( int numLines ) {
    currentScore += ( numLines + currentLevel ) * 
             ( numLines + currentLevel );
    if ( currentScore > highScore ) {
        highScore = currentScore;
    } // if
} // linesCleared()

void ScoreBoard::resetScore() {
    currentScore = 0;
} // resetScore()

void ScoreBoard::setLevel( int level ) {
    currentLevel = level;
} // setLevel()

void ScoreBoard::print() {
    cout << "Level:     " << currentLevel << endl;
    cout << "Score:     " << currentScore << endl;
    cout << "Hi Score:  " << highScore << endl;
} // print()

void ScoreBoard::draw( Xwindow * window ) {
    ostringstream sstream;
    sstream << " LEVEL: " << currentLevel;
    sstream << " SCORE: " << currentScore;
    sstream << " HI SCORE: " << highScore;

    // Augmenting .second by height so that the string touches the bottom of the
    // slot allotted to the scoreBoard.
    window->drawString( scoreBoardPosn.first , 
                        scoreBoardPosn.second + scoreBoardHeight , 
                        sstream.str() , Yellow );
} // draw()

