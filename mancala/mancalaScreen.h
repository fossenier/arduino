#ifndef MANCALASCREEN_H
#define MANCALASCREEN_H

#include <LiquidCrystal.h>

namespace screen
{
    constexpr int screenWidth{16};
    constexpr int screenHeight{2};

    constexpr int db7Pin{2};
    constexpr int db6Pin{3};
    constexpr int db5Pin{4};
    constexpr int db4Pin{5};
    constexpr int enPin{11};
    constexpr int rsPin{12};

    constexpr char blank{' '};
    constexpr char welcomeMessageTop[]{"Send any key"};
    constexpr char welcomeMessageBottom[]{"to begin Mancala"};
    constexpr char inputErrorTop[]{"Invalid input,"};
    constexpr char inputErrorBottom[]{"please enter 1-6."};
    constexpr char gameOverMessage[]{"Game over!"};
    constexpr char standingsMessageTie[]{"It's a tie!"};

    constexpr int boardOffset{1};
}

void centerPrint(LiquidCrystal &display, const char *message, int row, int offset = 0);
int stringLength(const char *str);

#endif