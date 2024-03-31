#ifndef MANCALABOARD_H
#define MANCALABOARD_H

namespace board
{
    constexpr int boardWidth{6};
    constexpr int playerCount{2};
}

using namespace board;

class MancalaBoard
{
private:
    struct MoveResult
    {
        int lastPit;
        int lastSide;
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
