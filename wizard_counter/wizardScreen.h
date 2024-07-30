#ifndef WIZARDSCREEN_H
#define WIZARDSCREEN_H

#include <LiquidCrystal.h>

namespace screen
{
    constexpr int screenWidth{16};
    constexpr int screenHeight{2};

    constexpr int db7Pin{2};
    constexpr int db6Pin{3};
    constexpr int db5Pin{4};
    constexpr int db4Pin{5};
    constexpr int enPin{9};
    constexpr int rsPin{10};

    constexpr long messageDelay{3000};

    constexpr char welcomeMessageTop1[]{"Welcome to the"};
    constexpr char welcomeMessageBottom1[]{"Wizard counter!"};
    constexpr char welcomeMessageTop2[]{"Press select to"};
    constexpr char welcomeMessageBottom2[]{"begin."};
    constexpr char playerCountTop[]{"# of players?"};
    constexpr char alphabet[]{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    constexpr int alphabetLength{sizeof(alphabet) - 1};
    constexpr char playerNameTop[]{"Select a name."};
    constexpr char playerNameBottom[]{"is ready!"};
    constexpr char dealerMessageTop[]{"Select dealer."};

    constexpr char blank{' '};
    constexpr char zero{'0'};
    constexpr char inputErrorTop[]{"Invalid input,"};
    constexpr char inputErrorBottom[]{"please enter 1-6."};
    constexpr char gameOverMessage[]{"Game over!"};
    constexpr char standingsMessageTie[]{"It's a tie!"};
    constexpr char pitErrorBottom[]{"can't be empty."};
    constexpr char stealMessage[]{"Steal!"};
    constexpr char storeMessage[]{"Move again!"};

    constexpr int boardOffset{1};
}

void centerPrint(LiquidCrystal &display, const char *message, int row, int offset = 0);
void centerPrint(LiquidCrystal &display, int number, int row, int offset = 0);
int stringLength(const char *str);
int numberLength(int number);

#endif