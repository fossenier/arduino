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
    struct MoveResult
    {
        int lastPit{};
        int lastSide{};

        MoveResult(int pit, int side) : lastPit(pit), lastSide(side) {}
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
    bool makeMove(int pit);
    Pair endGame();
};

#endif
