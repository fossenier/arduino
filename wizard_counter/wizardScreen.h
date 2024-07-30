#ifndef WIZARDSCREEN_H
#define WIZARDSCREEN_H

#include <LiquidCrystal.h>

namespace screen
{
    // LCD physical properties.
    constexpr int screenWidth{16};
    constexpr int screenHeight{2};

    // Breadboard wiring.
    constexpr int db7Pin{2};
    constexpr int db6Pin{3};
    constexpr int db5Pin{4};
    constexpr int db4Pin{5};
    constexpr int enPin{9};
    constexpr int rsPin{10};

    // Display configuration.
    constexpr long messageDelay{1000};

    // Messages.
    constexpr char alphabet[]{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    constexpr int alphabetLength{sizeof(alphabet) - 1};
    constexpr int segmentLength{3};
    constexpr char emptyMessage[]{" "};
    constexpr char welcomeMessageTop1[]{"Welcome to the"};
    constexpr char welcomeMessageBottom1[]{"Wizard counter!"};
    constexpr char welcomeMessageTop2[]{"Press select to"};
    constexpr char welcomeMessageBottom2[]{"begin."};
    constexpr char playerCountTop[]{"# of players?"};
    constexpr char playerNameTop[]{"Select a name."};
    constexpr char playerNameBottom[]{"is ready!"};
    constexpr char dealerMessageTop[]{"Select dealer."};
    constexpr char dealerMessageBottom[]{"is the dealer."};
    constexpr char playerBidTop[]{"--- please bid."};
    constexpr char playerBidSuccessTop[]{"You bid:"};
    constexpr char playerBidErrorTop[]{"Can't bid that."};
    constexpr char roundBidTop[]{"Round bid:"};
    constexpr char waitingMessage[]{"Waiting..."};
    constexpr char playerTricksTop[]{"---'s tricks:"};
    constexpr char playerTricksSuccessTop[]{"You scored:"};
    constexpr char gameOverTop[]{"Game over!"};
    constexpr char gameOverBottom[]{"--- wins!"};
}

void centerPrint(LiquidCrystal &display, const char *message, int row, int offset = 0);
void centerPrint(LiquidCrystal &display, int number, int row, int offset = 0);
int stringLength(const char *str);
int numberLength(int number);

#endif