#include "GameState.h"

// Constructor to initialize the game state.
GameState::GameState(int playerCount, int dealer) : m_playerCount{playerCount}, m_dealer{dealer}
{
    // Initialize players with empty names.
    for (int i = 0; i < m_playerCount; ++i)
    {
        m_players[i] = Player();
    }
    m_playerCount = playerCount;
}

// Method to get the player's name.
char *GameState::getPlayerName(int playerIndex)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        return m_players[playerIndex].name;
    }
    return nullptr;
}

// Method to get the player's score.
int GameState::getPlayerScore(int playerIndex)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        return m_players[playerIndex].score;
    }
    return 0;
}

// Method to set the player's name.
void GameState::setPlayerName(int playerIndex, const char name[])
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        // Copy each of the 3 letters of the name (plus null).
        for (int i = 0; i < 4; ++i)
        {
            m_players[playerIndex].name[i] = name[i];
        }
    }
}

// Method to set the player's bid.
void GameState::setPlayerBid(int playerIndex, int bid)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        m_players[playerIndex].bid = bid;
    }
}

// Method to shift the dealer to the next player.
void GameState::shiftDealer()
{
    m_dealer = (m_dealer + 1) % m_playerCount;
}

// Method to update the player's score based on trick points.
int GameState::updatePlayerScore(int playerIndex, int tricks)
{
    int bid = m_players[playerIndex].bid;
    int bidOffset = (bid > tricks) ? (bid - tricks) : (tricks - bid);
    int score{0};
    if (bidOffset == 0)
    {
        score += 20 + bid * 10;
    }
    else
    {
        score -= bidOffset * 10;
    }
    m_players[playerIndex].score += score;
    return score;
}