#ifndef _QUADRISGAMEH__
#define _QUADRISGAMEH__

#include <utility>
#include <map>

class Ai;
class Block;
class Board;
class Level;
class Xwindow;
class CommandTrie;
class ScoreBoard;

/*
 * This is the enveloping class for the game. It controls and coordinates user
 * input, drawing/pring, and other functions. It acts as a middle-man between
 * some of the other objects (e.g. Board and ScoreBoard). However, it doesn't
 * control any of the gameplay other than user input.
 */
class QuadrisGame {
    private:
        Board * board;
        CommandTrie * commandInterpreter;
        Level * level;
        ScoreBoard * scoreBoard;
        Xwindow * mainWindow;  //display board and score
	Xwindow * nextWindow;  //display the next block
        Ai * ai;

        bool gameOver;
        bool aiOn;
        bool textOnly;

        // Command Functions
        void rightRotate( int );
        void leftRotate( int );
        void moveLeft( int );
        void moveRight( int );
        void moveDown( int );
        void drop( int );
        void levelUp( int );
        void levelDown( int );
        void reset( int );
        void rename( int );
        // -------------------------

        bool processInput();
        void output();
        void print();
        void draw();

    public:
        explicit QuadrisGame( bool = false , int = -1 , int = -1 );
        ~QuadrisGame();
        void runGameLoop();
	void lineCleared( int );
        void blockCleared( int );

};

#endif
