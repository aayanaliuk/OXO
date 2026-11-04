grid = [[0 for _ in range(3)] for _ in range(3)]

# X_PLAYER and X_MARK must be equal to each other
EMPTY = 0
X_MARK = 1
Y_MARK = -1

DRAW = 0
X_PLAYER = 1
Y_PLAYER = -1


def game():
    turn = 0
    currentPlayer = X_PLAYER

    while True:
        printGrid()
        playerMove(currentPlayer)
        turn += 1

        if endGameCheck(currentPlayer) == currentPlayer:
            break
        elif turn >= 9:
            # If the current player hasnt won and there have been 9 turns then its a draw
            currentPlayer = DRAW
            break
        else:
            # Each player is the negative of the other.
            currentPlayer *= -1

    printGrid()
    winScreen(currentPlayer)

def printGrid():
    for row in range(3):
        print(f" {getGridValue(row, 0)} │ {getGridValue(row, 1)} │ {getGridValue(row, 2)} ")

        if row < 2:
            print("───┼───┼───")

def getPlayer(player):
    if player == X_PLAYER:
        return "X"
    else:
        return "O"

def getGridValue(row, column):
    gridVal = grid[row][column]

    if gridVal == EMPTY:
        # The plus 1 is so that everything starts at one and counts up
        # This places the numbers that users type so they can pick a square
        return str(1 + column + 3 * row)
    elif gridVal == X_MARK:
        return "X"
    elif gridVal == Y_MARK:
        return "O"


def playerMove(player):
    strPlayer = getPlayer(player)
    (row, column) = getPlayerMove(strPlayer)
    grid[row][column] = player

def getPlayerMove(strPlayer: str):
    while True:
        strInput = input(
            "Where would player " + strPlayer +
            " like to go? Please type in the number of a valid square: "
        )

        try:
            intInput: int = int(strInput)

            if intInput < 1:
                print("Too Low, please pick a valid number")

            elif intInput > 9:
                print("Too high, please pick a valid number")

            else:
                row = (intInput - 1) // 3
                column = (intInput - 1) % 3

                if grid[row][column] == EMPTY:
                    return (row, column)
                else:
                    print(
                        "That location already has a " +
                        getGridValue(row, column) +
                        " in it. Please pick a different square"
                    )

        except ValueError:
            print("Please type an integer between 1 and 9 inclusive.")


def winScreen(winner):
    if winner == DRAW:
        print("You both drew, sorry, maybe next time")
    else:
        print("Player " + getPlayer(winner) + " wins!")

def endGameCheck(player):
    # Could be more optimes by running the check after every function but makes the code look messier
    horizontal = endGameCheckHorizontal(player, grid)
    vertical = endGameCheckVertical(player, grid)
    diagonal = endGameCheckDiagonal(player)

    if horizontal == player or vertical == player or diagonal == player:
        return player

    return DRAW


def endGameCheckHorizontal(player, gridInput):
    for row in range(3):
        # X_PLAYER and X_MARK must be equal to each other
        winCondition: bool = gridInput[row] == [player, player, player]

        if winCondition:
            return player

    return DRAW


def endGameCheckVertical(player, gridInput):
    rotatedGrid = [[gridInput[row][column] for row in range(3)] for column in range(3)]
    return endGameCheckHorizontal(player, rotatedGrid)


def endGameCheckDiagonal(player):
    # Hard coded for a 3x3 grid
    diagonalOne = [grid[row][row] for row in range(3)]
    winConditionOne: bool = diagonalOne == [player, player, player]

    diagonalTwo = [grid[2 - row][row] for row in range(3)]
    winConditionTwo: bool = diagonalTwo == [player, player, player]

    return player if winConditionOne or winConditionTwo else DRAW


    

game()