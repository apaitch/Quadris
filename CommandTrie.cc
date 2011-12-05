#include "CommandTrie.h"
#include "QuadrisGame.h"

using namespace std;

CommandTrieNode::CommandTrieNode() : value(0),
                                     children(0) ,
                                     function(0) { } // CommandTrieNode()

CommandTrieNode::~CommandTrieNode() {
    int size = children.size();
    for ( int i = 0 ; i < size ; ++i ) {
        delete children.front();
        children.pop_front();
    } // for
} // ~CommandTrieNode

// Returns a pointer to the specified child node, or 0 if the child doesn't
// exist.
CommandTrieNode * CommandTrieNode::findChild( char value ) {
    CommandTrieNode * returnNode = 0;
    if ( children.size() > 0 ) {
        for ( list<CommandTrieNode *>::iterator it = children.begin() ;
                    it != children.end() ; ++it ) {
            if ( (*it)->value == value ) {
                returnNode = *it;
                break;
            } // if
        } // for
    } // if

    return returnNode;
} // findChild()

// Returns a pointer to the specified child node, or 0 if the child doesn't
// exist. Also deletes the child node. (Yes, this reuses code from above).
CommandTrieNode * CommandTrieNode::deleteChild( char value ) {
    CommandTrieNode * returnNode = 0;
    if ( children.size() > 0 ) {
        for ( list<CommandTrieNode *>::iterator it = children.begin() ;
                    it != children.end() ; ++it ) {
            if ( (*it)->value == value ) {
                returnNode = *it;
                children.erase( it );
                break;
            } // if
        } // for
    } // if

    return returnNode;
} // deleteChild()

void CommandTrie::addCommand( const string &  command , commandFunctPtr fn ) {
    CommandTrieNode * currentNode = rootNode;
    // Find path that matches the string, creating new nodes if necessary.
    for ( unsigned int i = 0 ; i < command.size() ; ++i ) {
        char value = command[i];
        CommandTrieNode * nextNode = currentNode->findChild( value );
        if ( nextNode == 0 ) {
            nextNode = new CommandTrieNode;
            nextNode->value = value;
            currentNode->children.push_front( nextNode );
        } // if
        currentNode = nextNode;
    } // for
    currentNode->function = fn;
} // addCommand()

commandFunctPtr CommandTrie::findCommand( const string & command ) const {

    CommandTrieNode * currentNode = rootNode;
    commandFunctPtr returnFunction = 0;
    bool notInTrie = false;

    for ( unsigned int i = 0 ; i < command.size() ; ++i ) {
        char value = command[i];
        currentNode = currentNode->findChild( value );
        if ( currentNode == 0 ) {
            notInTrie = true;
            break;
        } // if
    } // for

    if ( ! notInTrie ) {
        if ( currentNode->function != 0 ) {
            returnFunction = currentNode->function;
        } // if
        else {
            while( ( currentNode->children.size() == 1 ) 
                   && ( currentNode->function == 0 ) ) {
                currentNode = currentNode->children.front();
            } // while
            if ( currentNode->children.size() == 0 ) {
                returnFunction = currentNode->function;
            } // if
        } // else
    } // if

    return returnFunction;
} // findCommand()

commandFunctPtr CommandTrie::deleteCommand( const string & command ) {
    CommandTrieNode * currentNode = rootNode;
    CommandTrieNode * lastSharedNode = rootNode;
    char valueAtSharedNode = 0;
    commandFunctPtr returnFn = 0;

    for ( unsigned int i = 0 ; i < command.size() ; ++i ) {
        char value = command[ i ];
        CommandTrieNode * nextNode = currentNode->findChild( value );
        if ( nextNode == 0 ) {
            return 0;
        } // if
        if ( currentNode->children.size() > 1 ) {
            lastSharedNode = currentNode;
            valueAtSharedNode = value;
        } // if
        currentNode = nextNode;
    } // for
    
    if ( currentNode->function != 0 ) {
        returnFn = currentNode->function;
        currentNode->function = 0;

        if ( currentNode->children.size() == 0 ) {
            delete lastSharedNode->deleteChild( valueAtSharedNode );
        } // if
    } // if

    return returnFn;
} // deleteCommand()

void CommandTrie::changeCommand( const string & oldName , 
                                 const string & newName ) {
    commandFunctPtr command = deleteCommand( oldName );
    if ( command != 0 ) {
        addCommand( newName , command );
    } // if
} // changeCommand()

