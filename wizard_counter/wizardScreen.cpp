#include <LiquidCrystal.h>
#include "wizardScreen.h"

using namespace screen;

void centerPrint(LiquidCrystal &display, const char *message, int row, int offset)
{
    int centerOffset = (screenWidth - stringLength(message)) / 2;
    display.setCursor(centerOffset + offset, row);
    display.print(message);
}

void centerPrint(LiquidCrystal &display, int number, int row, int offset)
{
    char buffer[screenWidth + 1]; // Buffer to hold the string representation of the number
    itoa(number, buffer, 10);     // Convert the integer to a string
    int centerOffset = (screenWidth - numberLength(number)) / 2;
    display.setCursor(centerOffset + offset, row);
    display.print(buffer);
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

int numberLength(int number)
{
    int length{0};
    if (number == 0)
        return 1; // Special case for 0
    while (number != 0)
    {
        number /= 10;
        ++length;
    }
    return length;
}