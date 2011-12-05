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

Block::Block( char type , pair< int, int > createPoint , int level , Board * board )
                : board( board ) , type( type ), startingOrigin( createPoint ), 
                  birthLevel(level) , numLivingCells(4) {


    origin = createPoint;
    fillPoints();
} // Block

// A transformation (e.g. rotate, move) generates an array of transformed points
// and passes it to tryTransformation(). If the transformation doesn't result in
// collisions in the board then tryTransformation() applies the transformation.
void Block::tryTransformation( pair< int , int > transformedPoints [] ,
                               pair< int , int > transformedOrigin ) {
    bool transformationSuccess = true;

    // Check for collisions
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        if ( board->cellOccupied( this , transformedPoints[i] ) ) {
            transformationSuccess = false;
            break;
        } // if
    } // for

    if ( transformationSuccess ) {
        board->deleteBlock( this );
        origin = transformedOrigin;
        for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
            points[i] = transformedPoints[i];
        } // for
        board->addBlock( this );
    } // if
} // tryTransformation()

// Moves each point in block by specified x and y coordinates.
void Block::move( int x , int y ) {
    pair< int , int > transformedPoints [pointsPerBlock];
    pair< int , int > transformedOrigin;
    transformedOrigin = make_pair( origin.first + x , origin.second + y );
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        transformedPoints[i] 
            = make_pair( points[i].first + x , points[i].second + y );
    } // for
    tryTransformation( transformedPoints , transformedOrigin );
} // move()

// Rotates block right if "left" is false.
void Block::rotate( bool left ) {
    pair< int , int > transformedPoints [pointsPerBlock];

    // Transform each point into local coordinate system, apply the rotation,
    // and then convert the result back into original coordinate system.
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        pair< int , int > currentPoint = points[i] - origin;
        
        int prevXCoord = currentPoint.first;
        if ( left ) {
            currentPoint.first = currentPoint.second;
            currentPoint.second = prevXCoord * -1;
        } // if
        else {
            currentPoint.first = currentPoint.second * -1;
            currentPoint.second = prevXCoord;
        } // else

        currentPoint = currentPoint + origin;
        transformedPoints[i] = currentPoint;
    } // for

    // Shift the transformed points in line with the origin to get final result
    // of transformation
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        int xCoordDiff = origin.first - transformedPoints[i].first;
        int yCoordDiff = transformedPoints[i].second - origin.second;
        if ( xCoordDiff > 0 ) {
            for ( int j = 0 ; j < pointsPerBlock ; ++j ) {
                transformedPoints[j].first += xCoordDiff;
            } // for
        } // if
        if ( yCoordDiff > 0 ) {
            for ( int j = 0 ; j < pointsPerBlock ; ++j ) {
                transformedPoints[j].second -= yCoordDiff;
            } // for
        } // if
    } // for

    tryTransformation( transformedPoints , origin );
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

//helper function to see how much a block can drop
int Block::calculateDrop()
{
   int furthestPossibleDrop = numRows;

    // Look down from each point in the block to see how far can
    // drop.
    for( int i = 0 ; i < pointsPerBlock ; ++i ) {
        int possibleDrop = 0;
        for ( int j = points[i].second + 1 ; j < numRows ; ++j ) {
            pair< int , int > currentPoint = 
                make_pair( points[i].first, j );
            if ( ! board->cellOccupied( this , currentPoint ) ) {
                possibleDrop += 1;
            } // if
            else {
                break;
            } // else
        } // for
        if ( possibleDrop < furthestPossibleDrop ) {
            furthestPossibleDrop = possibleDrop;
        }
    }
    return furthestPossibleDrop;
}
void Block::drop() {

  int furthestPossibleDrop=this->calculateDrop();
  move ( 0 , furthestPossibleDrop );
}

char Block::getType() const {
    return type;
} 

colour Block::getColour() const {
    return theColour;
} 

int Block::getLevel() const {
    return birthLevel;
}

void Block::getPoints( vector< pair<int,int> > & pointsVector ) const {
    pointsVector.assign( points , points + pointsPerBlock );
}

bool Block::deleteCell() {
    numLivingCells -= 1;
    return (numLivingCells == 0) ? true : false;
}

void Block::fillPoints() {
    pair< int , int > xIncrement = make_pair( 1 , 0 );
    pair< int , int > yIncrement = make_pair( 0 , 1 );

    switch ( type ) {
        case 'Z' :
            points[0] = origin - yIncrement; 
            points[1] = origin - yIncrement + xIncrement;
            points[2] = origin + xIncrement;
            points[3] = origin + 2 * xIncrement;
            theColour = Blue;
            break;
        case 'T' :
            points[0] = origin - yIncrement;
            points[1] = origin - yIncrement + xIncrement;
            points[2] = origin - yIncrement + 2 * xIncrement;
            points[3] = origin + xIncrement; 
            theColour = Orange;
            break;
        case 'I' :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + 2 * xIncrement;
            points[3] = origin + 3 * xIncrement;
            theColour = Red;
            break;
        case 'J' :
            points[0] = origin - yIncrement;
            points[1] = origin;
            points[2] = origin + xIncrement;
            points[3] = origin + 2 * xIncrement;
            theColour = Yellow;
            break;
        case 'L' :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + 2 * xIncrement;
            points[3] = origin + 2 * xIncrement - yIncrement;
            theColour = Green;
            break;
        case 'O' :
            points[0] = origin - yIncrement;
            points[1] = origin;
            points[2] = origin - yIncrement + xIncrement;
            points[3] = origin + xIncrement;
            theColour = Brown;
            break;
        case 'S' :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + xIncrement - yIncrement;
            points[3] = origin + 2 * xIncrement - yIncrement;
            theColour = Cyan;
    } // switch
}

// Resets the block to it's default position, using specified point as the
// origin.
void Block::reset() {
    board->deleteBlock( this );
    origin = startingOrigin;
    fillPoints();
    board->setActiveBlock(this);
}
