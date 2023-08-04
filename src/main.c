#include <stdio.h>

#define ROWS 3
#define COLS 3
#define BOARD_SIZE (ROWS * COLS)
#define INPUT_SIZE 64

#define LOG(file, fmt, ...)                                                             \
  do {                                                                                  \
    fprintf(file, "%s %s:%d:\n\t" fmt "\n", __FILE__, __func__, __LINE__, __VA_ARGS__); \
  } while (0)

enum pos {
  POS_ROW = 0,
  POS_COL = 1,
};

enum player {
  PLAYER_FIRST = 1,
  PLAYER_SECOND,
  PLAYER_END,
};

/*
 * 0 = None
 * 1 = X
 * 2 = O
 */

static char int2char(int n) {
  switch (n) {
    case 1:
      return 'X';
    case 2:
      return 'O';
    default:
      return ' ';
  }
}

static int validate_input(char *input) {
  return (input[POS_ROW] >= 'a' && input[POS_ROW] <= 'c' && input[POS_COL] >= '1' && input[POS_COL] <= '3');
}

static void output_board(int *board, size_t n_elem, size_t rows, size_t cols) {
  if (n_elem / cols < rows) return;

  printf("  1 2 3\n");
  for (size_t row = 0; row < rows; ++row) {
    printf("%c ", (int)row + 'a');
    for (size_t col = 0; col < cols; ++col) {
      printf("%c ", int2char(board[col + row * cols]));
    }
    printf("\n");
  }
}

static int attempt_move(int *board, size_t n_elem, size_t cols, int to, char col, char row) {
  int y = row - '1';
  int x = col - 'a';

  if (y + (x * cols) >= n_elem) return 0;
  if (board[y + (x * cols)] != 0) return 0;

  board[y + (x * cols)] = to;
  return 1;
}

static int check(int *board, size_t n_elem, unsigned x, unsigned y, unsigned z) {
  if ((size_t)x >= n_elem || (size_t)y >= n_elem || (size_t)z >= n_elem) return 0;

  return board[x] & board[y] & board[z];
}

static int checkboard(int *board, size_t n_elem) {
  /*
   * horizontals
   */
  if (check(board, n_elem, 0, 1, 2) > 0) return check(board, n_elem, 0, 1, 2);
  if (check(board, n_elem, 3, 4, 5) > 0) return check(board, n_elem, 3, 4, 5);
  if (check(board, n_elem, 6, 7, 8) > 0) return check(board, n_elem, 6, 7, 8);

  /*
   * verticals
   */
  if (check(board, n_elem, 0, 3, 6) > 0) return check(board, n_elem, 0, 3, 6);
  if (check(board, n_elem, 1, 4, 7) > 0) return check(board, n_elem, 1, 4, 7);
  if (check(board, n_elem, 2, 5, 8) > 0) return check(board, n_elem, 2, 5, 8);

  /*
   * diagonals
   */
  if (check(board, n_elem, 0, 4, 8) > 0) return check(board, n_elem, 0, 4, 8);
  if (check(board, n_elem, 6, 4, 2) > 0) return check(board, n_elem, 6, 4, 2);
  return 0;
}

int main(void) {
  char input[INPUT_SIZE] = {0};
  int board[BOARD_SIZE] = {0};
  size_t const n_elements = sizeof board / sizeof *board;

  output_board(board, n_elements, ROWS, COLS);

  enum player player = PLAYER_FIRST;
  unsigned turn = 0;
  while (turn < BOARD_SIZE) {
    printf("> %c to move: ", int2char(player));

    if (!fgets(input, sizeof input, stdin)) {
      LOG(stderr, "%s", "fgets");
      return 1;
    }

    if (!(validate_input(input) && attempt_move(board, n_elements, COLS, player, input[POS_ROW], input[POS_COL]))) {
      // maybe?
      // LOG(stderr, "%s", "illegal move");
      continue;
    }

    output_board(board, n_elements, ROWS, COLS);
    player = PLAYER_END - player;
    turn++;

    int result = checkboard(board, n_elements);

    if (result > 0) {
      printf("%c has won the game!\n", int2char(result));
      break;
    }
  }
  printf("Game Over\n");
  return 0;
}
