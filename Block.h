#ifndef __BLOCK_H__
#define __BLOCK_H__

#define POINTS_PER_BLOCK 4

#include <iostream>
#include <utility>
#include <vector>

class Board;

class Block {
    private:
        Board * board;
        char type;
        int num_living_cells;
        int birth_level;
        std::pair<int , int> origin;
        std::pair<int , int> points [POINTS_PER_BLOCK];
        
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
        int getLevel() const;
        void getPoints( std::vector< std::pair< int , int > > & ) const;
        bool deleteCell();
};

#endif
