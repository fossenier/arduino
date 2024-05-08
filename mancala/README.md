# Mancalculations

#### Video Demo: [click](https://youtu.be/YSgb6GBIYNI)

#### Overview:

This is a C++ implementation of Mancala that runs on the popular Arduino microcontroller. You can find the Arduino documentation [here](https://docs.arduino.cc/). The game is controlled via Serial Bus, and is displayed on an lcd.

#### Mancala:

For those of you unfamiliar with Mancala, it is a children's game played on pits with stones. Each player has six pits in front of them, each with 4 stones. To their right is their store, where they collect points. Every turn, the player picks up all stones in a non-empty pit. They drop one stone in each pit going counterclockwise, including scoring a point in their store, and including each pit on the opponent's side (but not their store!) continuing until they are out of stones. If they land in their store, they can move again. If they land in an empty pit on their side where the opponent's pit immediately opposite has stones, they pick up that last stone and the opponent's stones opposite and collect them in their store. Once a player cannot take another move, both player's add up their store stones and any stones on their side of the board for the final tabulation.

### Implementation

Since this is run on an Arduino, the game plays out over an initial setup() function and then a loop() function. My implementation can be broken down as follows:

<ol type="I">
  <li>Initialize a MancalaBoard object.</li>
  <li>Initialize a LiquidCrystal display object.</li>
  <li>Get a valid move from the user.</li>
  <li>Perform moves (displaying steals, errors, and double turns).</li>
  <li>Determine when the game is over (display scores, offer to restart).</li>
</ol>

### I. Initialize a MancalaBoard object

The MancalaBoard object lives in mancalaBoard.cpp. The main publicly accessible members are

    bool justStole{};
    bool justLandedInStore{};

    MancalaBoard();
    Pair endGame();
    const int getPlayer() const;
    const int getGameScore(int player) const;
    const PlayerPitsType &getGameState() const;
    bool isGameOver();
    bool makeMove(int pit);

The first two tell the Arduino when to display informative messages, and are okay to be public since they cannot be modified at a time where internal functionality would require the use of the booleans. The latter member functions are important to get data about the game state to show the user, and then to critically make a move based on the user's input.

### II. Initialize a LiquidCrystal display object

The lcd is how the user sees the game. The way I use it is predominantly by using the function

`void centerPrint(LiquidCrystal &display, const char *message, int row, int offset = 0);`

as it allows me to print to the center of the screen. The messages to be printed are in the screen namespace apart of the mancalaScreen.h header. I did my best not to use String objects since dynamic memory allocation on an Arduino is expensive, though I may have inadvertently used it at some point. The game state is displayed by interpreting what is returned from getGameState() and getGameScore().

### III. Get a valid move from the user

The user is repeatedly prompted for an input (between 1 and 6) after which that pit is moved. If they enter anything invalid (or an empty pit, which makeMove makes note of) the user must try again.

### IV. Perform moves (displaying steals, errors, and double turns)

The game progresses like this, switching which side of the board faces the user at every given turn, making sure the right error messages are given at the right times, making sure the screen clears properly before displaying, and making sure that double turns are awarded.

### V.Determine when the game is over (display scores, offer to restart)

Eventually, the game comes to a close, and the user is notified. They have the option to restart and continue.

## Notes

The game tries to use character arrays whenever possible for displaying messages, and I did my best to factor out all instances of characters or c-style strings in the code itself, sticking them instead in mancalaScreen.h.

The board is implemented as two arrays of length 6, where index 0 for each array represents the user's imaginary left-hand side as they play from left to right. Whenever displaying pits, it must be made certain to display the bottom and top rows in opposing order.

The circuitry for the hardware is from the Starter Kit projects book, and simply removes the compatibility with the tilt switch as the game is always on.

Mancala is my passion project, anytime I learn anything new about coding, I want to use it to implement Mancala! Thank you CS50x for the awesome learning experience!

I learned C++ from [Learn C++](learncpp.com). Thanks Alex.
