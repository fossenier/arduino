/*
Script to control an RGB LED using PWM
*/

const int redSensorPin = A0;
const int greenSensorPin = A2;
const int blueSensorPin = A1;

const int redLedPin = 11;
const int blueLedPin = 10;
const int greenLedPin = 9;

int redSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    redSensorValue = analogRead(redSensorPin);
    delay(5);
    greenSensorValue = analogRead(greenSensorPin);
    delay(5);
    blueSensorValue = analogRead(blueSensorPin);
    delay(5);

    String sensorData = "Red: " + String(redSensorValue) + ", Green: " + String(greenSensorValue) + ", Blue: " + String(blueSensorValue);
    Serial.println(sensorData);

    int redLedValue = redSensorValue / 4;
    int greenLedValue = greenSensorValue / 4;
    int blueLedValue = blueSensorValue / 4;

    analogWrite(redLedPin, redLedValue);
    analogWrite(greenLedPin, greenLedValue);
    analogWrite(blueLedPin, blueLedValue);

    String ledData = "Red: " + String(redLedValue) + ", Green: " + String(greenLedValue) + ", Blue: " + String(blueLedValue);
    Serial.println(ledData);
}
