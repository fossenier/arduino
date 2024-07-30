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

void setup()
{
    // Set up initial LCD state.
    lcd.begin(screenWidth, screenHeight);
    lcd.clear();
    // Say hello to the player.
    welcomeMessage();
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
    GameState game(playerCount, dealer);
    // Set the player names.
    for (int i = 0; i < playerCount; ++i)
    {
        game.setPlayerName(i, playerNames[i]);
    }
}

void loop()
{
    panel.cycle();

    if (panel.isNext())
    {
        lcd.clear();
        centerPrint(lcd, "Next Pressed", 0);
        centerPrint(lcd, "Random Func 1", 1);
    }
    else if (panel.isSelect())
    {
        lcd.clear();
        centerPrint(lcd, "Select Pressed", 0);
        centerPrint(lcd, "Random Func 2", 1);
    }
    else if (panel.isPrev())
    {
        lcd.clear();
        centerPrint(lcd, "Prev Pressed", 0);
        centerPrint(lcd, "Random Func 3", 1);
    }
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

void welcomeMessage()
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
                centerPrint(lcd, welcomeMessageTop1, 0);
                centerPrint(lcd, welcomeMessageBottom1, 1);
                displayMessage = 1;
            }
        }
        else if (millis() - lastRefresh < messageDelay * 2)
        {
            if (displayMessage != 2)
            {
                lcd.clear();
                centerPrint(lcd, welcomeMessageTop2, 0);
                centerPrint(lcd, welcomeMessageBottom2, 1);
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