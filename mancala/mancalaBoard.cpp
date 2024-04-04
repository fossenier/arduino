#include "mancalaBoard.h"

using namespace board;

MancalaBoard::MancalaBoard()
    : m_playerPits{{4, 4, 4, 4, 4, 4}, {4, 4, 4, 4, 4, 4}}, // initialize all pits to have 4 stones
      m_playerStores{0, 0},                                 // initialize both player stores to 0
      m_activePlayer(0),                                    // start the game with player 0
      justStole(false),                                     // no stones have been stolen at the start of the game
      justLandedInStore(false)                              // no player has landed in their store at the start
{
}

/**
 * @brief Ends the game and calculates the final scores.
 *
 * @return `Pair` The final scores for player 1 and player 2.

*/
Pair MancalaBoard::endGame()
{
    // fetch store scores
    int player0Score{m_playerStores[0]};
    int player1Score{m_playerStores[1]};

    // empty out board into stores
    for (int i{0}; i < boardWidth; i++)
    {
        player0Score += m_playerPits[0][i];
        m_playerPits[0][i] = 0;
        player1Score += m_playerPits[1][i];
        m_playerPits[1][i] = 0;
    }

    // return the final scores
    return Pair{player0Score, player1Score};
}

/**
 * @brief Retrieves the active player. 0 for player 1 and 1 for player 2.
 *
 * @return The active player as a constant int.
 */
const int MancalaBoard::getPlayer() const
{
    return m_activePlayer;
}

/**
 * @brief Retrieves the player's store.
 *
 * @param player The player whose store to retrieve (0 or 1).
 *
 * @return The player's store as a constant int.
 */
const int MancalaBoard::getGameScore(int player) const
{
    return m_playerStores[player];
}

/**
 * @brief Retrieves the player's store.
 *
 * @note Pit 0 for both players is to their left-hand side and pit 5 is to their right-hand side.
 * Assuming each player is facing the board sitting across from each other.
 *
 * @return The player's store as a constant reference to `playerPits`.
 */
const PlayerPitsType &MancalaBoard::getGameState() const
{
    return m_playerPits;
}

/**
 * @brief Checks if the game is over.
 *
 * @return `true` for a game over state, `false` otherwise.
 */
bool MancalaBoard::isGameOver()
{
    // if either player's side is empty, the game is over
    if (isPlayerEmpty(0) || isPlayerEmpty(1))
    {
        return true;
    }
    // the game is not over
    return false;
}

/**
 * @brief Checks if a player's side of the board is empty.
 *
 * @param player The player whose side of the board to check (0 or 1).
 * @return `true` if the side is empty, `false` otherwise.
 */
bool MancalaBoard::isPlayerEmpty(int player)
{
    bool allEmpty{true};
    // check if all pits on the player's side are empty
    for (int i{0}; i < boardWidth; i++)
    {
        if (m_playerPits[player][i] != 0)
        {
            allEmpty = false;
        }
    }

    return allEmpty;
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
 *
 * @return `true` if the move was successful, `false` otherwise.
 */
bool MancalaBoard::makeMove(int pit)
{
    // check if the player can move from the selected pit
    if (m_playerPits[m_activePlayer][pit] == 0)
    {
        return false;
    }
    Pair lastPitSide{moveStones(pit)};

    if (!justLandedInStore) // the player can't steal if they just landed in their store
    {
        makeSteal(lastPitSide.first, lastPitSide.second);
    }

    if (!justLandedInStore) // the player gets another turn if they landed in their store
    {
        switchActivePlayer();
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
void MancalaBoard::makeSteal(int lastPit, int lastSide)
{
    // check if the player just landed on their own side
    if (lastSide == m_activePlayer)
    {
        // check if the player just landed in an empty pit
        if (m_playerPits[lastSide][lastPit] == 1)
        {
            // check if the pit across the board is not empty
            int oppositePit = boardWidth - lastPit - 1;
            int oppositeSide = 1 - lastSide;
            if (m_playerPits[oppositeSide][oppositePit] != 0)
            {
                // steal the stones
                m_playerStores[m_activePlayer] += m_playerPits[oppositeSide][oppositePit];
                m_playerPits[oppositeSide][oppositePit] = 0;
                m_playerStores[m_activePlayer] += m_playerPits[lastSide][lastPit];
                m_playerPits[lastSide][lastPit] = 0;

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
 * @return `Pair` The last pit and side of the board a stone was placed.
 */
Pair MancalaBoard::moveStones(int pit)
{
    // pick up stones from the pit
    int stones{m_playerPits[m_activePlayer][pit]};
    m_playerPits[m_activePlayer][pit] = 0;

    int activeSide{m_activePlayer}; // track the side of the board stones are being placed on

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
            if (activeSide != m_activePlayer) // the player just passed their own store
            {
                m_playerStores[m_activePlayer]++; // they score
                stones--;
            }
            if (stones == 0) // the player just passed their store and has no more stones
            {
                justLandedInStore = true;
                // the last stone was placed on the other side of the board before the store
                return Pair{0, 1 - activeSide};
            }
        }

        // drop a stone in this pit
        m_playerPits[activeSide][pit]++;
        stones--;
    }
    justLandedInStore = false;
    return Pair{pit, activeSide};
}

/**
 * @brief Switches the `activePlayer`.
 */
void MancalaBoard::switchActivePlayer()
{
    m_activePlayer = 1 - m_activePlayer;
}
