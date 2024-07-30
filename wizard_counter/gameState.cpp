#include "GameState.h"

// Constructor to initialize the game state.
GameState::GameState(int playerCount, int dealer) : m_playerCount{playerCount}, m_dealer{dealer}
{
    // Initialize players with empty names.
    for (int i = 0; i < m_playerCount; ++i)
    {
        m_players[i] = Player();
    }
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

// Method to update the player's score based on trick points.
void GameState::updatePlayerScore(int playerIndex, int tricks)
{
    if (playerIndex >= 0 && playerIndex < m_playerCount)
    {
        int bidOffset = abs(m_players[playerIndex].bid - tricks);
        if (bidOffset == 0)
        {
            m_players[playerIndex].score += 20 + m_players[playerIndex].bid * 10;
        }
        else
        {
            m_players[playerIndex].score -= bidOffset * 10;
        }
    }
}