#ifndef MANCALABOARD_H
#define MANCALABOARD_H

namespace board
{
    constexpr int boardWidth{6};
    constexpr int playerCount{2};

    struct Pair
    {
        int first{};
        int second{};

        Pair(int f, int s) : first(f), second(s) {}
    };

    using PlayerPitsType = int[playerCount][boardWidth];
}

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
    int m_activePlayer{};
    int m_playerPits[playerCount][boardWidth]{};
    int m_playerStores[playerCount]{};

    bool isPlayerEmpty(int player);
    void makeSteal(int lastPit, int lastSide);
    Pair moveStones(int pit);
    void switchActivePlayer();

public:
    bool justStole{};
    bool justLandedInStore{};

    MancalaBoard();
    Pair endGame();
    const int getPlayer() const;
    const int getGameScore(int player) const;
    const PlayerPitsType &getGameState() const;
    bool isGameOver();
    bool makeMove(int pit);
};

#endif
