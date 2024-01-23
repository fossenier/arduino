/*
Arduino project 03
*/
const int sensorPin = A0;
int baselineTemp = 20;

void setup()
{
    Serial.begin(9600);
    baselineTemp = adc_to_celsius(sensorPin);

    for (int pinNumber = 2; pinNumber < 5; pinNumber++)
    {
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, LOW);
    }
}

void loop()
{
    int activeLed = 0;

    // read the temperature
    int temperature = adc_to_celsius(sensorPin);

    // determine active LED (2 < +1C, 3 > +1C, 4 > +2C)
    if (temperature > baselineTemp + 2)
    {
        activeLed = 4;
    }
    else if (temperature > baselineTemp + 1)
    {
        activeLed = 3;
    }
    else
    {
        activeLed = 2;
    }

    // illuminate correct LED
    for (int pinNumber = 2; pinNumber < 5; pinNumber++)
    {
        if (pinNumber == activeLed)
        {
            digitalWrite(pinNumber, HIGH);
        }
        else
        {
            digitalWrite(pinNumber, LOW);
        }
    }
    delay(5);
}

int adc_to_celsius(int sensorPin)
{
    int sensorValue = analogRead(sensorPin);
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);

    float voltage = (sensorValue / 1024.0) * 5.0;
    Serial.print(", Volts: ");
    Serial.print(voltage);

    Serial.print(", degrees C: ");
    float temperature = (voltage - 0.5) * 100;
    Serial.println(temperature);

    return temperature;
}