#include <stdio.h>
#include <stdlib.h>

/*
 * 0 = None
 * 1 = X
 * 2 = O
 */

char int2char(int n)
{
        switch (n) {
        case 1:  return 'X';
        case 2:  return 'O';
        default: return ' ';
        }
}

int validate_input(char *input) {
        int valid_row = input[0] >= 'a' && input[0] <= 'c';
        int valid_col = input[1] >= '1' && input[1] <= '3';

        return valid_row && valid_col;                        
}

void output_board(int board[9])
{
        printf("  1 2 3\n");
        for (int x = 0; x < 3; ++x) {
                printf("%c ", x+'a'); 
                for (int y = 0; y < 3; ++y) {
                        printf("%c ", int2char(board[y+(x*3)]));
                }
                printf("\n");
        }
}

int attempt_move(int board[9], int to, char col, char row) 
{
        int y = row-'1';
        int x = col-'a';
        if (board[y+(x*3)] != 0) return 0;
        board[y+(x*3)] = to;
        return 1;
}

static inline int _check(int board[9], int x, int y, int z) 
{
        return board[x] & board[y] & board[z];
}

int checkboard(int board[9]) 
{
        /*
         * horizontals
         */
        if (_check(board, 0,1,2) > 0) return _check(board, 0,1,2);
        if (_check(board, 3,4,5) > 0) return _check(board, 3,4,5);
        if (_check(board, 6,7,8) > 0) return _check(board, 6,7,8);

        /*
         * verticals
         */
        if (_check(board, 0,3,6) > 0) return _check(board, 0,3,6);
        if (_check(board, 1,4,7) > 0) return _check(board, 1,4,7);
        if (_check(board, 2,5,8) > 0) return _check(board, 2,5,8);

        /*
         * diagonals
         */
        if (_check(board, 0,4,8) > 0) return _check(board, 0,4,8);
        if (_check(board, 6,4,2) > 0) return _check(board, 6,4,2);
        return 0;
}

int main(void)
{
        char *input = (char*) malloc(64);
        int turn = 0;
        int toMove = 1;
        int board[9] = {0};
        output_board(board);
        while (1) {
                printf("> %c to move: ", int2char(toMove));
                fgets(input, sizeof(input), stdin);
                
                if (!(validate_input(input))) {
                        printf("Invalid square!\n");
                        continue;
                }
                if(!attempt_move(board, toMove, input[0], input[1])){
                        printf("Can't place there!\n");
                        continue;
                }
        
                output_board(board);
                toMove = 3 - toMove;
                turn++;

                if (turn < 5) continue;
                if (turn >= 9) break;
                
                int result = checkboard(board);

                if (result > 0) {
                        printf("%c has won the game!\n", int2char(result)); 
                        break;
                }
        }
        printf("Game Over\n");
        free(input);
        return 0;
}
