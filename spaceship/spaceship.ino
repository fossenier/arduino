/*
This program is for the spaceship project.
It will control LEDs using a button.
*/

int buttonPressed = 0;
const int buttonPin = 2;
const int redPin1 = 3;
const int redPin2 = 4;
const int greenPin = 5;

void setup()
{
    // initialize the button and LEDs
    pinMode(buttonPin, INPUT);
    pinMode(redPin1, OUTPUT);
    pinMode(redPin2, OUTPUT);
    pinMode(greenPin, OUTPUT);

    // fun starting LED pattern
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(redPin1, HIGH);
        digitalWrite(redPin2, LOW);
        digitalWrite(greenPin, LOW);
        delay(250);
        digitalWrite(redPin1, LOW);
        digitalWrite(redPin2, HIGH);
        delay(250);
        digitalWrite(redPin2, LOW);
        digitalWrite(greenPin, HIGH);
        delay(250);
    }
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(redPin1, HIGH);
        digitalWrite(redPin2, HIGH);
        digitalWrite(greenPin, HIGH);
        delay(500);
        digitalWrite(redPin1, LOW);
        digitalWrite(redPin2, LOW);
        digitalWrite(greenPin, LOW);
        delay(500);
    }
}

void loop()
{
    buttonPressed = digitalRead(buttonPin);

    if (buttonPressed == LOW)
    {
        // the button is not pressed
        digitalWrite(redPin1, LOW);
        digitalWrite(redPin2, LOW);
        digitalWrite(greenPin, HIGH);
    }
    else
    {
        // the button is pressed
        digitalWrite(redPin1, HIGH);
        digitalWrite(redPin2, LOW);
        digitalWrite(greenPin, LOW);

        delay(250);

        digitalWrite(redPin1, LOW);
        digitalWrite(redPin2, HIGH);

        delay(250);
    }
}