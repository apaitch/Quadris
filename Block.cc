#include "Block.h"
#include "Board.h"

using namespace std;

// Overriding operators for pairs -------------------
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

Block::Block( blockType type , pair< int, int > createPoint , 
              int level , Board * board )
                : board( board ) , type( type ), startingOrigin( createPoint ), 
                  birthLevel( level ) , numLivingCells( pointsPerBlock ) {


    origin = createPoint;
    initializePoints();
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

/*
 * Moves each point in block by specified amounts horizontally and vertically.
 */
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

/*
 * Tries to rotates a block by multiplying by the rotation matrix.
 */
void Block::rotate( bool counterclockwise ) {
    pair< int , int > transformedPoints [pointsPerBlock];

    // Transform each point into local coordinate system, apply the rotation,
    // and then convert the result back into original coordinate system.
    for ( int i = 0 ; i < pointsPerBlock ; ++i ) {
        pair< int , int > currentPoint = points[i] - origin;
        
        int prevXCoord = currentPoint.first;
        if ( counterclockwise ) {
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
} // moveLeft() 

void Block::moveRight() {
    move ( 1 , 0 );
} // moveRight()

void Block::moveDown() {
    move ( 0 , 1 );
} // moveDown()

void Block::rightRotate() {
    rotate( false );
} // rightRotate()

void Block::leftRotate() {
    rotate( true );
}  // leftRotate()

/*
 * Calculates how far the block can drop. Used by drop() and by the Board class
 * to draw the block shadow.
 */
int Block::getDropAmount()
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
} // getDropAmount()

void Block::drop() {
    int furthestPossibleDrop = calculateDrop();
    move ( 0 , furthestPossibleDrop );
} // drop()

char Block::getType() const {
    return (char) type;
} // getType() 

colour Block::getColour() const {
    return blockColour;
}  // getColour()

int Block::getLevel() const {
    return birthLevel;
} // getLevel()

/*
 * Copies the points of the block into the provided vector.
 */
void Block::getPoints( vector< pair<int,int> > & pointsVector ) const {
    pointsVector.assign( points , points + pointsPerBlock );
} // getPoints()

/*
 * Called when a cell (i.e. point) in the block is deleted from the board.
 * Returns true if there are no more cells in the block and the block can be
 * completely deleted.
 */
bool Block::deleteCell() {
    numLivingCells -= 1;
    return (numLivingCells == 0) ? true : false;
} // deleteCell()

/*
 * Calculates the default position and colour of each of the block's points
 * depending on the block type and position of the origin.
 */
void Block::initializePoints() {
    pair< int , int > xIncrement = make_pair( 1 , 0 );
    pair< int , int > yIncrement = make_pair( 0 , 1 );

    switch ( type ) {
        case Z :
            points[0] = origin - yIncrement; 
            points[1] = origin - yIncrement + xIncrement;
            points[2] = origin + xIncrement;
            points[3] = origin + 2 * xIncrement;
            blockColour = Blue;
            break;
        case T :
            points[0] = origin - yIncrement;
            points[1] = origin - yIncrement + xIncrement;
            points[2] = origin - yIncrement + 2 * xIncrement;
            points[3] = origin + xIncrement; 
            blockColour = Orange;
            break;
        case I :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + 2 * xIncrement;
            points[3] = origin + 3 * xIncrement;
            blockColour = Red;
            break;
        case J :
            points[0] = origin - yIncrement;
            points[1] = origin;
            points[2] = origin + xIncrement;
            points[3] = origin + 2 * xIncrement;
            blockColour = Yellow;
            break;
        case L :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + 2 * xIncrement;
            points[3] = origin + 2 * xIncrement - yIncrement;
            blockColour = Green;
            break;
        case O :
            points[0] = origin - yIncrement;
            points[1] = origin;
            points[2] = origin - yIncrement + xIncrement;
            points[3] = origin + xIncrement;
            blockColour = Brown;
            break;
        case S :
            points[0] = origin;
            points[1] = origin + xIncrement;
            points[2] = origin + xIncrement - yIncrement;
            points[3] = origin + 2 * xIncrement - yIncrement;
            blockColour = Cyan;
    } // switch
} // initializePoints()

/*
 * Resets the block to it's starting position
 */
void Block::reset() {
    board->deleteBlock( this );
    origin = startingOrigin;
    initializePoints();
    board->setActiveBlock(this);
} // reset()
