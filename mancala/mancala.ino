/*
My implementation of Mancala.
This second version is implemented fully using Arduino's LCD screen.
*/

#include <LiquidCrystal.h>

#include "mancalaBoard.h"
#include "mancalaScreen.h"

using namespace board;
using namespace screen;

MancalaBoard game;
LiquidCrystal lcd(rsPin, enPin, db4Pin, db5Pin, db6Pin, db7Pin);

void setup()
{
    constexpr char welcomeMessageTop[] = "send any key";
    constexpr char welcomeMessageBottom[] = "to begin Mancala";

    // the game state is set up

    // set up initial lcd state
    lcd.begin(screenWidth, screenHeight);
    lcd.clear();
    centerPrint(lcd, welcomeMessageTop, 0);
    centerPrint(lcd, welcomeMessageBottom, 1);

    // set up initial serial state
    Serial.begin(9600);
    Serial.print(welcomeMessageTop);
    Serial.print(blank);
    Serial.println(welcomeMessageBottom);

    while (Serial.available() < 1)
    {
        ;
    }
    lcd.clear();
    Serial.readStringUntil('\n');
}

void loop()
{
}

void displayGameState(const PlayerPitsType &state, int currentPlayer)
{
    // Loop for player and opponent
    for (int side = 0; side <= 1; ++side)
    {
        bool isCurrentPlayer = (side == currentPlayer);

        // Determine the display direction: always ascending in the array
        // but mirrored for the opponent when accessing the game state
        bool isAscending = true; // We'll keep the array filling ascending for alignment

        char pitNumbers[boardWidth * 2 + 1];
        int index = 0;

        for (int pit = 0; pit < boardWidth; pit++)
        {
            // Adjust pit index for opponent to mirror the display
            int adjustedPit = isCurrentPlayer ? pit : (boardWidth - 1 - pit);
            const int pitNumber = state[side][adjustedPit];

            if (pitNumber < 10)
            {
                pitNumbers[index++] = ' ';
                pitNumbers[index++] = '0' + pitNumber;
            }
            else
            {
                pitNumbers[index++] = '0' + pitNumber / 10;
                pitNumbers[index++] = '0' + pitNumber % 10;
            }
        }

        pitNumbers[index] = '\0'; // Null-terminate the string

        // Display the pit numbers with proper alignment
        centerPrint(lcd, pitNumbers, isCurrentPlayer ? 1 : 0, 1);
    }
}