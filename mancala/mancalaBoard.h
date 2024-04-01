#ifndef MANCALABOARD_H
#define MANCALABOARD_H

namespace board
{
    constexpr int boardWidth{6};
    constexpr int playerCount{2};
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
    struct MoveResult
    {
        int lastPit{};
        int lastSide{};
    };

    int playerPits[playerCount][boardWidth];
    int playerStore[playerCount];
    int activePlayer;
    bool isPlayerEmpty(int player);
    void makeSteal(int lastPit, int lastSide);
    MoveResult moveStones(int pit);
    void switchActivePlayer();

public:
    bool justStole;
    bool justLandedInStore;

    MancalaBoard();
    const int getPlayer();
    const int getGameScore(int player);
    auto getGameState() -> const decltype(playerPits) &;
    int isGameOver();
    void makeMove(int pit);
};

#endif
