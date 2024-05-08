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
    // display the game state
    displayGameScore(game.getGameScore(0), game.getGameScore(1), game.getPlayer());
    displayGameState(game.getGameState(), game.getPlayer());

    // handle the player's move
    handlePlayerMove();

    // handle win condition
    if (game.isGameOver())
    {
        handleGameEnd();
    }

    // steal or land in store message
    if (game.justStole)
    {
        lcd.clear();
        centerPrint(lcd, stealMessage, 0);
        delay(1000);
        lcd.clear();
    }
    else if (game.justLandedInStore)
    {
        lcd.clear();
        centerPrint(lcd, storeMessage, 0);
        delay(1000);
        lcd.clear();
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
            displayScore[0] = blank;
            displayScore[1] = zero + score;
        }
        else
        {
            displayScore[0] = zero + score / 10;
            displayScore[1] = zero + score % 10;
        }

        displayScore[2] = '\0'; // null-terminate the string

        // display on the bottom (0) if it's the active player's side, otherwise display on the top (1)
        const int row = (i == activePlayer) ? 1 : 0;
        lcd.setCursor(0, row);
        lcd.print(displayScore);
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
                pitNumbers[stringIndex++] = blank;
                pitNumbers[stringIndex++] = zero + pitNumber;
            }
            else
            {
                pitNumbers[stringIndex++] = zero + pitNumber / 10;
                pitNumbers[stringIndex++] = zero + pitNumber % 10;
            }
        }

        pitNumbers[stringIndex] = '\0'; // null-terminate the string

        // display on the bottom (0) if it's the active player's side, otherwise display on the top (1)
        centerPrint(lcd, pitNumbers, isActivePlayer ? 1 : 0, boardOffset);
    }
}

void handleGameEnd()
{
    Pair matchStandings = game.endGame();
    char winner = matchStandings.first > matchStandings.second ? '1' : '2';

    lcd.clear();
    centerPrint(lcd, gameOverMessage, 0); // tell user the game is over

    delay(2000); // wait for 2 seconds

    char standingsMessageTop[screenWidth + 1];
    char standingsMessageBottom[screenWidth + 1];
    sprintf(standingsMessageTop, "Player %c wins!", winner);
    sprintf(standingsMessageBottom, "Score: %d-%d", matchStandings.first, matchStandings.second);

    if (matchStandings.first == matchStandings.second)
    {
        sprintf(standingsMessageTop, standingsMessageTie);
    }

    unsigned long lastUpdateTime = millis();
    const long updateInterval = 4000;
    bool showingStandings = true;

    while (true)
    {
        if (Serial.available() > 0)
        {
            while (Serial.available() > 0)
            {
                Serial.read(); // clear the Serial buffer
            }
            game = MancalaBoard(); // reset the game
            lcd.clear();           // reset the LCD
            break;                 // exit the current loop iteration
        }

        unsigned long currentMillis = millis();

        // check if it's time to update the display
        if (currentMillis - lastUpdateTime >= updateInterval)
        {
            lcd.clear();
            if (showingStandings)
            {
                centerPrint(lcd, standingsMessageTop, 0);
                centerPrint(lcd, standingsMessageBottom, 1);
            }
            else
            {
                centerPrint(lcd, welcomeMessageTop, 0);
                centerPrint(lcd, welcomeMessageBottom, 1);
            }
            showingStandings = !showingStandings; // toggle the state
            lastUpdateTime = currentMillis;       // reset the timer
        }
    }
}

void handlePlayerMove()
{
    while (Serial.available() < 1)
        ; // wait for input

    char input = Serial.read();
    while (input < '1' || input > '6')
    {
        // clear serial
        while (Serial.available() > 0)
            Serial.read();
        Serial.print(inputErrorTop);
        Serial.print(blank);
        Serial.println(inputErrorBottom);

        // clear lcd
        lcd.clear();
        centerPrint(lcd, inputErrorTop, 0);
        centerPrint(lcd, inputErrorBottom, 1);

        while (Serial.available() < 1)
            ; // wait for input
        input = Serial.read();
    }

    // the user picked an empty pit
    if (!game.makeMove(input - zero - 1))
    {
        Serial.print(inputErrorTop);
        Serial.print(blank);
        Serial.println(pitErrorBottom);

        lcd.clear();
        centerPrint(lcd, inputErrorTop, 0);
        centerPrint(lcd, pitErrorBottom, 1);

        delay(2000);

        lcd.clear();

        // display the board for another choice
        displayGameScore(game.getGameScore(0), game.getGameScore(1), game.getPlayer());
        displayGameState(game.getGameState(), game.getPlayer());
        // cause another move
        handlePlayerMove();
    }

    lcd.clear();
}