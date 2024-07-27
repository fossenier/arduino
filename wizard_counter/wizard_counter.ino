/*
A counter for the card game Wizard.
*/

#include <LiquidCrystal.h>

#include "wizardScreen.h"

using namespace screen;

LiquidCrystal lcd(rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin);
constexpr int nextButtonPin{12};
constexpr int selectButtonPin{13};

void setup()
{
    // Set up initial LCD state.
    lcd.begin(screenWidth, screenHeight);
    lcd.clear();
    centerPrint(lcd, welcomeMessageTop, 0);
    centerPrint(lcd, welcomeMessageBottom, 1);
}

void loop()
{
}