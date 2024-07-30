/*
A counter for the card game Wizard.
*/

#include <LiquidCrystal.h>
#include "wizardScreen.h"
#include "controlPanel.h"
#include "gameState.h"

using namespace gameState;
using namespace screen;

LiquidCrystal lcd(rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin);
ControlPanel panel;

GameState *game{nullptr};
int roundLimit{};
int currentRound{0};
const char *winningPlayer{nullptr};

void setup()
{
    // Set up initial LCD state.
    lcd.begin(screenWidth, screenHeight);
    lcd.clear();
    // Say hello to the player.
    responsiveMessage(welcomeMessageTop1, welcomeMessageBottom1, welcomeMessageTop2, welcomeMessageBottom2);
    // Find out who's playing.
    int playerCount{getPlayerCount()};
    char playerNames[maxPlayers][nameLength + 1]{};
    // Get the names of the players.
    for (int i = 0; i < playerCount; ++i)
    {
        char *name{getPlayerName()};
        for (int j = 0; j < nameLength + 1; ++j)
        {
            playerNames[i][j] = name[j];
        }
        centerPrint(lcd, playerNames[i], 0);
        centerPrint(lcd, playerNameBottom, 1);
        delay(messageDelay);
        lcd.clear();
    }
    // Get the dealer.
    int dealer{getDealer(playerNames, playerCount)};
    // Create the game state.
    game = new GameState(playerCount, dealer);
    // Set the player names.
    for (int i = 0; i < playerCount; ++i)
    {
        game->setPlayerName(i, playerNames[i]);
    }

    // Set the round limit.
    roundLimit = cardCount / playerCount;
}

void loop()
{
    currentRound++;
    // if (currentRound >= roundLimit)
    if (currentRound == 2)
        endGame();

    // Handle this round's starting on dealer and going clockwise.
    int bidSum{0};
    const int leftOfDealer = game->m_dealer + 1;
    for (int i = leftOfDealer; i < game->m_playerCount + leftOfDealer; ++i)
    {
        int j = i % game->m_playerCount;
        // Display player name.
        char *playerName{game->getPlayerName(j)};
        // Get player bid (0+).
        int bid{getPlayerBid(playerName, bidSum, j == game->m_dealer)};
        game->setPlayerBid(j, bid);
        bidSum += bid;
        // Display player bid.
        centerPrint(lcd, playerBidSuccessTop, 0);
        centerPrint(lcd, bid, 1);
        delay(messageDelay);
        lcd.clear();
    }
    // Display this round's total bids.
    centerPrint(lcd, roundBidTop, 0);
    centerPrint(lcd, bidSum, 1);
    delay(messageDelay);
    lcd.clear();
    // Await the round being played
    responsiveMessage(waitingMessage, emptyMessage, emptyMessage, waitingMessage);

    // Handle this round's trick points.
    for (int i = leftOfDealer; i < game->m_playerCount + leftOfDealer; ++i)
    {
        int j = i % game->m_playerCount;
        // Display player name.
        char *playerName{game->getPlayerName(j)};
        // Get player trick points (0+).
        int tricks{getPlayerTricks(playerName)};
        // Display player earned score.
        int score{game->updatePlayerScore(j, tricks)};
        centerPrint(lcd, playerTricksSuccessTop, 0);
        centerPrint(lcd, score, 1);
        delay(messageDelay);
        lcd.clear();
    }

    // Shift the dealer clockwise.
    game->shiftDealer();
    centerPrint(lcd, game->getPlayerName(game->m_dealer), 0);
    centerPrint(lcd, dealerMessageBottom, 1);
    delay(messageDelay);
    lcd.clear();

    // Display the standings.
    winningPlayer = displayStandings();
}

void endGame()
{
    centerPrint(lcd, gameOverTop, 0);
    int messageLength = sizeof(gameOverBottom);
    char message[messageLength];

    for (int i = 0; i < messageLength; ++i)
    {
        message[i] = gameOverBottom[i];
    }
    for (int i = 0; i < nameLength; ++i)
    {
        message[i] = winningPlayer[i];
    }
    centerPrint(lcd, message, 1);

    delay(messageDelay * 3);

    while (true)
        displayStandings();
}

// Function to swap two integers
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Function to swap two character pointers
void swap(const char *&a, const char *&b)
{
    const char *temp = a;
    a = b;
    b = temp;
}

const char *displayStandings()
{
    const int playerCount = game->m_playerCount;
    int scores[playerCount];
    const char *names[playerCount];

    // Copy scores and names from the game object
    for (int i = 0; i < playerCount; ++i)
    {
        scores[i] = game->getPlayerScore(i);
        names[i] = game->getPlayerName(i);
    }

    // Bubble sort based on scores
    for (int i = 0; i < playerCount - 1; ++i)
    {
        for (int j = 0; j < playerCount - i - 1; ++j)
        {
            if (scores[j] < scores[j + 1])
            {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
            }
        }
    }

    long lastRefresh = millis();
    int currentPlayerIndex = 0;

    while (true)
    {
        panel.cycle();
        if (panel.isSelect())
            break;

        if (millis() - lastRefresh >= messageDelay)
        {
            // Clear the screen
            lcd.clear();

            // Display the current player on the top row
            lcd.setCursor(0, 0);
            lcd.print(currentPlayerIndex + 1);
            lcd.print(". ");
            lcd.print(names[currentPlayerIndex]);
            lcd.print(": ");
            lcd.print(scores[currentPlayerIndex]);

            // Update to the next player index
            int nextPlayerIndex = (currentPlayerIndex + 1) % playerCount;

            // Display the next player on the bottom row
            lcd.setCursor(0, 1);
            lcd.print(nextPlayerIndex + 1);
            lcd.print(". ");
            lcd.print(names[nextPlayerIndex]);
            lcd.print(": ");
            lcd.print(scores[nextPlayerIndex]);

            // Update to the next player
            currentPlayerIndex = nextPlayerIndex;

            lastRefresh = millis();
        }
    }

    lcd.clear();
    return names[0];
}

int getPlayerTricks(const char *playerName)
{
    int promptLength = sizeof(playerTricksTop);
    char prompt[promptLength];

    for (int i = 0; i < promptLength; ++i)
    {
        prompt[i] = playerTricksTop[i];
    }
    for (int i = 0; i < nameLength; ++i)
    {
        prompt[i] = playerName[i];
    }

    centerPrint(lcd, prompt, 0);

    // Assume the minimum bid
    int tricks = minBid;

    while (true)
    {
        panel.cycle();

        if (panel.isSelect())
        {
            break;
        }
        else if (panel.isNext())
        {
            ++tricks;
        }
        else if (panel.isPrev() && tricks > 0)
        {
            --tricks;
        }

        // Display the earned tricks
        lcd.setCursor(0, 1); // Assuming a 2-line LCD display
        lcd.print("Tricks: ");
        lcd.print(tricks);
    }

    lcd.clear();
    return tricks;
}

int getPlayerBid(const char *playerName, int bidSum, bool dealer)
{
    int promptLength = sizeof(playerBidTop);
    char prompt[promptLength];

    for (int i = 0; i < promptLength; ++i)
    {
        prompt[i] = playerBidTop[i];
    }
    for (int i = 0; i < nameLength; ++i)
    {
        prompt[i] = playerName[i];
    }

    centerPrint(lcd, prompt, 0);

    // Assume the minimum bid
    int bid = 0;

    while (true)
    {
        panel.cycle();

        if (panel.isSelect())
        {
            if (dealer && bidSum + bid == currentRound)
            {
                centerPrint(lcd, playerBidErrorTop, 0);
                delay(messageDelay);
                centerPrint(lcd, prompt, 0);
            }
            else
            {
                break;
            }
        }
        else if (panel.isNext() && bid < currentRound)
        {
            ++bid;
        }
        else if (panel.isPrev() && bid > 0)
        {
            --bid;
        }

        // Display the current bid
        lcd.setCursor(0, 1); // Assuming a 2-line LCD display
        lcd.print("Bid: ");
        lcd.print(bid);
    }

    lcd.clear();
    return bid;
}

int getDealer(char playerNames[][nameLength + 1], int playerCount)
{
    // Tell the player to select the dealer.
    centerPrint(lcd, dealerMessageTop, 0);

    // Assume the first player is the dealer.
    int dealer = 0;

    // Display the first player's name initially
    centerPrint(lcd, playerNames[dealer], 1);

    while (true)
    {
        panel.cycle();

        // If the player selects, break out of the loop.
        if (panel.isSelect())
            break;
        // If the player presses next, increment the dealer.
        else if (panel.isNext())
        {
            dealer = (dealer + 1) % playerCount;
            centerPrint(lcd, playerNames[dealer], 1);
        }
        // If the player presses prev, decrement the dealer.
        else if (panel.isPrev())
        {
            dealer = (dealer - 1 + playerCount) % playerCount;
            centerPrint(lcd, playerNames[dealer], 1);
        }
    }
    // Notify the player that the dealer has been selected.
    lcd.clear();
    centerPrint(lcd, playerNames[dealer], 0);
    centerPrint(lcd, dealerMessageBottom, 1);
    delay(messageDelay);

    lcd.clear();
    return dealer;
}

char *getPlayerName()
{
    static char name[nameLength + 1]{}; // The player's name (that we are building)
    int nameIndex = 0;                  // The index of the character we are currently editing
    int segmentStart = 0;               // The start of the current three-character segment
    bool selectingSegment = true;       // Whether we are selecting a segment
    const int segmentLength = 3;        // The length of the segment

    const int selectionStart = (screenWidth - nameLength) / 2; // The index of the first character on the screen
    int cursorIndex = selectionStart;                          // The index of the cursor on the screen

    // Tell the player to enter their name
    centerPrint(lcd, playerNameTop, 0);

    // Display the initial segment
    for (int i = 0; i < segmentLength; ++i)
    {
        lcd.setCursor(selectionStart + i, 1);
        lcd.print(alphabet[segmentStart + i]);
    }
    // Display the cursor without blinking initially
    lcd.setCursor(cursorIndex, 1);
    lcd.noBlink();

    // Once for each character in the name
    while (nameIndex < nameLength)
    {
        panel.cycle();

        // If the player selects
        if (panel.isSelect())
        {
            if (selectingSegment)
            {
                selectingSegment = false;
                lcd.blink();
            }
            else
            {
                selectingSegment = true;
                name[nameIndex] = alphabet[segmentStart + (cursorIndex - selectionStart) % segmentLength];
                lcd.noBlink();
                lcd.setCursor(cursorIndex, 1);
                lcd.print(name[nameIndex]);
                nameIndex++;
                cursorIndex = selectionStart + nameIndex;

                // Reset to initial segment (ABC)
                segmentStart = 0;
                for (int i = 0; i < segmentLength; ++i)
                {
                    lcd.setCursor(selectionStart + i, 1);
                    lcd.print(alphabet[segmentStart + i]);
                }

                lcd.setCursor(cursorIndex, 1);
            }
        }
        // If the player presses next
        else if (panel.isNext())
        {
            if (selectingSegment)
            {
                segmentStart = (segmentStart + segmentLength) % alphabetLength;
                for (int i = 0; i < segmentLength; ++i)
                {
                    lcd.setCursor(selectionStart + i, 1);
                    lcd.print(alphabet[segmentStart + i]);
                }
                lcd.setCursor(cursorIndex, 1);
            }
            else
            {
                cursorIndex = (cursorIndex + 1) % segmentLength + selectionStart;
                lcd.setCursor(cursorIndex, 1);
            }
        }
        // If the player presses prev
        else if (panel.isPrev())
        {
            if (selectingSegment)
            {
                segmentStart = (segmentStart - segmentLength + alphabetLength) % alphabetLength;
                for (int i = 0; i < segmentLength; ++i)
                {
                    lcd.setCursor(selectionStart + i, 1);
                    lcd.print(alphabet[segmentStart + i]);
                }
                lcd.setCursor(cursorIndex, 1);
            }
            else
            {
                cursorIndex = (cursorIndex - 1 + segmentLength) % segmentLength + selectionStart;
                lcd.setCursor(cursorIndex, 1);
            }
        }
    }

    name[nameLength] = '\0'; // Null-terminate the name
    lcd.noBlink();
    lcd.clear();
    return name;
}

int getPlayerCount()
{
    // Tell the player to select the number of players.
    centerPrint(lcd, playerCountTop, 0);

    // Assume the minimum number of players.
    int count{minPlayers};
    while (true)
    {
        panel.cycle();
        // If the player selects, break out of the loop.
        if (panel.isSelect())
            break;
        // If the player presses next, increment the count.
        else if (panel.isNext())
        {
            if (count < maxPlayers)
            {
                ++count;
            }
        }
        // If the player presses prev, decrement the count.
        else if (panel.isPrev())
        {
            if (count > minPlayers)
            {
                --count;
            }
        }
        // Display the current count.
        centerPrint(lcd, count, 1);
    }
    lcd.clear();
    return count;
}

void responsiveMessage(const char messageTop1[], const char messageBottom1[], const char messageTop2[], const char messageBottom2[])
{
    long lastRefresh{millis()};
    int displayMessage{0};

    // Display two alternating messages until the user presses select.
    while (true)
    {
        panel.cycle();
        if (panel.isSelect())
            break;

        if (millis() - lastRefresh < messageDelay)
        {
            if (displayMessage != 1)
            {
                lcd.clear();
                centerPrint(lcd, messageTop1, 0);
                centerPrint(lcd, messageBottom1, 1);
                displayMessage = 1;
            }
        }
        else if (millis() - lastRefresh < messageDelay * 2)
        {
            if (displayMessage != 2)
            {
                lcd.clear();
                centerPrint(lcd, messageTop2, 0);
                centerPrint(lcd, messageBottom2, 1);
                displayMessage = 2;
            }
        }
        else
        {
            lastRefresh = millis();
        }
    }
    lcd.clear();
}

/*

welcome player (welcome / press select)
wait for player to press select

get number of players (1-6)
create gamestate with that many players
get dealer

while game not over (60 cards / players remainder => last round)
    for each player (left of dealer)
        display player name
        get player bid (0+)
        display player bid
        AND if dealer, make sure sum of bids is not the round number
    for each player (left of dealer)
        display player name
        get player trick points (0+)
        display player earned score
    display standings

display GAME OVER!
display winner
display standings



*/