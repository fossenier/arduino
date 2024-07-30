#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace gameState
{
    constexpr int minPlayers{3};
    constexpr int maxPlayers{6};
    constexpr int nameLength{3};
    constexpr int cardCount{60};
    constexpr int minBid{0};
}

using namespace gameState;

class GameState
{
private:
    struct Player
    {
        char name[4]{}; // Name of the player.
        int score{};    // Score of the player.
        int bid{};      // Bid of the player this round.

        Player() = default;
    };

    Player m_players[6]{}; // Array of players (at most 6).

public:
    int m_dealer{};      // Index of the dealer.
    int m_playerCount{}; // Number of players.

    GameState(int playerCount, int dealer);
    char *getPlayerName(int playerIndex);
    int getPlayerScore(int playerIndex);
    void setPlayerName(int playerIndex, const char *name);
    void setPlayerBid(int playerIndex, int bid);
    void updatePlayerScore(int playerIndex, int tricks);
};

#endif