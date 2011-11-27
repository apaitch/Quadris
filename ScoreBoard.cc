#include "ScoreBoard.h"
#include "XWindow.h"
#include <iostream>

using namespace std;

ScoreBoard::ScoreBoard( int level ) : high_score ( 0 ) ,
                                      current_score ( 0 ) ,
                                      current_level ( level ) {}

void ScoreBoard::blockCleared( int level ) {
  current_score += ( level + 1 ) * (level + 1 );
  if ( current_score > high_score ) {
    high_score = current_score;
  }
}

void ScoreBoard::linesCleared( int num_lines ) {
    current_score += ( num_lines + current_level ) * 
             ( num_lines + current_level );
    if ( current_score > high_score ) {
        high_score = current_score;
    }
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

void ScoreBoard::draw( Xwindow * window , int x_coord , int y_coord ,
                       int width , int height ) {
}
