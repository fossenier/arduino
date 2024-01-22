/*
Arduino project 03
*/
const int sensorPin = A0;
int baselineTemp = 20;

void setup()
{
    Serial.begin(9600);
    baselineTemp = adc_to_celsius(sensorPin);
}

void loop()
{
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