int switchState = 0;

void setup()
{
    // green led
    pinMode(3, OUTPUT);
    // red led
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    // button
    pinMode(2, INPUT);
}

void loop()
{
    switchState = digitalRead(2);

    if (switchState == LOW)
    {
        // the button is not pressed
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
    }
    else
    {
        // the button is pressed
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);

        delay(250);
        digitalWrite(4, HIGH);
        digitalWrite(5, LOW);
        delay(250);
    }
}