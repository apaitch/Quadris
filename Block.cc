#include "Block.h"
#include "Board.h"

using namespace std;

// Overriding operators for points -------------------
pair< int , int > operator+( const pair<int,int> & p1 , const pair<int,int> & p2 ) {
    return make_pair( p1.first + p2.first , p1.second + p2.second );
} // operator+

pair< int , int > operator-( const pair<int,int> & p1 , const pair<int,int> & p2 ) {
    return make_pair( p1.first - p2.first , p1.second - p2.second );
} // operator-

pair< int , int > operator*( const int & i , const pair<int,int> & p1 ) {
    return make_pair( i * p1.first , i * p1.second );
} // operator*
// ---------------------------------------------------

Block::Block( char type , pair< int, int > create_point , int level , Board * board )
                : board( board ) , type( type ), origin( create_point ), 
                  birth_level(level) , num_living_cells(4) {

    pair< int , int > x_increment = make_pair( 1 , 0 );
    pair< int , int > y_increment = make_pair( 0 , 1 );

    switch ( type ) {
        case 'Z' :
            points[0] = origin - y_increment; 
            points[1] = origin - y_increment + x_increment;
            points[2] = origin + x_increment;
            points[3] = origin + 2 * x_increment;
            the_colour = Blue;
            break;
        case 'T' :
            points[0] = origin - y_increment;
            points[1] = origin - y_increment + x_increment;
            points[2] = origin - y_increment + 2 * x_increment;
            points[3] = origin + x_increment; 
            the_colour = Orange;
            break;
        case 'I' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + 2 * x_increment;
            points[3] = origin + 3 * x_increment;
            the_colour = Red;
            break;
        case 'J' :
            points[0] = origin - y_increment;
            points[1] = origin;
            points[2] = origin + x_increment;
            points[3] = origin + 2 * x_increment;
            the_colour = Yellow;
            break;
        case 'L' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + 2 * x_increment;
            points[3] = origin + 2 * x_increment - y_increment;
            the_colour = Green;
            break;
        case 'O' :
            points[0] = origin - y_increment;
            points[1] = origin;
            points[2] = origin - y_increment + x_increment;
            points[3] = origin + x_increment;
            the_colour = Brown;
            break;
        case 'S' :
            points[0] = origin;
            points[1] = origin + x_increment;
            points[2] = origin + x_increment - y_increment;
            points[3] = origin + 2 * x_increment - y_increment;
            the_colour = Cyan;
    } // switch
} // Block

// A transformation (e.g. rotate, move) generates an array of transformed points
// and passes it to tryTransformation(). If the transformation doesn't result in
// collisions in the board then tryTransformation() applies the transformation.
void Block::tryTransformation( pair< int , int > transformed_points [] ,
                               pair< int , int > transformed_origin ) {
    bool transformation_success = true;

    // Check for collisions
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        if ( board->cellOccupied( this , transformed_points[i] ) ) {
            transformation_success = false;
            break;
        } // if
    } // for

    if ( transformation_success ) {
        board->deleteBlock( this );
        origin = transformed_origin;
        for ( int i = 0 ; i < points_per_block ; ++i ) {
            points[i] = transformed_points[i];
        } // for
        board->addBlock( this );
    } // if
} // tryTransformation()

// Moves each point in block by specified x and y coordinates.
void Block::move( int x , int y ) {
    pair< int , int > transformed_points [points_per_block];
    pair< int , int > transformed_origin;
    transformed_origin = make_pair( origin.first + x , origin.second + y );
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        transformed_points[i] 
            = make_pair( points[i].first + x , points[i].second + y );
    } // for
    tryTransformation( transformed_points , transformed_origin );
} // move()

// Rotates block right if "left" is false.
void Block::rotate( bool left ) {
    pair< int , int > transformed_points [points_per_block];

    // Transform each point into local coordinate system, apply the rotation,
    // and then convert the result back into original coordinate system.
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        pair< int , int > current_point = points[i] - origin;
        
        int prev_x_coord = current_point.first;
        if ( left ) {
            current_point.first = current_point.second;
            current_point.second = prev_x_coord * -1;
        } // if
        else {
            current_point.first = current_point.second * -1;
            current_point.second = prev_x_coord;
        } // else

        current_point = current_point + origin;
        transformed_points[i] = current_point;
    } // for

    // Shift the transformed points in line with the origin to get final result
    // of transformation
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        int x_coord_diff = origin.first - transformed_points[i].first;
        int y_coord_diff = transformed_points[i].second - origin.second;
        if ( x_coord_diff > 0 ) {
            for ( int j = 0 ; j < points_per_block ; ++j ) {
                transformed_points[j].first += x_coord_diff;
            } // for
        } // if
        if ( y_coord_diff > 0 ) {
            for ( int j = 0 ; j < points_per_block ; ++j ) {
                transformed_points[j].second -= y_coord_diff;
            } // for
        } // if
    } // for

    tryTransformation( transformed_points , origin );
} // rotate() 

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
    int furthest_possible_drop = num_rows;

    // Look down from each point in the block to see how far can
    // drop.
    for( int i = 0 ; i < points_per_block ; ++i ) {
        int possible_drop = 0;
        for ( int j = points[i].second + 1 ; j < num_rows ; ++j ) {
            pair< int , int > current_point = 
                make_pair( points[i].first, j );
            if ( ! board->cellOccupied( this , current_point ) ) {
                possible_drop += 1;
            } // if
            else {
                break;
            } // else
        } // for
        if ( possible_drop < furthest_possible_drop ) {
            furthest_possible_drop = possible_drop;
        } // if
    } // for

    move ( 0 , furthest_possible_drop );
} // drop() 

char Block::getType() const {
    return type;
} 

colour Block::getColour() const {
    return the_colour;
} 

int Block::getLevel() const {
    return birth_level;
}

void Block::getPoints( vector< pair<int,int> > & points_vector ) const {
    points_vector.assign( points , points + points_per_block );
}

bool Block::deleteCell() {
    num_living_cells -= 1;
    return (num_living_cells == 0) ? true : false;
}
