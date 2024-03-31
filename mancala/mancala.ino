#include "mancalaBoard.cpp"
#include <LiquidCrystal.h>

/*
My implementation of Mancala.
This second version is implemented fully using Arduino's LCD screen.
*/

MancalaBoard game; // the board state

void setup()
{
}

void loop()
{
}

void setupSerialVersion()
{
    Serial.begin(9600); // start serial communication at 9600 baud rate
    while (!Serial)
    {
        ; // wait for the serial port to connect. needed for native USB port only
    }
    Serial.println("Welcome to Mancala.");
}

void loopSerialVersion()
{
    if (Serial.available() > 0)
    {
        // read the incoming byte:
        String input = Serial.readStringUntil('\n');
        input.trim(); // trim any whitespace

        if (input == "state")
        {
            // if the player requezsts the game state, print the board
            const auto &state = game.getGameState();
            // opponent's side (reversed for display purposes)
            const int opponent{1 - game.getPlayer()};
            Serial.print(game.getGameScore(opponent));
            Serial.println();
            Serial.print("Opponent (P" + String(opponent + 1) + "): ");
            for (int pit{boardWidth - 1}; pit >= 0; --pit)
            {
                Serial.print(state[opponent][pit]);
                Serial.print(" ");
            }
            Serial.println();
            // player's side
            const int player{game.getPlayer()};
            Serial.print("You (P" + String(player + 1) + "):      ");
            for (int pit{0}; pit < boardWidth; ++pit)
            {
                Serial.print(state[player][pit]);
                Serial.print(" ");
            }
            Serial.println();
            Serial.print(game.getGameScore(player));
        }
        else
        {
            // assuming the input is a number representing the pit to move
            int pit{input.toInt() - 1}; // make the pit 0-indexed
            if (pit >= 0 && pit < boardWidth)
            {
                Serial.print("Player ");
                Serial.print(game.getPlayer() + 1);
                Serial.println(" made a move.");
                game.makeMove(pit);
                if (game.isGameOver())
                {
                    Serial.println("Game over.");
                }
            }
            else
            {
                Serial.println("Invalid pit. Please enter a number between 1 and 6.");
            }
        }
    }
}