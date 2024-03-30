void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial)
    {
        Serial.write(0x03);

        if (Serial.available() > 0)
        {
            Serial.print("Received data: ");
            Serial.write(0x03);

            while (Serial.available() > 0)
            {
                char data = Serial.read();
                Serial.print(data);
            }

            Serial.println();
        }
    }
}
