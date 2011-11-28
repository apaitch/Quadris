#include "ScoreBoard.h"
#include "XWindow.h"
#include "Colours.h"
#include <iostream>
#include <sstream>

using namespace std;

ScoreBoard::ScoreBoard( int level ) : high_score ( 0 ) ,
                                      current_score ( 0 ) ,
                                      current_level ( level ) {}

void ScoreBoard::blockCleared( int level ) {
  current_score += ( level + 1 ) * (level + 1 );
  if ( current_score > high_score ) {
    high_score = current_score;
  } // if
}

void ScoreBoard::linesCleared( int num_lines ) {
    current_score += ( num_lines + current_level ) * 
             ( num_lines + current_level );
    if ( current_score > high_score ) {
        high_score = current_score;
    } // if
}

void ScoreBoard::resetScore() {
    current_score = 0;
}
void ScoreBoard::setLevel( int level ) {
    current_level = level;
}

void ScoreBoard::print() {
    cout << "Level:     " << current_level << endl;
    cout << "Score:     " << current_score << endl;
    cout << "Hi Score:  " << high_score << endl;
}

void ScoreBoard::draw( int x_coord , int y_coord ,
                       int width , int height , Xwindow * window ) {
    ostringstream sstream;
    sstream << "LEVEL: " << current_level;
    window->drawString( x_coord + 5 , y_coord + 22 , sstream.str() , Yellow );
    sstream.str("");
    sstream << "SCORE: " << current_score;
    window->drawString( x_coord + 5 , y_coord + 42 , sstream.str() , Yellow );
    sstream.str("");
    sstream << "HI SCORE: " << high_score;
    window->drawString( x_coord + 5 , y_coord + 62 , sstream.str() , Yellow );
}

