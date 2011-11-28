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
        void resetScore();

        void blockCleared( int );
        void linesCleared( int );

        void setLevel( int );

        void print();
        void draw( int , int , int , int , Xwindow * );
        
};

#endif
