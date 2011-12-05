#include "ScoreBoard.h"
#include "XWindow.h"
#include "Colours.h"
#include "GameSettings.h"
#include <iostream>
#include <sstream>

using namespace std;

ScoreBoard::ScoreBoard( int level ) : highScore ( 0 ) ,
                                      currentScore ( 0 ) ,
                                      currentLevel ( level ) {}

void ScoreBoard::blockCleared( int level ) {
  currentScore += ( level + 1 ) * (level + 1 );
  if ( currentScore > highScore ) {
    highScore = currentScore;
  } // if
}

void ScoreBoard::linesCleared( int numLines ) {
    currentScore += ( numLines + currentLevel ) * 
             ( numLines + currentLevel );
    if ( currentScore > highScore ) {
        highScore = currentScore;
    } // if
}

void ScoreBoard::resetScore() {
    currentScore = 0;
}
void ScoreBoard::setLevel( int level ) {
    currentLevel = level;
}

void ScoreBoard::print() {
    cout << "Level:     " << currentLevel << endl;
    cout << "Score:     " << currentScore << endl;
    cout << "Hi Score:  " << highScore << endl;
}

void ScoreBoard::draw( Xwindow * window ) {
    ostringstream sstream;
    sstream << " LEVEL: " << currentLevel;
    sstream << " SCORE: " << currentScore;
    sstream << " HI SCORE: " << highScore;

    window->drawString( scoreBoardPosn.first + 5 , 
                        scoreBoardPosn.second + 20 , 
                        sstream.str() , Yellow );
/*
    sstream << "LEVEL: " << currentLevel;
    window->drawString( xCoord + 5 , yCoord + 22 , sstream.str() , Yellow );
    sstream.str("");
    sstream << "SCORE: " << currentScore;
    window->drawString( xCoord + 5 , yCoord + 42 , sstream.str() , Yellow );
    sstream.str("");
    sstream << "HI SCORE: " << highScore;
    window->drawString( xCoord + 5 , yCoord + 62 , sstream.str() , Yellow );
*/
}

