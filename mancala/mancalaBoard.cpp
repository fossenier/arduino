#include "mancalaBoard.h"

using namespace board;

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
    /**
     * @brief The result of a move. The last pit and side of the board a stone was placed.
     */
    struct MoveResult
    {
        int lastPit;
        int lastSide;
    };

    /*
    The board is represented as a 2x6 array
    Be aware! Each side of the board is updated such that index 0 is to that player's
    imaginary left hand side and the last index is to that player's imaginary right hand side.
     */
    int playerPits[playerCount][boardWidth]{
        {4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 4}};
    int playerStore[playerCount]{0, 0}; // the player's store
    int activePlayer{0};                // the current player (0 or 1)

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
     *
     * @note Toggles the `justStole` variable.
     *
     * @param lastPit The last pit a stone was placed.
     * @param lastSide The last side of the board a stone was placed.
     */
    void makeSteal(int lastPit, int lastSide)
    {
        // check if the player just landed on their own side
        if (lastSide == activePlayer)
        {
            // check if the player just landed in an empty pit
            if (playerPits[lastSide][lastPit] == 1)
            {
                // check if the pit across the board is not empty
                int oppositePit = boardWidth - lastPit - 1;
                int oppositeSide = 1 - lastSide;
                if (playerPits[oppositeSide][oppositePit] != 0)
                {
                    // steal the stones
                    playerStore[activePlayer] += playerPits[oppositeSide][oppositePit];
                    playerPits[oppositeSide][oppositePit] = 0;
                    playerStore[activePlayer] += playerPits[lastSide][lastPit];
                    playerPits[lastSide][lastPit] = 0;

                    justStole = true;
                }
            }
        }
        justStole = false;
    }

    /**
     * @brief Moves a selected pit from the active player's side.
     *
     * @note Toggles the `justLandedInStore` variable.
     *
     * @param pit The pit (0-5) to move.
     *
     * @return `std::pair<int, int>` The last pit and side of the board a stone was placed.
     */
    MoveResult moveStones(int pit)
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
                    // the last stone was placed on the other side of the board before the store
                    return {boardWidth - 1, 1 - activeSide};
                }
            }

            // drop a stone in this pit
            playerPits[activeSide][pit]++;
            stones--;
        }
        justLandedInStore = false;
        return {pit, activeSide};
    }

    /**
     * @brief Switches the `activePlayer` and updates the user facing `player` variable.
     */
    void switchActivePlayer()
    {
        if (activePlayer == 0)
        {
            activePlayer = 1;
        }
        else
        {
            activePlayer = 0;
        }
    }

public:
    bool justStole{false};         // if the player just stole from the other player
    bool justLandedInStore{false}; // if the player just landed in their store

    /**
     * @brief Retrieves the active player. 0 for player 1 and 1 for player 2.
     *
     * @return The active player as a constant int.
     */
    const int getPlayer()
    {
        return activePlayer;
    }

    /**
     * @brief Retrieves the player's store.
     *
     * @param player The player whose store to retrieve (0 or 1).
     *
     * @return The player's store as a constant int.
     */
    const int getGameScore(int player)
    {
        return playerStore[player];
    }

    /**
     * @brief Retrieves the player's store.
     *
     * @note Pit 0 for both players is to their left-hand side and pit 5 is to their right-hand side.
     * Assuming each player is facing the board sitting across from each other.
     *
     * @return The player's store as a constant reference to `playerPits`.
     */
    auto getGameState() -> const decltype(playerPits) &
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
     * @brief Performs a player's move action.
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
        MoveResult lastPitSide{moveStones(pit)};

        if (!justLandedInStore) // the player can't steal if they just landed in their store
        {
            makeSteal(lastPitSide.lastPit, lastPitSide.lastSide);
        }

        if (!justLandedInStore) // the player gets another turn if they landed in their store
        {
            switchActivePlayer();
        }
    }
};