#include "QuadrisGame.h"
#include "Board.h"
#include "Level.h"
#include "Block.h"

using namespace std;

QuadrisGame::QuadrisGame() : board( new Board(this) ) ,
                             command_interpreter( new CommandTrie ) ,
                             level( new Level (board,this) ) ,
                             high_score( 0 ) ,
                             score( 0 ) { initialize(); }

QuadrisGame::~QuadrisGame() {
    delete board;
    delete command_interpreter;
    delete level;
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
    draw();
}

void QuadrisGame::lineCleared( int num_cleared ) {
  score += ( num_cleared + level->getLevel() ) * ( num_cleared + level->getLevel() );
    if ( score > high_score ) {
        high_score = score;
    }
}

void QuadrisGame::blockCleared( int level ) {
    score += ( level + 1 ) * (level + 1 );
    if ( score > high_score ) {
        high_score = score;
    }
}

// Take in and process one command.
bool QuadrisGame::processInput() {
    string full_command;
    commandFunctPtr command;

    if ( cin >> full_command ) {
        string command;
        int multiplier = 1;

        // If there is a numeric input, get multiplier
        if ( full_command[0] >= '0' && full_command[0] <= '9' ) {
            multiplier = 0;
            for ( int i = 0 ; i < full_command.size() ; ++i ) {
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

void QuadrisGame::draw() {
    cout << "Level:   " << level->getLevel() << endl;
    cout << "Score:   " << score << endl;
    cout << "Hi Score: " << high_score << endl;
    cout << "----------" << endl;
    board->print();
    cout << "----------" << endl;
    //cout << "Next:" << endl;
    //level->printNext();
}

void QuadrisGame::runGameLoop() {
    while ( processInput() ) {
        draw();
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
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->drop();
        board->examine();
        board->setActiveBlock( level->createNew() );
    }
}

void QuadrisGame::levelUp( int multiplier ) {
  for(int times=0;times<multiplier;++times)
    {
      level->levelup();
    }
}

void QuadrisGame::levelDown( int multiplier ) {
   for(int times=0;times<multiplier;++times)
    {
      level->leveldown();
    }
}

void QuadrisGame::reset( int multiplier ) {
    score = 0;
    delete board;
    delete level;

    board = new Board(this);
    level = new Level ( board, this );

}

// -------------------------
// End of Command Functions
// -------------------------


