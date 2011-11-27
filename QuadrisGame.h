#ifndef __QUADRIS_GAME_H__
#define __QUADRIS_GAME_H__

class Block;
class Board;
class Level;
class Xwindow;
class CommandTrie;

const int window_width = 500;
const int window_height = 500;

class QuadrisGame {
    private:
        Board * board;
        CommandTrie * command_interpreter;
        Level * level;
        Xwindow * window;

        bool text_only;
        int high_score;
        int score;

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
        explicit QuadrisGame( bool = false , int = -1 );
        ~QuadrisGame();
        void runGameLoop();
	void lineCleared( int );
        void blockCleared( int );
};

#endif
