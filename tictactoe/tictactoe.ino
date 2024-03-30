/**
 * @class TicTacToeBoard
 * @brief A class to manage a TicTacToe game board.
 *
 * This class encapsulates the game state for a TicTacToe game, including the board layout,
 * the active player, and methods to make moves and check for a winner.
 */
class TicTacToeBoard
{
private:
    int activePlayer = 0;                                                   ///< Index of the currently active player (0 or 1).
    char players[2] = {'X', 'O'};                                           ///< Player symbols, 'X' for player 1, 'O' for player 2.
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}}; ///< The game board.

public:
    /**
     * @brief Prints the current state of the game board to the serial output.
     *
     * This method iterates over the board, printing each cell with borders. It's designed
     * to visually represent the board in a readable format via serial communication.
     */
    void printBoard()
    {
        for (int i = 0; i < 3; i++)
        {
            Serial.println("-------"); // Top border and front border
            Serial.print("|");
            for (int j = 0; j < 3; j++)
            {
                // Print value and divider (and end border)
                Serial.print(board[i][j]);
                Serial.print("|");
            }
            Serial.println();
        }
        Serial.println("-------"); // Bottom border
    }

    /**
     * @brief Attempts to make a move for the current player at the specified location.
     *
     * @param row The row in which to make the move (0-based).
     * @param col The column in which to make the move (0-based).
     * @return true If the move was successful (i.e., the cell was empty).
     * @return false If the move was not successful (i.e., the cell was already occupied).
     */
    bool makeMove(int row, int col)
    {
        if (board[row][col] == ' ')
        {
            board[row][col] = players[activePlayer]; // Make the move for the current player
            activePlayer = 1 - activePlayer;         // Switch active player
            return true;
        }
        else
        {
            return false; // Move was not successful
        }
    }

    /**
     * @brief Checks if the game has been won and by whom.
     *
     * This method checks all possible winning conditions for the TicTacToe game.
     * It checks all rows, columns, and diagonals for a winning condition (three of the same player's symbols).
     *
     * @return char The symbol of the winning player ('X' or 'O') or a space character (' ') if there's no winner yet.
     */
    char gameWon()
    {
        // Check rows and columns for winning conditions
        for (int i = 0; i < 3; i++)
        {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            {
                return board[i][0];
            }
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            {
                return board[0][i];
            }
        }

        // Check diagonals for winning conditions
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        {
            return board[0][0];
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
        {
            return board[0][2];
        }

        return ' '; // No winner yet
    }
    /**
     * Retrieves the active player.
     *
     * @return The character representing the active player.
     */
    char getActivePlayer()
    {
        return players[activePlayer];
    }
};

// Global instance of TicTacToeBoard
TicTacToeBoard board;
bool playerReady = false;
unsigned long lastMessageTime = 0;
const long messageInterval = 5000;

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 bits per second
    delay(1000);        // Wait a bit for serial monitor to open
}

void loop()
{
    while (!playerReady)
    {
        if (Serial.available() > 0)
        {
            Serial.read(); // Clear the input buffer
            playerReady = true;
        }
        else
        {
            unsigned long currentTime = millis();
            if (currentTime - lastMessageTime >= messageInterval)
            {
                Serial.println("TicTacToe Game, send any character to start.");
                lastMessageTime = currentTime;
            }
        }
    }

    if (Serial)
    {
        char activePlayer = board.getActivePlayer();
        Serial.print("Player ");
        Serial.print(activePlayer);
        Serial.println("'s turn.");
        board.printBoard();
        Serial.println("Enter row and column (1-3) separated by a space:");
        {
            int row = {-1};
            int col = {-1};
            while (true)
            {
                if (Serial.peek() == '\n')
                    Serial.read(); // discard newline character
                if (Serial.available() >= 3)
                {
                    row = Serial.parseInt();
                    col = Serial.parseInt();

                    if (row >= 1 && row <= 3 && col >= 1 && col <= 3)
                        break;
                    else
                        Serial.println("Invalid input. Enter row and column (1-3) separated by a space:");
                }
            }

            if (board.makeMove(row - 1, col - 1))
            {
                char winner = board.gameWon();
                if (winner != ' ')
                {
                    Serial.print("Player ");
                    Serial.print(winner);
                    Serial.println(" wins!");
                    // Reset the game
                    board = TicTacToeBoard();
                    playerReady = false;
                }
            }
            else
                Serial.println("Invalid move. Try again.");
        }
    }
}
