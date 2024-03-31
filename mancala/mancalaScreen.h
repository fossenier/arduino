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
}

void centerPrint(LiquidCrystal &display, const char *message, int row);
int stringLength(const char *str);

#endif