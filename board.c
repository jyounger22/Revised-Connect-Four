#include "board.h"
#include <stdio.h>
#include <stdlib.h>

board* board_new(unsigned int width, unsigned int height, enum type type)
{
    if (width==0) {
        fprintf(stderr, "error board_new : board cannot have width 0\n");
        exit(1);
    } else if (height==0) {
        fprintf(stderr, "error board_new : board cannot have height 0\n");
        exit(1);
    } else {
        board* res=(board*)malloc(sizeof(board));
        if (res==NULL) {
            fprintf(stderr, "board_new : malloc failed\n");
            exit(1);
        }
        res->width=width;
        res->height=height;
        int i, j;
        if (type==BITS) {
            res->type=BITS;
            unsigned int cells, elements;
            cells=width*height;
            elements=cells/16; /* elements store 16 cells,
                                * or 32 bits
                                */
            unsigned int* res_bits=(unsigned int*)
                                   malloc(elements*sizeof(unsigned int));
            if (res_bits==NULL) {
                fprintf(stderr, "board_new : malloc failed\n");
                exit(1);
            }
            for (i=0; i<elements; i++) {
                res_bits[i]=0;
            }
            res->u.bits=res_bits;
        } else {
            res->type=MATRIX;
            cell** res_cell=(cell**)malloc(sizeof(cell*)*height);
            if (res_cell==NULL) {
                fprintf(stderr, "board_new : malloc failed\n");
                exit(1);
            }
	    for (i=height-1; i>=0; i--) {
                res_cell[i]=(cell*)malloc(sizeof(cell)*width);
                for (j=0; j<width; j++) {
                    res_cell[i][j]=EMPTY;
                }
            }  
            res->u.matrix=res_cell;
        }
        return res;
    }
}

void board_free(board* b)
{
    if (b->type==BITS) {
        free(b->u.bits);
    } else {
        unsigned int i;
        for (i=b->height; i>0; i--) {
	    free(b->u.matrix[i]);
        }
    }
    free(b);
}

/* row_contents : fills in the cells that make up each row */
void row_contents(board* b, int row)
{
    unsigned int i;
    unsigned char c;
    for (i=0; i<b->width; i++) {
        cell cell_temp=board_get(b, make_pos(row, i));
        if (cell_temp==EMPTY) {
            c=46;  /* period */
            if (i<b->width-1) {
                printf("%c ", c);
            } else {
                printf("%c", c);
            }
        } else if (cell_temp==BLACK) {
            c=42;  /* asterisk */
            if (i<b->width-1) {
                printf("%c ", c);
            } else {
                printf("%c", c);
            }
        } else {
            c=111; /* o */
            if (i<b->width-1) {
                printf("%c ", c);
            } else {
                printf("%c", c);
            }
        }
    }
    printf("\n");
}

/* row_show : populates each row, including the axis */
void row_show(board* b)
{
    unsigned int first_nine, col_res;
    first_nine=0;
    col_res=65;
    unsigned int height=b->height;
    int i;
    for (i=height-1; i>=0; i--) {         /* no different end case; 
                                           * will have line break at end
                                           */
        if (first_nine<10) {              /* print integers 0 to 9 */
            printf("%d ", first_nine);
            first_nine++;
        } else if (col_res<=90) {         /* print capital letters */
            printf("%c ", col_res);
            col_res++;
        } else if (col_res==91) {         /* skip to lowercase letters */
            col_res=97;
            printf("%c ", col_res);
            col_res++;
        } else if (col_res<=122) {        /* print lowercase letters */
            printf("%c ", col_res);
            col_res++;
        } else {
            printf("%c ", 63);            /* print question marks */
        }
	row_contents(b, i);               /* fills in each row */
    }
}

/* col_show : populates each column, including the axis */
void col_show(board* b)
{
    unsigned int i, first_nine, row_res;
    first_nine=0;
    row_res=65;
    for (i=0; i<b->width; i++) {
        if (i<b->width-1) {
            if (first_nine<10) {              /* print integers 0 to 9 */
                printf("%d ", first_nine);
                first_nine++;
            } else if (row_res<=90) {         /* print capital letters */
                printf("%c ", row_res);
                row_res++;
            } else if (row_res==91) {         /* skip to lowercase letters */
                row_res=97;
                printf("%c ", row_res);
                row_res++;
            } else if (row_res<=122) {        /* print lowercase letters */
                printf("%c ", row_res);
                row_res++;
            } else {
                printf("%c ", 63);            /* print question marks */
            }
        } else {                              /* end case; no space */
            if (first_nine<10) {              /* print integers 0 to 9 */
                printf("%d", first_nine);
                first_nine++;
            } else if (row_res<=90) {         /* print capital letters */
                printf("%c", row_res);
                row_res++;
            } else if (row_res==91) {         /* skip to lowercase letters */
                row_res=97;                
                printf("%c ", row_res);
                row_res++;
            } else if (row_res<=122) {        /* print lowercase letters */
                printf("%c", row_res);
                row_res++;
            } else {                          /* print question marks */
                printf("%c", 63);
            }
        }
    }
}

void board_show(board* b)
{
    printf("  "); col_show(b); printf("\n");
    row_show(b);
}

cell board_get(board* b, pos p)
{
    unsigned int height_given, width_given;
    height_given=b->height;
    width_given=b->width;
    if ((p.r>=height_given)||(p.c>=width_given)) {
        fprintf(stderr, "error board_get : position out of bounds\n");
        exit(1);
    } else if (b->type==BITS) {
        unsigned int cell_temp, bit_temp, index, res;
        cell_temp=(p.r*height_given)+(p.c*width_given);
        bit_temp=cell_temp%16;
        index=cell_temp/16;
        res=(b->u.bits[index]>>(bit_temp*2))&3;
        switch (res) {
            case 0:
                return EMPTY;
            case 1:
                return BLACK;
            default:
                return WHITE;
        }
    } else {
        return b->u.matrix[p.r][p.c];
    }
}

void board_set(board* b, pos p, cell c)
{
    unsigned int height_given, width_given;
    height_given=b->height;
    width_given=b->width;
    if ((p.r>=height_given)||(p.c>=width_given)) {
        fprintf(stderr, "error board_set : position out of bounds\n");
        exit(1);
    } else if (b->type==BITS) {
        unsigned int temp;
        switch (c) {
            case EMPTY:
                temp=0;
                break;
            case BLACK:
                temp=1;
                break;
            default:
                temp=2;
                break;
        }
        unsigned int cell_temp, bit_temp, index, empty_cell;
        cell_temp=(p.r*height_given)+(p.c*width_given);
        bit_temp=cell_temp%16;
        index=cell_temp/16;
        empty_cell=(b->u.bits[index])&(~(3<<(bit_temp*2))); /* create
                                                             * cells as
                                                             * empty, then
                                                             * update
                                                             * them
                                                             */
        b->u.bits[index]=empty_cell|(temp<<(bit_temp*2));
    } else {
        b->u.matrix[p.r][p.c]=c;
    }
}
