#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "XWindow.h"

using namespace std;

Xwindow::Xwindow( int width , int height ) {

    display = XOpenDisplay( NULL );
    if ( display == NULL ) {
        cerr << "Cannot open display" << endl;
        exit( 1 );
    }
    screen_num = DefaultScreen( display );

    window = XCreateSimpleWindow( display, 
                                  RootWindow( display, screen_num ),
                                  10, 10, width, height, 1,
                                  BlackPixel( display, screen_num ), 
                                  WhitePixel( display, screen_num ) );

    XSelectInput( display, window, ExposureMask | KeyPressMask );
    XMapRaised( display, window );

    Pixmap pix = XCreatePixmap( display,window,width,
                height,DefaultDepth( display,DefaultScreen( display ) ) );
    graphics_context = XCreateGC( display, pix, 0,( XGCValues * )0 );

    XFlush( display );
    XFlush( display );
    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    char color_vals[11][15] = { "white" , "black" , "red" , "green" ,
                                "blue" , "cyan" , "yellow" , "magenta" ,
                                "orange" , "brown" , "rgb:0a/0a/0a" };

    cmap = DefaultColormap( display,DefaultScreen( display ) );
    for( int i = 0 ; i < 11 ; ++i ) {
        if ( ! XParseColor( display , cmap , color_vals[i] , &xcolour ) ) {
             cerr << "Bad colour: " << color_vals[i] << endl;
        }
        if ( ! XAllocColor( display , cmap , &xcolour ) ) {
             cerr << "Bad colour: " << color_vals[i] << endl;
        }
        colours[i] = xcolour.pixel;
    }

    XSetForeground( display , graphics_context , colours[Black] );

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = ( USPosition | PSize | PMinSize | PMaxSize  );
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints( display , window , &hints );

    XSynchronize( display , True );

    usleep( 1000 );
}

Xwindow::~Xwindow() {
    XFreeGC( display , graphics_context );
    XCloseDisplay( display );
}

void Xwindow::fillBorderedRectangle( int x, int y, int width, int height, int colour ) {
    XSetForeground( display, graphics_context, colours[colour] );
    XFillRectangle( display, window, graphics_context, x+2, y+2, width-4, height-4 );
    XSetForeground( display, graphics_context, colours[Black] );
}

void Xwindow::fillRectangle( int x, int y, int width, int height, int colour ) {
    XSetForeground( display, graphics_context, colours[colour] );
    XFillRectangle( display, window, graphics_context, x+2, y+2, width-4, height-4 );
    XSetForeground( display, graphics_context, colours[Black] );
}

void Xwindow::drawString( int x, int y, string msg, int colour ) {
    XSetForeground( display, graphics_context, colours[colour] );
    //XDrawString( display, window, DefaultGC( display, screen_num ), x, y, msg.c_str(), msg.length() );
    XDrawString( display, window, graphics_context, x, y, msg.c_str(), msg.length() );
    XSetForeground( display, graphics_context, colours[Black] );
}


