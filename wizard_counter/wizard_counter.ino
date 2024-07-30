/*
A counter for the card game Wizard.
*/

#include <LiquidCrystal.h>
#include "wizardScreen.h"
#include "controlPanel.h"

using namespace screen;

LiquidCrystal lcd(rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin);
ControlPanel panel;

void setup()
{
    // Set up initial LCD state.
    lcd.begin(screenWidth, screenHeight);
    lcd.clear();
    centerPrint(lcd, welcomeMessageTop, 0);
    centerPrint(lcd, welcomeMessageBottom, 1);

    delay(1000);
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