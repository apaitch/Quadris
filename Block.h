#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include "Colours.h"

class Board;

const int points_per_block = 4;

class Block {
    private:
        Board * board;
        char type;
        colour the_colour;
        std::pair<int , int> origin;
        int birth_level;
        int num_living_cells;
        std::pair<int , int> points [points_per_block];
        
        void tryTransformation( std::pair<int , int> [] , 
                                std::pair<int , int> );
        void move( int , int );
        void rotate( bool );
    public:
        Block ( char, std::pair<int , int> , int , Board * );
        void moveLeft();
        void moveRight();
        void moveDown();
        void rightRotate();
        void leftRotate();
        void drop();
        char getType() const;
        colour getColour() const;
        int getLevel() const;
        void getPoints( std::vector< std::pair< int , int > > & ) const;
        bool deleteCell();
};

#endif
