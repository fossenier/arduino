int buttonPressed = 0;

void setup()
{
    pinMode(4, OUTPUT);
    pinMode(2, INPUT);

    digitalWrite(4, HIGH);
    delay(2000);
}

void loop()
{
    buttonPressed = digitalRead(2);

    if (buttonPressed == LOW)
    {
        digitalWrite(4, LOW);
    }
    else
    {
        digitalWrite(4, HIGH);
    }
}
