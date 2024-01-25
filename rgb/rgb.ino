/*
Script to control an RGB LED using PWM
*/

const int redPin = 11;

void setup()
{
    pinMode(redPin, OUTPUT);
}

void loop()
{
    digitalWrite(redPin, HIGH);
    delay(1000);
    digitalWrite(redPin, LOW);
    delay(1000);
}
