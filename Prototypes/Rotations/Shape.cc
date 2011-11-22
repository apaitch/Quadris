#include "Shape.h"

using namespace std;

point operator-( const point & p1 , const point & p2 ) {
    point new_p;
    new_p.x_coord = p1.x_coord - p2.x_coord;
    new_p.y_coord = p1.y_coord - p2.y_coord;
    return new_p;
}

point operator+( const point & p1 , const point & p2 ) {
    point new_p;
    new_p.x_coord = p1.x_coord + p2.x_coord;
    new_p.y_coord = p1.y_coord + p2.y_coord;
    return new_p;
}

// Rotation 90 degrees clockwise (in left-handed coordinate system).
void point::rotate() {
    int prev_x_coord = x_coord;
    x_coord = y_coord * -1;
    y_coord = prev_x_coord;
}

Shape::Shape( point p1 , point p2 , point p3 , point p4 ) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;

    local_origin = p1;
    for( int i = 1; i < 4; ++i) {
        if ( points[i].x_coord < local_origin.x_coord ) {
            local_origin.x_coord = points[i].x_coord;
        }
        if ( points[i].y_coord > local_origin.y_coord ) {
            local_origin.y_coord = points[i].y_coord;
        }
    }
}

void Shape::shiftUp(int n ) {
    for ( int i = 0 ; i < 4 ; ++i ) {
        points[i].y_coord -= n;
    }
}

void Shape::shiftRight(int n ) {
    for ( int i = 0 ; i < 4 ; ++i ) {
        points[i].x_coord += n;
    }
}

// Rotates clockwise 90 degrees
void Shape::cRotate() {
    for ( int i = 0 ; i < 4 ; ++i ) {
        points[i] = points[i] - local_origin;
        points[i].rotate();
        points[i] = points[i] + local_origin;
    }
    for ( int i = 0 ; i < 4 ; ++i ) {
        if ( points[i].x_coord < local_origin.x_coord ) {
            shiftRight( local_origin.x_coord - points[i].x_coord ) ;
        }
        if ( points[i].y_coord > local_origin.y_coord ) {
            shiftUp( points[i].y_coord - local_origin.y_coord );
        }
    }
}

// Rotates counterclowckwise 90 degrees
void Shape::counterCRotate() {
    cRotate();
    cRotate();
    cRotate();
}

ostream & operator<<( ostream & out , const point & point ) {
    out << "x: " << point.x_coord << " y: " << point.y_coord;
    return out;
}
ostream & operator<<( ostream & out , const Shape & shape ) {
    for ( int y = shape.local_origin.y_coord - 3 ; y <= shape.local_origin.y_coord ; ++y ) {
        for ( int x = shape.local_origin.x_coord ; x <=
shape.local_origin.x_coord + 3 ; ++x) {
            char output = '_';
            for ( int i = 0 ; i < 4 ; ++i ) {
                if ( shape.points[i].x_coord == x && shape.points[i].y_coord == y ) {
                    output = 'X';
                    break;
                }
            }
            out << output;
        }
        out << endl;
    }
}
