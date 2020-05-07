#include "logic.h"
#include <stdio.h>
#include <stdlib.h>

game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type)
{
    game* res=(game*)malloc(sizeof(game));
    if (res==NULL) {
        fprintf(stderr, "new_game : malloc failed\n");
        exit(1);
    }
    res->run=run;
    res->b=board_new(width, height, type);
    return res;
}

void game_free(game* g)
{
    board_free(g->b);
    free(g);
}

int drop_piece(game* g, unsigned int col)
{
    int i;
    unsigned int height_start=g->b->height;
    for (i=0; i<=height_start; i++) {
        pos p=make_pos(i, col);
        switch(board_get(g->b, p)) {
            case EMPTY:
                switch(g->next) {
                    case BLACK_NEXT:
                        board_set(g->b, p, BLACK);
                        break;
                    case WHITE_NEXT:
                        board_set(g->b, p, WHITE);
                        break;
                }
                return 1;
            default:
                break;
        }
    }
    return 0;
}

/* gravity : drags pieces down like gravity would in
 * a connect-4 game
 */
board* gravity(board* b)
{
    unsigned int w, h;
    w=b->width;
    h=b->height;
    int i, j, empty_count;
    cell cell_final;
    for (i=0; i<=w-1; i++) {
        empty_count=0; /* resets every time column changes */
        for (j=0; j<=h-1; j++) {
            pos p1=make_pos(j, i);
            cell_final=board_get(b, p1);
            if (cell_final==EMPTY) {
                empty_count++;
            } else if ((cell_final!=EMPTY)&&(empty_count!=0)) {
                pos p2=make_pos(j-empty_count, i);
                board_set(b, p2, cell_final);
                board_set(b, p1, EMPTY);
            }
        }
    }
    return b;
}

void twist(game* g, direction d)
{
    unsigned int width_start, height_start;
    int i, j, temp;
    width_start=g->b->width;
    height_start=g->b->height;
    enum type type_start=g->b->type;
    board* res=board_new(height_start,
                         width_start, type_start); /* cols become
                                                    * rows and
                                                    * vice-versa
                                                    */
    switch(d) {
        case CW:
            for (i=0; i<height_start; i++) {
                temp=width_start-1;
                for (j=width_start-1; j>=0; j--) {
                    pos p1=make_pos(i, j);
                    pos p2=make_pos(temp, i);
                    switch(board_get(g->b, p1)) {
                        case EMPTY:
                            break;
                        case BLACK:
                            board_set(res, p2, BLACK);
                            temp--;
                            break;
                        case WHITE:
                            board_set(res, p2, WHITE);
                            temp--;
                            break;
                    }
                }
            }
            break;
        case CCW:
            for (i=0; i<height_start; i++) {
                temp=width_start-1;
                for (j=width_start-1; j>=0; j--) {
                    pos p1=make_pos(i, j);
                    pos p2=make_pos(temp, height_start-i-1);
                    switch(board_get(g->b, p1)) {
                        case EMPTY:
                            break;
                        case BLACK:
                            board_set(res, p2, BLACK);
                            temp--;
                            break;
                        case WHITE:
                            board_set(res, p2, WHITE);
                            temp--;
                            break;
                    }    
                }
            }
            break;
    }
    res=gravity(res);
    g->b=res;
}

/* row_checker : checks if in a row */
int row_checker(game* g, cell c, pos p1, char direction)
{
    int i, res, height_given, width_given;
    height_given=g->b->height;
    width_given=g->b->width;
    res=0;
    for (i=0; i<height_given-p1.r; i++) {
        if (direction=='h') { /* horizontal */
            if (c!=board_get(g->b, make_pos(p1.r, p1.c+i))) {
                res++;
                break;
            }
        } else if (direction=='v') { /* vertical */
            if (c!=board_get(g->b, make_pos(p1.r+i, p1.c))) {
                res++;
                break;
            } 
        } else if (direction=='r') { /* right diagonal */
            if (c!=board_get(g->b, make_pos(p1.r+i, p1.c+i))) {
                res++;
                break;
            }
        } else { /* left diagonal */
            if (c!=board_get(g->b, make_pos(p1.r+i, p1.c-i))) {
                res++;
                break;
            }
        }
    }
    return (res==1)?0:1; /* returns if in row */
}

/* board_full : checks if board is full */
int board_full(game* g)
{
    int i, j, count;
    unsigned int w, h;
    w=g->b->width;
    h=g->b->height;
    count=0;
    for (i=0; i<=h-2; i++) {
        for (j=0; j<=w-1; j++) {
            pos p1=make_pos(i, j);
            if (board_get(g->b, p1)==EMPTY) {
                count+=1;
            }
        }
    }
    int total_spots=h*w;
    if (count==total_spots) {
        return 1;
    } else {
        return 0;
    }
}

/* game_state: determines if a game is finished or still in progress */
int game_state(game* g) {
    int black, white, row, col, width_given, height_given;
    char h, v, r, l;
    h='h';
    v='v';
    r='r';
    l='l';
    black=0;
    white=0;
    width_given=g->b->width;
    height_given=g->b->height;
    for (col=0; col<width_given; col++) { /* check for vertical win */
        for (row=0; row<=1; row++) {
            cell cell_res=board_get(g->b, make_pos(row, col));
            if ((cell_res!=EMPTY)&&row_checker(g, cell_res, 
                                               make_pos(row, col), v)) {
                if (cell_res==BLACK) {
                    black++;
                } else {
                    white++;
                }
            }
        }
    }
    for (row=0; row<height_given; row++) { /* checks for horizontal win */
        for (col=0; col<=1; col++) {
            cell cell_res=board_get(g->b, make_pos(row, col));
            if ((cell_res!=EMPTY)&&row_checker(g, cell_res, 
                                               make_pos(row, col), h)) {
                if (cell_res==BLACK) {
                    black++;
                } else {
                    white++;
                }
            }
        }
    }
    for (row=0; row<=1; row++) { /* checks for left diagonal win */
        for (col=width_given-2; col<=width_given-1; col++) {
            cell cell_res=board_get(g->b, make_pos(row, col));
            if ((cell_res!=EMPTY)&&row_checker(g, cell_res,
                                               make_pos(row, col), l)) {
                if (cell_res==BLACK) {
                    black++;
                } else {
                    white++;
                }
            }
        }
    }
    for (row=0; row<=1; row++) { /* check for 
                                  * right diagonal win 
                                  */
        for (col=0; col<=1; col++) {
            cell cell_res=board_get(g->b, make_pos(row, col));
            if ((cell_res!=EMPTY)&&row_checker(g, cell_res, 
                                               make_pos(row, col), r)) {
                if (cell_res==BLACK) {
                    black++;
                } else {
                    white++;
                }
            }
        }
    }
    if (black!=0&&white==0) {
        return 1; /* black wins */
    } else if (black==0&&white!=0) {
        return 2; /* white wins */
    } else if (((black==0&&white==0)&&(board_full(g)))||
               (black!=0&&white!=0)) {
        return 3; /* neither wins or board full */
    } else {
        return 0; /* game in progress */
    }
}

outcome game_outcome(game* g)
{
    int res=game_state(g);
    switch (res) {
        case 1:
            return BLACK_WIN;
        case 2:
            return WHITE_WIN;
        case 3:
            return DRAW;
        default:
            return IN_PROGRESS;
    }
}
