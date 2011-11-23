#include "Block.h"

using namespace std;

// Overriding operators for points -------------------
pair< int , int > operator+( const pair<int,int> & p1 , const pair<int,int> & p2 ) {
    return make_pair( p1.first + p2.first , p1.second + p2.second );
}

pair< int , int > operator-( const pair<int,int> & p1 , const pair<int,int> & p2 ) {
    return make_pair( p1.first - p2.first , p1.second - p2.second );
}

pair< int , int > operator*( const int & i , const pair<int,int> & p1 ) {
    return make_pair( i * p1.first , i * p1.second );
}
// ---------------------------------------------------

Block::Block( char type , pair< int, int > create_point , int level , Board * board )
                : type( type ), origin( create_point ), 
                  birth_level(level) , board( board ) {

    pair< int , int > x_increment = make_pair( 1 , 0 );
    pair< int , int > y_increment = make_pair( 0 , 1 );

    switch ( type ) {
        case 'Z' :
            points[0] = origin - y_increment; 
            points[1] = origin - y_increment + x_increment;
            points[2] = origin + x_increment;
            points[3] = origin + 2 * x_increment;
            break;
        case 'T' :
            points[0] = origin - y_increment;
            points[1] = origin - y_increment + x_increment;
            points[2] = origin - y_increment + 2 * x_increment;
            points[3] = origin + x_increment;
            break;
        case 'I' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + 2 * x_increment;
            points[3] = origin + 3 * x_increment;
            break;
        case 'J' :
            points[0] = origin - y_increment;
            points[1] = origin;
            points[2] = origin + x_increment;
            points[3] = origin + 2 * x_increment;
            break;
        case 'L' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + 2 * x_increment;
            points[3] = origin + 2 * x_increment - y_increment;
            break;
        case 'O' :
            points[0] = origin - y_increment;
            points[1] = origin;
            points[2] = origin - y_increment + x_increment;
            points[3] = origin + x_increment;
            break;
        case 'S' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + x_increment - y_increment;
            points[3] = origin + 2 * x_increment - y_increment;
    }
}

// A transformation (e.g. rotate, move) generates an array of transformed points
// and passes it to tryTransformation(). If the transformation doesn't result in
// collisions in the board then tryTransformation() applies the transformation.
void Block::tryTransformation( pair< int , int > transformed_points [] ) {
    bool transformation_success = true;

    // Check for collisions
    for ( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
        Block * block_at_point = board->getBlockPtr( transformed_points[i] );
        if ( block_at_point != this && block_at_point != 0 ) {
            transformation_success = false;
            break;
        }
    }

    if ( transformation_success ) {
        board->deleteBlock( points );
        for ( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
            points[i] = transformed_points[i];
        }
        board->addBlock( points , this );
    }
}

// Moves each point in block by specified x and y coordinates.
void Block::move( int x , int y ) {
    pair< int , int > transformed_points [POINTS_PER_BLOCK];
    for ( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
        transformed_points[i] 
            = make_pair( points[i].first + x , points[i].second + y );
    }
    tryTransformation( transformed_points );
}

// Rotates block right if "left" is false.
void Block::rotate( bool left ) {
    pair< int , int > transformed_points [POINTS_PER_BLOCK];

    // Transform each point into local coordinate system, apply the rotation,
    // and then convert the result back into original coordinate system.
    for ( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
        pair< int , int > current_point = points[i] - origin;
        
        int prev_x_coord = current_point.first;
        if ( left ) {
            current_point.first = current_point.second;
            current_point.second = prev_x_coord * -1;
        }
        else {
            current_point.first = current_point.second -1;
            current_point.second = prev_x_coord;
        }

        current_point = current_point + origin;
        transformed_points[i] = current_point;
    }

    // Shift the transformed points in line with the origin to get final result
    // of transformation
    for ( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
        int x_coord_diff = origin.first - transformed_points[i].first;
        int y_coord_diff = transformed_points[i].second - origin.second;
        if ( x_coord_diff > 0 ) {
            for ( int j = 0 ; j < POINTS_PER_BLOCK ; ++j ) {
                transformed_points[j].first += x_coord_diff;
            }
        }
        if ( y_coord_diff > 0 ) {
            for ( int j = 0 ; j < POINTS_PER_BLOCK ; ++j ) {
                transformed_points[j].second -= y_coord_diff;
            }
        }
    }

    tryTransformation( transformed_points );
}

void Block::moveLeft() {
    move( -1 , 0 );
}

void Block::moveRight() {
    move ( 1 , 0 );
}

void Block::moveDown() {
    move ( 0 , 1 );
}

void Block::rightRotate() {
    rotate( false );
}

void Block::leftRotate() {
    rotate( true );
}

void Block::drop() {
    int furthest_possible_drop = 14;

    // Look down from each of the low points in the block to see how far can
    // drop.
    for( int i = 0 ; i < POINTS_PER_BLOCK ; ++i ) {
        if ( points[i].second == origin.second ) {
            int possible_drop = 0;
            for ( int j = origin.second ; j < 15 ; ++j ) {
                pair< int , int > current_point = 
                    make_pair( points[i].first, origin.second );
                if ( board->getBlockPtr( current_point ) == 0 ) {
                    possible_drop += 1;
                }
            }
            if ( possible_drop < furthest_possible_drop ) {
                furthest_possible_drop = possible_drop;
            }
        }
    }

    move ( 0 , furthest_possible_drop );
}

char Block::getType() const {
    return type;
}
