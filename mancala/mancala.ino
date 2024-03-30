/*
My implementation of Mancala.
This first version simply plays the game via Serial Monitor.
The next version will be implemented fully using Arduino's LCD screen.
*/

constexpr int boardWidth{6};
constexpr int playerCount{2};

/**
 * @brief The Mancala board.
 *
 * @note Instructions.
 * I) Call `makeMove` to move a pit.
 * II) Call `isGameOver` to check if the game is over.
 */
class MancalaBoard
{
private:
    /*
    The board is represented as a 2x6 array
    Be aware! Each side of the board is updated such that index 0 is to that player's
    imaginary left hand side and index 5 is to that player's imaginary right hand side.
     */
    int playerPits[playerCount][boardWidth]{
        {4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 4}};
    int playerStore[playerCount]{0, 0}; // the player's store
    int activePlayer{0};                // the current player (0 or 1)
    int lastPlayerPit[2]{-1, -1};       // the last pit a stone was placed in (player then pit index)

    /**
     * @brief Checks if a player's side of the board is empty.
     *
     * @param player The player whose side of the board to check (0 or 1).
     * @return `true` if the side is empty, `false` otherwise.
     */
    bool isPlayerEmpty(int player)
    {
        for (int i{0}; i < boardWidth; i++)
        {
            if (playerPits[player][i] != 0)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Makes a steal if possible.
     */
    void makeSteal()
    {
        // check if the player just landed on their own side
        if (lastPlayerPit[0] == activePlayer)
        {
            // check if the player just landed in an empty pit
            if (playerPits[lastPlayerPit[0]][lastPlayerPit[1]] == 1)
            {
                // check if the pit across the board is not empty
                int oppositePit = boardWidth - lastPlayerPit[1] - 1;
                int oppositeSide = 1 - lastPlayerPit[0];
                if (playerPits[oppositeSide][oppositePit] != 0)
                {
                    // steal the stones
                    playerStore[activePlayer] += playerPits[oppositeSide][oppositePit];
                    playerPits[oppositeSide][oppositePit] = 0;
                    playerStore[activePlayer] += playerPits[lastPlayerPit[0]][lastPlayerPit[1]];
                    playerPits[lastPlayerPit[0]][lastPlayerPit[1]] = 0;
                }
            }
            justStole = true;
        }
        justStole = false;
    }

    /**
     * @brief Switches the `activePlayer` and updates the user facing `player` variable.
     */
    void switchActivePlayer()
    {
        if (activePlayer == 0)
        {
            activePlayer = 1;
            player = 2;
        }
        else
        {
            activePlayer = 0;
            player = 1;
        }
    }

public:
    int player = 1;                // player 1 or 2 (this is to display to users, not to change the game state)
    bool justStole{false};         // if the player just stole from the other player
    bool justLandedInStore{false}; // if the player just landed in their store

    /**
     * @brief Retrieves the player's store.
     *
     * @return The player's store as a constant reference to `playerPits`.
     */
    auto getPlayerPitsView() -> const decltype(playerPits) &
    {
        return playerPits;
    }

    /**
     * @brief Checks if the game is over.
     *
     * @return `true` for a game over state, `false` otherwise.
     */
    int isGameOver()
    {
        for (int i{0}; i < playerCount; i++)
        {
            if (!isPlayerEmpty(i))
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Moves a selected pit from the active player's side.
     *
     * @note
     * I) Moves stones
     * II) Makes a steal if possible
     * III) Determines the next player
     *
     * @param pit The pit (0-5) to move.
     */
    void makeMove(int pit)
    {
        // pick up stones from the pit
        int stones{playerPits[activePlayer][pit]};
        playerPits[activePlayer][pit] = 0;

        int activeSide{activePlayer}; // track the side of the board stones are being placed on

        while (stones > 0)
        {
            pit++; // go to set a stone in the next pit

            if (pit == boardWidth) // it's a store
            {
                activeSide = 1 - activeSide; // switch to the other side of the board
                pit = 0;                     // go to the first pit
            }

            // special case, the player just passed a store
            if (pit == 0)
            {
                if (activeSide != activePlayer) // the player just passed their own store
                {
                    playerStore[activePlayer]++; // they score
                    stones--;
                }
                if (stones == 0) // the player just passed their store and has no more stones
                {
                    justLandedInStore = true;
                    break;
                }
            }

            // drop a stone in this pit
            playerPits[activeSide][pit]++;
            stones--;
            justLandedInStore = false;
        }
        // remember the last pit a stone was placed in
        lastPlayerPit[0] = activeSide;
        lastPlayerPit[1] = pit;

        if (!justLandedInStore) // the player can't steal if they just landed in their store
        {
            makeSteal();
        }

        if (!justStole) // the player gets another turn if they just stole
        {
            switchActivePlayer();
        }
    }
};

void setup()
{
}

void loop()
{
}