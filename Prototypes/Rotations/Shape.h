#ifndef __SHAPE_H__
#define __SHAPE_H__
#include <iostream>

struct point {
    int x_coord;
    int y_coord;

    void rotate();
};

point operator-( const point & , const point & );
point operator+( const point & , const point & );
std::ostream & operator<<( const std::ostream & , const point & );

class Shape {
    private:
        point points [4];
        point local_origin;

        void shiftUp( int );
        void shiftRight( int );
    public:
        Shape( point , point , point , point );
        void cRotate();
        void counterCRotate();
        friend std::ostream & operator<<( std::ostream & , const Shape & );
};

#endif
