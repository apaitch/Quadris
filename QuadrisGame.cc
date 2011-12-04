#include "QuadrisGame.h"
#include "Ai.h"
#include "Board.h"
#include "Level.h"
#include "Block.h"
#include "XWindow.h"
#include "CommandTrie.h"
#include "ScoreBoard.h"
#include <cstdlib>

using namespace std;

QuadrisGame::QuadrisGame( bool text_only , bool ai_on , int seed ) 
                           : board( new Board(this) ) ,
                             command_interpreter( new CommandTrie ) ,
                             level( new Level ( board , this , seed ) ) ,
                             score_board( new ScoreBoard ( level->getLevel() ) ) ,
                             window( 0 ) ,
                             smallWindow( 0 ) ,
                             ai ( 0 ) ,
                             text_only( text_only ) {
    if ( ! text_only ) {
        window = new Xwindow;
	smallWindow=new Xwindow(200,200);

    }

    if ( ai_on ) {
        ai = new Ai ( this , board );
    }

    min_padding = 4;
    board_height = window_height - 2 * min_padding;
    board_width = board_height * num_columns / num_rows;
    board_posn.first = min_padding;
    board_posn.second = min_padding;

    score_board_posn.first = 2 * min_padding + board_width;
    score_board_posn.second = min_padding;
    score_board_width = window_width - board_width - 3 * min_padding;
    score_board_height = window_height / 5;
    initialize(); 
} // QuadrisGame 

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
    if ( ai != 0 ) {
        levelUp( 4 );
    }
    output();
} // initialize()

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
                } // if
                else {
                    command = full_command.substr(i);
                    break;
                } // else
            } // for
        } // if
        else {
            command = full_command;
        } // else

        commandFunctPtr commandFn = command_interpreter->findCommand( command );    
        if ( commandFn != 0 ) {
            CALL_MEMBER_FN(*this , commandFn)( multiplier );
        } // if
        return true;
    } // if
    else {
        return false;
    } // else
} // processInput()

void QuadrisGame::print() {
    score_board->print();
    cout << "----------" << endl;
    board->print();
    cout << "----------" << endl;
    cout << "Next:" << endl;
    level->printNext();
}

void QuadrisGame::draw() {
    window->fillRectangle( 0 , 0 , window_width , window_height );

    board->draw( board_posn.first , board_posn.second , 
                 board_width , board_height , window );

    score_board->draw( score_board_posn.first , 
                       score_board_posn.second ,
                       score_board_width , score_board_height , 
                       window );

    level->drawNext(smallWindow);
    
}

void QuadrisGame::output() {
    print();
    if ( ! text_only ) {
        draw();
    } // if
}

void QuadrisGame::runGameLoop() {
    if ( ai == 0 ) {
        while ( processInput() ) {
            output();
        } // while
    }
    else {
        while ( true ) {
            vector< string > commands;
            ai->makeNextMove( commands );
            for ( unsigned int i = 0 ; i < commands.size() ; ++i ) {
                commandFunctPtr commandFn = 
                    command_interpreter->findCommand( commands[i] );
                if ( commandFn != 0 ) {
                    CALL_MEMBER_FN(*this , commandFn)( 1 );
                    output();
                    usleep( 200000 );
                } // if
            }
        }
    }
}

// -------------------------
// Defining Command Functions
// -------------------------
void QuadrisGame::rightRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->rightRotate();
    } // for
}

void QuadrisGame::leftRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->leftRotate();
    } // for
}

void QuadrisGame::moveLeft( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveLeft();
    } // for
}

void QuadrisGame::moveRight( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveRight();
    } // for
}

void QuadrisGame::moveDown( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveDown();
    } // for
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
        } // if
    } // for
}

void QuadrisGame::levelUp( int multiplier ) {
    for( int times = 0; times < multiplier ; ++times ) {
        level->levelup();
    } // for
    score_board->setLevel( level->getLevel() );
}

void QuadrisGame::levelDown( int multiplier ) {
    for( int times = 0 ; times < multiplier ; ++times ) {
        level->leveldown();
    } // for
    score_board->setLevel( level->getLevel() );
}

void QuadrisGame::reset( int multiplier ) {
    delete board;
    delete level;
    if ( ai != 0 ) {
        delete ai;
    }

    board = new Board(this);
    level = new Level ( board, this );
    
    if ( ai != 0 ) {
        ai = new Ai( this , board );
    }

    score_board->resetScore();
    score_board->setLevel( level->getLevel() );
    board->setActiveBlock( level->createNew() );
    output();

} // reset

// -------------------------
// End of Command Functions
// -------------------------



