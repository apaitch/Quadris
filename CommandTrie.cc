#include "CommandTrie.h"
#include "QuadrisGame.h"

using namespace std;

CommandTrieNode::CommandTrieNode() : value(0),
                                     children(0) ,
                                     function(0) { }

CommandTrieNode::~CommandTrieNode() {
    int size = children.size();
    for ( int i = 0 ; i < size ; ++i ) {
        delete children.front();
        children.pop_front();
    }
}

// Returns a pointer to the specified child node, or 0 if the child doesn't
// exist.
CommandTrieNode * CommandTrieNode::findChild( char value ) {
    CommandTrieNode * return_node = 0;
    if ( children.size() > 0 ) {
        for ( list<CommandTrieNode *>::iterator it = children.begin() ;
                    it != children.end() ; ++it ) {
            if ( (*it)->value == value ) {
                return_node = *it;
                break;
            }
        }
    }

    return return_node;
}

// Returns a pointer to the specified child node, or 0 if the child doesn't
// exist. Also deletes the child node. (Yes, this reuses code from above).
CommandTrieNode * CommandTrieNode::deleteChild( char value ) {
    CommandTrieNode * return_node = 0;
    if ( children.size() > 0 ) {
        for ( list<CommandTrieNode *>::iterator it = children.begin() ;
                    it != children.end() ; ++it ) {
            if ( (*it)->value == value ) {
                return_node = *it;
                children.erase( it );
                break;
            }
        }
    }

    return return_node;
}

void CommandTrie::addCommand( const string &  command , commandFunctPtr fn ) {
    CommandTrieNode * current_node = root_node;
    // Find path that matches the string, creating new nodes if necessary.
    for ( int i = 0 ; i < command.size() ; ++i ) {
        char value = command[i];
        CommandTrieNode * next_node = current_node->findChild( value );
        if ( next_node == 0 ) {
            next_node = new CommandTrieNode;
            next_node->value = value;
            current_node->children.push_front( next_node );
        }
        current_node = next_node;
    }
    current_node->function = fn;
}

commandFunctPtr CommandTrie::findCommand( const string & command ) const {

    CommandTrieNode * current_node = root_node;
    commandFunctPtr return_function = 0;
    bool not_in_trie = false;

    for ( int i = 0 ; i < command.size() ; ++i ) {
        char value = command[i];
        current_node = current_node->findChild( value );
        if ( current_node == 0 ) {
            not_in_trie = true;
            break;
        }
    }

    if ( ! not_in_trie ) {
        if ( current_node->function != 0 ) {
            return_function = current_node->function;
        }
        else {
            while( ( current_node->children.size() == 1 ) 
                   && ( current_node->function == 0 ) ) {
                current_node = current_node->children.front();
            }
            if ( current_node->children.size() == 0 ) {
                return_function = current_node->function;
            }
        }
    }

    return return_function;
}

commandFunctPtr CommandTrie::deleteCommand( const string & command ) {
    CommandTrieNode * current_node = root_node;
    CommandTrieNode * last_shared_node = root_node;
    char value_at_shared_node = 0;
    commandFunctPtr return_fn = 0;

    for ( int i = 0 ; i < command.size() ; ++i ) {
        char value = command[ i ];
        CommandTrieNode * next_node = current_node->findChild( value );
        if ( next_node == 0 ) {
            return 0;
        }
        if ( current_node->children.size() > 1 ) {
            last_shared_node = current_node;
            value_at_shared_node = value;
        }
        current_node = next_node;
    }
    
    if ( current_node->function != 0 ) {
        return_fn = current_node->function;
        current_node->function = 0;

        if ( current_node->children.size() == 0 ) {
            delete last_shared_node->deleteChild( value_at_shared_node );
        }
    }

    return return_fn;
}

void CommandTrie::changeCommand( const string & old_name , 
                                 const string & new_name ) {
    commandFunctPtr command = deleteCommand( old_name );
    if ( command != 0 ) {
        addCommand( new_name , command );
    }
}

