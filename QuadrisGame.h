#ifndef __QUADRIS_GAME_H__
#define __QUADRIS_GAME_H__

#include "CommandTrie.h"

class Block;
class Board;
class Level;

class QuadrisGame {
    private:
        Board * board;
        CommandTrie * command_interpreter;
        Level * level;

        int high_score;
        int score;
        bool text_only;

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
        void draw();
    public:
        explicit QuadrisGame( bool = false , int = -1 );
        ~QuadrisGame();
        void runGameLoop();
	void lineCleared( int );
        void blockCleared( int );
};

#endif
