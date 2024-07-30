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
    welcomeMessage();
    int count{playerCount()};
    char countChar[2]{static_cast<char>(count + '0'), '\0'};
    centerPrint(lcd, countChar, 0);
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

int playerCount()
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