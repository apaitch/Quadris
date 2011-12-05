#include "QuadrisGame.h"
#include "Ai.h"
#include "Board.h"
#include "Level.h"
#include "Block.h"
#include "XWindow.h"
#include "CommandTrie.h"
#include "GameSettings.h"
#include "ScoreBoard.h"
#include <cstdlib>

using namespace std;

QuadrisGame::QuadrisGame( bool textOnly , bool aiOn , int seed ) 
                           : board( new Board(this) ) ,
                             commandInterpreter( new CommandTrie ) ,
                             level( new Level ( board , this , seed ) ) ,
                             scoreBoard( new ScoreBoard ( level->getLevel() ) ) ,
                             mainWindow( 0 ) ,
                             nextWindow( 0 ) ,
                             ai ( 0 ) ,
                             textOnly( textOnly ) {
    if ( ! textOnly ) {
        mainWindow = new Xwindow( mainWindowWidth , mainWindowHeight );
        nextWindow = new Xwindow( nextWindowWidth , nextWindowHeight );
    } // if

    if ( aiOn ) {
        ai = new Ai ( this , board );
    } // if

    initialize(); 
} // QuadrisGame 

QuadrisGame::~QuadrisGame() {
    delete mainWindow;
    delete board;
    delete commandInterpreter;
    delete level;
    delete scoreBoard;
} 

void QuadrisGame::initialize() {
    commandInterpreter->addCommand( "left" , &QuadrisGame::moveLeft );
    commandInterpreter->addCommand( "right" , &QuadrisGame::moveRight );
    commandInterpreter->addCommand( "clockwise" , &QuadrisGame::rightRotate );
    commandInterpreter->addCommand( "counterclockwise" , &QuadrisGame::leftRotate );
    commandInterpreter->addCommand( "down" , &QuadrisGame::moveDown );
    commandInterpreter->addCommand( "drop" , &QuadrisGame::drop );
    commandInterpreter->addCommand( "levelup" , &QuadrisGame::levelUp );
    commandInterpreter->addCommand( "leveldown" , &QuadrisGame::levelDown );
    commandInterpreter->addCommand( "restart" , &QuadrisGame::reset );
    board->setActiveBlock( level->getNextBlock() );

    if ( ai != 0 ) {
        levelUp( 4 );
    } // if

    output();
} // initialize()

void QuadrisGame::lineCleared( int numCleared ) {
    scoreBoard->linesCleared( numCleared );
}

void QuadrisGame::blockCleared( int level ) {
    scoreBoard->blockCleared( level );
}

// Take in and process one command.
bool QuadrisGame::processInput() {
    string fullCommand;

    if ( cin >> fullCommand ) {
        string command;
        int multiplier = 1;

        // If there is a numeric input, get multiplier
        if ( fullCommand[0] >= '0' && fullCommand[0] <= '9' ) {
            multiplier = 0;
            for ( unsigned int i = 0 ; i < fullCommand.size() ; ++i ) {
                char nextChar = fullCommand[i];
                if ( nextChar >= '0' && nextChar <= '9' ) {
                   multiplier = multiplier * 10 + (nextChar - 48); 
                } // if
                else {
                    command = fullCommand.substr(i);
                    break;
                } // else
            } // for
        } // if
        else {
            command = fullCommand;
        } // else

        commandFunctPtr commandFn = commandInterpreter->findCommand( command );    
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
    scoreBoard->print();
    cout << "----------" << endl;
    board->print();
    cout << "----------" << endl;
    cout << "Next:" << endl;
    level->printNext();
}

void QuadrisGame::draw() {
    mainWindow->fillRectangle( 0 , 0 , mainWindowWidth , mainWindowHeight);
    nextWindow->fillRectangle( 0 , 0 , nextWindowWidth , nextWindowHeight);

    board->draw( mainWindow );

    scoreBoard->draw( mainWindow );

    level->drawNext( nextWindow );
    
} // draw()

void QuadrisGame::output() {
    print();
    if ( ! textOnly ) {
        draw();
    } // if
}

void QuadrisGame::runGameLoop() {
    if ( ai == 0 ) {
        while ( processInput() ) {
            output();
        } // while
    } // if
    else {
        while ( true ) {
            vector< string > commands;
            ai->makeNextMove( commands );
            for ( unsigned int i = 0 ; i < commands.size() ; ++i ) {
                commandFunctPtr commandFn = 
                    commandInterpreter->findCommand( commands[i] );
                if ( commandFn != 0 ) {
                    CALL_MEMBER_FN(*this , commandFn)( 1 );
                    output();
                    usleep( 200000 );
                } // if
            } // for
        } // while
    } // else
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
    bool gameOver = false;
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->drop();
        board->examine();
        gameOver = board->setActiveBlock( level->getNextBlock() );
        if ( gameOver ) {
            reset( 1 );
            break;
        } // if
    } // for
}

void QuadrisGame::levelUp( int multiplier ) {
    for( int times = 0; times < multiplier ; ++times ) {
        level->levelUp();
    } // for
    scoreBoard->setLevel( level->getLevel() );
}

void QuadrisGame::levelDown( int multiplier ) {
    for( int times = 0 ; times < multiplier ; ++times ) {
        level->levelDown();
    } // for
    scoreBoard->setLevel( level->getLevel() );
}

void QuadrisGame::reset( int multiplier ) {
    delete board;
    delete level;

    if ( ai != 0 ) {
        delete ai;
    } // if

    board = new Board(this);
    level = new Level ( board, this );
    
    if ( ai != 0 ) {
        ai = new Ai( this , board );
    } // if

    scoreBoard->resetScore();
    scoreBoard->setLevel( level->getLevel() );
    board->setActiveBlock( level->getNextBlock() );
    output();

} // reset

// -------------------------
// End of Command Functions
// -------------------------

