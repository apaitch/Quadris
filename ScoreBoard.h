#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__

class Xwindow;

/*
 * Encapsulates scorekeeping and drawing the score for the game.
 */
class ScoreBoard {
    private:
        int highScore;
        int currentScore;
        int currentLevel;

    public:
        ScoreBoard( int );
        void resetScore();

        void blockCleared( int );
        void linesCleared( int );

        void setLevel( int );

        void print();
        void draw( Xwindow * );
        
};

#endif
