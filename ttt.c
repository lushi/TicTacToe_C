#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 9
#define NUM_ROW 3
#define NUM_COL 3
#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define COLOR_DEF "\x1B[0m" /* default */
#define COLOR_MOVE1 "\x1B[31m" /* red */
#define COLOR_MOVE2 "\x1B[34m" /* blue */

void init_board(char *board, int num_row, int num_col);
void play(char board[], char player);
int get_move(void);
void update_board(char board[], char move, int post);
void draw(char board[], int num_col, char current_player, char state);
void draw_msg(char current_player, char state);
void draw_board(char *board);
int input_valid(char board[], int pos);
int game_over(char board[], char player);
int check_win(char board[]);

int main()
{
  /* init board */
  char b[NUM_ROW][NUM_COL + 1];
  init_board((char *)b, NUM_ROW, NUM_COL);
  /* TEST */
  draw_board((char *)b);
  // /* Filling the board array with spaces because i'm not sure what else to do */
  // char board[BOARD_SIZE+1];
  //
  // int i;
  // for (i=0; i<BOARD_SIZE; i++) {
  //   board[i] = ' ';
  // }
  // char current_player = PLAYER_1; /* start with X */
  // // board[BOARD_SIZE + 1] = '\0';
  //
  // printf("Let's play Tic Tac Toe!\n");
  // play(board, current_player);
  //
  // while (!game_over(board, current_player)) {
  //   current_player = (current_player == PLAYER_1) ? PLAYER_2 : PLAYER_1;
  //   play(board, current_player);
  // }
}

/* fill in board with space characters */
void init_board(char *board, int num_row, int num_col)
{
  int i, j;
  int row_len = num_col + 1;

  for (i=0; i<num_row; i++) {
    for (j=0; j<num_col; j++) {
      *(board + j + i * row_len) = ' ';
    }
    *(board + j + i * row_len) = '\n'; // end of row
  }

  *(board + j + (i-1) * row_len) = '\0'; // end of board

}

void draw_board(char *board)
{
  int pos = 1; /* first square on board labeled 1 */
  printf("%s\n|", COLOR_DEF);
  for (int i = 0; *(board + i) != '\0'; i++) {
    switch (*(board + i)) {
    case '\n': /* end of line, print new line */
      printf("%s%c|", COLOR_DEF, *(board + i));
      break;
    case PLAYER_1:
      pos++;
      printf("%s%c|", COLOR_MOVE1, *(board + i));
      break;
    case PLAYER_2:
      pos++;
      printf("%s%c|", COLOR_MOVE2, *(board + i));
      break;
    default:
      printf("%s%d|", COLOR_DEF, pos++); /* print square position # */
      break;
    }
  }

  printf("%s\n", COLOR_DEF);
}

void play(char board[], char current_player)
{
  draw(board, 3, current_player, '\0');

  int i = get_move();

  while(!input_valid(board, i)) {
    printf("Illegal move. Try again.\n");
    i = get_move();
  }

  update_board(board, current_player, i);
}

int get_move ()
{
  char c;
  c = getchar();
  /* HACK: this is to prevent c from being assigned line break (enter key)
   * because getchar() is capturing the line break (enter key)
   * Is there a better way?
   */
  if (c == '\n')
    c = getchar();

  return (int) c - '0' - 1; /* printed position is off index by 1 */
}

void update_board(char board[], char mov, int pos)
{
  if (pos >= 0 && board[pos] == ' ')
    board[pos] = mov;
}

void draw(char board[], int num_col, char current_player, char state)
{
  draw_msg(current_player, state);
  // draw_board(**board, num_row, num_col); /* TEST */
  printf("\n");
}

void draw_msg(char current_player, char state)
{
  printf("\n");
  switch (state) {
  case 'w':
    printf("%c wins!\n", current_player);
    break;
  case 't':
    printf("It's a tie.\n");
    break;
  default:
    printf("%c's turn\n", current_player);
    break;
  }
}

int input_valid(char board[], int pos)
{
  if (pos >= 0 && pos < strlen(board) && board[pos] == ' ')
    return 1;

  return 0;
}

int game_over(char board[], char player)
{
  if (check_win(board)) {
    draw(board, 3, player, 'w');
    return 1;
  }

  int i;
  int f = 0;
  for (i = 0; board[i] != '\0'; i++)
    if (board[i] == ' ')
      f = 1; /* Can I return from a loop? */

  if (f)
    return 0; /* Game not over */

  draw(board, 3, player, 't');
  return 1; /* tie */
}

/* Assumes 3x3 board, should be smarter */
int check_win(char board[])
{
  /* first row */
  if (board[0] != ' ' && board[0] == board[1] && board[0] == board[2])
    return (int) board[0];

  /* second row */
  if (board[3] != ' ' && board[3] == board[4] && board[3] == board[5])
    return (int) board[3];

  /* third row */
  if (board[6] != ' ' && board[6] == board[7] && board[6] == board[8])
    return (int) board[6];

  /* first column */
  if (board[0] != ' ' && board[0] == board[3] && board[0] == board[6])
    return (int) board[0];

  /* second column */
  if (board[1] != ' ' && board[1] == board[4] && board[1] == board[7])
    return (int) board[1];

  /* third column */
  if (board[2] != ' ' && board[2] == board[5] && board[2] == board[8])
    return (int) board[2];

  /* diagonal left to right */
  if (board[0] != ' ' && board[0] == board[4] && board[0] == board[8])
    return (int) board[0];

  /* diagonal right to left */
  if (board[2] != ' ' && board[2] == board[4] && board[2] == board[6])
    return (int) board[2];

  return 0; /* no winner */
}
