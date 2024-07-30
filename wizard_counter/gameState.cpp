#include "GameState.h"
#include <cstring>

// Constructor to initialize the game state.
GameState::GameState(int playerCount) : m_playerCount{playerCount}, m_dealer{0}
{
    // Initialize players with empty names.
    for (int i = 0; i < m_playerCount; ++i)
    {
        m_players[i] = Player();
    }
}

// Method to set the player's name.
void GameState::setPlayerName(int playerIndex, const char *name)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        std::strncpy(m_players[playerIndex].name, name, 4);
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

// Method to update the player's score based on trick points.
void GameState::updatePlayerScore(int playerIndex, int trickPoints)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        if (m_players[playerIndex].bid == trickPoints)
        {
            // Implement the score increase function as needed.
            m_players[playerIndex].score += trickPoints; // Placeholder implementation.
        }
    }
}