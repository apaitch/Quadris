#include "QuadrisGame.h"
#include "Board.h"
#include "Level.h"
#include "Block.h"
#include "XWindow.h"
#include "CommandTrie.h"
#include "ScoreBoard.h"
#include <cstdlib>

using namespace std;

QuadrisGame::QuadrisGame( bool text_only , int seed ) 
                           : board( new Board(this) ) ,
                             command_interpreter( new CommandTrie ) ,
                             level( new Level ( board , this , seed ) ) ,
                             score_board( new ScoreBoard ( level->getLevel() ) ) ,
                             window( new Xwindow ) ,
                             text_only( text_only ) { initialize(); }

QuadrisGame::~QuadrisGame() {
    delete window;
    delete board;
    delete command_interpreter;
    delete level;
    delete score_board;
}

void QuadrisGame::initialize() {
    command_interpreter->addCommand( "left" , &QuadrisGame::moveLeft );
    command_interpreter->addCommand( "right" , &QuadrisGame::moveRight );
    command_interpreter->addCommand( "clockwise" , &QuadrisGame::rightRotate );
    command_interpreter->addCommand( "counterclockwise" , &QuadrisGame::leftRotate );
    command_interpreter->addCommand( "down" , &QuadrisGame::moveDown );
    command_interpreter->addCommand( "drop" , &QuadrisGame::drop );
    command_interpreter->addCommand( "levelup" , &QuadrisGame::levelUp );
    command_interpreter->addCommand( "leveldown" , &QuadrisGame::levelDown );
    command_interpreter->addCommand( "restart" , &QuadrisGame::reset );
    board->setActiveBlock( level->createNew() );
    output();
}

void QuadrisGame::lineCleared( int num_cleared ) {
    score_board->linesCleared( num_cleared );
}

void QuadrisGame::blockCleared( int level ) {
    score_board->blockCleared( level );
}

// Take in and process one command.
bool QuadrisGame::processInput() {
    string full_command;

    if ( cin >> full_command ) {
        string command;
        int multiplier = 1;

        // If there is a numeric input, get multiplier
        if ( full_command[0] >= '0' && full_command[0] <= '9' ) {
            multiplier = 0;
            for ( unsigned int i = 0 ; i < full_command.size() ; ++i ) {
                char next_char = full_command[i];
                if ( next_char >= '0' && next_char <= '9' ) {
                   multiplier = multiplier * 10 + (next_char - 48); 
                }
                else {
                    command = full_command.substr(i);
                    break;
                }
            }
        }
        else {
            command = full_command;
        }

        commandFunctPtr commandFn = command_interpreter->findCommand( command );    
        if ( commandFn != 0 ) {
            CALL_MEMBER_FN(*this , commandFn)( multiplier );
        }
        return true;
    }
    else {
        return false;
    }
}

void QuadrisGame::print() {
    score_board->print();
    cout << "----------" << endl;
    board->print();
    cout << "----------" << endl;
    cout << "Next:" << endl;
    level->printNext();
}

void QuadrisGame::draw() {
    int board_height = 500;
    int board_width = 500 * num_columns / num_rows;
    window->fillRectangle( 0 , 0 , window_width , window_height );
    board->draw( 2 , 0 , board_width , board_height , window );
}

void QuadrisGame::output() {
    print();
    if ( ! text_only ) {
        draw();
    }
}

void QuadrisGame::runGameLoop() {
    while ( processInput() ) {
        output();
    }
}

// -------------------------
// Defining Command Functions
// -------------------------
void QuadrisGame::rightRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->rightRotate();
    }
}

void QuadrisGame::leftRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->leftRotate();
    }
}

void QuadrisGame::moveLeft( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveLeft();
    }
}

void QuadrisGame::moveRight( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveRight();
    }
}

void QuadrisGame::moveDown( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveDown();
    }
}

void QuadrisGame::drop( int multiplier ) {
    bool game_over = false;
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->drop();
        board->examine();
        game_over = board->setActiveBlock( level->createNew() );
        if ( game_over ) {
            reset( 1 );
            break;
        }
    }
}

void QuadrisGame::levelUp( int multiplier ) {
    for( int times = 0; times < multiplier ; ++times ) {
        level->levelup();
    }
    score_board->setLevel( level->getLevel() );
}

void QuadrisGame::levelDown( int multiplier ) {
    for( int times = 0 ; times < multiplier ; ++times ) {
        level->leveldown();
    }
    score_board->setLevel( level->getLevel() );
}

void QuadrisGame::reset( int multiplier ) {
    delete board;
    delete level;

    board = new Board(this);
    level = new Level ( board, this );

    score_board->resetScore();
    score_board->setLevel( level->getLevel() );
    board->setActiveBlock( level->createNew() );
    output();

}

// -------------------------
// End of Command Functions
// -------------------------


