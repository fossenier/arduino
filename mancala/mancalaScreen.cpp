#include <LiquidCrystal.h>

#include "mancalaScreen.h"

using namespace screen;

void centerPrint(LiquidCrystal &display, const char *message, int row)
{
    display.setCursor((screenWidth - stringLength(message)) / 2, row);
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