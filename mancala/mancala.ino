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
    // update the game state
    displayGameScore(game.getGameScore(0), game.getGameScore(1), game.getPlayer());
    displayGameState(game.getGameState(), game.getPlayer());

    while (Serial.available() < 1)
        ; // wait for input

    char input = Serial.read();
    if (input >= '1' && input <= '6')
        game.makeMove(input - '0' - 1);
    else
    {
        // clear searial
        while (Serial.available() > 0)
            Serial.read();
        Serial.print(inputErrorTop);
        Serial.print(blank);
        Serial.println(inputErrorBottom);

        // clear lcd
        lcd.clear();
        centerPrint(lcd, inputErrorTop, 0);
        centerPrint(lcd, inputErrorBottom, 1);
    }
}

void displayGameScore(const int score0, const int score1, int activePlayer)
{
    for (int i{0}; i < 2; i++)
    {
        char displayScore[3]{};
        const int score{(i == 0) ? score0 : score1};
        if (score < 10)
        {
            displayScore[0] = ' ';
            displayScore[1] = '0' + score;
        }
        else
        {
            displayScore[0] = '0' + score / 10;
            displayScore[1] = '0' + score % 10;
        }

        displayScore[2] = '\0'; // null-terminate the string

        // display on the bottom (0) if it's the active player's side, otherwise display on the top (1)
        const int row = (i == activePlayer) ? 1 : 0;
    }
}

void displayGameState(const PlayerPitsType &state, int activePlayer)
{
    // display the active player's side and the opponent's side
    for (int sideOfBoard = 0; sideOfBoard <= 1; ++sideOfBoard)
    {
        const bool isActivePlayer = (sideOfBoard == activePlayer);

        // accumulate the pit numbers into a string
        char pitNumbers[boardWidth * 2 + 1]{};
        int stringIndex = 0;

        for (int pit = 0; pit < boardWidth; pit++)
        {
            // grab pits left to right for the active player and right to left for the opponent
            const int adjustedPit = isActivePlayer ? pit : (boardWidth - 1 - pit);
            const int pitNumber = state[sideOfBoard][adjustedPit];

            if (pitNumber < 10)
            {
                pitNumbers[stringIndex++] = ' ';
                pitNumbers[stringIndex++] = '0' + pitNumber;
            }
            else
            {
                pitNumbers[stringIndex++] = '0' + pitNumber / 10;
                pitNumbers[stringIndex++] = '0' + pitNumber % 10;
            }
        }

        pitNumbers[stringIndex] = '\0'; // null-terminate the string

        // display on the bottom (0) if it's the active player's side, otherwise display on the top (1)
        centerPrint(lcd, pitNumbers, isActivePlayer ? 1 : 0, boardOffset);
    }
}