#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__

class Xwindow;

class ScoreBoard {
    private:
        int high_score;
        int current_score;
        int current_level;

    public:
        ScoreBoard( int );
        void blockCleared( int );
        void linesCleared( int );
        void resetScore();
        void setLevel( int );
        void print();
        void draw( Xwindow * , int , int , int , int );
        
};

#endif
