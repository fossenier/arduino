#include <LiquidCrystal.h>

#include "mancalaScreen.h"

using namespace screen;

void centerPrint(LiquidCrystal &display, const char *message, int row, int offset = 0)
{
    int centerOffset = (screenWidth - stringLength(message)) / 2;
    display.setCursor(centerOffset + offset, row);
    display.print(message);
}

int stringLength(const char *str)
{
    int length{0};
    while (*str++)
    {
        ++length;
    }
    return length;
}