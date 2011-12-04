#include "Ai.h"
#include "QuadrisGame.h"
#include "Board.h"
#include "Block.h"
#include <cassert>

using namespace std;

Ai::Ai( QuadrisGame * game , Board * board ) : game( game ) ,
                                               board( board ) ,
                                               active_block( 0 ) {

    // Punish height at which block lands
    weights.push_back(-3.5);
    // Punish holes
    weights.push_back(-7.5);
    // Reward filled rows
    weights.push_back(4.4);
    // Punish row transitions
    weights.push_back(-4.2);
    // Punish column transitions
    weights.push_back(-5.3);
    // Punish wells
    weights.push_back(-3.5);
    // Punish blockades
    weights.push_back(-4.5);

}

/*
 * Called after the AI tries a particular move to assign a score to the
 * resulting state of the board. This is the key method of the AI - the other
 * methods just set up the board appropriately.
 */
float Ai::evaluateBoard() const {

    int height = 0; 
    int filled_rows = 0;
    int row_transitions = 0;
    int column_transitions = 0;
    // A hole is any empty space that has a block above it (since the AI doesn't
    // do tricks with sliding things in).
    int holes_num = 0;
    int wells_total = 0;
    int blockades_total = 0;

    // find height at which block landed by looking at the tallest point in the
    // block
    vector< pair<int,int> > block_points;
    active_block->getPoints( block_points );
    height = 0;
    for ( int i = 0 ; i < points_per_block ; ++i ) {
        if ( ( num_rows - block_points[i].second ) > height ) {
            height = num_rows - block_points[i].second;
        }
    } 

    // find filled rows
    for ( int i = 0 ; i < num_rows ; ++i ) {
        if ( (*board).rowFilled[i] == num_columns ) {
            filled_rows += 1;
        }
    }

    // find row transitions
    for ( int y = num_rows - height ; y < num_rows ; ++y ) {
        bool prev_empty = ( (*board).blockPtr[0][y] == 0 ) ? true : false;

        for ( int x = 1 ; x < num_columns ; ++x ) {
            bool current_empty = ( (*board).blockPtr[x][y] == 0 ) ? true : false;
            if ( prev_empty != current_empty ) {
                row_transitions += 1;
            }
            prev_empty = current_empty;
        }
    }

    int num_of_empties = 0;
    // There can only be one well per column
    int well_depth = 0;
    int blockade_size = 0;
    bool blockade_started = false;
    // find column transitions and holes
    for ( int x = 0 ; x < num_columns ; ++x ) {
        bool prev_empty = ( (*board).blockPtr[x][num_rows - 1] == 0 ) ? true : false;
        if ( prev_empty ) {
            num_of_empties += 1;

            // Ugly code to look left and right
            bool left_occupied = ( x == 0 ) ? true : false;
            bool right_occupied = ( x == num_columns  - 1 ) ? true : false;
            if ( ! left_occupied ) {
                left_occupied = ( (*board).blockPtr[ x-1 ][num_rows - 1] != 0 ) ? true : false;
            }
            if ( ! right_occupied ) {
                right_occupied = ( (*board).blockPtr[ x+1 ][num_rows - 1] != 0 ) ? true : false;
            }
            if ( left_occupied && right_occupied ) {
                well_depth += 1;
            }
        }
 
        for ( int y = num_rows - 2 ; y > 0 ; --y ) {
            bool current_empty = ( (*board).blockPtr[x][y] == 0 ) ? true : false;
            if ( current_empty ) {
                
                num_of_empties += 1;
                blockade_started = false;
                blockades_total += blockade_size;
                blockade_size = 0;

                // Look left and right
                bool left_occupied = ( x == 0 ) ? true : false;
                bool right_occupied = ( x == num_columns  - 1 ) ? true : false;
                if ( ! left_occupied ) {
                    left_occupied = ( (*board).blockPtr[ x-1 ][y] != 0 ) ? true : false;
                }
                if ( ! right_occupied ) {
                    right_occupied = ( (*board).blockPtr[ x+1 ][y] != 0 ) ? true : false;
                }
                if ( left_occupied && right_occupied ) {
                    well_depth += 1;
                }

            }
            else if ( blockade_started ) {
                blockade_size += 1;
            }
            
            if ( prev_empty != current_empty ) {
                column_transitions += 1;
                if ( current_empty == false ) {
                    holes_num += num_of_empties;
                    num_of_empties = 0;
                    blockade_started = true;
                    blockade_size += 1;
                    well_depth = 0;
                }
            }
            prev_empty = current_empty;
        }
        wells_total += well_depth;
        well_depth = 0;
        if ( blockade_started ) {
            blockades_total += blockade_size;
        }
        blockade_size = 0;
        blockade_started = false;
        num_of_empties = 0;
    }

    float score =
        weights[0] * height +
        weights[1] * holes_num +
        weights[2] * filled_rows +
        weights[3] * row_transitions +
        weights[4] * column_transitions +
        weights[5] * wells_total +
        weights[6] * blockades_total;

    /*cout << "Height: " << height << " holes: " << holes_num << " filled rows: " <<
filled_rows << " row_transitions: " << row_transitions << " col_transitions: "
<< column_transitions << " wells_total: " << wells_total << " blockades_total: "
<< blockades_total << " final score: " << score << endl;*/
    return score;
}

/*
 * Get move loops through all possible ways to drop the current active block and
 * records the sequence of moves for the best one. Called from makeNextMove().
 * 
 * Note: Assuming block is always at original position - 0 , 3. This should
 * be true because there is no alternating human/AI control - it's either
 * all AI or all player.
 */
void Ai::getBestMove( vector<moves> & best_moves_sequence ) {
    active_block = board->getActiveBlock();

    int num_rotations = 0;
    switch ( active_block->getType() ) {
        case 'O' :
            num_rotations = 0;
            break;
        case 'Z' : case 'S' : case 'I' :
            num_rotations = 1;
            break;
        case 'T' : case 'L' : case 'J' :
            num_rotations = 3;
    }

    float high_score = 0;
    vector<moves> moves_sequence;
    int num_of_combinations_considered = 0;
    for ( int rotation = 0 ; rotation <= num_rotations ; ++rotation ) {
        // TODO: Change Block to report success to prevent redundant
        // calculations when can't move right anymore.
        for ( int right_move = 0 ; right_move <= num_columns ; ++right_move ) {
            moves_sequence.clear();
            for ( int i = 0 ; i < rotation ; ++i ) {
                active_block->rightRotate();
                moves_sequence.push_back( clockwise );
            }
            for ( int i = 0 ; i < right_move ; ++i ) {
                active_block->moveRight();
                moves_sequence.push_back( right );
            }
            active_block->drop();
            moves_sequence.push_back( drop );

            board->print();
            float score = evaluateBoard();
            if ( ( high_score == 0 ) || ( score > high_score ) ) {
                high_score = score;
                best_moves_sequence = moves_sequence;
            }
            num_of_combinations_considered += 1;
            active_block->reset();
        }
    }

    return;
}

/*
 * Figures out the best move to make and executes the appropriate commands. This
 * is the way for the game to call the AI!
 */
void Ai::makeNextMove( vector<string> & commands ) {
    vector<moves> best_moves_sequence;
    best_moves_sequence.clear();
    // This is the key call here.
    getBestMove( best_moves_sequence );

    commands.clear();
    for( unsigned int i = 0 ; i < best_moves_sequence.size() ; ++i ) {
        switch ( best_moves_sequence[i] ) {
            case right : 
                commands.push_back( "right" );
                break;
            case clockwise : 
                commands.push_back( "clockwise" );
                break;
            case drop :
                commands.push_back( "drop" );
                break;
        }
    }
}


