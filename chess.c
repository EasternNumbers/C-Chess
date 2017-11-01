#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* bools */
#define FALSE 0
#define TRUE 1

/* cell states */
#define UNOCCUPIED 2
#define FRIENDLY 3
#define ENEMY 4
#define OCCUPIED 5

/* teams */
#define WHITE 17
#define BLACK 18 

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
typedef int8_t sinbyte;

byte board[8][8]; /* 2-D Board Array (y, x) */

byte isodd(byte i) { /* test if int is odd */
    if (i % 2 == 0) {
        return FALSE;
    } 
    else {
        return TRUE;
    }
}

void drawboard(void) { /* print array in 8^2 board grid and fill in empty space */
    printf("\t Y\n\n");
    byte i; /* vertical (y) */
    for (i = 0; i < 8; i++) {
        byte j; /* horizontal (x) */
        printf("\t %i ", i );
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
    printf("\n\t X 01234567\n");
    printf("\n");
}

byte getinput(void) { /* get single-byte input */
    byte inp;
    scanf("%d", &inp);
    return inp;
}

byte isoccupied(byte cords) { /* test if cell is occupied */
    if (cords == 0 | cords >= 177) {
        return UNOCCUPIED;
    }
    else {
        return OCCUPIED;
    }
}

byte isonteam(byte team, byte cords) { /* test if piece is on current team */
    if (cords < 177 && cords > 0) {
        if (team == WHITE && cords < 98) {
            return ENEMY;
        }
        else if (team == WHITE && cords >= 98) {
            return FRIENDLY;
        }
        
        if (team == BLACK && cords > 82) {
            return ENEMY;
        }
        else if (team == BLACK && cords <= 82)  {
            return FRIENDLY;
        }
    }
    else {
        return UNOCCUPIED;
    }
}

void move (byte src_x, byte src_y, byte dst_x, byte dst_y) { /* move piece */
    board[dst_y][dst_x] = board[src_y][src_x];
    board[src_y][src_x] = 0;
}

void attemptmove(byte team) { /* test conditions and move if successful */
    if (team == WHITE) {
        printf("White player's turn\n\n");
    }
    else {
        printf("Black player's turn\n\n");
    }
   
    selection: /* get coordinates */
    printf("Source X: ");
    sinbyte src_x = getinput();
    printf("Source Y: ");
    sinbyte src_y = getinput();
    printf("\n");
    printf("Destination X: ");
    sinbyte dst_x = getinput();
    printf("Destination Y: ");
    sinbyte dst_y = getinput();
    printf("\n");
    
    if (isoccupied(board[src_y][src_x]) == UNOCCUPIED) { /* deny selection if an empty cell is selected */
        printf("invalid selection: source cell is empty\n\n");
        goto selection;
    }
    
    if (isonteam(team, board[dst_y][dst_x]) == FRIENDLY) { /* deny selection if destination contains a friendly piece */
        printf("invalid selection: destination cell contains friendly\n\n");
        goto selection;    
    }
    
    if (isonteam(team, board[src_y][src_x]) == ENEMY) { /* deny selection if source is enemy */
        printf("invalid selection: source cell contains enemy\n\n");
        goto selection;        
    }
    
    if  (src_x > 7 | src_y > 7 | dst_x > 7 |  dst_y > 7 | src_x < 0 | src_y < 0 | dst_x < 0 | dst_y < 0) { /* deny selection if out of range */
        printf("invalid selection: out of range\n\n");
        goto selection;
    }
    
    /* piece-specific restrictions */
    
    /* start pawn */
    if (board[src_y][src_x] == W_PAWN | board[src_y][src_x] == B_PAWN) {
        if (board[src_y][src_x] == W_PAWN) { /* if pawn is white */
            if (src_y - dst_y > 2 && src_y == 6) { /* if pawn is in starting position and moves more than 2 spaces */
                printf("invalid move: out of piece's range of motion\n\n");
                goto selection;
            }
            else {
                if (src_y - dst_y > 1 && src_y != 6) { /* if pawn is not in starting position and moves more than 1 or in the wrong direction */
                    printf("invalid move: out of piece's range of motion\n\n");
                    goto selection;
                }
            }
            
            if (abs(src_x - dst_x) == 1 && src_y - dst_y > 1) { /* if pawn tries to move 2 and capture from starting position */
                printf("invalid move: out of piece's range of motion\n\n");
                goto selection;
            }
        }
       
       else if (board[src_y][src_x] == B_PAWN) { /* if pawn is black */
            if (src_y - dst_y < -2 && src_y == 1) { /* if pawn is in starting position moves more than 2 spaces */
                printf("invalid move: out of piece's range of motion\n\n");
                goto selection;
            }
            else {
                if (dst_y - src_y < -1 && src_y != 1) { /* if pawn is not in starting position and moves more than 1 or in the wrong direction */
                    printf("invalid move: out of piece's range of motion\n\n");
                    goto selection;
                }
            }
            
            if (abs(src_x - dst_x) == 1 && src_y - dst_y < -1) { /* if pawn tries to move 2 and capture from starting position */
                printf("invalid move: out of piece's range of motion\n\n");
                goto selection;
            }
        }
        
        if (isoccupied(board[dst_y][dst_x]) == OCCUPIED && abs(src_x - dst_x) == 0) { /* if position is occupied and is directly ahead */
            printf("invalid move: occupied position\n\n");
            goto selection;
        }
        
        if (abs(src_x - dst_x) == 1 && isoccupied(board[dst_y][dst_x]) == UNOCCUPIED) { /* if a capture is attempted with an unoccupied square */
            printf("invalid move: out of piece's range of motion\n\n");
            goto selection;
        } 
 
        if (abs(src_x - dst_x) > 1) { /*if a change in X greater than 2 is attempted */
            printf("invalid move: out of piece's range of motion\n\n");
            goto selection;
        }
    }
    /* end pawn */
    
    /* start bishop */
    if (board[src_y][src_x] == W_BISHOP | board[src_y][src_x] == B_BISHOP) { 
        if (abs(src_x - dst_x) != abs(src_y - dst_y)) {
            printf("invalid move: out of piece's range of motion\n\n"); /* if bishop does not move in diagonal */
            goto selection;
        }
        else { /* loop to ensure that no collisions occur along the way */
            byte x;
            byte y;
            if(src_x < dst_x) {
                byte y = src_y + 1;
                for (x = src_x + 1; x < dst_x; x++ ) {
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                    y++;
                }
            }
            else {
                byte y = src_y - 1;
                for (x = src_x - 1; x > dst_x; x-- ) {
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                    y--;
                }
            }
        }
    }
    /* end bishop */
    
    /* start rook */
    if (board[src_y][src_x] == W_ROOK | board[src_y][src_x] == B_ROOK) {
        if(abs(src_x - dst_x) != 0 && abs(src_y - dst_y) != 0) { /* if rook does not move in straight line */
            printf("invalid move: out of piece's range of motion\n\n");
            goto selection;
        }
        else if (abs(src_x - dst_x) != 0) { /* if it moves along x */
            byte x;
            byte y = src_y;
            if(src_x < dst_x) {
                for (x = src_x + 1; x < dst_x; x++ ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
            else {
                for (x = src_x - 1; x > dst_x; x-- ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
        }
        else if (abs(src_y - dst_y) != 0) { /* if it moves along y */
            byte y;
            byte x = src_x;
            if(src_y < dst_y) {
                for (y = src_y + 1; y < dst_y; y++ ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
            else {
                for (y = src_y - 1; y > dst_y; y++ ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
        }
    }
    /* end rook */
    
    /* start knight */
    if (board[src_y][src_x] == W_KNIGHT | board[src_y][src_x] == B_KNIGHT) {
        if (abs(src_x - dst_x) == 2 && abs(src_y - dst_y) == 1) {} /* test if move is valid */
        else if (abs(src_x - dst_x) == 1 && abs(src_y - dst_y) == 2) {}
        else {
            printf("invalid move: out of piece's range of motion\n\n");
            goto selection;
        }
    }
    /* end knight */
    
    /* start king */
    if (board[src_y][src_x] == W_KING | board[src_y][src_x] == B_KING) {
        if (abs(src_x - dst_x) > 1 | abs(src_y - dst_y) > 1) { /* if king moves more than one in any direction */
            printf("invalid move: out of piece's range of motion\n\n");
            goto selection;
        }
    }
    /* end king */
    
    /* start queen */
    if (board[src_y][src_x] == W_QUEEN | board[src_y][src_x] == B_QUEEN) {
        if (abs(src_x - dst_x) == abs(src_y - dst_y)) { /* if move is diagonal */
            byte x;
            byte y;
            if(src_x < dst_x) {
                byte y = src_y + 1;
                for (x = src_x + 1; x < dst_x; x++ ) {
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                    y++;
                }
            }
            else {
                byte y = src_y - 1;
                for (x = src_x - 1; x > dst_x; x-- ) {
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                    y--;
                }
            }
        }
        else if (abs(src_x - dst_x) != 0) { /* if it moves along x */
            byte x;
            byte y = src_y;
            if(src_x < dst_x) {
                for (x = src_x + 1; x < dst_x; x++ ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
            else {
                for (x = src_x - 1; x > dst_x; x-- ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
        }
        else if (abs(src_y - dst_y) != 0) { /* if it moves along y */
            byte y;
            byte x = src_x;
            if(src_y < dst_y) {
                for (y = src_y + 1; y < dst_y; y++ ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
            else {
                for (y = src_y - 1; y > dst_y; y-- ) { 
                    if(isoccupied(board[y][x]) == OCCUPIED) {
                        printf("invalid move: piece in path\n\n");
                        goto selection;
                    }
                }
            }
        }
        
        if (abs(src_x - dst_x) != 0 && abs(src_y - dst_y) != 0){ /* if not straight*/
            if (abs(src_x - dst_x) != abs(src_y - dst_y)) { /* if not diagonal */
                printf("invalid move: out of piece's range of motion\n\n");
                goto selection;
            }
        }
    }
    /* end queen */
   
    move(src_x, src_y, dst_x, dst_y); /* move if move is valid */
}

void setboard() {
    byte i;
    for (i = 0; i < 8; i++) {
        board[6][i] = W_PAWN;
        board[1][i] = B_PAWN;
    }
    
    board[7][0] = W_ROOK;
    board[7][1] = W_KNIGHT;
    board[7][2] = W_BISHOP;
    board[7][3] = W_QUEEN;
    board[7][4] = W_KING;
    board[7][5] = W_BISHOP;
    board[7][6] = W_KNIGHT;
    board[7][7] = W_ROOK;
    
    board[0][0] = B_ROOK;
    board[0][1] = B_KNIGHT;
    board[0][2] = B_BISHOP;
    board[0][3] = B_KING;
    board[0][4] = B_QUEEN;
    board[0][5] = B_BISHOP;
    board[0][6] = B_KNIGHT;
    board[0][7] = B_ROOK;
}

void testvictory() {
    byte wvictory = TRUE;
    byte bvictory = TRUE;
    byte i; /* vertical (y) */
    for (i = 0; i < 8; i++) {
        byte j;
        for (j = 0; j < 8; j++) {
            if (board[i][j] == W_KING) {
                bvictory = FALSE;
            }
            if (board[i][j] == B_KING) {
                wvictory = FALSE;
            }
        }
    }
    
    if (wvictory == TRUE) {
        printf("White Victory!\n");
        exit(0);
    }
    if(bvictory == TRUE) {
        printf("Black Victory!\n");
        exit(0);
    }
}

int main(void) { /* black starts on top, white on bottom */
    byte team = WHITE;
    setboard();
    while(1) {         
        drawboard();
        attemptmove(team);
        if (team == WHITE) {
            team = BLACK;
        }
        else {
            team = WHITE;
        }
        testvictory();
    }
}