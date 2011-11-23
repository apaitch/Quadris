#include "QuadrisGame.h"

using namespace std;

QuadrisGame::QuadrisGame() : board( new Board ) ,
                             command_interpreter( new CommandTrie ) ,
                             level( new LevelOne ) ,
                             high_scocre( 0 ) ,
                             score( 0 ) { initialize(); }

QuadrisGame::~QuadrisGame() {
    delete board;
    delete command_interpreter;
    delete level;
}

void QuadrisGame::initialize() {
    command_interpreter.addCommand( "left" , &moveLeft() );
    command_interpreter.addCommand( "right" , &moveRight() );
    command_interpreter.addCommand( "clockwise" , &rightRotate() );
    command_interpreter.addCommand( "counterclockwise" , &leftRotate() );
    command_interpreter.addCommand( "drop" , &drop() );
    command_interpreter.addCommand( "levelup" , &levelUp() );
    command_interpreter.addCommand( "leveldown" , &levelDown() );
    command_interpreter.addCommand( "restart" , &reset() );
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
    if ( cin >> command ) {
        commandFunctPtr = command_interpreter.findCommand( command );    
        if ( commandFunctPtr != 0 ) {
            (*commandFunctPtr)();
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
    cout << "------------" << endl;
    board->print();
    cout << "------------" << endl;
    cout << "Next:" << endl;
    level->printNext();
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
}

void QuadrisGame::levelUp() {
}

void QuadrisGame::levelDown() {
}

void QuadrisGame::reset() {
    score = 0;
    delete board;
    delete level;

    board = new Board;
    level = new LevelOne;
}

// -------------------------
// End of Command Functions
// -------------------------


