#ifndef __COMMAND_TRIE_H__
#define __COMMAND_TRIE_H__

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
        CommandTrieNode * root_node;
    public:
        CommandTrie() : root_node( new CommandTrieNode ) { };
        ~CommandTrie() { delete root_node; };

        void addCommand( const std::string & , commandFunctPtr );
        void changeCommand( const std::string & , const std::string & );
        commandFunctPtr deleteCommand( const std::string & );

        // findCommand and deleteCommand returns pointers to the functions
        // containing the command in question.
        commandFunctPtr findCommand( const std::string & ) const;
};

#endif
