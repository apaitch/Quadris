#include "Shape.h"
#include <string>

using namespace std;

point getPoint() {
    cout << "Enter the x and y coordinates for a point - e.g. 3 2 means x:3 y:2"
                << endl;
    cout << " Remember, in left-hand coordinate system y-coords increase as you go down" << endl;
    int x, y;
    while ( true ) {
        if ( cin >> x && cin >> y ) {
            point p;
            p.x_coord = x;
            p.y_coord = y;
            return p;
        }
        else {
            cout << "Bad input, try again" << endl;
        }
    }
}

int main() {
    point p1 = getPoint();
    point p2 = getPoint();
    point p3 = getPoint();
    point p4 = getPoint();
    Shape s (p1 , p2 , p3, p4);
    cout << "Here is your shape" << endl;
    cout << s;
    cout << "Type 'right' to rotate clockwise, 'left' to rotate counterclockwise" << endl;
    string command;
    while ( cin >> command ) {
        if ( command.compare("right") == 0 ) {
            s.cRotate();
        }
        else if ( command.compare("left") == 0 ) {
            s.counterCRotate();
        }
        cout << s;
    }
    return 0;
}
