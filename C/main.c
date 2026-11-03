#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// All elements initialise to 0 anyway
int grid[3][3] = {0};

const int EMPTY = 0;
const int X_MARK = 1;
const int O_MARK = -1;

const int DRAW = 0;
const int X_PLAYER = 1;
const int O_PLAYER = -1;


// Function declarations
char getGridValue(int row, int column);
void printGrid();

int validPlayerMove(int input);
int getPlayerMove(char charPlayer);
char getPlayer(int player);

int endGameCheckHorizontal(int player, int gridInput[3][3]);
int endGameCheckVertical(int player, int gridInput[3][3]);
int endGameCheckDiagonal(int player, int gridInput[3][3]);
int endGameCheck(int player);

void winScreen(int winner);
void playerMove(int currentPlayer);


char getGridValue(int row, int column) {
 int gridVal = grid[row][column];

 if (gridVal == EMPTY) {
  // The plus 1 is so that everything starts at one and counts up
  // This places the numbers that users type so they can pick a square

  return '1' + column + 3 * row;
 }
 else if (gridVal == X_MARK) return 'X';
 else if (gridVal == O_MARK) return 'O';
 else return '?';
}


void printGrid() {
 for (int row = 0; row < 3; row++) {
  printf(" %c │ %c │ %c\n", getGridValue(row, 0), getGridValue(row, 1), getGridValue(row, 2));

  if (row < 2) printf("───┼───┼───\n");
  else printf("   │   │   \n");
 }
}


int validPlayerMove(int input) {
 if (input < 1 || input > 9) return 0;

 int row = (input - 1) / 3;
 int column = (input - 1) % 3;

 if (grid[row][column] != EMPTY) return 0;

 return 1;
}


int getPlayerMove(char charPlayer) {
 int input;

 printf("Where would player %c like to go? Please type in the number of a valid square: ", charPlayer);
 scanf("%d", &input);

 while (validPlayerMove(input) != 1) {
  printf("Invalid move. Please pick an empty square between 1 and 9.\n");

  printf("Where would player %c like to go? Please type in the number of a valid square: ", charPlayer);
  scanf("%d", &input);
 }

 return input;
}


char getPlayer(int player) {
 if (player == X_PLAYER) return 'X';
 else return 'O';
}


int endGameCheckHorizontal(int player, int gridInput[3][3]) {
 for (int row = 0; row < 3; row++) {
  bool winCondition =
   gridInput[row][0] == player &&
   gridInput[row][1] == player &&
   gridInput[row][2] == player;

  if (winCondition) return player;
 }

 return DRAW;
}


int endGameCheckVertical(int player, int gridInput[3][3]) {
 int rotatedGrid[3][3];

 for (int row = 0; row < 3; row++) {
  for (int column = 0; column < 3; column++) {
   rotatedGrid[column][row] = gridInput[row][column];
  }
 }

 return endGameCheckHorizontal(player, rotatedGrid);
}


int endGameCheckDiagonal(int player, int gridInput[3][3]) {
 if (gridInput[1][1] != player) return DRAW;

 // Top-left to bottom-right
 if (gridInput[0][0] == player && gridInput[2][2] == player) return player;

 // Top-right to bottom-left
 if (gridInput[0][2] == player && gridInput[2][0] == player) return player;

 return DRAW;
}


int endGameCheck(int player) {
 // Could be more optimised by running the check after every function but makes the code look messier

 int horizontal = endGameCheckHorizontal(player, grid);
 int vertical = endGameCheckVertical(player, grid);
 int diagonal = endGameCheckDiagonal(player, grid);

 if (horizontal == player ||
  vertical == player ||
  diagonal == player) {
  return player;
 }

 return DRAW;
}


void winScreen(int winner) {
 if (winner == DRAW) printf("You both drew, sorry, maybe next time\n");
 else {
  char charPlayer = getPlayer(winner);

  printf("Player %c wins!\n", charPlayer);
 }

}


void playerMove(int currentPlayer) {
 char charPlayer = getPlayer(currentPlayer);
 int num = getPlayerMove(charPlayer);

 int row = (num - 1) / 3;
 int column = (num - 1) % 3;

 grid[row][column] = currentPlayer;
}


int main() {
 int turn = 0;
 int currentPlayer = X_PLAYER;

 while (true) {
  printGrid();
  playerMove(currentPlayer);
  turn++;

  if (endGameCheck(currentPlayer) == currentPlayer) break;
  else if (turn >= 9) {
   // If the current player hasn't won and there have been 9 turns then its a draw
   currentPlayer = DRAW;
   break;
  }
  else {
   currentPlayer *= -1;
  }
 }

 printGrid();

 winScreen(currentPlayer);

 return 0;
}