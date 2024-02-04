/*
Takes in values from a phototransistor and plays a frequency to a piezo based on the light range.
*/

// delay tracking
unsigned long previousMillis = 0;
const unsigned long interval = 100;

// input output
const int photoPin = A0;
const int piezoPin = 8;
const int ledPin = 13;

int sensorLow = 1023;
int sensorHigh = 0;

void setup()
{
    // turn on LED to signal start of calibration
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    while (millis() < 5000)
    {
        // calibrate sensor to the ambient lighting
        int sensorValue = analogRead(photoPin);
        if (sensorValue > sensorHigh)
        {
            sensorHigh = sensorValue;
        }
        if (sensorValue < sensorLow)
        {
            sensorLow = sensorValue;
        }
    }

    // turn off LED to signal end of calibration
    digitalWrite(ledPin, LOW);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        // play the frequency to the piezo
        previousMillis = currentMillis;
        int frequency = piezoFrequency();
        tone(piezoPin, frequency, 10);
    }
}

int piezoFrequency()
{
    // read the phototransistor
    int sensorValue = analogRead(photoPin);

    // map the sensor value to a frequency
    int frequency = map(sensorValue, sensorLow, sensorHigh, 50, 4000);
    return frequency;
}