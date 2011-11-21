#include "CommandTrie.h"
#include <iostream>

using namespace std;

CommandTrie trie;

void command1() {
    cout << "I'm a piggie" << endl;
}

void command2() {
    cout << "I'm a doggie" << endl;
}

void command3() {
    cout << "I'm a kitty" << endl;
}

void command4() {
    cout << "I'm a kit. Lol. What's a kit?" << endl;
}

void command5() {
    cout << "I'm a dongaroo, mate." << endl;
}

void changeCommand() {
    string old_name;
    string new_name;
    if ( cin >> old_name && cin >> new_name ) {
        trie.changeCommand( old_name , new_name );
    }
}

int main() {
    trie.addCommand( "piggie" , &command1 );
    trie.addCommand( "doggie" , &command2 );
    trie.addCommand( "kitty" , &command3 );
    trie.addCommand( "kit" , &command4 );
    trie.addCommand( "dongaroo" , &command5 );
    trie.addCommand( "change" , &changeCommand );

    string command;
    while( cin >> command ) {
        commandFunctPtr fn = trie.findCommand( command );
        if ( fn != 0 ) {
            (*fn)();
        }
    }

    return 0;
}

