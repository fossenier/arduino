#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Goodnight,");
    lcd.setCursor(0, 1);
    lcd.print("Mom.");

    prevSwitchState = digitalRead(switchPin);
}

void loop()
{
    switchState = digitalRead(switchPin);
    if (switchState != prevSwitchState)
    {
        lcd.clear();
    }
    prevSwitchState = switchState;
}
