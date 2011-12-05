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
        blockType type;
        colour blockColour;

        std::pair<int , int> origin;
        int birthLevel;
        int numLivingCells;
        std::pair<int , int> points [ pointsPerBlock ];

        // For reset().
        std::pair<int , int> startingOrigin;

        
        void tryTransformation( std::pair<int , int> [] , 
                                std::pair<int , int> );
        void move( int , int );
        void rotate( bool );
        void initializePoints();
    public:
        Block ( blockType , std::pair<int , int> , int , Board * );
        bool deleteCell();

        // Movement interface
        void moveLeft();
        void moveRight();
        void moveDown();
        void rightRotate();
        void leftRotate();

	int getDropAmount();
        void drop();

        // For use by AI.
        void reset();

        char getType() const;
        colour getColour() const;
        int getLevel() const;
        void getPoints( std::vector< std::pair< int , int > > & ) const;
};

#endif
