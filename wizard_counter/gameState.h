#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
private:
    struct Player
    {
        char name[4]{}; // Name of the player.
        int score{};    // Score of the player.
        int bid{};      // Bid of the player this round.

        Player() = default;

        Player(const char *n) : score(0), bid(0)
        {
            for (int i = 0; i < 4; ++i)
            {
                name[i] = n[i];
            }
        }
    };

    Player m_players[6]{}; // Array of players (at most 6).
    int m_playerCount{};   // Number of players.
    int m_dealer{};        // Index of the dealer.

public:
    GameState(int playerCount);
    void setPlayerName(int playerIndex, const char *name);
    void setPlayerBid(int playerIndex, int bid);
    void updatePlayerScore(int playerIndex, int trickPoints);
};

#endif