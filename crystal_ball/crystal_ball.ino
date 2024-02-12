#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;

void setup()
{
    lcd.begin(16, 2);
    pinMode(switchPin, INPUT);
    lcd.print("Ask the :)");
    lcd.setCursor(0, 1);
    lcd.print("Crystal Ball!");
}

void loop()
{
    switchState = digitalRead(switchPin);
    if (switchState != prevSwitchState)
    {
        if (switchState == HIGH)
        {
            lcd.clear();
            reply = random(8);
            lcd.setCursor(0, 0);
            lcd.print("The Crystal Ball");
            lcd.setCursor(0, 1);
            lcd.print("says: ");
            switch (reply)
            {
            case 0:
                lcd.print("Yes");
                break;
            case 1:
                lcd.print("No");
                break;
            case 2:
                lcd.print("Ask again");
                break;
            case 3:
                lcd.print("Maybe");
                break;
            case 4:
                lcd.print("Not sure");
                break;
            case 5:
                lcd.print("Doubtful");
                break;
            case 6:
                lcd.print("Definitely");
                break;
            case 7:
                lcd.print("Absolutely");
                break;
            }
        }
    }
    prevSwitchState = switchState;
}
