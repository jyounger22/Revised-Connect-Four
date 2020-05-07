/* Jay Younger, jyounger
 * CS 152, Winter 2020
 * Project Phase 2
 */

#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"


enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;


enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW,
    IN_PROGRESS
};

typedef enum outcome outcome;


enum direction {
    CW, CCW
};

typedef enum direction direction;


struct game {
    unsigned int run;
    board* b;
    turn next;
};

typedef struct game game;

/* new_game : creates a new game */
game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type);

/* game_free : frees a game */
void game_free(game* g);

/* drop_piece : drops a piece into a column and drags it to the lowest
 * non-empty row
 */
int drop_piece(game* g, unsigned int col);

/* twist : rotates a board either clockwise or counterclockwise, and drags
 * pieces to lowest non-empty row
 */
void twist(game* g, direction d);

/* game_outcome : returns the outcome of a game, or if that game is
 * still in progress
 */
outcome game_outcome(game* g);

int game_state(game* g);
int board_full(game* g);
#endif /* LOGIC_H */
