#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include "Colours.h"
#include "GameSettings.h"

class Board;

class Block {
    private:
        Board * board;
        char type;
        colour theColour;
        std::pair<int , int> origin;
        std::pair<int , int> startingOrigin;
        int birthLevel;
        int numLivingCells;
        std::pair<int , int> points [pointsPerBlock];
        
        void tryTransformation( std::pair<int , int> [] , 
                                std::pair<int , int> );
        void move( int , int );
        void rotate( bool );
        void fillPoints();
    public:
        Block ( char, std::pair<int , int> , int , Board * );
        bool deleteCell();

        // Movement interface
        void moveLeft();
        void moveRight();
        void moveDown();
        void rightRotate();
        void leftRotate();
	int calculateDrop();
        void drop();

        void reset();

        char getType() const;
        colour getColour() const;
        int getLevel() const;
        void getPoints( std::vector< std::pair< int , int > > & ) const;
};

#endif
