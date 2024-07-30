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

    // Welcome the player to the program.
    responsiveMessage(welcomeMessageTop1, welcomeMessageBottom1, welcomeMessageTop2, welcomeMessageBottom2);

    // Find out how many are playing.
    int playerCount{getPlayerCount()};
    // Set the round limit.
    roundLimit = cardCount / playerCount;
    // Find out who is playing.
    char playerNames[maxPlayers][nameLength + 1]{};
    for (int i = 0; i < playerCount; ++i)
    {
        char name[nameLength + 1]; // Create the variable here
        getPlayerName(name);       // Pass it as a parameter

        for (int j = 0; j < nameLength + 1; ++j)
        {
            playerNames[i][j] = name[j];
        }
    }
    // Find out who the dealer is.
    int dealer{getDealer(playerNames, playerCount)};

    // Create the game state.
    game = new GameState(playerCount, dealer);
    // Store the player names.
    for (int i = 0; i < playerCount; ++i)
    {
        game->setPlayerName(i, playerNames[i]);
    }
}

void loop()
{
    // Track the rounds, and stop when reaching the round limit which, in Wizard,
    // is when player's hands are as big is possible given the deck size.
    currentRound++;
    if (currentRound >= roundLimit)
        endGame();

    // 1. Get the bids from each player.
    int bidSum{0};                               // The sum of the bids for this round (for display).
    const int leftOfDealer = game->m_dealer + 1; // The starting player this round.

    for (int i = leftOfDealer; i < game->m_playerCount + leftOfDealer; ++i)
    {
        // Represents each player in turn order from left of dealer clockwise ending with the dealer.
        int j = i % game->m_playerCount;
        char *playerName{game->getPlayerName(j)};

        // Find out the player's bid.
        int bid{getPlayerBid(playerName, bidSum, j == game->m_dealer)};
        game->setPlayerBid(j, bid);
        bidSum += bid;
    }
    // Display the final sum of bids made this round.
    centerPrint(lcd, roundBidTop, 0);
    centerPrint(lcd, bidSum, 1);
    delay(messageDelay);
    lcd.clear();

    // 2. Wait while players play the round.
    responsiveMessage(waitingMessage, emptyMessage, emptyMessage, waitingMessage);

    // 3. Find out how many tricks each player earned and award points.
    for (int i = leftOfDealer; i < game->m_playerCount + leftOfDealer; ++i)
    {
        // Represents each player in turn order from left of dealer clockwise ending with the dealer.
        int j = i % game->m_playerCount;
        char *playerName{game->getPlayerName(j)};

        // Find out the player's earned tricks and update their score.
        int tricks{getPlayerTricks(playerName)};
        int score{game->updatePlayerScore(j, tricks)};

        // Display the player's earned score.
        centerPrint(lcd, playerTricksSuccessTop, 0);
        centerPrint(lcd, score, 1);
        delay(messageDelay);
        lcd.clear();
    }

    // Shift the dealer clockwise.
    game->shiftDealer();

    // Display the new dealer.
    centerPrint(lcd, game->getPlayerName(game->m_dealer), 0);
    centerPrint(lcd, dealerMessageBottom, 1);
    delay(messageDelay);
    lcd.clear();

    // Display the standings after this round. Store the winning player (ties aren't handled).
    winningPlayer = displayStandings();
}

/**
 * @brief Displays two alternating messages on the LCD until the user presses the select button.
 *
 * This function continuously cycles between two pairs of messages displayed on an LCD.
 * It checks for the user's selection to exit the loop. The messages are displayed centered
 * on the screen using the `centerPrint` function.
 *
 * @param messageTop1 The first top line message to be displayed.
 * @param messageBottom1 The first bottom line message to be displayed.
 * @param messageTop2 The second top line message to be displayed.
 * @param messageBottom2 The second bottom line message to be displayed.
 */
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

/**
 * @brief Prompts the user to select the number of players and returns the selected count.
 *
 * This function displays a message on the LCD instructing the player to select the number of players.
 * The user can increase or decrease the count using the next and previous buttons, respectively.
 * The function waits for the user to confirm their selection by pressing the select button.
 *
 * @return The selected number of players.
 */
int getPlayerCount()
{
    // Display the prompt for selecting the number of players.
    centerPrint(lcd, playerCountTop, 0);
    int count{minPlayers};

    // Loop until the user confirms the selection.
    while (true)
    {
        panel.cycle();
        if (panel.isSelect())
            break;

        // If the next button is pressed, increase the player count.
        else if (panel.isNext())
        {
            if (count < maxPlayers)
            {
                ++count;
            }
        }
        // If the previous button is pressed, decrease the player count.
        else if (panel.isPrev())
        {
            if (count > minPlayers)
            {
                --count;
            }
        }

        // Display the current player count.
        centerPrint(lcd, count, 1);
    }

    // Clear the LCD before returning the count.
    lcd.clear();
    return count;
}

/**
 * @brief Prompts the user to enter their name and populates the passed name parameter.
 *
 * This function allows the user to input their name one character at a time.
 * The user navigates through segments of the alphabet to select each character.
 * Once all characters are selected, the name is displayed on the LCD.
 *
 * @param name A pointer to the character array to be populated with the player's name.
 */
void getPlayerName(char *name)
{
    int nameIndex = 0;            // Index of the current character being edited.
    int segmentStart = 0;         // Start index of the current three-character segment.
    bool selectingSegment = true; // Flag to indicate if selecting a segment or character.

    const int selectionStart = (screenWidth - nameLength) / 2; // Starting index of the selection on the screen.
    int cursorIndex = selectionStart;                          // Current index of the cursor on the screen.

    // Display the prompt for entering the player's name.
    centerPrint(lcd, playerNameTop, 0);

    // Display the initial segment of the alphabet.
    for (int i = 0; i < segmentLength; ++i)
    {
        lcd.setCursor(selectionStart + i, 1);
        lcd.print(alphabet[segmentStart + i]);
    }
    // Display the cursor without blinking initially.
    lcd.setCursor(cursorIndex, 1);
    lcd.noBlink();

    // Loop until the full name is entered.
    while (nameIndex < nameLength)
    {
        panel.cycle();

        // If the select button is pressed.
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

                // Reset to the initial segment (ABC).
                segmentStart = 0;
                for (int i = 0; i < segmentLength; ++i)
                {
                    lcd.setCursor(selectionStart + i, 1);
                    lcd.print(alphabet[segmentStart + i]);
                }

                lcd.setCursor(cursorIndex, 1);
            }
        }
        // If the next button is pressed.
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
        // If the previous button is pressed.
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

    // Display the sellected name.
    centerPrint(lcd, name, 0);
    centerPrint(lcd, playerNameBottom, 1);
    delay(messageDelay);
    lcd.clear();
}

/**
 * @brief Prompts the players to select the dealer and returns the selected dealer's index.
 *
 * This function displays a prompt asking the players to select the dealer.
 * The players can cycle through the list of player names using the next and previous buttons.
 * Once the dealer is selected by pressing the select button, the function returns the index of the dealer.
 *
 * @param playerNames A 2D array containing the names of the players.
 * @param playerCount The number of players.
 * @return The index of the selected dealer.
 */
int getDealer(char playerNames[][nameLength + 1], int playerCount)
{
    // Display the prompt for selecting the dealer.
    centerPrint(lcd, dealerMessageTop, 0);

    // Assume the first player is the dealer.
    int dealer = 0;

    // Display the first player's name initially.
    centerPrint(lcd, playerNames[dealer], 1);

    // Loop until the dealer is selected.
    while (true)
    {
        panel.cycle();

        // If the select button is pressed, exit the loop.
        if (panel.isSelect())
            break;

        // If the next button is pressed, move to the next player.
        else if (panel.isNext())
        {
            dealer = (dealer + 1) % playerCount;
            centerPrint(lcd, playerNames[dealer], 1);
        }

        // If the previous button is pressed, move to the previous player.
        else if (panel.isPrev())
        {
            dealer = (dealer - 1 + playerCount) % playerCount;
            centerPrint(lcd, playerNames[dealer], 1);
        }
    }

    // Notify the players that the dealer has been selected.
    lcd.clear();
    centerPrint(lcd, playerNames[dealer], 0);
    centerPrint(lcd, dealerMessageBottom, 1);
    delay(messageDelay);

    lcd.clear();
    return dealer;
}

/**
 * @brief Prompts the player to enter their bid and returns the entered bid.
 *
 * This function allows the player to input their bid for the current round.
 * The player can increase or decrease their bid using the next and previous buttons, respectively.
 * If the dealer's bid causes the total bid sum to equal the current round, an error message is displayed.
 * The function waits for the player to confirm their bid by pressing the select button.
 *
 * @param playerName The name of the player placing the bid.
 * @param bidSum The sum of all bids placed so far in the current round.
 * @param dealer A boolean indicating if the current player is the dealer.
 * @return The player's bid for the current round.
 */
int getPlayerBid(const char *playerName, int bidSum, bool dealer)
{
    int promptLength = sizeof(playerBidTop);
    char prompt[promptLength];

    // Construct the prompt message by combining playerBidTop and the player's name.
    for (int i = 0; i < promptLength; ++i)
    {
        prompt[i] = playerBidTop[i];
    }
    for (int i = 0; i < nameLength; ++i)
    {
        prompt[i] = playerName[i];
    }

    centerPrint(lcd, prompt, 0);

    // Assume the minimum bid.
    int bid = minBid;

    // Loop until the player confirms their bid.
    while (true)
    {
        panel.cycle();

        // If the select button is pressed.
        if (panel.isSelect())
        {
            // Check if the dealer's bid causes the total bid sum to equal the current round.
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
        // If the next button is pressed, increase the bid.
        else if (panel.isNext() && bid < currentRound)
        {
            ++bid;
        }
        // If the previous button is pressed, decrease the bid.
        else if (panel.isPrev() && bid > minBid)
        {
            --bid;
        }

        // Display the current bid.
        lcd.setCursor(0, 1); // Assuming a 2-line LCD display
        lcd.print("Bid: ");
        lcd.print(bid);
    }

    // Display player bid.
    centerPrint(lcd, playerBidSuccessTop, 0);
    centerPrint(lcd, bid, 1);
    delay(messageDelay);
    lcd.clear();

    return bid;
}

/**
 * @brief Prompts the player to enter the number of tricks they have won and returns the entered number.
 *
 * This function allows the player to input the number of tricks they have won in the current round.
 * The player can increase or decrease the number using the next and previous buttons, respectively.
 * The function waits for the player to confirm their input by pressing the select button.
 *
 * @param playerName The name of the player entering the number of tricks won.
 * @return The number of tricks the player has won.
 */
int getPlayerTricks(const char *playerName)
{
    int promptLength = sizeof(playerTricksTop);
    char prompt[promptLength];

    // Construct the prompt message by combining playerTricksTop and the player's name.
    for (int i = 0; i < promptLength; ++i)
    {
        prompt[i] = playerTricksTop[i];
    }
    for (int i = 0; i < nameLength; ++i)
    {
        prompt[i] = playerName[i];
    }

    centerPrint(lcd, prompt, 0);

    // Assume the minimum tricks
    int tricks = minBid;

    // Loop until the player confirms the number of tricks.
    while (true)
    {
        panel.cycle();

        // If the select button is pressed, exit the loop.
        if (panel.isSelect())
        {
            break;
        }
        // If the next button is pressed, increase the number of tricks.
        else if (panel.isNext())
        {
            ++tricks;
        }
        // If the previous button is pressed, decrease the number of tricks.
        else if (panel.isPrev() && tricks > minBid)
        {
            --tricks;
        }

        // Display the earned tricks.
        lcd.setCursor(0, 1); // Assuming a 2-line LCD display
        lcd.print("Tricks: ");
        lcd.print(tricks);
    }

    // Clear the LCD before returning the tricks count.
    lcd.clear();
    return tricks;
}

/**
 * @brief Swaps the values of two integers.
 *
 * This function swaps the values of the two provided integer references.
 *
 * @param a Reference to the first integer.
 * @param b Reference to the second integer.
 */
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Swaps the values of two character pointers.
 *
 * This function swaps the values of the two provided character pointer references.
 *
 * @param a Reference to the first character pointer.
 * @param b Reference to the second character pointer.
 */
void swap(const char *&a, const char *&b)
{
    const char *temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Displays the standings of players based on their scores and returns the name of the top player.
 *
 * This function retrieves player scores and names from the game object, sorts them in descending order,
 * and displays the standings on the LCD. The display cycles through the standings until the select button is pressed.
 *
 * @return The name of the player with the highest score.
 */
const char *displayStandings()
{
    const int playerCount = game->m_playerCount;
    int scores[playerCount];
    const char *names[playerCount];

    // Copy scores and names from the game object.
    for (int i = 0; i < playerCount; ++i)
    {
        scores[i] = game->getPlayerScore(i);
        names[i] = game->getPlayerName(i);
    }

    // Bubble sort based on scores.
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

    // Loop until the select button is pressed.
    while (true)
    {
        panel.cycle();
        if (panel.isSelect())
            break;

        if (millis() - lastRefresh >= messageDelay)
        {
            // Clear the screen.
            lcd.clear();

            // Display the current player on the top row.
            lcd.setCursor(0, 0);
            lcd.print(currentPlayerIndex + 1);
            lcd.print(". ");
            lcd.print(names[currentPlayerIndex]);
            lcd.print(": ");
            lcd.print(scores[currentPlayerIndex]);

            // Update to the next player index.
            int nextPlayerIndex = (currentPlayerIndex + 1) % playerCount;

            // Display the next player on the bottom row.
            lcd.setCursor(0, 1);
            lcd.print(nextPlayerIndex + 1);
            lcd.print(". ");
            lcd.print(names[nextPlayerIndex]);
            lcd.print(": ");
            lcd.print(scores[nextPlayerIndex]);

            // Update to the next player.
            currentPlayerIndex = nextPlayerIndex;

            lastRefresh = millis();
        }
    }

    // Clear the LCD before returning the top player's name.
    lcd.clear();
    return names[0];
}

/**
 * @brief Ends the game by displaying a game over message and the final standings.
 *
 * This function displays a game over message on the LCD, including the name of the winning player.
 * After displaying the message for a delay, it continuously displays the final standings until interrupted.
 */
void endGame()
{
    // Display the game over message on the top row.
    centerPrint(lcd, gameOverTop, 0);

    // Construct the game over message with the winning player's name.
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

    // Display the constructed message on the bottom row.
    centerPrint(lcd, message, 1);

    // Delay for a specified duration to allow the message to be read.
    delay(messageDelay * 3);

    // Continuously display the final standings.
    while (true)
        displayStandings();
}