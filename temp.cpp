void setupArduinoVersion()
{
    lcd.clear();
    Serial.begin(9600); // start serial communication at 9600 baud rate
    while (!Serial)
    {
        ; // wait for the serial port to connect. needed for native USB port only
    }
    Serial.println("Welcome to Mancala.");

    lcd.begin(screenWidth, screenHeight);
    centerPrint(lcd, "Welcome to", 0);
    centerPrint(lcd, "Mancala.", 1);
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
            // if the player requezsts the game state, print the game
            const PlayerPitsType &state = game.getGameState();
            // opponent's side (reversed for display purposes)
            const int opponent{1 - game.getPlayer()};
            Serial.print(game.getGameScore(opponent));
            lcd.setCursor(0, 0);
            lcd.print(game.getGameScore(opponent));
            Serial.println();
            Serial.print("Opponent (P" + String(opponent + 1) + "): ");
            // char pitNumbers[boardWidth + 1]; // char array to store pit numbers
            // int index = 0;                   // index to keep track of the position in the char array
            displayGameState(state, game.getPlayer());

            for (int pit{boardWidth - 1}; pit >= 0; --pit)
            {
                Serial.print(state[opponent][pit]);
                Serial.print(" ");
                // pitNumbers[index++] = state[opponent][pit] + '0'; // convert pit number to char and store in the array                // add a space after each pit number
            }
            // pitNumbers[index] = '\0'; // add null terminator at the end of the char array
            // centerPrint(lcd, pitNumbers, 0);
            Serial.println();
            // player's side
            const int player{game.getPlayer()};
            Serial.print("You (P" + String(player + 1) + "):      ");
            for (int pit{0}; pit < boardWidth; ++pit)
            {
                Serial.print(state[player][pit]);
                Serial.print(" ");
            }
            lcd.setCursor(0, 1);
            lcd.print(game.getGameScore(player));
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