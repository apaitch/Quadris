#include "QuadrisGame.h"
#include "Board.h"
#include "Level.h"
#include "Block.h"

using namespace std;

QuadrisGame::QuadrisGame() : board( new Board ) ,
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

void QuadrisGame::lineCleared( int num_cleared , int level ) {
    score += ( num_cleared + level ) * ( num_cleared + level );
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
        commandFunctPtr command = command_interpreter->findCommand( full_command );    
        if ( command != 0 ) {
            CALL_MEMBER_FN(*this , command)();
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
void QuadrisGame::rightRotate() {
    board->getActiveBlock()->rightRotate();
}

void QuadrisGame::leftRotate() {
    board->getActiveBlock()->leftRotate();
}

void QuadrisGame::moveLeft() {
    board->getActiveBlock()->moveLeft();
}

void QuadrisGame::moveRight() {
    board->getActiveBlock()->moveRight();
}

void QuadrisGame::moveDown() {
    board->getActiveBlock()->moveDown();
}

void QuadrisGame::drop() {
    board->getActiveBlock()->drop();
    board->examine();
    board->setActiveBlock( level->createNew() );
}

void QuadrisGame::levelUp() {
  level->levelup();
}

void QuadrisGame::levelDown() {
  level->leveldown();
}

void QuadrisGame::reset() {
    score = 0;
    delete board;
    delete level;

    board = new Board;
    level = new Level ( board, this );

}

// -------------------------
// End of Command Functions
// -------------------------


