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
void getGridValue(int row, int column, char gridValue[]);
void printGrid();

int getPlayerMove(char strPlayer[]);
void getPlayer(int player, char strPlayer[]);

int endGameCheckHorizontal(int player, int gridInput[3][3]);
int endGameCheckVertical(int player, int gridInput[3][3]);
int endGameCheckDiagonal(int player, int gridInput[3][3]);
int endGameCheck(int player);

void winScreen(int winner);
void playerMove(int currentPlayer);


void getGridValue(int row, int column, char gridValue[]) {
    int gridVal = grid[row][column];

    if (gridVal == EMPTY) {
        // The plus 1 is so that everything starts at one and counts up
        // This places the numbers that users type so they can pick a square

        gridValue[0] = '1' + column + 3 * row;
        gridValue[1] = '\0';
    }
    else if (gridVal == X_MARK) {
        gridValue[0] = 'X';
        gridValue[1] = '\0';
    }
    else if (gridVal == O_MARK) {
        gridValue[0] = 'O';
        gridValue[1] = '\0';
    }
    else {
        gridValue[0] = '?';
        gridValue[1] = '\0';
    }
}


void printGrid() {
    char value1[2];
    char value2[2];
    char value3[2];

    for (int row = 0; row < 3; row++) {
        getGridValue(row, 0, value1);
        getGridValue(row, 1, value2);
        getGridValue(row, 2, value3);

        printf(" %s │ %s │ %s\n", value1, value2, value3);

        if (row < 2) {
            printf("───┼───┼───\n");
        }
        else {
            printf("   │   │   \n");
        }
    }
}


int getPlayerMove(char strPlayer[]) {
    char strInput[100];

    while (true) {
        printf("Where would player %s like to go? Please type in the number of a valid square: ",
            strPlayer);

        if (scanf("%99s", strInput) != 1) {
            printf("Input error.\n");
            exit(EXIT_FAILURE);
        }

        errno = 0;
        char* endPointer;

        long intInput = strtol(strInput, &endPointer, 10);

        if (endPointer == strInput ||
            *endPointer != '\0' ||
            errno == ERANGE) {

            printf("Please type an integer between 1 and 9 inclusive.\n");
            continue;
        }

        if (intInput < 1) {
            printf("Too Low, please pick a valid number\n");
        }
        else if (intInput > 9) {
            printf("Too high, please pick a valid number\n");
        }
        else {
            int row = ((int)intInput - 1) / 3;
            int column = ((int)intInput - 1) % 3;

            if (grid[row][column] == EMPTY) {
                return (int)intInput;
            }
            else {
                char gridValue[2];
                getGridValue(row, column, gridValue);

                printf(
                    "That location already has a %s in it. Please pick a different square\n",
                    gridValue
                );
            }
        }
    }
}


void getPlayer(int player, char strPlayer[]) {
    if (player == X_PLAYER) {
        strPlayer[0] = 'X';
    }
    else {
        strPlayer[0] = 'O';
    }

    strPlayer[1] = '\0';
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
    if (gridInput[1][1] != player)
        return DRAW;

    // Top-left to bottom-right
    if (gridInput[0][0] == player && gridInput[2][2] == player)
        return player;

    // Top-right to bottom-left
    if (gridInput[0][2] == player && gridInput[2][0] == player)
        return player;

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
    if (winner == DRAW) {
        printf("You both drew, sorry, maybe next time\n");
    }
    else {
        char strPlayer[2];
        getPlayer(winner, strPlayer);

        printf("Player %s wins!\n", strPlayer);
    }
}


void playerMove(int currentPlayer) {
    char strPlayer[2];

    getPlayer(currentPlayer, strPlayer);

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

        if (endGameCheck(currentPlayer) == currentPlayer) {
            break;
        }
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
