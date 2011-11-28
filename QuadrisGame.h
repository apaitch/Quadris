#ifndef __QUADRIS_GAME_H__
#define __QUADRIS_GAME_H__

#include <utility>

class Block;
class Board;
class Level;
class Xwindow;
class CommandTrie;
class ScoreBoard;

const int window_width = 500;
const int window_height = 600;

class QuadrisGame {
    private:
        Board * board;
        CommandTrie * command_interpreter;
        Level * level;
        ScoreBoard * score_board;
        Xwindow * window;

        bool text_only;

        // Variables for drawing
        int min_padding;
        int board_height;
        int board_width;
        std::pair<int,int> board_posn;
        int score_board_width;
        int score_board_height;
        std::pair<int,int> score_board_posn;
        int next_box_width;
        int next_box_height;
        std::pair<int,int> next_box_posn;
        

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
