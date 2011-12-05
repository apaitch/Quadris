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
                             aiOn ( aiOn ) ,
                             gameOver ( false ) ,
                             textOnly( textOnly ) {
    if ( ! textOnly ) {
        mainWindow = new Xwindow( mainWindowWidth , mainWindowHeight );
        nextWindow = new Xwindow( nextWindowWidth , nextWindowHeight );
    } // if

    // Don't bother creating Ai object if the -ai flag is not set - it won't be
    // used.
    if ( aiOn ) {
        ai = new Ai ( this , board );
    } // if

    commandInterpreter->addCommand( "left" , &QuadrisGame::moveLeft );
    commandInterpreter->addCommand( "right" , &QuadrisGame::moveRight );
    commandInterpreter->addCommand( "clockwise" , &QuadrisGame::rightRotate );
    commandInterpreter->addCommand( "counterclockwise" , &QuadrisGame::leftRotate );
    commandInterpreter->addCommand( "down" , &QuadrisGame::moveDown );
    commandInterpreter->addCommand( "drop" , &QuadrisGame::drop );
    commandInterpreter->addCommand( "levelup" , &QuadrisGame::levelUp );
    commandInterpreter->addCommand( "leveldown" , &QuadrisGame::levelDown );
    commandInterpreter->addCommand( "restart" , &QuadrisGame::reset );
    commandInterpreter->addCommand( "rename" , &QuadrisGame::rename );

    board->setActiveBlock( level->getNextBlock() );

    output();

} // QuadrisGame 

QuadrisGame::~QuadrisGame() {
    delete mainWindow;
    delete board;
    delete commandInterpreter;
    delete level;
    delete scoreBoard;
} 

/*
 * Uses the command interpreter to process a single command from the user.
 */
bool QuadrisGame::processInput() {
    string fullCommand;

    if ( cin >> fullCommand ) {
        string command;
        int multiplier = 1;

        // If there is a numeric prefix, use it as a multipler
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

/*
 * Prints the game state to stdout.
 */
void QuadrisGame::print() {
    scoreBoard->print();
    cout << "----------" << endl;
    board->print();
    cout << "----------" << endl;
    cout << "Next:" << endl;
    level->printNext();
} // print()

/*
 * Draws the current state of the game. NOTE: Does not draw only the parts that
 * changed. Instead, fills the windows with black and draws everything.
 */
void QuadrisGame::draw() {
    mainWindow->fillRectangle( 0 , 0 , mainWindowWidth , mainWindowHeight);
    nextWindow->fillRectangle( 0 , 0 , nextWindowWidth , nextWindowHeight);

    board->draw( mainWindow );

    scoreBoard->draw( mainWindow );

    level->drawNext( nextWindow );
    
} // draw()

/*
 * The main output function.
 */
void QuadrisGame::output() {
    print();
    if ( ! textOnly ) {
        draw();
    } // if
} // output()

void QuadrisGame::lineCleared( int numCleared ) {
    scoreBoard->linesCleared( numCleared );
} // lineCleared()

void QuadrisGame::blockCleared( int level ) {
    scoreBoard->blockCleared( level );
} // blockCleared()

/*
 * The engine of the game. Called after QuadrisGame is created and initialized
 * in order to actually start playing.
 */
void QuadrisGame::runGameLoop() {
    // User input version
    if ( ai == 0 ) {
        while ( processInput() ) {
            if ( gameOver ) {
                reset( 1 );
            }
            output();
        } // while
    } // if
    // AI version
    else {
        while ( ! gameOver ) {
            vector< string > commands;
            ai->getNextMove( commands );
            for ( unsigned int i = 0 ; i < commands.size() ; ++i ) {
                commandFunctPtr commandFn = 
                    commandInterpreter->findCommand( commands[i] );
                if ( commandFn != 0 ) {
                    CALL_MEMBER_FN(*this , commandFn)( 1 );
                    output();
                    usleep( 50000 );
                } // if
            } // for
        } // while
    } // else
} // runGameLoop()

// -------------------------
// Defining Command Functions
// -------------------------
void QuadrisGame::rightRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->rightRotate();
    } // for
} // rightRotate()

void QuadrisGame::leftRotate( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->leftRotate();
    } // for
}

void QuadrisGame::moveLeft( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveLeft();
    } // for
} // moveLeft()

void QuadrisGame::moveRight( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveRight();
    } // for
} // moveRight()

void QuadrisGame::moveDown( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->moveDown();
    } // for
} // moveDown()

/*
 * Drops a specified number of next blocks, stopping and resetting if the 
 * dropping results in a game over.
 */
void QuadrisGame::drop( int multiplier ) {
    for ( int i = 0 ; i < multiplier ; ++i ) {
        board->getActiveBlock()->drop();
        board->clearFilledRows();
        gameOver = board->setActiveBlock( level->getNextBlock() );
        if ( gameOver ) {
            break;
        } // if
    } // for
} // drop()

void QuadrisGame::levelUp( int multiplier ) {
    for( int times = 0; times < multiplier ; ++times ) {
        level->levelUp();
    } // for
    scoreBoard->setLevel( level->getLevel() );
} // levelUp()

void QuadrisGame::levelDown( int multiplier ) {
    for( int times = 0 ; times < multiplier ; ++times ) {
        level->levelDown();
    } // for
    scoreBoard->setLevel( level->getLevel() );
} // levelDown()

/*
 * Resets the game once, ignoring its multiplier.
 */
void QuadrisGame::reset( int multiplier ) {
    gameOver = false;
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
} // reset()

/*
 * Renames a command according to user input. 
 */
void QuadrisGame::rename( int multiplier ) {
    string oldName;
    string newName;
    if ( cin >> oldName && cin >> newName ) {
        // Ignore new names that start with a numeric prefix ( because of the
        // way processInput() handles numeric prefixes )
        if ( ( newName[0] < '0' ) || ( newName[0] > '9' ) ) {
            commandInterpreter->changeCommand( oldName , newName );
        } // if
        else {
            cerr << "Ignoring new name - " << newName; 
            cerr << " due to numeric prefix." << endl;
        } // else
    } // if
    else {
        cerr << "rename command expects two arguments" << endl;
    } // else
} // rename()

// -------------------------
// End of Command Functions
// -------------------------

