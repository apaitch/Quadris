#ifndef __GAMESETTINGS_H__
#define __GAMESETTINGS_H__

#include <utility>
#include <map>

// This header file contains definitions for gameplay and drawing settings 

// Gameplay Settings -------------
const int pointsPerBlock = 4;

const int numColumns = 10;
const int numRows = 18;

const int maxLevel = 3;
const int minLevel = 0;

const std::pair<int,int> startingOrigin = std::make_pair( 0 , 3 );

enum blockType { T = 'T',
                 S = 'S',
                 I = 'I',
                 J = 'J',
                 L = 'L',
                 O = 'O',
                 Z = 'Z' };

// -------------------------------
// Drawing settings---------------
const int mainWindowWidth = 400;
const int mainWindowHeight = 700;


const int scoreBoardHeight = 20;
const int scoreBoardWidth = mainWindowWidth;
const std::pair<int,int> scoreBoardPosn = std::make_pair( 0 , 0 );

const int boardHeight = mainWindowHeight - scoreBoardHeight;
const int boardWidth = mainWindowWidth;
const std::pair<int,int> boardPosn = std::make_pair( 0 , scoreBoardHeight );

const int blockHeight = boardHeight / numRows;
const int blockWidth = boardWidth / numColumns;

const int nextWindowHeight = 4 * blockHeight + 10;
const int nextWindowWidth = 4 * blockWidth + 10;
// -------------------------------

#endif

