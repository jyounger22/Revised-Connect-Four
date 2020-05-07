/* Jay Younger, jyounger
 * CS 152, Winter 2020
 * Project Phase 2
 */

#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* board_new : creates a new board */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* bouard_free : frees a board */
void board_free(board* b);

/* board_show : prints a board to the screen */
void board_show(board* b);

/* board_get : returns the cell in a specific position of a board */
cell board_get(board* b, pos p);

/* board_set : replaces the value of a cell at a specific position with
 * the value of the inputted cell
 */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
