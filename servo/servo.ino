/*
 */

int potentiometer_pin = A0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int potentiometer_value = analogRead(potentiometer_pin);
    Serial.println(potentiometer_value);
    delay(100);
}
