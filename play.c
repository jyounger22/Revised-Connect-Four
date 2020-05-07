#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"

/* create_initial_game : reads initial command line arguments to
 * create a new game
 */
game* create_initial_game(int argc, char* argv[])
{
    int count_run, count_w, count_h, count_type, is_bit, i;
    unsigned int run, w, h;
    count_run=0; /* count command line arguments individually to
                  * ensure that each argument is entered exactly once
                  */
    count_w=0;
    count_h=0;
    count_type=0;
    is_bit=0; /* indicates if bit is entered, 0 if false, 1 if true */
    game* res;
    for (i=0; i<argc; i++) {
        if (strcmp(argv[i], "-w")==0) {
            count_w++;
            if (atoi(argv[i+1])<=0) {
                fprintf(stderr, "error create_initial_game : must enter "
                                 "positive width\n");
                exit(1);
            } else {
                w=atoi(argv[i+1]);
            }
        } else if (strcmp(argv[i], "-h")==0) {
            count_h++;
            if (atoi(argv[i+1])<=0) {
                fprintf(stderr, "error create_initial_game : must enter "
                                 "positive height\n");
                exit(1);
            } else {
                h=atoi(argv[i+1]);
            }
        } else if (strcmp(argv[i], "-r")==0) {
            count_run++;
            if (atoi(argv[i+1])<=0) {
                fprintf(stderr, "error create_initial_game : must enter "
                                 "positive run\n");
                exit(1);
            } else {
                run=atoi(argv[i+1]);
            }
        } else if (strcmp(argv[i], "-m")==0) {
            count_type++;
        } else if (strcmp(argv[i], "-b")==0) {
            count_type++;
            is_bit++; /* assumes that typing -b twice would
                       * raise an error
                       */
        }
    }
    if ((count_run!=1)||(count_h!=1)||(count_w!=1)||(count_type!=1)) {
        fprintf(stderr, "error create_initial_game : must enter exactly\n "
                         "1 width, 1 height, 1 run, and 1 type\n");
        exit(1);
    } else if (is_bit==0) { /* matrix implementation */
        res=new_game(run, w, h, MATRIX);
    } else { /* bits implementation */
        res=new_game(run, w, h, BITS);
    }
    return res;
}

/* num_converter : converts characters to numbers, 
 * specifically used for command line entries
 */
int num_converter(char c) {
    if ((c>47)&&(58>c)) { /* integer */
        return c-48;
    } else if ((c>64)&&(91>c)) { /* capital letters */
        return c-55;
    } else if ((c>96)&&(123>c)) { /* lower case letters */
        return c-61;
    } else {
        return -1;
    }
}

/* turn_prompt : updates the player whose turn
 * it is and prompts that player to take their turn
 */
void turn_prompt(game* g, int turn) 
{   
    if ((turn==0)||(turn%2==0)) { /* black goes first */
        g->next=BLACK_NEXT; 
    } else {
        g->next=WHITE_NEXT;
    }
    if (g->next==BLACK_NEXT) {
        printf("Black: ");
    } else {
        printf("White: ");
    }
}

/* turn_input : called immediately after a player 
 * is prompted to take their turn.
 * Takes in the player's execution of their turn (i.e. the 
 * character inputted to the terminal), checks if the
 * input is valid, and, if so, executes the input on the board
 */
int turn_input(game* g)
{
    char col;
    scanf("%c%*c", &col);
    if (col=='<') {
        twist(g, CCW);
    } else if (col=='>') {
        twist(g, CW);
    } else {
        int char_converted=num_converter(col);
        if (char_converted<0) {
            printf("Please enter a column or twist command\n");
            return 0;
        } else if (char_converted>=g->b->width) {
            printf("Please enter a column that is within the board\n");
            return 0;
        } else if (board_get(g->b, 
                   make_pos(g->b->height-1, 
                            (unsigned int)char_converted))!=EMPTY) {
            printf("Column full, please enter a different column\n");
            return 0;
        }
        drop_piece(g, char_converted);
    }
    return 1;
}

/* main : implement the game */
int main(int argc, char *argv[])
{
    game* g=create_initial_game(argc, argv);
    printf("Welcome to Downturn!\nHere is the board:\n");
    int i=0;
    while (game_outcome(g)==IN_PROGRESS) {
        board_show(g->b); 
        turn_prompt(g, i); /* loop through with i to update
                            * whose turn it is
                            */
        if (turn_input(g)==1) {
            i++; /* turn does not change if input is non-sensical,
                  * i.e. if white does not enter a column or twist
                  * command, it remains white's turn until white
                  * does enter a column or twist command
                  */
        }
    }
    if (game_outcome(g)==BLACK_WIN) {
        board_show(g->b);
        printf("Game over. Black wins! Final board state:\n");
        exit(1);
    } else if (game_outcome(g)==WHITE_WIN) {
        board_show(g->b);
        printf("Game over. White wins! Final board state:\n");
        exit(1);
    } else {
        board_show(g->b);
        printf("Game over. It's a draw! Final board state:\n");
        exit(1);
    }
    return 0;
}
