#ifndef __COMMANDTRIE_H__
#define __COMMANDTRIE_H__

#include <string>
#include <list>
#include <cassert>
#include <iostream>

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class QuadrisGame;

// A pointer to a command function. See how it's used in main.cc
typedef void (QuadrisGame::*commandFunctPtr) ( int );

struct CommandTrieNode {
    char value;
    // Keep children in a linked list
    std::list<CommandTrieNode *> children;
    commandFunctPtr function;

    CommandTrieNode();
    ~CommandTrieNode();
    CommandTrieNode * findChild ( char );
    CommandTrieNode * deleteChild ( char );
};

class CommandTrie {
    private:
        CommandTrieNode * rootNode;
    public:
        CommandTrie() : rootNode( new CommandTrieNode ) { };
        ~CommandTrie() { delete rootNode; };

        void addCommand( const std::string & , commandFunctPtr );
        void changeCommand( const std::string & , const std::string & );
        commandFunctPtr deleteCommand( const std::string & );

        // findCommand and deleteCommand returns pointers to the functions
        // containing the command in question.
        commandFunctPtr findCommand( const std::string & ) const;
};

#endif
