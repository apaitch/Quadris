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

class QuadrisGame {
    private:
        Board * board;
        CommandTrie * commandInterpreter;
        Level * level;
        ScoreBoard * scoreBoard;
        Xwindow * mainWindow;  //display the main board
	Xwindow * nextWindow;  //display the next block
        Ai * ai;

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
        // -------------------------

        void initialize();
        bool processInput();

        void print();
        void draw();
        void output();

    public:
        explicit QuadrisGame( bool = false , bool = false , int = -1 );
        ~QuadrisGame();
        void runGameLoop();
	void lineCleared( int );
        void blockCleared( int );

};

#endif
