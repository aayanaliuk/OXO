#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// All elements initialise to 0 anyway
int grid[3][3] = {};
const int EMPTY = 0;
const int X_MARK = 1;
const int O_MARK = -1;

const int DRAW = 0;
const int X_PLAYER = 1;
const int O_PLAYER = -1;


// Function declarations
string getGridValue(int row, int column);
void printGrid();

int getPlayerMove(string strPlayer);
string getPlayer(int player);
int endGameCheckHorizontal(int player, int gridInput[3][3]);
int endGameCheckVertical(int player, int gridInput[3][3]);
int endGameCheckDiagonal(int player, int gridInput[3][3]);
int endGameCheck(int player);
void winScreen(int winner);
void playerMove(int currentPlayer);


string getGridValue(int row, int column) {
  int gridVal = grid[row][column];

  if (gridVal == EMPTY) {
    // The plus 1 is so that everything starts at one and counts up
    // This places the numbers that users type so they can pick a square
    return to_string(1 + column + 3 * row);
  }
  
  else if (gridVal == X_MARK) return "X";
  
  else if (gridVal == O_MARK) return "O";

  return "?";
}


void printGrid() {
  for (int row = 0; row < 3; row++) {
    cout << " " << getGridValue(row, 0)
         << " │ " << getGridValue(row, 1)
         << " │ " << getGridValue(row, 2) << '\n';

    if (row < 2) cout << "───┼───┼───" << '\n';
    
    else cout << "   │   │   " << '\n';
  }
}


int getPlayerMove(string strPlayer) {
  while (true) {
    cout << "Where would player " + strPlayer + " like to go? Please type in the number of a valid square: ";

    string strInput;
    cin >> strInput;

    try {
      size_t pos;
      int intInput = stoi(strInput, &pos);

      if (pos != strInput.length()) {
        cout << "Please type an integer between 1 and 9 inclusive." << endl;
        continue;
      }

      if (intInput < 1) cout << "Too Low, please pick a valid number" << endl;

      else if (intInput > 9) cout << "Too high, please pick a valid number" << endl;

      else {
        int row = (intInput - 1) / 3;
        int column = (intInput - 1) % 3;

        if (grid[row][column] == EMPTY) return intInput;
        else {
          cout << 
          (
            "That location already has a " +
            getGridValue(row, column) +
            " in it. Please pick a different square"
          ) << endl;
        }
      }
    }
    catch (...) {
        cout << "Please type an integer between 1 and 9 inclusive." << endl;
    }
  }
}


string getPlayer(int player) {
  if (player == X_PLAYER) return "X";
  else return "O";
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

  if (horizontal == player || vertical == player || diagonal == player) {
    return player;
  }

  return DRAW;
}


void winScreen(int winner) {
  if (winner == DRAW) cout << "You both drew, sorry, maybe next time" << endl;
  else cout << "Player " + getPlayer(winner) + " wins!" << endl;
}


void playerMove(int currentPlayer) {
  string strPlayer = getPlayer(currentPlayer);
  int num = getPlayerMove(strPlayer);

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