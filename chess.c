#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* bools */
#define FALSE 0
#define TRUE 1

/* board backgrounds */
#define WHITECELL 177
#define BLACKCELL 178

/* white = lowercase, black = upper */
#define W_PAWN 112    /* p */
#define W_ROOK 114    /* r */
#define W_BISHOP 98   /* b */
#define W_KNIGHT 104  /* h */
#define W_QUEEN 113   /* q */
#define W_KING 107    /* k */

#define B_PAWN 80     /* P */
#define B_ROOK 82     /* R */
#define B_BISHOP 66   /* B */
#define B_KNIGHT 72   /* H */
#define B_QUEEN 81    /* Q */
#define B_KING 75     /* K */

typedef uint8_t byte;

byte board[8][8]; /* 2-D Board Array */

byte isodd(byte i) { /* test if int is odd */
    if (i % 2 == 0) {
        return FALSE;
    } 
    else {
        return TRUE;
    }
}

void drawboard(void) { /* print array in 8^2 board grid and fill in empty space */
    byte i; /* vertical (y) */
    for (i = 0; i < 8; i++) {
        byte j; /* horizontal (x) */
        printf("\t");
        for (j = 0; j < 8; j++) {
            if (board[i][j] == 0) { /* fill in empty space if board is empty */
                if (isodd(i) == TRUE) {
                    if (isodd(j) == TRUE) {
                        board[i][j] = BLACKCELL; /* set to black if both are odd */
                    }
                    else {
                        board[i][j] = WHITECELL; /* set to white if i is odd but j is not */
                    }
                }
                else {
                    if (isodd(j) == TRUE) {
                        board[i][j] = WHITECELL; /* set to white if i is even but j is not */
                    }
                    else {
                        board[i][j] = BLACKCELL; /* set to black if both are even */
                    }
                }
            }
            printf("%c", board[i][j]); 
        }
        printf("\n");
    }
}


int main(void) {
    board [3][4] = W_QUEEN;
    drawboard();
}